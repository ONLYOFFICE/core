// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_gl_api_implementation.h"

#include <algorithm>
#include <vector>

#include "base/command_line.h"
#include "base/string_util.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_state_restorer.h"
#include "ui/gl/gl_surface.h"
#include "ui/gl/gl_switches.h"

namespace gfx {

// The GL Api being used. This could be g_real_gl or gl_trace_gl
static GLApi* g_gl;
// A GL Api that calls directly into the driver.
static RealGLApi* g_real_gl;
// A GL Api that calls TRACE and then calls another GL api.
static TraceGLApi* g_trace_gl;

void InitializeGLBindingsGL() {
  g_driver_gl.InitializeBindings();
  if (!g_real_gl) {
    g_real_gl = new RealGLApi();
    g_trace_gl = new TraceGLApi(g_real_gl);
  }
  g_real_gl->Initialize(&g_driver_gl);
  g_gl = g_real_gl;
  if (CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableGPUServiceTracing)) {
    g_gl = g_trace_gl;
  }
  SetGLToRealGLApi();
}

GLApi* GetCurrentGLApi() {
  return g_current_gl_context;
}

void SetGLApi(GLApi* api) {
  g_current_gl_context = api;
}

void SetGLToRealGLApi() {
  SetGLApi(g_gl);
}

void InitializeGLExtensionBindingsGL(GLContext* context) {
  g_driver_gl.InitializeExtensionBindings(context);
}

void InitializeDebugGLBindingsGL() {
  g_driver_gl.InitializeDebugBindings();
}

void ClearGLBindingsGL() {
  if (g_real_gl) {
    delete g_real_gl;
    g_real_gl = NULL;
  }
  if (g_trace_gl) {
    delete g_trace_gl;
    g_trace_gl = NULL;
  }
  g_gl = NULL;
  g_current_gl_context = NULL;
  g_driver_gl.ClearBindings();
}

GLApi::GLApi() {
}

GLApi::~GLApi() {
}

GLApiBase::GLApiBase()
    : driver_(NULL) {
}

GLApiBase::~GLApiBase() {
}

void GLApiBase::InitializeBase(DriverGL* driver) {
  driver_ = driver;
}

RealGLApi::RealGLApi() {
}

RealGLApi::~RealGLApi() {
}

void RealGLApi::Initialize(DriverGL* driver) {
  InitializeBase(driver);
}

TraceGLApi::~TraceGLApi() {
}

VirtualGLApi::VirtualGLApi()
    : real_context_(NULL),
      current_context_(NULL) {
}

VirtualGLApi::~VirtualGLApi() {
}

void VirtualGLApi::Initialize(DriverGL* driver, GLContext* real_context) {
  InitializeBase(driver);
  real_context_ = real_context;

  DCHECK(real_context->IsCurrent(NULL));
  std::string ext_string(
      reinterpret_cast<const char*>(driver_->fn.glGetStringFn(GL_EXTENSIONS)));
  std::vector<std::string> ext;
  Tokenize(ext_string, " ", &ext);

  std::vector<std::string>::iterator it;
  // We can't support GL_EXT_occlusion_query_boolean which is
  // based on GL_ARB_occlusion_query without a lot of work virtualizing
  // queries.
  it = std::find(ext.begin(), ext.end(), "GL_EXT_occlusion_query_boolean");
  if (it != ext.end())
    ext.erase(it);

  extensions_ = JoinString(ext, " ");
}

bool VirtualGLApi::MakeCurrent(GLContext* virtual_context, GLSurface* surface) {
  bool switched_contexts = g_current_gl_context != this;
  GLSurface* current_surface = GLSurface::GetCurrent();
  if (switched_contexts || surface != current_surface) {
    if (!switched_contexts && current_surface &&
        virtual_context->IsCurrent(surface)) {
      // MakeCurrent 'lite' path that avoids potentially expensive MakeCurrent()
      // calls if the GLSurface uses the same underlying surface or renders to
      // an FBO.
      if (!surface->OnMakeCurrent(real_context_)) {
        LOG(ERROR) << "Could not make GLSurface current.";
        return false;
      }
    } else if (!real_context_->MakeCurrent(surface)) {
      return false;
    }
  }

  DCHECK(GLSurface::GetCurrent());
  DCHECK(real_context_->IsCurrent(GLSurface::GetCurrent()));
  DCHECK(virtual_context->IsCurrent(surface));

  if (switched_contexts || virtual_context != current_context_) {
    // There should be no errors from the previous context leaking into the
    // new context.
    DCHECK_EQ(glGetErrorFn(), static_cast<GLenum>(GL_NO_ERROR));

    current_context_ = virtual_context;
    // Set all state that is different from the real state
    // NOTE: !!! This is a temporary implementation that just restores all
    // state to let us test that it works.
    // TODO: ASAP, change this to something that only restores the state
    // needed for individual GL calls.
    GLApi* temp = GetCurrentGLApi();
    SetGLToRealGLApi();
    virtual_context->GetGLStateRestorer()->RestoreState();
    SetGLApi(temp);
  }
  SetGLApi(this);
  return true;
}

void VirtualGLApi::OnDestroyVirtualContext(GLContext* virtual_context) {
  if (current_context_ == virtual_context)
    current_context_ = NULL;
}

const GLubyte* VirtualGLApi::glGetStringFn(GLenum name) {
  switch (name) {
    case GL_EXTENSIONS:
      return reinterpret_cast<const GLubyte*>(extensions_.c_str());
    default:
      return driver_->fn.glGetStringFn(name);
  }
}

}  // namespace gfx
