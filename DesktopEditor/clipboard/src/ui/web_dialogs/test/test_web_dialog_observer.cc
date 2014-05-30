// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/web_dialogs/test/test_web_dialog_observer.h"

#include "base/logging.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/notification_details.h"
#include "content/public/browser/notification_service.h"
#include "content/public/browser/notification_source.h"
#include "content/public/browser/notification_types.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"
#include "content/public/test/js_injection_ready_observer.h"
#include "content/public/test/test_utils.h"

using content::NavigationController;

namespace ui {
namespace test {

TestWebDialogObserver::TestWebDialogObserver(
    content::JsInjectionReadyObserver* js_injection_ready_observer)
    : js_injection_ready_observer_(js_injection_ready_observer),
      web_ui_(NULL),
      done_(false),
      running_(false) {
}

TestWebDialogObserver::~TestWebDialogObserver() {
}

void TestWebDialogObserver::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  switch (type) {
      break;
    case content::NOTIFICATION_LOAD_STOP:
      DCHECK(web_ui_);
      registrar_.Remove(this, content::NOTIFICATION_LOAD_STOP,
                        content::Source<NavigationController>(
                            &web_ui_->GetWebContents()->GetController()));
      done_ = true;
      // If the message loop is running stop it.
      if (running_) {
        running_ = false;
        message_loop_runner_->Quit();
      }
      break;
    default:
      NOTREACHED();
  };
}

void TestWebDialogObserver::OnDialogShown(
      content::WebUI* webui,
      content::RenderViewHost* render_view_host) {
  if (js_injection_ready_observer_) {
    js_injection_ready_observer_->OnJsInjectionReady(render_view_host);
  }
  web_ui_ = webui;
  // Wait for navigation on the new WebUI instance to complete. This depends
  // on receiving the notification of the WebDialog being shown before the
  // NavigationController finishes loading. The WebDialog notification is
  // issued from web_dialog_ui.cc on RenderView creation which results from
  // the call to render_manager_.Navigate in the method
  // WebContents::NavigateToEntry. The new RenderView is later told to
  // navigate in this method, ensuring that this is not a race condition.
  registrar_.Add(this, content::NOTIFICATION_LOAD_STOP,
                 content::Source<NavigationController>(
                     &web_ui_->GetWebContents()->GetController()));
}

content::WebUI* TestWebDialogObserver::GetWebUI() {
  if (!done_) {
    DCHECK(running_ == false);
    running_ = true;
    message_loop_runner_ = new content::MessageLoopRunner;
    message_loop_runner_->Run();
  }
  return web_ui_;
}

}  // namespace test
}  // namespace ui
