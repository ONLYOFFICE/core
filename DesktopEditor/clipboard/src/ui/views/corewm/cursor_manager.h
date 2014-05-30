// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_CURSOR_MANAGER_H_
#define UI_VIEWS_COREWM_CURSOR_MANAGER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "ui/aura/client/cursor_client.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/point.h"
#include "ui/views/corewm/native_cursor_manager_delegate.h"
#include "ui/views/views_export.h"

namespace gfx {
class Display;
}

namespace views {
namespace corewm {

namespace internal {
class CursorState;
}

class NativeCursorManager;

// This class receives requests to change cursor properties, as well as
// requests to queue any further changes until a later time. It sends changes
// to the NativeCursorManager, which communicates back to us when these changes
// were made through the NativeCursorManagerDelegate interface.
class VIEWS_EXPORT CursorManager : public aura::client::CursorClient,
                                   public NativeCursorManagerDelegate {
 public:
  CursorManager(scoped_ptr<NativeCursorManager> delegate);
  virtual ~CursorManager();

  bool is_cursor_locked() const { return cursor_lock_count_ > 0; }

  // Overridden from aura::client::CursorClient:
  virtual void SetCursor(gfx::NativeCursor) OVERRIDE;
  virtual void ShowCursor() OVERRIDE;
  virtual void HideCursor() OVERRIDE;
  virtual bool IsCursorVisible() const OVERRIDE;
  virtual void EnableMouseEvents() OVERRIDE;
  virtual void DisableMouseEvents() OVERRIDE;
  virtual bool IsMouseEventsEnabled() const OVERRIDE;
  virtual void SetDisplay(const gfx::Display& display) OVERRIDE;
  virtual void LockCursor() OVERRIDE;
  virtual void UnlockCursor() OVERRIDE;
  virtual void SetCursorResourceModule(const string16& module_name) OVERRIDE;
  virtual void AddObserver(
      aura::client::CursorClientObserver* observer) OVERRIDE;
  virtual void RemoveObserver(
      aura::client::CursorClientObserver* observer) OVERRIDE;

 private:
  // Overridden from NativeCursorManagerDelegate:
  virtual gfx::NativeCursor GetCurrentCursor() const OVERRIDE;
  virtual bool GetCurrentVisibility() const OVERRIDE;
  virtual bool GetMouseEventsEnabled() const OVERRIDE;
  virtual void CommitCursor(gfx::NativeCursor cursor) OVERRIDE;
  virtual void CommitVisibility(bool visible) OVERRIDE;
  virtual void CommitMouseEventsEnabled(bool enabled) OVERRIDE;

  scoped_ptr<NativeCursorManager> delegate_;

  // Number of times LockCursor() has been invoked without a corresponding
  // UnlockCursor().
  int cursor_lock_count_;

  // The current state of the cursor.
  scoped_ptr<internal::CursorState> current_state_;

  // The cursor state to restore when the cursor is unlocked.
  scoped_ptr<internal::CursorState> state_on_unlock_;

  ObserverList<aura::client::CursorClientObserver> observers_;

  DISALLOW_COPY_AND_ASSIGN(CursorManager);
};

}  // namespace corewm
}  // namespace views

#endif  // UI_VIEWS_COREWM_CURSOR_MANAGER_H_
