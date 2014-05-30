// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_CONTEXT_NSVIEW_H_
#define UI_GL_GL_CONTEXT_NSVIEW_H_

#import <AppKit/NSOpenGL.h>

#include "base/compiler_specific.h"
#include "base/memory/scoped_nsobject.h"
#include "ui/gl/gl_context.h"

namespace gfx {

class GLSurface;

// GLContextNSView encapsulates an NSView-based GLContext.  This is paired with
// the GLSurfaceNSView class.
class GLContextNSView : public GLContext {
 public:
  explicit GLContextNSView(GLShareGroup* group);
  virtual ~GLContextNSView();

  // GLContext:
  virtual bool Initialize(GLSurface* surface,
                          GpuPreference gpu_preference) OVERRIDE;
  virtual void Destroy() OVERRIDE;
  virtual bool MakeCurrent(GLSurface* surface) OVERRIDE;
  virtual void ReleaseCurrent(GLSurface* surface) OVERRIDE;
  virtual bool IsCurrent(GLSurface* surface) OVERRIDE;
  virtual void* GetHandle() OVERRIDE;
  virtual void SetSwapInterval(int interval) OVERRIDE;

  // Flush the |context_|.  Swaps buffers.
  void FlushBuffer();

 private:
  scoped_nsobject<NSOpenGLContext> context_;
  GpuPreference gpu_preference_;

  DISALLOW_COPY_AND_ASSIGN(GLContextNSView);
};

}  // namespace gfx

#endif  // UI_GL_GL_CONTEXT_NSVIEW_H_
