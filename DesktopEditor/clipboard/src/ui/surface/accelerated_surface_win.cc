// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/surface/accelerated_surface_win.h"

#include <windows.h>
#include <algorithm>

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/command_line.h"
#include "base/debug/trace_event.h"
#include "base/files/file_path.h"
#include "base/lazy_instance.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/scoped_native_library.h"
#include "base/stringprintf.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/thread.h"
#include "base/threading/thread_restrictions.h"
#include "base/win/wrapped_window_proc.h"
#include "media/base/video_frame.h"
#include "media/base/video_util.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/win/dpi.h"
#include "ui/base/win/hwnd_util.h"
#include "ui/base/win/shell.h"
#include "ui/gfx/rect.h"
#include "ui/gl/gl_switches.h"
#include "ui/surface/accelerated_surface_transformer_win.h"
#include "ui/surface/d3d9_utils_win.h"
#include "ui/surface/surface_switches.h"

namespace d3d_utils = ui_surface_d3d9_utils;

namespace {

UINT GetPresentationInterval() {
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kDisableGpuVsync))
    return D3DPRESENT_INTERVAL_IMMEDIATE;
  else
    return D3DPRESENT_INTERVAL_ONE;
}

bool DoFirstShowPresentWithGDI() {
  return CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDoFirstShowPresentWithGDI);
}

bool DoAllShowPresentWithGDI() {
  return CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDoAllShowPresentWithGDI);
}

// Use a SurfaceReader to copy into one plane of the VideoFrame.
bool CopyPlane(AcceleratedSurfaceTransformer* gpu_ops,
               IDirect3DSurface9* src_surface,
               media::VideoFrame* dst_frame,
               size_t plane_id) {
  int width_in_bytes = dst_frame->row_bytes(plane_id);
  return gpu_ops->ReadFast(src_surface, dst_frame->data(plane_id),
                           width_in_bytes, dst_frame->rows(plane_id),
                           dst_frame->row_bytes(plane_id));
}

}  // namespace

// A PresentThread is a thread that is dedicated to presenting surfaces to a
// window. It owns a Direct3D device and a Direct3D query for this purpose.
class PresentThread : public base::Thread,
                      public base::RefCountedThreadSafe<PresentThread> {
 public:
  explicit PresentThread(const char* name);

  IDirect3DDevice9Ex* device() { return device_.get(); }
  IDirect3DQuery9* query() { return query_.get(); }
  AcceleratedSurfaceTransformer* surface_transformer() {
    return &surface_transformer_;
  }

  void InitDevice();
  void LockAndResetDevice();
  void ResetDevice();
  bool IsDeviceLost();

  base::Lock* lock() {
    return &lock_;
  }

 protected:
  virtual void Init();
  virtual void CleanUp();

 private:
  friend class base::RefCountedThreadSafe<PresentThread>;

  ~PresentThread();

  // The lock is taken while any thread is calling an AcceleratedPresenter
  // associated with this thread.
  base::Lock lock_;

  base::ScopedNativeLibrary d3d_module_;
  base::win::ScopedComPtr<IDirect3DDevice9Ex> device_;
  // This query is used to wait until a certain amount of progress has been
  // made by the GPU and it is safe for the producer to modify its shared
  // texture again.
  base::win::ScopedComPtr<IDirect3DQuery9> query_;
  AcceleratedSurfaceTransformer surface_transformer_;

  DISALLOW_COPY_AND_ASSIGN(PresentThread);
};

// There is a fixed sized pool of PresentThreads and therefore the maximum
// number of Direct3D devices owned by those threads is bounded.
class PresentThreadPool {
 public:
  static const int kNumPresentThreads = 4;

  PresentThreadPool();
  PresentThread* NextThread();

 private:
  int next_thread_;
  scoped_refptr<PresentThread> present_threads_[kNumPresentThreads];

  DISALLOW_COPY_AND_ASSIGN(PresentThreadPool);
};

// A thread safe map of presenters by surface ID that returns presenters via
// a scoped_refptr to keep them alive while they are referenced.
class AcceleratedPresenterMap {
 public:
  AcceleratedPresenterMap();
  scoped_refptr<AcceleratedPresenter> CreatePresenter(
      gfx::PluginWindowHandle window);
  void RemovePresenter(const scoped_refptr<AcceleratedPresenter>& presenter);
  scoped_refptr<AcceleratedPresenter> GetPresenter(
      gfx::PluginWindowHandle window);


  // Destroy any D3D resources owned by the given present thread. Called on
  // the given present thread.
  void ResetPresentThread(PresentThread* present_thread);

 private:
  base::Lock lock_;
  typedef std::map<gfx::PluginWindowHandle, AcceleratedPresenter*> PresenterMap;
  PresenterMap presenters_;
  DISALLOW_COPY_AND_ASSIGN(AcceleratedPresenterMap);
};

base::LazyInstance<PresentThreadPool>
    g_present_thread_pool = LAZY_INSTANCE_INITIALIZER;

base::LazyInstance<AcceleratedPresenterMap>
    g_accelerated_presenter_map = LAZY_INSTANCE_INITIALIZER;

PresentThread::PresentThread(const char* name) : base::Thread(name) {
}

void PresentThread::InitDevice() {
  lock_.AssertAcquired();

  if (device_)
    return;

  TRACE_EVENT0("gpu", "PresentThread::Init");
  d3d_utils::LoadD3D9(&d3d_module_);
  ResetDevice();
}

void PresentThread::LockAndResetDevice() {
  base::AutoLock locked(lock_);
  ResetDevice();
}

void PresentThread::ResetDevice() {
  TRACE_EVENT0("gpu", "PresentThread::ResetDevice");

  lock_.AssertAcquired();

  // The D3D device must be created on the present thread.
  CHECK(message_loop() == base::MessageLoop::current());

  // This will crash some Intel drivers but we can't render anything without
  // reseting the device, which would be disappointing.
  query_ = NULL;
  device_ = NULL;
  surface_transformer_.ReleaseAll();

  g_accelerated_presenter_map.Pointer()->ResetPresentThread(this);

  if (!d3d_utils::CreateDevice(d3d_module_,
                               D3DDEVTYPE_HAL,
                               GetPresentationInterval(),
                               device_.Receive())) {
    return;
  }

  HRESULT hr = device_->CreateQuery(D3DQUERYTYPE_EVENT, query_.Receive());
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to create query";
    device_ = NULL;
    return;
  }

  if (!surface_transformer_.Init(device_)) {
    LOG(ERROR) << "Failed to initialize surface transformer";
    query_ = NULL;
    device_ = NULL;
    return;
  }
}

bool PresentThread::IsDeviceLost() {
  lock_.AssertAcquired();

  HRESULT hr = device_->CheckDeviceState(NULL);
  return FAILED(hr) || hr == S_PRESENT_MODE_CHANGED;
}

void PresentThread::Init() {
  TRACE_EVENT0("gpu", "Initialize thread");
}

void PresentThread::CleanUp() {
  // The D3D device and query are leaked because destroying the associated D3D
  // query crashes some Intel drivers.
  surface_transformer_.DetachAll();
  device_.Detach();
  query_.Detach();
}

PresentThread::~PresentThread() {
  Stop();
}

PresentThreadPool::PresentThreadPool() : next_thread_(0) {
}

PresentThread* PresentThreadPool::NextThread() {
  next_thread_ = (next_thread_ + 1) % kNumPresentThreads;
  PresentThread* thread = present_threads_[next_thread_].get();
  if (!thread) {
    thread = new PresentThread(
        base::StringPrintf("PresentThread #%d", next_thread_).c_str());
    thread->Start();
    present_threads_[next_thread_] = thread;
  }

  return thread;
}

AcceleratedPresenterMap::AcceleratedPresenterMap() {
}

scoped_refptr<AcceleratedPresenter> AcceleratedPresenterMap::CreatePresenter(
    gfx::PluginWindowHandle window) {
  scoped_refptr<AcceleratedPresenter> presenter(
      new AcceleratedPresenter(window));

  base::AutoLock locked(lock_);
  DCHECK(presenters_.find(window) == presenters_.end());
  presenters_[window] = presenter.get();

  return presenter;
}

void AcceleratedPresenterMap::RemovePresenter(
    const scoped_refptr<AcceleratedPresenter>& presenter) {
  base::AutoLock locked(lock_);
  for (PresenterMap::iterator it = presenters_.begin();
      it != presenters_.end();
      ++it) {
    if (it->second == presenter.get()) {
      presenters_.erase(it);
      return;
    }
  }

  NOTREACHED();
}

scoped_refptr<AcceleratedPresenter> AcceleratedPresenterMap::GetPresenter(
    gfx::PluginWindowHandle window) {
  base::AutoLock locked(lock_);

#if defined(USE_AURA)
  if (!window)
    return presenters_.begin()->second;
#endif

  PresenterMap::iterator it = presenters_.find(window);
  if (it == presenters_.end())
    return scoped_refptr<AcceleratedPresenter>();

  return it->second;
}

void AcceleratedPresenterMap::ResetPresentThread(
    PresentThread* present_thread) {
  base::AutoLock locked(lock_);

  for (PresenterMap::iterator it = presenters_.begin();
      it != presenters_.end();
      ++it) {
    it->second->ResetPresentThread(present_thread);
  }
}

AcceleratedPresenter::AcceleratedPresenter(gfx::PluginWindowHandle window)
    : present_thread_(g_present_thread_pool.Pointer()->NextThread()),
      window_(window),
      event_(false, false),
      hidden_(true),
      do_present_with_GDI_(DoAllShowPresentWithGDI() ||
                           DoFirstShowPresentWithGDI()),
      is_session_locked_(false) {
}

// static
scoped_refptr<AcceleratedPresenter> AcceleratedPresenter::GetForWindow(
    gfx::PluginWindowHandle window) {
  return g_accelerated_presenter_map.Pointer()->GetPresenter(window);
}

void AcceleratedPresenter::AsyncPresentAndAcknowledge(
    const gfx::Size& size,
    int64 surface_handle,
    const CompletionTask& completion_task) {
  if (!surface_handle) {
    TRACE_EVENT1("gpu", "EarlyOut_ZeroSurfaceHandle",
                 "surface_handle", surface_handle);
    completion_task.Run(true, base::TimeTicks(), base::TimeDelta());
    return;
  }

  present_thread_->message_loop()->PostTask(
      FROM_HERE,
      base::Bind(&AcceleratedPresenter::DoPresentAndAcknowledge,
                 this,
                 size,
                 surface_handle,
                 completion_task));
}

void AcceleratedPresenter::Present(HDC dc) {
  TRACE_EVENT0("gpu", "Present");

  base::AutoLock locked(*present_thread_->lock());

  // If invalidated, do nothing. The window is gone.
  if (!window_)
    return;

  // Suspended or nothing has ever been presented.
  if (!swap_chain_)
    return;

  PresentWithGDI(dc);
}

void AcceleratedPresenter::AsyncCopyTo(
    const gfx::Rect& requested_src_subrect,
    const gfx::Size& dst_size,
    const base::Callback<void(bool, const SkBitmap&)>& callback) {
  present_thread_->message_loop()->PostTask(
      FROM_HERE,
      base::Bind(&AcceleratedPresenter::DoCopyToAndAcknowledge,
                 this,
                 requested_src_subrect,
                 dst_size,
                 base::MessageLoopProxy::current(),
                 callback));
}

void AcceleratedPresenter::AsyncCopyToVideoFrame(
    const gfx::Rect& requested_src_subrect,
    const scoped_refptr<media::VideoFrame>& target,
    const base::Callback<void(bool)>& callback) {
  present_thread_->message_loop()->PostTask(
      FROM_HERE,
      base::Bind(&AcceleratedPresenter::DoCopyToVideoFrameAndAcknowledge,
                 this,
                 requested_src_subrect,
                 target,
                 base::MessageLoopProxy::current(),
                 callback));
}

void AcceleratedPresenter::DoCopyToAndAcknowledge(
    const gfx::Rect& src_subrect,
    const gfx::Size& dst_size,
    scoped_refptr<base::SingleThreadTaskRunner> callback_runner,
    const base::Callback<void(bool, const SkBitmap&)>& callback) {
  SkBitmap target;
  bool result = DoCopyToARGB(src_subrect, dst_size, &target);
  if (!result)
    target.reset();
  callback_runner->PostTask(FROM_HERE, base::Bind(callback, result, target));
}

void AcceleratedPresenter::DoCopyToVideoFrameAndAcknowledge(
    const gfx::Rect& src_subrect,
    const scoped_refptr<media::VideoFrame>& target,
    const scoped_refptr<base::SingleThreadTaskRunner>& callback_runner,
    const base::Callback<void(bool)>& callback) {

  bool result = DoCopyToYUV(src_subrect, target);
  callback_runner->PostTask(FROM_HERE, base::Bind(callback, result));
}

bool AcceleratedPresenter::DoCopyToARGB(const gfx::Rect& requested_src_subrect,
                                        const gfx::Size& dst_size,
                                        SkBitmap* bitmap) {
  TRACE_EVENT2(
      "gpu", "CopyTo",
      "width", dst_size.width(),
      "height", dst_size.height());

  base::AutoLock locked(*present_thread_->lock());

  if (!swap_chain_)
    return false;

  AcceleratedSurfaceTransformer* gpu_ops =
      present_thread_->surface_transformer();

  base::win::ScopedComPtr<IDirect3DSurface9> back_buffer;
  HRESULT hr = swap_chain_->GetBackBuffer(0,
                                          D3DBACKBUFFER_TYPE_MONO,
                                          back_buffer.Receive());
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to get back buffer";
    return false;
  }

  D3DSURFACE_DESC desc;
  hr = back_buffer->GetDesc(&desc);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to get buffer description";
    return false;
  }

  const gfx::Size back_buffer_size(desc.Width, desc.Height);
  if (back_buffer_size.IsEmpty())
    return false;

  // With window resizing, it's possible that the back buffer is smaller than
  // the requested src subset. Clip to the actual back buffer.
  gfx::Rect src_subrect = requested_src_subrect;
  src_subrect.Intersect(gfx::Rect(back_buffer_size));
  base::win::ScopedComPtr<IDirect3DSurface9> final_surface;
  {
    if (!d3d_utils::CreateOrReuseLockableSurface(present_thread_->device(),
                                                 dst_size,
                                                 &final_surface)) {
      LOG(ERROR) << "Failed to create temporary lockable surface";
      return false;
    }
  }

  {
    // Let the surface transformer start the resize into |final_surface|.
    TRACE_EVENT0("gpu", "ResizeBilinear");
    if (!gpu_ops->ResizeBilinear(back_buffer, src_subrect,
                                 final_surface, gfx::Rect(dst_size))) {
      LOG(ERROR) << "Failed to resize bilinear";
      return false;
    }
  }

  bitmap->setConfig(SkBitmap::kARGB_8888_Config,
                    dst_size.width(), dst_size.height());
  if (!bitmap->allocPixels())
    return false;
  bitmap->setIsOpaque(true);

  // Copy |final_surface| to |bitmap|. This is always a synchronous operation.
  return gpu_ops->ReadFast(final_surface,
                           reinterpret_cast<uint8*>(bitmap->getPixels()),
                           bitmap->width() * bitmap->bytesPerPixel(),
                           bitmap->height(),
                           static_cast<int>(bitmap->rowBytes()));
}

bool AcceleratedPresenter::DoCopyToYUV(
    const gfx::Rect& requested_src_subrect,
    const scoped_refptr<media::VideoFrame>& frame) {
  gfx::Size dst_size = frame->coded_size();
  TRACE_EVENT2(
      "gpu", "CopyToYUV",
      "width", dst_size.width(),
      "height", dst_size.height());

  base::AutoLock locked(*present_thread_->lock());

  if (!swap_chain_)
    return false;

  AcceleratedSurfaceTransformer* gpu_ops =
      present_thread_->surface_transformer();

  base::win::ScopedComPtr<IDirect3DSurface9> back_buffer;
  HRESULT hr = swap_chain_->GetBackBuffer(0,
                                          D3DBACKBUFFER_TYPE_MONO,
                                          back_buffer.Receive());
  if (FAILED(hr))
    return false;

  D3DSURFACE_DESC desc;
  hr = back_buffer->GetDesc(&desc);
  if (FAILED(hr))
    return false;

  const gfx::Size back_buffer_size(desc.Width, desc.Height);
  if (back_buffer_size.IsEmpty())
    return false;

  // With window resizing, it's possible that the back buffer is smaller than
  // the requested src subset. Clip to the actual back buffer.
  gfx::Rect src_subrect = requested_src_subrect;
  src_subrect.Intersect(gfx::Rect(back_buffer_size));

  base::win::ScopedComPtr<IDirect3DSurface9> resized;
  base::win::ScopedComPtr<IDirect3DTexture9> resized_as_texture;
  if (!gpu_ops->GetIntermediateTexture(dst_size,
                                       resized_as_texture.Receive(),
                                       resized.Receive())) {
    return false;
  }

  // Shrink the source to fit entirely in the destination while preserving
  // aspect ratio. Fill in any margin with black.
  // TODO(nick): It would be more efficient all around to implement
  // letterboxing as a memset() on the dst.
  gfx::Rect letterbox = media::ComputeLetterboxRegion(gfx::Rect(dst_size),
                                                      src_subrect.size());
  if (letterbox != gfx::Rect(dst_size)) {
    TRACE_EVENT0("gpu", "Letterbox");
    present_thread_->device()->ColorFill(resized, NULL, 0xFF000000);
  }

  {
    TRACE_EVENT0("gpu", "ResizeBilinear");
    if (!gpu_ops->ResizeBilinear(back_buffer, src_subrect, resized, letterbox))
      return false;
  }

  base::win::ScopedComPtr<IDirect3DSurface9> y, u, v;
  {
    TRACE_EVENT0("gpu", "TransformRGBToYV12");
    if (!gpu_ops->TransformRGBToYV12(resized_as_texture,
                                     dst_size,
                                     y.Receive(), u.Receive(), v.Receive())) {
      return false;
    }
  }

  if (!CopyPlane(gpu_ops, y, frame, media::VideoFrame::kYPlane))
    return false;
  if (!CopyPlane(gpu_ops, u, frame, media::VideoFrame::kUPlane))
    return false;
  if (!CopyPlane(gpu_ops, v, frame, media::VideoFrame::kVPlane))
    return false;
  return true;
}

void AcceleratedPresenter::Suspend() {
  present_thread_->message_loop()->PostTask(
      FROM_HERE,
      base::Bind(&AcceleratedPresenter::DoSuspend,
                 this));
}

void AcceleratedPresenter::WasHidden() {
  base::AutoLock locked(*present_thread_->lock());
  hidden_ = true;
}

void AcceleratedPresenter::ReleaseSurface() {
  present_thread_->message_loop()->PostTask(
      FROM_HERE,
      base::Bind(&AcceleratedPresenter::DoReleaseSurface,
                 this));
}

void AcceleratedPresenter::SetIsSessionLocked(bool locked) {
  is_session_locked_ = locked;
}

void AcceleratedPresenter::Invalidate() {
  // Make any pending or future presentation tasks do nothing. Once the last
  // last pending task has been ignored, the reference count on the presenter
  // will go to zero and the presenter, and potentially also the present thread
  // it has a reference count on, will be destroyed.
  base::AutoLock locked(*present_thread_->lock());
  window_ = NULL;
}

void AcceleratedPresenter::ResetPresentThread(
    PresentThread* present_thread) {
  TRACE_EVENT0("gpu", "ResetPresentThread");

  // present_thread_ can be accessed without the lock because it is immutable.
  if (present_thread_ != present_thread)
    return;

  present_thread_->lock()->AssertAcquired();

  source_texture_ = NULL;
  swap_chain_ = NULL;
  quantized_size_ = gfx::Size();
}

#if defined(USE_AURA)
void AcceleratedPresenter::SetNewTargetWindow(gfx::PluginWindowHandle window) {
  window_ = window;
}
#endif

AcceleratedPresenter::~AcceleratedPresenter() {
}

void AcceleratedPresenter::DoPresentAndAcknowledge(
    const gfx::Size& size,
    int64 surface_handle,
    const CompletionTask& completion_task) {
  TRACE_EVENT2(
      "gpu", "DoPresentAndAcknowledge",
      "width", size.width(),
      "height", size.height());

  HRESULT hr;

  base::AutoLock locked(*present_thread_->lock());

  // Initialize the device lazily since calling Direct3D can crash bots.
  present_thread_->InitDevice();

  if (!present_thread_->device()) {
    completion_task.Run(false, base::TimeTicks(), base::TimeDelta());
    TRACE_EVENT0("gpu", "EarlyOut_NoDevice");
    return;
  }

  // Ensure the task is acknowledged on early out after this point.
  base::ScopedClosureRunner scoped_completion_runner(
      base::Bind(completion_task, true, base::TimeTicks(), base::TimeDelta()));

  // If invalidated, do nothing, the window is gone.
  if (!window_) {
    TRACE_EVENT0("gpu", "EarlyOut_NoWindow");
    return;
  }

#if !defined(USE_AURA)
  // If the window is a different size than the swap chain that is being
  // presented then drop the frame.
  gfx::Size window_size = GetWindowSize();
#if defined(ENABLE_HIDPI)
  // Check if the size mismatch is within allowable round off or truncation
  // error.
  gfx::Size dip_size = ui::win::ScreenToDIPSize(window_size);
  gfx::Size pixel_size = ui::win::DIPToScreenSize(dip_size);
  bool size_mismatch = abs(window_size.width() - size.width()) >
      abs(window_size.width() - pixel_size.width()) ||
      abs(window_size.height() - size.height()) >
      abs(window_size.height() - pixel_size.height());
#else
  bool size_mismatch = size != window_size;
#endif
  if (hidden_ && size_mismatch) {
    TRACE_EVENT2("gpu", "EarlyOut_WrongWindowSize",
                 "backwidth", size.width(), "backheight", size.height());
    TRACE_EVENT2("gpu", "EarlyOut_WrongWindowSize2",
                 "windowwidth", window_size.width(),
                 "windowheight", window_size.height());
    return;
  }
#endif

  // Round up size so the swap chain is not continuously resized with the
  // surface, which could lead to memory fragmentation.
  const int kRound = 64;
  gfx::Size quantized_size(
      std::max(1, (size.width() + kRound - 1) / kRound * kRound),
      std::max(1, (size.height() + kRound - 1) / kRound * kRound));

  // Ensure the swap chain exists and is the same size (rounded up) as the
  // surface to be presented.
  if (!swap_chain_ || quantized_size_ != quantized_size) {
    TRACE_EVENT0("gpu", "CreateAdditionalSwapChain");
    quantized_size_ = quantized_size;

    D3DPRESENT_PARAMETERS parameters = { 0 };
    parameters.BackBufferWidth = quantized_size.width();
    parameters.BackBufferHeight = quantized_size.height();
    parameters.BackBufferCount = 1;
    parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
    parameters.hDeviceWindow = GetShellWindow();
    parameters.Windowed = TRUE;
    parameters.Flags = 0;
    parameters.PresentationInterval = GetPresentationInterval();
    parameters.SwapEffect = D3DSWAPEFFECT_COPY;

    swap_chain_ = NULL;
    HRESULT hr = present_thread_->device()->CreateAdditionalSwapChain(
        &parameters,
        swap_chain_.Receive());
    if (FAILED(hr)) {
      LOG(ERROR) << "Failed to create swap chain "
                 << quantized_size.width() << " x " <<quantized_size.height();
      return;
    }
  }

  if (!source_texture_.get()) {
    TRACE_EVENT0("gpu", "OpenSharedTexture");
    if (!d3d_utils::OpenSharedTexture(present_thread_->device(),
                                      surface_handle,
                                      size,
                                      source_texture_.Receive())) {
      LOG(ERROR) << "Failed to open shared texture";
      return;
    }
  }

  base::win::ScopedComPtr<IDirect3DSurface9> source_surface;
  hr = source_texture_->GetSurfaceLevel(0, source_surface.Receive());
  if (FAILED(hr)) {
    TRACE_EVENT0("gpu", "EarlyOut_NoSurfaceLevel");
    LOG(ERROR) << "Failed to get source surface";
    return;
  }

  base::win::ScopedComPtr<IDirect3DSurface9> dest_surface;
  hr = swap_chain_->GetBackBuffer(0,
                                  D3DBACKBUFFER_TYPE_MONO,
                                  dest_surface.Receive());
  if (FAILED(hr)) {
    TRACE_EVENT0("gpu", "EarlyOut_NoBackbuffer");
    LOG(ERROR) << "Failed to get back buffer";
    return;
  }

  RECT rect = {
    0, 0,
    size.width(), size.height()
  };

  {
    TRACE_EVENT0("gpu", "Copy");

    // Copy while flipping the source texture on the vertical axis.
    bool result = present_thread_->surface_transformer()->CopyInverted(
        source_texture_, dest_surface, size);
    if (!result) {
      LOG(ERROR) << "Failed to copy shared texture";
      return;
    }
  }

  hr = present_thread_->query()->Issue(D3DISSUE_END);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to issue query";
    return;
  }

  present_size_ = size;

  // If it is expected that Direct3D cannot be used reliably because the window
  // is resizing, fall back to presenting with GDI.
  if (CheckDirect3DWillWork()) {
    TRACE_EVENT0("gpu", "PresentD3D");

    hr = swap_chain_->Present(&rect, &rect, window_, NULL, 0);

    // For latency_tests.cc:
    UNSHIPPED_TRACE_EVENT_INSTANT0("test_gpu", "CompositorSwapBuffersComplete",
                                   TRACE_EVENT_SCOPE_THREAD);

    if (FAILED(hr)) {
      if (present_thread_->IsDeviceLost())
        present_thread_->ResetDevice();
      return;
    }
  } else {
    HDC dc = GetDC(window_);
    PresentWithGDI(dc);
    ReleaseDC(window_, dc);
  }

  hidden_ = false;

  D3DDISPLAYMODE display_mode;
  hr = present_thread_->device()->GetDisplayMode(0, &display_mode);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to get display mode";
    return;
  }

  D3DRASTER_STATUS raster_status;
  hr = swap_chain_->GetRasterStatus(&raster_status);
  if (FAILED(hr)) {
    LOG(ERROR) << "Failed to get raster status";
    return;
  }

  // I can't figure out how to determine how many scanlines are in the
  // vertical blank so clamp it such that scanline / height <= 1.
  int clamped_scanline = std::min(raster_status.ScanLine, display_mode.Height);

  // The Internet says that on some GPUs, the scanline is not available
  // while in the vertical blank.
  if (raster_status.InVBlank)
    clamped_scanline = display_mode.Height;

  base::TimeTicks current_time = base::TimeTicks::HighResNow();

  // Figure out approximately how far back in time the last vsync was based on
  // the ratio of the raster scanline to the display height.
  base::TimeTicks last_vsync_time;
  base::TimeDelta refresh_period;
  if (display_mode.Height) {
      last_vsync_time = current_time -
        base::TimeDelta::FromMilliseconds((clamped_scanline * 1000) /
            (display_mode.RefreshRate * display_mode.Height));
      refresh_period = base::TimeDelta::FromMicroseconds(
          1000000 / display_mode.RefreshRate);
  }

  // Wait for the StretchRect to complete before notifying the GPU process
  // that it is safe to write to its backing store again.
  {
    TRACE_EVENT0("gpu", "spin");

    do {
      hr = present_thread_->query()->GetData(NULL, 0, D3DGETDATA_FLUSH);
      if (hr == S_FALSE) {
        Sleep(1);

        if (present_thread_->IsDeviceLost()) {
          present_thread_->ResetDevice();
          return;
        }
      }
    } while (hr == S_FALSE);
  }

  scoped_completion_runner.Release();
  completion_task.Run(true, last_vsync_time, refresh_period);
}

void AcceleratedPresenter::DoSuspend() {
  base::AutoLock locked(*present_thread_->lock());
  swap_chain_ = NULL;
}

void AcceleratedPresenter::DoReleaseSurface() {
  base::AutoLock locked(*present_thread_->lock());
  present_thread_->InitDevice();
  source_texture_.Release();
}

void AcceleratedPresenter::PresentWithGDI(HDC dc) {
  TRACE_EVENT0("gpu", "PresentWithGDI");

  if (!present_thread_->device()) {
    LOG(ERROR) << "No device";
    return;
  }

  if (!swap_chain_) {
    LOG(ERROR) << "No swap chain";
    return;
  }

  base::win::ScopedComPtr<IDirect3DTexture9> system_texture;
  {
    TRACE_EVENT0("gpu", "CreateSystemTexture");
    HRESULT hr = present_thread_->device()->CreateTexture(
        quantized_size_.width(),
        quantized_size_.height(),
        1,
        0,
        D3DFMT_A8R8G8B8,
        D3DPOOL_SYSTEMMEM,
        system_texture.Receive(),
        NULL);
    if (FAILED(hr)) {
      LOG(ERROR) << "Failed to create system memory texture";
      return;
    }
  }

  base::win::ScopedComPtr<IDirect3DSurface9> system_surface;
  HRESULT hr = system_texture->GetSurfaceLevel(0, system_surface.Receive());
  DCHECK(SUCCEEDED(hr));

  base::win::ScopedComPtr<IDirect3DSurface9> back_buffer;
  hr = swap_chain_->GetBackBuffer(0,
                                  D3DBACKBUFFER_TYPE_MONO,
                                  back_buffer.Receive());
  DCHECK(SUCCEEDED(hr));

  {
    TRACE_EVENT0("gpu", "GetRenderTargetData");
    hr = present_thread_->device()->GetRenderTargetData(back_buffer,
                                                        system_surface);

    if (FAILED(hr)) {
      if (present_thread_->IsDeviceLost()) {
        present_thread_->message_loop()->PostTask(
            FROM_HERE,
            base::Bind(&PresentThread::LockAndResetDevice, present_thread_));
      }
      return;
    }

    DCHECK(SUCCEEDED(hr));
  }

  D3DLOCKED_RECT locked_surface;
  hr = system_surface->LockRect(&locked_surface, NULL, D3DLOCK_READONLY);
  DCHECK(SUCCEEDED(hr));

  BITMAPINFO bitmap_info = {
    {
      sizeof(BITMAPINFOHEADER),
      quantized_size_.width(),
      -quantized_size_.height(),
      1,  // planes
      32,  // bitcount
      BI_RGB
    },
    {
      {0, 0, 0, 0}
    }
  };

  {
    TRACE_EVENT0("gpu", "StretchDIBits");
    StretchDIBits(dc,
                  0, 0,
                  present_size_.width(),
                  present_size_.height(),
                  0, 0,
                  present_size_.width(),
                  present_size_.height(),
                  locked_surface.pBits,
                  &bitmap_info,
                  DIB_RGB_COLORS,
                  SRCCOPY);
  }

  system_surface->UnlockRect();

  // For latency_tests.cc:
  UNSHIPPED_TRACE_EVENT_INSTANT0("test_gpu", "CompositorSwapBuffersComplete",
                                 TRACE_EVENT_SCOPE_THREAD);
}

gfx::Size AcceleratedPresenter::GetWindowSize() {
  RECT rect;
  GetClientRect(window_, &rect);
  return gfx::Rect(rect).size();
}

bool AcceleratedPresenter::CheckDirect3DWillWork() {
  // On a composited desktop, when the screen saver or logon screen are
  // active, D3D presents never make it to the window but GDI presents
  // do. If the session is locked GDI presents can be avoided since
  // the window gets a message on unlock and forces a repaint.
  if (!is_session_locked_ && ui::win::IsAeroGlassEnabled()) {
    // Failure to open the input desktop is a sign of running with a non-default
    // desktop.
    HDESK input_desktop = ::OpenInputDesktop(0, 0, GENERIC_READ);
    if (!input_desktop)
      return false;
    ::CloseDesktop(input_desktop);
  }

  gfx::Size window_size = GetWindowSize();
  if (window_size != last_window_size_ && last_window_size_.GetArea() != 0) {
    last_window_size_ = window_size;
    last_window_resize_time_ = base::Time::Now();
    return false;
  }

  if (do_present_with_GDI_ && hidden_) {
    if (DoFirstShowPresentWithGDI())
      do_present_with_GDI_ = false;

    return false;
  }

  return base::Time::Now() - last_window_resize_time_ >
      base::TimeDelta::FromMilliseconds(100);
}

AcceleratedSurface::AcceleratedSurface(gfx::PluginWindowHandle window)
    : presenter_(g_accelerated_presenter_map.Pointer()->CreatePresenter(
          window)) {
}

AcceleratedSurface::~AcceleratedSurface() {
  g_accelerated_presenter_map.Pointer()->RemovePresenter(presenter_);
  presenter_->Invalidate();
}

void AcceleratedSurface::Present(HDC dc) {
  presenter_->Present(dc);
}

void AcceleratedSurface::AsyncCopyTo(
    const gfx::Rect& src_subrect,
    const gfx::Size& dst_size,
    const base::Callback<void(bool, const SkBitmap&)>& callback) {
  presenter_->AsyncCopyTo(src_subrect, dst_size, callback);
}

void AcceleratedSurface::AsyncCopyToVideoFrame(
    const gfx::Rect& src_subrect,
    const scoped_refptr<media::VideoFrame>& target,
    const base::Callback<void(bool)>& callback) {
  presenter_->AsyncCopyToVideoFrame(src_subrect, target, callback);
}

void AcceleratedSurface::Suspend() {
  presenter_->Suspend();
}

void AcceleratedSurface::WasHidden() {
  presenter_->WasHidden();
}

void AcceleratedSurface::SetIsSessionLocked(bool locked) {
  presenter_->SetIsSessionLocked(locked);
}
