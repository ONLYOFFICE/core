// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_DRAG_DROP_CLIENT_WIN_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_DRAG_DROP_CLIENT_WIN_H_

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "ui/aura/client/drag_drop_client.h"
#include "ui/views/views_export.h"

namespace ui {
class DragSourceWin;
class RootWindow;
}

namespace views {
class DesktopDragDragSourceWin;
class DesktopDropTargetWin;

class VIEWS_EXPORT DesktopDragDropClientWin
    : public aura::client::DragDropClient {
 public:
  DesktopDragDropClientWin(aura::RootWindow* root_window, HWND window);
  virtual ~DesktopDragDropClientWin();

  // Overridden from aura::client::DragDropClient:
  virtual int StartDragAndDrop(
      const ui::OSExchangeData& data,
      aura::RootWindow* root_window,
      aura::Window* source_window,
      const gfx::Point& root_location,
      int operation,
      ui::DragDropTypes::DragEventSource source) OVERRIDE;
  virtual void DragUpdate(aura::Window* target,
                          const ui::LocatedEvent& event) OVERRIDE;
  virtual void Drop(aura::Window* target,
                    const ui::LocatedEvent& event) OVERRIDE;
  virtual void DragCancel() OVERRIDE;
  virtual bool IsDragDropInProgress() OVERRIDE;

  void OnNativeWidgetDestroying(HWND window);

 private:
  bool drag_drop_in_progress_;

  int drag_operation_;

  scoped_refptr<ui::DragSourceWin> drag_source_;

  scoped_refptr<DesktopDropTargetWin> drop_target_;

  DISALLOW_COPY_AND_ASSIGN(DesktopDragDropClientWin);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_DRAG_DROP_CLIENT_WIN_H_
