// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/keyboard/keyboard_ui_controller.h"

#include "content/public/browser/browser_context.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"
#include "grit/keyboard_resources.h"
#include "ui/keyboard/keyboard_constants.h"
#include "ui/keyboard/keyboard_ui_handler.h"

namespace {

content::WebUIDataSource* CreateKeyboardUIDataSource() {
  content::WebUIDataSource* source =
      content::WebUIDataSource::Create(keyboard::kKeyboardWebUIHost);

  source->SetDefaultResource(IDR_KEYBOARD_INDEX);

  source->AddResourcePath("common.js", IDR_KEYBOARD_COMMON_JS);
  source->AddResourcePath("images/chevron.svg", IDR_KEYBOARD_IMAGES_CHEVRON);
  source->AddResourcePath("images/del.svg", IDR_KEYBOARD_IMAGES_DEL);
  source->AddResourcePath("images/keyboard.svg", IDR_KEYBOARD_IMAGES_KEYBOARD);
  source->AddResourcePath("images/mic.svg", IDR_KEYBOARD_IMAGES_MIC);
  source->AddResourcePath("images/ret.svg", IDR_KEYBOARD_IMAGES_RET);
  source->AddResourcePath("images/shift.svg", IDR_KEYBOARD_IMAGES_SHIFT);
  source->AddResourcePath("images/shift_down.svg",
      IDR_KEYBOARD_IMAGES_SHIFT_DOWN);
  source->AddResourcePath("images/tab.svg", IDR_KEYBOARD_IMAGES_TAB);
  source->AddResourcePath("main.js", IDR_KEYBOARD_MAIN_JS);

  // These files are specific to the WebUI version
  source->AddResourcePath("layout_us.js", IDR_KEYBOARD_WEBUI_LAYOUT_US_JS);
  source->AddResourcePath("main.css", IDR_KEYBOARD_WEBUI_MAIN_CSS);
  source->AddResourcePath("api_adapter.js", IDR_KEYBOARD_WEBUI_API_ADAPTER_JS);

  return source;
}

}  // namespace

namespace keyboard {

KeyboardUIController::KeyboardUIController(content::WebUI* web_ui)
    : WebUIController(web_ui) {
  content::BrowserContext* browser_context =
      web_ui->GetWebContents()->GetBrowserContext();
  web_ui->AddMessageHandler(new KeyboardUIHandler());
  content::WebUIDataSource::Add(
      browser_context,
      CreateKeyboardUIDataSource());
}

KeyboardUIController::~KeyboardUIController() {}

}  // namespace keyboard
