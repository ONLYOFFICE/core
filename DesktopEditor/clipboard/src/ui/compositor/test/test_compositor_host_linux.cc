// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/compositor/test/test_compositor_host.h"

#include <X11/Xlib.h>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop.h"
#include "ui/base/x/x11_util.h"
#include "ui/compositor/compositor.h"
#include "ui/gfx/rect.h"

namespace ui {

class TestCompositorHostLinux : public TestCompositorHost,
                                public CompositorDelegate {
 public:
  TestCompositorHostLinux(const gfx::Rect& bounds);
  virtual ~TestCompositorHostLinux();

 private:
  // Overridden from TestCompositorHost:
  virtual void Show() OVERRIDE;
  virtual ui::Compositor* GetCompositor() OVERRIDE;

  // Overridden from CompositorDelegate:
  virtual void ScheduleDraw() OVERRIDE;

  void Draw();

  gfx::Rect bounds_;

  scoped_ptr<ui::Compositor> compositor_;

  XID window_;

  base::WeakPtrFactory<TestCompositorHostLinux> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(TestCompositorHostLinux);
};

TestCompositorHostLinux::TestCompositorHostLinux(const gfx::Rect& bounds)
    : bounds_(bounds),
      method_factory_(this) {
}

TestCompositorHostLinux::~TestCompositorHostLinux() {
}

void TestCompositorHostLinux::Show() {
  Display* display = GetXDisplay();
  XSetWindowAttributes swa;
  swa.event_mask = StructureNotifyMask | ExposureMask;
  swa.override_redirect = True;
  window_ = XCreateWindow(
      display,
      RootWindow(display, DefaultScreen(display)),  // parent
      bounds_.x(), bounds_.y(), bounds_.width(), bounds_.height(),
      0,  // border width
      CopyFromParent,  // depth
      InputOutput,
      CopyFromParent,  // visual
      CWEventMask | CWOverrideRedirect, &swa);
  XMapWindow(display, window_);

  while (1) {
    XEvent event;
    XNextEvent(display, &event);
    if (event.type == MapNotify && event.xmap.window == window_)
      break;
  }
  compositor_.reset(new ui::Compositor(this, window_));
  compositor_->SetScaleAndSize(1.0f, bounds_.size());
}

ui::Compositor* TestCompositorHostLinux::GetCompositor() {
  return compositor_.get();
}

void TestCompositorHostLinux::ScheduleDraw() {
  DCHECK(!ui::Compositor::WasInitializedWithThread());
  if (!method_factory_.HasWeakPtrs()) {
    base::MessageLoopForUI::current()->PostTask(
        FROM_HERE,
        base::Bind(&TestCompositorHostLinux::Draw,
                   method_factory_.GetWeakPtr()));
  }
}

void TestCompositorHostLinux::Draw() {
  if (compositor_.get())
    compositor_->Draw();
}

// static
TestCompositorHost* TestCompositorHost::Create(const gfx::Rect& bounds) {
  return new TestCompositorHostLinux(bounds);
}

}  // namespace ui
