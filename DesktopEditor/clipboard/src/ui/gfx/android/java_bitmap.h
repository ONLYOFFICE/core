// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANDROID_JAVA_BITMAP_H_
#define UI_GFX_ANDROID_JAVA_BITMAP_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "ui/gfx/size.h"

class SkBitmap;

namespace gfx {

// This class wraps a JNI AndroidBitmap object to make it easier to use. It
// handles locking and unlocking of the underlying pixels, along with wrapping
// various JNI methods.
class UI_EXPORT JavaBitmap {
 public:
  explicit JavaBitmap(jobject bitmap);
  ~JavaBitmap();

  inline void* pixels() { return pixels_; }
  inline const gfx::Size& size() const { return size_; }
  // Formats are in android/bitmap.h; e.g. ANDROID_BITMAP_FORMAT_RGBA_8888
  inline int format() const { return format_; }
  inline uint32_t stride() const { return stride_; }

  // Registers methods with JNI and returns true if succeeded.
  static bool RegisterJavaBitmap(JNIEnv* env);

 private:
  jobject bitmap_;
  void* pixels_;
  gfx::Size size_;
  int format_;
  uint32_t stride_;

  DISALLOW_COPY_AND_ASSIGN(JavaBitmap);
};

UI_EXPORT base::android::ScopedJavaLocalRef<jobject> CreateJavaBitmap(
    const gfx::Size& size);

UI_EXPORT base::android::ScopedJavaLocalRef<jobject> ConvertToJavaBitmap(
    const SkBitmap* skbitmap);

SkBitmap CreateSkBitmapFromResource(const char* name);

}  // namespace gfx

#endif  // UI_GFX_ANDROID_JAVA_BITMAP_H_
