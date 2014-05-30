// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_NATIVE_NATIVE_VIEW_HOST_WIN_H_
#define UI_VIEWS_CONTROLS_NATIVE_NATIVE_VIEW_HOST_WIN_H_

#include "base/basictypes.h"
#include "ui/views/controls/native/native_view_host_wrapper.h"

namespace views {

class NativeViewHost;

// A Windows implementation of NativeViewHostWrapper
class NativeViewHostWin : public NativeViewHostWrapper {
 public:
  explicit NativeViewHostWin(NativeViewHost* host);
  virtual ~NativeViewHostWin();

  // Overridden from NativeViewHostWrapper:
  virtual void NativeViewWillAttach();
  virtual void NativeViewDetaching(bool destroyed);
  virtual void AddedToWidget();
  virtual void RemovedFromWidget();
  virtual void InstallClip(int x, int y, int w, int h);
  virtual bool HasInstalledClip();
  virtual void UninstallClip();
  virtual void ShowWidget(int x, int y, int w, int h);
  virtual void HideWidget();
  virtual void SetFocus();
  virtual gfx::NativeViewAccessible GetNativeViewAccessible();

 private:
  // Our associated NativeViewHost.
  NativeViewHost* host_;

  // Have we installed a region on the gfx::NativeView used to clip to only the
  // visible portion of the gfx::NativeView ?
  bool installed_clip_;

  DISALLOW_COPY_AND_ASSIGN(NativeViewHostWin);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_NATIVE_NATIVE_VIEW_HOST_WIN_H_
