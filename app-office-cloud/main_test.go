package main

import (
	"encoding/json"
	"net/http"
	"net/http/httptest"
	"os"
	"path/filepath"
	"strings"
	"testing"
	"time"
)

func writeStable(t *testing.T, p, s string) {
	t.Helper()
	os.Chmod(p, 0600)
	if e := os.WriteFile(p, []byte(s), 0600); e != nil {
		t.Fatal(e)
	}
	past := time.Now().Add(-time.Minute)
	os.Chtimes(p, past, past)
}
func TestOpenedFilesAndVersionSafety(t *testing.T) {
	dir := t.TempDir()
	recent := filepath.Join(dir, "recents.xml")
	old := filepath.Join(dir, "old.xlsx")
	fresh := filepath.Join(dir, "照片.xlsx")
	writeStable(t, old, "historical")
	writeStable(t, fresh, "original photos")
	os.WriteFile(recent, []byte(`<recents><file path="`+old+`" date="12.10.2026 12:00"/></recents>`), 0600)
	a, e := newAgent(dir, "not-launched", recent)
	if e != nil {
		t.Fatal(e)
	}
	a.state.User = "alice"
	a.state.Token = strings.Repeat("a", 64)
	remote := ""
	version := 0
	failNetwork := false
	permission := "owner"
	a.opener = func(string) error { return nil }
	server := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		if failNetwork {
			http.Error(w, `{"error":"offline"}`, 503)
			return
		}
		file := File{ID: "fixture", Name: "照片.xlsx", Version: version, Hash: hash([]byte(remote)), Permission: permission, Owner: "alice"}
		switch {
		case r.URL.Path == "/files" && r.Method == "GET":
			json.NewEncoder(w).Encode(map[string]any{"files": []File{file}})
		case r.URL.Path == "/files/fixture/content" && r.Method == "GET":
			w.Write([]byte(remote))
		case r.Method == "POST" || r.Method == "PUT":
			if r.Method == "PUT" && r.URL.Query().Get("version") != "1" {
				http.Error(w, `{"error":"conflict"}`, 409)
				return
			}
			if r.Method == "PUT" && version != 1 {
				http.Error(w, `{"error":"conflict"}`, 409)
				return
			}
			b := make([]byte, r.ContentLength)
			r.Body.Read(b)
			remote = string(b)
			version++
			file.Version = version
			file.Hash = hash(b)
			json.NewEncoder(w).Encode(map[string]any{"file": file})
		}
	}))
	defer server.Close()
	a.base = server.URL
	a.sync()
	if len(a.state.Tracks["alice"]) != 0 {
		t.Fatal("historical files must not upload")
	}
	os.WriteFile(recent, []byte(`<recents><file path="`+fresh+`" date="14.10.2026 01:00"/><file path="`+old+`" date="12.10.2026 12:00"/></recents>`), 0600)
	a.sync()
	tracked := a.state.Tracks["alice"][fresh]
	if tracked == nil || tracked.File.ID != "fixture" || remote != "original photos" {
		t.Fatalf("open did not upload: %+v", tracked)
	}
	writeStable(t, fresh, "saved change")
	failNetwork = true
	a.sync()
	if tracked.Error == "" || remote != "original photos" {
		t.Fatal("offline edit must stay local")
	}
	failNetwork = false
	a.sync()
	if remote != "saved change" || tracked.File.Version != 2 {
		t.Fatal("offline change not retried")
	}
	remote = "other user saved"
	version = 3
	writeStable(t, fresh, "my unsent change")
	a.sync()
	if tracked.Error != "conflict" || remote != "other user saved" {
		t.Fatal("stale edit overwrote cloud")
	}
	p, e := a.open("fixture")
	if e != nil {
		t.Fatal(e)
	}
	if b, _ := os.ReadFile(p); string(b) != remote {
		t.Fatal("wrong download")
	}
	writeStable(t, p, "unsent local cloud copy")
	if _, e = a.open("fixture"); e == nil {
		t.Fatal("opening again discarded local edit")
	}
	if _, e = a.open("unauthorized"); e == nil {
		t.Fatal("arbitrary file opened")
	}
	permission = "read"
	writeStable(t, p, remote)
	p, e = a.open("fixture")
	if e != nil {
		t.Fatal(e)
	}
	writeStable(t, p, "attempt read-only modification")
	a.sync()
	if remote != "other user saved" {
		t.Fatal("read-only change uploaded")
	}
}
func TestLoopbackOriginNonceAndNoPathAPI(t *testing.T) {
	a, e := newAgent(t.TempDir(), "", "")
	if e != nil {
		t.Fatal(e)
	}
	call := func(host, site, method, path, nonce, body string) *httptest.ResponseRecorder {
		r := httptest.NewRequest(method, "http://"+host+path, strings.NewReader(body))
		r.Host = host
		r.Header.Set("Origin", site)
		r.Header.Set("X-MIDIA-Nonce", nonce)
		w := httptest.NewRecorder()
		a.handler(w, r)
		return w
	}
	if call(address, "https://evil.example", "GET", "/status", "", "").Code != 403 {
		t.Fatal("foreign origin")
	}
	if call("evil.example", origin, "GET", "/status", "", "").Code != 403 {
		t.Fatal("DNS rebinding")
	}
	if call(address, origin, "GET", "/status", "", "").Code != 200 {
		t.Fatal("status")
	}
	if call(address, origin, "POST", "/open", "bad", `{"id":"fixture"}`).Code != 403 {
		t.Fatal("nonce")
	}
	if call(address, origin, "POST", "/track", a.nonce, `{"path":"/etc/secret.xlsx"}`).Code != 404 {
		t.Fatal("arbitrary path API")
	}
	if call(address, origin, "POST", "/open", a.nonce, `{"id":"../../secret"}`).Code != 400 {
		t.Fatal("bad ID")
	}
}

func TestPortableCacheName(t *testing.T) {
	if cacheName("Oferta: octubre?.xlsx") != "MIDIA-Oferta_ octubre_.xlsx" {
		t.Fatal("Windows filename mapping")
	}
	if len([]rune(cacheName(strings.Repeat("照", 100)+".xlsx"))) > 80 {
		t.Fatal("cache filename too long")
	}
}
