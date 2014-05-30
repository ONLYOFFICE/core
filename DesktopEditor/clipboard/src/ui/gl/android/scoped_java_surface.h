// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_ANDROID_SCOPED_JAVA_SURFACE_H_
#define UI_GL_ANDROID_SCOPED_JAVA_SURFACE_H_

#include <jni.h>

#include "base/android/scoped_java_ref.h"
#include "ui/gl/gl_export.h"

namespace gfx {

class SurfaceTextureBridge;

// A helper class for holding a scoped reference to a Java Surface instance.
// When going out of scope, Surface.release() is called on the Java object to
// make sure server-side references (esp. wrt graphics memory) are released.
class GL_EXPORT ScopedJavaSurface {
 public:
  ScopedJavaSurface();

  // Wraps an existing Java Surface object in a ScopedJavaSurface.
  explicit ScopedJavaSurface(const base::android::JavaRef<jobject>& surface);

  // Creates a Java Surface from a SurfaceTexture and wraps it in a
  // ScopedJavaSurface.
  explicit ScopedJavaSurface(const SurfaceTextureBridge* surface_texture);

  ~ScopedJavaSurface();

  const base::android::JavaRef<jobject>& j_surface() const {
    return j_surface_;
  }

 private:
  base::android::ScopedJavaGlobalRef<jobject> j_surface_;
};

}  // namespace gfx

#endif  // UI_GL_ANDROID_SCOPED_JAVA_SURFACE_H_
