// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_KEYBOARD_KEYBOARD_CONTROLLER_H_
#define UI_KEYBOARD_KEYBOARD_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "ui/aura/window_observer.h"
#include "ui/base/ime/input_method_observer.h"
#include "ui/keyboard/keyboard_export.h"

namespace aura {
class Window;
}
namespace gfx {
class Rect;
}
namespace ui {
class InputMethod;
class TextInputClient;
}

namespace keyboard {

class KeyboardControllerObserver;
class KeyboardControllerProxy;
class KeyboardLayoutManager;

// Provides control of the virtual keyboard, including providing a container
// and controlling visibility.
class KEYBOARD_EXPORT KeyboardController : public ui::InputMethodObserver,
                                           public aura::WindowObserver {
 public:
  // Takes ownership of |proxy|.
  explicit KeyboardController(KeyboardControllerProxy* proxy);
  virtual ~KeyboardController();

  // Returns the container for the keyboard, which is then owned by the caller.
  // It is the responsibility of the caller to Show() the returned window.
  aura::Window* GetContainerWindow();

  // Management of the observer list.
  virtual void AddObserver(KeyboardControllerObserver* observer);
  virtual void RemoveObserver(KeyboardControllerObserver* observer);

 private:
  // For access to Observer methods for simulation.
  friend class KeyboardControllerTest;

  // aura::WindowObserver overrides
  virtual void OnWindowHierarchyChanged(
      const HierarchyChangeParams& params) OVERRIDE;
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE;

  // InputMethodObserver overrides
  virtual void OnTextInputStateChanged(
      const ui::TextInputClient* client) OVERRIDE;
  virtual void OnInputMethodDestroyed(
      const ui::InputMethod* input_method) OVERRIDE;

  scoped_ptr<KeyboardControllerProxy> proxy_;
  aura::Window* container_;
  ui::InputMethod* input_method_;

  ObserverList<KeyboardControllerObserver> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(KeyboardController);
};

}  // namespace keyboard

#endif  // UI_KEYBOARD_KEYBOARD_CONTROLLER_H_
