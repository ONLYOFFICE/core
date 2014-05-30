// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_image_egl.h"

#include "ui/gl/gl_bindings.h"

namespace gfx {

GLImageEGL::GLImageEGL(gfx::Size size)
    : egl_image_(EGL_NO_IMAGE_KHR),
      size_(size) {
}

GLImageEGL::~GLImageEGL() {
  Destroy();
}

bool GLImageEGL::Initialize(gfx::GpuMemoryBufferHandle buffer) {
  EGLClientBuffer cbuf = static_cast<EGLClientBuffer>(buffer);
  EGLint attrs[] = {
    EGL_IMAGE_PRESERVED_KHR, EGL_TRUE,
    EGL_NONE,
  };
  egl_image_ = eglCreateImageKHR(
      eglGetDisplay(EGL_DEFAULT_DISPLAY),
      EGL_NO_CONTEXT,
      EGL_NATIVE_BUFFER_ANDROID,
      cbuf,
      attrs);

  if (egl_image_ == EGL_NO_IMAGE_KHR) {
    EGLint error = eglGetError();
    LOG(ERROR) << "Error creating EGLImage: " << error;
    return false;
  }

  return true;
}

bool GLImageEGL::BindTexImage() {
  if (egl_image_ == EGL_NO_IMAGE_KHR) {
    LOG(ERROR) << "NULL EGLImage in BindTexImage";
    return false;
  }

  glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, egl_image_);

  if (glGetError() != GL_NO_ERROR) {
    return false;
  }

  return true;
}

gfx::Size GLImageEGL::GetSize() {
  return size_;
}

void GLImageEGL::Destroy() {
  if (egl_image_ == EGL_NO_IMAGE_KHR)
    return;

  EGLBoolean success = eglDestroyImageKHR(
      eglGetDisplay(EGL_DEFAULT_DISPLAY), egl_image_);

  if (success == EGL_FALSE) {
    EGLint error = eglGetError();
    LOG(ERROR) << "Error destroying EGLImage: " << error;
  }

  egl_image_ = EGL_NO_IMAGE_KHR;
}

void GLImageEGL::ReleaseTexImage() {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      NULL);
}

}  // namespace gfx
