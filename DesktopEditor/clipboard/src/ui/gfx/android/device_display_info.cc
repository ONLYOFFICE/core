// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/android/device_display_info.h"

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "base/logging.h"
#include "jni/DeviceDisplayInfo_jni.h"

using base::android::AttachCurrentThread;
using base::android::ScopedJavaLocalRef;

namespace gfx {

DeviceDisplayInfo::DeviceDisplayInfo() {
  JNIEnv* env = AttachCurrentThread();
  j_device_info_.Reset(Java_DeviceDisplayInfo_create(env,
      base::android::GetApplicationContext()));
}

DeviceDisplayInfo::~DeviceDisplayInfo() {
}

int DeviceDisplayInfo::GetDisplayHeight() {
  JNIEnv* env = AttachCurrentThread();
  jint result =
      Java_DeviceDisplayInfo_getDisplayHeight(env, j_device_info_.obj());
  return static_cast<int>(result);
}

int DeviceDisplayInfo::GetDisplayWidth() {
  JNIEnv* env = AttachCurrentThread();
  jint result =
      Java_DeviceDisplayInfo_getDisplayWidth(env, j_device_info_.obj());
  return static_cast<int>(result);
}

int DeviceDisplayInfo::GetBitsPerPixel() {
  JNIEnv* env = AttachCurrentThread();
  jint result =
      Java_DeviceDisplayInfo_getBitsPerPixel(env, j_device_info_.obj());
  return static_cast<int>(result);
}

int DeviceDisplayInfo::GetBitsPerComponent() {
  JNIEnv* env = AttachCurrentThread();
  jint result =
      Java_DeviceDisplayInfo_getBitsPerComponent(env, j_device_info_.obj());
  return static_cast<int>(result);
}

double DeviceDisplayInfo::GetDIPScale() {
  JNIEnv* env = AttachCurrentThread();
  jdouble result =
      Java_DeviceDisplayInfo_getDIPScale(env, j_device_info_.obj());
  return static_cast<double>(result);
}

double DeviceDisplayInfo::GetRefreshRate() {
  JNIEnv* env = AttachCurrentThread();
  jdouble result =
      Java_DeviceDisplayInfo_getRefreshRate(env, j_device_info_.obj());
  return static_cast<double>(result);
}

// static
bool DeviceDisplayInfo::RegisterDeviceDisplayInfo(JNIEnv* env) {
  return RegisterNativesImpl(env);
}

}  // namespace gfx
