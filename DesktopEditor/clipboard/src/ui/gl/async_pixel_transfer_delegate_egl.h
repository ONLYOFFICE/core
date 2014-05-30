// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_ASYNC_PIXEL_TRANSFER_DELEGATE_EGL_H_
#define UI_GL_ASYNC_PIXEL_TRANSFER_DELEGATE_EGL_H_

#include <list>

#include "ui/gl/async_pixel_transfer_delegate.h"

namespace gfx {
class ScopedSafeSharedMemory;
class TextureUploadStats;
class TransferStateInternal;

// Class which handles async pixel transfers using EGLImageKHR and another
// upload thread
class AsyncPixelTransferDelegateEGL
    : public AsyncPixelTransferDelegate,
      public base::SupportsWeakPtr<AsyncPixelTransferDelegateEGL> {
 public:
  AsyncPixelTransferDelegateEGL();
  virtual ~AsyncPixelTransferDelegateEGL();

  // Implement AsyncPixelTransferDelegate:
  virtual AsyncPixelTransferState* CreatePixelTransferState(
      GLuint texture_id,
      const AsyncTexImage2DParams& define_params) OVERRIDE;
  virtual bool BindCompletedAsyncTransfers() OVERRIDE;
  virtual void AsyncNotifyCompletion(
      const AsyncMemoryParams& mem_params,
      const CompletionCallback& callback) OVERRIDE;
  virtual void AsyncTexImage2D(
      AsyncPixelTransferState* state,
      const AsyncTexImage2DParams& tex_params,
      const AsyncMemoryParams& mem_params,
      const base::Closure& bind_callback) OVERRIDE;
  virtual void AsyncTexSubImage2D(
      AsyncPixelTransferState* state,
      const AsyncTexSubImage2DParams& tex_params,
      const AsyncMemoryParams& mem_params) OVERRIDE;
  virtual void WaitForTransferCompletion(
      AsyncPixelTransferState* state) OVERRIDE;
  virtual uint32 GetTextureUploadCount() OVERRIDE;
  virtual base::TimeDelta GetTotalTextureUploadTime() OVERRIDE;
  virtual bool ProcessMorePendingTransfers() OVERRIDE;
  virtual bool NeedsProcessMorePendingTransfers() OVERRIDE;

 private:
  static void PerformNotifyCompletion(
      AsyncMemoryParams mem_params,
      ScopedSafeSharedMemory* safe_shared_memory,
      const CompletionCallback& callback);
  static void PerformAsyncTexImage2D(
      TransferStateInternal* state,
      AsyncTexImage2DParams tex_params,
      AsyncMemoryParams mem_params,
      ScopedSafeSharedMemory* safe_shared_memory);
  static void PerformAsyncTexSubImage2D(
      TransferStateInternal* state,
      AsyncTexSubImage2DParams tex_params,
      AsyncMemoryParams mem_params,
      ScopedSafeSharedMemory* safe_shared_memory,
      scoped_refptr<TextureUploadStats> texture_upload_stats);

  // Returns true if a work-around was used.
  bool WorkAroundAsyncTexImage2D(
      AsyncPixelTransferState* state,
      const AsyncTexImage2DParams& tex_params,
      const AsyncMemoryParams& mem_params,
      const base::Closure& bind_callback);
  bool WorkAroundAsyncTexSubImage2D(
      AsyncPixelTransferState* state,
      const AsyncTexSubImage2DParams& tex_params,
      const AsyncMemoryParams& mem_params);

  typedef std::list<base::WeakPtr<AsyncPixelTransferState> > TransferQueue;
  TransferQueue pending_allocations_;

  scoped_refptr<TextureUploadStats> texture_upload_stats_;
  bool is_imagination_;
  bool is_qualcomm_;

  DISALLOW_COPY_AND_ASSIGN(AsyncPixelTransferDelegateEGL);
};

}  // namespace gfx

#endif  // UI_GL_ASYNC_PIXEL_TRANSFER_DELEGATE_EGL_H_
