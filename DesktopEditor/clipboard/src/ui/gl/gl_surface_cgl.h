// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_SURFACE_CGL_H_
#define UI_GL_GL_SURFACE_CGL_H_

#include "ui/gfx/size.h"
#include "ui/gl/gl_surface.h"

namespace gfx {

// Base class for CGL surfaces.
class GLSurfaceCGL : public GLSurface {
 public:
  GLSurfaceCGL();

  static bool InitializeOneOff();
  static void* GetPixelFormat();

 protected:
  virtual ~GLSurfaceCGL();

 private:
  DISALLOW_COPY_AND_ASSIGN(GLSurfaceCGL);
};

// A "no-op" surface. It is not required that a CGLContextObj have an
// associated drawable (pbuffer or fullscreen context) in order to be
// made current. Everywhere this surface type is used, we allocate an
// FBO at the user level as the drawable of the associated context.
class GL_EXPORT NoOpGLSurfaceCGL : public GLSurfaceCGL {
 public:
  explicit NoOpGLSurfaceCGL(const gfx::Size& size);

  // Implement GLSurface.
  virtual bool Initialize() OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual bool IsOffscreen() OVERRIDE;
  virtual bool SwapBuffers() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual void* GetHandle() OVERRIDE;
  virtual void* GetDisplay() OVERRIDE;

 protected:
  virtual ~NoOpGLSurfaceCGL();

 private:
  gfx::Size size_;

  DISALLOW_COPY_AND_ASSIGN(NoOpGLSurfaceCGL);
};

}  // namespace gfx

#endif  // UI_GL_GL_SURFACE_CGL_H_
