// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gl/async_pixel_transfer_delegate_egl.h"

#include <string>

#include "base/bind.h"
#include "base/debug/trace_event.h"
#include "base/lazy_instance.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/shared_memory.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_surface_egl.h"
#include "ui/gl/safe_shared_memory_pool.h"

#if defined(OS_ANDROID)
// TODO(epenner): Move thread priorities to base. (crbug.com/170549)
#include <sys/resource.h>
#endif

using base::SharedMemory;
using base::SharedMemoryHandle;

namespace gfx {

namespace {

bool CheckErrors(const char* file, int line) {
  EGLint eglerror;
  GLenum glerror;
  bool success = true;
  while ((eglerror = eglGetError()) != EGL_SUCCESS) {
     LOG(ERROR) << "Async transfer EGL error at "
                << file << ":" << line << " " << eglerror;
     success = false;
  }
  while ((glerror = glGetError()) != GL_NO_ERROR) {
     LOG(ERROR) << "Async transfer OpenGL error at "
                << file << ":" << line << " " << glerror;
     success = false;
  }
  return success;
}
#define CHECK_GL() CheckErrors(__FILE__, __LINE__)

const char kAsyncTransferThreadName[] = "AsyncTransferThread";

// Regular glTexImage2D call.
void DoTexImage2D(const AsyncTexImage2DParams& tex_params, void* data) {
  glTexImage2D(
      GL_TEXTURE_2D, tex_params.level, tex_params.internal_format,
      tex_params.width, tex_params.height,
      tex_params.border, tex_params.format, tex_params.type, data);
}

// Regular glTexSubImage2D call.
void DoTexSubImage2D(const AsyncTexSubImage2DParams& tex_params, void* data) {
  glTexSubImage2D(
      GL_TEXTURE_2D, tex_params.level,
      tex_params.xoffset, tex_params.yoffset,
      tex_params.width, tex_params.height,
      tex_params.format, tex_params.type, data);
}

// Full glTexSubImage2D call, from glTexImage2D params.
void DoFullTexSubImage2D(const AsyncTexImage2DParams& tex_params, void* data) {
  glTexSubImage2D(
      GL_TEXTURE_2D, tex_params.level,
      0, 0, tex_params.width, tex_params.height,
      tex_params.format, tex_params.type, data);
}

// Gets the address of the data from shared memory.
void* GetAddress(SharedMemory* shared_memory, uint32 shm_data_offset) {
  // Memory bounds have already been validated, so there
  // are just DCHECKS here.
  CHECK(shared_memory);
  CHECK(shared_memory->memory());
  return static_cast<int8*>(shared_memory->memory()) + shm_data_offset;
}

class TransferThread : public base::Thread {
 public:
  TransferThread() : base::Thread(kAsyncTransferThreadName) {
    Start();
  }
  virtual ~TransferThread() {
    Stop();
  }

  virtual void Init() OVERRIDE {
    GLShareGroup* share_group = NULL;
    bool software = false;
    surface_ = new gfx::PbufferGLSurfaceEGL(software, gfx::Size(1,1));
    surface_->Initialize();
    context_ = gfx::GLContext::CreateGLContext(share_group,
                                               surface_,
                                               gfx::PreferDiscreteGpu);
    bool is_current = context_->MakeCurrent(surface_);
    DCHECK(is_current);

#if defined(OS_ANDROID)
    // TODO(epenner): Move thread priorities to base. (crbug.com/170549)
    int nice_value = 10; // Idle priority.
    setpriority(PRIO_PROCESS, base::PlatformThread::CurrentId(), nice_value);
#endif

  }

  virtual void CleanUp() OVERRIDE {
    surface_ = NULL;
    context_->ReleaseCurrent(surface_);
    context_ = NULL;
  }

  SafeSharedMemoryPool* safe_shared_memory_pool() {
      return &safe_shared_memory_pool_;
  }

 private:
  scoped_refptr<gfx::GLContext> context_;
  scoped_refptr<gfx::GLSurface> surface_;

  SafeSharedMemoryPool safe_shared_memory_pool_;

  DISALLOW_COPY_AND_ASSIGN(TransferThread);
};

base::LazyInstance<TransferThread>
    g_transfer_thread = LAZY_INSTANCE_INITIALIZER;

base::MessageLoopProxy* transfer_message_loop_proxy() {
  return g_transfer_thread.Pointer()->message_loop_proxy();
}

SafeSharedMemoryPool* safe_shared_memory_pool() {
  return g_transfer_thread.Pointer()->safe_shared_memory_pool();
}

}  // namespace

// Class which holds async pixel transfers state (EGLImage).
// The EGLImage is accessed by either thread, but everything
// else accessed only on the main thread.
class TransferStateInternal
    : public base::RefCountedThreadSafe<TransferStateInternal> {
 public:
  TransferStateInternal(GLuint texture_id,
                        const AsyncTexImage2DParams& define_params,
                        bool wait_for_uploads,
                        bool wait_for_creation,
                        bool use_image_preserved)
      : texture_id_(texture_id),
        thread_texture_id_(0),
        transfer_completion_(true, true),
        egl_image_(EGL_NO_IMAGE_KHR),
        wait_for_uploads_(wait_for_uploads),
        wait_for_creation_(wait_for_creation),
        use_image_preserved_(use_image_preserved) {
    define_params_ = define_params;
  }

  bool TransferIsInProgress() {
    return !transfer_completion_.IsSignaled();
  }

  void BindTransfer() {
    TRACE_EVENT2("gpu", "BindAsyncTransfer glEGLImageTargetTexture2DOES",
                 "width", define_params_.width,
                 "height", define_params_.height);
    DCHECK(texture_id_);
    DCHECK_NE(EGL_NO_IMAGE_KHR, egl_image_);

    // We can only change the active texture and unit 0,
    // as that is all that will be restored.
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, egl_image_);
    bind_callback_.Run();

    DCHECK(CHECK_GL());
  }

  void CreateEglImage(GLuint texture_id) {
    TRACE_EVENT0("gpu", "eglCreateImageKHR");
    DCHECK(texture_id);
    DCHECK_EQ(egl_image_, EGL_NO_IMAGE_KHR);

    EGLDisplay egl_display = eglGetCurrentDisplay();
    EGLContext egl_context = eglGetCurrentContext();
    EGLenum egl_target = EGL_GL_TEXTURE_2D_KHR;
    EGLClientBuffer egl_buffer =
        reinterpret_cast<EGLClientBuffer>(texture_id);

    EGLint image_preserved = use_image_preserved_ ? EGL_TRUE : EGL_FALSE;
    EGLint egl_attrib_list[] = {
        EGL_GL_TEXTURE_LEVEL_KHR, 0, // mip-level.
        EGL_IMAGE_PRESERVED_KHR, image_preserved,
        EGL_NONE
    };
    egl_image_ = eglCreateImageKHR(
        egl_display,
        egl_context,
        egl_target,
        egl_buffer,
        egl_attrib_list);

    DCHECK_NE(EGL_NO_IMAGE_KHR, egl_image_);
  }

  void CreateEglImageOnUploadThread() {
    CreateEglImage(thread_texture_id_);
  }

  void CreateEglImageOnMainThreadIfNeeded() {
    if (egl_image_ == EGL_NO_IMAGE_KHR) {
      CreateEglImage(texture_id_);
      if (wait_for_creation_) {
        TRACE_EVENT0("gpu", "glFinish creation");
        glFinish();
      }
    }
  }

  void WaitForLastUpload() {
    // This glFinish is just a safe-guard for if uploads have some
    // GPU action that needs to occur. We could use fences and try
    // to do this less often. However, on older drivers fences are
    // not always reliable (eg. Mali-400 just blocks forever).
    if (wait_for_uploads_) {
      TRACE_EVENT0("gpu", "glFinish");
      glFinish();
    }
  }

  void MarkAsTransferIsInProgress() {
    transfer_completion_.Reset();
  }

  void MarkAsCompleted() {
    transfer_completion_.Signal();
  }

  void WaitForTransferCompletion() {
    transfer_completion_.Wait();
  }

 protected:
  friend class base::RefCountedThreadSafe<TransferStateInternal>;
  friend class AsyncPixelTransferDelegateEGL;

  static void DeleteTexture(GLuint id) {
    glDeleteTextures(1, &id);
  }

  virtual ~TransferStateInternal() {
    if (egl_image_ != EGL_NO_IMAGE_KHR) {
      EGLDisplay display = eglGetCurrentDisplay();
      eglDestroyImageKHR(display, egl_image_);
    }
    if (thread_texture_id_) {
      transfer_message_loop_proxy()->PostTask(FROM_HERE,
          base::Bind(&DeleteTexture, thread_texture_id_));
    }
  }

  // The 'real' texture.
  GLuint texture_id_;

  // The EGLImage sibling on the upload thread.
  GLuint thread_texture_id_;

  // Definition params for texture that needs binding.
  AsyncTexImage2DParams define_params_;

  // Indicates that an async transfer is in progress.
  base::WaitableEvent transfer_completion_;

  // It would be nice if we could just create a new EGLImage for
  // every upload, but I found that didn't work, so this stores
  // one for the lifetime of the texture.
  EGLImageKHR egl_image_;

  // Callback to invoke when AsyncTexImage2D is complete
  // and the client can safely use the texture. This occurs
  // during BindCompletedAsyncTransfers().
  base::Closure bind_callback_;

  // Customize when we block on fences (these are work-arounds).
  bool wait_for_uploads_;
  bool wait_for_creation_;
  bool use_image_preserved_;
};

class TextureUploadStats
    : public base::RefCountedThreadSafe<TextureUploadStats> {
 public:
  TextureUploadStats() : texture_upload_count_(0) {}

  void AddUpload(base::TimeDelta transfer_time) {
    base::AutoLock scoped_lock(lock_);
    texture_upload_count_++;
    total_texture_upload_time_ += transfer_time;
  }

  int GetStats(base::TimeDelta* total_texture_upload_time) {
    base::AutoLock scoped_lock(lock_);
    if (total_texture_upload_time)
      *total_texture_upload_time = total_texture_upload_time_;
    return texture_upload_count_;
  }

 private:
  friend class base::RefCountedThreadSafe<TextureUploadStats>;

  ~TextureUploadStats() {}

  int texture_upload_count_;
  base::TimeDelta total_texture_upload_time_;
  base::Lock lock_;

  DISALLOW_COPY_AND_ASSIGN(TextureUploadStats);
};

namespace {

// EGL needs thread-safe ref-counting, so this just wraps
// an internal thread-safe ref-counted state object.
class AsyncTransferStateImpl : public AsyncPixelTransferState {
 public:
  AsyncTransferStateImpl(GLuint texture_id,
                         const AsyncTexImage2DParams& define_params,
                         bool wait_for_uploads,
                         bool wait_for_creation,
                         bool use_image_preserved)
      : internal_(new TransferStateInternal(texture_id,
                                            define_params,
                                            wait_for_uploads,
                                            wait_for_creation,
                                            use_image_preserved)) {
  }
  virtual ~AsyncTransferStateImpl() {}

  virtual bool TransferIsInProgress() OVERRIDE {
      return internal_->TransferIsInProgress();
  }

  void BindTransfer() {
    internal_->BindTransfer();
  }

  scoped_refptr<TransferStateInternal> internal_;
};

}  // namespace

AsyncPixelTransferDelegateEGL::AsyncPixelTransferDelegateEGL() {
  std::string vendor;
  vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
  is_imagination_ = vendor.find("Imagination") != std::string::npos;
  is_qualcomm_ = vendor.find("Qualcomm") != std::string::npos;
  // TODO(reveman): Skip this if --enable-gpu-benchmarking is not present.
  texture_upload_stats_ = make_scoped_refptr(new TextureUploadStats);
}

AsyncPixelTransferDelegateEGL::~AsyncPixelTransferDelegateEGL() {}

AsyncPixelTransferState* AsyncPixelTransferDelegateEGL::
    CreatePixelTransferState(GLuint texture_id,
                             const AsyncTexImage2DParams& define_params) {
  // We can't wait on uploads on imagination (it can take 200ms+).
  // In practice, they are complete when the CPU glTexSubImage2D completes.
  bool wait_for_uploads = !is_imagination_;

  // Qualcomm runs into texture corruption problems if the same texture is
  // uploaded to with both async and normal uploads. Synchronize after EGLImage
  // creation on the main thread as a work-around.
  bool wait_for_creation = is_qualcomm_;

  // Qualcomm has a race when using image_preserved=FALSE,
  // which can result in black textures even after the first upload.
  // Since using FALSE is mainly for performance (to avoid layout changes),
  // but Qualcomm itself doesn't seem to get any performance benefit,
  // we just using image_preservedd=TRUE on Qualcomm as a work-around.
  bool use_image_preserved = is_qualcomm_ || is_imagination_;

  return new AsyncTransferStateImpl(texture_id,
                                    define_params,
                                    wait_for_uploads,
                                    wait_for_creation,
                                    use_image_preserved);
}

bool AsyncPixelTransferDelegateEGL::BindCompletedAsyncTransfers() {
  bool texture_dirty = false;
  while(!pending_allocations_.empty()) {
    if (!pending_allocations_.front().get()) {
      pending_allocations_.pop_front();
      continue;
    }
    scoped_refptr<TransferStateInternal> state =
        static_cast<AsyncTransferStateImpl*>
            (pending_allocations_.front().get())->internal_.get();
    // Terminate early, as all transfers finish in order, currently.
    if (state->TransferIsInProgress())
      break;
    // If the transfer is finished, bind it to the texture
    // and remove it from pending list.
    texture_dirty = true;
    state->BindTransfer();
    pending_allocations_.pop_front();
  }
  return texture_dirty;
}

void AsyncPixelTransferDelegateEGL::AsyncNotifyCompletion(
    const AsyncMemoryParams& mem_params,
    const CompletionCallback& callback) {
  DCHECK(mem_params.shared_memory);
  DCHECK_LE(mem_params.shm_data_offset + mem_params.shm_data_size,
            mem_params.shm_size);
  // Post a PerformNotifyCompletion task to the upload thread. This task
  // will run after all async transfers are complete.
  transfer_message_loop_proxy()->PostTask(
      FROM_HERE,
      base::Bind(&AsyncPixelTransferDelegateEGL::PerformNotifyCompletion,
                 mem_params,
                 base::Owned(
                     new ScopedSafeSharedMemory(safe_shared_memory_pool(),
                                                mem_params.shared_memory,
                                                mem_params.shm_size)),
                 callback));
}

void AsyncPixelTransferDelegateEGL::WaitForTransferCompletion(
      AsyncPixelTransferState* transfer_state) {
  TRACE_EVENT0("gpu", "WaitForTransferCompletion");
  scoped_refptr<TransferStateInternal> state =
      static_cast<AsyncTransferStateImpl*>(transfer_state)->internal_.get();
  DCHECK(state);
  DCHECK(state->texture_id_);

  if (state->TransferIsInProgress()) {
#if defined(OS_ANDROID)
    // TODO(epenner): Move thread priorities to base. (crbug.com/170549)
    int high_nice_value = -6;  // Default priority.
    int idle_nice_value    = 10; // Idle priority.
    setpriority(PRIO_PROCESS,
                g_transfer_thread.Pointer()->thread_id(),
                high_nice_value);
#endif

    state->WaitForTransferCompletion();
    DCHECK(!state->TransferIsInProgress());

#if defined(OS_ANDROID)
    // TODO(epenner): Move thread priorities to base. (crbug.com/170549)
    setpriority(PRIO_PROCESS,
                g_transfer_thread.Pointer()->thread_id(),
                idle_nice_value);
#endif
  }
}

void AsyncPixelTransferDelegateEGL::AsyncTexImage2D(
    AsyncPixelTransferState* transfer_state,
    const AsyncTexImage2DParams& tex_params,
    const AsyncMemoryParams& mem_params,
    const base::Closure& bind_callback) {
  if (WorkAroundAsyncTexImage2D(transfer_state, tex_params,
                                mem_params, bind_callback))
    return;

  scoped_refptr<TransferStateInternal> state =
      static_cast<AsyncTransferStateImpl*>(transfer_state)->internal_.get();
  DCHECK(mem_params.shared_memory);
  DCHECK_LE(mem_params.shm_data_offset + mem_params.shm_data_size,
            mem_params.shm_size);
  DCHECK(state);
  DCHECK(state->texture_id_);
  DCHECK(!state->TransferIsInProgress());
  DCHECK_EQ(state->egl_image_, EGL_NO_IMAGE_KHR);
  DCHECK_EQ(static_cast<GLenum>(GL_TEXTURE_2D), tex_params.target);
  DCHECK_EQ(tex_params.level, 0);

  // Mark the transfer in progress and save the late bind
  // callback, so we can notify the client when it is bound.
  pending_allocations_.push_back(transfer_state->AsWeakPtr());
  state->bind_callback_ = bind_callback;

  // Mark the transfer in progress.
  state->MarkAsTransferIsInProgress();

  // Duplicate the shared memory so there is no way we can get
  // a use-after-free of the raw pixels.
  transfer_message_loop_proxy()->PostTask(FROM_HERE,
      base::Bind(
          &AsyncPixelTransferDelegateEGL::PerformAsyncTexImage2D,
          state,
          tex_params,
          mem_params,
          base::Owned(new ScopedSafeSharedMemory(safe_shared_memory_pool(),
                                                 mem_params.shared_memory,
                                                 mem_params.shm_size))));


  DCHECK(CHECK_GL());
}

void AsyncPixelTransferDelegateEGL::AsyncTexSubImage2D(
    AsyncPixelTransferState* transfer_state,
    const AsyncTexSubImage2DParams& tex_params,
    const AsyncMemoryParams& mem_params) {
  TRACE_EVENT2("gpu", "AsyncTexSubImage2D",
               "width", tex_params.width,
               "height", tex_params.height);
  if (WorkAroundAsyncTexSubImage2D(transfer_state, tex_params, mem_params))
    return;
  scoped_refptr<TransferStateInternal> state =
      static_cast<AsyncTransferStateImpl*>(transfer_state)->internal_.get();

  DCHECK(state->texture_id_);
  DCHECK(!state->TransferIsInProgress());
  DCHECK(mem_params.shared_memory);
  DCHECK_LE(mem_params.shm_data_offset + mem_params.shm_data_size,
            mem_params.shm_size);
  DCHECK_EQ(static_cast<GLenum>(GL_TEXTURE_2D), tex_params.target);
  DCHECK_EQ(tex_params.level, 0);

  // Mark the transfer in progress.
  state->MarkAsTransferIsInProgress();

  // If this wasn't async allocated, we don't have an EGLImage yet.
  // Create the EGLImage if it hasn't already been created.
  state->CreateEglImageOnMainThreadIfNeeded();

  // Duplicate the shared memory so there are no way we can get
  // a use-after-free of the raw pixels.
  transfer_message_loop_proxy()->PostTask(FROM_HERE,
      base::Bind(
          &AsyncPixelTransferDelegateEGL::PerformAsyncTexSubImage2D,
          state,
          tex_params,
          mem_params,
          base::Owned(new ScopedSafeSharedMemory(safe_shared_memory_pool(),
                                                 mem_params.shared_memory,
                                                 mem_params.shm_size)),
          texture_upload_stats_));

  DCHECK(CHECK_GL());
}

uint32 AsyncPixelTransferDelegateEGL::GetTextureUploadCount() {
  CHECK(texture_upload_stats_);
  return texture_upload_stats_->GetStats(NULL);
}

base::TimeDelta AsyncPixelTransferDelegateEGL::GetTotalTextureUploadTime() {
  CHECK(texture_upload_stats_);
  base::TimeDelta total_texture_upload_time;
  texture_upload_stats_->GetStats(&total_texture_upload_time);
  return total_texture_upload_time;
}

bool AsyncPixelTransferDelegateEGL::ProcessMorePendingTransfers() {
  return false;
}

bool AsyncPixelTransferDelegateEGL::NeedsProcessMorePendingTransfers() {
  return false;
}

namespace {
void SetGlParametersForEglImageTexture() {
  // These params are needed for EGLImage creation to succeed on several
  // Android devices. I couldn't find this requirement in the EGLImage
  // extension spec, but several devices fail without it.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
} // namespace

void AsyncPixelTransferDelegateEGL::PerformNotifyCompletion(
    AsyncMemoryParams mem_params,
    ScopedSafeSharedMemory* safe_shared_memory,
    const CompletionCallback& callback) {
  TRACE_EVENT0("gpu", "PerformNotifyCompletion");
  gfx::AsyncMemoryParams safe_mem_params = mem_params;
  safe_mem_params.shared_memory = safe_shared_memory->shared_memory();
  callback.Run(safe_mem_params);
}

void AsyncPixelTransferDelegateEGL::PerformAsyncTexImage2D(
    TransferStateInternal* state,
    AsyncTexImage2DParams tex_params,
    AsyncMemoryParams mem_params,
    ScopedSafeSharedMemory* safe_shared_memory) {
  TRACE_EVENT2("gpu", "PerformAsyncTexImage",
               "width", tex_params.width,
               "height", tex_params.height);
  DCHECK(state);
  DCHECK(!state->thread_texture_id_);
  DCHECK_EQ(0, tex_params.level);
  DCHECK_EQ(EGL_NO_IMAGE_KHR, state->egl_image_);

  void* data = GetAddress(safe_shared_memory->shared_memory(),
                          mem_params.shm_data_offset);
  {
    TRACE_EVENT0("gpu", "glTexImage2D no data");
    glGenTextures(1, &state->thread_texture_id_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state->thread_texture_id_);

    SetGlParametersForEglImageTexture();

    // If we need to use image_preserved, we pass the data with
    // the allocation. Otherwise we use a NULL allocation to
    // try to avoid any costs associated with creating the EGLImage.
    if (state->use_image_preserved_)
       DoTexImage2D(tex_params, data);
    else
       DoTexImage2D(tex_params, NULL);
  }

  state->CreateEglImageOnUploadThread();

  {
    TRACE_EVENT0("gpu", "glTexSubImage2D with data");

    // If we didn't use image_preserved, we haven't uploaded
    // the data yet, so we do this with a full texSubImage.
    if (!state->use_image_preserved_)
      DoFullTexSubImage2D(tex_params, data);
  }

  state->WaitForLastUpload();
  state->MarkAsCompleted();

  DCHECK(CHECK_GL());
}

void AsyncPixelTransferDelegateEGL::PerformAsyncTexSubImage2D(
    TransferStateInternal* state,
    AsyncTexSubImage2DParams tex_params,
    AsyncMemoryParams mem_params,
    ScopedSafeSharedMemory* safe_shared_memory,
    scoped_refptr<TextureUploadStats> texture_upload_stats) {
  TRACE_EVENT2("gpu", "PerformAsyncTexSubImage2D",
               "width", tex_params.width,
               "height", tex_params.height);

  DCHECK(state);
  DCHECK_NE(EGL_NO_IMAGE_KHR, state->egl_image_);
  DCHECK_EQ(0, tex_params.level);

  void* data = GetAddress(safe_shared_memory->shared_memory(),
                          mem_params.shm_data_offset);

  base::TimeTicks begin_time;
  if (texture_upload_stats)
    begin_time = base::TimeTicks::HighResNow();

  if (!state->thread_texture_id_) {
    TRACE_EVENT0("gpu", "glEGLImageTargetTexture2DOES");
    glGenTextures(1, &state->thread_texture_id_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state->thread_texture_id_);
    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, state->egl_image_);
  } else {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state->thread_texture_id_);
  }
  {
    TRACE_EVENT0("gpu", "glTexSubImage2D");
    DoTexSubImage2D(tex_params, data);
  }
  state->WaitForLastUpload();
  state->MarkAsCompleted();

  DCHECK(CHECK_GL());
  if (texture_upload_stats) {
    texture_upload_stats->AddUpload(
        base::TimeTicks::HighResNow() - begin_time);
  }
}

namespace {
bool IsPowerOfTwo (unsigned int x) {
  return ((x != 0) && !(x & (x - 1)));
}

bool IsMultipleOfEight(unsigned int x) {
  return (x & 7) == 0;
}

bool DimensionsSupportImgFastPath(int width, int height) {
  // Multiple of eight, but not a power of two.
  return IsMultipleOfEight(width) &&
         IsMultipleOfEight(height) &&
         !(IsPowerOfTwo(width) &&
           IsPowerOfTwo(height));
}
} // namespace

// It is very difficult to stream uploads on Imagination GPUs:
// - glTexImage2D defers a swizzle/stall until draw-time
// - glTexSubImage2D will sleep for 16ms on a good day, and 100ms
//   or longer if OpenGL is in heavy use by another thread.
// The one combination that avoids these problems requires:
// a.) Allocations/Uploads must occur on different threads/contexts.
// b.) Texture size must be non-power-of-two.
// When using a+b, uploads will be incorrect/corrupt unless:
// c.) Texture size must be a multiple-of-eight.
//
// To achieve a.) we allocate synchronously on the main thread followed
// by uploading on the upload thread. When b/c are not true we fall back
// on purely synchronous allocation/upload on the main thread.

bool AsyncPixelTransferDelegateEGL::WorkAroundAsyncTexImage2D(
    AsyncPixelTransferState* transfer_state,
    const AsyncTexImage2DParams& tex_params,
    const AsyncMemoryParams& mem_params,
    const base::Closure& bind_callback) {
  if (!is_imagination_)
    return false;
  scoped_refptr<TransferStateInternal> state =
      static_cast<AsyncTransferStateImpl*>(transfer_state)->internal_.get();

  // On imagination we allocate synchronously all the time, even
  // if the dimensions support fast uploads. This is for part a.)
  // above, so allocations occur on a different thread/context as uploads.
  void* data = GetAddress(mem_params.shared_memory,
                          mem_params.shm_data_offset);
  SetGlParametersForEglImageTexture();

  {
    TRACE_EVENT0("gpu", "glTexImage2D with data");
    DoTexImage2D(tex_params, data);
  }

  // The allocation has already occured, so mark it as finished
  // and ready for binding.
  CHECK(!state->TransferIsInProgress());

  // If the dimensions support fast async uploads, create the
  // EGLImage for future uploads. The late bind should not
  // be needed since the EGLImage was created from the main thread
  // texture, but this is required to prevent an imagination driver crash.
  if (DimensionsSupportImgFastPath(tex_params.width, tex_params.height)) {
    state->CreateEglImageOnMainThreadIfNeeded();
    pending_allocations_.push_back(transfer_state->AsWeakPtr());
    state->bind_callback_ = bind_callback;
  }

  DCHECK(CHECK_GL());
  return true;
}

bool AsyncPixelTransferDelegateEGL::WorkAroundAsyncTexSubImage2D(
    AsyncPixelTransferState* transfer_state,
    const AsyncTexSubImage2DParams& tex_params,
    const AsyncMemoryParams& mem_params) {
  if (!is_imagination_)
    return false;

  // If the dimensions support fast async uploads, we can use the
  // normal async upload path for uploads.
  if (DimensionsSupportImgFastPath(tex_params.width, tex_params.height))
    return false;

  scoped_refptr<TransferStateInternal> state =
      static_cast<AsyncTransferStateImpl*>(transfer_state)->internal_.get();

  // Fall back on a synchronous stub as we don't have a known fast path.
  // Also, older ICS drivers crash when we do any glTexSubImage2D on the
  // same thread. To work around this we do glTexImage2D instead. Since
  // we didn't create an EGLImage for this texture (see above), this is
  // okay, but it limits this API to full updates for now.
  DCHECK(!state->egl_image_);
  DCHECK_EQ(tex_params.xoffset, 0);
  DCHECK_EQ(tex_params.yoffset, 0);
  DCHECK_EQ(state->define_params_.width, tex_params.width);
  DCHECK_EQ(state->define_params_.height, tex_params.height);
  DCHECK_EQ(state->define_params_.level, tex_params.level);
  DCHECK_EQ(state->define_params_.format, tex_params.format);
  DCHECK_EQ(state->define_params_.type, tex_params.type);

  void* data = GetAddress(mem_params.shared_memory,
                          mem_params.shm_data_offset);
  base::TimeTicks begin_time;
  if (texture_upload_stats_)
    begin_time = base::TimeTicks::HighResNow();
  {
    TRACE_EVENT0("gpu", "glTexSubImage2D");
    // Note we use define_params_ instead of tex_params.
    // The DCHECKs above verify this is always the same.
    DoTexImage2D(state->define_params_, data);
  }
  if (texture_upload_stats_) {
    texture_upload_stats_->AddUpload(
        base::TimeTicks::HighResNow() - begin_time);
  }

  DCHECK(CHECK_GL());
  return true;
}

}  // namespace gfx
