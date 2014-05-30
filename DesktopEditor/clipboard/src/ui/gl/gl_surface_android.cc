// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_surface.h"

#include <EGL/egl.h>

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "ui/gl/egl_util.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_implementation.h"
#include "ui/gl/gl_surface_egl.h"
#include "ui/gl/gl_surface_stub.h"

using ui::GetLastEGLErrorString;

namespace gfx {

bool GLSurface::InitializeOneOffInternal() {
  static bool initialized = false;
  if (initialized)
    return true;

  switch (GetGLImplementation()) {
    case kGLImplementationEGLGLES2:
      if (!GLSurfaceEGL::InitializeOneOff()) {
        LOG(ERROR) << "GLSurfaceEGL::InitializeOneOff failed.";
        return false;
      }
      break;
    default:
      NOTREACHED();
      break;
  }

  initialized = true;
  return true;
}
// static
scoped_refptr<GLSurface>
GLSurface::CreateViewGLSurface(bool software, gfx::AcceleratedWidget window) {
  if (software)
    return NULL;

  switch (GetGLImplementation()) {
    case kGLImplementationEGLGLES2: {
      scoped_refptr<GLSurface> surface;
      if (window)
        surface = new NativeViewGLSurfaceEGL(false, window);
      else
        surface = new GLSurfaceStub();
      if (!surface->Initialize())
        return NULL;
      return surface;
    }
    default:
      NOTREACHED();
      return NULL;
  }
}

// static
scoped_refptr<GLSurface>
GLSurface::CreateOffscreenGLSurface(bool software, const gfx::Size& size) {
  if (software)
    return NULL;

  switch (GetGLImplementation()) {
    case kGLImplementationEGLGLES2: {
      scoped_refptr<PbufferGLSurfaceEGL> surface(
          new PbufferGLSurfaceEGL(false, size));
      if (!surface->Initialize())
        return NULL;
      return surface;
    }
    default:
      NOTREACHED();
      return NULL;
  }
}

}  // namespace gfx
