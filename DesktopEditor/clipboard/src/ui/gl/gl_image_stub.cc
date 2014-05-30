// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/gl_image_stub.h"

namespace gfx {

void GLImageStub::Destroy() {
}

gfx::Size GLImageStub::GetSize() {
  return gfx::Size(1, 1);
}

bool GLImageStub::BindTexImage() {
  return true;
}

void GLImageStub::ReleaseTexImage() {
}

GLImageStub::~GLImageStub() {}

}  // namespace gfx
