// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_ANDROID_UI_JNI_REGISTRAR_H_
#define UI_ANDROID_UI_JNI_REGISTRAR_H_

#include <jni.h>

#include "ui/base/ui_export.h"

namespace ui {
namespace android {

// Register all JNI bindings necessary for chrome.
UI_EXPORT bool RegisterJni(JNIEnv* env);

}  // namespace android
}  // namespace ui

#endif  // UI_ANDROID_UI_JNI_REGISTRAR_H_
