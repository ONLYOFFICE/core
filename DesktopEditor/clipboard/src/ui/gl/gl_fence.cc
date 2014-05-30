// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_fence.h"

#include "base/compiler_specific.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_context.h"

namespace {

class GLFenceNVFence: public gfx::GLFence {
 public:
  GLFenceNVFence() {
    // What if either of these GL calls fails? TestFenceNV will return true.
    // See spec:
    // http://www.opengl.org/registry/specs/NV/fence.txt
    //
    // What should happen if TestFenceNV is called for a name before SetFenceNV
    // is called?
    //     We generate an INVALID_OPERATION error, and return TRUE.
    //     This follows the semantics for texture object names before
    //     they are bound, in that they acquire their state upon binding.
    //     We will arbitrarily return TRUE for consistency.
    glGenFencesNV(1, &fence_);
    glSetFenceNV(fence_, GL_ALL_COMPLETED_NV);
    glFlush();
  }

  virtual bool HasCompleted() OVERRIDE {
    return !!glTestFenceNV(fence_);
  }

 private:
  virtual ~GLFenceNVFence() {
    glDeleteFencesNV(1, &fence_);
  }

  GLuint fence_;
};

class GLFenceARBSync: public gfx::GLFence {
 public:
  GLFenceARBSync() {
    sync_ = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    glFlush();
  }

  virtual bool HasCompleted() OVERRIDE {
    // Handle the case where FenceSync failed.
    if (!sync_)
      return true;

    GLsizei length = 0;
    GLsizei value = 0;
    glGetSynciv(sync_,
                GL_SYNC_STATUS,
                1,  // bufSize
                &length,
                &value);
    return length == 1 && value == GL_SIGNALED;
  }

 private:
  virtual ~GLFenceARBSync() {
    glDeleteSync(sync_);
  }

  GLsync sync_;
};

}  // namespace

namespace gfx {

GLFence::GLFence() {
}

GLFence::~GLFence() {
}

// static
GLFence* GLFence::Create() {
  if (gfx::g_driver_gl.ext.b_GL_NV_fence) {
    return new GLFenceNVFence();
  } else if (gfx::g_driver_gl.ext.b_GL_ARB_sync) {
    return new GLFenceARBSync();
  } else {
    return NULL;
  }
}

}  // namespace gfx
