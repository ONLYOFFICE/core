// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file implements mock GL Interface for unit testing. It has to mock
// Desktop GL, not GLES2 as it is used to test the service side code.

#ifndef UI_GL_GL_MOCK_H_
#define UI_GL_GL_MOCK_H_

#include "testing/gmock/include/gmock/gmock.h"
#include "ui/gl/gl_interface.h"

namespace gfx {

class MockGLInterface : public GLInterface {
 public:
  MockGLInterface();
  virtual ~MockGLInterface();

  // Include the auto-generated part of this class. We split this because
  // it means we can easily edit the non-auto generated parts right here in
  // this file instead of having to edit some template or the code generator.
  #include "gl_mock_autogen_gl.h"
};

}  // namespace gfx

#endif  // UI_GL_GL_MOCK_H_
