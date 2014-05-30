// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_ANDROID_SURFACE_TEXTURE_LISTENER_H_
#define UI_GL_ANDROID_SURFACE_TEXTURE_LISTENER_H_

#include <jni.h>
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "ui/gl/gl_export.h"

namespace base {
class MessageLoopProxy;
}

namespace gfx {

// Listener class for all the callbacks from android SurfaceTexture.
class GL_EXPORT SurfaceTextureListener {
public:
  // Destroy this listener.
  void Destroy(JNIEnv* env, jobject obj);

  // A new frame is available to consume.
  void FrameAvailable(JNIEnv* env, jobject obj);

  static bool RegisterSurfaceTextureListener(JNIEnv* env);

private:
  SurfaceTextureListener(const base::Closure& callback);
  ~SurfaceTextureListener();

  friend class SurfaceTextureBridge;

  // Static factory method for the creation of a SurfaceTextureListener.
  // The native code should not hold any reference to the returned object,
  // but only use it to pass it up to Java for being referenced by a
  // SurfaceTexture instance.
  static jobject CreateSurfaceTextureListener(JNIEnv* env,
                                              const base::Closure& callback);

  base::Closure callback_;

  scoped_refptr<base::MessageLoopProxy> browser_loop_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(SurfaceTextureListener);
};

}  // namespace gfx

#endif  // UI_GL_ANDROID_SURFACE_TEXTURE_LISTENER_H_
