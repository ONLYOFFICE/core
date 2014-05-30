// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/root_window_host_x11.h"

#include <strings.h>
#include <X11/cursorfont.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/Xrandr.h>
#include <X11/Xatom.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/Xlib.h>

#include <algorithm>
#include <limits>
#include <string>

#include "base/command_line.h"
#include "base/message_loop.h"
#include "base/message_pump_aurax11.h"
#include "base/stl_util.h"
#include "base/string_number_conversions.h"
#include "base/string_util.h"
#include "base/stringprintf.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/skia/include/core/SkPostConfig.h"
#include "ui/aura/client/capture_client.h"
#include "ui/aura/client/cursor_client.h"
#include "ui/aura/client/screen_position_client.h"
#include "ui/aura/client/user_action_client.h"
#include "ui/aura/env.h"
#include "ui/aura/root_window.h"
#include "ui/base/cursor/cursor.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_utils.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/base/touch/touch_factory_x11.h"
#include "ui/base/ui_base_switches.h"
#include "ui/base/view_prop.h"
#include "ui/base/x/device_list_cache_x.h"
#include "ui/base/x/valuators.h"
#include "ui/base/x/x11_util.h"
#include "ui/compositor/dip_util.h"
#include "ui/compositor/layer.h"
#include "ui/gfx/codec/png_codec.h"
#include "ui/gfx/screen.h"

#if defined(OS_CHROMEOS)
#include "base/chromeos/chromeos_version.h"
#endif

using std::max;
using std::min;

namespace aura {

namespace {

// Standard Linux mouse buttons for going back and forward.
const int kBackMouseButton = 8;
const int kForwardMouseButton = 9;

const char* kAtomsToCache[] = {
  "WM_DELETE_WINDOW",
  "_NET_WM_PING",
  "_NET_WM_PID",
  "WM_S0",
#if defined(OS_CHROMEOS)
  "Tap Paused",  // Defined in the gestures library.
#endif
  NULL
};

::Window FindEventTarget(const base::NativeEvent& xev) {
  ::Window target = xev->xany.window;
  if (xev->type == GenericEvent)
    target = static_cast<XIDeviceEvent*>(xev->xcookie.data)->event;
  return target;
}

void SelectEventsForRootWindow() {
  Display* display = ui::GetXDisplay();
  ::Window root_window = ui::GetX11RootWindow();

  // Receive resize events for the root-window so |x_root_bounds_| can be
  // updated.
  XWindowAttributes attr;
  XGetWindowAttributes(display, root_window, &attr);
  if (!(attr.your_event_mask & StructureNotifyMask)) {
    XSelectInput(display, root_window,
                 StructureNotifyMask | attr.your_event_mask);
  }

  if (!base::MessagePumpForUI::HasXInput2())
    return;

  unsigned char mask[XIMaskLen(XI_LASTEVENT)] = {};
  memset(mask, 0, sizeof(mask));

  XISetMask(mask, XI_HierarchyChanged);
  XISetMask(mask, XI_KeyPress);
  XISetMask(mask, XI_KeyRelease);

  XIEventMask evmask;
  evmask.deviceid = XIAllDevices;
  evmask.mask_len = sizeof(mask);
  evmask.mask = mask;
  XISelectEvents(display, root_window, &evmask, 1);

  // Selecting for touch events seems to fail on some cases (e.g. when logging
  // in incognito). So select for non-touch events first, and then select for
  // touch-events (but keep the other events in the mask, i.e. do not memset
  // |mask| back to 0).
  // TODO(sad): Figure out why this happens. http://crbug.com/153976
#if defined(USE_XI2_MT)
  XISetMask(mask, XI_TouchBegin);
  XISetMask(mask, XI_TouchUpdate);
  XISetMask(mask, XI_TouchEnd);
  XISelectEvents(display, root_window, &evmask, 1);
#endif
}

// We emulate Windows' WM_KEYDOWN and WM_CHAR messages.  WM_CHAR events are only
// generated for certain keys; see
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms646268.aspx.  Per
// discussion on http://crbug.com/108480, char events should furthermore not be
// generated for Tab, Escape, and Backspace.
bool ShouldSendCharEventForKeyboardCode(ui::KeyboardCode keycode) {
  if ((keycode >= ui::VKEY_0 && keycode <= ui::VKEY_9) ||
      (keycode >= ui::VKEY_A && keycode <= ui::VKEY_Z) ||
      (keycode >= ui::VKEY_NUMPAD0 && keycode <= ui::VKEY_NUMPAD9)) {
    return true;
  }

  switch (keycode) {
    case ui::VKEY_RETURN:
    case ui::VKEY_SPACE:
    // In addition to the keys listed at MSDN, we include other
    // graphic-character and numpad keys.
    case ui::VKEY_MULTIPLY:
    case ui::VKEY_ADD:
    case ui::VKEY_SUBTRACT:
    case ui::VKEY_DECIMAL:
    case ui::VKEY_DIVIDE:
    case ui::VKEY_OEM_1:
    case ui::VKEY_OEM_2:
    case ui::VKEY_OEM_3:
    case ui::VKEY_OEM_4:
    case ui::VKEY_OEM_5:
    case ui::VKEY_OEM_6:
    case ui::VKEY_OEM_7:
    case ui::VKEY_OEM_102:
    case ui::VKEY_OEM_PLUS:
    case ui::VKEY_OEM_COMMA:
    case ui::VKEY_OEM_MINUS:
    case ui::VKEY_OEM_PERIOD:
      return true;
    default:
      return false;
  }
}

}  // namespace

namespace internal {

// Accomplishes 2 tasks concerning touch event calibration:
// 1. Being a message-pump observer,
//    routes all the touch events to the X root window,
//    where they can be calibrated later.
// 2. Has the Calibrate method that does the actual bezel calibration,
//    when invoked from X root window's event dispatcher.
class TouchEventCalibrate : public base::MessagePumpObserver {
 public:
  TouchEventCalibrate()
    : left_(0),
      right_(0),
      top_(0),
      bottom_(0) {
    base::MessageLoopForUI::current()->AddObserver(this);
#if defined(USE_XI2_MT)
    std::vector<std::string> parts;
    if (Tokenize(CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
        switches::kTouchCalibration), ",", &parts) >= 4) {
      if (!base::StringToInt(parts[0], &left_))
        DLOG(ERROR) << "Incorrect left border calibration value passed.";
      if (!base::StringToInt(parts[1], &right_))
        DLOG(ERROR) << "Incorrect right border calibration value passed.";
      if (!base::StringToInt(parts[2], &top_))
        DLOG(ERROR) << "Incorrect top border calibration value passed.";
      if (!base::StringToInt(parts[3], &bottom_))
        DLOG(ERROR) << "Incorrect bottom border calibration value passed.";
    }
#endif  // defined(USE_XI2_MT)
  }

  virtual ~TouchEventCalibrate() {
    base::MessageLoopForUI::current()->RemoveObserver(this);
  }

  // Modify the location of the |event|,
  // expanding it from |bounds| to (|bounds| + bezels).
  // Required when touchscreen is bigger than screen (i.e. has bezels),
  // because we receive events in touchscreen coordinates,
  // which need to be expanded when converting to screen coordinates,
  // so that location on bezels will be outside of screen area.
  void Calibrate(ui::TouchEvent* event, const gfx::Rect& bounds) {
#if defined(USE_XI2_MT)
    int x = event->x();
    int y = event->y();

    if (!left_ && !right_ && !top_ && !bottom_)
      return;

    const int resolution_x = bounds.width();
    const int resolution_y = bounds.height();
    // The "grace area" (10% in this case) is to make it easier for the user to
    // navigate to the corner.
    const double kGraceAreaFraction = 0.1;
    if (left_ || right_) {
      // Offset the x position to the real
      x -= left_;
      // Check if we are in the grace area of the left side.
      // Note: We might not want to do this when the gesture is locked?
      if (x < 0 && x > -left_ * kGraceAreaFraction)
        x = 0;
      // Check if we are in the grace area of the right side.
      // Note: We might not want to do this when the gesture is locked?
      if (x > resolution_x - left_ &&
          x < resolution_x - left_ + right_ * kGraceAreaFraction)
        x = resolution_x - left_;
      // Scale the screen area back to the full resolution of the screen.
      x = (x * resolution_x) / (resolution_x - (right_ + left_));
    }
    if (top_ || bottom_) {
      // When there is a top bezel we add our border,
      y -= top_;

      // Check if we are in the grace area of the top side.
      // Note: We might not want to do this when the gesture is locked?
      if (y < 0 && y > -top_ * kGraceAreaFraction)
        y = 0;

      // Check if we are in the grace area of the bottom side.
      // Note: We might not want to do this when the gesture is locked?
      if (y > resolution_y - top_ &&
          y < resolution_y - top_ + bottom_ * kGraceAreaFraction)
        y = resolution_y - top_;
      // Scale the screen area back to the full resolution of the screen.
      y = (y * resolution_y) / (resolution_y - (bottom_ + top_));
    }

    // Set the modified coordinate back to the event.
    if (event->root_location() == event->location()) {
      // Usually those will be equal,
      // if not, I am not sure what the correct value should be.
      event->set_root_location(gfx::Point(x, y));
    }
    event->set_location(gfx::Point(x, y));
#endif  // defined(USE_XI2_MT)
  }

 private:
  // Overridden from base::MessagePumpObserver:
  virtual base::EventStatus WillProcessEvent(
      const base::NativeEvent& event) OVERRIDE {
#if defined(USE_XI2_MT)
    if (event->type == GenericEvent &&
        (event->xgeneric.evtype == XI_TouchBegin ||
         event->xgeneric.evtype == XI_TouchUpdate ||
         event->xgeneric.evtype == XI_TouchEnd)) {
      XIDeviceEvent* xievent = static_cast<XIDeviceEvent*>(event->xcookie.data);
      xievent->event = xievent->root;
      xievent->event_x = xievent->root_x;
      xievent->event_y = xievent->root_y;
    }
#endif  // defined(USE_XI2_MT)
    return base::EVENT_CONTINUE;
  }

  virtual void DidProcessEvent(const base::NativeEvent& event) OVERRIDE {
  }

  // The difference in screen's native resolution pixels between
  // the border of the touchscreen and the border of the screen,
  // aka bezel sizes.
  int left_;
  int right_;
  int top_;
  int bottom_;

  DISALLOW_COPY_AND_ASSIGN(TouchEventCalibrate);
};

}  // namespace internal

////////////////////////////////////////////////////////////////////////////////
// RootWindowHostX11::MouseMoveFilter filters out the move events that
// jump back and forth between two points. This happens when sub pixel mouse
// move is enabled and mouse move events could be jumping between two neighbor
// pixels, e.g. move(0,0), move(1,0), move(0,0), move(1,0) and on and on.
// The filtering is done by keeping track of the last two event locations and
// provides a Filter method to find out whether a mouse event is in a different
// location and should be processed.

class RootWindowHostX11::MouseMoveFilter {
 public:
  MouseMoveFilter() : insert_index_(0) {
    for (size_t i = 0; i < kMaxEvents; ++i) {
      const int int_max = std::numeric_limits<int>::max();
      recent_locations_[i] = gfx::Point(int_max, int_max);
    }
  }
  ~MouseMoveFilter() {}

  // Returns true if |event| is known and should be ignored.
  bool Filter(const base::NativeEvent& event) {
    const gfx::Point& location = ui::EventLocationFromNative(event);
    for (size_t i = 0; i < kMaxEvents; ++i) {
      if (location == recent_locations_[i])
        return true;
    }

    recent_locations_[insert_index_] = location;
    insert_index_ = (insert_index_ + 1) % kMaxEvents;
    return false;
  }

 private:
  static const size_t kMaxEvents = 2;

  gfx::Point recent_locations_[kMaxEvents];
  size_t insert_index_;

  DISALLOW_COPY_AND_ASSIGN(MouseMoveFilter);
};

////////////////////////////////////////////////////////////////////////////////
// RootWindowHostX11

RootWindowHostX11::RootWindowHostX11(const gfx::Rect& bounds)
    : delegate_(NULL),
      xdisplay_(base::MessagePumpAuraX11::GetDefaultXDisplay()),
      xwindow_(0),
      x_root_window_(DefaultRootWindow(xdisplay_)),
      current_cursor_(ui::kCursorNull),
      window_mapped_(false),
      bounds_(bounds),
      is_internal_display_(false),
      focus_when_shown_(false),
      touch_calibrate_(new internal::TouchEventCalibrate),
      mouse_move_filter_(new MouseMoveFilter),
      atom_cache_(xdisplay_, kAtomsToCache) {
  XSetWindowAttributes swa;
  memset(&swa, 0, sizeof(swa));
  swa.background_pixmap = None;
  xwindow_ = XCreateWindow(
      xdisplay_, x_root_window_,
      bounds.x(), bounds.y(), bounds.width(), bounds.height(),
      0,               // border width
      CopyFromParent,  // depth
      InputOutput,
      CopyFromParent,  // visual
      CWBackPixmap,
      &swa);
  base::MessagePumpAuraX11::Current()->AddDispatcherForWindow(this, xwindow_);
  base::MessagePumpAuraX11::Current()->AddDispatcherForRootWindow(this);

  long event_mask = ButtonPressMask | ButtonReleaseMask | FocusChangeMask |
                    KeyPressMask | KeyReleaseMask |
                    EnterWindowMask | LeaveWindowMask |
                    ExposureMask | VisibilityChangeMask |
                    StructureNotifyMask | PropertyChangeMask |
                    PointerMotionMask;
  XSelectInput(xdisplay_, xwindow_, event_mask);
  XFlush(xdisplay_);

  if (base::MessagePumpForUI::HasXInput2())
    ui::TouchFactory::GetInstance()->SetupXI2ForXWindow(xwindow_);

  SelectEventsForRootWindow();

  // Get the initial size of the X root window.
  XWindowAttributes attrs;
  XGetWindowAttributes(xdisplay_, x_root_window_, &attrs);
  x_root_bounds_.SetRect(attrs.x, attrs.y, attrs.width, attrs.height);

  // TODO(erg): We currently only request window deletion events. We also
  // should listen for activation events and anything else that GTK+ listens
  // for, and do something useful.
  ::Atom protocols[2];
  protocols[0] = atom_cache_.GetAtom("WM_DELETE_WINDOW");
  protocols[1] = atom_cache_.GetAtom("_NET_WM_PING");
  XSetWMProtocols(xdisplay_, xwindow_, protocols, 2);

  // We need a WM_CLIENT_MACHINE and WM_LOCALE_NAME value so we integrate with
  // the desktop environment.
  XSetWMProperties(xdisplay_, xwindow_, NULL, NULL, NULL, 0, NULL, NULL, NULL);

  // Likewise, the X server needs to know this window's pid so it knows which
  // program to kill if the window hangs.
  pid_t pid = getpid();
  XChangeProperty(xdisplay_,
                  xwindow_,
                  atom_cache_.GetAtom("_NET_WM_PID"),
                  XA_CARDINAL,
                  32,
                  PropModeReplace,
                  reinterpret_cast<unsigned char*>(&pid), 1);

  // crbug.com/120229 - set the window title so gtalk can find the primary root
  // window to broadcast.
  // TODO(jhorwich) Remove this once Chrome supports window-based broadcasting.
  static int root_window_number = 0;
  std::string name = base::StringPrintf("aura_root_%d", root_window_number++);
  XStoreName(xdisplay_, xwindow_, name.c_str());
  XRRSelectInput(xdisplay_, x_root_window_,
                 RRScreenChangeNotifyMask | RROutputChangeNotifyMask);
  Env::GetInstance()->AddObserver(this);
}

RootWindowHostX11::~RootWindowHostX11() {
  Env::GetInstance()->RemoveObserver(this);
  base::MessagePumpAuraX11::Current()->RemoveDispatcherForRootWindow(this);
  base::MessagePumpAuraX11::Current()->RemoveDispatcherForWindow(xwindow_);

  UnConfineCursor();

  XDestroyWindow(xdisplay_, xwindow_);
}

bool RootWindowHostX11::Dispatch(const base::NativeEvent& event) {
  XEvent* xev = event;

  if (FindEventTarget(event) == x_root_window_)
    return DispatchEventForRootWindow(event);

  switch (xev->type) {
    case EnterNotify: {
      ui::MouseEvent mouseenter_event(xev);
      TranslateAndDispatchMouseEvent(&mouseenter_event);
      break;
    }
    case Expose: {
      gfx::Rect damage_rect(xev->xexpose.x, xev->xexpose.y,
                            xev->xexpose.width, xev->xexpose.height);
      delegate_->AsRootWindow()->ScheduleRedrawRect(damage_rect);
      break;
    }
    case KeyPress: {
      ui::KeyEvent keydown_event(xev, false);
      delegate_->OnHostKeyEvent(&keydown_event);
      break;
    }
    case KeyRelease: {
      ui::KeyEvent keyup_event(xev, false);
      delegate_->OnHostKeyEvent(&keyup_event);
      break;
    }
    case ButtonPress: {
      if (static_cast<int>(xev->xbutton.button) == kBackMouseButton ||
          static_cast<int>(xev->xbutton.button) == kForwardMouseButton) {
        client::UserActionClient* gesture_client =
            client::GetUserActionClient(delegate_->AsRootWindow());
        if (gesture_client) {
          gesture_client->OnUserAction(
              static_cast<int>(xev->xbutton.button) == kBackMouseButton ?
              client::UserActionClient::BACK :
              client::UserActionClient::FORWARD);
        }
        break;
      }
    }  // fallthrough
    case ButtonRelease: {
      ui::MouseEvent mouseev(xev);
      TranslateAndDispatchMouseEvent(&mouseev);
      break;
    }
    case FocusOut:
      if (xev->xfocus.mode != NotifyGrab)
        delegate_->OnHostLostWindowCapture();
      break;
    case ConfigureNotify: {
      DCHECK_EQ(xwindow_, xev->xconfigure.event);
      DCHECK_EQ(xwindow_, xev->xconfigure.window);
      // It's possible that the X window may be resized by some other means
      // than from within aura (e.g. the X window manager can change the
      // size). Make sure the root window size is maintained properly.
      gfx::Rect bounds(xev->xconfigure.x, xev->xconfigure.y,
          xev->xconfigure.width, xev->xconfigure.height);
      bool size_changed = bounds_.size() != bounds.size();
      bool origin_changed = bounds_.origin() != bounds.origin();
      bounds_ = bounds;
      UpdateIsInternalDisplay();
      // Always update barrier and mouse location because |bounds_| might
      // have already been updated in |SetBounds|.
      if (pointer_barriers_) {
        UnConfineCursor();
        ConfineCursorToRootWindow();
      }
      if (size_changed)
        delegate_->OnHostResized(bounds.size());
      if (origin_changed)
        delegate_->OnHostMoved(bounds_.origin());
      break;
    }
    case GenericEvent:
      DispatchXI2Event(event);
      break;
    case MapNotify: {
      // If there's no window manager running, we need to assign the X input
      // focus to our host window.
      if (!IsWindowManagerPresent() && focus_when_shown_)
        XSetInputFocus(xdisplay_, xwindow_, RevertToNone, CurrentTime);
      break;
    }
    case ClientMessage: {
      Atom message_type = static_cast<Atom>(xev->xclient.data.l[0]);
      if (message_type == atom_cache_.GetAtom("WM_DELETE_WINDOW")) {
        // We have received a close message from the window manager.
        delegate_->AsRootWindow()->OnRootWindowHostCloseRequested();
      } else if (message_type == atom_cache_.GetAtom("_NET_WM_PING")) {
        XEvent reply_event = *xev;
        reply_event.xclient.window = x_root_window_;

        XSendEvent(xdisplay_,
                   reply_event.xclient.window,
                   False,
                   SubstructureRedirectMask | SubstructureNotifyMask,
                   &reply_event);
      }
      break;
    }
    case MappingNotify: {
      switch (xev->xmapping.request) {
        case MappingModifier:
        case MappingKeyboard:
          XRefreshKeyboardMapping(&xev->xmapping);
          delegate_->AsRootWindow()->OnKeyboardMappingChanged();
          break;
        case MappingPointer:
          ui::UpdateButtonMap();
          break;
        default:
          NOTIMPLEMENTED() << " Unknown request: " << xev->xmapping.request;
          break;
      }
      break;
    }
    case MotionNotify: {
      // Discard all but the most recent motion event that targets the same
      // window with unchanged state.
      XEvent last_event;
      while (XPending(xev->xany.display)) {
        XEvent next_event;
        XPeekEvent(xev->xany.display, &next_event);
        if (next_event.type == MotionNotify &&
            next_event.xmotion.window == xev->xmotion.window &&
            next_event.xmotion.subwindow == xev->xmotion.subwindow &&
            next_event.xmotion.state == xev->xmotion.state) {
          XNextEvent(xev->xany.display, &last_event);
          xev = &last_event;
        } else {
          break;
        }
      }

      ui::MouseEvent mouseev(xev);
      TranslateAndDispatchMouseEvent(&mouseev);
      break;
    }
  }
  return true;
}

void RootWindowHostX11::SetDelegate(RootWindowHostDelegate* delegate) {
  delegate_ = delegate;
}

RootWindow* RootWindowHostX11::GetRootWindow() {
  return delegate_->AsRootWindow();
}

gfx::AcceleratedWidget RootWindowHostX11::GetAcceleratedWidget() {
  return xwindow_;
}

void RootWindowHostX11::Show() {
  if (!window_mapped_) {
    // Before we map the window, set size hints. Otherwise, some window managers
    // will ignore toplevel XMoveWindow commands.
    XSizeHints size_hints;
    size_hints.flags = PPosition | PWinGravity;
    size_hints.x = bounds_.x();
    size_hints.y = bounds_.y();
    // Set StaticGravity so that the window position is not affected by the
    // frame width when running with window manager.
    size_hints.win_gravity = StaticGravity;
    XSetWMNormalHints(xdisplay_, xwindow_, &size_hints);

    XMapWindow(xdisplay_, xwindow_);

    // We now block until our window is mapped. Some X11 APIs will crash and
    // burn if passed |xwindow_| before the window is mapped, and XMapWindow is
    // asynchronous.
    base::MessagePumpAuraX11::Current()->BlockUntilWindowMapped(xwindow_);
    window_mapped_ = true;
  }
}

void RootWindowHostX11::Hide() {
  if (window_mapped_) {
    XWithdrawWindow(xdisplay_, xwindow_, 0);
    window_mapped_ = false;
  }
}

void RootWindowHostX11::ToggleFullScreen() {
  NOTIMPLEMENTED();
}

gfx::Rect RootWindowHostX11::GetBounds() const {
  return bounds_;
}

void RootWindowHostX11::SetBounds(const gfx::Rect& bounds) {
  // Even if the host window's size doesn't change, aura's root window
  // size, which is in DIP, changes when the scale changes.
  float current_scale = delegate_->GetDeviceScaleFactor();
  float new_scale = gfx::Screen::GetScreenFor(delegate_->AsRootWindow())->
      GetDisplayNearestWindow(delegate_->AsRootWindow()).device_scale_factor();
  bool origin_changed = bounds_.origin() != bounds.origin();
  bool size_changed = bounds_.size() != bounds.size();
  XWindowChanges changes = {0};
  unsigned value_mask = 0;

  if (size_changed) {
    changes.width = bounds.width();
    changes.height = bounds.height();
    value_mask = CWHeight | CWWidth;
  }

  if (origin_changed) {
    changes.x = bounds.x();
    changes.y = bounds.y();
    value_mask |= CWX | CWY;
  }
  if (value_mask)
    XConfigureWindow(xdisplay_, xwindow_, value_mask, &changes);

  // Assume that the resize will go through as requested, which should be the
  // case if we're running without a window manager.  If there's a window
  // manager, it can modify or ignore the request, but (per ICCCM) we'll get a
  // (possibly synthetic) ConfigureNotify about the actual size and correct
  // |bounds_| later.
  bounds_ = bounds;
  UpdateIsInternalDisplay();
  if (origin_changed)
    delegate_->OnHostMoved(bounds.origin());
  if (size_changed || current_scale != new_scale) {
    delegate_->OnHostResized(bounds.size());
  } else {
    delegate_->AsRootWindow()->SchedulePaintInRect(
        delegate_->AsRootWindow()->bounds());
  }
}

gfx::Insets RootWindowHostX11::GetInsets() const {
  return insets_;
}

void RootWindowHostX11::SetInsets(const gfx::Insets& insets) {
  insets_ = insets;
  if (pointer_barriers_) {
    UnConfineCursor();
    ConfineCursorToRootWindow();
  }
}

gfx::Point RootWindowHostX11::GetLocationOnNativeScreen() const {
  return bounds_.origin();
}

void RootWindowHostX11::SetCapture() {
  // TODO(oshima): Grab x input.
}

void RootWindowHostX11::ReleaseCapture() {
  // TODO(oshima): Release x input.
}

void RootWindowHostX11::SetCursor(gfx::NativeCursor cursor) {
  if (cursor == current_cursor_)
    return;
  current_cursor_ = cursor;
  SetCursorInternal(cursor);
}

bool RootWindowHostX11::QueryMouseLocation(gfx::Point* location_return) {
  client::CursorClient* cursor_client =
      client::GetCursorClient(GetRootWindow());
  if (cursor_client && !cursor_client->IsMouseEventsEnabled()) {
    *location_return = gfx::Point(0, 0);
    return false;
  }

  ::Window root_return, child_return;
  int root_x_return, root_y_return, win_x_return, win_y_return;
  unsigned int mask_return;
  XQueryPointer(xdisplay_,
                xwindow_,
                &root_return,
                &child_return,
                &root_x_return, &root_y_return,
                &win_x_return, &win_y_return,
                &mask_return);
  *location_return = gfx::Point(max(0, min(bounds_.width(), win_x_return)),
                                max(0, min(bounds_.height(), win_y_return)));
  return (win_x_return >= 0 && win_x_return < bounds_.width() &&
          win_y_return >= 0 && win_y_return < bounds_.height());
}

bool RootWindowHostX11::ConfineCursorToRootWindow() {
#if XFIXES_MAJOR >= 5
  DCHECK(!pointer_barriers_.get());
  if (pointer_barriers_)
    return false;
  pointer_barriers_.reset(new XID[4]);
  gfx::Rect bounds(bounds_);
  bounds.Inset(insets_);
  // Horizontal, top barriers.
  pointer_barriers_[0] = XFixesCreatePointerBarrier(
      xdisplay_, x_root_window_,
      bounds.x(), bounds.y(), bounds.right(), bounds.y(),
      BarrierPositiveY,
      0, XIAllDevices);
  // Horizontal, bottom barriers.
  pointer_barriers_[1] = XFixesCreatePointerBarrier(
      xdisplay_, x_root_window_,
      bounds.x(), bounds.bottom(), bounds.right(),  bounds.bottom(),
      BarrierNegativeY,
      0, XIAllDevices);
  // Vertical, left  barriers.
  pointer_barriers_[2] = XFixesCreatePointerBarrier(
      xdisplay_, x_root_window_,
      bounds.x(), bounds.y(), bounds.x(), bounds.bottom(),
      BarrierPositiveX,
      0, XIAllDevices);
  // Vertical, right barriers.
  pointer_barriers_[3] = XFixesCreatePointerBarrier(
      xdisplay_, x_root_window_,
      bounds.right(), bounds.y(), bounds.right(), bounds.bottom(),
      BarrierNegativeX,
      0, XIAllDevices);
#endif
  return true;
}

void RootWindowHostX11::UnConfineCursor() {
#if XFIXES_MAJOR >= 5
  if (pointer_barriers_) {
    XFixesDestroyPointerBarrier(xdisplay_, pointer_barriers_[0]);
    XFixesDestroyPointerBarrier(xdisplay_, pointer_barriers_[1]);
    XFixesDestroyPointerBarrier(xdisplay_, pointer_barriers_[2]);
    XFixesDestroyPointerBarrier(xdisplay_, pointer_barriers_[3]);
    pointer_barriers_.reset();
  }
#endif
}

void RootWindowHostX11::OnCursorVisibilityChanged(bool show) {
#if defined(OS_CHROMEOS)
  // Temporarily pause tap-to-click when the cursor is hidden.
  Atom prop = atom_cache_.GetAtom("Tap Paused");
  unsigned char value = !show;
  XIDeviceList dev_list =
      ui::DeviceListCacheX::GetInstance()->GetXI2DeviceList(xdisplay_);

  // Only slave pointer devices could possibly have tap-paused property.
  for (int i = 0; i < dev_list.count; i++) {
    if (dev_list[i].use == XISlavePointer) {
      Atom old_type;
      int old_format;
      unsigned long old_nvalues, bytes;
      unsigned char* data;
      int result = XIGetProperty(xdisplay_, dev_list[i].deviceid, prop, 0, 0,
                                 False, AnyPropertyType, &old_type, &old_format,
                                 &old_nvalues, &bytes, &data);
      if (result != Success)
        continue;
      XFree(data);
      XIChangeProperty(xdisplay_, dev_list[i].deviceid, prop, XA_INTEGER, 8,
                       PropModeReplace, &value, 1);
    }
  }
#endif
}

void RootWindowHostX11::MoveCursorTo(const gfx::Point& location) {
  XWarpPointer(xdisplay_, None, x_root_window_, 0, 0, 0, 0,
               bounds_.x() + location.x(),
               bounds_.y() + location.y());
}

void RootWindowHostX11::SetFocusWhenShown(bool focus_when_shown) {
  static const char* k_NET_WM_USER_TIME = "_NET_WM_USER_TIME";
  focus_when_shown_ = focus_when_shown;
  if (IsWindowManagerPresent() && !focus_when_shown_) {
    ui::SetIntProperty(xwindow_,
                       k_NET_WM_USER_TIME,
                       k_NET_WM_USER_TIME,
                       0);
  }
}

bool RootWindowHostX11::CopyAreaToSkCanvas(const gfx::Rect& source_bounds,
                                             const gfx::Point& dest_offset,
                                             SkCanvas* canvas) {
  scoped_ptr<ui::XScopedImage> scoped_image(GetXImage(source_bounds));
  if (!scoped_image)
    return false;

  XImage* image = scoped_image->get();
  DCHECK(image);

  if (image->bits_per_pixel == 32) {
    if ((0xff << SK_R32_SHIFT) != image->red_mask ||
        (0xff << SK_G32_SHIFT) != image->green_mask ||
        (0xff << SK_B32_SHIFT) != image->blue_mask) {
      LOG(WARNING) << "XImage and Skia byte orders differ";
      return false;
    }

    // Set the alpha channel before copying to the canvas.  Otherwise, areas of
    // the framebuffer that were cleared by ply-image rather than being obscured
    // by an image during boot may end up transparent.
    // TODO(derat|marcheu): Remove this if/when ply-image has been updated to
    // set the framebuffer's alpha channel regardless of whether the device
    // claims to support alpha or not.
    for (int i = 0; i < image->width * image->height * 4; i += 4)
      image->data[i + 3] = 0xff;

    SkBitmap bitmap;
    bitmap.setConfig(SkBitmap::kARGB_8888_Config,
                     image->width, image->height,
                     image->bytes_per_line);
    bitmap.setPixels(image->data);
    canvas->drawBitmap(bitmap, SkIntToScalar(0), SkIntToScalar(0), NULL);
  } else {
    NOTIMPLEMENTED() << "Unsupported bits-per-pixel " << image->bits_per_pixel;
    return false;
  }

  return true;
}

void RootWindowHostX11::PostNativeEvent(
    const base::NativeEvent& native_event) {
  DCHECK(xwindow_);
  DCHECK(xdisplay_);
  XEvent xevent = *native_event;
  xevent.xany.display = xdisplay_;
  xevent.xany.window = xwindow_;

  switch (xevent.type) {
    case EnterNotify:
    case LeaveNotify:
    case MotionNotify:
    case KeyPress:
    case KeyRelease:
    case ButtonPress:
    case ButtonRelease: {
      // The fields used below are in the same place for all of events
      // above. Using xmotion from XEvent's unions to avoid repeating
      // the code.
      xevent.xmotion.root = x_root_window_;
      xevent.xmotion.time = CurrentTime;

      gfx::Point point(xevent.xmotion.x, xevent.xmotion.y);
      delegate_->AsRootWindow()->ConvertPointToNativeScreen(&point);
      xevent.xmotion.x_root = point.x();
      xevent.xmotion.y_root = point.y();
    }
    default:
      break;
  }
  XSendEvent(xdisplay_, xwindow_, False, 0, &xevent);
}

void RootWindowHostX11::OnDeviceScaleFactorChanged(
    float device_scale_factor) {
}

void RootWindowHostX11::PrepareForShutdown() {
  base::MessagePumpAuraX11::Current()->RemoveDispatcherForWindow(xwindow_);
}

void RootWindowHostX11::OnWindowInitialized(Window* window) {
}

void RootWindowHostX11::OnRootWindowInitialized(RootWindow* root_window) {
  // UpdateIsInternalDisplay relies on:
  // 1. delegate_ pointing to RootWindow - available after SetDelegate.
  // 2. RootWindow's kDisplayIdKey property set - available by the time
  //    RootWindow::Init is called.
  //    (set in DisplayManager::CreateRootWindowForDisplay)
  // Ready when NotifyRootWindowInitialized is called from RootWindow::Init.
  if (!delegate_ || root_window != GetRootWindow())
    return;
  UpdateIsInternalDisplay();
}

bool RootWindowHostX11::DispatchEventForRootWindow(
    const base::NativeEvent& event) {
  switch (event->type) {
    case ConfigureNotify:
      DCHECK_EQ(x_root_window_, event->xconfigure.event);
      x_root_bounds_.SetRect(event->xconfigure.x, event->xconfigure.y,
          event->xconfigure.width, event->xconfigure.height);
      break;

    case GenericEvent:
      DispatchXI2Event(event);
      break;
  }

  return true;
}

void RootWindowHostX11::DispatchXI2Event(const base::NativeEvent& event) {
  ui::TouchFactory* factory = ui::TouchFactory::GetInstance();
  XEvent* xev = event;
  if (!factory->ShouldProcessXI2Event(xev))
    return;

  ui::EventType type = ui::EventTypeFromNative(xev);
  XEvent last_event;
  int num_coalesced = 0;

  switch (type) {
    case ui::ET_TOUCH_MOVED:
      num_coalesced = ui::CoalescePendingMotionEvents(xev, &last_event);
      if (num_coalesced > 0)
        xev = &last_event;
      // fallthrough
    case ui::ET_TOUCH_PRESSED:
    case ui::ET_TOUCH_CANCELLED:
    case ui::ET_TOUCH_RELEASED: {
      ui::TouchEvent touchev(xev);
#if defined(OS_CHROMEOS)
      if (base::chromeos::IsRunningOnChromeOS()) {
        if (!bounds_.Contains(touchev.location()))
          break;
        // X maps the touch-surface to the size of the X root-window.
        // In multi-monitor setup, Coordinate Transformation Matrix
        // repositions the touch-surface onto part of X root-window
        // containing aura root-window corresponding to the touchscreen.
        // However, if aura root-window has non-zero origin,
        // we need to relocate the event into aura root-window coordinates.
        touchev.Relocate(bounds_.origin());
#if defined(USE_XI2_MT)
        if (is_internal_display_)
          touch_calibrate_->Calibrate(&touchev, bounds_);
#endif  // defined(USE_XI2_MT)
      }
#endif  // defined(OS_CHROMEOS)
      delegate_->OnHostTouchEvent(&touchev);
      break;
    }
    case ui::ET_MOUSE_MOVED:
    case ui::ET_MOUSE_DRAGGED:
    case ui::ET_MOUSE_PRESSED:
    case ui::ET_MOUSE_RELEASED:
    case ui::ET_MOUSE_ENTERED:
    case ui::ET_MOUSE_EXITED: {
      if (type == ui::ET_MOUSE_MOVED || type == ui::ET_MOUSE_DRAGGED) {
        // If this is a motion event, we want to coalesce all pending motion
        // events that are at the top of the queue.
        num_coalesced = ui::CoalescePendingMotionEvents(xev, &last_event);
        if (num_coalesced > 0)
          xev = &last_event;

        if (mouse_move_filter_ && mouse_move_filter_->Filter(xev))
          break;
      } else if (type == ui::ET_MOUSE_PRESSED ||
                 type == ui::ET_MOUSE_RELEASED) {
        XIDeviceEvent* xievent =
            static_cast<XIDeviceEvent*>(xev->xcookie.data);
        int button = xievent->detail;
        if (button == kBackMouseButton || button == kForwardMouseButton) {
          if (type == ui::ET_MOUSE_RELEASED)
            break;
          client::UserActionClient* gesture_client =
              client::GetUserActionClient(delegate_->AsRootWindow());
          if (gesture_client) {
            bool reverse_direction =
                ui::IsTouchpadEvent(xev) && ui::IsNaturalScrollEnabled();
            gesture_client->OnUserAction(
                (button == kBackMouseButton && !reverse_direction) ||
                (button == kForwardMouseButton && reverse_direction) ?
                client::UserActionClient::BACK :
                client::UserActionClient::FORWARD);
          }
          break;
        }
      }
      ui::MouseEvent mouseev(xev);
      TranslateAndDispatchMouseEvent(&mouseev);
      break;
    }
    case ui::ET_MOUSEWHEEL: {
      ui::MouseWheelEvent mouseev(xev);
      TranslateAndDispatchMouseEvent(&mouseev);
      break;
    }
    case ui::ET_SCROLL_FLING_START:
    case ui::ET_SCROLL_FLING_CANCEL:
    case ui::ET_SCROLL: {
      ui::ScrollEvent scrollev(xev);
      delegate_->OnHostScrollEvent(&scrollev);
      break;
    }
    case ui::ET_UNKNOWN:
      break;
    default:
      NOTREACHED();
  }

  // If we coalesced an event we need to free its cookie.
  if (num_coalesced > 0)
    XFreeEventData(xev->xgeneric.display, &last_event.xcookie);
}

bool RootWindowHostX11::IsWindowManagerPresent() {
  // Per ICCCM 2.8, "Manager Selections", window managers should take ownership
  // of WM_Sn selections (where n is a screen number).
  return XGetSelectionOwner(
      xdisplay_, atom_cache_.GetAtom("WM_S0")) != None;
}

void RootWindowHostX11::SetCursorInternal(gfx::NativeCursor cursor) {
  XDefineCursor(xdisplay_, xwindow_, cursor.platform());
}

void RootWindowHostX11::TranslateAndDispatchMouseEvent(
    ui::MouseEvent* event) {
  RootWindow* root_window = GetRootWindow();
  client::ScreenPositionClient* screen_position_client =
      GetScreenPositionClient(root_window);
  gfx::Rect local(bounds_.size());

  if (screen_position_client && !local.Contains(event->location())) {
    gfx::Point location(event->location());
    // In order to get the correct point in screen coordinates
    // during passive grab, we first need to find on which host window
    // the mouse is on, and find out the screen coordinates on that
    // host window, then convert it back to this host window's coordinate.
    screen_position_client->ConvertHostPointToScreen(root_window, &location);
    screen_position_client->ConvertPointFromScreen(root_window, &location);
    root_window->ConvertPointToHost(&location);
    event->set_location(location);
    event->set_root_location(location);
  }
  delegate_->OnHostMouseEvent(event);
}

scoped_ptr<ui::XScopedImage> RootWindowHostX11::GetXImage(
    const gfx::Rect& snapshot_bounds) {
  scoped_ptr<ui::XScopedImage> image(new ui::XScopedImage(
      XGetImage(xdisplay_, xwindow_,
                snapshot_bounds.x(), snapshot_bounds.y(),
                snapshot_bounds.width(), snapshot_bounds.height(),
                AllPlanes, ZPixmap)));
  if (!image) {
    LOG(ERROR) << "XGetImage failed";
    image.reset();
  }
  return image.Pass();
}

void RootWindowHostX11::UpdateIsInternalDisplay() {
  RootWindow* root_window = GetRootWindow();
  gfx::Screen* screen = gfx::Screen::GetScreenFor(root_window);
  gfx::Display display = screen->GetDisplayNearestWindow(root_window);
  is_internal_display_ = display.IsInternal();
}

// static
RootWindowHost* RootWindowHost::Create(const gfx::Rect& bounds) {
  return new RootWindowHostX11(bounds);
}

// static
gfx::Size RootWindowHost::GetNativeScreenSize() {
  ::Display* xdisplay = base::MessagePumpAuraX11::GetDefaultXDisplay();
  return gfx::Size(DisplayWidth(xdisplay, 0), DisplayHeight(xdisplay, 0));
}

}  // namespace aura
