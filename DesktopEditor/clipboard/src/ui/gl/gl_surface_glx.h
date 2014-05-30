// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_SURFACE_GLX_H_
#define UI_GL_GL_SURFACE_GLX_H_

#include <string>

#include "base/compiler_specific.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "ui/gl/gl_export.h"
#include "ui/gl/gl_surface.h"
#include "ui/gl/vsync_provider.h"

namespace gfx {

// Base class for GLX surfaces.
class GL_EXPORT GLSurfaceGLX : public GLSurface {
 public:
  GLSurfaceGLX();

  static bool InitializeOneOff();

  // These aren't particularly tied to surfaces, but since we already
  // have the static InitializeOneOff here, it's easiest to reuse its
  // initialization guards.
  static const char* GetGLXExtensions();
  static bool HasGLXExtension(const char* name);
  static bool IsCreateContextSupported();
  static bool IsCreateContextRobustnessSupported();
  static bool IsTextureFromPixmapSupported();
  static bool IsOMLSyncControlSupported();

  virtual void* GetDisplay() OVERRIDE;

  // Get the FB config that the surface was created with or NULL if it is not
  // a GLX drawable.
  virtual void* GetConfig() = 0;

 protected:
  virtual ~GLSurfaceGLX();

 private:
  DISALLOW_COPY_AND_ASSIGN(GLSurfaceGLX);
};

// A surface used to render to a view.
class GL_EXPORT NativeViewGLSurfaceGLX : public GLSurfaceGLX {
 public:
  explicit NativeViewGLSurfaceGLX(gfx::AcceleratedWidget window);

  // Implement GLSurfaceGLX.
  virtual bool Initialize() OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual bool Resize(const gfx::Size& size) OVERRIDE;
  virtual bool IsOffscreen() OVERRIDE;
  virtual bool SwapBuffers() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual void* GetHandle() OVERRIDE;
  virtual std::string GetExtensions() OVERRIDE;
  virtual void* GetConfig() OVERRIDE;
  virtual bool PostSubBuffer(int x, int y, int width, int height) OVERRIDE;
  virtual VSyncProvider* GetVSyncProvider() OVERRIDE;

 protected:
  NativeViewGLSurfaceGLX();
  virtual ~NativeViewGLSurfaceGLX();

  gfx::AcceleratedWidget window_;

 private:
  void* config_;
  gfx::Size size_;

  scoped_ptr<VSyncProvider> vsync_provider_;

  DISALLOW_COPY_AND_ASSIGN(NativeViewGLSurfaceGLX);
};

// A surface used to render to an offscreen pbuffer.
class GL_EXPORT PbufferGLSurfaceGLX : public GLSurfaceGLX {
 public:
  explicit PbufferGLSurfaceGLX(const gfx::Size& size);

  // Implement GLSurfaceGLX.
  virtual bool Initialize() OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual bool IsOffscreen() OVERRIDE;
  virtual bool SwapBuffers() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual void* GetHandle() OVERRIDE;
  virtual void* GetConfig() OVERRIDE;

 protected:
  virtual ~PbufferGLSurfaceGLX();

 private:
  gfx::Size size_;
  void* config_;
  XID pbuffer_;

  DISALLOW_COPY_AND_ASSIGN(PbufferGLSurfaceGLX);
};

}  // namespace gfx

#endif  // UI_GL_GL_SURFACE_GLX_H_
