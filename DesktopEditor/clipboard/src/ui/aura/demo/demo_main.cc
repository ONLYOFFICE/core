// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/i18n/icu_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "third_party/skia/include/core/SkXfermode.h"
#include "ui/aura/client/default_capture_client.h"
#include "ui/aura/client/stacking_client.h"
#include "ui/aura/env.h"
#include "ui/aura/root_window.h"
#include "ui/aura/test/test_screen.h"
#include "ui/aura/window.h"
#include "ui/aura/window_delegate.h"
#include "ui/base/events/event.h"
#include "ui/base/hit_test.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/ui_base_paths.h"
#include "ui/compositor/test/compositor_test_support.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/rect.h"

#if defined(USE_X11)
#include "base/message_pump_aurax11.h"
#endif

namespace {

// Trivial WindowDelegate implementation that draws a colored background.
class DemoWindowDelegate : public aura::WindowDelegate {
 public:
  explicit DemoWindowDelegate(SkColor color) : color_(color) {}

  // Overridden from WindowDelegate:
  virtual gfx::Size GetMinimumSize() const OVERRIDE {
    return gfx::Size();
  }

  virtual gfx::Size GetMaximumSize() const OVERRIDE {
    return gfx::Size();
  }

  virtual void OnBoundsChanged(const gfx::Rect& old_bounds,
                               const gfx::Rect& new_bounds) OVERRIDE {}
  virtual gfx::NativeCursor GetCursor(const gfx::Point& point) OVERRIDE {
    return gfx::kNullCursor;
  }
  virtual int GetNonClientComponent(const gfx::Point& point) const OVERRIDE {
    return HTCAPTION;
  }
  virtual bool ShouldDescendIntoChildForEventHandling(
      aura::Window* child,
      const gfx::Point& location) OVERRIDE {
    return true;
  }
  virtual bool CanFocus() OVERRIDE { return true; }
  virtual void OnCaptureLost() OVERRIDE {}
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE {
    canvas->DrawColor(color_, SkXfermode::kSrc_Mode);
  }
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE {}
  virtual void OnWindowDestroying() OVERRIDE {}
  virtual void OnWindowDestroyed() OVERRIDE {}
  virtual void OnWindowTargetVisibilityChanged(bool visible) OVERRIDE {}
  virtual bool HasHitTestMask() const OVERRIDE { return false; }
  virtual void GetHitTestMask(gfx::Path* mask) const OVERRIDE {}
  virtual scoped_refptr<ui::Texture> CopyTexture() OVERRIDE {
    return scoped_refptr<ui::Texture>();
  }

 private:
  SkColor color_;

  DISALLOW_COPY_AND_ASSIGN(DemoWindowDelegate);
};

class DemoStackingClient : public aura::client::StackingClient {
 public:
  explicit DemoStackingClient(aura::RootWindow* root_window)
      : root_window_(root_window) {
    aura::client::SetStackingClient(root_window_, this);
  }

  virtual ~DemoStackingClient() {
    aura::client::SetStackingClient(root_window_, NULL);
  }

  // Overridden from aura::client::StackingClient:
  virtual aura::Window* GetDefaultParent(aura::Window* context,
                                         aura::Window* window,
                                         const gfx::Rect& bounds) OVERRIDE {
    if (!capture_client_) {
      capture_client_.reset(
          new aura::client::DefaultCaptureClient(root_window_));
    }
    return root_window_;
  }

 private:
  aura::RootWindow* root_window_;

  scoped_ptr<aura::client::DefaultCaptureClient> capture_client_;

  DISALLOW_COPY_AND_ASSIGN(DemoStackingClient);
};

int DemoMain() {
  // Create the message-loop here before creating the root window.
  base::MessageLoop message_loop(base::MessageLoop::TYPE_UI);
  ui::CompositorTestSupport::Initialize();
  aura::Env::GetInstance();
  scoped_ptr<aura::TestScreen> test_screen(aura::TestScreen::Create());
  gfx::Screen::SetScreenInstance(gfx::SCREEN_TYPE_NATIVE, test_screen.get());
  scoped_ptr<aura::RootWindow> root_window(
      test_screen->CreateRootWindowForPrimaryDisplay());
  scoped_ptr<DemoStackingClient> stacking_client(new DemoStackingClient(
      root_window.get()));

  // Create a hierarchy of test windows.
  DemoWindowDelegate window_delegate1(SK_ColorBLUE);
  aura::Window window1(&window_delegate1);
  window1.set_id(1);
  window1.Init(ui::LAYER_TEXTURED);
  window1.SetBounds(gfx::Rect(100, 100, 400, 400));
  window1.Show();
  window1.SetDefaultParentByRootWindow(root_window.get(), gfx::Rect());

  DemoWindowDelegate window_delegate2(SK_ColorRED);
  aura::Window window2(&window_delegate2);
  window2.set_id(2);
  window2.Init(ui::LAYER_TEXTURED);
  window2.SetBounds(gfx::Rect(200, 200, 350, 350));
  window2.Show();
  window2.SetDefaultParentByRootWindow(root_window.get(), gfx::Rect());

  DemoWindowDelegate window_delegate3(SK_ColorGREEN);
  aura::Window window3(&window_delegate3);
  window3.set_id(3);
  window3.Init(ui::LAYER_TEXTURED);
  window3.SetBounds(gfx::Rect(10, 10, 50, 50));
  window3.Show();
  window2.AddChild(&window3);

  root_window->ShowRootWindow();
  base::MessageLoopForUI::current()->Run();

  ui::CompositorTestSupport::Terminate();

  return 0;
}

}  // namespace

int main(int argc, char** argv) {
  CommandLine::Init(argc, argv);

  // The exit manager is in charge of calling the dtors of singleton objects.
  base::AtExitManager exit_manager;

  ui::RegisterPathProvider();
  icu_util::Initialize();
  ResourceBundle::InitSharedInstanceWithLocale("en-US", NULL);

  return DemoMain();
}
