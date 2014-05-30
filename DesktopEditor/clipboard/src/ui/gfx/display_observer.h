// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_DISPLAY_OBSERVER_H_
#define UI_GFX_DISPLAY_OBSERVER_H_

#include "ui/base/ui_export.h"

namespace gfx {
class Display;

// Observers for display configuration changes.
// TODO(oshima): consolidate |WorkAreaWatcherObserver| and
// |DisplaySettingsProvier|. crbug.com/122863.
class UI_EXPORT DisplayObserver {
 public:
  // Called when the |display|'s bound has changed.
  virtual void OnDisplayBoundsChanged(const Display& display) = 0;

  // Called when |new_display| has been added.
  virtual void OnDisplayAdded(const Display& new_display) = 0;

  // Called when |old_display| has been removed.
  virtual void OnDisplayRemoved(const Display& old_display) = 0;

 protected:
  virtual ~DisplayObserver();
};

}  // namespace gfx

#endif  // UI_GFX_DISPLAY_OBSERVER_H_
