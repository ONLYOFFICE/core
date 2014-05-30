// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/desktop_capture_client.h"

#include "ui/aura/root_window.h"

namespace views {

std::set<DesktopCaptureClient*> DesktopCaptureClient::live_capture_clients_;

DesktopCaptureClient::DesktopCaptureClient(aura::RootWindow* root_window)
    : root_window_(root_window),
      capture_window_(NULL) {
  aura::client::SetCaptureClient(root_window_, this);
  live_capture_clients_.insert(this);
}

DesktopCaptureClient::~DesktopCaptureClient() {
  live_capture_clients_.erase(this);
  aura::client::SetCaptureClient(root_window_, NULL);
}

void DesktopCaptureClient::SetCapture(aura::Window* window) {
  if (capture_window_ == window)
    return;
  if (window)
    root_window_->gesture_recognizer()->
        TransferEventsTo(capture_window_, window);

  aura::Window* old_capture_window = capture_window_;
  capture_window_ = window;

  if (capture_window_) {
    root_window_->SetNativeCapture();

    for (std::set<DesktopCaptureClient*>::iterator it =
             live_capture_clients_.begin(); it != live_capture_clients_.end();
         ++it) {
      if (*it != this)
        (*it)->OnOtherCaptureClientTookCapture();
    }
  } else {
    root_window_->ReleaseNativeCapture();
  }

  root_window_->UpdateCapture(old_capture_window, capture_window_);
}

void DesktopCaptureClient::ReleaseCapture(aura::Window* window) {
  if (capture_window_ != window)
    return;
  SetCapture(NULL);
}

aura::Window* DesktopCaptureClient::GetCaptureWindow() {
  return capture_window_;
}

void DesktopCaptureClient::OnOtherCaptureClientTookCapture() {
  if (capture_window_ == NULL) {
    // While RootWindow may not technically have capture, it will store state
    // that needs to be cleared on capture changed regarding mouse up/down.
    root_window_->ClearMouseHandlers();
  }
#if defined(OS_LINUX)
  else {
    // Capture is a concept that we emulate on Linux; we don't receive messages
    // from the X11 server about right click menu dismissal.
    SetCapture(NULL);
  }
#endif
}

}  // namespace views
