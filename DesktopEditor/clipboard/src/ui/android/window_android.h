// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_ANDROID_WINDOW_ANDROID_H_
#define UI_ANDROID_WINDOW_ANDROID_H_

#include <jni.h>
#include "base/android/jni_helper.h"
#include "base/android/scoped_java_ref.h"
#include "ui/base/ui_export.h"

namespace ui {

// Android implementation of the activity window.
class UI_EXPORT WindowAndroid {
 public:
  WindowAndroid(JNIEnv* env, jobject obj);

  void Destroy(JNIEnv* env, jobject obj);

  base::android::ScopedJavaLocalRef<jobject> GetJavaObject();

  static bool RegisterWindowAndroid(JNIEnv* env);

 private:
  ~WindowAndroid();

  JavaObjectWeakGlobalRef weak_java_window_;

  DISALLOW_COPY_AND_ASSIGN(WindowAndroid);
};

}  // namespace ui

#endif  // UI_ANDROID_WINDOW_ANDROID_H_
