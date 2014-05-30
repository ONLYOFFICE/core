// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/shell_dialogs/android/shell_dialogs_jni_registrar.h"

#include "base/android/jni_android.h"
#include "base/android/jni_registrar.h"
#include "ui/shell_dialogs/select_file_dialog_android.h"

namespace ui {
namespace shell_dialogs {

static base::android::RegistrationMethod kUiRegisteredMethods[] = {
  { "SelectFileDialog", ui::SelectFileDialogImpl::RegisterSelectFileDialog },
};

bool RegisterJni(JNIEnv* env) {
  return RegisterNativeMethods(env, kUiRegisteredMethods,
                               arraysize(kUiRegisteredMethods));
}

}  // namespace shell_dialogs
}  // namespace ui
