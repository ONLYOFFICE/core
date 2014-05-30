// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_COMPOSITOR_H_
#define UI_COMPOSITOR_COMPOSITOR_H_

#include <string>

#include "base/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/time.h"
#include "cc/trees/layer_tree_host_client.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/compositor/compositor_export.h"
#include "ui/compositor/compositor_observer.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"
#include "ui/gfx/transform.h"
#include "ui/gfx/vector2d.h"
#include "ui/gl/gl_share_group.h"

class SkBitmap;

namespace base {
class RunLoop;
}

namespace cc {
class ContextProvider;
class Layer;
class LayerTreeDebugState;
class LayerTreeHost;
}

namespace gfx {
class GLContext;
class GLSurface;
class GLShareGroup;
class Point;
class Rect;
}

namespace WebKit {
class WebGraphicsContext3D;
}

namespace ui {

class Compositor;
class CompositorObserver;
class ContextProviderFromContextFactory;
class Layer;
class PostedSwapQueue;

// This class abstracts the creation of the 3D context for the compositor. It is
// a global object.
class COMPOSITOR_EXPORT ContextFactory {
 public:
  virtual ~ContextFactory() {}

  // Gets the global instance.
  static ContextFactory* GetInstance();

  // Sets the global instance. Caller keeps ownership.
  // If this function isn't called (for tests), a "default" factory will be
  // created on the first call of GetInstance.
  static void SetInstance(ContextFactory* instance);

  // Creates an output surface for the given compositor. The factory may keep
  // per-compositor data (e.g. a shared context), that needs to be cleaned up
  // by calling RemoveCompositor when the compositor gets destroyed.
  virtual cc::OutputSurface* CreateOutputSurface(
      Compositor* compositor) = 0;

  // Creates a context used for offscreen rendering. This context can be shared
  // with all compositors.
  virtual WebKit::WebGraphicsContext3D* CreateOffscreenContext() = 0;

  virtual scoped_refptr<cc::ContextProvider>
      OffscreenContextProviderForMainThread() = 0;
  virtual scoped_refptr<cc::ContextProvider>
      OffscreenContextProviderForCompositorThread() = 0;

  // Destroys per-compositor data.
  virtual void RemoveCompositor(Compositor* compositor) = 0;
};

// The default factory that creates in-process contexts.
class COMPOSITOR_EXPORT DefaultContextFactory : public ContextFactory {
 public:
  DefaultContextFactory();
  virtual ~DefaultContextFactory();

  // ContextFactory implementation
  virtual cc::OutputSurface* CreateOutputSurface(
      Compositor* compositor) OVERRIDE;
  virtual WebKit::WebGraphicsContext3D* CreateOffscreenContext() OVERRIDE;
  virtual scoped_refptr<cc::ContextProvider>
      OffscreenContextProviderForMainThread() OVERRIDE;
  virtual scoped_refptr<cc::ContextProvider>
      OffscreenContextProviderForCompositorThread() OVERRIDE;
  virtual void RemoveCompositor(Compositor* compositor) OVERRIDE;

  bool Initialize();

 private:
  WebKit::WebGraphicsContext3D* CreateContextCommon(
      Compositor* compositor,
      bool offscreen);

  scoped_refptr<ContextProviderFromContextFactory>
      offscreen_contexts_main_thread_;
  scoped_refptr<ContextProviderFromContextFactory>
      offscreen_contexts_compositor_thread_;

  DISALLOW_COPY_AND_ASSIGN(DefaultContextFactory);
};

// The factory that creates test contexts.
class COMPOSITOR_EXPORT TestContextFactory : public ContextFactory {
 public:
  TestContextFactory();
  virtual ~TestContextFactory();

  // ContextFactory implementation
  virtual cc::OutputSurface* CreateOutputSurface(
      Compositor* compositor) OVERRIDE;
  virtual WebKit::WebGraphicsContext3D* CreateOffscreenContext() OVERRIDE;
  virtual scoped_refptr<cc::ContextProvider>
      OffscreenContextProviderForMainThread() OVERRIDE;
  virtual scoped_refptr<cc::ContextProvider>
      OffscreenContextProviderForCompositorThread() OVERRIDE;
  virtual void RemoveCompositor(Compositor* compositor) OVERRIDE;

 private:
  scoped_refptr<ContextProviderFromContextFactory>
      offscreen_contexts_main_thread_;
  scoped_refptr<ContextProviderFromContextFactory>
      offscreen_contexts_compositor_thread_;

  DISALLOW_COPY_AND_ASSIGN(TestContextFactory);
};

// Texture provide an abstraction over the external texture that can be passed
// to a layer.
class COMPOSITOR_EXPORT Texture : public base::RefCounted<Texture> {
 public:
  Texture(bool flipped, const gfx::Size& size, float device_scale_factor);

  bool flipped() const { return flipped_; }
  gfx::Size size() const { return size_; }
  float device_scale_factor() const { return device_scale_factor_; }

  virtual unsigned int PrepareTexture() = 0;
  virtual WebKit::WebGraphicsContext3D* HostContext3D() = 0;

  // Replaces the texture with the texture from the specified mailbox.
  virtual void Consume(const std::string& mailbox_name,
                       const gfx::Size& new_size) {}

  // Moves the texture into the mailbox and returns the mailbox name.
  // The texture must have been previously consumed from a mailbox.
  virtual std::string Produce();

 protected:
  virtual ~Texture();
  gfx::Size size_;  // in pixel

 private:
  friend class base::RefCounted<Texture>;

  bool flipped_;
  float device_scale_factor_;

  DISALLOW_COPY_AND_ASSIGN(Texture);
};

// An interface to allow the compositor to communicate with its owner.
class COMPOSITOR_EXPORT CompositorDelegate {
 public:
  // Requests the owner to schedule a redraw of the layer tree.
  virtual void ScheduleDraw() = 0;

 protected:
  virtual ~CompositorDelegate() {}
};

// This class represents a lock on the compositor, that can be used to prevent
// commits to the compositor tree while we're waiting for an asynchronous
// event. The typical use case is when waiting for a renderer to produce a frame
// at the right size. The caller keeps a reference on this object, and drops the
// reference once it desires to release the lock.
// Note however that the lock is cancelled after a short timeout to ensure
// responsiveness of the UI, so the compositor tree should be kept in a
// "reasonable" state while the lock is held.
// Don't instantiate this class directly, use Compositor::GetCompositorLock.
class COMPOSITOR_EXPORT CompositorLock
    : public base::RefCounted<CompositorLock>,
      public base::SupportsWeakPtr<CompositorLock> {
 private:
  friend class base::RefCounted<CompositorLock>;
  friend class Compositor;

  explicit CompositorLock(Compositor* compositor);
  ~CompositorLock();

  void CancelLock();

  Compositor* compositor_;
  DISALLOW_COPY_AND_ASSIGN(CompositorLock);
};

// This is only to be used for test. It allows execution of other tasks on
// the current message loop before the current task finishs (there is a
// potential for re-entrancy).
class COMPOSITOR_EXPORT DrawWaiterForTest : public ui::CompositorObserver {
 public:
  // Waits for a draw to be issued by the compositor. If the test times out
  // here, there may be a logic error in the compositor code causing it
  // not to draw.
  static void Wait(Compositor* compositor);

 private:
  DrawWaiterForTest();
  virtual ~DrawWaiterForTest();

  void WaitImpl(Compositor* compositor);

  // CompositorObserver implementation.
  virtual void OnCompositingDidCommit(Compositor* compositor) OVERRIDE;
  virtual void OnCompositingStarted(Compositor* compositor,
                                    base::TimeTicks start_time) OVERRIDE;
  virtual void OnCompositingEnded(Compositor* compositor) OVERRIDE;
  virtual void OnCompositingAborted(Compositor* compositor) OVERRIDE;
  virtual void OnCompositingLockStateChanged(Compositor* compositor) OVERRIDE;
  virtual void OnUpdateVSyncParameters(Compositor* compositor,
                                       base::TimeTicks timebase,
                                       base::TimeDelta interval) OVERRIDE;

  scoped_ptr<base::RunLoop> wait_run_loop_;

  DISALLOW_COPY_AND_ASSIGN(DrawWaiterForTest);
};

// Compositor object to take care of GPU painting.
// A Browser compositor object is responsible for generating the final
// displayable form of pixels comprising a single widget's contents. It draws an
// appropriately transformed texture for each transformed view in the widget's
// view hierarchy.
class COMPOSITOR_EXPORT Compositor
    : NON_EXPORTED_BASE(public cc::LayerTreeHostClient),
      public base::SupportsWeakPtr<Compositor> {
 public:
  Compositor(CompositorDelegate* delegate,
             gfx::AcceleratedWidget widget);
  virtual ~Compositor();

  static void Initialize();
  static bool WasInitializedWithThread();
  static void Terminate();

  // Schedules a redraw of the layer tree associated with this compositor.
  void ScheduleDraw();

  // Sets the root of the layer tree drawn by this Compositor. The root layer
  // must have no parent. The compositor's root layer is reset if the root layer
  // is destroyed. NULL can be passed to reset the root layer, in which case the
  // compositor will stop drawing anything.
  // The Compositor does not own the root layer.
  const Layer* root_layer() const { return root_layer_; }
  Layer* root_layer() { return root_layer_; }
  void SetRootLayer(Layer* root_layer);

  // Called when we need the compositor to preserve the alpha channel in the
  // output for situations when we want to render transparently atop something
  // else, e.g. Aero glass.
  void SetHostHasTransparentBackground(bool host_has_transparent_background);

  // The scale factor of the device that this compositor is
  // compositing layers on.
  float device_scale_factor() const { return device_scale_factor_; }

  // Draws the scene created by the layer tree and any visual effects.
  void Draw();

  // Where possible, draws are scissored to a damage region calculated from
  // changes to layer properties.  This bypasses that and indicates that
  // the whole frame needs to be drawn.
  void ScheduleFullRedraw();

  // Schedule redraw and append damage_rect to the damage region calculated
  // from changes to layer properties.
  void ScheduleRedrawRect(const gfx::Rect& damage_rect);

  // Reads the region |bounds_in_pixel| of the contents of the last rendered
  // frame into the given bitmap.
  // Returns false if the pixels could not be read.
  bool ReadPixels(SkBitmap* bitmap, const gfx::Rect& bounds_in_pixel);

  // Sets the compositor's device scale factor and size.
  void SetScaleAndSize(float scale, const gfx::Size& size_in_pixel);

  // Returns the size of the widget that is being drawn to in pixel coordinates.
  const gfx::Size& size() const { return size_; }

  // Sets the background color used for areas that aren't covered by
  // the |root_layer|.
  void SetBackgroundColor(SkColor color);

  // Returns the widget for this compositor.
  gfx::AcceleratedWidget widget() const { return widget_; }

  // Compositor does not own observers. It is the responsibility of the
  // observer to remove itself when it is done observing.
  void AddObserver(CompositorObserver* observer);
  void RemoveObserver(CompositorObserver* observer);
  bool HasObserver(CompositorObserver* observer);

  // Creates a compositor lock. Returns NULL if it is not possible to lock at
  // this time (i.e. we're waiting to complete a previous unlock).
  scoped_refptr<CompositorLock> GetCompositorLock();

  // Internal functions, called back by command-buffer contexts on swap buffer
  // events.

  // Signals swap has been posted.
  void OnSwapBuffersPosted();

  // Signals swap has completed.
  void OnSwapBuffersComplete();

  // Signals swap has aborted (e.g. lost context).
  void OnSwapBuffersAborted();

  void OnUpdateVSyncParameters(base::TimeTicks timebase,
                               base::TimeDelta interval);

  // LayerTreeHostClient implementation.
  virtual void WillBeginFrame() OVERRIDE {}
  virtual void DidBeginFrame() OVERRIDE {}
  virtual void Animate(double frame_begin_time) OVERRIDE {}
  virtual void Layout() OVERRIDE;
  virtual void ApplyScrollAndScale(gfx::Vector2d scroll_delta,
                                   float page_scale) OVERRIDE {}
  virtual scoped_ptr<cc::OutputSurface>
      CreateOutputSurface() OVERRIDE;
  virtual void DidRecreateOutputSurface(bool success) OVERRIDE {}
  virtual scoped_ptr<cc::InputHandlerClient> CreateInputHandlerClient()
      OVERRIDE;
  virtual void WillCommit() OVERRIDE {}
  virtual void DidCommit() OVERRIDE;
  virtual void DidCommitAndDrawFrame() OVERRIDE;
  virtual void DidCompleteSwapBuffers() OVERRIDE;
  virtual void ScheduleComposite() OVERRIDE;
  virtual scoped_refptr<cc::ContextProvider>
      OffscreenContextProviderForMainThread() OVERRIDE;
  virtual scoped_refptr<cc::ContextProvider>
      OffscreenContextProviderForCompositorThread() OVERRIDE;

  int last_started_frame() { return last_started_frame_; }
  int last_ended_frame() { return last_ended_frame_; }

  bool IsLocked() { return compositor_lock_ != NULL; }

  const cc::LayerTreeDebugState& GetLayerTreeDebugState() const;
  void SetLayerTreeDebugState(const cc::LayerTreeDebugState& debug_state);

 private:
  friend class base::RefCounted<Compositor>;
  friend class CompositorLock;

  // Called by CompositorLock.
  void UnlockCompositor();

  // Called to release any pending CompositorLock
  void CancelCompositorLock();

  // Notifies the compositor that compositing is complete.
  void NotifyEnd();

  CompositorDelegate* delegate_;
  gfx::Size size_;

  // The root of the Layer tree drawn by this compositor.
  Layer* root_layer_;

  ObserverList<CompositorObserver> observer_list_;

  gfx::AcceleratedWidget widget_;
  scoped_refptr<cc::Layer> root_web_layer_;
  scoped_ptr<cc::LayerTreeHost> host_;

  // Used to verify that we have at most one draw swap in flight.
  scoped_ptr<PostedSwapQueue> posted_swaps_;

  // The device scale factor of the monitor that this compositor is compositing
  // layers on.
  float device_scale_factor_;

  int last_started_frame_;
  int last_ended_frame_;

  bool next_draw_is_resize_;

  bool disable_schedule_composite_;

  CompositorLock* compositor_lock_;

  DISALLOW_COPY_AND_ASSIGN(Compositor);
};

}  // namespace ui

#endif  // UI_COMPOSITOR_COMPOSITOR_H_
