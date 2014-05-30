// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/compositor/compositor.h"

#include <algorithm>
#include <deque>

#include "base/bind.h"
#include "base/command_line.h"
#include "base/memory/singleton.h"
#include "base/message_loop.h"
#include "base/run_loop.h"
#include "base/string_util.h"
#include "base/threading/thread.h"
#include "base/threading/thread_restrictions.h"
#include "cc/base/switches.h"
#include "cc/base/thread_impl.h"
#include "cc/input/input_handler.h"
#include "cc/layers/layer.h"
#include "cc/output/context_provider.h"
#include "cc/output/output_surface.h"
#include "cc/trees/layer_tree_host.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/compositor/compositor_observer.h"
#include "ui/compositor/compositor_switches.h"
#include "ui/compositor/dip_util.h"
#include "ui/compositor/layer.h"
#include "ui/compositor/test_web_graphics_context_3d.h"
#include "ui/gl/gl_context.h"
#include "ui/gl/gl_implementation.h"
#include "ui/gl/gl_surface.h"
#include "ui/gl/gl_switches.h"
#include "webkit/gpu/grcontext_for_webgraphicscontext3d.h"
#include "webkit/gpu/webgraphicscontext3d_in_process_command_buffer_impl.h"

#if defined(OS_CHROMEOS)
#include "base/chromeos/chromeos_version.h"
#endif

namespace {

const double kDefaultRefreshRate = 60.0;
const double kTestRefreshRate = 200.0;

enum SwapType {
  DRAW_SWAP,
  READPIXELS_SWAP,
};

base::Thread* g_compositor_thread = NULL;

bool g_test_compositor_enabled = false;

ui::ContextFactory* g_implicit_factory = NULL;
ui::ContextFactory* g_context_factory = NULL;

const int kCompositorLockTimeoutMs = 67;

class PendingSwap {
 public:
  PendingSwap(SwapType type, ui::PostedSwapQueue* posted_swaps);
  ~PendingSwap();

  SwapType type() const { return type_; }
  bool posted() const { return posted_; }

 private:
  friend class ui::PostedSwapQueue;

  SwapType type_;
  bool posted_;
  ui::PostedSwapQueue* posted_swaps_;

  DISALLOW_COPY_AND_ASSIGN(PendingSwap);
};

void SetupImplicitFactory() {
  // We leak the implicit factory so that we don't race with the tear down of
  // the gl_bindings.
  DCHECK(!g_context_factory);
  DCHECK(!g_implicit_factory);
  if (g_test_compositor_enabled) {
    g_implicit_factory = new ui::TestContextFactory;
  } else {
    DVLOG(1) << "Using DefaultContextFactory";
    scoped_ptr<ui::DefaultContextFactory> instance(
        new ui::DefaultContextFactory());
    if (instance->Initialize())
      g_implicit_factory = instance.release();
  }
  g_context_factory = g_implicit_factory;
}

void ResetImplicitFactory() {
  if (!g_implicit_factory || g_context_factory != g_implicit_factory)
    return;
  delete g_implicit_factory;
  g_implicit_factory = NULL;
  g_context_factory = NULL;
}

}  // namespace

namespace ui {

// static
ContextFactory* ContextFactory::GetInstance() {
  if (!g_context_factory)
    SetupImplicitFactory();
  return g_context_factory;
}

// static
void ContextFactory::SetInstance(ContextFactory* instance) {
  g_context_factory = instance;
}

class ContextProviderFromContextFactory : public cc::ContextProvider {
 public:
  static scoped_refptr<ContextProviderFromContextFactory> Create(
      ContextFactory* factory) {
    scoped_refptr<ContextProviderFromContextFactory> provider =
        new ContextProviderFromContextFactory(factory);
    if (!provider->InitializeOnMainThread())
      return NULL;
    return provider;
  }

  virtual bool BindToCurrentThread() OVERRIDE {
    DCHECK(context3d_);

    return context3d_->makeContextCurrent();
  }

  virtual WebKit::WebGraphicsContext3D* Context3d() OVERRIDE {
    DCHECK(context3d_);

    return context3d_.get();
  }

  virtual class GrContext* GrContext() OVERRIDE {
    DCHECK(context3d_);

    if (!gr_context_) {
      gr_context_.reset(
          new webkit::gpu::GrContextForWebGraphicsContext3D(context3d_.get()));
    }
    return gr_context_->get();
  }

  virtual void VerifyContexts() OVERRIDE {
    DCHECK(context3d_);

    if (context3d_->isContextLost()) {
      base::AutoLock lock(destroyed_lock_);
      destroyed_ = true;
    }
  }

  virtual bool DestroyedOnMainThread() OVERRIDE {
    base::AutoLock lock(destroyed_lock_);
    return destroyed_;
  }

 protected:
  explicit ContextProviderFromContextFactory(ContextFactory* factory)
      : factory_(factory),
        destroyed_(false) {}
  virtual ~ContextProviderFromContextFactory() {}

  bool InitializeOnMainThread() {
    if (context3d_)
      return true;
    context3d_.reset(factory_->CreateOffscreenContext());
    return !!context3d_;
  }

 private:
  ContextFactory* factory_;
  base::Lock destroyed_lock_;
  bool destroyed_;
  scoped_ptr<WebKit::WebGraphicsContext3D> context3d_;
  scoped_ptr<webkit::gpu::GrContextForWebGraphicsContext3D> gr_context_;
};

DefaultContextFactory::DefaultContextFactory() {
}

DefaultContextFactory::~DefaultContextFactory() {
}

bool DefaultContextFactory::Initialize() {
  if (!gfx::GLSurface::InitializeOneOff() ||
      gfx::GetGLImplementation() == gfx::kGLImplementationNone) {
    LOG(ERROR) << "Could not load the GL bindings";
    return false;
  }
  return true;
}

cc::OutputSurface* DefaultContextFactory::CreateOutputSurface(
    Compositor* compositor) {
  return new cc::OutputSurface(
      make_scoped_ptr(CreateContextCommon(compositor, false)));
}

WebKit::WebGraphicsContext3D* DefaultContextFactory::CreateOffscreenContext() {
  return CreateContextCommon(NULL, true);
}

scoped_refptr<cc::ContextProvider>
DefaultContextFactory::OffscreenContextProviderForMainThread() {
  if (!offscreen_contexts_main_thread_ ||
      !offscreen_contexts_main_thread_->DestroyedOnMainThread()) {
    offscreen_contexts_main_thread_ =
        ContextProviderFromContextFactory::Create(this);
    if (offscreen_contexts_main_thread_ &&
        !offscreen_contexts_main_thread_->BindToCurrentThread())
      offscreen_contexts_main_thread_ = NULL;
  }
  return offscreen_contexts_main_thread_;
}

scoped_refptr<cc::ContextProvider>
DefaultContextFactory::OffscreenContextProviderForCompositorThread() {
  if (!offscreen_contexts_compositor_thread_ ||
      !offscreen_contexts_compositor_thread_->DestroyedOnMainThread()) {
    offscreen_contexts_compositor_thread_ =
        ContextProviderFromContextFactory::Create(this);
  }
  return offscreen_contexts_compositor_thread_;
}

void DefaultContextFactory::RemoveCompositor(Compositor* compositor) {
}

WebKit::WebGraphicsContext3D* DefaultContextFactory::CreateContextCommon(
    Compositor* compositor,
    bool offscreen) {
  DCHECK(offscreen || compositor);
  WebKit::WebGraphicsContext3D::Attributes attrs;
  attrs.depth = false;
  attrs.stencil = false;
  attrs.antialias = false;
  attrs.shareResources = true;
  using webkit::gpu::WebGraphicsContext3DInProcessCommandBufferImpl;
  WebKit::WebGraphicsContext3D* context =
      offscreen ?
      WebGraphicsContext3DInProcessCommandBufferImpl::CreateOffscreenContext(
          attrs) :
      WebGraphicsContext3DInProcessCommandBufferImpl::CreateViewContext(
          attrs, compositor->widget());
  if (!context)
    return NULL;

  CommandLine* command_line = CommandLine::ForCurrentProcess();
  if (!offscreen) {
    context->makeContextCurrent();
    gfx::GLContext* gl_context = gfx::GLContext::GetCurrent();
    bool vsync = !command_line->HasSwitch(switches::kDisableGpuVsync);
    gl_context->SetSwapInterval(vsync ? 1 : 0);
    gl_context->ReleaseCurrent(NULL);
  }
  return context;
}

TestContextFactory::TestContextFactory() {}

TestContextFactory::~TestContextFactory() {}

cc::OutputSurface* TestContextFactory::CreateOutputSurface(
    Compositor* compositor) {
  return new cc::OutputSurface(make_scoped_ptr(CreateOffscreenContext()));
}

WebKit::WebGraphicsContext3D* TestContextFactory::CreateOffscreenContext() {
  ui::TestWebGraphicsContext3D* context = new ui::TestWebGraphicsContext3D;
  context->Initialize();
  return context;
}

scoped_refptr<cc::ContextProvider>
TestContextFactory::OffscreenContextProviderForMainThread() {
  if (!offscreen_contexts_main_thread_ ||
      offscreen_contexts_main_thread_->DestroyedOnMainThread()) {
    offscreen_contexts_main_thread_ =
        ContextProviderFromContextFactory::Create(this);
    CHECK(offscreen_contexts_main_thread_->BindToCurrentThread());
  }
  return offscreen_contexts_main_thread_;
}

scoped_refptr<cc::ContextProvider>
TestContextFactory::OffscreenContextProviderForCompositorThread() {
  if (!offscreen_contexts_compositor_thread_ ||
      offscreen_contexts_compositor_thread_->DestroyedOnMainThread()) {
    offscreen_contexts_compositor_thread_ =
        ContextProviderFromContextFactory::Create(this);
  }
  return offscreen_contexts_compositor_thread_;
}

void TestContextFactory::RemoveCompositor(Compositor* compositor) {
}

Texture::Texture(bool flipped, const gfx::Size& size, float device_scale_factor)
    : size_(size),
      flipped_(flipped),
      device_scale_factor_(device_scale_factor) {
}

Texture::~Texture() {
}

std::string Texture::Produce() {
  return EmptyString();
}

CompositorLock::CompositorLock(Compositor* compositor)
    : compositor_(compositor) {
  base::MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&CompositorLock::CancelLock, AsWeakPtr()),
      base::TimeDelta::FromMilliseconds(kCompositorLockTimeoutMs));
}

CompositorLock::~CompositorLock() {
  CancelLock();
}

void CompositorLock::CancelLock() {
  if (!compositor_)
    return;
  compositor_->UnlockCompositor();
  compositor_ = NULL;
}

// static
void DrawWaiterForTest::Wait(Compositor* compositor) {
  DrawWaiterForTest waiter;
  waiter.WaitImpl(compositor);
}

DrawWaiterForTest::DrawWaiterForTest() {
}

DrawWaiterForTest::~DrawWaiterForTest() {
}

void DrawWaiterForTest::WaitImpl(Compositor* compositor) {
  compositor->AddObserver(this);
  wait_run_loop_.reset(new base::RunLoop());
  wait_run_loop_->Run();
  compositor->RemoveObserver(this);
}

void DrawWaiterForTest::OnCompositingDidCommit(Compositor* compositor) {
}

void DrawWaiterForTest::OnCompositingStarted(Compositor* compositor,
                                             base::TimeTicks start_time) {
}

void DrawWaiterForTest::OnCompositingEnded(Compositor* compositor) {
  wait_run_loop_->Quit();
}

void DrawWaiterForTest::OnCompositingAborted(Compositor* compositor) {
}

void DrawWaiterForTest::OnCompositingLockStateChanged(Compositor* compositor) {
}

void DrawWaiterForTest::OnUpdateVSyncParameters(Compositor* compositor,
                                                base::TimeTicks timebase,
                                                base::TimeDelta interval) {
}

class PostedSwapQueue {
 public:
  PostedSwapQueue() : pending_swap_(NULL) {
  }

  ~PostedSwapQueue() {
    DCHECK(!pending_swap_);
  }

  SwapType NextPostedSwap() const {
    return queue_.front();
  }

  bool AreSwapsPosted() const {
    return !queue_.empty();
  }

  int NumSwapsPosted(SwapType type) const {
    int count = 0;
    for (std::deque<SwapType>::const_iterator it = queue_.begin();
         it != queue_.end(); ++it) {
      if (*it == type)
        count++;
    }
    return count;
  }

  void PostSwap() {
    DCHECK(pending_swap_);
    queue_.push_back(pending_swap_->type());
    pending_swap_->posted_ = true;
  }

  void EndSwap() {
    queue_.pop_front();
  }

 private:
  friend class ::PendingSwap;

  PendingSwap* pending_swap_;
  std::deque<SwapType> queue_;

  DISALLOW_COPY_AND_ASSIGN(PostedSwapQueue);
};

}  // namespace ui

namespace {

PendingSwap::PendingSwap(SwapType type, ui::PostedSwapQueue* posted_swaps)
    : type_(type), posted_(false), posted_swaps_(posted_swaps) {
  // Only one pending swap in flight.
  DCHECK_EQ(static_cast<PendingSwap*>(NULL), posted_swaps_->pending_swap_);
  posted_swaps_->pending_swap_ = this;
}

PendingSwap::~PendingSwap() {
  DCHECK_EQ(this, posted_swaps_->pending_swap_);
  posted_swaps_->pending_swap_ = NULL;
}

}  // namespace

namespace ui {

Compositor::Compositor(CompositorDelegate* delegate,
                       gfx::AcceleratedWidget widget)
    : delegate_(delegate),
      root_layer_(NULL),
      widget_(widget),
      posted_swaps_(new PostedSwapQueue()),
      device_scale_factor_(0.0f),
      last_started_frame_(0),
      last_ended_frame_(0),
      next_draw_is_resize_(false),
      disable_schedule_composite_(false),
      compositor_lock_(NULL) {
  root_web_layer_ = cc::Layer::Create();
  root_web_layer_->SetAnchorPoint(gfx::PointF(0.f, 0.f));
  // TODO(piman): remove after crbug.com/235302 is fixed.
  root_web_layer_->SetMasksToBounds(true);

  CommandLine* command_line = CommandLine::ForCurrentProcess();

  cc::LayerTreeSettings settings;
  settings.refresh_rate =
      g_test_compositor_enabled ? kTestRefreshRate : kDefaultRefreshRate;
  settings.partial_swap_enabled =
      !command_line->HasSwitch(cc::switches::kUIDisablePartialSwap);
  settings.per_tile_painting_enabled =
      command_line->HasSwitch(cc::switches::kUIEnablePerTilePainting);

  // These flags should be mirrored by renderer versions in content/renderer/.
  settings.initial_debug_state.show_debug_borders =
      command_line->HasSwitch(cc::switches::kUIShowCompositedLayerBorders);
  settings.initial_debug_state.show_fps_counter =
      command_line->HasSwitch(cc::switches::kUIShowFPSCounter);
  settings.initial_debug_state.show_paint_rects =
      command_line->HasSwitch(switches::kUIShowPaintRects);
  settings.initial_debug_state.show_platform_layer_tree =
      command_line->HasSwitch(cc::switches::kUIShowCompositedLayerTree);
  settings.initial_debug_state.show_property_changed_rects =
      command_line->HasSwitch(cc::switches::kUIShowPropertyChangedRects);
  settings.initial_debug_state.show_surface_damage_rects =
      command_line->HasSwitch(cc::switches::kUIShowSurfaceDamageRects);
  settings.initial_debug_state.show_screen_space_rects =
      command_line->HasSwitch(cc::switches::kUIShowScreenSpaceRects);
  settings.initial_debug_state.show_replica_screen_space_rects =
      command_line->HasSwitch(cc::switches::kUIShowReplicaScreenSpaceRects);
  settings.initial_debug_state.show_occluding_rects =
      command_line->HasSwitch(cc::switches::kUIShowOccludingRects);
  settings.initial_debug_state.show_non_occluding_rects =
      command_line->HasSwitch(cc::switches::kUIShowNonOccludingRects);

  scoped_ptr<cc::Thread> thread;
  if (g_compositor_thread) {
    thread = cc::ThreadImpl::CreateForDifferentThread(
        g_compositor_thread->message_loop_proxy());
  }

  host_ = cc::LayerTreeHost::Create(this, settings, thread.Pass());
  host_->SetRootLayer(root_web_layer_);
  host_->SetSurfaceReady();
}

Compositor::~Compositor() {
  CancelCompositorLock();
  DCHECK(!compositor_lock_);

  // Don't call |CompositorDelegate::ScheduleDraw| from this point.
  delegate_ = NULL;
  if (root_layer_)
    root_layer_->SetCompositor(NULL);

  // Stop all outstanding draws before telling the ContextFactory to tear
  // down any contexts that the |host_| may rely upon.
  host_.reset();

  ContextFactory::GetInstance()->RemoveCompositor(this);
}

// static
void Compositor::Initialize() {
#if defined(OS_CHROMEOS)
  bool use_thread = !CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kUIDisableThreadedCompositing);
#else
  bool use_thread =
      CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kUIEnableThreadedCompositing) &&
      !CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kUIDisableThreadedCompositing);
#endif
  if (use_thread) {
    g_compositor_thread = new base::Thread("Browser Compositor");
    g_compositor_thread->Start();
  }
}

// static
bool Compositor::WasInitializedWithThread() {
  return !!g_compositor_thread;
}

// static
void Compositor::Terminate() {
  if (g_compositor_thread) {
    g_compositor_thread->Stop();
    delete g_compositor_thread;
    g_compositor_thread = NULL;
  }
}

void Compositor::ScheduleDraw() {
  if (g_compositor_thread)
    host_->Composite(base::TimeTicks::Now());
  else if (delegate_)
    delegate_->ScheduleDraw();
}

void Compositor::SetRootLayer(Layer* root_layer) {
  if (root_layer_ == root_layer)
    return;
  if (root_layer_)
    root_layer_->SetCompositor(NULL);
  root_layer_ = root_layer;
  if (root_layer_ && !root_layer_->GetCompositor())
    root_layer_->SetCompositor(this);
  root_web_layer_->RemoveAllChildren();
  if (root_layer_)
    root_web_layer_->AddChild(root_layer_->cc_layer());
}

void Compositor::SetHostHasTransparentBackground(
    bool host_has_transparent_background) {
  host_->set_has_transparent_background(host_has_transparent_background);
}

void Compositor::Draw() {
  DCHECK(!g_compositor_thread);

  if (!root_layer_)
    return;

  last_started_frame_++;
  PendingSwap pending_swap(DRAW_SWAP, posted_swaps_.get());
  if (!IsLocked()) {
    // TODO(nduca): Temporary while compositor calls
    // compositeImmediately() directly.
    Layout();
    host_->Composite(base::TimeTicks::Now());

#if defined(OS_WIN)
    // While we resize, we are usually a few frames behind. By blocking
    // the UI thread here we minize the area that is mis-painted, specially
    // in the non-client area. See RenderWidgetHostViewAura::SetBounds for
    // more details and bug 177115.
    if (next_draw_is_resize_ && (last_ended_frame_ > 1)) {
      next_draw_is_resize_ = false;
      host_->FinishAllRendering();
    }
#endif

  }
  if (!pending_swap.posted())
    NotifyEnd();
}

void Compositor::ScheduleFullRedraw() {
  host_->SetNeedsRedraw();
}

void Compositor::ScheduleRedrawRect(const gfx::Rect& damage_rect) {
  host_->SetNeedsRedrawRect(damage_rect);
}

bool Compositor::ReadPixels(SkBitmap* bitmap,
                            const gfx::Rect& bounds_in_pixel) {
  if (bounds_in_pixel.right() > size().width() ||
      bounds_in_pixel.bottom() > size().height())
    return false;
  bitmap->setConfig(SkBitmap::kARGB_8888_Config,
                    bounds_in_pixel.width(), bounds_in_pixel.height());
  bitmap->allocPixels();
  SkAutoLockPixels lock_image(*bitmap);
  unsigned char* pixels = static_cast<unsigned char*>(bitmap->getPixels());
  CancelCompositorLock();
  PendingSwap pending_swap(READPIXELS_SWAP, posted_swaps_.get());
  return host_->CompositeAndReadback(pixels, bounds_in_pixel);
}

void Compositor::SetScaleAndSize(float scale, const gfx::Size& size_in_pixel) {
  DCHECK_GT(scale, 0);
  if (!size_in_pixel.IsEmpty()) {
    size_ = size_in_pixel;
    host_->SetViewportSize(size_in_pixel);
    root_web_layer_->SetBounds(size_in_pixel);

    next_draw_is_resize_ = true;
  }
  if (device_scale_factor_ != scale) {
    device_scale_factor_ = scale;
    if (root_layer_)
      root_layer_->OnDeviceScaleFactorChanged(scale);
  }
}

void Compositor::SetBackgroundColor(SkColor color) {
  host_->set_background_color(color);
  ScheduleDraw();
}

void Compositor::AddObserver(CompositorObserver* observer) {
  observer_list_.AddObserver(observer);
}

void Compositor::RemoveObserver(CompositorObserver* observer) {
  observer_list_.RemoveObserver(observer);
}

bool Compositor::HasObserver(CompositorObserver* observer) {
  return observer_list_.HasObserver(observer);
}

void Compositor::OnSwapBuffersPosted() {
  DCHECK(!g_compositor_thread);
  posted_swaps_->PostSwap();
}

void Compositor::OnSwapBuffersComplete() {
  DCHECK(!g_compositor_thread);
  DCHECK(posted_swaps_->AreSwapsPosted());
  DCHECK_GE(1, posted_swaps_->NumSwapsPosted(DRAW_SWAP));
  if (posted_swaps_->NextPostedSwap() == DRAW_SWAP)
    NotifyEnd();
  posted_swaps_->EndSwap();
}

void Compositor::OnSwapBuffersAborted() {
  if (!g_compositor_thread) {
    DCHECK_GE(1, posted_swaps_->NumSwapsPosted(DRAW_SWAP));

    // We've just lost the context, so unwind all posted_swaps.
    while (posted_swaps_->AreSwapsPosted()) {
      if (posted_swaps_->NextPostedSwap() == DRAW_SWAP)
        NotifyEnd();
      posted_swaps_->EndSwap();
    }
  }

  FOR_EACH_OBSERVER(CompositorObserver,
                    observer_list_,
                    OnCompositingAborted(this));
}

void Compositor::OnUpdateVSyncParameters(base::TimeTicks timebase,
                                         base::TimeDelta interval) {
  FOR_EACH_OBSERVER(CompositorObserver,
                    observer_list_,
                    OnUpdateVSyncParameters(this, timebase, interval));
}

void Compositor::Layout() {
  // We're sending damage that will be addressed during this composite
  // cycle, so we don't need to schedule another composite to address it.
  disable_schedule_composite_ = true;
  if (root_layer_)
    root_layer_->SendDamagedRects();
  disable_schedule_composite_ = false;
}

scoped_ptr<cc::OutputSurface> Compositor::CreateOutputSurface() {
  return make_scoped_ptr(
      ContextFactory::GetInstance()->CreateOutputSurface(this));
}

scoped_ptr<cc::InputHandlerClient> Compositor::CreateInputHandlerClient() {
  return scoped_ptr<cc::InputHandlerClient>();
}

void Compositor::DidCommit() {
  DCHECK(!IsLocked());
  FOR_EACH_OBSERVER(CompositorObserver,
                    observer_list_,
                    OnCompositingDidCommit(this));
}

void Compositor::DidCommitAndDrawFrame() {
  base::TimeTicks start_time = base::TimeTicks::Now();
  FOR_EACH_OBSERVER(CompositorObserver,
                    observer_list_,
                    OnCompositingStarted(this, start_time));
  // If we're threaded without a swap complete callback, we have to
  // call DidCompleteSwapBuffersManually.
  if (g_compositor_thread &&
      !host_->GetRendererCapabilities().using_swap_complete_callback)
    DidCompleteSwapBuffers();
}

void Compositor::DidCompleteSwapBuffers() {
  DCHECK(g_compositor_thread);
  NotifyEnd();
}

void Compositor::ScheduleComposite() {
  if (!disable_schedule_composite_)
    ScheduleDraw();
}

scoped_refptr<cc::ContextProvider>
Compositor::OffscreenContextProviderForMainThread() {
  return ContextFactory::GetInstance()->OffscreenContextProviderForMainThread();
}

scoped_refptr<cc::ContextProvider>
Compositor::OffscreenContextProviderForCompositorThread() {
  return ContextFactory::GetInstance()->
      OffscreenContextProviderForCompositorThread();
}

const cc::LayerTreeDebugState& Compositor::GetLayerTreeDebugState() const {
  return host_->debug_state();
}

void Compositor::SetLayerTreeDebugState(
    const cc::LayerTreeDebugState& debug_state) {
  host_->SetDebugState(debug_state);
}

scoped_refptr<CompositorLock> Compositor::GetCompositorLock() {
  if (!compositor_lock_) {
    compositor_lock_ = new CompositorLock(this);
    if (g_compositor_thread)
      host_->SetDeferCommits(true);
    FOR_EACH_OBSERVER(CompositorObserver,
                      observer_list_,
                      OnCompositingLockStateChanged(this));
  }
  return compositor_lock_;
}

void Compositor::UnlockCompositor() {
  DCHECK(compositor_lock_);
  compositor_lock_ = NULL;
  if (g_compositor_thread)
    host_->SetDeferCommits(false);
  FOR_EACH_OBSERVER(CompositorObserver,
                    observer_list_,
                    OnCompositingLockStateChanged(this));
}

void Compositor::CancelCompositorLock() {
  if (compositor_lock_)
    compositor_lock_->CancelLock();
}

void Compositor::NotifyEnd() {
  last_ended_frame_++;
  FOR_EACH_OBSERVER(CompositorObserver,
                    observer_list_,
                    OnCompositingEnded(this));
}

COMPOSITOR_EXPORT void SetupTestCompositor() {
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableTestCompositor)) {
    g_test_compositor_enabled = true;
  }
#if defined(OS_CHROMEOS)
  // If the test is running on the chromeos envrionment (such as
  // device or vm bots), use the real compositor.
  if (base::chromeos::IsRunningOnChromeOS())
    g_test_compositor_enabled = false;
#endif
  ResetImplicitFactory();
}

COMPOSITOR_EXPORT void DisableTestCompositor() {
  ResetImplicitFactory();
  g_test_compositor_enabled = false;
}

COMPOSITOR_EXPORT bool IsTestCompositorEnabled() {
  return g_test_compositor_enabled;
}

}  // namespace ui
