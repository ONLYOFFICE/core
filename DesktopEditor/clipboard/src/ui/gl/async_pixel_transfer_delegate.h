// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GL_ASYNC_PIXEL_TRANSFER_DELEGATE_H_
#define UI_GL_ASYNC_PIXEL_TRANSFER_DELEGATE_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/time.h"
#include "ui/gl/gl_bindings.h"
#include "ui/gl/gl_export.h"

namespace base {
class SharedMemory;
}

namespace gfx {

struct AsyncTexImage2DParams {
  GLenum target;
  GLint level;
  GLenum internal_format;
  GLsizei width;
  GLsizei height;
  GLint border;
  GLenum format;
  GLenum type;
};

struct AsyncTexSubImage2DParams {
  GLenum target;
  GLint level;
  GLint xoffset;
  GLint yoffset;
  GLsizei width;
  GLsizei height;
  GLenum format;
  GLenum type;
};

struct AsyncMemoryParams {
  base::SharedMemory* shared_memory;
  uint32 shm_size;
  uint32 shm_data_offset;
  uint32 shm_data_size;
};

// AsyncPixelTransferState holds the resources required to do async
// transfers on one texture. It should stay alive for the lifetime
// of the texture to allow multiple transfers.
class GL_EXPORT AsyncPixelTransferState :
    public base::SupportsWeakPtr<AsyncPixelTransferState> {
 public:
  virtual ~AsyncPixelTransferState();

  // Returns true if there is a transfer in progress.
  virtual bool TransferIsInProgress() = 0;

 protected:
  friend class base::RefCounted<AsyncPixelTransferState>;
  AsyncPixelTransferState();

 private:
  DISALLOW_COPY_AND_ASSIGN(AsyncPixelTransferState);
};

class GL_EXPORT AsyncPixelTransferDelegate {
 public:
  typedef base::Callback<void(const AsyncMemoryParams&)> CompletionCallback;

  static AsyncPixelTransferDelegate* Create(gfx::GLContext* context);

  virtual ~AsyncPixelTransferDelegate();

  virtual AsyncPixelTransferState* CreatePixelTransferState(
      GLuint texture_id,
      const AsyncTexImage2DParams& define_params) = 0;

  // Returns true iff a texture was bound to texture-unit zero.
  virtual bool BindCompletedAsyncTransfers() = 0;

  // There's no guarantee that callback will run on the caller thread.
  virtual void AsyncNotifyCompletion(
      const AsyncMemoryParams& mem_params,
      const CompletionCallback& callback) = 0;

  // The callback occurs on the caller thread, once the texture is
  // safe/ready to be used.
  virtual void AsyncTexImage2D(
      AsyncPixelTransferState* state,
      const AsyncTexImage2DParams& tex_params,
      const AsyncMemoryParams& mem_params,
      const base::Closure& bind_callback) = 0;

  virtual void AsyncTexSubImage2D(
      AsyncPixelTransferState* state,
      const AsyncTexSubImage2DParams& tex_params,
      const AsyncMemoryParams& mem_params) = 0;

  // Block until the specified transfer completes.
  virtual void WaitForTransferCompletion(
      AsyncPixelTransferState* state) = 0;

  virtual uint32 GetTextureUploadCount() = 0;
  virtual base::TimeDelta GetTotalTextureUploadTime() = 0;

  // ProcessMorePendingTransfers() will be called at a good time
  // to process a small amount of pending transfer work while
  // NeedsProcessMorePendingTransfers() returns true. Implementations
  // that can't dispatch work to separate threads should use
  // this to avoid blocking the caller thread inappropriately.
  // ProcessMorePendingTransfers() returns true iff a texture was
  // bound to texture-unit zero.
  virtual bool ProcessMorePendingTransfers() = 0;
  virtual bool NeedsProcessMorePendingTransfers() = 0;

 protected:
  AsyncPixelTransferDelegate();

 private:
  DISALLOW_COPY_AND_ASSIGN(AsyncPixelTransferDelegate);
};

}  // namespace gfx

#endif  // UI_GL_ASYNC_PIXEL_TRANSFER_DELEGATE_H_

