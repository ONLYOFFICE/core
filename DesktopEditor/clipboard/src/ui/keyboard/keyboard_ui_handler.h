// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_KEYBOARD_KEYBOARD_UI_HANDLER_H_
#define UI_KEYBOARD_KEYBOARD_UI_HANDLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "content/public/browser/web_ui_message_handler.h"

namespace keyboard {

// The WebUI handler for chrome://keyboard.  Delivers events to the
// aura::RootWindow associated with the WebUI.
class KeyboardUIHandler : public content::WebUIMessageHandler {
 public:
  KeyboardUIHandler();
  virtual ~KeyboardUIHandler();

 private:
  // |content::WebUIMessageHandler| implementation:
  virtual void RegisterMessages() OVERRIDE;

  // Callback for the "sendKeyEvent" message.
  void HandleSendKeyEventMessage(const base::ListValue* args);

  DISALLOW_COPY_AND_ASSIGN(KeyboardUIHandler);
};

}  // namespace keyboard

#endif  // UI_KEYBOARD_KEYBOARD_UI_HANDLER_H_
