// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DROP_TARGET_WIN_H_
#define UI_VIEWS_WIDGET_DROP_TARGET_WIN_H_

#include "ui/base/dragdrop/drop_target_win.h"
#include "ui/views/widget/drop_helper.h"

namespace views {

class View;
namespace internal {
class RootView;
}

// DropTargetWin takes care of managing drag and drop for NativeWidgetWin. It
// converts Windows OLE drop messages into Views drop messages.
//
// DropTargetWin uses DropHelper to manage the appropriate view to target
// drop messages at.
class DropTargetWin : public ui::DropTargetWin {
 public:
  explicit DropTargetWin(internal::RootView* root_view);
  virtual ~DropTargetWin();

  // If a drag and drop is underway and view is the current drop target, the
  // drop target is set to null.
  // This is invoked when a View is removed from the RootView to make sure
  // we don't target a view that was removed during dnd.
  void ResetTargetViewIfEquals(View* view);

 protected:
  virtual DWORD OnDragOver(IDataObject* data_object,
                           DWORD key_state,
                           POINT cursor_position,
                           DWORD effect);

  virtual void OnDragLeave(IDataObject* data_object);

  virtual DWORD OnDrop(IDataObject* data_object,
                       DWORD key_state,
                       POINT cursor_position,
                       DWORD effect);

 private:
  views::DropHelper helper_;

  DISALLOW_COPY_AND_ASSIGN(DropTargetWin);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DROP_TARGET_WIN_H_
