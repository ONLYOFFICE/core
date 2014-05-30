// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_glx_api_implementation.h"

namespace gfx {

RealGLXApi* g_real_glx;

void InitializeGLBindingsGLX() {
  g_driver_glx.InitializeBindings();
  if (!g_real_glx) {
    g_real_glx = new RealGLXApi();
  }
  g_real_glx->Initialize(&g_driver_glx);
  g_current_glx_context = g_real_glx;
}

void InitializeGLExtensionBindingsGLX(GLContext* context) {
  g_driver_glx.InitializeExtensionBindings(context);
}

void InitializeDebugGLBindingsGLX() {
  g_driver_glx.InitializeDebugBindings();
}

void ClearGLBindingsGLX() {
  if (g_real_glx) {
    delete g_real_glx;
    g_real_glx = NULL;
  }
  g_current_glx_context = NULL;
  g_driver_glx.ClearBindings();
}

GLXApi::GLXApi() {
}

GLXApi::~GLXApi() {
}

GLXApiBase::GLXApiBase()
    : driver_(NULL) {
}

GLXApiBase::~GLXApiBase() {
}

void GLXApiBase::InitializeBase(DriverGLX* driver) {
  driver_ = driver;
}

RealGLXApi::RealGLXApi() {
}

RealGLXApi::~RealGLXApi() {
}

void RealGLXApi::Initialize(DriverGLX* driver) {
  InitializeBase(driver);
}

TraceGLXApi::~TraceGLXApi() {
}

}  // namespace gfx


