// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_IMAGE_EGL_H_
#define UI_GL_GL_IMAGE_EGL_H_

#include "ui/gl/gl_bindings.h"  // for EGLImageKHR
#include "ui/gl/gl_image.h"

namespace gfx {

class GL_EXPORT GLImageEGL : public GLImage {
 public:
  explicit GLImageEGL(gfx::Size size);

  bool Initialize(gfx::GpuMemoryBufferHandle buffer);

  // Implement GLImage.
  virtual void Destroy() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual bool BindTexImage() OVERRIDE;
  virtual void ReleaseTexImage() OVERRIDE;

 protected:
  virtual ~GLImageEGL();

 private:
  EGLImageKHR egl_image_;
  gfx::Size size_;

  DISALLOW_COPY_AND_ASSIGN(GLImageEGL);
};

}  // namespace gfx

#endif  // UI_GL_GL_IMAGE_EGL_H_
