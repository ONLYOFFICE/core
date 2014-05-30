// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/clipboard/clipboard.h"

#include <gtk/gtk.h>
#include <X11/extensions/Xfixes.h>
#include <X11/Xatom.h>
#include <map>
#include <set>
#include <string>
#include <utility>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/memory/singleton.h"
#include "base/utf_string_conversions.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/clipboard/custom_data_helper.h"
#include "ui/base/gtk/gtk_signal.h"
#include "ui/base/gtk/scoped_gobject.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/gtk_util.h"
#include "ui/gfx/size.h"

namespace ui {

namespace {

class SelectionChangeObserver {
 public:
  static SelectionChangeObserver* GetInstance();

  uint64 clipboard_sequence_number() const {
    return clipboard_sequence_number_;
  }
  uint64 primary_sequence_number() const { return primary_sequence_number_; }

 private:
  friend struct DefaultSingletonTraits<SelectionChangeObserver>;

  SelectionChangeObserver();
  ~SelectionChangeObserver();

  CHROMEG_CALLBACK_1(SelectionChangeObserver, GdkFilterReturn, OnXEvent,
                     GdkXEvent*, GdkEvent*);

  int event_base_;
  Atom clipboard_atom_;
  uint64 clipboard_sequence_number_;
  uint64 primary_sequence_number_;

  DISALLOW_COPY_AND_ASSIGN(SelectionChangeObserver);
};

SelectionChangeObserver::SelectionChangeObserver()
    : event_base_(-1),
      clipboard_atom_(None),
      clipboard_sequence_number_(0),
      primary_sequence_number_(0) {
  int ignored;
  if (XFixesQueryExtension(GetXDisplay(), &event_base_, &ignored)) {
    clipboard_atom_ = XInternAtom(GetXDisplay(), "CLIPBOARD", false);
    XFixesSelectSelectionInput(GetXDisplay(), GetX11RootWindow(),
                               clipboard_atom_,
                               XFixesSetSelectionOwnerNotifyMask |
                               XFixesSelectionWindowDestroyNotifyMask |
                               XFixesSelectionClientCloseNotifyMask);
    // This seems to be semi-optional. For some reason, registering for any
    // selection notify events seems to subscribe us to events for both the
    // primary and the clipboard buffers. Register anyway just to be safe.
    XFixesSelectSelectionInput(GetXDisplay(), GetX11RootWindow(),
                               XA_PRIMARY,
                               XFixesSetSelectionOwnerNotifyMask |
                               XFixesSelectionWindowDestroyNotifyMask |
                               XFixesSelectionClientCloseNotifyMask);
    gdk_window_add_filter(NULL, &SelectionChangeObserver::OnXEventThunk, this);
  }
}

SelectionChangeObserver::~SelectionChangeObserver() {
}

SelectionChangeObserver* SelectionChangeObserver::GetInstance() {
  return Singleton<SelectionChangeObserver>::get();
}

GdkFilterReturn SelectionChangeObserver::OnXEvent(GdkXEvent* xevent,
                                                  GdkEvent* event) {
  XEvent* xev = static_cast<XEvent*>(xevent);

  if (xev->type == event_base_ + XFixesSelectionNotify) {
    XFixesSelectionNotifyEvent* ev =
        reinterpret_cast<XFixesSelectionNotifyEvent*>(xev);
    if (ev->selection == clipboard_atom_) {
      clipboard_sequence_number_++;
    } else if (ev->selection == XA_PRIMARY) {
      primary_sequence_number_++;
    } else {
      DLOG(ERROR) << "Unexpected selection atom: " << ev->selection;
    }
  }
  return GDK_FILTER_CONTINUE;
}

const char kSourceTagType[] = "org.chromium.source-tag";
const char kMimeTypeBitmap[] = "image/bmp";
const char kMimeTypeMozillaURL[] = "text/x-moz-url";
const char kMimeTypePepperCustomData[] = "chromium/x-pepper-custom-data";
const char kMimeTypeWebkitSmartPaste[] = "chromium/x-webkit-paste";

std::string GdkAtomToString(const GdkAtom& atom) {
  gchar* name = gdk_atom_name(atom);
  std::string rv(name);
  g_free(name);
  return rv;
}

GdkAtom StringToGdkAtom(const std::string& str) {
  return gdk_atom_intern(str.c_str(), FALSE);
}

// GtkClipboardGetFunc callback.
// GTK will call this when an application wants data we copied to the clipboard.
void GetData(GtkClipboard* clipboard,
             GtkSelectionData* selection_data,
             guint info,
             gpointer user_data) {
  Clipboard::TargetMap* data_map =
      reinterpret_cast<Clipboard::TargetMap*>(user_data);

  std::string target_string = GdkAtomToString(
      gtk_selection_data_get_target(selection_data));
  Clipboard::TargetMap::iterator iter = data_map->find(target_string);

  if (iter == data_map->end())
    return;

  if (target_string == kMimeTypeBitmap) {
    gtk_selection_data_set_pixbuf(selection_data,
        reinterpret_cast<GdkPixbuf*>(iter->second.first));
  } else {
    gtk_selection_data_set(selection_data,
                           gtk_selection_data_get_target(selection_data), 8,
                           reinterpret_cast<guchar*>(iter->second.first),
                           iter->second.second);
  }
}

// GtkClipboardClearFunc callback.
// We are guaranteed this will be called exactly once for each call to
// gtk_clipboard_set_with_data.
void ClearData(GtkClipboard* /*clipboard*/,
               gpointer user_data) {
  Clipboard::TargetMap* map =
      reinterpret_cast<Clipboard::TargetMap*>(user_data);
  // The same data may be inserted under multiple keys, so use a set to
  // uniq them.
  std::set<char*> ptrs;

  for (Clipboard::TargetMap::iterator iter = map->begin();
       iter != map->end(); ++iter) {
    if (iter->first == kMimeTypeBitmap)
      g_object_unref(reinterpret_cast<GdkPixbuf*>(iter->second.first));
    else
      ptrs.insert(iter->second.first);
  }

  for (std::set<char*>::iterator iter = ptrs.begin();
       iter != ptrs.end(); ++iter) {
    delete[] *iter;
  }

  delete map;
}

}  // namespace

Clipboard::FormatType::FormatType() : data_(GDK_NONE) {
}

Clipboard::FormatType::FormatType(const std::string& format_string)
    : data_(StringToGdkAtom(format_string)) {
}

Clipboard::FormatType::FormatType(const GdkAtom& native_format)
    : data_(native_format) {
}

Clipboard::FormatType::~FormatType() {
}

std::string Clipboard::FormatType::Serialize() const {
  return GdkAtomToString(data_);
}

// static
Clipboard::FormatType Clipboard::FormatType::Deserialize(
    const std::string& serialization) {
  return FormatType(serialization);
}

bool Clipboard::FormatType::Equals(const FormatType& other) const {
  return data_ == other.data_;
}

Clipboard::Clipboard() : clipboard_data_(NULL) {
  DCHECK(CalledOnValidThread());
  clipboard_ = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
  primary_selection_ = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
}

Clipboard::~Clipboard() {
  DCHECK(CalledOnValidThread());
  gtk_clipboard_store(clipboard_);
}

void Clipboard::WriteObjectsImpl(Buffer buffer,
                                 const ObjectMap& objects,
                                 SourceTag tag) {
  DCHECK(CalledOnValidThread());
  clipboard_data_ = new TargetMap();

  for (ObjectMap::const_iterator iter = objects.begin();
       iter != objects.end(); ++iter) {
    DispatchObject(static_cast<ObjectType>(iter->first), iter->second);
  }
  WriteSourceTag(tag);
  SetGtkClipboard(buffer);

  if (buffer == BUFFER_STANDARD) {
    ObjectMap::const_iterator text_iter = objects.find(CBF_TEXT);
    if (text_iter != objects.end()) {
      // Copy text and SourceTag to the selection clipboard.
      ObjectMap::const_iterator next_iter = text_iter;
      WriteObjectsImpl(BUFFER_SELECTION,
                       ObjectMap(text_iter, ++next_iter),
                       tag);
    }
  }
}

// Take ownership of the GTK clipboard and inform it of the targets we support.
void Clipboard::SetGtkClipboard(Buffer buffer) {
  scoped_ptr<GtkTargetEntry[]> targets(
      new GtkTargetEntry[clipboard_data_->size()]);

  int i = 0;
  for (Clipboard::TargetMap::iterator iter = clipboard_data_->begin();
       iter != clipboard_data_->end(); ++iter, ++i) {
    targets[i].target = const_cast<char*>(iter->first.c_str());
    targets[i].flags = 0;
    targets[i].info = 0;
  }

  GtkClipboard *clipboard = LookupBackingClipboard(buffer);

  if (gtk_clipboard_set_with_data(clipboard, targets.get(),
                                  clipboard_data_->size(),
                                  GetData, ClearData,
                                  clipboard_data_)) {
    gtk_clipboard_set_can_store(clipboard,
                                targets.get(),
                                clipboard_data_->size());
  }

  // clipboard_data_ now owned by the GtkClipboard.
  clipboard_data_ = NULL;
}

void Clipboard::WriteText(const char* text_data, size_t text_len) {
  char* data = new char[text_len];
  memcpy(data, text_data, text_len);

  InsertMapping(kMimeTypeText, data, text_len);
  InsertMapping("TEXT", data, text_len);
  InsertMapping("STRING", data, text_len);
  InsertMapping("UTF8_STRING", data, text_len);
  InsertMapping("COMPOUND_TEXT", data, text_len);
}

void Clipboard::WriteHTML(const char* markup_data,
                          size_t markup_len,
                          const char* url_data,
                          size_t url_len) {
  // TODO(estade): We need to expand relative links with |url_data|.
  static const char* html_prefix = "<meta http-equiv=\"content-type\" "
                                   "content=\"text/html; charset=utf-8\">";
  size_t html_prefix_len = strlen(html_prefix);
  size_t total_len = html_prefix_len + markup_len + 1;

  char* data = new char[total_len];
  snprintf(data, total_len, "%s", html_prefix);
  memcpy(data + html_prefix_len, markup_data, markup_len);
  // Some programs expect NULL-terminated data. See http://crbug.com/42624
  data[total_len - 1] = '\0';

  InsertMapping(kMimeTypeHTML, data, total_len);
}

void Clipboard::WriteRTF(const char* rtf_data, size_t data_len) {
  WriteData(GetRtfFormatType(), rtf_data, data_len);
}

// Write an extra flavor that signifies WebKit was the last to modify the
// pasteboard. This flavor has no data.
void Clipboard::WriteWebSmartPaste() {
  InsertMapping(kMimeTypeWebkitSmartPaste, NULL, 0);
}

void Clipboard::WriteBitmap(const char* pixel_data, const char* size_data) {
  const gfx::Size* size = reinterpret_cast<const gfx::Size*>(size_data);

  // Adopt the pixels into a SkBitmap. Note that the pixel order in memory is
  // actually BGRA.
  SkBitmap bitmap;
  bitmap.setConfig(SkBitmap::kARGB_8888_Config, size->width(), size->height());
  bitmap.setPixels(const_cast<char*>(pixel_data));
  GdkPixbuf* pixbuf = gfx::GdkPixbufFromSkBitmap(bitmap);

  // We store the GdkPixbuf*, and the size_t half of the pair is meaningless.
  // Note that this contrasts with the vast majority of entries in our target
  // map, which directly store the data and its length.
  InsertMapping(kMimeTypeBitmap, reinterpret_cast<char*>(pixbuf), 0);
}

void Clipboard::WriteBookmark(const char* title_data, size_t title_len,
                              const char* url_data, size_t url_len) {
  // Write as a mozilla url (UTF16: URL, newline, title).
  string16 url = UTF8ToUTF16(std::string(url_data, url_len) + "\n");
  string16 title = UTF8ToUTF16(std::string(title_data, title_len));
  if (title.length() >= std::numeric_limits<size_t>::max() / 4 ||
      url.length() >= std::numeric_limits<size_t>::max() / 4)
    return;
  size_t data_len = 2 * (title.length() + url.length());

  char* data = new char[data_len];
  memcpy(data, url.data(), 2 * url.length());
  memcpy(data + 2 * url.length(), title.data(), 2 * title.length());
  InsertMapping(kMimeTypeMozillaURL, data, data_len);
}

void Clipboard::WriteData(const FormatType& format,
                          const char* data_data,
                          size_t data_len) {
  // We assume that certain mapping types are only written by trusted code.
  // Therefore we must upkeep their integrity.
  if (format.Equals(GetBitmapFormatType()))
    return;
  char* data = new char[data_len];
  memcpy(data, data_data, data_len);
  // TODO(dcheng): Maybe this map should use GdkAtoms...
  InsertMapping(GdkAtomToString(format.ToGdkAtom()).c_str(), data, data_len);
}

void Clipboard::WriteSourceTag(SourceTag tag) {
  if (tag != SourceTag()) {
    ObjectMapParam binary = SourceTag2Binary(tag);
    WriteData(GetSourceTagFormatType(), &binary[0], binary.size());
  }
}

// We do not use gtk_clipboard_wait_is_target_available because of
// a bug with the gtk clipboard. It caches the available targets
// and does not always refresh the cache when it is appropriate.
bool Clipboard::IsFormatAvailable(const Clipboard::FormatType& format,
                                  Clipboard::Buffer buffer) const {
  DCHECK(CalledOnValidThread());
  GtkClipboard* clipboard = LookupBackingClipboard(buffer);
  if (clipboard == NULL)
    return false;

  bool retval = false;
  GtkSelectionData* data = gtk_clipboard_wait_for_contents(
      clipboard, gdk_atom_intern_static_string("TARGETS"));

  bool format_is_plain_text = GetPlainTextFormatType().Equals(format);
  if (format_is_plain_text) {
    // This tries a number of common text targets.
    if (data) {
      retval = gtk_selection_data_targets_include_text(data);
    }
    // Some programs like Java decide to set an empty TARGETS list, so even if
    // data is not NULL, we still have to fall back.
    if (!retval) {
      // Some programs post data to the clipboard without any targets. If this
      // is the case we attempt to make sense of the contents as text. This is
      // pretty unfortunate since it means we have to actually copy the data to
      // see if it is available, but at least this path shouldn't be hit for
      // conforming programs.
      gchar* text = gtk_clipboard_wait_for_text(clipboard);
      if (text) {
        g_free(text);
        retval = true;
      }
    }
  } else if (data) {
    GdkAtom* targets = NULL;
    int num = 0;
    gtk_selection_data_get_targets(data, &targets, &num);

    for (int i = 0; i < num; i++) {
      if (targets[i] == format.ToGdkAtom()) {
        retval = true;
        break;
      }
    }

    g_free(targets);
  }

  if (data)
    gtk_selection_data_free(data);

  return retval;
}

void Clipboard::Clear(Clipboard::Buffer buffer) {
  DCHECK(CalledOnValidThread());
  GtkClipboard* clipboard = LookupBackingClipboard(buffer);
  if (clipboard == NULL)
    return;
  gtk_clipboard_clear(clipboard);
}

void Clipboard::ReadAvailableTypes(Clipboard::Buffer buffer,
                                   std::vector<string16>* types,
                                   bool* contains_filenames) const {
  DCHECK(CalledOnValidThread());
  if (!types || !contains_filenames) {
    NOTREACHED();
    return;
  }

  types->clear();
  if (IsFormatAvailable(GetPlainTextFormatType(), buffer))
    types->push_back(UTF8ToUTF16(kMimeTypeText));
  if (IsFormatAvailable(GetHtmlFormatType(), buffer))
    types->push_back(UTF8ToUTF16(kMimeTypeHTML));
  if (IsFormatAvailable(GetRtfFormatType(), buffer))
    types->push_back(UTF8ToUTF16(kMimeTypeRTF));
  if (IsFormatAvailable(GetBitmapFormatType(), buffer))
    types->push_back(UTF8ToUTF16(kMimeTypePNG));
  *contains_filenames = false;

  GtkClipboard* clipboard = LookupBackingClipboard(buffer);
  if (!clipboard)
    return;

  GtkSelectionData* data = gtk_clipboard_wait_for_contents(
      clipboard, GetWebCustomDataFormatType().ToGdkAtom());
  if (!data)
    return;
  ReadCustomDataTypes(gtk_selection_data_get_data(data),
                      gtk_selection_data_get_length(data),
                      types);
  gtk_selection_data_free(data);
}


void Clipboard::ReadText(Clipboard::Buffer buffer, string16* result) const {
  DCHECK(CalledOnValidThread());
  GtkClipboard* clipboard = LookupBackingClipboard(buffer);
  if (clipboard == NULL)
    return;

  result->clear();
  gchar* text = gtk_clipboard_wait_for_text(clipboard);

  if (text == NULL)
    return;

  // TODO(estade): do we want to handle the possible error here?
  UTF8ToUTF16(text, strlen(text), result);
  g_free(text);
}

void Clipboard::ReadAsciiText(Clipboard::Buffer buffer,
                              std::string* result) const {
  DCHECK(CalledOnValidThread());
  GtkClipboard* clipboard = LookupBackingClipboard(buffer);
  if (clipboard == NULL)
    return;

  result->clear();
  gchar* text = gtk_clipboard_wait_for_text(clipboard);

  if (text == NULL)
    return;

  result->assign(text);
  g_free(text);
}

// TODO(estade): handle different charsets.
// TODO(port): set *src_url.
void Clipboard::ReadHTML(Clipboard::Buffer buffer, string16* markup,
                         std::string* src_url, uint32* fragment_start,
                         uint32* fragment_end) const {
  DCHECK(CalledOnValidThread());
  markup->clear();
  if (src_url)
    src_url->clear();
  *fragment_start = 0;
  *fragment_end = 0;

  GtkClipboard* clipboard = LookupBackingClipboard(buffer);
  if (clipboard == NULL)
    return;
  GtkSelectionData* data = gtk_clipboard_wait_for_contents(clipboard,
      GetHtmlFormatType().ToGdkAtom());

  if (!data)
    return;

  // If the data starts with 0xFEFF, i.e., Byte Order Mark, assume it is
  // UTF-16, otherwise assume UTF-8.
  gint data_length = gtk_selection_data_get_length(data);
  const guchar* raw_data = gtk_selection_data_get_data(data);

  if (data_length >= 2 &&
      reinterpret_cast<const uint16_t*>(raw_data)[0] == 0xFEFF) {
    markup->assign(reinterpret_cast<const uint16_t*>(raw_data) + 1,
                   (data_length / 2) - 1);
  } else {
    UTF8ToUTF16(reinterpret_cast<const char*>(raw_data), data_length, markup);
  }

  // If there is a terminating NULL, drop it.
  if (!markup->empty() && markup->at(markup->length() - 1) == '\0')
    markup->resize(markup->length() - 1);

  *fragment_start = 0;
  DCHECK(markup->length() <= kuint32max);
  *fragment_end = static_cast<uint32>(markup->length());

  gtk_selection_data_free(data);
}

void Clipboard::ReadRTF(Buffer buffer, std::string* result) const {
  DCHECK(CalledOnValidThread());
  ReadData(GetRtfFormatType(), result);
}

SkBitmap Clipboard::ReadImage(Buffer buffer) const {
  DCHECK(CalledOnValidThread());
  ScopedGObject<GdkPixbuf>::Type pixbuf(
      gtk_clipboard_wait_for_image(clipboard_));
  if (!pixbuf.get())
    return SkBitmap();

  gfx::Canvas canvas(gfx::Size(gdk_pixbuf_get_width(pixbuf.get()),
                               gdk_pixbuf_get_height(pixbuf.get())),
                     ui::SCALE_FACTOR_100P,
                     false);
  {
    skia::ScopedPlatformPaint scoped_platform_paint(canvas.sk_canvas());
    cairo_t* context = scoped_platform_paint.GetPlatformSurface();
    gdk_cairo_set_source_pixbuf(context, pixbuf.get(), 0.0, 0.0);
    cairo_paint(context);
  }
  return canvas.ExtractImageRep().sk_bitmap();
}

void Clipboard::ReadCustomData(Buffer buffer,
                               const string16& type,
                               string16* result) const {
  DCHECK(CalledOnValidThread());
  GtkClipboard* clipboard = LookupBackingClipboard(buffer);
  if (!clipboard)
    return;

  GtkSelectionData* data = gtk_clipboard_wait_for_contents(
      clipboard, GetWebCustomDataFormatType().ToGdkAtom());
  if (!data)
    return;
  ReadCustomDataForType(gtk_selection_data_get_data(data),
                        gtk_selection_data_get_length(data),
                        type, result);
  gtk_selection_data_free(data);
}

void Clipboard::ReadBookmark(string16* title, std::string* url) const {
  // TODO(estade): implement this.
  NOTIMPLEMENTED();
}

void Clipboard::ReadData(const FormatType& format, std::string* result) const {
  ReadDataImpl(BUFFER_STANDARD, format, result);
}

void Clipboard::ReadDataImpl(Buffer buffer,
                             const FormatType& format,
                             std::string* result) const {
  DCHECK(CalledOnValidThread());
  result->clear();
  GtkClipboard* clipboard = LookupBackingClipboard(buffer);
  GtkSelectionData* data =
      gtk_clipboard_wait_for_contents(clipboard, format.ToGdkAtom());
  if (!data)
    return;
  result->assign(reinterpret_cast<const char*>(
                     gtk_selection_data_get_data(data)),
                 gtk_selection_data_get_length(data));
  gtk_selection_data_free(data);
}

SourceTag Clipboard::ReadSourceTag(Buffer buffer) const {
  std::string result;
  ReadDataImpl(buffer, GetSourceTagFormatType(), &result);
  return Binary2SourceTag(result);
}

uint64 Clipboard::GetSequenceNumber(Buffer buffer) {
  DCHECK(CalledOnValidThread());
  if (buffer == BUFFER_STANDARD)
    return SelectionChangeObserver::GetInstance()->clipboard_sequence_number();
  else
    return SelectionChangeObserver::GetInstance()->primary_sequence_number();
}

//static
Clipboard::FormatType Clipboard::GetFormatType(
    const std::string& format_string) {
  return FormatType::Deserialize(format_string);
}

// static
const Clipboard::FormatType& Clipboard::GetPlainTextFormatType() {
  CR_DEFINE_STATIC_LOCAL(
      FormatType, type, (GDK_TARGET_STRING));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetPlainTextWFormatType() {
  return GetPlainTextFormatType();
}

// static
const Clipboard::FormatType& Clipboard::GetUrlFormatType() {
  return GetPlainTextFormatType();
}

// static
const Clipboard::FormatType& Clipboard::GetUrlWFormatType() {
  return GetPlainTextWFormatType();
}

// static
const Clipboard::FormatType& Clipboard::GetHtmlFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kMimeTypeHTML));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetRtfFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kMimeTypeRTF));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetBitmapFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kMimeTypeBitmap));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetWebKitSmartPasteFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kMimeTypeWebkitSmartPaste));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetWebCustomDataFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kMimeTypeWebCustomData));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetPepperCustomDataFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kMimeTypePepperCustomData));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetSourceTagFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kSourceTagType));
  return type;
}

void Clipboard::InsertMapping(const char* key,
                              char* data,
                              size_t data_len) {
  DCHECK(clipboard_data_->find(key) == clipboard_data_->end());
  (*clipboard_data_)[key] = std::make_pair(data, data_len);
}

GtkClipboard* Clipboard::LookupBackingClipboard(Buffer clipboard) const {
  switch (clipboard) {
    case BUFFER_STANDARD:
      return clipboard_;
    case BUFFER_SELECTION:
      return primary_selection_;
    default:
      NOTREACHED();
      return NULL;
  }
}

}  // namespace ui
