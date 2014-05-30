// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_NATIVE_CURSOR_MANAGER_DELEGATE_H_
#define UI_VIEWS_COREWM_NATIVE_CURSOR_MANAGER_DELEGATE_H_

#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"

namespace views {
namespace corewm {

// The non-public interface that CursorManager exposes to its users. This
// gives accessors to all the current state, and mutators to all the current
// state.
class VIEWS_EXPORT NativeCursorManagerDelegate {
 public:
  virtual ~NativeCursorManagerDelegate() {}

  virtual gfx::NativeCursor GetCurrentCursor() const = 0;
  virtual bool GetCurrentVisibility() const = 0;
  virtual bool GetMouseEventsEnabled() const = 0;

  virtual void CommitCursor(gfx::NativeCursor cursor) = 0;
  virtual void CommitVisibility(bool visible) = 0;
  virtual void CommitMouseEventsEnabled(bool enabled) = 0;
};

}  // namespace corewm
}  // namespace views

#endif  // UI_VIEWS_COREWM_NATIVE_CURSOR_MANAGER_DELEGATE_H_
