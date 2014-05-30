// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_API_IMPLEMENTATION_H_
#define UI_GL_GL_API_IMPLEMENTATION_H_

#include "base/compiler_specific.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_export.h"

namespace gpu {
namespace gles2 {
class GLES2Decoder;
}
}
namespace gfx {

class GLContext;
class GLSurface;

void InitializeGLBindingsGL();
void InitializeGLExtensionBindingsGL(GLContext* context);
void InitializeDebugGLBindingsGL();
void ClearGLBindingsGL();
void SetGLToRealGLApi();
void SetGLApi(GLApi* api);

class GL_EXPORT GLApiBase : public GLApi {
 public:
  // Include the auto-generated part of this class. We split this because
  // it means we can easily edit the non-auto generated parts right here in
  // this file instead of having to edit some template or the code generator.
  #include "gl_bindings_api_autogen_gl.h"

 protected:
  GLApiBase();
  virtual ~GLApiBase();
  void InitializeBase(DriverGL* driver);

  DriverGL* driver_;
};

// Implemenents the GL API by calling directly into the driver.
class GL_EXPORT RealGLApi : public GLApiBase {
 public:
  RealGLApi();
  virtual ~RealGLApi();
  void Initialize(DriverGL* driver);
};

// Inserts a TRACE for every GL call.
class GL_EXPORT TraceGLApi : public GLApi {
 public:
  TraceGLApi(GLApi* gl_api) : gl_api_(gl_api) { }
  virtual ~TraceGLApi();

  // Include the auto-generated part of this class. We split this because
  // it means we can easily edit the non-auto generated parts right here in
  // this file instead of having to edit some template or the code generator.
  #include "gl_bindings_api_autogen_gl.h"

 private:
  GLApi* gl_api_;
};

// Implementents the GL API using co-operative state restoring.
// Assumes there is only one real GL context and that multiple virtual contexts
// are implemented above it. Restores the needed state from the current context.
class GL_EXPORT VirtualGLApi : public GLApiBase {
 public:
  VirtualGLApi();
  virtual ~VirtualGLApi();
  void Initialize(DriverGL* driver, GLContext* real_context);

  // Sets the current virutal context.
  bool MakeCurrent(GLContext* virtual_context, GLSurface* surface);

  void OnDestroyVirtualContext(GLContext* virtual_context);

  // Overridden functions from GLApiBase
  virtual const GLubyte* glGetStringFn(GLenum name) OVERRIDE;

 private:
  // The real context we're running on.
  GLContext* real_context_;

  // The current virtual context.
  GLContext* current_context_;

  // The supported extensions being advertised for this virtual context.
  std::string extensions_;
};

}  // namespace gfx

#endif  // UI_GL_GL_API_IMPLEMENTATION_H_
