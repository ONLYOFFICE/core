// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/keyboard/keyboard_controller.h"

#include "ui/aura/layout_manager.h"
#include "ui/aura/window.h"
#include "ui/aura/window_delegate.h"
#include "ui/base/hit_test.h"
#include "ui/base/ime/input_method.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/gfx/path.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/skia_util.h"
#include "ui/keyboard/keyboard_controller_observer.h"
#include "ui/keyboard/keyboard_controller_proxy.h"

namespace {

gfx::Rect KeyboardBoundsFromWindowBounds(const gfx::Rect& window_bounds) {
  const float kKeyboardHeightRatio = 0.3f;
  return gfx::Rect(
      window_bounds.x(),
      window_bounds.y() + window_bounds.height() * (1 - kKeyboardHeightRatio),
      window_bounds.width(),
      window_bounds.height() * kKeyboardHeightRatio);
}

// The KeyboardWindowDelegate makes sure the keyboard-window does not get focus.
// This is necessary to make sure that the synthetic key-events reach the target
// window.
// The delegate deletes itself when the window is destroyed.
class KeyboardWindowDelegate : public aura::WindowDelegate {
 public:
  KeyboardWindowDelegate() {}
  virtual ~KeyboardWindowDelegate() {}

 private:
  // Overridden from aura::WindowDelegate:
  virtual gfx::Size GetMinimumSize() const OVERRIDE { return gfx::Size(); }
  virtual gfx::Size GetMaximumSize() const OVERRIDE { return gfx::Size(); }
  virtual void OnBoundsChanged(const gfx::Rect& old_bounds,
                               const gfx::Rect& new_bounds) OVERRIDE {
    bounds_ = new_bounds;
  }
  virtual gfx::NativeCursor GetCursor(const gfx::Point& point) OVERRIDE {
    return gfx::kNullCursor;
  }
  virtual int GetNonClientComponent(const gfx::Point& point) const OVERRIDE {
    return HTNOWHERE;
  }
  virtual bool ShouldDescendIntoChildForEventHandling(
      aura::Window* child,
      const gfx::Point& location) OVERRIDE {
    return true;
  }
  virtual bool CanFocus() OVERRIDE { return false; }
  virtual void OnCaptureLost() OVERRIDE {}
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE {}
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE {}
  virtual void OnWindowDestroying() OVERRIDE {}
  virtual void OnWindowDestroyed() OVERRIDE { delete this; }
  virtual void OnWindowTargetVisibilityChanged(bool visible) OVERRIDE {}
  virtual bool HasHitTestMask() const OVERRIDE { return true; }
  virtual void GetHitTestMask(gfx::Path* mask) const OVERRIDE {
    gfx::Rect keyboard_bounds = KeyboardBoundsFromWindowBounds(bounds_);
    mask->addRect(RectToSkRect(keyboard_bounds));
  }
  virtual scoped_refptr<ui::Texture> CopyTexture() OVERRIDE { return NULL; }

  gfx::Rect bounds_;
  DISALLOW_COPY_AND_ASSIGN(KeyboardWindowDelegate);
};

}  // namespace

namespace keyboard {

// LayoutManager for the virtual keyboard container.  Manages a single window
// (the virtual keyboard) and keeps it positioned at the bottom of the
// owner window.
class KeyboardLayoutManager : public aura::LayoutManager {
 public:
  KeyboardLayoutManager(aura::Window* container)
      : container_(container), keyboard_(NULL) {
    CHECK(container_);
  }

  // Overridden from aura::LayoutManager
  virtual void OnWindowResized() OVERRIDE {
    if (!keyboard_)
      return;
    SetChildBoundsDirect(keyboard_,
                         KeyboardBoundsFromWindowBounds(container_->bounds()));
  }
  virtual void OnWindowAddedToLayout(aura::Window* child) OVERRIDE {
    DCHECK(!keyboard_);
    keyboard_ = child;
  }
  virtual void OnWillRemoveWindowFromLayout(aura::Window* child) OVERRIDE {}
  virtual void OnWindowRemovedFromLayout(aura::Window* child) OVERRIDE {}
  virtual void OnChildWindowVisibilityChanged(aura::Window* child,
                                              bool visible) OVERRIDE {}
  virtual void SetChildBounds(aura::Window* child,
                              const gfx::Rect& requested_bounds) OVERRIDE {
    // Drop these: the size should only be set in OnWindowResized.
  }

 private:
  aura::Window* container_;
  aura::Window* keyboard_;

  DISALLOW_COPY_AND_ASSIGN(KeyboardLayoutManager);
};

KeyboardController::KeyboardController(KeyboardControllerProxy* proxy)
    : proxy_(proxy),
      container_(NULL),
      input_method_(NULL) {
  CHECK(proxy);
  input_method_ = proxy_->GetInputMethod();
  input_method_->AddObserver(this);
}

KeyboardController::~KeyboardController() {
  if (container_)
    container_->RemoveObserver(this);
  if (input_method_)
    input_method_->RemoveObserver(this);
}

aura::Window* KeyboardController::GetContainerWindow() {
  if (!container_) {
    container_ = new aura::Window(new KeyboardWindowDelegate());
    container_->SetName("KeyboardContainer");
    container_->Init(ui::LAYER_NOT_DRAWN);
    container_->AddObserver(this);
    container_->SetLayoutManager(new KeyboardLayoutManager(container_));
  }
  return container_;
}

void KeyboardController::AddObserver(KeyboardControllerObserver* observer) {
  observer_list_.AddObserver(observer);
}

void KeyboardController::RemoveObserver(KeyboardControllerObserver* observer) {
  observer_list_.RemoveObserver(observer);
}

void KeyboardController::OnWindowHierarchyChanged(
    const HierarchyChangeParams& params) {
  if (params.new_parent && params.target == container_)
    OnTextInputStateChanged(proxy_->GetInputMethod()->GetTextInputClient());
}

void KeyboardController::OnWindowDestroying(aura::Window* window) {
  DCHECK_EQ(container_, window);
  container_ = NULL;
}

void KeyboardController::OnTextInputStateChanged(
    const ui::TextInputClient* client) {
  if (!container_)
    return;

  bool was_showing = container_->IsVisible();
  bool should_show = was_showing;
  if (!client || client->GetTextInputType() == ui::TEXT_INPUT_TYPE_NONE) {
    should_show = false;
  } else {
    if (container_->children().empty()) {
      aura::Window* keyboard = proxy_->GetKeyboardWindow();
      keyboard->Show();
      container_->AddChild(keyboard);
      container_->layout_manager()->OnWindowResized();
    }
    container_->parent()->StackChildAtTop(container_);
    should_show = true;
  }

  if (was_showing != should_show) {
    gfx::Rect new_bounds(
        should_show ? container_->children()[0]->bounds() : gfx::Rect());

    FOR_EACH_OBSERVER(
        KeyboardControllerObserver,
        observer_list_,
        OnKeyboardBoundsChanging(new_bounds));

    if (should_show)
      container_->Show();
    else
      container_->Hide();

    proxy_->OnKeyboardBoundsChanged(new_bounds);
  }

  // TODO(bryeung): whenever the TextInputClient changes we need to notify the
  // keyboard (with the TextInputType) so that it can reset it's state (e.g.
  // abandon compositions in progress)
}

void KeyboardController::OnInputMethodDestroyed(
    const ui::InputMethod* input_method) {
  DCHECK_EQ(input_method_, input_method);
  input_method_ = NULL;
}

}  // namespace keyboard
