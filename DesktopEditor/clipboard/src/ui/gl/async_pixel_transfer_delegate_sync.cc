// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/async_pixel_transfer_delegate_sync.h"

#include "base/memory/shared_memory.h"

using base::SharedMemory;
using base::SharedMemoryHandle;

namespace gfx {

namespace {

// Gets the address of the data from shared memory.
void* GetAddress(SharedMemory* shared_memory,
                 uint32 shm_size,
                 uint32 shm_data_offset,
                 uint32 shm_data_size) {
  // Memory bounds have already been validated, so there
  // are just DCHECKS here.
  DCHECK(shared_memory);
  DCHECK(shared_memory->memory());
  DCHECK_LE(shm_data_offset + shm_data_size, shm_size);
  return static_cast<int8*>(shared_memory->memory()) + shm_data_offset;
}

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

AsyncPixelTransferDelegateSync::AsyncPixelTransferDelegateSync()
    : texture_upload_count_(0) {
}

AsyncPixelTransferDelegateSync::~AsyncPixelTransferDelegateSync() {}

AsyncPixelTransferState* AsyncPixelTransferDelegateSync::
    CreatePixelTransferState(GLuint texture_id,
                             const AsyncTexImage2DParams& define_params) {
  return new AsyncPixelTransferStateImpl;
}

bool AsyncPixelTransferDelegateSync::BindCompletedAsyncTransfers() {
  // Everything is already bound.
  return false;
}

void AsyncPixelTransferDelegateSync::AsyncNotifyCompletion(
    const AsyncMemoryParams& mem_params,
    const CompletionCallback& callback) {
  callback.Run(mem_params);
}

void AsyncPixelTransferDelegateSync::AsyncTexImage2D(
    AsyncPixelTransferState* transfer_state,
    const AsyncTexImage2DParams& tex_params,
    const AsyncMemoryParams& mem_params,
    const base::Closure& bind_callback) {
  // Save the define params to return later during deferred
  // binding of the transfer texture.
  DCHECK(transfer_state);
  void* data = GetAddress(mem_params.shared_memory,
                          mem_params.shm_size,
                          mem_params.shm_data_offset,
                          mem_params.shm_data_size);
  glTexImage2D(
      tex_params.target,
      tex_params.level,
      tex_params.internal_format,
      tex_params.width,
      tex_params.height,
      tex_params.border,
      tex_params.format,
      tex_params.type,
      data);
  // The texture is already fully bound so just call it now.
  bind_callback.Run();
}

void AsyncPixelTransferDelegateSync::AsyncTexSubImage2D(
    AsyncPixelTransferState* transfer_state,
    const AsyncTexSubImage2DParams& tex_params,
    const AsyncMemoryParams& mem_params) {
  void* data = GetAddress(mem_params.shared_memory,
                          mem_params.shm_size,
                          mem_params.shm_data_offset,
                          mem_params.shm_data_size);
  DCHECK(transfer_state);
  base::TimeTicks begin_time(base::TimeTicks::HighResNow());
  glTexSubImage2D(
      tex_params.target,
      tex_params.level,
      tex_params.xoffset,
      tex_params.yoffset,
      tex_params.width,
      tex_params.height,
      tex_params.format,
      tex_params.type,
      data);
  texture_upload_count_++;
  total_texture_upload_time_ += base::TimeTicks::HighResNow() - begin_time;
}

void AsyncPixelTransferDelegateSync::WaitForTransferCompletion(
    AsyncPixelTransferState* state) {
  // Already done.
}

uint32 AsyncPixelTransferDelegateSync::GetTextureUploadCount() {
  return texture_upload_count_;
}

base::TimeDelta AsyncPixelTransferDelegateSync::GetTotalTextureUploadTime() {
  return total_texture_upload_time_;
}

bool AsyncPixelTransferDelegateSync::ProcessMorePendingTransfers() {
  return false;
}

bool AsyncPixelTransferDelegateSync::NeedsProcessMorePendingTransfers() {
  return false;
}

}  // namespace gfx

