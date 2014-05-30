// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_ASYNC_PIXEL_TRANSFER_DELEGATE_SYNC_H_
#define UI_GL_ASYNC_PIXEL_TRANSFER_DELEGATE_SYNC_H_

#include "ui/gl/async_pixel_transfer_delegate.h"

namespace gfx {

// Class which handles async pixel transfers synchronously.
class AsyncPixelTransferDelegateSync : public AsyncPixelTransferDelegate {
 public:
  AsyncPixelTransferDelegateSync();
  virtual ~AsyncPixelTransferDelegateSync();

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
      AsyncPixelTransferState* transfer_state,
      const AsyncTexSubImage2DParams& tex_params,
      const AsyncMemoryParams& mem_params) OVERRIDE;
  virtual void WaitForTransferCompletion(
      AsyncPixelTransferState* state) OVERRIDE;
  virtual uint32 GetTextureUploadCount() OVERRIDE;
  virtual base::TimeDelta GetTotalTextureUploadTime() OVERRIDE;
  virtual bool ProcessMorePendingTransfers() OVERRIDE;
  virtual bool NeedsProcessMorePendingTransfers() OVERRIDE;

 private:
  int texture_upload_count_;
  base::TimeDelta total_texture_upload_time_;

  DISALLOW_COPY_AND_ASSIGN(AsyncPixelTransferDelegateSync);
};

}  // namespace gfx

#endif  // UI_GL_ASYNC_PIXEL_TRANSFER_DELEGATE_SYNC_H_
