// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/dragdrop/os_exchange_data.h"

#include "base/pickle.h"
#include "googleurl/src/gurl.h"

namespace ui {

OSExchangeData::DownloadFileInfo::DownloadFileInfo(
    const base::FilePath& filename,
    DownloadFileProvider* downloader)
    : filename(filename),
      downloader(downloader) {
}

OSExchangeData::DownloadFileInfo::~DownloadFileInfo() {}

OSExchangeData::FileInfo::FileInfo(
    const base::FilePath& path,
    const base::FilePath& display_name)
    : path(path),
      display_name(display_name) {
}

OSExchangeData::FileInfo::~FileInfo() {}

OSExchangeData::OSExchangeData() : provider_(CreateProvider()) {
}

OSExchangeData::OSExchangeData(Provider* provider) : provider_(provider) {
}

OSExchangeData::~OSExchangeData() {
}

void OSExchangeData::SetString(const string16& data) {
  provider_->SetString(data);
}

void OSExchangeData::SetURL(const GURL& url, const string16& title) {
  provider_->SetURL(url, title);
}

void OSExchangeData::SetFilename(const base::FilePath& path) {
  provider_->SetFilename(path);
}

void OSExchangeData::SetFilenames(
    const std::vector<FileInfo>& filenames) {
  provider_->SetFilenames(filenames);
}

void OSExchangeData::SetPickledData(CustomFormat format, const Pickle& data) {
  provider_->SetPickledData(format, data);
}

bool OSExchangeData::GetString(string16* data) const {
  return provider_->GetString(data);
}

bool OSExchangeData::GetURLAndTitle(GURL* url, string16* title) const {
  return provider_->GetURLAndTitle(url, title);
}

bool OSExchangeData::GetFilename(base::FilePath* path) const {
  return provider_->GetFilename(path);
}

bool OSExchangeData::GetFilenames(
    std::vector<FileInfo>* filenames) const {
  return provider_->GetFilenames(filenames);
}

bool OSExchangeData::GetPickledData(CustomFormat format, Pickle* data) const {
  return provider_->GetPickledData(format, data);
}

bool OSExchangeData::HasString() const {
  return provider_->HasString();
}

bool OSExchangeData::HasURL() const {
  return provider_->HasURL();
}

bool OSExchangeData::HasFile() const {
  return provider_->HasFile();
}

bool OSExchangeData::HasCustomFormat(CustomFormat format) const {
  return provider_->HasCustomFormat(format);
}

bool OSExchangeData::HasAllFormats(
    int formats,
    const std::set<CustomFormat>& custom_formats) const {
  if ((formats & STRING) != 0 && !HasString())
    return false;
  if ((formats & URL) != 0 && !HasURL())
    return false;
#if defined(OS_WIN)
  if ((formats & FILE_CONTENTS) != 0 && !provider_->HasFileContents())
    return false;
#endif
#if defined(OS_WIN) || defined(USE_AURA)
  if ((formats & HTML) != 0 && !provider_->HasHtml())
    return false;
#endif
  if ((formats & FILE_NAME) != 0 && !provider_->HasFile())
    return false;
  for (std::set<CustomFormat>::const_iterator i = custom_formats.begin();
       i != custom_formats.end(); ++i) {
    if (!HasCustomFormat(*i))
      return false;
  }
  return true;
}

bool OSExchangeData::HasAnyFormat(
    int formats,
    const std::set<CustomFormat>& custom_formats) const {
  if ((formats & STRING) != 0 && HasString())
    return true;
  if ((formats & URL) != 0 && HasURL())
    return true;
#if defined(OS_WIN)
  if ((formats & FILE_CONTENTS) != 0 && provider_->HasFileContents())
    return true;
#endif
#if defined(OS_WIN) || defined(USE_AURA)
  if ((formats & HTML) != 0 && provider_->HasHtml())
    return true;
#endif
  if ((formats & FILE_NAME) != 0 && provider_->HasFile())
    return true;
  for (std::set<CustomFormat>::const_iterator i = custom_formats.begin();
       i != custom_formats.end(); ++i) {
    if (HasCustomFormat(*i))
      return true;
  }
  return false;
}

#if defined(OS_WIN)
void OSExchangeData::SetFileContents(const base::FilePath& filename,
                                     const std::string& file_contents) {
  provider_->SetFileContents(filename, file_contents);
}

bool OSExchangeData::GetFileContents(base::FilePath* filename,
                                     std::string* file_contents) const {
  return provider_->GetFileContents(filename, file_contents);
}

void OSExchangeData::SetDownloadFileInfo(const DownloadFileInfo& download) {
  provider_->SetDownloadFileInfo(download);
}

void OSExchangeData::SetInDragLoop(bool in_drag_loop) {
  provider_->SetInDragLoop(in_drag_loop);
}
#endif

#if defined(OS_WIN) || defined(USE_AURA)
void OSExchangeData::SetHtml(const string16& html, const GURL& base_url) {
  provider_->SetHtml(html, base_url);
}

bool OSExchangeData::GetHtml(string16* html, GURL* base_url) const {
  return provider_->GetHtml(html, base_url);
}
#endif

}  // namespace ui
