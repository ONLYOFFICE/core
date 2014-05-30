// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/desktop_drag_drop_client_aurax11.h"

#include <X11/Xatom.h>

#include "base/event_types.h"
#include "base/message_loop.h"
#include "ui/aura/client/drag_drop_client.h"
#include "ui/aura/client/drag_drop_delegate.h"
#include "ui/aura/root_window.h"
#include "ui/aura/window.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/base/dragdrop/os_exchange_data_provider_aurax11.h"
#include "ui/base/events/event.h"
#include "ui/base/x/x11_util.h"
#include "ui/views/widget/desktop_aura/desktop_root_window_host_x11.h"

using aura::client::DragDropDelegate;
using ui::OSExchangeData;

namespace {

const char kXdndActionCopy[] = "XdndActionCopy";
const char kXdndActionMove[] = "XdndActionMove";
const char kXdndActionLink[] = "XdndActionLink";

const char* kAtomsToCache[] = {
  "XdndActionAsk",
  kXdndActionCopy,
  kXdndActionLink,
  "XdndActionList",
  kXdndActionMove,
  "XdndActionPrivate",
  "XdndAware",
  "XdndDrop",
  "XdndEnter",
  "XdndFinished",
  "XdndLeave",
  "XdndPosition",
  "XdndProxy",  // Proxy windows?
  "XdndSelection",
  "XdndStatus",
  "XdndTypeList",
  NULL
};

}  // namespace

namespace views {

class DesktopDragDropClientAuraX11::X11DragContext :
    public base::MessageLoop::Dispatcher {
 public:
  X11DragContext(ui::X11AtomCache* atom_cache,
                 const XClientMessageEvent& event);
  virtual ~X11DragContext();

  const std::vector<Atom>& targets() { return targets_; }

  // Reads the "XdndActionList" property from |source_window| and copies it
  // into |actions|.
  void ReadActions();

  // Creates a ui::DragDropTypes::DragOperation representation of the current
  // action list.
  int GetDragOperation() const;

 private:
  // Overridden from MessageLoop::Dispatcher:
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

  // The atom cache owned by our parent.
  ui::X11AtomCache* atom_cache_;

  // The XID of the window that's initiated the drag.
  unsigned long source_window_;

  // targets.
  std::vector<Atom> targets_;

  // supplied actions
  std::vector<Atom> actions_;

  DISALLOW_COPY_AND_ASSIGN(X11DragContext);
};

DesktopDragDropClientAuraX11::X11DragContext::X11DragContext(
    ui::X11AtomCache* atom_cache,
    const XClientMessageEvent& event)
    : atom_cache_(atom_cache),
      source_window_(event.data.l[0]) {
  bool get_types = ((event.data.l[1] & 1) != 0);

  if (get_types) {
    if (!ui::GetAtomArrayProperty(source_window_,
                                  "XdndTypeList",
                                  &targets_)) {
      return;
    }
  } else {
    // data.l[2,3,4] contain the first three types. Unused slots can be None.
    for (int i = 0; i < 3; ++i) {
      if (event.data.l[2+i] != None) {
        targets_.push_back(event.data.l[2+i]);
      }
    }
  }

  // TODO(erg): If this window is part of our process, don't listen through the
  // MessagePump, but instead listen to the DesktopDragDropClientAuraX11
  // associated with that window.
  base::MessagePumpAuraX11::Current()->AddDispatcherForWindow(
      this, source_window_);
  XSelectInput(base::MessagePumpAuraX11::GetDefaultXDisplay(),
               source_window_, PropertyChangeMask);

  // We must perform a full sync here because we could be racing
  // |source_window_|.
  XSync(base::MessagePumpAuraX11::GetDefaultXDisplay(), False);

  ReadActions();
}

DesktopDragDropClientAuraX11::X11DragContext::~X11DragContext() {
  // Unsubscribe to message events.
  base::MessagePumpAuraX11::Current()->RemoveDispatcherForWindow(
      source_window_);
}

void DesktopDragDropClientAuraX11::X11DragContext::ReadActions() {
  std::vector<Atom> atom_array;

  // TODO(erg): The GTK+ code has a fast path that short circuits talking over
  // X11 for local windows. When we become a drop source, we should have a
  // similar fast path.

  if (!ui::GetAtomArrayProperty(source_window_,
                                "XdndActionList",
                                &atom_array)) {
    actions_.clear();
  } else {
    actions_.swap(atom_array);
  }
}

int DesktopDragDropClientAuraX11::X11DragContext::GetDragOperation() const {
  int drag_operation = ui::DragDropTypes::DRAG_NONE;
  for (std::vector<Atom>::const_iterator it = actions_.begin();
       it != actions_.end(); ++it) {
    if (*it == atom_cache_->GetAtom(kXdndActionCopy))
      drag_operation |= ui::DragDropTypes::DRAG_COPY;
    else if (*it == atom_cache_->GetAtom(kXdndActionMove))
      drag_operation |= ui::DragDropTypes::DRAG_MOVE;
    else if (*it == atom_cache_->GetAtom(kXdndActionLink))
      drag_operation |= ui::DragDropTypes::DRAG_LINK;
  }

  return drag_operation;
}

bool DesktopDragDropClientAuraX11::X11DragContext::Dispatch(
    const base::NativeEvent& event) {
  if (event->type == PropertyNotify &&
      event->xproperty.atom == atom_cache_->GetAtom("XdndActionList")) {
    ReadActions();
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////

DesktopDragDropClientAuraX11::DesktopDragDropClientAuraX11(
    views::DesktopRootWindowHostX11* root_window_host,
    aura::RootWindow* root_window,
    Display* xdisplay,
    ::Window xwindow)
    : root_window_host_(root_window_host),
      root_window_(root_window),
      xdisplay_(xdisplay),
      xwindow_(xwindow),
      atom_cache_(xdisplay_, kAtomsToCache),
      target_window_(NULL),
      drag_drop_in_progress_(false),
      drag_operation_(0) {
  // Mark that we are aware of drag and drop concepts.
  unsigned long xdnd_version = 5;
  XChangeProperty(xdisplay_, xwindow_, atom_cache_.GetAtom("XdndAware"),
                  XA_ATOM, 32, PropModeReplace,
                  reinterpret_cast<unsigned char*>(&xdnd_version), 1);
}

DesktopDragDropClientAuraX11::~DesktopDragDropClientAuraX11() {
}

void DesktopDragDropClientAuraX11::OnXdndEnter(
    const XClientMessageEvent& event) {
  DVLOG(1) << "XdndEnter";

  int version = (event.data.l[1] & 0xff000000) >> 24;
  if (version < 3) {
    LOG(ERROR) << "Received old XdndEnter message.";
    return;
  }

  // Make sure that we've run ~X11DragContext() before creating another one.
  current_context_.reset();
  current_context_.reset(new X11DragContext(&atom_cache_, event));

  // In the Windows implementation, we immediately call DesktopDropTargetWin::
  // Translate(). Here, we wait until we receive an XdndPosition message
  // because the enter message doesn't convey any positioning
  // information.
}

void DesktopDragDropClientAuraX11::OnXdndLeave(
    const XClientMessageEvent& event) {
  NotifyDragLeave();
  current_context_.reset();
}

void DesktopDragDropClientAuraX11::OnXdndPosition(
    const XClientMessageEvent& event) {
  DVLOG(1) << "XdndPosition";

  unsigned long source_window = event.data.l[0];
  int x_root_window = event.data.l[2] >> 16;
  int y_root_window = event.data.l[2] & 0xffff;

  if (!current_context_.get()) {
    NOTREACHED();
    return;
  }

  int drag_operation = ui::DragDropTypes::DRAG_NONE;
  scoped_ptr<ui::OSExchangeData> data;
  scoped_ptr<ui::DropTargetEvent> drop_target_event;
  DragDropDelegate* delegate;
  DragTranslate(gfx::Point(x_root_window, y_root_window),
                &data, &drop_target_event, &delegate);
  if (delegate)
    drag_operation = delegate->OnDragUpdated(*drop_target_event);

  // Sends an XdndStatus message back to the source_window. l[2,3]
  // theoretically represent an area in the window where the current action is
  // the same as what we're returning, but I can't find any implementation that
  // actually making use of this. A client can return (0, 0) and/or set the
  // first bit of l[1] to disable the feature, and it appears that gtk neither
  // sets this nor respects it if set.
  XEvent xev;
  xev.xclient.type = ClientMessage;
  xev.xclient.message_type = atom_cache_.GetAtom("XdndStatus");
  xev.xclient.format = 32;
  xev.xclient.window = source_window;
  xev.xclient.data.l[0] = xwindow_;
  xev.xclient.data.l[1] = (drag_operation != 0) ? (2 | 1) : 0;
  xev.xclient.data.l[2] = 0;
  xev.xclient.data.l[3] = 0;
  xev.xclient.data.l[4] = DragOperationToAtom(drag_operation);

  SendXClientEvent(source_window, &xev);
}

void DesktopDragDropClientAuraX11::OnXdndStatus(
    const XClientMessageEvent& event) {
  DVLOG(1) << "XdndStatus";
}

void DesktopDragDropClientAuraX11::OnXdndFinished(
    const XClientMessageEvent& event) {
  DVLOG(1) << "XdndFinished";
}

void DesktopDragDropClientAuraX11::OnXdndDrop(
    const XClientMessageEvent& event) {
  DVLOG(1) << "XdndDrop";

  unsigned long source_window = event.data.l[0];

  int drag_operation = ui::DragDropTypes::DRAG_NONE;
  if (target_window_) {
    aura::client::DragDropDelegate* delegate =
        aura::client::GetDragDropDelegate(target_window_);
    if (delegate) {
      ui::OSExchangeData data(new ui::OSExchangeDataProviderAuraX11(
          root_window_host_, xwindow_, current_context_->targets()));
      ui::DropTargetEvent event(data,
                                target_window_location_,
                                target_window_root_location_,
                                current_context_->GetDragOperation());
      drag_operation = delegate->OnPerformDrop(event);
    }

    target_window_->RemoveObserver(this);
    target_window_ = NULL;
  }

  XEvent xev;
  xev.xclient.type = ClientMessage;
  xev.xclient.message_type = atom_cache_.GetAtom("XdndFinished");
  xev.xclient.format = 32;
  xev.xclient.window = source_window;
  xev.xclient.data.l[0] = xwindow_;
  xev.xclient.data.l[1] = (drag_operation != 0) ? 1 : 0;
  xev.xclient.data.l[2] = DragOperationToAtom(drag_operation);

  SendXClientEvent(source_window, &xev);
}

int DesktopDragDropClientAuraX11::StartDragAndDrop(
    const ui::OSExchangeData& data,
    aura::RootWindow* root_window,
    aura::Window* source_window,
    const gfx::Point& root_location,
    int operation,
    ui::DragDropTypes::DragEventSource source) {
  NOTIMPLEMENTED();

  // TODO(erg): Once this is implemented, make sure to reenable the
  // NativeTextfieldViewsTest_DragAndDrop* tests.

  return ui::DragDropTypes::DRAG_NONE;
}

void DesktopDragDropClientAuraX11::DragUpdate(aura::Window* target,
                                              const ui::LocatedEvent& event) {
  NOTIMPLEMENTED();
}

void DesktopDragDropClientAuraX11::Drop(aura::Window* target,
                                        const ui::LocatedEvent& event) {
  NOTIMPLEMENTED();
}

void DesktopDragDropClientAuraX11::DragCancel() {
  NOTIMPLEMENTED();
}

bool DesktopDragDropClientAuraX11::IsDragDropInProgress() {
  return drag_drop_in_progress_;
}

void DesktopDragDropClientAuraX11::OnWindowDestroyed(aura::Window* window) {
  DCHECK_EQ(target_window_, window);
  target_window_ = NULL;
}

void DesktopDragDropClientAuraX11::DragTranslate(
    const gfx::Point& root_window_location,
    scoped_ptr<ui::OSExchangeData>* data,
    scoped_ptr<ui::DropTargetEvent>* event,
    aura::client::DragDropDelegate** delegate) {
  gfx::Point root_location = root_window_location;
  root_window_->ConvertPointFromNativeScreen(&root_location);
  aura::Window* target_window =
      root_window_->GetEventHandlerForPoint(root_location);
  bool target_window_changed = false;
  if (target_window != target_window_) {
    if (target_window_)
      NotifyDragLeave();
    target_window_ = target_window;
    if (target_window_)
      target_window_->AddObserver(this);
    target_window_changed = true;
  }
  *delegate = NULL;
  if (!target_window_)
    return;
  *delegate = aura::client::GetDragDropDelegate(target_window_);
  if (!*delegate)
    return;

  data->reset(new OSExchangeData(new ui::OSExchangeDataProviderAuraX11(
      root_window_host_, xwindow_, current_context_->targets())));
  gfx::Point location = root_location;
  aura::Window::ConvertPointToTarget(root_window_, target_window_, &location);

  target_window_location_ = location;
  target_window_root_location_ = root_location;

  event->reset(new ui::DropTargetEvent(
      *(data->get()),
      location,
      root_location,
      current_context_->GetDragOperation()));
  if (target_window_changed)
    (*delegate)->OnDragEntered(*event->get());
}

void DesktopDragDropClientAuraX11::NotifyDragLeave() {
  if (!target_window_)
    return;
  DragDropDelegate* delegate =
      aura::client::GetDragDropDelegate(target_window_);
  if (delegate)
    delegate->OnDragExited();
  target_window_->RemoveObserver(this);
  target_window_ = NULL;
}

unsigned long DesktopDragDropClientAuraX11::DragOperationToAtom(
    int drag_operation) {
  if (drag_operation & ui::DragDropTypes::DRAG_COPY)
    return atom_cache_.GetAtom(kXdndActionCopy);
  if (drag_operation & ui::DragDropTypes::DRAG_MOVE)
    return atom_cache_.GetAtom(kXdndActionMove);
  if (drag_operation & ui::DragDropTypes::DRAG_LINK)
    return atom_cache_.GetAtom(kXdndActionLink);

  return None;
}

void DesktopDragDropClientAuraX11::SendXClientEvent(unsigned long xid,
                                                    XEvent* xev) {
  DCHECK_EQ(ClientMessage, xev->type);

  // TODO(erg): When I get drag receiving working, shortcut messages to the X
  // server and call the receivers DesktopDragDropClientAuraX11 instance
  // instead.
  //
  // I don't understand why the GTK+ code is doing what it's doing here. It
  // goes out of its way to send the XEvent so that it receives a callback on
  // success or failure, and when it fails, it then sends an internal GdkEvent
  // about the failed drag. (And sending this message doesn't appear to go
  // through normal xlib machinery, but instead passes through the low level
  // xProto (the x11 wire format) that I don't understand.
  //
  // I'm unsure if I have to jump through those hoops, or if XSendEvent is
  // sufficient.

  XSendEvent(xdisplay_, xid, False, 0, xev);
}

}  // namespace views
