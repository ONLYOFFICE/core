// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_WEBUI_WEB_UI_UTIL_H_
#define UI_WEBUI_WEB_UI_UTIL_H_

#include <string>

#include "base/strings/string_piece.h"
#include "base/values.h"
#include "ui/base/layout.h"
#include "ui/base/ui_export.h"
#include "ui/base/window_open_disposition.h"

class GURL;
class SkBitmap;

namespace webui {

// Convenience routine to convert SkBitmap object to data url
// so that it can be used in WebUI.
UI_EXPORT std::string GetBitmapDataUrl(const SkBitmap& bitmap);

// Convenience routine to get data url that corresponds to given
// resource_id as a bitmap. This function does not check if the
// resource for the |resource_id| is a bitmap, therefore it is the
// caller's responsibility to make sure the resource is indeed a
// bitmap. Returns empty string if a resource does not exist for given
// |resource_id|.
UI_EXPORT std::string GetBitmapDataUrlFromResource(int resource_id);

// Extracts a disposition from click event arguments. |args| should contain
// an integer button and booleans alt key, ctrl key, meta key, and shift key
// (in that order), starting at |start_index|.
UI_EXPORT WindowOpenDisposition GetDispositionFromClick(const ListValue* args,
                                                        int start_index);

// Given a scale factor such as "1x", "2x" or "1.99x", sets |scale_factor| to
// the closest ScaleFactor enum value for this scale factor. If string can not
// be parsed, then |scale_factor| is set to SCALE_FACTOR_100P, and false is
// returned.
UI_EXPORT bool ParseScaleFactor(const base::StringPiece&identifier,
                                ui::ScaleFactor* scale_factor);

// Parses a URL containing some path @{scale}x. If it does not contain a scale
// factor then the default scale factor is returned.
UI_EXPORT void ParsePathAndScale(const GURL& url,
                                 std::string* path,
                                 ui::ScaleFactor* scale_factor);

// Helper function to set the font family, size, and text direction into the
// given dictionary.
UI_EXPORT void SetFontAndTextDirection(
    base::DictionaryValue* localized_strings);

}  // namespace webui

#endif  // UI_WEBUI_WEB_UI_UTIL_H_
