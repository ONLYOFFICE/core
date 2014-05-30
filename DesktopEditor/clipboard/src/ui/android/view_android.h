// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_ANDROID_VIEW_ANDROID_H_
#define UI_ANDROID_VIEW_ANDROID_H_

#include <jni.h>
#include "base/android/jni_helper.h"
#include "base/android/scoped_java_ref.h"
#include "ui/base/ui_export.h"

namespace ui {

class WindowAndroid;

// This class is the native counterpart for ViewAndroid. It is owned by the
// Java ViewAndroid object.
class UI_EXPORT ViewAndroid {
 public:
  ViewAndroid(JNIEnv* env, jobject obj, WindowAndroid* window);

  void Destroy(JNIEnv* env, jobject obj);

  WindowAndroid* GetWindowAndroid();

  base::android::ScopedJavaLocalRef<jobject> GetJavaObject();

  static bool RegisterViewAndroid(JNIEnv* env);

 private:
  ~ViewAndroid();
  JavaObjectWeakGlobalRef weak_java_view_;
  WindowAndroid* window_android_;

  DISALLOW_COPY_AND_ASSIGN(ViewAndroid);
};

}  // namespace ui

#endif  // UI_ANDROID_VIEW_ANDROID_H_
