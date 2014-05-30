// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_context_nsview.h"

#include <vector>

#import <AppKit/NSOpenGL.h>
#import <AppKit/NSView.h>

#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "ui/gl/gl_surface_nsview.h"

namespace gfx {

GLContextNSView::GLContextNSView(GLShareGroup* group)
    : GLContext(group) {
}

GLContextNSView::~GLContextNSView() {
}

bool GLContextNSView::Initialize(GLSurface* surface,
                                 GpuPreference gpu_preference) {
  DCHECK(!context_) << "NSGLContext was previously initialized.";
  gpu_preference_ = gpu_preference;

  std::vector<NSOpenGLPixelFormatAttribute> attributes;
  attributes.push_back(NSOpenGLPFAAccelerated);
  attributes.push_back(NSOpenGLPFADoubleBuffer);
  attributes.push_back(0);

  scoped_nsobject<NSOpenGLPixelFormat> pixel_format;
  pixel_format.reset([[NSOpenGLPixelFormat alloc]
                         initWithAttributes:&attributes.front()]);
  if (!pixel_format) {
    LOG(ERROR) << "NSOpenGLPixelFormat initWithAttributes failed.";
    return false;
  }

  context_.reset([[NSOpenGLContext alloc] initWithFormat:pixel_format
                                            shareContext:nil]);
  if (!context_) {
    LOG(ERROR) << "NSOpenGLContext initWithFormat failed";
    return false;
  }

  return true;
}

void GLContextNSView::Destroy() {
  context_.reset(nil);
}

bool GLContextNSView::MakeCurrent(GLSurface* surface) {
  TRACE_EVENT0("gpu", "GLContextNSView::MakeCurrent");
  AcceleratedWidget view =
      static_cast<AcceleratedWidget>(surface->GetHandle());
  // Only set the context's view if the view is parented.
  // I.e. it is a valid drawable.
  if ([view window])
    [context_ setView:view];
  [context_ makeCurrentContext];

  SetCurrent(this, surface);

  if (!surface->OnMakeCurrent(this)) {
    LOG(ERROR) << "Unable to make gl context current.";
    return false;
  }

  return true;
}

void GLContextNSView::ReleaseCurrent(GLSurface* surface) {
  [NSOpenGLContext clearCurrentContext];
}

bool GLContextNSView::IsCurrent(GLSurface* surface) {
  return context_ == [NSOpenGLContext currentContext];
}

void* GLContextNSView::GetHandle() {
  return context_;
}

void GLContextNSView::SetSwapInterval(int interval) {
  DCHECK(interval == 0 || interval == 1);
  GLint swap = interval;
  [context_ setValues:&swap forParameter:NSOpenGLCPSwapInterval];
}

void GLContextNSView::FlushBuffer() {
  [context_ flushBuffer];
}

}  // namespace gfx
