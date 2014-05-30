// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/android/surface_texture_bridge.h"

#include <android/native_window_jni.h>

// TODO(boliu): Remove this include when we move off ICS.
#include "base/android/build_info.h"
#include "base/android/jni_android.h"
#include "base/logging.h"
#include "jni/SurfaceTexture_jni.h"
#include "ui/gl/android/scoped_java_surface.h"
#include "ui/gl/android/surface_texture_listener.h"

using base::android::AttachCurrentThread;
using base::android::CheckException;
using base::android::GetClass;
using base::android::ScopedJavaLocalRef;

namespace {
bool g_jni_initialized = false;

void RegisterNativesIfNeeded(JNIEnv* env) {
  if (!g_jni_initialized) {
    JNI_SurfaceTexture::RegisterNativesImpl(env);
    g_jni_initialized = true;
  }
}

// TODO(boliu): Remove this method when when we move off ICS. See
// http://crbug.com/161864.
bool GlContextMethodsAvailable() {
  bool available = base::android::BuildInfo::GetInstance()->sdk_int() >= 16;
  if (!available)
    LOG(WARNING) << "Running on unsupported device: rendering may not work";
  return available;
}

}  // namespace

namespace gfx {

SurfaceTextureBridge::SurfaceTextureBridge(int texture_id)
    : texture_id_(texture_id) {
  JNIEnv* env = AttachCurrentThread();
  CHECK(env);
  RegisterNativesIfNeeded(env);

  ScopedJavaLocalRef<jobject> tmp(
      JNI_SurfaceTexture::Java_SurfaceTexture_Constructor(
          env, texture_id));
  DCHECK(!tmp.is_null());
  j_surface_texture_.Reset(tmp);
}

SurfaceTextureBridge::~SurfaceTextureBridge() {
  JNIEnv* env = AttachCurrentThread();
  CHECK(env);

  // Release the listener.
  JNI_SurfaceTexture::Java_SurfaceTexture_setOnFrameAvailableListener(
      env, j_surface_texture_.obj(), NULL);

  // Release graphics memory.
  JNI_SurfaceTexture::Java_SurfaceTexture_release(
      env, j_surface_texture_.obj());
}

void SurfaceTextureBridge::SetFrameAvailableCallback(
    const base::Closure& callback) {
  JNIEnv* env = AttachCurrentThread();
  CHECK(env);

  // Since the listener is owned by the Java SurfaceTexture object, setting
  // a new listener here will release an existing one at the same time.
  ScopedJavaLocalRef<jobject> j_listener(
      env,
      SurfaceTextureListener::CreateSurfaceTextureListener(env, callback));
  DCHECK(!j_listener.is_null());

  // Set it as the onFrameAvailableListener for our SurfaceTexture instance.
  JNI_SurfaceTexture::Java_SurfaceTexture_setOnFrameAvailableListener(
      env, j_surface_texture_.obj(), j_listener.obj());
}

void SurfaceTextureBridge::UpdateTexImage() {
  JNIEnv* env = AttachCurrentThread();
  CHECK(env);

  JNI_SurfaceTexture::Java_SurfaceTexture_updateTexImage(
      env, j_surface_texture_.obj());
}

void SurfaceTextureBridge::GetTransformMatrix(float mtx[16]) {
  JNIEnv* env = AttachCurrentThread();
  CHECK(env);

  ScopedJavaLocalRef<jfloatArray> jmatrix(env, env->NewFloatArray(16));
  JNI_SurfaceTexture::Java_SurfaceTexture_getTransformMatrix(
      env, j_surface_texture_.obj(), jmatrix.obj());

  jboolean is_copy;
  jfloat* elements = env->GetFloatArrayElements(jmatrix.obj(), &is_copy);
  for (int i = 0; i < 16; ++i) {
    mtx[i] = static_cast<float>(elements[i]);
  }
  env->ReleaseFloatArrayElements(jmatrix.obj(), elements, JNI_ABORT);
}

void SurfaceTextureBridge::SetDefaultBufferSize(int width, int height) {
  JNIEnv* env = AttachCurrentThread();
  CHECK(env);

  if (width > 0 && height > 0) {
    JNI_SurfaceTexture::Java_SurfaceTexture_setDefaultBufferSize(
        env, j_surface_texture_.obj(), static_cast<jint>(width),
        static_cast<jint>(height));
  } else {
    LOG(WARNING) << "Not setting surface texture buffer size - "
                    "width or height is 0";
  }
}

void SurfaceTextureBridge::AttachToGLContext(int texture_id) {
  if (GlContextMethodsAvailable()) {
    JNIEnv* env = AttachCurrentThread();
    // Note: This method is only available on JB and greater.
    JNI_SurfaceTexture::Java_SurfaceTexture_attachToGLContext(
        env, j_surface_texture_.obj(), texture_id);
  }
}

void SurfaceTextureBridge::DetachFromGLContext() {
  if (GlContextMethodsAvailable()) {
    JNIEnv* env = AttachCurrentThread();
    // Note: This method is only available on JB and greater.
    JNI_SurfaceTexture::Java_SurfaceTexture_detachFromGLContext(
        env, j_surface_texture_.obj());
  }
}

ANativeWindow* SurfaceTextureBridge::CreateSurface() {
  JNIEnv* env = AttachCurrentThread();
  ScopedJavaSurface surface(this);
  ANativeWindow* native_window =
      ANativeWindow_fromSurface(env, surface.j_surface().obj());
  return native_window;
}

}  // namespace gfx
