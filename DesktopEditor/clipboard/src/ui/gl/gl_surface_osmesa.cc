// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/logging.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_surface_osmesa.h"

namespace gfx {

GLSurfaceOSMesa::GLSurfaceOSMesa(unsigned format, const gfx::Size& size)
    : format_(format),
      size_(size) {
}

bool GLSurfaceOSMesa::Initialize() {
  return Resize(size_);
}

void GLSurfaceOSMesa::Destroy() {
  buffer_.reset();
}

bool GLSurfaceOSMesa::Resize(const gfx::Size& new_size) {
  GLContext* current_context = GLContext::GetCurrent();
  bool was_current = current_context && current_context->IsCurrent(this);
  if (was_current)
    current_context->ReleaseCurrent(this);

  // Preserve the old buffer.
  scoped_ptr<int32[]> old_buffer(buffer_.release());

  // Allocate a new one.
  buffer_.reset(new int32[new_size.GetArea()]);
  memset(buffer_.get(), 0, new_size.GetArea() * sizeof(buffer_[0]));

  // Copy the old back buffer into the new buffer.
  if (old_buffer.get()) {
    int copy_width = std::min(size_.width(), new_size.width());
    int copy_height = std::min(size_.height(), new_size.height());
    for (int y = 0; y < copy_height; ++y) {
      for (int x = 0; x < copy_width; ++x) {
        buffer_[y * new_size.width() + x] = old_buffer[y * size_.width() + x];
      }
    }
  }

  size_ = new_size;

  if (was_current)
    return current_context->MakeCurrent(this);

  return true;
}

bool GLSurfaceOSMesa::IsOffscreen() {
  return true;
}

bool GLSurfaceOSMesa::SwapBuffers() {
  NOTREACHED() << "Should not call SwapBuffers on an GLSurfaceOSMesa.";
  return false;
}

gfx::Size GLSurfaceOSMesa::GetSize() {
  return size_;
}

void* GLSurfaceOSMesa::GetHandle() {
  return buffer_.get();
}

unsigned GLSurfaceOSMesa::GetFormat() {
  return format_;
}

GLSurfaceOSMesa::~GLSurfaceOSMesa() {
  Destroy();
}

}  // namespace gfx
