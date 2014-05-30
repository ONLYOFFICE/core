// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_wgl_api_implementation.h"

namespace gfx {

RealWGLApi* g_real_wgl;

void InitializeGLBindingsWGL() {
  g_driver_wgl.InitializeBindings();
  if (!g_real_wgl) {
    g_real_wgl = new RealWGLApi();
  }
  g_real_wgl->Initialize(&g_driver_wgl);
  g_current_wgl_context = g_real_wgl;
}

void InitializeGLExtensionBindingsWGL(GLContext* context) {
  g_driver_wgl.InitializeExtensionBindings(context);
}

void InitializeDebugGLBindingsWGL() {
  g_driver_wgl.InitializeDebugBindings();
}

void ClearGLBindingsWGL() {
  if (g_real_wgl) {
    delete g_real_wgl;
    g_real_wgl = NULL;
  }
  g_current_wgl_context = NULL;
  g_driver_wgl.ClearBindings();
}

WGLApi::WGLApi() {
}

WGLApi::~WGLApi() {
}

WGLApiBase::WGLApiBase()
    : driver_(NULL) {
}

WGLApiBase::~WGLApiBase() {
}

void WGLApiBase::InitializeBase(DriverWGL* driver) {
  driver_ = driver;
}

RealWGLApi::RealWGLApi() {
}

RealWGLApi::~RealWGLApi() {
}

void RealWGLApi::Initialize(DriverWGL* driver) {
  InitializeBase(driver);
}

TraceWGLApi::~TraceWGLApi() {
}

}  // namespace gfx


