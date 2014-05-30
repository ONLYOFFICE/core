// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/dragdrop/os_exchange_data_provider_aurax11.h"

#include "base/logging.h"
#include "base/message_pump_aurax11.h"
#include "base/string_util.h"
#include "base/utf_string_conversions.h"
#include "net/base/net_util.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/clipboard/scoped_clipboard_writer.h"
#include "ui/base/dragdrop/desktop_selection_provider_aurax11.h"
#include "ui/base/x/selection_utils.h"
#include "ui/base/x/x11_util.h"

// Note: the GetBlah() methods are used immediately by the
// web_contents_view_aura.cc:PrepareWebDropData(), while the omnibox is a
// little more discriminating and calls HasBlah() before trying to get the
// information.

namespace ui {

namespace {

const char kDndSelection[] = "XdndSelection";

const char* kAtomsToCache[] = {
  kString,
  kText,
  kUtf8String,
  kDndSelection,
  Clipboard::kMimeTypeURIList,
  kMimeTypeMozillaURL,
  NULL
};

}  // namespace

OSExchangeDataProviderAuraX11::OSExchangeDataProviderAuraX11(
    ui::DesktopSelectionProviderAuraX11* provider,
    ::Window x_window,
    const std::vector< ::Atom> targets)
    : x_display_(GetXDisplay()),
      x_root_window_(DefaultRootWindow(x_display_)),
      own_window_(false),
      selection_event_provider_(provider),
      x_window_(x_window),
      atom_cache_(x_display_, kAtomsToCache),
      selection_requestor_(x_display_, x_window_,
                           atom_cache_.GetAtom(kDndSelection)),
      selection_owner_(x_display_, x_window_,
                       atom_cache_.GetAtom(kDndSelection)),
      targets_(targets) {
  // We don't know all possible MIME types at compile time.
  atom_cache_.allow_uncached_atoms();

  selection_event_provider_->SetDropHandler(this);
}

OSExchangeDataProviderAuraX11::OSExchangeDataProviderAuraX11()
    : x_display_(GetXDisplay()),
      x_root_window_(DefaultRootWindow(x_display_)),
      own_window_(true),
      selection_event_provider_(NULL),
      x_window_(XCreateWindow(
          x_display_,
          x_root_window_,
          -100, -100, 10, 10,  // x, y, width, height
          0,                   // border width
          CopyFromParent,      // depth
          InputOnly,
          CopyFromParent,      // visual
          0,
          NULL)),
      atom_cache_(x_display_, kAtomsToCache),
      selection_requestor_(x_display_, x_window_,
                           atom_cache_.GetAtom(kDndSelection)),
      selection_owner_(x_display_, x_window_,
                       atom_cache_.GetAtom(kDndSelection)) {
  // We don't know all possible MIME types at compile time.
  atom_cache_.allow_uncached_atoms();

  XStoreName(x_display_, x_window_, "Chromium Drag & Drop Window");

  base::MessagePumpAuraX11::Current()->AddDispatcherForWindow(this, x_window_);
}

OSExchangeDataProviderAuraX11::~OSExchangeDataProviderAuraX11() {
  if (own_window_) {
    base::MessagePumpAuraX11::Current()->RemoveDispatcherForWindow(x_window_);
    XDestroyWindow(x_display_, x_window_);
  } else {
    selection_event_provider_->SetDropHandler(NULL);
  }
}

void OSExchangeDataProviderAuraX11::OnSelectionNotify(
    const XSelectionEvent& event) {
  selection_requestor_.OnSelectionNotify(event);
}

void OSExchangeDataProviderAuraX11::SetString(const string16& data) {
  NOTIMPLEMENTED();
}

void OSExchangeDataProviderAuraX11::SetURL(const GURL& url,
                                           const string16& title) {
  NOTIMPLEMENTED();
}

void OSExchangeDataProviderAuraX11::SetFilename(const base::FilePath& path) {
  NOTIMPLEMENTED();
}

void OSExchangeDataProviderAuraX11::SetFilenames(
    const std::vector<OSExchangeData::FileInfo>& filenames) {
  NOTIMPLEMENTED();
}

void OSExchangeDataProviderAuraX11::SetPickledData(
    OSExchangeData::CustomFormat format,
    const Pickle& data) {
  NOTIMPLEMENTED();
}

bool OSExchangeDataProviderAuraX11::GetString(string16* result) const {
  std::vector< ::Atom> text_atoms = ui::GetTextAtomsFrom(&atom_cache_);
  std::vector< ::Atom> requested_types;
  ui::GetAtomIntersection(text_atoms, targets_, &requested_types);

  scoped_ptr<ui::SelectionData> data(
      selection_requestor_.RequestAndWaitForTypes(requested_types));
  if (data) {
    std::string text = data->GetText();
    *result = UTF8ToUTF16(text);
    return true;
  }

  return false;
}

bool OSExchangeDataProviderAuraX11::GetURLAndTitle(GURL* url,
                                                   string16* title) const {
  std::vector< ::Atom> url_atoms = ui::GetURLAtomsFrom(&atom_cache_);
  std::vector< ::Atom> requested_types;
  ui::GetAtomIntersection(url_atoms, targets_, &requested_types);

  scoped_ptr<ui::SelectionData> data(
      selection_requestor_.RequestAndWaitForTypes(requested_types));
  if (data) {
    // TODO(erg): Technically, both of these forms can accept multiple URLs,
    // but that doesn't match the assumptions of the rest of the system which
    // expect single types.

    if (data->type() == atom_cache_.GetAtom(kMimeTypeMozillaURL)) {
      // Mozilla URLs are (UTF16: URL, newline, title).
      string16 unparsed;
      data->AssignTo(&unparsed);

      std::vector<string16> tokens;
      size_t num_tokens = Tokenize(unparsed, ASCIIToUTF16("\n"), &tokens);
      if (num_tokens >= 2) {
        *url = GURL(tokens[0]);
        *title = tokens[1];
        return true;
      } else {
        NOTREACHED() << "Data that claimed to be a Mozilla URL has "
                     << num_tokens << " tokens instead of 2.";
      }
    } else if (data->type() == atom_cache_.GetAtom(
                   Clipboard::kMimeTypeURIList)) {
      // uri-lists are newline separated file lists in URL encoding.
      std::string unparsed;
      data->AssignTo(&unparsed);

      std::vector<std::string> tokens;
      size_t num_tokens = Tokenize(unparsed, "\n", &tokens);
      if (!num_tokens) {
        NOTREACHED() << "Empty URI list";
        return false;
      }

      *url = GURL(tokens[0]);
      *title = string16();

      return true;
    }
  }

  return false;
}

bool OSExchangeDataProviderAuraX11::GetFilename(base::FilePath* path) const {
  // On X11, files are passed by URL and aren't separate.
  return false;
}

bool OSExchangeDataProviderAuraX11::GetFilenames(
    std::vector<OSExchangeData::FileInfo>* filenames) const {
  // On X11, files are passed by URL and aren't separate.
  return false;
}

bool OSExchangeDataProviderAuraX11::GetPickledData(
    OSExchangeData::CustomFormat format,
    Pickle* data) const {
  NOTIMPLEMENTED();
  return false;
}

bool OSExchangeDataProviderAuraX11::HasString() const {
  std::vector< ::Atom> text_atoms = ui::GetTextAtomsFrom(&atom_cache_);
  std::vector< ::Atom> requested_types;
  ui::GetAtomIntersection(text_atoms, targets_, &requested_types);
  return !requested_types.empty();
}

bool OSExchangeDataProviderAuraX11::HasURL() const {
  std::vector< ::Atom> url_atoms = ui::GetURLAtomsFrom(&atom_cache_);
  std::vector< ::Atom> requested_types;
  ui::GetAtomIntersection(url_atoms, targets_, &requested_types);
  return !requested_types.empty();
}

bool OSExchangeDataProviderAuraX11::HasFile() const {
  // On X11, files are passed by URL and aren't separate.
  return false;
}

bool OSExchangeDataProviderAuraX11::HasCustomFormat(
    OSExchangeData::CustomFormat format) const {
  std::vector< ::Atom> url_atoms;
  url_atoms.push_back(atom_cache_.GetAtom(format.ToString().c_str()));
  std::vector< ::Atom> requested_types;
  ui::GetAtomIntersection(url_atoms, targets_, &requested_types);

  return !requested_types.empty();
}

void OSExchangeDataProviderAuraX11::SetHtml(const string16& html,
                                            const GURL& base_url) {
  NOTIMPLEMENTED();
}

bool OSExchangeDataProviderAuraX11::GetHtml(string16* html,
                                            GURL* base_url) const {
  std::vector< ::Atom> url_atoms;
  url_atoms.push_back(atom_cache_.GetAtom(Clipboard::kMimeTypeHTML));
  std::vector< ::Atom> requested_types;
  ui::GetAtomIntersection(url_atoms, targets_, &requested_types);

  scoped_ptr<ui::SelectionData> data(
      selection_requestor_.RequestAndWaitForTypes(requested_types));
  if (data) {
    *html = data->GetHtml();
    *base_url = GURL();
    return true;
  }

  return false;
}

bool OSExchangeDataProviderAuraX11::HasHtml() const {
  std::vector< ::Atom> url_atoms;
  url_atoms.push_back(atom_cache_.GetAtom(Clipboard::kMimeTypeHTML));
  std::vector< ::Atom> requested_types;
  ui::GetAtomIntersection(url_atoms, targets_, &requested_types);

  return !requested_types.empty();
}

void OSExchangeDataProviderAuraX11::SetDragImage(
    const gfx::ImageSkia& image,
    const gfx::Vector2d& cursor_offset) {
  NOTIMPLEMENTED();
}

const gfx::ImageSkia& OSExchangeDataProviderAuraX11::GetDragImage() const {
  NOTIMPLEMENTED();
  return drag_image_;
}

const gfx::Vector2d& OSExchangeDataProviderAuraX11::GetDragImageOffset() const {
  NOTIMPLEMENTED();
  return drag_image_offset_;
}

bool OSExchangeDataProviderAuraX11::Dispatch(const base::NativeEvent& event) {
  // TODO(erg): Implement this side when we implement sending data.
  return false;
}

bool OSExchangeDataProviderAuraX11::GetPlainTextURL(GURL* url) const {
  NOTIMPLEMENTED();
  return false;
}

///////////////////////////////////////////////////////////////////////////////
// OSExchangeData, public:

// static
OSExchangeData::Provider* OSExchangeData::CreateProvider() {
  return new OSExchangeDataProviderAuraX11();
}

// static
OSExchangeData::CustomFormat
OSExchangeData::RegisterCustomFormat(const std::string& type) {
  // On AuraX11 you probably want to just use the Clipboard::Get*FormatType APIs
  // instead.  But we can also dynamically generate new CustomFormat objects
  // here too if really necessary.
  return Clipboard::FormatType::Deserialize(type);
}

}  // namespace ui
