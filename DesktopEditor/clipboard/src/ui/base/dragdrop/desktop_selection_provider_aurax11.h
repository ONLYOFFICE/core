// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_DRAGDROP_DESKTOP_SELECTION_PROVIDER_AURAX11_H_
#define UI_BASE_DRAGDROP_DESKTOP_SELECTION_PROVIDER_AURAX11_H_

#include "ui/base/ui_export.h"

namespace ui {
class OSExchangeDataProviderAuraX11;

// Used by OSExchangeDataProviderAuraX11 to attach itself to an object which
// can provide it with SelectionNotify events.
class UI_EXPORT DesktopSelectionProviderAuraX11 {
 public:
  virtual ~DesktopSelectionProviderAuraX11() {}

  virtual void SetDropHandler(OSExchangeDataProviderAuraX11* handler) = 0;
};

}  // namespace

#endif  // UI_BASE_DRAGDROP_DESKTOP_SELECTION_PROVIDER_AURAX11_H_
