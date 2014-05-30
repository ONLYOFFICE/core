// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/keyboard/keyboard_ui_handler.h"

#include <string>

#include "base/bind.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_view.h"
#include "content/public/browser/web_ui.h"
#include "ui/aura/root_window.h"
#include "ui/base/events/event.h"
#include "ui/keyboard/keyboard_util.h"

namespace keyboard {

KeyboardUIHandler::KeyboardUIHandler() {
}

KeyboardUIHandler::~KeyboardUIHandler() {
}

void KeyboardUIHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "sendKeyEvent",
      base::Bind(&KeyboardUIHandler::HandleSendKeyEventMessage,
                 base::Unretained(this)));
}

void KeyboardUIHandler::HandleSendKeyEventMessage(const base::ListValue* args) {
  std::string error;
  scoped_ptr<ui::KeyEvent> event(keyboard::KeyEventFromArgs(args, &error));
  if (!event) {
    LOG(ERROR) << "sendKeyEvent failed: " << error;
    return;
  }

  aura::RootWindow* root_window =
      web_ui()->GetWebContents()->GetView()->GetNativeView()->GetRootWindow();
  root_window->AsRootWindowHostDelegate()->OnHostKeyEvent(event.get());
}

}  // namespace keyboard
