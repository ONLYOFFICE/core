// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_WEB_DIALOGS_TEST_TEST_WEB_DIALOG_DELEGATE_H_
#define UI_WEB_DIALOGS_TEST_TEST_WEB_DIALOG_DELEGATE_H_

#include <string>

#include "base/compiler_specific.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/size.h"
#include "ui/web_dialogs/web_dialog_delegate.h"

namespace ui {
namespace test {

class TestWebDialogDelegate : public WebDialogDelegate {
 public:
  explicit TestWebDialogDelegate(const GURL& url);
  virtual ~TestWebDialogDelegate();

  void set_size(int width, int height) {
    size_.SetSize(width, height);
  }

  // WebDialogDelegate implementation:
  virtual ModalType GetDialogModalType() const OVERRIDE;
  virtual string16 GetDialogTitle() const OVERRIDE;
  virtual GURL GetDialogContentURL() const OVERRIDE;
  virtual void GetWebUIMessageHandlers(
      std::vector<content::WebUIMessageHandler*>* handlers) const OVERRIDE;
  virtual void GetDialogSize(gfx::Size* size) const OVERRIDE;
  virtual std::string GetDialogArgs() const OVERRIDE;
  virtual void OnDialogClosed(const std::string& json_retval) OVERRIDE;
  virtual void OnCloseContents(content::WebContents* source,
                               bool* out_close_dialog) OVERRIDE;
  virtual bool ShouldShowDialogTitle() const OVERRIDE;

 protected:
  const GURL url_;
  gfx::Size size_;

  DISALLOW_COPY_AND_ASSIGN(TestWebDialogDelegate);
};

}  // namespace test
}  // namespace ui

#endif  // UI_WEB_DIALOGS_TEST_TEST_WEB_DIALOG_DELEGATE_H_
