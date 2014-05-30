// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/android/ui_jni_registrar.h"

#include "base/android/jni_android.h"
#include "base/android/jni_registrar.h"
#include "ui/android/view_android.h"
#include "ui/android/window_android.h"
#include "ui/base/clipboard/clipboard_android_initialization.h"
#include "ui/base/l10n/l10n_util_android.h"
#include "ui/gfx/android/device_display_info.h"
#include "ui/gfx/android/java_bitmap.h"

namespace ui {
namespace android {

static base::android::RegistrationMethod kUiRegisteredMethods[] = {
  { "Clipboard", ui::RegisterClipboardAndroid },
  { "DeviceDisplayInfo", gfx::DeviceDisplayInfo::RegisterDeviceDisplayInfo },
  { "JavaBitmap", gfx::JavaBitmap::RegisterJavaBitmap },
  { "LocalizationUtils", l10n_util::RegisterLocalizationUtil },
  { "ViewAndroid", ui::ViewAndroid::RegisterViewAndroid },
  { "WindowAndroid", ui::WindowAndroid::RegisterWindowAndroid },
};

bool RegisterJni(JNIEnv* env) {
  return RegisterNativeMethods(env, kUiRegisteredMethods,
                               arraysize(kUiRegisteredMethods));
}

}  // namespace android
}  // namespace ui
