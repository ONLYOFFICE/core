// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_DRAGDROP_OS_EXCHANGE_DATA_H_
#define UI_BASE_DRAGDROP_OS_EXCHANGE_DATA_H_

#include "build/build_config.h"

#include <set>
#include <string>

#if defined(OS_WIN)
#include <objidl.h>
#elif defined(TOOLKIT_GTK)
#include <gtk/gtk.h>
#endif

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "ui/base/dragdrop/download_file_interface.h"
#include "ui/base/ui_export.h"

#if defined(USE_AURA)
#include "ui/base/clipboard/clipboard.h"
#endif

class GURL;
class Pickle;

namespace gfx {
class ImageSkia;
class Vector2d;
}

namespace ui {

///////////////////////////////////////////////////////////////////////////////
//
// OSExchangeData
//  An object that holds interchange data to be sent out to OS services like
//  clipboard, drag and drop, etc. This object exposes an API that clients can
//  use to specify raw data and its high level type. This object takes care of
//  translating that into something the OS can understand.
//
///////////////////////////////////////////////////////////////////////////////

// NOTE: Support for html and file contents is required by TabContentViewWin.
// TabContentsViewGtk uses a different class to handle drag support that does
// not use OSExchangeData. As such, file contents and html support is only
// compiled on windows.
class UI_EXPORT OSExchangeData {
 public:
  // CustomFormats are used for non-standard data types. For example, bookmark
  // nodes are written using a CustomFormat.
#if defined(OS_WIN)
  typedef CLIPFORMAT CustomFormat;
#elif defined(USE_AURA)
  // Use the same type as the clipboard (why do we want two different
  // definitions of this on other platforms?).
  typedef Clipboard::FormatType CustomFormat;
#elif defined(TOOLKIT_GTK)
  typedef GdkAtom CustomFormat;
#else
  typedef void* CustomFormat;
#endif

  // Enumeration of the known formats.
  enum Format {
    STRING         = 1 << 0,
    URL            = 1 << 1,
    FILE_NAME      = 1 << 2,
    PICKLED_DATA   = 1 << 3,
#if defined(OS_WIN)
    FILE_CONTENTS  = 1 << 4,
#endif
#if defined(OS_WIN) || defined(USE_AURA)
    HTML           = 1 << 5,
#endif
  };

  // Encapsulates the info about a file to be downloaded.
  struct UI_EXPORT DownloadFileInfo {
    DownloadFileInfo(const base::FilePath& filename,
                     DownloadFileProvider* downloader);
    ~DownloadFileInfo();

    base::FilePath filename;
    scoped_refptr<DownloadFileProvider> downloader;
  };

  // Encapsulates the info about a file.
  struct UI_EXPORT FileInfo {
    FileInfo(const base::FilePath& path, const base::FilePath& display_name);
    ~FileInfo();

    // The path of the file.
    base::FilePath path;
    // The display name of the file. This field is optional.
    base::FilePath display_name;
  };

  // Provider defines the platform specific part of OSExchangeData that
  // interacts with the native system.
  class UI_EXPORT Provider {
   public:
    Provider() {}
    virtual ~Provider() {}

    virtual void SetString(const string16& data) = 0;
    virtual void SetURL(const GURL& url, const string16& title) = 0;
    virtual void SetFilename(const base::FilePath& path) = 0;
    virtual void SetFilenames(
        const std::vector<FileInfo>& file_names) = 0;
    virtual void SetPickledData(CustomFormat format, const Pickle& data) = 0;

    virtual bool GetString(string16* data) const = 0;
    virtual bool GetURLAndTitle(GURL* url, string16* title) const = 0;
    virtual bool GetFilename(base::FilePath* path) const = 0;
    virtual bool GetFilenames(
        std::vector<FileInfo>* file_names) const = 0;
    virtual bool GetPickledData(CustomFormat format, Pickle* data) const = 0;

    virtual bool HasString() const = 0;
    virtual bool HasURL() const = 0;
    virtual bool HasFile() const = 0;
    virtual bool HasCustomFormat(
        OSExchangeData::CustomFormat format) const = 0;

#if defined(OS_WIN)
    virtual void SetFileContents(const base::FilePath& filename,
                                 const std::string& file_contents) = 0;
    virtual bool GetFileContents(base::FilePath* filename,
                                 std::string* file_contents) const = 0;
    virtual bool HasFileContents() const = 0;
    virtual void SetDownloadFileInfo(const DownloadFileInfo& download) = 0;
    virtual void SetInDragLoop(bool in_drag_loop) = 0;
#endif

#if defined(OS_WIN) || defined(USE_AURA)
    virtual void SetHtml(const string16& html, const GURL& base_url) = 0;
    virtual bool GetHtml(string16* html, GURL* base_url) const = 0;
    virtual bool HasHtml() const = 0;
#endif

#if defined(USE_AURA)
    virtual void SetDragImage(const gfx::ImageSkia& image,
                              const gfx::Vector2d& cursor_offset) = 0;
    virtual const gfx::ImageSkia& GetDragImage() const = 0;
    virtual const gfx::Vector2d& GetDragImageOffset() const = 0;
#endif
  };

  // Creates the platform specific Provider.
  static Provider* CreateProvider();

  OSExchangeData();
  // Creates an OSExchangeData with the specified provider. OSExchangeData
  // takes ownership of the supplied provider.
  explicit OSExchangeData(Provider* provider);

  ~OSExchangeData();

  // Registers the specific string as a possible format for data.
  static CustomFormat RegisterCustomFormat(const std::string& type);

  // Returns the Provider, which actually stores and manages the data.
  const Provider& provider() const { return *provider_; }
  Provider& provider() { return *provider_; }

  // These functions add data to the OSExchangeData object of various Chrome
  // types. The OSExchangeData object takes care of translating the data into
  // a format suitable for exchange with the OS.
  // NOTE WELL: Typically, a data object like this will contain only one of the
  //            following types of data. In cases where more data is held, the
  //            order in which these functions are called is _important_!
  //       ---> The order types are added to an OSExchangeData object controls
  //            the order of enumeration in our IEnumFORMATETC implementation!
  //            This comes into play when selecting the best (most preferable)
  //            data type for insertion into a DropTarget.
  void SetString(const string16& data);
  // A URL can have an optional title in some exchange formats.
  void SetURL(const GURL& url, const string16& title);
  // A full path to a file.
  void SetFilename(const base::FilePath& path);
  // Full path to one or more files. See also SetFilenames() in Provider.
  void SetFilenames(
      const std::vector<FileInfo>& file_names);
  // Adds pickled data of the specified format.
  void SetPickledData(CustomFormat format, const Pickle& data);

  // These functions retrieve data of the specified type. If data exists, the
  // functions return and the result is in the out parameter. If the data does
  // not exist, the out parameter is not touched. The out parameter cannot be
  // NULL.
  bool GetString(string16* data) const;
  bool GetURLAndTitle(GURL* url, string16* title) const;
  // Return the path of a file, if available.
  bool GetFilename(base::FilePath* path) const;
  bool GetFilenames(
      std::vector<FileInfo>* file_names) const;
  bool GetPickledData(CustomFormat format, Pickle* data) const;

  // Test whether or not data of certain types is present, without actually
  // returning anything.
  bool HasString() const;
  bool HasURL() const;
  bool HasFile() const;
  bool HasCustomFormat(CustomFormat format) const;

  // Returns true if this OSExchangeData has data for ALL the formats in
  // |formats| and ALL the custom formats in |custom_formats|.
  bool HasAllFormats(int formats,
                     const std::set<CustomFormat>& custom_formats) const;

  // Returns true if this OSExchangeData has data in any of the formats in
  // |formats| or any custom format in |custom_formats|.
  bool HasAnyFormat(int formats,
                     const std::set<CustomFormat>& custom_formats) const;

#if defined(OS_WIN)
  // Adds the bytes of a file (CFSTR_FILECONTENTS and CFSTR_FILEDESCRIPTOR).
  void SetFileContents(const base::FilePath& filename,
                       const std::string& file_contents);
  bool GetFileContents(base::FilePath* filename,
                       std::string* file_contents) const;

  // Adds a download file with full path (CF_HDROP).
  void SetDownloadFileInfo(const DownloadFileInfo& download);

  void SetInDragLoop(bool in_drag_loop);
#endif

#if defined(OS_WIN) || defined(USE_AURA)
  // Adds a snippet of HTML.  |html| is just raw html but this sets both
  // text/html and CF_HTML.
  void SetHtml(const string16& html, const GURL& base_url);
  bool GetHtml(string16* html, GURL* base_url) const;
#endif

 private:
  // Provides the actual data.
  scoped_ptr<Provider> provider_;

  DISALLOW_COPY_AND_ASSIGN(OSExchangeData);
};

}  // namespace ui

#endif  // UI_BASE_DRAGDROP_OS_EXCHANGE_DATA_H_
