// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/async_pixel_transfer_delegate_stub.h"

namespace gfx {

namespace {

class AsyncPixelTransferStateImpl : public AsyncPixelTransferState {
 public:
  AsyncPixelTransferStateImpl() {}
  virtual ~AsyncPixelTransferStateImpl() {}

  // Implement AsyncPixelTransferState:
  virtual bool TransferIsInProgress() OVERRIDE {
    return false;
  }
};

}  // namespace

AsyncPixelTransferDelegateStub::AsyncPixelTransferDelegateStub() {}

AsyncPixelTransferDelegateStub::~AsyncPixelTransferDelegateStub() {}

AsyncPixelTransferState* AsyncPixelTransferDelegateStub::
    CreatePixelTransferState(GLuint texture_id,
                             const AsyncTexImage2DParams& define_params) {
  return new AsyncPixelTransferStateImpl;
}

bool AsyncPixelTransferDelegateStub::BindCompletedAsyncTransfers() {
  return false;
}

void AsyncPixelTransferDelegateStub::AsyncNotifyCompletion(
    const AsyncMemoryParams& mem_params,
    const CompletionCallback& callback) {
  callback.Run(mem_params);
}

void AsyncPixelTransferDelegateStub::AsyncTexImage2D(
    AsyncPixelTransferState* transfer_state,
    const AsyncTexImage2DParams& tex_params,
    const AsyncMemoryParams& mem_params,
    const base::Closure& bind_callback) {
  bind_callback.Run();
}

void AsyncPixelTransferDelegateStub::AsyncTexSubImage2D(
    AsyncPixelTransferState* transfer_state,
    const AsyncTexSubImage2DParams& tex_params,
    const AsyncMemoryParams& mem_params) {
}

void AsyncPixelTransferDelegateStub::WaitForTransferCompletion(
    AsyncPixelTransferState* state) {
}

uint32 AsyncPixelTransferDelegateStub::GetTextureUploadCount() {
  return 0;
}

base::TimeDelta AsyncPixelTransferDelegateStub::GetTotalTextureUploadTime() {
  return base::TimeDelta();
}

bool AsyncPixelTransferDelegateStub::ProcessMorePendingTransfers() {
  return false;
}

bool AsyncPixelTransferDelegateStub::NeedsProcessMorePendingTransfers() {
  return false;
}

}  // namespace gfx

