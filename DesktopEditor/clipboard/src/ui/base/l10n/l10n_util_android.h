// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_L10N_L10N_UTIL_ANDROID_H_
#define UI_BASE_L10N_L10N_UTIL_ANDROID_H_

#include <jni.h>

#include <string>

#include "base/string16.h"
#include "ui/base/ui_export.h"

namespace l10n_util {

// Return the current default locale of the device.
UI_EXPORT std::string GetDefaultLocale();

UI_EXPORT string16 GetDisplayNameForLocale(const std::string& locale,
                                           const std::string& display_locale);

bool RegisterLocalizationUtil(JNIEnv* env);

}  // namespace l10n_util

#endif  // UI_BASE_L10N_L10N_UTIL_ANDROID_H_
