// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/client/default_capture_client.h"

#include "ui/aura/root_window.h"

namespace aura {
namespace client {

DefaultCaptureClient::DefaultCaptureClient(RootWindow* root_window)
    : root_window_(root_window),
      capture_window_(NULL) {
  client::SetCaptureClient(root_window_, this);
}

DefaultCaptureClient::~DefaultCaptureClient() {
  client::SetCaptureClient(root_window_, NULL);
}

void DefaultCaptureClient::SetCapture(Window* window) {
  if (capture_window_ == window)
    return;
  if (window)
    root_window_->gesture_recognizer()->
        TransferEventsTo(capture_window_, window);

  Window* old_capture_window = capture_window_;
  capture_window_ = window;

  if (capture_window_)
    root_window_->SetNativeCapture();
  else
    root_window_->ReleaseNativeCapture();

  root_window_->UpdateCapture(old_capture_window, capture_window_);
}

void DefaultCaptureClient::ReleaseCapture(Window* window) {
  if (capture_window_ != window)
    return;
  SetCapture(NULL);
}

Window* DefaultCaptureClient::GetCaptureWindow() {
  return capture_window_;
}

}  // namespace client
}  // namespace aura
