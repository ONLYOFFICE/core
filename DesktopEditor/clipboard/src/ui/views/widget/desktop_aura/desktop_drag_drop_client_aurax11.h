// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_DRAG_DROP_CLIENT_AURAX11_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_DRAG_DROP_CLIENT_AURAX11_H_

#include <X11/Xlib.h>

// Get rid of a macro from Xlib.h that conflicts with Aura's RootWindow class.
#undef RootWindow

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/client/drag_drop_client.h"
#include "ui/aura/window_observer.h"
#include "ui/base/x/x11_atom_cache.h"
#include "ui/gfx/point.h"
#include "ui/views/views_export.h"

namespace aura {
class RootWindow;
namespace client {
class DragDropDelegate;
}
}

namespace gfx {
class Point;
}

namespace ui {
class DragSource;
class DropTargetEvent;
class OSExchangeData;
class RootWindow;
}

namespace views {
class DesktopRootWindowHostX11;

// Implements drag and drop on X11 for aura. On one side, this class takes raw
// X11 events forwarded from DesktopRootWindowHostLinux, while on the other, it
// handles the views drag events.
class VIEWS_EXPORT DesktopDragDropClientAuraX11
    : public aura::client::DragDropClient,
      public aura::WindowObserver {
 public:
  DesktopDragDropClientAuraX11(
      views::DesktopRootWindowHostX11* root_window_host,
      aura::RootWindow* root_window,
      Display* xdisplay,
      ::Window xwindow);
  virtual ~DesktopDragDropClientAuraX11();

  // These methods handle the various X11 client messages from the platform.
  void OnXdndEnter(const XClientMessageEvent& event);
  void OnXdndLeave(const XClientMessageEvent& event);
  void OnXdndPosition(const XClientMessageEvent& event);
  void OnXdndStatus(const XClientMessageEvent& event);
  void OnXdndFinished(const XClientMessageEvent& event);
  void OnXdndDrop(const XClientMessageEvent& event);

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

  // aura::WindowObserver implementation:
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;

 private:
  // When we receive an position x11 message, we need to translate that into
  // the underlying aura::Window representation, as moves internal to the X11
  // window can cause internal drag leave and enter messages.
  void DragTranslate(const gfx::Point& root_window_location,
                     scoped_ptr<ui::OSExchangeData>* data,
                     scoped_ptr<ui::DropTargetEvent>* event,
                     aura::client::DragDropDelegate** delegate);

  // Called when we need to notify the current aura::Window that we're no
  // longer dragging over it.
  void NotifyDragLeave();

  // Converts our bitfield of actions into an Atom that represents what action
  // we're most likely to take on drop.
  unsigned long DragOperationToAtom(int drag_operation);

  // Sends |xev| to |xid|, optionally short circuiting the round trip to the X
  // server.
  void SendXClientEvent(unsigned long xid, XEvent* xev);

  views::DesktopRootWindowHostX11* root_window_host_;
  aura::RootWindow* root_window_;

  Display* xdisplay_;
  ::Window xwindow_;

  ui::X11AtomCache atom_cache_;

  // Target side information.

  class X11DragContext;
  scoped_ptr<X11DragContext> current_context_;

  // The Aura window that is currently under the cursor. We need to manually
  // keep track of this because Windows will only call our drag enter method
  // once when the user enters the associated X Window. But inside that X
  // Window there could be multiple aura windows, so we need to generate drag
  // enter events for them.
  aura::Window* target_window_;

  // Because Xdnd messages don't contain the position in messages other than
  // the XdndPosition message, we must manually keep track of the last position
  // change.
  gfx::Point target_window_location_;
  gfx::Point target_window_root_location_;

  bool drag_drop_in_progress_;

  int drag_operation_;

  DISALLOW_COPY_AND_ASSIGN(DesktopDragDropClientAuraX11);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_DRAG_DROP_CLIENT_AURAX11_H_
