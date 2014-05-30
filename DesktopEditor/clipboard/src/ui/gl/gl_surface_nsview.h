// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_SURFACE_NSVIEW_H_
#define UI_GL_GL_SURFACE_NSVIEW_H_

#include "base/compiler_specific.h"
#include "ui/gfx/size.h"
#include "ui/gl/gl_surface.h"

namespace gfx {

class GLContextNSView;

// GLSurfaceNSView provides an implementation of the the GLSurface interface
// that is backed by an NSView.  This interface pairs with the GLContextNSView
// class, and the NSView is expected to use this context for drawing.
class GLSurfaceNSView : public GLSurface {
 public:
  explicit GLSurfaceNSView(AcceleratedWidget view);
  virtual ~GLSurfaceNSView();

  // GLSurface:
  virtual void Destroy() OVERRIDE;
  virtual bool IsOffscreen() OVERRIDE;
  virtual bool SwapBuffers() OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual void* GetHandle() OVERRIDE;

  // Allow the surface to call back to context when in need of |FlushBuffer|.
  virtual bool OnMakeCurrent(GLContext* context) OVERRIDE;

 private:
  // Weak.  An |NSView*|.
  AcceleratedWidget view_;

  // Weak.  Associated context.
  GLContextNSView* context_;

  DISALLOW_COPY_AND_ASSIGN(GLSurfaceNSView);
};

}  // namespace gfx

#endif  // UI_GL_GL_SURFACE_NSVIEW_H_
