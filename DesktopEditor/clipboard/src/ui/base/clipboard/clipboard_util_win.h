// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Some helper functions for working with the clipboard and IDataObjects.

#ifndef UI_BASE_CLIPBOARD_CLIPBOARD_UTIL_WIN_H_
#define UI_BASE_CLIPBOARD_CLIPBOARD_UTIL_WIN_H_

#include <shlobj.h>
#include <map>
#include <string>
#include <vector>

#include "base/string16.h"
#include "ui/base/ui_export.h"

namespace ui {

class UI_EXPORT ClipboardUtil {
 public:
  /////////////////////////////////////////////////////////////////////////////
  // Clipboard formats.
  static FORMATETC* GetUrlFormat();
  static FORMATETC* GetUrlWFormat();
  static FORMATETC* GetMozUrlFormat();
  static FORMATETC* GetPlainTextFormat();
  static FORMATETC* GetPlainTextWFormat();
  static FORMATETC* GetFilenameFormat();
  static FORMATETC* GetFilenameWFormat();
  // MS HTML Format
  static FORMATETC* GetHtmlFormat();
  // MS RTF Format
  static FORMATETC* GetRtfFormat();
  // Firefox text/html
  static FORMATETC* GetTextHtmlFormat();
  static FORMATETC* GetCFHDropFormat();
  static FORMATETC* GetFileDescriptorFormat();
  static FORMATETC* GetFileContentFormatZero();
  static FORMATETC* GetWebKitSmartPasteFormat();
  static FORMATETC* GetWebCustomDataFormat();
  static FORMATETC* GetPepperCustomDataFormat();
  static FORMATETC* GetSourceTagFormat();

  /////////////////////////////////////////////////////////////////////////////
  // These methods check to see if |data_object| has the requested type.
  // Returns true if it does.
  static bool HasUrl(IDataObject* data_object);
  static bool HasFilenames(IDataObject* data_object);
  static bool HasPlainText(IDataObject* data_object);
  static bool HasFileContents(IDataObject* data_object);
  static bool HasHtml(IDataObject* data_object);

  /////////////////////////////////////////////////////////////////////////////
  // Helper methods to extract information from an IDataObject.  These methods
  // return true if the requested data type is found in |data_object|.
  static bool GetUrl(IDataObject* data_object,
                     string16* url,
                     string16* title,
                     bool convert_filenames);
  static bool GetFilenames(IDataObject* data_object,
                           std::vector<string16>* filenames);
  static bool GetPlainText(IDataObject* data_object, string16* plain_text);
  static bool GetHtml(IDataObject* data_object,
                      string16* text_html,
                      std::string* base_url);
  static bool GetFileContents(IDataObject* data_object,
                              string16* filename,
                              std::string* file_contents);
  // This represents custom MIME types a web page might set to transport its
  // own types of data for drag and drop. It is sandboxed in its own CLIPFORMAT
  // to avoid polluting the ::RegisterClipboardFormat() namespace with random
  // strings from web content.
  static bool GetWebCustomData(IDataObject* data_object,
                               std::map<string16, string16>* custom_data);

  // Helper method for converting between MS CF_HTML format and plain
  // text/html.
  static std::string HtmlToCFHtml(const std::string& html,
                                  const std::string& base_url);
  static void CFHtmlToHtml(const std::string& cf_html,
                           std::string* html,
                           std::string* base_url);
  static void CFHtmlExtractMetadata(const std::string& cf_html,
                                    std::string* base_url,
                                    size_t* html_start,
                                    size_t* fragment_start,
                                    size_t* fragment_end);
};

}

#endif  // UI_BASE_CLIPBOARD_CLIPBOARD_UTIL_WIN_H_
