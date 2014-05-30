// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/dragdrop/os_exchange_data_provider_win.h"

#include <algorithm>
#include <vector>

#include "base/files/file_path.h"
#include "base/i18n/file_util_icu.h"
#include "base/logging.h"
#include "base/memory/scoped_handle.h"
#include "base/pickle.h"
#include "base/stl_util.h"
#include "base/utf_string_conversions.h"
#include "base/win/scoped_hglobal.h"
#include "googleurl/src/gurl.h"
#include "grit/ui_strings.h"
#include "net/base/net_util.h"
#include "ui/base/clipboard/clipboard_util_win.h"
#include "ui/base/l10n/l10n_util.h"

namespace ui {

// Creates a new STGMEDIUM object to hold the specified text. The caller
// owns the resulting object. The "Bytes" version does not NULL terminate, the
// string version does.
static STGMEDIUM* GetStorageForBytes(const char* data, size_t bytes);
static STGMEDIUM* GetStorageForString16(const string16& data);
static STGMEDIUM* GetStorageForString(const std::string& data);
// Creates the contents of an Internet Shortcut file for the given URL.
static void GetInternetShortcutFileContents(const GURL& url, std::string* data);
// Creates a valid file name given a suggested title and URL.
static void CreateValidFileNameFromTitle(const GURL& url,
                                         const string16& title,
                                         string16* validated);
// Creates a new STGMEDIUM object to hold a file.
static STGMEDIUM* GetStorageForFileName(const base::FilePath& path);
// Creates a File Descriptor for the creation of a file to the given URL and
// returns a handle to it.
static STGMEDIUM* GetStorageForFileDescriptor(const base::FilePath& path);

///////////////////////////////////////////////////////////////////////////////
// FormatEtcEnumerator

//
// This object implements an enumeration interface. The existence of an
// implementation of this interface is exposed to clients through
// OSExchangeData's EnumFormatEtc method. Our implementation is nobody's
// business but our own, so it lives in this file.
//
// This Windows API is truly a gem. It wants to be an enumerator but assumes
// some sort of sequential data (why not just use an array?). See comments
// throughout.
//
class FormatEtcEnumerator : public IEnumFORMATETC {
 public:
  FormatEtcEnumerator(DataObjectImpl::StoredData::const_iterator begin,
                      DataObjectImpl::StoredData::const_iterator end);
  ~FormatEtcEnumerator();

  // IEnumFORMATETC implementation:
  HRESULT __stdcall Next(
      ULONG count, FORMATETC* elements_array, ULONG* elements_fetched);
  HRESULT __stdcall Skip(ULONG skip_count);
  HRESULT __stdcall Reset();
  HRESULT __stdcall Clone(IEnumFORMATETC** clone);

  // IUnknown implementation:
  HRESULT __stdcall QueryInterface(const IID& iid, void** object);
  ULONG __stdcall AddRef();
  ULONG __stdcall Release();

 private:
  // This can only be called from |CloneFromOther|, since it initializes the
  // contents_ from the other enumerator's contents.
  FormatEtcEnumerator() : cursor_(0), ref_count_(0) {
  }

  // Clone a new FormatEtc from another instance of this enumeration.
  static FormatEtcEnumerator* CloneFromOther(const FormatEtcEnumerator* other);

 private:
  // We are _forced_ to use a vector as our internal data model as Windows'
  // retarded IEnumFORMATETC API assumes a deterministic ordering of elements
  // through methods like Next and Skip. This exposes the underlying data
  // structure to the user. Bah.
  std::vector<FORMATETC*> contents_;

  // The cursor of the active enumeration - an index into |contents_|.
  size_t cursor_;

  LONG ref_count_;

  DISALLOW_COPY_AND_ASSIGN(FormatEtcEnumerator);
};

// Safely makes a copy of all of the relevant bits of a FORMATETC object.
static void CloneFormatEtc(FORMATETC* source, FORMATETC* clone) {
  *clone = *source;
  if (source->ptd) {
    source->ptd =
        static_cast<DVTARGETDEVICE*>(CoTaskMemAlloc(sizeof(DVTARGETDEVICE)));
    *(clone->ptd) = *(source->ptd);
  }
}

FormatEtcEnumerator::FormatEtcEnumerator(
    DataObjectImpl::StoredData::const_iterator start,
    DataObjectImpl::StoredData::const_iterator end)
    : ref_count_(0), cursor_(0) {
  // Copy FORMATETC data from our source into ourselves.
  while (start != end) {
    FORMATETC* format_etc = new FORMATETC;
    CloneFormatEtc(&(*start)->format_etc, format_etc);
    contents_.push_back(format_etc);
    ++start;
  }
}

FormatEtcEnumerator::~FormatEtcEnumerator() {
  STLDeleteContainerPointers(contents_.begin(), contents_.end());
}

STDMETHODIMP FormatEtcEnumerator::Next(
    ULONG count, FORMATETC* elements_array, ULONG* elements_fetched) {
  // MSDN says |elements_fetched| is allowed to be NULL if count is 1.
  if (!elements_fetched)
    DCHECK_EQ(count, 1ul);

  // This method copies count elements into |elements_array|.
  ULONG index = 0;
  while (cursor_ < contents_.size() && index < count) {
    CloneFormatEtc(contents_[cursor_], &elements_array[index]);
    ++cursor_;
    ++index;
  }
  // The out param is for how many we actually copied.
  if (elements_fetched)
    *elements_fetched = index;

  // If the two don't agree, then we fail.
  return index == count ? S_OK : S_FALSE;
}

STDMETHODIMP FormatEtcEnumerator::Skip(ULONG skip_count) {
  cursor_ += skip_count;
  // MSDN implies it's OK to leave the enumerator trashed.
  // "Whatever you say, boss"
  return cursor_ <= contents_.size() ? S_OK : S_FALSE;
}

STDMETHODIMP FormatEtcEnumerator::Reset() {
  cursor_ = 0;
  return S_OK;
}

STDMETHODIMP FormatEtcEnumerator::Clone(IEnumFORMATETC** clone) {
  // Clone the current enumerator in its exact state, including cursor.
  FormatEtcEnumerator* e = CloneFromOther(this);
  e->AddRef();
  *clone = e;
  return S_OK;
}

STDMETHODIMP FormatEtcEnumerator::QueryInterface(const IID& iid,
                                                 void** object) {
  *object = NULL;
  if (IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IEnumFORMATETC)) {
    *object = this;
  } else {
    return E_NOINTERFACE;
  }
  AddRef();
  return S_OK;
}

ULONG FormatEtcEnumerator::AddRef() {
  return InterlockedIncrement(&ref_count_);
}

ULONG FormatEtcEnumerator::Release() {
  if (InterlockedDecrement(&ref_count_) == 0) {
    ULONG copied_refcnt = ref_count_;
    delete this;
    return copied_refcnt;
  }
  return ref_count_;
}

// static
FormatEtcEnumerator* FormatEtcEnumerator::CloneFromOther(
    const FormatEtcEnumerator* other) {
  FormatEtcEnumerator* e = new FormatEtcEnumerator;
  // Copy FORMATETC data from our source into ourselves.
  std::vector<FORMATETC*>::const_iterator start = other->contents_.begin();
  while (start != other->contents_.end()) {
    FORMATETC* format_etc = new FORMATETC;
    CloneFormatEtc(*start, format_etc);
    e->contents_.push_back(format_etc);
    ++start;
  }
  // Carry over
  e->cursor_ = other->cursor_;
  return e;
}

///////////////////////////////////////////////////////////////////////////////
// OSExchangeDataProviderWin, public:

// static
bool OSExchangeDataProviderWin::HasPlainTextURL(IDataObject* source) {
  string16 plain_text;
  return (ClipboardUtil::GetPlainText(source, &plain_text) &&
          !plain_text.empty() && GURL(plain_text).is_valid());
}

// static
bool OSExchangeDataProviderWin::GetPlainTextURL(IDataObject* source,
                                                GURL* url) {
  string16 plain_text;
  if (ClipboardUtil::GetPlainText(source, &plain_text) &&
      !plain_text.empty()) {
    GURL gurl(plain_text);
    if (gurl.is_valid()) {
      *url = gurl;
      return true;
    }
  }
  return false;
}

// static
DataObjectImpl* OSExchangeDataProviderWin::GetDataObjectImpl(
    const OSExchangeData& data) {
  return static_cast<const OSExchangeDataProviderWin*>(&data.provider())->
      data_.get();
}

// static
IDataObject* OSExchangeDataProviderWin::GetIDataObject(
    const OSExchangeData& data) {
  return static_cast<const OSExchangeDataProviderWin*>(&data.provider())->
      data_object();
}

// static
IDataObjectAsyncCapability* OSExchangeDataProviderWin::GetIAsyncOperation(
    const OSExchangeData& data) {
  return static_cast<const OSExchangeDataProviderWin*>(&data.provider())->
      async_operation();
}

OSExchangeDataProviderWin::OSExchangeDataProviderWin(IDataObject* source)
    : data_(new DataObjectImpl()),
      source_object_(source) {
}

OSExchangeDataProviderWin::OSExchangeDataProviderWin()
    : data_(new DataObjectImpl()),
      source_object_(data_.get()) {
}

OSExchangeDataProviderWin::~OSExchangeDataProviderWin() {
}

void OSExchangeDataProviderWin::SetString(const string16& data) {
  STGMEDIUM* storage = GetStorageForString16(data);
  data_->contents_.push_back(
      new DataObjectImpl::StoredDataInfo(CF_UNICODETEXT, storage));

  // Also add plain text.
  storage = GetStorageForString(UTF16ToUTF8(data));
  data_->contents_.push_back(
      new DataObjectImpl::StoredDataInfo(CF_TEXT, storage));
}

void OSExchangeDataProviderWin::SetURL(const GURL& url,
                                       const string16& title) {
  // NOTE WELL:
  // Every time you change the order of the first two CLIPFORMATS that get
  // added here, you need to update the EnumerationViaCOM test case in
  // the _unittest.cc file to reflect the new arrangement otherwise that test
  // will fail! It assumes an insertion order.

  // Add text/x-moz-url for drags from Firefox
  string16 x_moz_url_str = UTF8ToUTF16(url.spec());
  x_moz_url_str += '\n';
  x_moz_url_str += title;
  STGMEDIUM* storage = GetStorageForString16(x_moz_url_str);
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      ClipboardUtil::GetMozUrlFormat()->cfFormat, storage));

  // Add a .URL shortcut file for dragging to Explorer.
  string16 valid_file_name;
  CreateValidFileNameFromTitle(url, title, &valid_file_name);
  std::string shortcut_url_file_contents;
  GetInternetShortcutFileContents(url, &shortcut_url_file_contents);
  SetFileContents(base::FilePath(valid_file_name), shortcut_url_file_contents);

  // Add a UniformResourceLocator link for apps like IE and Word.
  storage = GetStorageForString16(UTF8ToUTF16(url.spec()));
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      ClipboardUtil::GetUrlWFormat()->cfFormat, storage));
  storage = GetStorageForString(url.spec());
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      ClipboardUtil::GetUrlFormat()->cfFormat, storage));

  // TODO(beng): add CF_HTML.
  // http://code.google.com/p/chromium/issues/detail?id=6767

  // Also add text representations (these should be last since they're the
  // least preferable).
  storage = GetStorageForString16(UTF8ToUTF16(url.spec()));
  data_->contents_.push_back(
      new DataObjectImpl::StoredDataInfo(CF_UNICODETEXT, storage));
  storage = GetStorageForString(url.spec());
  data_->contents_.push_back(
      new DataObjectImpl::StoredDataInfo(CF_TEXT, storage));
}

void OSExchangeDataProviderWin::SetFilename(const base::FilePath& path) {
  STGMEDIUM* storage = GetStorageForFileName(path);
  DataObjectImpl::StoredDataInfo* info =
      new DataObjectImpl::StoredDataInfo(CF_HDROP, storage);
  data_->contents_.push_back(info);
}

void OSExchangeDataProviderWin::SetFilenames(
    const std::vector<OSExchangeData::FileInfo>& filenames) {
  for (size_t i = 0; i < filenames.size(); ++i) {
    STGMEDIUM* storage = GetStorageForFileName(filenames[i].path);
    DataObjectImpl::StoredDataInfo* info =
        new DataObjectImpl::StoredDataInfo(CF_HDROP, storage);
    data_->contents_.push_back(info);
  }
}

void OSExchangeDataProviderWin::SetPickledData(CLIPFORMAT format,
                                               const Pickle& data) {
  STGMEDIUM* storage = GetStorageForBytes(static_cast<const char*>(data.data()),
                                          data.size());
  data_->contents_.push_back(
      new DataObjectImpl::StoredDataInfo(format, storage));
}

void OSExchangeDataProviderWin::SetFileContents(
    const base::FilePath& filename,
    const std::string& file_contents) {
  // Add CFSTR_FILEDESCRIPTOR
  STGMEDIUM* storage = GetStorageForFileDescriptor(filename);
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      ClipboardUtil::GetFileDescriptorFormat()->cfFormat, storage));

  // Add CFSTR_FILECONTENTS
  storage = GetStorageForBytes(file_contents.data(), file_contents.length());
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      ClipboardUtil::GetFileContentFormatZero(), storage));
}

void OSExchangeDataProviderWin::SetHtml(const string16& html,
                                        const GURL& base_url) {
  // Add both MS CF_HTML and text/html format.  CF_HTML should be in utf-8.
  std::string utf8_html = UTF16ToUTF8(html);
  std::string url = base_url.is_valid() ? base_url.spec() : std::string();

  std::string cf_html = ClipboardUtil::HtmlToCFHtml(utf8_html, url);
  STGMEDIUM* storage = GetStorageForBytes(cf_html.c_str(), cf_html.size());
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      ClipboardUtil::GetHtmlFormat()->cfFormat, storage));

  STGMEDIUM* storage_plain = GetStorageForBytes(utf8_html.c_str(),
                                                utf8_html.size());
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      ClipboardUtil::GetTextHtmlFormat()->cfFormat, storage_plain));
}

bool OSExchangeDataProviderWin::GetString(string16* data) const {
  return ClipboardUtil::GetPlainText(source_object_, data);
}

bool OSExchangeDataProviderWin::GetURLAndTitle(GURL* url,
                                               string16* title) const {
  string16 url_str;
  bool success = ClipboardUtil::GetUrl(source_object_, &url_str, title, true);
  if (success) {
    GURL test_url(url_str);
    if (test_url.is_valid()) {
      *url = test_url;
      return true;
    }
  } else if (GetPlainTextURL(source_object_, url)) {
    if (url->is_valid())
      *title = net::GetSuggestedFilename(*url, "", "", "", "", std::string());
    else
      title->clear();
    return true;
  }
  return false;
}

bool OSExchangeDataProviderWin::GetFilename(base::FilePath* path) const {
  std::vector<string16> filenames;
  bool success = ClipboardUtil::GetFilenames(source_object_, &filenames);
  if (success)
    *path = base::FilePath(filenames[0]);
  return success;
}

bool OSExchangeDataProviderWin::GetFilenames(
    std::vector<OSExchangeData::FileInfo>* filenames) const {
  std::vector<string16> filenames_local;
  bool success = ClipboardUtil::GetFilenames(source_object_, &filenames_local);
  if (success) {
    for (size_t i = 0; i < filenames_local.size(); ++i)
      filenames->push_back(
          OSExchangeData::FileInfo(base::FilePath(filenames_local[i]),
                                   base::FilePath()));
  }
  return success;
}

bool OSExchangeDataProviderWin::GetPickledData(CLIPFORMAT format,
                                               Pickle* data) const {
  DCHECK(data);
  FORMATETC format_etc =
      { format, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
  bool success = false;
  STGMEDIUM medium;
  if (SUCCEEDED(source_object_->GetData(&format_etc, &medium))) {
    if (medium.tymed & TYMED_HGLOBAL) {
      base::win::ScopedHGlobal<char> c_data(medium.hGlobal);
      DCHECK_GT(c_data.Size(), 0u);
      *data = Pickle(c_data.get(), static_cast<int>(c_data.Size()));
      success = true;
    }
    ReleaseStgMedium(&medium);
  }
  return success;
}

bool OSExchangeDataProviderWin::GetFileContents(
    base::FilePath* filename,
    std::string* file_contents) const {
  string16 filename_str;
  if (!ClipboardUtil::GetFileContents(source_object_, &filename_str,
                                      file_contents)) {
    return false;
  }
  *filename = base::FilePath(filename_str);
  return true;
}

bool OSExchangeDataProviderWin::GetHtml(string16* html,
                                        GURL* base_url) const {
  std::string url;
  bool success = ClipboardUtil::GetHtml(source_object_, html, &url);
  if (success)
    *base_url = GURL(url);
  return success;
}

bool OSExchangeDataProviderWin::HasString() const {
  return ClipboardUtil::HasPlainText(source_object_);
}

bool OSExchangeDataProviderWin::HasURL() const {
  return (ClipboardUtil::HasUrl(source_object_) ||
          HasPlainTextURL(source_object_));
}

bool OSExchangeDataProviderWin::HasFile() const {
  return ClipboardUtil::HasFilenames(source_object_);
}

bool OSExchangeDataProviderWin::HasFileContents() const {
  return ClipboardUtil::HasFileContents(source_object_);
}

bool OSExchangeDataProviderWin::HasHtml() const {
  return ClipboardUtil::HasHtml(source_object_);
}

bool OSExchangeDataProviderWin::HasCustomFormat(CLIPFORMAT format) const {
  FORMATETC format_etc =
      { format, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
  return (source_object_->QueryGetData(&format_etc) == S_OK);
}

void OSExchangeDataProviderWin::SetDownloadFileInfo(
    const OSExchangeData::DownloadFileInfo& download) {
  // If the filename is not provided, set storage to NULL to indicate that
  // the delay rendering will be used.
  // TODO(dcheng): Is it actually possible for filename to be empty here? I
  // think we always synthesize one in WebContentsDragWin.
  STGMEDIUM* storage = NULL;
  if (!download.filename.empty())
    storage = GetStorageForFileName(download.filename);

  // Add CF_HDROP.
  DataObjectImpl::StoredDataInfo* info = new DataObjectImpl::StoredDataInfo(
      ClipboardUtil::GetCFHDropFormat()->cfFormat, storage);
  info->downloader = download.downloader;
  data_->contents_.push_back(info);
}

void OSExchangeDataProviderWin::SetInDragLoop(bool in_drag_loop) {
  data_->set_in_drag_loop(in_drag_loop);
}

#if defined(USE_AURA)

void OSExchangeDataProviderWin::SetDragImage(
    const gfx::ImageSkia& image,
    const gfx::Vector2d& cursor_offset) {
  drag_image_ = image;
  drag_image_offset_ = cursor_offset;
}

const gfx::ImageSkia& OSExchangeDataProviderWin::GetDragImage() const {
  return drag_image_;
}

const gfx::Vector2d& OSExchangeDataProviderWin::GetDragImageOffset() const {
  return drag_image_offset_;
}

#endif

///////////////////////////////////////////////////////////////////////////////
// DataObjectImpl, IDataObject implementation:

// The following function, DuplicateMedium, is derived from WCDataObject.cpp
// in the WebKit source code. This is the license information for the file:
/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
static void DuplicateMedium(CLIPFORMAT source_clipformat,
                            STGMEDIUM* source,
                            STGMEDIUM* destination) {
  switch (source->tymed) {
    case TYMED_HGLOBAL:
      destination->hGlobal =
          static_cast<HGLOBAL>(OleDuplicateData(
              source->hGlobal, source_clipformat, 0));
      break;
    case TYMED_MFPICT:
      destination->hMetaFilePict =
          static_cast<HMETAFILEPICT>(OleDuplicateData(
              source->hMetaFilePict, source_clipformat, 0));
      break;
    case TYMED_GDI:
      destination->hBitmap =
          static_cast<HBITMAP>(OleDuplicateData(
              source->hBitmap, source_clipformat, 0));
      break;
    case TYMED_ENHMF:
      destination->hEnhMetaFile =
          static_cast<HENHMETAFILE>(OleDuplicateData(
              source->hEnhMetaFile, source_clipformat, 0));
      break;
    case TYMED_FILE:
      destination->lpszFileName =
          static_cast<LPOLESTR>(OleDuplicateData(
              source->lpszFileName, source_clipformat, 0));
      break;
    case TYMED_ISTREAM:
      destination->pstm = source->pstm;
      destination->pstm->AddRef();
      break;
    case TYMED_ISTORAGE:
      destination->pstg = source->pstg;
      destination->pstg->AddRef();
      break;
  }

  destination->tymed = source->tymed;
  destination->pUnkForRelease = source->pUnkForRelease;
  if (destination->pUnkForRelease)
    destination->pUnkForRelease->AddRef();
}

DataObjectImpl::DataObjectImpl()
    : is_aborting_(false),
      in_drag_loop_(false),
      in_async_mode_(false),
      async_operation_started_(false),
      observer_(NULL) {
}

DataObjectImpl::~DataObjectImpl() {
  StopDownloads();
  STLDeleteContainerPointers(contents_.begin(), contents_.end());
  if (observer_)
    observer_->OnDataObjectDisposed();
}

void DataObjectImpl::StopDownloads() {
  for (StoredData::iterator iter = contents_.begin();
       iter != contents_.end(); ++iter) {
    if ((*iter)->downloader.get()) {
      (*iter)->downloader->Stop();
      (*iter)->downloader = 0;
    }
  }
}

void DataObjectImpl::RemoveData(const FORMATETC& format) {
  if (format.ptd)
    return;  // Don't attempt to compare target devices.

  for (StoredData::iterator i = contents_.begin(); i != contents_.end(); ++i) {
    if (!(*i)->format_etc.ptd &&
        format.cfFormat == (*i)->format_etc.cfFormat &&
        format.dwAspect == (*i)->format_etc.dwAspect &&
        format.lindex == (*i)->format_etc.lindex &&
        format.tymed == (*i)->format_etc.tymed) {
      delete *i;
      contents_.erase(i);
      return;
    }
  }
}

void DataObjectImpl::OnDownloadCompleted(const base::FilePath& file_path) {
  CLIPFORMAT hdrop_format = ClipboardUtil::GetCFHDropFormat()->cfFormat;
  DataObjectImpl::StoredData::iterator iter = contents_.begin();
  for (; iter != contents_.end(); ++iter) {
    if ((*iter)->format_etc.cfFormat == hdrop_format) {
      // Release the old storage.
      if ((*iter)->owns_medium) {
        ReleaseStgMedium((*iter)->medium);
        delete (*iter)->medium;
      }

      // Update the storage.
      (*iter)->owns_medium = true;
      (*iter)->medium = GetStorageForFileName(file_path);

      break;
    }
  }
  DCHECK(iter != contents_.end());
}

void DataObjectImpl::OnDownloadAborted() {
}

HRESULT DataObjectImpl::GetData(FORMATETC* format_etc, STGMEDIUM* medium) {
  if (is_aborting_)
    return DV_E_FORMATETC;

  StoredData::iterator iter = contents_.begin();
  while (iter != contents_.end()) {
    if ((*iter)->format_etc.cfFormat == format_etc->cfFormat &&
        (*iter)->format_etc.lindex == format_etc->lindex &&
        ((*iter)->format_etc.tymed & format_etc->tymed)) {
      // If medium is NULL, delay-rendering will be used.
      if ((*iter)->medium) {
        DuplicateMedium((*iter)->format_etc.cfFormat, (*iter)->medium, medium);
      } else {
        // Fail all GetData() attempts for DownloadURL data if the drag and drop
        // operation is still in progress.
        if (in_drag_loop_)
          return DV_E_FORMATETC;

        bool wait_for_data = false;

        // In async mode, we do not want to start waiting for the data before
        // the async operation is started. This is because we want to postpone
        // until Shell kicks off a background thread to do the work so that
        // we do not block the UI thread.
        if (!in_async_mode_ || async_operation_started_)
          wait_for_data = true;

        if (!wait_for_data)
          return DV_E_FORMATETC;

        // Notify the observer we start waiting for the data. This gives
        // an observer a chance to end the drag and drop.
        if (observer_)
          observer_->OnWaitForData();

        // Now we can start the download.
        if ((*iter)->downloader.get()) {
          (*iter)->downloader->Start(this);
          if (!(*iter)->downloader->Wait()) {
            is_aborting_ = true;
            return DV_E_FORMATETC;
          }
        }

        // The stored data should have been updated with the final version.
        // So we just need to call this function again to retrieve it.
        return GetData(format_etc, medium);
      }
      return S_OK;
    }
    ++iter;
  }

  return DV_E_FORMATETC;
}

HRESULT DataObjectImpl::GetDataHere(FORMATETC* format_etc,
                                    STGMEDIUM* medium) {
  return DATA_E_FORMATETC;
}

HRESULT DataObjectImpl::QueryGetData(FORMATETC* format_etc) {
  StoredData::const_iterator iter = contents_.begin();
  while (iter != contents_.end()) {
    if ((*iter)->format_etc.cfFormat == format_etc->cfFormat)
      return S_OK;
    ++iter;
  }
  return DV_E_FORMATETC;
}

HRESULT DataObjectImpl::GetCanonicalFormatEtc(
    FORMATETC* format_etc, FORMATETC* result) {
  format_etc->ptd = NULL;
  return E_NOTIMPL;
}

HRESULT DataObjectImpl::SetData(
    FORMATETC* format_etc, STGMEDIUM* medium, BOOL should_release) {
  RemoveData(*format_etc);

  STGMEDIUM* local_medium = new STGMEDIUM;
  if (should_release) {
    *local_medium = *medium;
  } else {
    DuplicateMedium(format_etc->cfFormat, medium, local_medium);
  }

  DataObjectImpl::StoredDataInfo* info =
      new DataObjectImpl::StoredDataInfo(format_etc->cfFormat, local_medium);
  info->medium->tymed = format_etc->tymed;
  info->owns_medium = !!should_release;
  // Make newly added data appear first.
  // TODO(dcheng): Make various setters agree whether elements should be
  // prioritized from front to back or back to front.
  contents_.insert(contents_.begin(), info);

  return S_OK;
}

HRESULT DataObjectImpl::EnumFormatEtc(
    DWORD direction, IEnumFORMATETC** enumerator) {
  if (direction == DATADIR_GET) {
    FormatEtcEnumerator* e =
        new FormatEtcEnumerator(contents_.begin(), contents_.end());
    e->AddRef();
    *enumerator = e;
    return S_OK;
  }
  return E_NOTIMPL;
}

HRESULT DataObjectImpl::DAdvise(
    FORMATETC* format_etc, DWORD advf, IAdviseSink* sink, DWORD* connection) {
  return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT DataObjectImpl::DUnadvise(DWORD connection) {
  return OLE_E_ADVISENOTSUPPORTED;
}

HRESULT DataObjectImpl::EnumDAdvise(IEnumSTATDATA** enumerator) {
  return OLE_E_ADVISENOTSUPPORTED;
}

///////////////////////////////////////////////////////////////////////////////
// DataObjectImpl, IDataObjectAsyncCapability implementation:

HRESULT DataObjectImpl::EndOperation(
    HRESULT result, IBindCtx* reserved, DWORD effects) {
  async_operation_started_ = false;
  return S_OK;
}

HRESULT DataObjectImpl::GetAsyncMode(BOOL* is_op_async) {
  *is_op_async = in_async_mode_ ? TRUE : FALSE;
  return S_OK;
}

HRESULT DataObjectImpl::InOperation(BOOL* in_async_op) {
  *in_async_op = async_operation_started_ ? TRUE : FALSE;
  return S_OK;
}

HRESULT DataObjectImpl::SetAsyncMode(BOOL do_op_async) {
  in_async_mode_ = (do_op_async == TRUE);
  return S_OK;
}

HRESULT DataObjectImpl::StartOperation(IBindCtx* reserved) {
  async_operation_started_ = true;
  return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
// DataObjectImpl, IUnknown implementation:

HRESULT DataObjectImpl::QueryInterface(const IID& iid, void** object) {
  if (!object)
    return E_POINTER;
  if (IsEqualIID(iid, IID_IDataObject) || IsEqualIID(iid, IID_IUnknown)) {
    *object = static_cast<IDataObject*>(this);
  } else if (in_async_mode_ &&
             IsEqualIID(iid, __uuidof(IDataObjectAsyncCapability))) {
    *object = static_cast<IDataObjectAsyncCapability*>(this);
  } else {
    *object = NULL;
    return E_NOINTERFACE;
  }
  AddRef();
  return S_OK;
}

ULONG DataObjectImpl::AddRef() {
  base::RefCountedThreadSafe<DownloadFileObserver>::AddRef();
  return 0;
}

ULONG DataObjectImpl::Release() {
  base::RefCountedThreadSafe<DownloadFileObserver>::Release();
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
// DataObjectImpl, private:

static STGMEDIUM* GetStorageForBytes(const char* data, size_t bytes) {
  HANDLE handle = GlobalAlloc(GPTR, static_cast<int>(bytes));
  base::win::ScopedHGlobal<char> scoped(handle);
  size_t allocated = static_cast<size_t>(GlobalSize(handle));
  memcpy(scoped.get(), data, allocated);

  STGMEDIUM* storage = new STGMEDIUM;
  storage->hGlobal = handle;
  storage->tymed = TYMED_HGLOBAL;
  storage->pUnkForRelease = NULL;
  return storage;
}

template<class T>
static HGLOBAL CopyStringToGlobalHandle(const T& payload) {
  int bytes =
      static_cast<int>(payload.size() + 1) * sizeof(typename T::value_type);
  HANDLE handle = GlobalAlloc(GPTR, bytes);
  void* data = GlobalLock(handle);
  size_t allocated = static_cast<size_t>(GlobalSize(handle));
  memcpy(data, payload.c_str(), allocated);
  static_cast<typename T::value_type*>(data)[payload.size()] = '\0';
  GlobalUnlock(handle);
  return handle;
}

static STGMEDIUM* GetStorageForString16(const string16& data) {
  STGMEDIUM* storage = new STGMEDIUM;
  storage->hGlobal = CopyStringToGlobalHandle<string16>(data);
  storage->tymed = TYMED_HGLOBAL;
  storage->pUnkForRelease = NULL;
  return storage;
}

static STGMEDIUM* GetStorageForString(const std::string& data) {
  STGMEDIUM* storage = new STGMEDIUM;
  storage->hGlobal = CopyStringToGlobalHandle<std::string>(data);
  storage->tymed = TYMED_HGLOBAL;
  storage->pUnkForRelease = NULL;
  return storage;
}

static void GetInternetShortcutFileContents(const GURL& url,
                                            std::string* data) {
  DCHECK(data);
  static const std::string kInternetShortcutFileStart =
      "[InternetShortcut]\r\nURL=";
  static const std::string kInternetShortcutFileEnd =
      "\r\n";
  *data = kInternetShortcutFileStart + url.spec() + kInternetShortcutFileEnd;
}

static void CreateValidFileNameFromTitle(const GURL& url,
                                         const string16& title,
                                         string16* validated) {
  if (title.empty()) {
    if (url.is_valid()) {
      *validated = net::GetSuggestedFilename(url, "", "", "", "",
                                             std::string());
    } else {
      // Nothing else can be done, just use a default.
      *validated =
          l10n_util::GetStringUTF16(IDS_APP_UNTITLED_SHORTCUT_FILE_NAME);
    }
  } else {
    *validated = title;
    file_util::ReplaceIllegalCharactersInPath(validated, '-');
  }
  static const wchar_t extension[] = L".url";
  static const size_t max_length = MAX_PATH - arraysize(extension);
  if (validated->size() > max_length)
    validated->erase(max_length);
  *validated += extension;
}

static STGMEDIUM* GetStorageForFileName(const base::FilePath& path) {
  const size_t kDropSize = sizeof(DROPFILES);
  const size_t kTotalBytes =
      kDropSize + (path.value().length() + 2) * sizeof(wchar_t);
  HANDLE hdata = GlobalAlloc(GMEM_MOVEABLE, kTotalBytes);

  base::win::ScopedHGlobal<DROPFILES> locked_mem(hdata);
  DROPFILES* drop_files = locked_mem.get();
  drop_files->pFiles = sizeof(DROPFILES);
  drop_files->fWide = TRUE;
  wchar_t* data = reinterpret_cast<wchar_t*>(
      reinterpret_cast<BYTE*>(drop_files) + kDropSize);
  const size_t copy_size = (path.value().length() + 1) * sizeof(wchar_t);
  memcpy(data, path.value().c_str(), copy_size);
  data[path.value().length() + 1] = L'\0';  // Double NULL

  STGMEDIUM* storage = new STGMEDIUM;
  storage->tymed = TYMED_HGLOBAL;
  storage->hGlobal = hdata;
  storage->pUnkForRelease = NULL;
  return storage;
}

static STGMEDIUM* GetStorageForFileDescriptor(
    const base::FilePath& path) {
  string16 file_name = path.value();
  DCHECK(!file_name.empty());
  HANDLE hdata = GlobalAlloc(GPTR, sizeof(FILEGROUPDESCRIPTOR));
  base::win::ScopedHGlobal<FILEGROUPDESCRIPTOR> locked_mem(hdata);

  FILEGROUPDESCRIPTOR* descriptor = locked_mem.get();
  descriptor->cItems = 1;
  descriptor->fgd[0].dwFlags = FD_LINKUI;
  wcsncpy_s(descriptor->fgd[0].cFileName, MAX_PATH, file_name.c_str(),
            std::min(file_name.size(), static_cast<size_t>(MAX_PATH - 1u)));

  STGMEDIUM* storage = new STGMEDIUM;
  storage->tymed = TYMED_HGLOBAL;
  storage->hGlobal = hdata;
  storage->pUnkForRelease = NULL;
  return storage;
}

///////////////////////////////////////////////////////////////////////////////
// OSExchangeData, public:

// static
OSExchangeData::Provider* OSExchangeData::CreateProvider() {
  return new OSExchangeDataProviderWin();
}

// static
OSExchangeData::CustomFormat OSExchangeData::RegisterCustomFormat(
    const std::string& type) {
  return RegisterClipboardFormat(ASCIIToUTF16(type).c_str());
}

}  // namespace ui
