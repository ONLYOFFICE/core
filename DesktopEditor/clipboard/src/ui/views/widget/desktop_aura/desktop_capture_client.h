// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_CAPTURE_CLIENT_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_CAPTURE_CLIENT_H_

#include <set>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/aura/client/capture_client.h"
#include "ui/views/views_export.h"

namespace views {

// A capture client which will collaborate with all other capture clients of
// its class. When capture is changed in an instance of this capture client,
// capture is released in all other windows.
class VIEWS_EXPORT DesktopCaptureClient : public aura::client::CaptureClient {
 public:
  explicit DesktopCaptureClient(aura::RootWindow* root_window);
  virtual ~DesktopCaptureClient();

  // Overridden from client::CaptureClient:
  virtual void SetCapture(aura::Window* window) OVERRIDE;
  virtual void ReleaseCapture(aura::Window* window) OVERRIDE;
  virtual aura::Window* GetCaptureWindow() OVERRIDE;

 private:
  // Called when another instance of the capture client takes capture.
  void OnOtherCaptureClientTookCapture();

  aura::RootWindow* root_window_;
  aura::Window* capture_window_;

  static std::set<DesktopCaptureClient*> live_capture_clients_;

  DISALLOW_COPY_AND_ASSIGN(DesktopCaptureClient);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_CAPTURE_CLIENT_H_
