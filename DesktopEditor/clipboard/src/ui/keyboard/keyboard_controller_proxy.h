// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_KEYBOARD_KEYBOARD_CONTROLLER_PROXY_H_
#define UI_KEYBOARD_KEYBOARD_CONTROLLER_PROXY_H_

#include "base/memory/scoped_ptr.h"
#include "ui/keyboard/keyboard_export.h"

namespace aura {
class Window;
}
namespace content {
class BrowserContext;
class SiteInstance;
class WebContents;
}
namespace gfx {
class Rect;
}
namespace ui {
class InputMethod;
}

namespace keyboard {

// A proxy used by the KeyboardController to get access to the virtual
// keyboard window.
class KEYBOARD_EXPORT KeyboardControllerProxy {
 public:
  KeyboardControllerProxy();
  virtual ~KeyboardControllerProxy();

  // Gets the virtual keyboard window.  Ownership of the returned Window remains
  // with the proxy.
  virtual aura::Window* GetKeyboardWindow();

  // Gets the InputMethod that will provide notifications about changes in the
  // text input context.
  virtual ui::InputMethod* GetInputMethod() = 0;

  // Called after the keyboard bounds or visibility have changed, and after all
  // KeyboardController::Observer's have been notified.
  virtual void OnKeyboardBoundsChanged(const gfx::Rect& new_bounds) = 0;

 protected:
  // Gets the BrowserContext to use for creating the WebContents hosting the
  // keyboard.
  virtual content::BrowserContext* GetBrowserContext() = 0;

  // The implementation can choose to setup the WebContents before the virtual
  // keyboard page is loaded (e.g. install a WebContentsObserver).
  // SetupWebContents() is called right after creating the WebContents, before
  // loading the keyboard page.
  virtual void SetupWebContents(content::WebContents* contents);

 private:
  scoped_ptr<content::WebContents> keyboard_contents_;

  DISALLOW_COPY_AND_ASSIGN(KeyboardControllerProxy);
};

}  // namespace keyboard

#endif  // UI_KEYBOARD_KEYBOARD_CONTROLLER_PROXY_H_
