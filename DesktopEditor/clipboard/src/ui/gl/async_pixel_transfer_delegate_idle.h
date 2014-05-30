// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_ASYNC_PIXEL_TRANSFER_DELEGATE_IDLE_H_
#define UI_GL_ASYNC_PIXEL_TRANSFER_DELEGATE_IDLE_H_

#include <list>

#include "ui/gl/async_pixel_transfer_delegate.h"

namespace gfx {
class ScopedSafeSharedMemory;

// Class which handles async pixel transfers in a platform
// independent way.
class AsyncPixelTransferDelegateIdle : public AsyncPixelTransferDelegate,
    public base::SupportsWeakPtr<AsyncPixelTransferDelegateIdle> {
 public:
  AsyncPixelTransferDelegateIdle();
  virtual ~AsyncPixelTransferDelegateIdle();

  // implement AsyncPixelTransferDelegate:
  virtual AsyncPixelTransferState* CreatePixelTransferState(
      GLuint texture_id,
      const AsyncTexImage2DParams& define_params) OVERRIDE;
  virtual bool BindCompletedAsyncTransfers() OVERRIDE;
  virtual void AsyncNotifyCompletion(
      const AsyncMemoryParams& mem_params,
      const CompletionCallback& callback) OVERRIDE;
  virtual void AsyncTexImage2D(
      AsyncPixelTransferState* transfer_state,
      const AsyncTexImage2DParams& tex_params,
      const AsyncMemoryParams& mem_params,
      const base::Closure& bind_callback) OVERRIDE;
  virtual void AsyncTexSubImage2D(
      AsyncPixelTransferState* transfer_state,
      const AsyncTexSubImage2DParams& tex_params,
      const AsyncMemoryParams& mem_params) OVERRIDE;
  virtual void WaitForTransferCompletion(
      AsyncPixelTransferState* transfer_state) OVERRIDE;
  virtual uint32 GetTextureUploadCount() OVERRIDE;
  virtual base::TimeDelta GetTotalTextureUploadTime() OVERRIDE;
  virtual bool ProcessMorePendingTransfers() OVERRIDE;
  virtual bool NeedsProcessMorePendingTransfers() OVERRIDE;

 private:
  struct Task {
    Task(uint64 transfer_id, const base::Closure& task);
    ~Task();

    // This is non-zero if pixel transfer task.
    uint64 transfer_id;

    base::Closure task;
  };

  void ProcessNotificationTasks();

  void PerformNotifyCompletion(
      AsyncMemoryParams mem_params,
      ScopedSafeSharedMemory* safe_shared_memory,
      const CompletionCallback& callback);
  void PerformAsyncTexImage2D(
      AsyncTexImage2DParams tex_params,
      AsyncMemoryParams mem_params,
      const base::Closure& bind_callback,
      ScopedSafeSharedMemory* safe_shared_memory,
      GLuint texture_id);
  void PerformAsyncTexSubImage2D(
      AsyncTexSubImage2DParams tex_params,
      AsyncMemoryParams mem_params,
      ScopedSafeSharedMemory* safe_shared_memory,
      GLuint texture_id);

  int texture_upload_count_;
  base::TimeDelta total_texture_upload_time_;

  std::list<Task> tasks_;

  DISALLOW_COPY_AND_ASSIGN(AsyncPixelTransferDelegateIdle);
};

}  // namespace gfx

#endif  // UI_GL_ASYNC_PIXEL_TRANSFER_DELEGATE_IDLE_H_
