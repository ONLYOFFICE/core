// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/android/java_bitmap.h"

#include <android/bitmap.h>

#include "base/logging.h"
#include "jni/BitmapHelper_jni.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/size.h"

using base::android::AttachCurrentThread;

namespace gfx {

JavaBitmap::JavaBitmap(jobject bitmap)
    : bitmap_(bitmap),
      pixels_(NULL) {
  int err = AndroidBitmap_lockPixels(AttachCurrentThread(), bitmap_, &pixels_);
  DCHECK(!err);
  DCHECK(pixels_);

  AndroidBitmapInfo info;
  err = AndroidBitmap_getInfo(AttachCurrentThread(), bitmap_, &info);
  DCHECK(!err);
  size_ = gfx::Size(info.width, info.height);
  format_ = info.format;
  stride_ = info.stride;
}

JavaBitmap::~JavaBitmap() {
  int err = AndroidBitmap_unlockPixels(AttachCurrentThread(), bitmap_);
  DCHECK(!err);
}

// static
bool JavaBitmap::RegisterJavaBitmap(JNIEnv* env) {
  return ui::RegisterNativesImpl(env);
}

ScopedJavaLocalRef<jobject> CreateJavaBitmap(const gfx::Size& size) {
  return ui::Java_BitmapHelper_createBitmap(AttachCurrentThread(),
      size.width(), size.height());
}

ScopedJavaLocalRef<jobject> ConvertToJavaBitmap(const SkBitmap* skbitmap) {
  DCHECK(skbitmap);
  DCHECK_EQ(skbitmap->bytesPerPixel(), 4);

  ScopedJavaLocalRef<jobject> jbitmap =
      CreateJavaBitmap(gfx::Size(skbitmap->width(), skbitmap->height()));
  SkAutoLockPixels src_lock(*skbitmap);
  JavaBitmap dst_lock(jbitmap.obj());
  void* src_pixels = skbitmap->getPixels();
  void* dst_pixels = dst_lock.pixels();
  memcpy(dst_pixels, src_pixels, skbitmap->getSize());

  return jbitmap;
}

static ScopedJavaLocalRef<jobject> CreateJavaBitmapFromResource(
    const char* name) {
  JNIEnv* env = AttachCurrentThread();
  ScopedJavaLocalRef<jstring> jname(env, env->NewStringUTF(name));
  return ui::Java_BitmapHelper_decodeDrawableResource(env, jname.obj());
}

static SkBitmap ConvertToSkBitmap(ScopedJavaLocalRef<jobject> jbitmap) {
  JavaBitmap src_lock(jbitmap.obj());
  DCHECK_EQ(src_lock.format(), ANDROID_BITMAP_FORMAT_RGBA_8888);

  gfx::Size src_size = src_lock.size();

  SkBitmap skbitmap;
  skbitmap.setConfig(SkBitmap::kARGB_8888_Config,
      src_size.width(), src_size.height(), src_lock.stride());
  skbitmap.allocPixels();
  SkAutoLockPixels dst_lock(skbitmap);

  void* src_pixels = src_lock.pixels();
  void* dst_pixels = skbitmap.getPixels();

  memcpy(dst_pixels, src_pixels, skbitmap.getSize());

  return skbitmap;
}

SkBitmap CreateSkBitmapFromResource(const char* name) {
  return ConvertToSkBitmap(CreateJavaBitmapFromResource(name));
}

}  //  namespace gfx
