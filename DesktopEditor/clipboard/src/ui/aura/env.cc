// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/env.h"

#include "base/command_line.h"
#include "ui/aura/env_observer.h"
#include "ui/aura/root_window_host.h"
#include "ui/aura/window.h"
#include "ui/compositor/compositor.h"
#include "ui/compositor/compositor_switches.h"

#if defined(USE_X11)
#include "base/message_pump_aurax11.h"
#endif

namespace aura {

// static
Env* Env::instance_ = NULL;

////////////////////////////////////////////////////////////////////////////////
// Env, public:

Env::Env()
    : mouse_button_flags_(0),
      is_touch_down_(false),
      render_white_bg_(true) {
}

Env::~Env() {
#if defined(USE_X11)
  base::MessagePumpAuraX11::Current()->RemoveObserver(
      &device_list_updater_aurax11_);
#endif

  FOR_EACH_OBSERVER(EnvObserver, observers_, OnWillDestroyEnv());

  ui::Compositor::Terminate();
}

// static
Env* Env::GetInstance() {
  if (!instance_) {
    instance_ = new Env;
    instance_->Init();
  }
  return instance_;
}

// static
void Env::DeleteInstance() {
  delete instance_;
  instance_ = NULL;
}

void Env::AddObserver(EnvObserver* observer) {
  observers_.AddObserver(observer);
}

void Env::RemoveObserver(EnvObserver* observer) {
  observers_.RemoveObserver(observer);
}

#if !defined(OS_MACOSX)
MessageLoop::Dispatcher* Env::GetDispatcher() {
#if defined(USE_X11)
  return base::MessagePumpAuraX11::Current();
#else
  return dispatcher_.get();
#endif
}
#endif

void Env::RootWindowActivated(RootWindow* root_window) {
  FOR_EACH_OBSERVER(EnvObserver, observers_,
                    OnRootWindowActivated(root_window));
}

////////////////////////////////////////////////////////////////////////////////
// Env, private:

void Env::Init() {
#if !defined(USE_X11) && !defined(USE_MESSAGEPUMP_LINUX)
  dispatcher_.reset(CreateDispatcher());
#endif
#if defined(USE_X11)
  // We can't do this with a root window listener because XI_HierarchyChanged
  // messages don't have a target window.
  base::MessagePumpAuraX11::Current()->AddObserver(
      &device_list_updater_aurax11_);
#endif
  ui::Compositor::Initialize();
}

void Env::NotifyWindowInitialized(Window* window) {
  FOR_EACH_OBSERVER(EnvObserver, observers_, OnWindowInitialized(window));
}

void Env::NotifyRootWindowInitialized(RootWindow* root_window) {
  FOR_EACH_OBSERVER(EnvObserver,
                    observers_,
                    OnRootWindowInitialized(root_window));
}

////////////////////////////////////////////////////////////////////////////////
// Env, ui::EventTarget implementation:

bool Env::CanAcceptEvent(const ui::Event& event) {
  return true;
}

ui::EventTarget* Env::GetParentTarget() {
  return NULL;
}

}  // namespace aura
