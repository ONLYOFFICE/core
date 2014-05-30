// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_surface_nsview.h"

#import <AppKit/NSOpenGL.h>
#import <AppKit/NSView.h>

#include "ui/gl/gl_context_nsview.h"

namespace gfx {

GLSurfaceNSView::GLSurfaceNSView(AcceleratedWidget view)
    : view_(view),
      context_(NULL) {
}

GLSurfaceNSView::~GLSurfaceNSView() {
}

void GLSurfaceNSView::Destroy() {
}

bool GLSurfaceNSView::IsOffscreen() {
  return false;
}

bool GLSurfaceNSView::SwapBuffers() {
  context_->FlushBuffer();
  return true;
}

gfx::Size GLSurfaceNSView::GetSize() {
  return gfx::Size(NSSizeToCGSize([view_ bounds].size));
}

void* GLSurfaceNSView::GetHandle() {
  return view_;
}

bool GLSurfaceNSView::OnMakeCurrent(GLContext* context) {
  context_ = static_cast<GLContextNSView *>(context);
  return true;
}

}  // namespace gfx
