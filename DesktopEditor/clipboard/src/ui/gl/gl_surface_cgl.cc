// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_surface_cgl.h"

#include <OpenGL/CGLRenderers.h>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/mac/mac_util.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_implementation.h"
#include "ui/gl/gpu_switching_manager.h"

namespace gfx {

namespace {
CGLPixelFormatObj g_pixel_format;
}

GLSurfaceCGL::GLSurfaceCGL() {}

bool GLSurfaceCGL::InitializeOneOff() {
  static bool initialized = false;
  if (initialized)
    return true;

  // This is called from the sandbox warmup code on Mac OS X.
  // GPU-related stuff is very slow without this, probably because
  // the sandbox prevents loading graphics drivers or some such.
  std::vector<CGLPixelFormatAttribute> attribs;
  if (ui::GpuSwitchingManager::GetInstance()->SupportsDualGpus()) {
    // Avoid switching to the discrete GPU just for this pixel
    // format selection.
    attribs.push_back(kCGLPFAAllowOfflineRenderers);
  }
  if (GetGLImplementation() == kGLImplementationAppleGL) {
    attribs.push_back(kCGLPFARendererID);
    attribs.push_back(static_cast<CGLPixelFormatAttribute>(
      kCGLRendererGenericFloatID));
  }
  attribs.push_back(static_cast<CGLPixelFormatAttribute>(0));

  CGLPixelFormatObj format;
  GLint num_pixel_formats;
  if (CGLChoosePixelFormat(&attribs.front(),
                           &format,
                           &num_pixel_formats) != kCGLNoError) {
    LOG(ERROR) << "Error choosing pixel format.";
    return false;
  }
  if (!format) {
    LOG(ERROR) << "format == 0.";
    return false;
  }
  CGLReleasePixelFormat(format);
  DCHECK_NE(num_pixel_formats, 0);
  initialized = true;
  return true;
}

void* GLSurfaceCGL::GetPixelFormat() {
  return g_pixel_format;
}

GLSurfaceCGL::~GLSurfaceCGL() {}

NoOpGLSurfaceCGL::NoOpGLSurfaceCGL(const gfx::Size& size)
  : size_(size) {
}

bool NoOpGLSurfaceCGL::Initialize() {
  return true;
}

void NoOpGLSurfaceCGL::Destroy() {
}

bool NoOpGLSurfaceCGL::IsOffscreen() {
  return true;
}

bool NoOpGLSurfaceCGL::SwapBuffers() {
  NOTREACHED() << "Cannot call SwapBuffers on a NoOpGLSurfaceCGL.";
  return false;
}

gfx::Size NoOpGLSurfaceCGL::GetSize() {
  return size_;
}

void* NoOpGLSurfaceCGL::GetHandle() {
  return NULL;
}

void* NoOpGLSurfaceCGL::GetDisplay() {
  return NULL;
}

NoOpGLSurfaceCGL::~NoOpGLSurfaceCGL() {
  Destroy();
}

}  // namespace gfx
