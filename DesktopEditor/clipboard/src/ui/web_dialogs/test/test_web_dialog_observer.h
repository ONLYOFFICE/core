// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_WEB_DIALOGS_TEST_TEST_WEB_DIALOG_OBSERVER_H_
#define UI_WEB_DIALOGS_TEST_TEST_WEB_DIALOG_OBSERVER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/web_dialogs/web_dialog_observer.h"


namespace content {
class JsInjectionReadyObserver;
class MessageLoopRunner;
class RenderViewHost;
class WebUI;
}

namespace ui {
namespace test {

// For browser_tests, which run on the UI thread, run a second message
// MessageLoop to detect WebDialog creation and quit when the constructed
// WebUI instance is captured and ready.
class TestWebDialogObserver : public content::NotificationObserver,
                              public WebDialogObserver {
 public:
  // Create and register a new TestWebDialogObserver. If
  // |js_injection_ready_observer| is non-NULL, notify it as soon as the RVH is
  // available.
  explicit TestWebDialogObserver(
      content::JsInjectionReadyObserver* js_injection_ready_observer);
  virtual ~TestWebDialogObserver();

  // Overridden from WebDialogObserver:
  virtual void OnDialogShown(
      content::WebUI* webui,
      content::RenderViewHost* render_view_host) OVERRIDE;

  // Waits for an WebDialog to be created. The WebUI instance is captured
  // and the method returns it when the navigation on the dialog is complete.
  content::WebUI* GetWebUI();

 private:
  // content::NotificationObserver:
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;

  // Observer to take some action when the dialog is ready for JavaScript
  // injection.
  content::JsInjectionReadyObserver* js_injection_ready_observer_;
  content::WebUI* web_ui_;
  bool done_;
  bool running_;
  scoped_refptr<content::MessageLoopRunner> message_loop_runner_;

  DISALLOW_COPY_AND_ASSIGN(TestWebDialogObserver);
};

}  // namespace test
}  // namespace ui

#endif  // UI_WEB_DIALOGS_TEST_TEST_WEB_DIALOG_OBSERVER_H_
