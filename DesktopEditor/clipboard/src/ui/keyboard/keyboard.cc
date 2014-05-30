// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/keyboard/keyboard.h"

#include "base/files/file_path.h"
#include "base/memory/singleton.h"
#include "base/path_service.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/web_ui_controller_factory.h"
#include "googleurl/src/gurl.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/keyboard/keyboard_constants.h"
#include "ui/keyboard/keyboard_ui_controller.h"

namespace {

using content::BrowserContext;
using content::WebUI;
using content::WebUIController;

class KeyboardWebUIControllerFactory : public content::WebUIControllerFactory {
 public:
  // |WebUIControllerFactory| implementation:
  virtual content::WebUI::TypeID GetWebUIType(
      content::BrowserContext* browser_context,
      const GURL& url) const OVERRIDE {
    if (url == GURL(keyboard::kKeyboardWebUIURL))
      return const_cast<KeyboardWebUIControllerFactory*>(this);

    return WebUI::kNoWebUI;
  }
  virtual bool UseWebUIForURL(content::BrowserContext* browser_context,
                              const GURL& url) const OVERRIDE {
    return GetWebUIType(browser_context, url) != WebUI::kNoWebUI;
  }
  virtual bool UseWebUIBindingsForURL(content::BrowserContext* browser_context,
                                      const GURL& url) const OVERRIDE {
    return UseWebUIForURL(browser_context, url);
  }
  virtual content::WebUIController* CreateWebUIControllerForURL(
      content::WebUI* web_ui,
      const GURL& url) const OVERRIDE {
    if (url == GURL(keyboard::kKeyboardWebUIURL))
      return new keyboard::KeyboardUIController(web_ui);
    return NULL;
  }

  static KeyboardWebUIControllerFactory* GetInstance() {
    return Singleton<KeyboardWebUIControllerFactory>::get();
  }

 protected:
  KeyboardWebUIControllerFactory() {}
  virtual ~KeyboardWebUIControllerFactory() {}

 private:
  friend struct DefaultSingletonTraits<KeyboardWebUIControllerFactory>;

  DISALLOW_COPY_AND_ASSIGN(KeyboardWebUIControllerFactory);
};

}  // namespace

namespace keyboard {

void InitializeKeyboard() {
  base::FilePath pak_dir;
  PathService::Get(base::DIR_MODULE, &pak_dir);
  base::FilePath pak_file = pak_dir.Append(
      FILE_PATH_LITERAL("keyboard_resources.pak"));
  ui::ResourceBundle::GetSharedInstance().AddDataPackFromPath(
      pak_file, ui::SCALE_FACTOR_100P);

  content::WebUIControllerFactory::RegisterFactory(
      KeyboardWebUIControllerFactory::GetInstance());
}

}  // namespace keyboard
