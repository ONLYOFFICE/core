// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_egl_api_implementation.h"

namespace gfx {

RealEGLApi* g_real_egl;

void InitializeGLBindingsEGL() {
  g_driver_egl.InitializeBindings();
  if (!g_real_egl) {
    g_real_egl = new RealEGLApi();
  }
  g_real_egl->Initialize(&g_driver_egl);
  g_current_egl_context = g_real_egl;
}

void InitializeGLExtensionBindingsEGL(GLContext* context) {
  g_driver_egl.InitializeExtensionBindings(context);
}

void InitializeDebugGLBindingsEGL() {
  g_driver_egl.InitializeDebugBindings();
}

void ClearGLBindingsEGL() {
  if (g_real_egl) {
    delete g_real_egl;
    g_real_egl = NULL;
  }
  g_current_egl_context = NULL;
  g_driver_egl.ClearBindings();
}

EGLApi::EGLApi() {
}

EGLApi::~EGLApi() {
}

EGLApiBase::EGLApiBase()
    : driver_(NULL) {
}

EGLApiBase::~EGLApiBase() {
}

void EGLApiBase::InitializeBase(DriverEGL* driver) {
  driver_ = driver;
}

RealEGLApi::RealEGLApi() {
}

RealEGLApi::~RealEGLApi() {
}

void RealEGLApi::Initialize(DriverEGL* driver) {
  InitializeBase(driver);
}

TraceEGLApi::~TraceEGLApi() {
}

}  // namespace gfx


