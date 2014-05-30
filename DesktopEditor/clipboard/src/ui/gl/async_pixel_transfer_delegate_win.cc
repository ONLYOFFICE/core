// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/async_pixel_transfer_delegate.h"

#include "base/debug/trace_event.h"
#include "ui/gl/async_pixel_transfer_delegate_idle.h"
#include "ui/gl/async_pixel_transfer_delegate_stub.h"
#include "ui/gl/gl_implementation.h"

namespace gfx {

AsyncPixelTransferDelegate* AsyncPixelTransferDelegate::Create(
    gfx::GLContext* context) {
  TRACE_EVENT0("gpu", "AsyncPixelTransferDelegate::Create");
  switch (GetGLImplementation()) {
    case kGLImplementationOSMesaGL:
    case kGLImplementationDesktopGL:
    case kGLImplementationEGLGLES2:
      return new AsyncPixelTransferDelegateIdle;
    case kGLImplementationMockGL:
      return new AsyncPixelTransferDelegateStub;
    default:
      NOTREACHED();
      return NULL;
  }
}

}  // namespace gfx
