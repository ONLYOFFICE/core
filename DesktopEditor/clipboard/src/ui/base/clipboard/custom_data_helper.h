// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Due to restrictions of most operating systems, we don't directly map each
// type of custom data to a native data transfer type. Instead, we serialize
// each key-value pair into the pickle as a pair of string objects, and then
// write the binary data in the pickle to the native data transfer object.

#ifndef UI_BASE_CLIPBOARD_CUSTOM_DATA_HELPER_H_
#define UI_BASE_CLIPBOARD_CUSTOM_DATA_HELPER_H_

#include <map>
#include <vector>

#include "base/string16.h"
#include "ui/base/ui_export.h"

class Pickle;

#if defined(OS_MACOSX)
#ifdef __OBJC__
@class NSString;
#else
class NSString;
#endif
#endif  // defined(OS_MACOSX)

namespace ui {

#if defined(OS_MACOSX)
UI_EXPORT extern NSString* const kWebCustomDataPboardType;
#elif (!defined(OS_WIN) && defined(USE_AURA)) || defined(TOOLKIT_GTK)
UI_EXPORT extern const char kMimeTypeWebCustomData[];
#endif

UI_EXPORT void ReadCustomDataTypes(const void* data,
                                   size_t data_length,
                                   std::vector<string16>* types);
UI_EXPORT void ReadCustomDataForType(const void* data,
                                     size_t data_length,
                                     const string16& type,
                                     string16* result);
UI_EXPORT void ReadCustomDataIntoMap(const void* data,
                                     size_t data_length,
                                     std::map<string16, string16>* result);

UI_EXPORT void WriteCustomDataToPickle(const std::map<string16, string16>& data,
                                       Pickle* pickle);

}  // namespace ui

#endif  // UI_BASE_CLIPBOARD_CLIPBOARD_H_
