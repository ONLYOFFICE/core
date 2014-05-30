// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/menu/menu_controller.h"

#include "base/run_loop.h"
#include "ui/aura/client/activation_change_observer.h"
#include "ui/aura/client/activation_client.h"
#include "ui/aura/client/dispatcher_client.h"
#include "ui/aura/client/drag_drop_client.h"
#include "ui/aura/root_window.h"
#include "ui/aura/window_observer.h"
#include "ui/gfx/screen.h"
#include "ui/views/widget/widget.h"

namespace views {

namespace {

// ActivationChangeObserverImpl is used to observe activation changes and close
// the menu. Additionally it listens for the root window to be destroyed and
// cancel the menu as well.
class ActivationChangeObserverImpl
    : public aura::client::ActivationChangeObserver,
      public aura::WindowObserver,
      public ui::EventHandler {
 public:
  ActivationChangeObserverImpl(MenuController* controller,
                               aura::RootWindow* root)
      : controller_(controller),
        root_(root) {
    aura::client::GetActivationClient(root_)->AddObserver(this);
    root_->AddObserver(this);
    root_->AddPreTargetHandler(this);
  }

  virtual ~ActivationChangeObserverImpl() {
    Cleanup();
  }

  // aura::client::ActivationChangeObserver overrides:
  virtual void OnWindowActivated(aura::Window* gained_active,
                                 aura::Window* lost_active) OVERRIDE {
    if (!controller_->drag_in_progress())
      controller_->CancelAll();
  }

  // aura::WindowObserver overrides:
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE {
    Cleanup();
  }

  // ui::EventHandler overrides:
  virtual void OnCancelMode(ui::CancelModeEvent* event) OVERRIDE {
    controller_->CancelAll();
  }

 private:
  void Cleanup() {
    if (!root_)
      return;
    // The ActivationClient may have been destroyed by the time we get here.
    aura::client::ActivationClient* client =
        aura::client::GetActivationClient(root_);
    if (client)
      client->RemoveObserver(this);
    root_->RemovePreTargetHandler(this);
    root_->RemoveObserver(this);
    root_ = NULL;
  }

  MenuController* controller_;
  aura::RootWindow* root_;

  DISALLOW_COPY_AND_ASSIGN(ActivationChangeObserverImpl);
};

aura::RootWindow* GetOwnerRootWindow(views::Widget* owner) {
  return owner ? owner->GetNativeWindow()->GetRootWindow() : NULL;
}

}  // namespace

void MenuController::RunMessageLoop(bool nested_menu) {
  // |owner_| may be NULL.
  aura::RootWindow* root = GetOwnerRootWindow(owner_);
  if (root) {
    scoped_ptr<ActivationChangeObserverImpl> observer;
    if (!nested_menu)
      observer.reset(new ActivationChangeObserverImpl(this, root));
    aura::client::GetDispatcherClient(root)->
        RunWithDispatcher(this, owner_->GetNativeWindow(), true);
  } else {
    base::MessageLoopForUI* loop = base::MessageLoopForUI::current();
    base::MessageLoop::ScopedNestableTaskAllower allow(loop);
    base::RunLoop run_loop(this);
    run_loop.Run();
  }
}

bool MenuController::ShouldQuitNow() const {
  aura::RootWindow* root = GetOwnerRootWindow(owner_);
  return !aura::client::GetDragDropClient(root) ||
      !aura::client::GetDragDropClient(root)->IsDragDropInProgress();
}

gfx::Screen* MenuController::GetScreen() {
  aura::RootWindow* root = GetOwnerRootWindow(owner_);
  return root ?
      gfx::Screen::GetScreenFor(root) : gfx::Screen::GetNativeScreen();
}


}  // namespace views
