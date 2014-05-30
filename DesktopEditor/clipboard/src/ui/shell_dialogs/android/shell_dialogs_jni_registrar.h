// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_SHELL_DIALOGS_ANDROID_UI_SHELL_DIALOGS_JNI_REGISTRAR_H_
#define UI_SHELL_DIALOGS_ANDROID_UI_SHELL_DIALOGS_JNI_REGISTRAR_H_

#include <jni.h>

#include "ui/shell_dialogs/shell_dialogs_export.h"

namespace ui {
namespace shell_dialogs {

// Register all JNI bindings necessary for chrome.
SHELL_DIALOGS_EXPORT bool RegisterJni(JNIEnv* env);

}  // namespace shell_dialogs
}  // namespace ui

#endif  // UI_SHELL_DIALOGS_ANDROID_UI_SHELL_DIALOGS_JNI_REGISTRAR_H_
