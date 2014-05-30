// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/drop_target_win.h"

#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/base/dragdrop/os_exchange_data_provider_win.h"
#include "ui/gfx/point.h"
#include "ui/views/widget/root_view.h"
#include "ui/views/widget/widget.h"

using ui::OSExchangeData;
using ui::OSExchangeDataProviderWin;

namespace views {

DropTargetWin::DropTargetWin(internal::RootView* root_view)
    : ui::DropTargetWin(root_view->GetWidget()->GetNativeView()),
      helper_(root_view) {
}

DropTargetWin::~DropTargetWin() {
}

void DropTargetWin::ResetTargetViewIfEquals(View* view) {
  helper_.ResetTargetViewIfEquals(view);
}

DWORD DropTargetWin::OnDragOver(IDataObject* data_object,
                                DWORD key_state,
                                POINT cursor_position,
                                DWORD effect) {
  gfx::Point root_view_location(cursor_position.x, cursor_position.y);
  View::ConvertPointToTarget(NULL, helper_.root_view(), &root_view_location);
  OSExchangeData data(new OSExchangeDataProviderWin(data_object));
  int drop_operation =
      helper_.OnDragOver(data, root_view_location,
                         ui::DragDropTypes::DropEffectToDragOperation(effect));
  return ui::DragDropTypes::DragOperationToDropEffect(drop_operation);
}

void DropTargetWin::OnDragLeave(IDataObject* data_object) {
  helper_.OnDragExit();
}

DWORD DropTargetWin::OnDrop(IDataObject* data_object,
                            DWORD key_state,
                            POINT cursor_position,
                            DWORD effect) {
  gfx::Point root_view_location(cursor_position.x, cursor_position.y);
  View::ConvertPointToTarget(NULL, helper_.root_view(), &root_view_location);

  OSExchangeData data(new OSExchangeDataProviderWin(data_object));
  int drop_operation = ui::DragDropTypes::DropEffectToDragOperation(effect);
  drop_operation = helper_.OnDragOver(data, root_view_location,
                                      drop_operation);
  drop_operation = helper_.OnDrop(data, root_view_location, drop_operation);
  return ui::DragDropTypes::DragOperationToDropEffect(drop_operation);
}

}  // namespace views
