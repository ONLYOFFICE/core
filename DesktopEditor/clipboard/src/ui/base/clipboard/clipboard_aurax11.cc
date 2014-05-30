// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/clipboard/clipboard.h"

#include <X11/extensions/Xfixes.h>
#include <X11/Xatom.h>
#include <list>
#include <set>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/message_pump_aurax11.h"
#include "base/message_pump_observer.h"
#include "base/stl_util.h"
#include "base/utf_string_conversions.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/clipboard/custom_data_helper.h"
#include "ui/base/x/selection_owner.h"
#include "ui/base/x/selection_requestor.h"
#include "ui/base/x/selection_utils.h"
#include "ui/base/x/x11_atom_cache.h"
#include "ui/base/x/x11_util.h"

#include "ui/gfx/size.h"

namespace ui {

namespace {

const char kClipboard[] = "CLIPBOARD";
const char kMimeTypeBitmap[] = "image/bmp";
const char kMimeTypeFilename[] = "chromium/filename";
const char kMimeTypePepperCustomData[] = "chromium/x-pepper-custom-data";
const char kMimeTypeWebkitSmartPaste[] = "chromium/x-webkit-paste";
const char kSourceTagType[] = "org.chromium.source-tag";
const char kTargets[] = "TARGETS";

const char* kAtomsToCache[] = {
  kClipboard,
  kMimeTypeBitmap,
  kMimeTypeFilename,
  kMimeTypeMozillaURL,
  kMimeTypeWebkitSmartPaste,
  kSourceTagType,
  kString,
  kTargets,
  kText,
  kUtf8String,
  NULL
};

///////////////////////////////////////////////////////////////////////////////

// Uses the XFixes API to provide sequence numbers for GetSequenceNumber().
class SelectionChangeObserver : public base::MessagePumpObserver {
 public:
  static SelectionChangeObserver* GetInstance();

  uint64 clipboard_sequence_number() const {
    return clipboard_sequence_number_;
  }
  uint64 primary_sequence_number() const { return primary_sequence_number_; }

 private:
  friend struct DefaultSingletonTraits<SelectionChangeObserver>;

  SelectionChangeObserver();
  virtual ~SelectionChangeObserver();

  // Overridden from base::MessagePumpObserver:
  virtual base::EventStatus WillProcessEvent(
      const base::NativeEvent& event) OVERRIDE;
  virtual void DidProcessEvent(
      const base::NativeEvent& event) OVERRIDE {}

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
    clipboard_atom_ = XInternAtom(GetXDisplay(), kClipboard, false);
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

    base::MessagePumpAuraX11::Current()->AddObserver(this);
  }
}

SelectionChangeObserver::~SelectionChangeObserver() {
  // We are a singleton; we will outlive our message pump.
}

SelectionChangeObserver* SelectionChangeObserver::GetInstance() {
  return Singleton<SelectionChangeObserver>::get();
}

base::EventStatus SelectionChangeObserver::WillProcessEvent(
    const base::NativeEvent& event) {
  if (event->type == event_base_ + XFixesSelectionNotify) {
    XFixesSelectionNotifyEvent* ev =
        reinterpret_cast<XFixesSelectionNotifyEvent*>(event);
    if (ev->selection == clipboard_atom_) {
      clipboard_sequence_number_++;
    } else if (ev->selection == XA_PRIMARY) {
      primary_sequence_number_++;
    } else {
      DLOG(ERROR) << "Unexpected selection atom: " << ev->selection;
    }
  }
  return base::EVENT_CONTINUE;
}

///////////////////////////////////////////////////////////////////////////////

// Represents a list of possible return types. Copy constructable.
class TargetList {
 public:
  typedef std::vector< ::Atom> AtomVector;

  TargetList(const AtomVector& target_list, X11AtomCache* atom_cache);

  const AtomVector& target_list() { return target_list_; }

  bool ContainsText() const;
  bool ContainsFormat(const Clipboard::FormatType& format_type) const;
  bool ContainsAtom(::Atom atom) const;

 private:
  AtomVector target_list_;
  X11AtomCache* atom_cache_;
};

TargetList::TargetList(const AtomVector& target_list,
                       X11AtomCache* atom_cache)
    : target_list_(target_list),
      atom_cache_(atom_cache) {
}

bool TargetList::ContainsText() const {
  std::vector< ::Atom> atoms = GetTextAtomsFrom(atom_cache_);
  for (std::vector< ::Atom>::const_iterator it = atoms.begin();
       it != atoms.end(); ++it) {
    if (ContainsAtom(*it))
      return true;
  }

  return false;
}

bool TargetList::ContainsFormat(
    const Clipboard::FormatType& format_type) const {
  ::Atom atom = atom_cache_->GetAtom(format_type.ToString().c_str());
  return ContainsAtom(atom);
}

bool TargetList::ContainsAtom(::Atom atom) const {
  return find(target_list_.begin(), target_list_.end(), atom)
      != target_list_.end();
}

}  // namespace

///////////////////////////////////////////////////////////////////////////////

// I would love for the FormatType to really be a wrapper around an X11 ::Atom,
// but there are a few problems. Chromeos unit tests spawn a new X11 server for
// each test, so Atom numeric values don't persist across tests. We could still
// maybe deal with that if we didn't have static accessor methods everywhere.

Clipboard::FormatType::FormatType() {
}

Clipboard::FormatType::FormatType(const std::string& native_format)
    : data_(native_format) {
}

Clipboard::FormatType::~FormatType() {
}

std::string Clipboard::FormatType::Serialize() const {
  return data_;
}

// static
Clipboard::FormatType Clipboard::FormatType::Deserialize(
    const std::string& serialization) {
  return FormatType(serialization);
}

bool Clipboard::FormatType::Equals(const FormatType& other) const {
  return data_ == other.data_;
}

///////////////////////////////////////////////////////////////////////////////
// Clipboard::AuraX11Details

// Private implementation of our X11 integration. Keeps X11 headers out of the
// majority of chrome, which break badly.
class Clipboard::AuraX11Details : public base::MessagePumpDispatcher {
 public:
  AuraX11Details();
  virtual ~AuraX11Details();

  X11AtomCache* atom_cache() { return &atom_cache_; }

  // Returns the X11 type that we pass to various XSelection functions for the
  // given buffer.
  ::Atom LookupSelectionForBuffer(Buffer buffer) const;

  // Returns the object which is responsible for communication on |buffer|.
  SelectionRequestor* GetSelectionRequestorForBuffer(Buffer buffer);

  // Finds the SelectionFormatMap for the incoming selection atom.
  SelectionFormatMap* LookupStorageForAtom(::Atom atom);

  // As we need to collect all the data types before we tell X11 that we own a
  // particular selection, we create a temporary clipboard mapping that
  // InsertMapping writes to. Then we commit it in TakeOwnershipOfSelection,
  // where we save it in one of the clipboard data slots.
  void CreateNewClipboardData();

  // Inserts a mapping into clipboard_data_.
  void InsertMapping(const std::string& key, char* data, size_t data_len);

  // Moves the temporary |clipboard_data_| to the long term data storage for
  // |buffer|.
  void TakeOwnershipOfSelection(Buffer buffer);

  // Returns the first of |types| offered by the current selection holder in
  // |data_out|, or returns NULL if none of those types are available.
  //
  // If the selection holder is us, this call is synchronous and we pull
  // the data out of |clipboard_selection_| or |primary_selection_|. If the
  // selection holder is some other window, we spin up a nested message loop
  // and do the asynchronous dance with whatever application is holding the
  // selection.
  scoped_ptr<SelectionData> RequestAndWaitForTypes(
      Buffer buffer,
      const std::vector< ::Atom>& types);

  // Retrieves the list of possible data types the current clipboard owner has.
  //
  // If the selection holder is us, this is synchronous, otherwise this runs a
  // blocking message loop.
  TargetList WaitAndGetTargetsList(Buffer buffer);

  // Returns a list of all text atoms that we handle.
  std::vector< ::Atom> GetTextAtoms() const;

  // Returns a vector with a |format| converted to an X11 atom.
  std::vector< ::Atom> GetAtomsForFormat(const Clipboard::FormatType& format);

  // Clears a certain data buffer.
  void Clear(Buffer buffer);

 private:
  // Overridden from base::MessagePumpDispatcher:
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

  // Our X11 state.
  Display* x_display_;
  ::Window x_root_window_;

  // Input-only window used as a selection owner.
  ::Window x_window_;

  X11AtomCache atom_cache_;

  // Objects which request and receive selection data.
  SelectionRequestor clipboard_requestor_;
  SelectionRequestor primary_requestor_;

  // Temporary target map that we write to during DispatchObects.
  scoped_ptr<SelectionFormatMap> clipboard_data_;

  // Objects which offer selection data to other windows.
  SelectionOwner clipboard_owner_;
  SelectionOwner primary_owner_;

  DISALLOW_COPY_AND_ASSIGN(AuraX11Details);
};

Clipboard::AuraX11Details::AuraX11Details()
    : x_display_(GetXDisplay()),
      x_root_window_(DefaultRootWindow(x_display_)),
      x_window_(XCreateWindow(
          x_display_, x_root_window_,
          -100, -100, 10, 10,  // x, y, width, height
          0,                   // border width
          CopyFromParent,      // depth
          InputOnly,
          CopyFromParent,      // visual
          0,
          NULL)),
      atom_cache_(x_display_, kAtomsToCache),
      clipboard_requestor_(x_display_, x_window_,
                           atom_cache_.GetAtom(kClipboard)),
      primary_requestor_(x_display_, x_window_, XA_PRIMARY),
      clipboard_owner_(x_display_, x_window_, atom_cache_.GetAtom(kClipboard)),
      primary_owner_(x_display_, x_window_, XA_PRIMARY) {
  // We don't know all possible MIME types at compile time.
  atom_cache_.allow_uncached_atoms();

  XStoreName(x_display_, x_window_, "Chromium clipboard");
  XSelectInput(x_display_, x_window_, PropertyChangeMask);

  base::MessagePumpAuraX11::Current()->AddDispatcherForWindow(this, x_window_);
}

Clipboard::AuraX11Details::~AuraX11Details() {
  base::MessagePumpAuraX11::Current()->RemoveDispatcherForWindow(x_window_);

  XDestroyWindow(x_display_, x_window_);
}

::Atom Clipboard::AuraX11Details::LookupSelectionForBuffer(
    Buffer buffer) const {
  if (buffer == BUFFER_STANDARD)
    return atom_cache_.GetAtom(kClipboard);
  else
    return XA_PRIMARY;
}

SelectionFormatMap* Clipboard::AuraX11Details::LookupStorageForAtom(
    ::Atom atom) {
  if (atom == XA_PRIMARY)
    return primary_owner_.selection_format_map();
  else if (atom == atom_cache_.GetAtom(kClipboard))
    return clipboard_owner_.selection_format_map();
  else
    return NULL;
}

ui::SelectionRequestor*
Clipboard::AuraX11Details::GetSelectionRequestorForBuffer(Buffer buffer) {
  if (buffer == BUFFER_STANDARD)
    return &clipboard_requestor_;
  else
    return &primary_requestor_;
}

void Clipboard::AuraX11Details::CreateNewClipboardData() {
  clipboard_data_.reset(new SelectionFormatMap);
}

void Clipboard::AuraX11Details::InsertMapping(const std::string& key,
                                              char* data,
                                              size_t data_len) {
  ::Atom atom_key = atom_cache_.GetAtom(key.c_str());
  clipboard_data_->Insert(atom_key, data, data_len);
}

void Clipboard::AuraX11Details::TakeOwnershipOfSelection(Buffer buffer) {
  if (buffer == BUFFER_STANDARD)
    return clipboard_owner_.TakeOwnershipOfSelection(clipboard_data_.Pass());
  else
    return primary_owner_.TakeOwnershipOfSelection(clipboard_data_.Pass());
}

scoped_ptr<SelectionData> Clipboard::AuraX11Details::RequestAndWaitForTypes(
    Buffer buffer,
    const std::vector< ::Atom>& types) {
  ::Atom selection_name = LookupSelectionForBuffer(buffer);
  if (XGetSelectionOwner(x_display_, selection_name) == x_window_) {
    // We can local fastpath instead of playing the nested message loop game
    // with the X server.
    SelectionFormatMap* format_map = LookupStorageForAtom(selection_name);
    DCHECK(format_map);

    for (std::vector< ::Atom>::const_iterator it = types.begin();
         it != types.end(); ++it) {
      SelectionFormatMap::const_iterator format_map_it = format_map->find(*it);
      if (format_map_it != format_map->end()) {
        scoped_ptr<SelectionData> data_out(new SelectionData(x_display_));
        data_out->Set(format_map_it->first, format_map_it->second.first,
                      format_map_it->second.second, false);
        return data_out.Pass();
      }
    }
  } else {
    TargetList targets = WaitAndGetTargetsList(buffer);
    SelectionRequestor* receiver = GetSelectionRequestorForBuffer(buffer);

    std::vector< ::Atom> intersection;
    ui::GetAtomIntersection(targets.target_list(), types, &intersection);
    return receiver->RequestAndWaitForTypes(intersection);
  }

  return scoped_ptr<SelectionData>();
}

TargetList Clipboard::AuraX11Details::WaitAndGetTargetsList(
    Buffer buffer) {
  ::Atom selection_name = LookupSelectionForBuffer(buffer);
  std::vector< ::Atom> out;
  if (XGetSelectionOwner(x_display_, selection_name) == x_window_) {
    // We can local fastpath and return the list of local targets.
    SelectionFormatMap* format_map = LookupStorageForAtom(selection_name);
    DCHECK(format_map);

    for (SelectionFormatMap::const_iterator it = format_map->begin();
         it != format_map->end(); ++it) {
      out.push_back(it->first);
    }
  } else {
    unsigned char* data = NULL;
    size_t out_data_items = 0;
    ::Atom out_type = None;

    SelectionRequestor* receiver = GetSelectionRequestorForBuffer(buffer);
    if (receiver->PerformBlockingConvertSelection(atom_cache_.GetAtom(kTargets),
                                                  &data,
                                                  NULL,
                                                  &out_data_items,
                                                  &out_type)) {
      ::Atom* atom_array = reinterpret_cast< ::Atom*>(data);
      for (size_t i = 0; i < out_data_items; ++i)
        out.push_back(atom_array[i]);

      XFree(data);
    } else {
      // There was no target list. Most Java apps doesn't offer a TARGETS list,
      // even though they AWT to. They will offer individual text types if you
      // ask. If this is the case we attempt to make sense of the contents as
      // text. This is pretty unfortunate since it means we have to actually
      // copy the data to see if it is available, but at least this path
      // shouldn't be hit for conforming programs.
      std::vector< ::Atom> types = GetTextAtoms();
      for (std::vector< ::Atom>::const_iterator it = types.begin();
           it != types.end(); ++it) {
        ::Atom type = None;
        if (receiver->PerformBlockingConvertSelection(*it,
                                                      NULL,
                                                      NULL,
                                                      NULL,
                                                      &type) &&
            type == *it) {
          out.push_back(*it);
        }
      }
    }
  }

  return TargetList(out, &atom_cache_);
}

std::vector< ::Atom> Clipboard::AuraX11Details::GetTextAtoms() const {
  return GetTextAtomsFrom(&atom_cache_);
}

std::vector< ::Atom> Clipboard::AuraX11Details::GetAtomsForFormat(
    const Clipboard::FormatType& format) {
  std::vector< ::Atom> atoms;
  atoms.push_back(atom_cache_.GetAtom(format.ToString().c_str()));
  return atoms;
}

void Clipboard::AuraX11Details::Clear(Buffer buffer) {
  if (buffer == BUFFER_STANDARD)
    return clipboard_owner_.Clear();
  else
    return primary_owner_.Clear();
}

bool Clipboard::AuraX11Details::Dispatch(const base::NativeEvent& event) {
  XEvent* xev = event;

  switch (xev->type) {
    case SelectionRequest: {
      if (xev->xselectionrequest.selection == XA_PRIMARY)
        primary_owner_.OnSelectionRequest(xev->xselectionrequest);
      else
        clipboard_owner_.OnSelectionRequest(xev->xselectionrequest);
      break;
    }
    case SelectionNotify: {
      if (xev->xselection.selection == XA_PRIMARY)
        primary_requestor_.OnSelectionNotify(xev->xselection);
      else
        clipboard_requestor_.OnSelectionNotify(xev->xselection);
      break;
    }
    case SelectionClear: {
      if (xev->xselectionclear.selection == XA_PRIMARY)
        primary_owner_.OnSelectionClear(xev->xselectionclear);
      else
        clipboard_owner_.OnSelectionClear(xev->xselectionclear);
      break;
    }
    default:
      break;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Clipboard

Clipboard::Clipboard()
    : aurax11_details_(new AuraX11Details) {
  DCHECK(CalledOnValidThread());
}

Clipboard::~Clipboard() {
  DCHECK(CalledOnValidThread());

  // TODO(erg): We need to do whatever the equivalent of
  // gtk_clipboard_store(clipboard_) is here. When we shut down, we want the
  // current selection to live on.
}

void Clipboard::WriteObjectsImpl(Buffer buffer,
                                 const ObjectMap& objects,
                                 SourceTag tag) {
  DCHECK(CalledOnValidThread());
  DCHECK(IsValidBuffer(buffer));

  aurax11_details_->CreateNewClipboardData();
  for (ObjectMap::const_iterator iter = objects.begin();
       iter != objects.end(); ++iter) {
    DispatchObject(static_cast<ObjectType>(iter->first), iter->second);
  }
  WriteSourceTag(tag);
  aurax11_details_->TakeOwnershipOfSelection(buffer);

  if (buffer == BUFFER_STANDARD) {
    ObjectMap::const_iterator text_iter = objects.find(CBF_TEXT);
    if (text_iter != objects.end()) {
      aurax11_details_->CreateNewClipboardData();
      const ObjectMapParam& char_vector = text_iter->second[0];
      WriteText(&char_vector.front(), char_vector.size());
      WriteSourceTag(tag);
      aurax11_details_->TakeOwnershipOfSelection(BUFFER_SELECTION);
    }
  }
}

bool Clipboard::IsFormatAvailable(const FormatType& format,
                                  Buffer buffer) const {
  DCHECK(CalledOnValidThread());
  DCHECK(IsValidBuffer(buffer));

  TargetList target_list =
      aurax11_details_->WaitAndGetTargetsList(buffer);
  return target_list.ContainsFormat(format);
}

void Clipboard::Clear(Buffer buffer) {
  DCHECK(CalledOnValidThread());
  DCHECK(IsValidBuffer(buffer));
  aurax11_details_->Clear(buffer);
}

void Clipboard::ReadAvailableTypes(Buffer buffer, std::vector<string16>* types,
    bool* contains_filenames) const {
  DCHECK(CalledOnValidThread());
  if (!types || !contains_filenames) {
    NOTREACHED();
    return;
  }

  TargetList target_list =
      aurax11_details_->WaitAndGetTargetsList(buffer);

  types->clear();

  if (target_list.ContainsText())
    types->push_back(UTF8ToUTF16(kMimeTypeText));
  if (target_list.ContainsFormat(GetHtmlFormatType()))
    types->push_back(UTF8ToUTF16(kMimeTypeHTML));
  if (target_list.ContainsFormat(GetRtfFormatType()))
    types->push_back(UTF8ToUTF16(kMimeTypeRTF));
  if (target_list.ContainsFormat(GetBitmapFormatType()))
    types->push_back(UTF8ToUTF16(kMimeTypePNG));
  *contains_filenames = false;

  scoped_ptr<SelectionData> data(aurax11_details_->RequestAndWaitForTypes(
      buffer,
      aurax11_details_->GetAtomsForFormat(GetWebCustomDataFormatType())));
  if (!data.get())
    return;

  ReadCustomDataTypes(data->data(), data->size(), types);
}

void Clipboard::ReadText(Buffer buffer, string16* result) const {
  DCHECK(CalledOnValidThread());

  scoped_ptr<SelectionData> data(aurax11_details_->RequestAndWaitForTypes(
      buffer, aurax11_details_->GetTextAtoms()));
  if (data.get()) {
    std::string text = data->GetText();
    *result = UTF8ToUTF16(text);
  }
}

void Clipboard::ReadAsciiText(Buffer buffer, std::string* result) const {
  DCHECK(CalledOnValidThread());

  scoped_ptr<SelectionData> data(aurax11_details_->RequestAndWaitForTypes(
      buffer, aurax11_details_->GetTextAtoms()));
  if (data.get())
    result->assign(data->GetText());
}

// TODO(estade): handle different charsets.
// TODO(port): set *src_url.
void Clipboard::ReadHTML(Buffer buffer,
                         string16* markup,
                         std::string* src_url,
                         uint32* fragment_start,
                         uint32* fragment_end) const {
  DCHECK(CalledOnValidThread());
  markup->clear();
  if (src_url)
    src_url->clear();
  *fragment_start = 0;
  *fragment_end = 0;

  scoped_ptr<SelectionData> data(aurax11_details_->RequestAndWaitForTypes(
      buffer, aurax11_details_->GetAtomsForFormat(GetHtmlFormatType())));
  if (!data.get())
    return;

  *markup = data->GetHtml();

  *fragment_start = 0;
  DCHECK(markup->length() <= kuint32max);
  *fragment_end = static_cast<uint32>(markup->length());
}

void Clipboard::ReadRTF(Buffer buffer, std::string* result) const {
  DCHECK(CalledOnValidThread());

  scoped_ptr<SelectionData> data(aurax11_details_->RequestAndWaitForTypes(
      buffer, aurax11_details_->GetAtomsForFormat(GetRtfFormatType())));
  if (data.get())
    data->AssignTo(result);
}

SkBitmap Clipboard::ReadImage(Buffer buffer) const {
  DCHECK(CalledOnValidThread());
  NOTIMPLEMENTED();
  return SkBitmap();
}

void Clipboard::ReadCustomData(Buffer buffer,
                               const string16& type,
                               string16* result) const {
  DCHECK(CalledOnValidThread());

  scoped_ptr<SelectionData> data(aurax11_details_->RequestAndWaitForTypes(
      buffer,
      aurax11_details_->GetAtomsForFormat(GetWebCustomDataFormatType())));
  if (!data.get())
    return;

  ReadCustomDataForType(data->data(), data->size(), type, result);
}

void Clipboard::ReadBookmark(string16* title, std::string* url) const {
  DCHECK(CalledOnValidThread());
  // TODO(erg): This was left NOTIMPLEMENTED() in the gtk port too.
  NOTIMPLEMENTED();
}

void Clipboard::ReadData(const FormatType& format, std::string* result) const {
  ReadDataImpl(BUFFER_STANDARD, format, result);
}

void Clipboard::ReadDataImpl(Buffer buffer,
                             const FormatType& format,
                             std::string* result) const {
  DCHECK(CalledOnValidThread());

  scoped_ptr<SelectionData> data(aurax11_details_->RequestAndWaitForTypes(
      buffer, aurax11_details_->GetAtomsForFormat(format)));
  if (data.get())
    data->AssignTo(result);
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

void Clipboard::WriteText(const char* text_data, size_t text_len) {
  char* data = new char[text_len];
  memcpy(data, text_data, text_len);

  aurax11_details_->InsertMapping(kMimeTypeText, data, text_len);
  aurax11_details_->InsertMapping(kText, data, text_len);
  aurax11_details_->InsertMapping(kString, data, text_len);
  aurax11_details_->InsertMapping(kUtf8String, data, text_len);
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

  aurax11_details_->InsertMapping(kMimeTypeHTML, data, total_len);
}

void Clipboard::WriteRTF(const char* rtf_data, size_t data_len) {
  WriteData(GetRtfFormatType(), rtf_data, data_len);
}

void Clipboard::WriteBookmark(const char* title_data,
                              size_t title_len,
                              const char* url_data,
                              size_t url_len) {
  // Write as a mozilla url (UTF16: URL, newline, title).
  string16 url = UTF8ToUTF16(std::string(url_data, url_len) + "\n");
  string16 title = UTF8ToUTF16(std::string(title_data, title_len));
  int data_len = 2 * (title.length() + url.length());

  char* data = new char[data_len];
  memcpy(data, url.data(), 2 * url.length());
  memcpy(data + 2 * url.length(), title.data(), 2 * title.length());
  aurax11_details_->InsertMapping(kMimeTypeMozillaURL, data, data_len);
}

// Write an extra flavor that signifies WebKit was the last to modify the
// pasteboard. This flavor has no data.
void Clipboard::WriteWebSmartPaste() {
  aurax11_details_->InsertMapping(kMimeTypeWebkitSmartPaste, NULL, 0);
}

void Clipboard::WriteBitmap(const char* pixel_data, const char* size_data) {
  // TODO(erg): I'm not sure if we should be writting BMP data here or
  // not. It's what the GTK port does, but I'm not sure it's the right thing to
  // do.
  NOTIMPLEMENTED();
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
  aurax11_details_->InsertMapping(format.ToString(), data, data_len);
}

void Clipboard::WriteSourceTag(SourceTag tag) {
  if (tag != SourceTag()) {
    ObjectMapParam binary = SourceTag2Binary(tag);
    WriteData(GetSourceTagFormatType(), &binary[0], binary.size());
  }
}

// static
Clipboard::FormatType Clipboard::GetFormatType(
    const std::string& format_string) {
  return FormatType::Deserialize(format_string);
}

// static
const Clipboard::FormatType& Clipboard::GetUrlFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kMimeTypeURIList));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetUrlWFormatType() {
  return GetUrlFormatType();
}

// static
const Clipboard::FormatType& Clipboard::GetPlainTextFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kMimeTypeText));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetPlainTextWFormatType() {
  return GetPlainTextFormatType();
}

// static
const Clipboard::FormatType& Clipboard::GetFilenameFormatType() {
  CR_DEFINE_STATIC_LOCAL(FormatType, type, (kMimeTypeFilename));
  return type;
}

// static
const Clipboard::FormatType& Clipboard::GetFilenameWFormatType() {
  return Clipboard::GetFilenameFormatType();
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

}  // namespace ui
