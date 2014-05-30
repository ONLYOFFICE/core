// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANDROID_DEVICE_DISPLAY_INFO_H_
#define UI_GFX_ANDROID_DEVICE_DISPLAY_INFO_H_

#include <jni.h>
#include <string>

#include "base/android/scoped_java_ref.h"
#include "base/basictypes.h"
#include "ui/base/ui_export.h"

namespace gfx {

// Facilitates access to device information typically only
// available using the Android SDK, including Display properties.
class UI_EXPORT DeviceDisplayInfo {
 public:
  DeviceDisplayInfo();
  ~DeviceDisplayInfo();

  // Returns display height in physical pixels.
  int GetDisplayHeight();

  // Returns display width in physical pixels.
  int GetDisplayWidth();

  // Returns number of bits per pixel.
  int GetBitsPerPixel();

  // Returns number of bits per component.
  int GetBitsPerComponent();

  // Returns a scaling factor for Density Independent Pixel unit
  // (1.0 is 160dpi, 0.75 is 120dpi, 2.0 is 320dpi).
  double GetDIPScale();

  // Returns display refresh rate in number of frames per second.
  double GetRefreshRate();

  // Registers methods with JNI and returns true if succeeded.
  static bool RegisterDeviceDisplayInfo(JNIEnv* env);

 private:
  base::android::ScopedJavaGlobalRef<jobject> j_device_info_;

  DISALLOW_COPY_AND_ASSIGN(DeviceDisplayInfo);
};

}  // namespace gfx

#endif  // UI_GFX_ANDROID_DEVICE_DISPLAY_INFO_H_
