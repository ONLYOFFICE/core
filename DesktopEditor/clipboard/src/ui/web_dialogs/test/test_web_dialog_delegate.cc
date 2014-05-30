// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/web_dialogs/test/test_web_dialog_delegate.h"

#include "base/utf_string_conversions.h"

using content::WebContents;
using content::WebUIMessageHandler;

namespace ui {
namespace test {

TestWebDialogDelegate::TestWebDialogDelegate(const GURL& url)
    : url_(url),
      size_(400, 400) {
}

TestWebDialogDelegate::~TestWebDialogDelegate() {
}

ModalType TestWebDialogDelegate::GetDialogModalType() const {
  return MODAL_TYPE_WINDOW;
}

string16 TestWebDialogDelegate::GetDialogTitle() const {
  return UTF8ToUTF16("Test");
}

GURL TestWebDialogDelegate::GetDialogContentURL() const {
  return url_;
}

void TestWebDialogDelegate::GetWebUIMessageHandlers(
    std::vector<WebUIMessageHandler*>* handlers) const {
}

void TestWebDialogDelegate::GetDialogSize(gfx::Size* size) const {
  *size = size_;
}

std::string TestWebDialogDelegate::GetDialogArgs() const {
  return std::string();
}

void TestWebDialogDelegate::OnDialogClosed(const std::string& json_retval) {
}

void TestWebDialogDelegate::OnCloseContents(WebContents* source,
    bool* out_close_dialog) {
  if (out_close_dialog)
    *out_close_dialog = true;
}

bool TestWebDialogDelegate::ShouldShowDialogTitle() const {
  return true;
}

}  // namespace test
}  // namespace ui
