// MIDIA Cloud companion: original bytes and existing MERP sessions, standard library only.
package main

import (
	"bytes"
	"crypto/rand"
	"crypto/sha256"
	"encoding/hex"
	"encoding/json"
	"encoding/xml"
	"errors"
	"fmt"
	"io"
	"log"
	"net"
	"net/http"
	"net/url"
	"os"
	"os/exec"
	"path/filepath"
	"runtime"
	"strings"
	"sync"
	"time"
)

const origin = "https://gestion.midiashops.com"
const api = "https://bridge.momoda.es/v4/office/hojas/cloud"
const address = "127.0.0.1:43129"
const maxFile = 25 * 1024 * 1024

type File struct {
	ID         string `json:"id"`
	Name       string `json:"name"`
	Version    int    `json:"version"`
	Hash       string `json:"sha256"`
	Permission string `json:"permission"`
	Owner      string `json:"owner_id"`
}
type Tracked struct {
	Path     string `json:"path"`
	File     File   `json:"file"`
	Hash     string `json:"saved_hash"`
	Key      string `json:"client_key"`
	Error    string `json:"error,omitempty"`
	Size     int64  `json:"saved_size,omitempty"`
	Modified int64  `json:"saved_modified,omitempty"`
}
type State struct {
	User   string                         `json:"user_id"`
	Name   string                         `json:"user_name"`
	Token  string                         `json:"session_token"`
	Device string                         `json:"device_id"`
	Tracks map[string]map[string]*Tracked `json:"tracks"`
}
type Agent struct {
	mu                                 sync.Mutex
	state                              State
	dir, config, nonce, editor, recent string
	seen                               map[string]string
	pending                            map[string]bool
	lastTop, lastError                 string
	client                             *http.Client
	opener                             func(string) error
	base                               string
	blocked                            bool
}

func randomID() string {
	b := make([]byte, 32)
	if _, e := rand.Read(b); e != nil {
		panic(e)
	}
	return hex.EncodeToString(b)
}
func hash(b []byte) string { h := sha256.Sum256(b); return hex.EncodeToString(h[:]) }
func dataDir() string {
	h, _ := os.UserHomeDir()
	if runtime.GOOS == "windows" {
		return filepath.Join(os.Getenv("LOCALAPPDATA"), "MIDIA", "OfficeCloud")
	}
	return filepath.Join(h, "Library", "Application Support", "MIDIA", "OfficeCloud")
}
func paths() (string, string) {
	exe, _ := os.Executable()
	dir := filepath.Dir(exe)
	h, _ := os.UserHomeDir()
	if runtime.GOOS == "windows" {
		return filepath.Join(dir, "DesktopEditors.exe"), filepath.Join(os.Getenv("LOCALAPPDATA"), "ONLYOFFICE", "DesktopEditors", "data", "recents.xml")
	}
	return filepath.Join(dir, "ONLYOFFICE WPS Native"), filepath.Join(h, "Library", "Application Support", "asc.onlyoffice.ONLYOFFICE.wps", "recents.xml")
}
func newAgent(dir, editor, recent string) (*Agent, error) {
	if e := os.MkdirAll(dir, 0700); e != nil {
		return nil, e
	}
	a := &Agent{dir: dir, config: filepath.Join(dir, "state.json"), editor: editor, recent: recent, nonce: randomID(), seen: map[string]string{}, pending: map[string]bool{}, base: api,
		client: &http.Client{Timeout: 45 * time.Second, CheckRedirect: func(_ *http.Request, _ []*http.Request) error { return errors.New("redirección rechazada") }}}
	b, e := os.ReadFile(a.config)
	if e == nil {
		if e = json.Unmarshal(b, &a.state); e != nil {
			return nil, errors.New("Estado de sincronización ilegible; se conserva para recuperar los archivos.")
		}
	} else if !os.IsNotExist(e) {
		return nil, e
	}
	if a.state.Device == "" {
		a.state.Device = randomID()
	}
	if a.state.Tracks == nil {
		a.state.Tracks = map[string]map[string]*Tracked{}
	}
	a.opener = func(p string) error {
		if _, e := os.Stat(a.editor); e != nil {
			return errors.New("No se encuentra ONLYOFFICE WPS.")
		}
		cmd := exec.Command(a.editor, p)
		cmd.Stdout = io.Discard
		cmd.Stderr = io.Discard
		if e := cmd.Start(); e != nil {
			return e
		}
		go cmd.Wait()
		return nil
	}
	a.readRecents(false)
	return a, a.save()
}
func (a *Agent) save() error {
	b, e := json.MarshalIndent(a.state, "", "  ")
	if e != nil {
		return e
	}
	tmp := a.config + ".tmp"
	if e = os.WriteFile(tmp, b, 0600); e != nil {
		return e
	}
	return replaceFile(tmp, a.config)
}
func allowedPath(p string) bool {
	switch strings.ToLower(filepath.Ext(p)) {
	case ".doc", ".docx", ".xls", ".xlsx", ".ppt", ".pptx", ".odt", ".ods", ".pdf", ".csv", ".tsv", ".txt", ".rtf":
		return true
	}
	return false
}
func (a *Agent) track(p string) {
	p = filepath.Clean(p)
	if !filepath.IsAbs(p) || !allowedPath(p) || a.state.User == "" {
		return
	}
	if strings.HasPrefix(p, filepath.Join(a.dir, "files")+string(os.PathSeparator)) {
		return
	}
	if a.state.Tracks[a.state.User] == nil {
		a.state.Tracks[a.state.User] = map[string]*Tracked{}
	}
	if a.state.Tracks[a.state.User][p] == nil {
		a.state.Tracks[a.state.User][p] = &Tracked{Path: p, Key: hash([]byte(a.state.Device + "\x00" + p))}
	}
}
func (a *Agent) readRecents(capture bool) {
	b, e := os.ReadFile(a.recent)
	if e != nil {
		return
	}
	var r struct {
		Files []struct {
			Path string `xml:"path,attr"`
			Date string `xml:"date,attr"`
			URL  string `xml:"url,attr"`
		} `xml:"file"`
	}
	if xml.Unmarshal(b, &r) != nil {
		return
	} // Native editor briefly replaces this XML while writing.
	current := map[string]string{}
	top := ""
	for i, f := range r.Files {
		if f.URL != "" || f.Path == "" || !allowedPath(f.Path) {
			continue
		}
		if strings.HasPrefix(f.Path, "file://") {
			u, e := url.Parse(f.Path)
			if e != nil {
				continue
			}
			f.Path = u.Path
		}
		current[f.Path] = f.Date
		if i == 0 {
			top = f.Path
		}
		if capture && (a.seen[f.Path] != f.Date || (i == 0 && top != a.lastTop)) {
			a.pending[f.Path] = true
		}
	}
	a.seen = current
	a.lastTop = top
	if capture && a.state.User != "" {
		for p := range a.pending {
			a.track(p)
			delete(a.pending, p)
		}
	}
}
func (a *Agent) request(method, path string, body []byte, kind string) ([]byte, int, error) {
	req, e := http.NewRequest(method, a.base+path, bytes.NewReader(body))
	if e != nil {
		return nil, 0, e
	}
	req.Header.Set("Authorization", "Bearer "+a.state.Token)
	if kind != "" {
		req.Header.Set("Content-Type", kind)
	}
	resp, e := a.client.Do(req)
	if e != nil {
		a.blocked = true
		return nil, 0, errors.New("Sin conexión. Pendiente de guardar en MIDIA Cloud.")
	}
	defer resp.Body.Close()
	b, e := io.ReadAll(io.LimitReader(resp.Body, maxFile+1024*1024))
	if e != nil {
		return nil, resp.StatusCode, e
	}
	if resp.StatusCode < 200 || resp.StatusCode >= 300 {
		if resp.StatusCode == 401 || resp.StatusCode >= 500 {
			a.blocked = true
		}
		var m struct {
			Error string `json:"error"`
		}
		json.Unmarshal(b, &m)
		if resp.StatusCode == 401 {
			m.Error = "Inicia sesión de nuevo en MIDIA Cloud. Los cambios locales se conservan."
		}
		if m.Error == "" {
			m.Error = fmt.Sprintf("Cloud HTTP %d; copia local conservada", resp.StatusCode)
		}
		return b, resp.StatusCode, errors.New(m.Error)
	}
	return b, resp.StatusCode, nil
}
func stableFile(p string) ([]byte, error) {
	before, e := os.Stat(p)
	if e != nil {
		return nil, e
	}
	if !before.Mode().IsRegular() || before.Size() > maxFile || before.Size() == 0 {
		return nil, errors.New("El archivo debe ocupar entre 1 byte y 25 MB.")
	}
	if time.Since(before.ModTime()) < 1500*time.Millisecond {
		return nil, errors.New("Esperando a que Office termine de guardar.")
	}
	b, e := os.ReadFile(p)
	if e != nil {
		return nil, e
	}
	after, e := os.Stat(p)
	if e != nil || before.Size() != after.Size() || !before.ModTime().Equal(after.ModTime()) {
		return nil, errors.New("Esperando a que Office termine de guardar.")
	}
	return b, nil
}
func (a *Agent) syncOne(t *Tracked) {
	observed, _ := os.Stat(t.Path)
	if observed != nil && t.Hash != "" && observed.Size() == t.Size && observed.ModTime().UnixNano() == t.Modified {
		return
	}
	b, e := stableFile(t.Path)
	if e != nil {
		t.Error = "No se pudo leer la copia local: " + e.Error()
		return
	}
	h := hash(b)
	if h == t.Hash {
		t.Error = ""
		return
	}
	if t.File.Permission == "read" {
		t.Error = "Solo lectura: los cambios de esta copia no se subirán."
		return
	}
	path := "/files?name=" + url.QueryEscape(filepath.Base(t.Path)) + "&key=" + url.QueryEscape(t.Key)
	method := "POST"
	if t.File.ID != "" {
		method = "PUT"
		path = "/files/" + url.PathEscape(t.File.ID) + "/content?version=" + fmt.Sprint(t.File.Version)
	}
	response, _, e := a.request(method, path, b, "application/octet-stream")
	if e != nil {
		t.Error = e.Error()
		return
	}
	var result struct {
		File File `json:"file"`
	}
	if json.Unmarshal(response, &result) != nil || result.File.ID == "" {
		t.Error = "El cloud no confirmó el archivo. Se conserva la copia local."
		return
	}
	if result.File.Hash != h {
		t.Error = "La confirmación del cloud no coincide con el archivo local."
		return
	}
	t.File = result.File
	t.Hash = h
	t.Error = ""
	t.Size = int64(len(b))
	t.Modified = 0
	if observed != nil {
		if info, e := os.Stat(t.Path); e == nil && info.Size() == observed.Size() && info.ModTime().Equal(observed.ModTime()) {
			t.Modified = observed.ModTime().UnixNano()
		}
	}
}
func (a *Agent) sync() {
	a.mu.Lock()
	defer a.mu.Unlock()
	a.readRecents(true)
	if a.state.Token == "" || a.state.User == "" {
		return
	}
	a.blocked = false
	for _, t := range a.state.Tracks[a.state.User] {
		a.syncOne(t)
		if a.blocked {
			break
		}
	}
	if e := a.save(); e != nil {
		a.lastError = "No se pudo guardar el estado local; tus documentos se conservan."
	}
}
func (a *Agent) open(id string) (string, error) {
	if a.state.Token == "" {
		return "", errors.New("Inicia sesión en MIDIA Cloud.")
	}
	list, _, e := a.request("GET", "/files", nil, "")
	if e != nil {
		return "", e
	}
	var result struct {
		Files []File `json:"files"`
	}
	if json.Unmarshal(list, &result) != nil {
		return "", errors.New("Lista del cloud ilegible.")
	}
	var f File
	for _, x := range result.Files {
		if x.ID == id {
			f = x
			break
		}
	}
	if f.ID == "" {
		return "", errors.New("Archivo no disponible o permiso retirado.")
	}
	if filepath.Base(f.Name) != f.Name || strings.ContainsAny(f.Name, "\\/\x00") || !allowedPath(f.Name) {
		return "", errors.New("Nombre del archivo no válido.")
	}
	dir := filepath.Join(a.dir, "files", a.state.User, f.ID)
	p := filepath.Join(dir, "MIDIA-"+f.Name)
	if a.state.Tracks[a.state.User] == nil {
		a.state.Tracks[a.state.User] = map[string]*Tracked{}
	}
	old := a.state.Tracks[a.state.User][p]
	if old != nil {
		b, e := os.ReadFile(p)
		if e == nil && hash(b) != old.Hash {
			return "", errors.New("Esta copia tiene cambios locales pendientes. Consérvalos antes de volver a abrir el original.")
		}
		if e == nil && hash(b) == f.Hash {
			old.File = f
			if f.Permission == "read" {
				os.Chmod(p, 0400)
			} else {
				os.Chmod(p, 0600)
			}
			old.Error = ""
			a.save()
			return p, a.opener(p)
		}
	}
	b, _, e := a.request("GET", "/files/"+url.PathEscape(f.ID)+"/content?version="+fmt.Sprint(f.Version), nil, "")
	if e != nil {
		return "", e
	}
	if hash(b) != f.Hash {
		return "", errors.New("El archivo cambió durante la descarga. Actualiza y vuelve a abrirlo.")
	}
	if e = os.MkdirAll(dir, 0700); e != nil {
		return "", e
	}
	if e = os.WriteFile(p+".download", b, 0600); e != nil {
		return "", e
	}
	os.Chmod(p, 0600)
	if e = replaceFile(p+".download", p); e != nil {
		return "", errors.New("Cierra esta copia en Office antes de actualizarla.")
	}
	if f.Permission == "read" {
		os.Chmod(p, 0400)
	}
	a.state.Tracks[a.state.User][p] = &Tracked{Path: p, File: f, Hash: f.Hash}
	if e = a.save(); e != nil {
		return "", e
	}
	return p, a.opener(p)
}
func jsonResponse(w http.ResponseWriter, status int, v any) {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(status)
	json.NewEncoder(w).Encode(v)
}
func (a *Agent) handler(w http.ResponseWriter, r *http.Request) {
	// DNS rebinding / CSRF guard. This API never accepts arbitrary filesystem paths.
	if r.Host != address || r.Header.Get("Origin") != origin {
		http.Error(w, "Forbidden", 403)
		return
	}
	w.Header().Set("Access-Control-Allow-Origin", origin)
	w.Header().Set("Vary", "Origin")
	w.Header().Set("Cache-Control", "no-store")
	w.Header().Set("Access-Control-Allow-Private-Network", "true")
	w.Header().Set("Access-Control-Allow-Headers", "Content-Type, X-MIDIA-Nonce")
	w.Header().Set("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
	if r.Method == "OPTIONS" {
		w.WriteHeader(204)
		return
	}
	a.mu.Lock()
	defer a.mu.Unlock()
	if r.URL.Path == "/status" && r.Method == "GET" {
		issues := []map[string]string{}
		count := 0
		for _, t := range a.state.Tracks[a.state.User] {
			count++
			if t.Error != "" {
				issues = append(issues, map[string]string{"name": filepath.Base(t.Path), "error": t.Error})
			}
		}
		jsonResponse(w, 200, map[string]any{"ok": true, "nonce": a.nonce, "user_id": a.state.User, "name": a.state.Name, "connected": a.state.Token != "", "files": count, "issues": issues, "error": a.lastError})
		return
	}
	if r.Method != "POST" || r.Header.Get("X-MIDIA-Nonce") != a.nonce {
		http.Error(w, "Forbidden", 403)
		return
	}
	var input struct {
		Token string `json:"token"`
		ID    string `json:"id"`
	}
	if json.NewDecoder(io.LimitReader(r.Body, 8192)).Decode(&input) != nil {
		http.Error(w, "Bad request", 400)
		return
	}
	switch r.URL.Path {
	case "/connect":
		if len(input.Token) < 32 || len(input.Token) > 1024 {
			http.Error(w, "Bad token", 400)
			return
		}
		previous := a.state.Token
		a.state.Token = input.Token
		b, _, e := a.request("GET", "/status", nil, "")
		if e != nil {
			a.state.Token = previous
			jsonResponse(w, 401, map[string]string{"error": e.Error()})
			return
		}
		var v struct {
			User struct {
				ID   string `json:"id"`
				Name string `json:"name"`
			} `json:"user"`
		}
		if json.Unmarshal(b, &v) != nil || v.User.ID == "" {
			a.state.Token = previous
			http.Error(w, "Invalid profile", 502)
			return
		}
		if a.state.User != "" && a.state.User != v.User.ID {
			a.pending = map[string]bool{}
			a.readRecents(false)
		}
		a.state.User = v.User.ID
		a.state.Name = v.User.Name
		for p := range a.pending {
			a.track(p)
			delete(a.pending, p)
		}
		if e = a.save(); e != nil {
			jsonResponse(w, 500, map[string]string{"error": "No se pudo guardar la conexión local."})
			return
		}
		jsonResponse(w, 200, map[string]any{"ok": true, "user_id": a.state.User})
	case "/disconnect":
		a.state.Token = ""
		a.state.User = ""
		a.pending = map[string]bool{}
		a.readRecents(false)
		a.save()
		jsonResponse(w, 200, map[string]bool{"ok": true})
	case "/open":
		if input.ID == "" || len(input.ID) > 80 || strings.ContainsAny(input.ID, "/\\") {
			http.Error(w, "Bad ID", 400)
			return
		}
		_, e := a.open(input.ID)
		if e != nil {
			jsonResponse(w, 409, map[string]string{"error": e.Error()})
			return
		}
		jsonResponse(w, 200, map[string]bool{"ok": true})
	default:
		http.NotFound(w, r)
	}
}
func main() {
	editor, recent := paths()
	if len(os.Args) > 1 && os.Args[1] == "--office" {
		exe, _ := os.Executable()
		agent := exec.Command(exe, "--serve")
		agent.Stdout = io.Discard
		agent.Stderr = io.Discard
		if agent.Start() == nil {
			go agent.Wait()
		}
		args := os.Args[2:]
		cmd := exec.Command(editor, args...)
		cmd.Stdout = io.Discard
		cmd.Stderr = io.Discard
		cmd.Run()
		return
	}
	listener, e := net.Listen("tcp", address)
	if e != nil {
		return
	} // Existing user agent owns the port.
	a, e := newAgent(dataDir(), editor, recent)
	if e != nil {
		log.Print(e)
		return
	}
	logfile, e := os.OpenFile(filepath.Join(a.dir, "agent.log"), os.O_CREATE|os.O_APPEND|os.O_WRONLY, 0600)
	if e == nil {
		log.SetOutput(logfile)
	}
	go func() {
		ticker := time.NewTicker(5 * time.Second)
		defer ticker.Stop()
		for range ticker.C {
			a.sync()
		}
	}()
	server := &http.Server{Handler: http.HandlerFunc(a.handler), ReadHeaderTimeout: 5 * time.Second, ReadTimeout: 15 * time.Second, WriteTimeout: 90 * time.Second, IdleTimeout: 30 * time.Second, MaxHeaderBytes: 8192}
	if e = server.Serve(listener); e != nil {
		log.Print("MIDIA Cloud detenido")
	}
}
