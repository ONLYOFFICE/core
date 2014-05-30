// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_KEYBOARD_KEYBOARD_UI_CONTROLLER_H_
#define UI_KEYBOARD_KEYBOARD_UI_CONTROLLER_H_

#include "content/public/browser/web_ui_controller.h"
#include "ui/keyboard/keyboard_export.h"

namespace content {
class WebUI;
};

namespace keyboard {

// WebUIController for chrome://keyboard/.
class KEYBOARD_EXPORT KeyboardUIController : public content::WebUIController {
 public:
  explicit KeyboardUIController(content::WebUI* web_ui);
  virtual ~KeyboardUIController();

 private:
  DISALLOW_COPY_AND_ASSIGN(KeyboardUIController);
};

}  // namespace keyboard

#endif  // UI_KEYBOARD_KEYBOARD_UI_CONTROLLER_H_
