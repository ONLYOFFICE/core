// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_GL_INTERFACE_H_
#define UI_GL_GL_INTERFACE_H_

// This file implements glue to a GL interface so we can mock it for unit
// testing. It has to be Desktop GL, not GLES2 as it is used to test the service
// side code.

#include "ui/gl/gl_bindings.h"

namespace gfx {

class GL_EXPORT GLInterface {
 public:
  virtual ~GLInterface() {}

  static void SetGLInterface(GLInterface* gl_interface);

  static GLInterface* GetGLInterface();

  // Include the auto-generated part of this class. We split this because
  // it means we can easily edit the non-auto generated parts right here in
  // this file instead of having to edit some template or the code generator.
  #include "gl_interface_autogen_gl.h"

 private:
  static GLInterface* interface_;
};

}  // namespace gfx

#endif  // UI_GL_GL_INTERFACE_H_
