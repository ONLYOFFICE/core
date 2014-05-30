// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/keyboard/keyboard_controller_proxy.h"

#include "content/public/browser/site_instance.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_view.h"
#include "ui/keyboard/keyboard_constants.h"

namespace {

// The WebContentsDelegate for the keyboard.
// The delegate deletes itself when the keyboard is destroyed.
class KeyboardContentsDelegate : public content::WebContentsDelegate,
                                 public content::WebContentsObserver {
 public:
  KeyboardContentsDelegate() {}
  virtual ~KeyboardContentsDelegate() {}

 private:
  // Overridden from content::WebContentsDelegate:
  virtual content::WebContents* OpenURLFromTab(
      content::WebContents* source,
      const content::OpenURLParams& params) OVERRIDE {
    source->GetController().LoadURL(
        params.url, params.referrer, params.transition, params.extra_headers);
    Observe(source);
    return source;
  }

  // Overridden from content::WebContentsObserver:
  virtual void WebContentsDestroyed(content::WebContents* contents) OVERRIDE {
    delete this;
  }

  DISALLOW_COPY_AND_ASSIGN(KeyboardContentsDelegate);
};

}  // namespace

namespace keyboard {

KeyboardControllerProxy::KeyboardControllerProxy() {
}

KeyboardControllerProxy::~KeyboardControllerProxy() {
}

aura::Window* KeyboardControllerProxy::GetKeyboardWindow() {
  if (!keyboard_contents_) {
    content::BrowserContext* context = GetBrowserContext();
    GURL url(kKeyboardWebUIURL);
    keyboard_contents_.reset(content::WebContents::Create(
        content::WebContents::CreateParams(context,
            content::SiteInstance::CreateForURL(context, url))));
    keyboard_contents_->SetDelegate(new KeyboardContentsDelegate);
    SetupWebContents(keyboard_contents_.get());

    content::OpenURLParams params(url,
                                  content::Referrer(),
                                  SINGLETON_TAB,
                                  content::PAGE_TRANSITION_AUTO_TOPLEVEL,
                                  false);
    keyboard_contents_->OpenURL(params);
  }

  return keyboard_contents_->GetView()->GetNativeView();
}

void KeyboardControllerProxy::SetupWebContents(content::WebContents* contents) {
}

}  // namespace keyboard
