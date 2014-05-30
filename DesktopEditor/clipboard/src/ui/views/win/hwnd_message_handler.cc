// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/win/hwnd_message_handler.h"

#include <dwmapi.h>
#include <shellapi.h>

#include "base/bind.h"
#include "base/debug/trace_event.h"
#include "base/win/windows_version.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_utils.h"
#include "ui/base/gestures/gesture_sequence.h"
#include "ui/base/keycodes/keyboard_code_conversion_win.h"
#include "ui/base/win/hwnd_util.h"
#include "ui/base/win/mouse_wheel_util.h"
#include "ui/base/win/shell.h"
#include "ui/base/win/touch_input.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/canvas_skia_paint.h"
#include "ui/gfx/icon_util.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/path.h"
#include "ui/gfx/path_win.h"
#include "ui/gfx/screen.h"
#include "ui/native_theme/native_theme_win.h"
#include "ui/views/views_delegate.h"
#include "ui/views/widget/monitor_win.h"
#include "ui/views/widget/native_widget_win.h"
#include "ui/views/widget/widget_hwnd_utils.h"
#include "ui/views/win/fullscreen_handler.h"
#include "ui/views/win/hwnd_message_handler_delegate.h"
#include "ui/views/win/scoped_fullscreen_visibility.h"

#if !defined(USE_AURA)
#include "ui/views/accessibility/native_view_accessibility_win.h"
#include "ui/views/widget/child_window_message_processor.h"
#endif

namespace views {
namespace {

// MoveLoopMouseWatcher is used to determine if the user canceled or completed a
// move. win32 doesn't appear to offer a way to determine the result of a move,
// so we install hooks to determine if we got a mouse up and assume the move
// completed.
class MoveLoopMouseWatcher {
 public:
  explicit MoveLoopMouseWatcher(HWNDMessageHandler* host);
  ~MoveLoopMouseWatcher();

  // Returns true if the mouse is up, or if we couldn't install the hook.
  bool got_mouse_up() const { return got_mouse_up_; }

 private:
  // Instance that owns the hook. We only allow one instance to hook the mouse
  // at a time.
  static MoveLoopMouseWatcher* instance_;

  // Key and mouse callbacks from the hook.
  static LRESULT CALLBACK MouseHook(int n_code, WPARAM w_param, LPARAM l_param);
  static LRESULT CALLBACK KeyHook(int n_code, WPARAM w_param, LPARAM l_param);

  void Unhook();

  // HWNDMessageHandler that created us.
  HWNDMessageHandler* host_;

  // Did we get a mouse up?
  bool got_mouse_up_;

  // Hook identifiers.
  HHOOK mouse_hook_;
  HHOOK key_hook_;

  DISALLOW_COPY_AND_ASSIGN(MoveLoopMouseWatcher);
};

// static
MoveLoopMouseWatcher* MoveLoopMouseWatcher::instance_ = NULL;

MoveLoopMouseWatcher::MoveLoopMouseWatcher(HWNDMessageHandler* host)
    : host_(host),
      got_mouse_up_(false),
      mouse_hook_(NULL),
      key_hook_(NULL) {
  // Only one instance can be active at a time.
  if (instance_)
    instance_->Unhook();

  mouse_hook_ = SetWindowsHookEx(
      WH_MOUSE, &MouseHook, NULL, GetCurrentThreadId());
  if (mouse_hook_) {
    instance_ = this;
    // We don't care if setting the key hook succeeded.
    key_hook_ = SetWindowsHookEx(
        WH_KEYBOARD, &KeyHook, NULL, GetCurrentThreadId());
  }
  if (instance_ != this) {
    // Failed installation. Assume we got a mouse up in this case, otherwise
    // we'll think all drags were canceled.
    got_mouse_up_ = true;
  }
}

MoveLoopMouseWatcher::~MoveLoopMouseWatcher() {
  Unhook();
}

void MoveLoopMouseWatcher::Unhook() {
  if (instance_ != this)
    return;

  DCHECK(mouse_hook_);
  UnhookWindowsHookEx(mouse_hook_);
  if (key_hook_)
    UnhookWindowsHookEx(key_hook_);
  key_hook_ = NULL;
  mouse_hook_ = NULL;
  instance_ = NULL;
}

// static
LRESULT CALLBACK MoveLoopMouseWatcher::MouseHook(int n_code,
                                                 WPARAM w_param,
                                                 LPARAM l_param) {
  DCHECK(instance_);
  if (n_code == HC_ACTION && w_param == WM_LBUTTONUP)
    instance_->got_mouse_up_ = true;
  return CallNextHookEx(instance_->mouse_hook_, n_code, w_param, l_param);
}

// static
LRESULT CALLBACK MoveLoopMouseWatcher::KeyHook(int n_code,
                                               WPARAM w_param,
                                               LPARAM l_param) {
  if (n_code == HC_ACTION && w_param == VK_ESCAPE) {
    if (base::win::GetVersion() >= base::win::VERSION_VISTA) {
      int value = TRUE;
      HRESULT result = DwmSetWindowAttribute(
          instance_->host_->hwnd(),
          DWMWA_TRANSITIONS_FORCEDISABLED,
          &value,
          sizeof(value));
    }
    // Hide the window on escape, otherwise the window is visibly going to snap
    // back to the original location before we close it.
    // This behavior is specific to tab dragging, in that we generally wouldn't
    // want this functionality if we have other consumers using this API.
    instance_->host_->Hide();
  }
  return CallNextHookEx(instance_->key_hook_, n_code, w_param, l_param);
}

// Called from OnNCActivate.
BOOL CALLBACK EnumChildWindowsForRedraw(HWND hwnd, LPARAM lparam) {
  DWORD process_id;
  GetWindowThreadProcessId(hwnd, &process_id);
  int flags = RDW_INVALIDATE | RDW_NOCHILDREN | RDW_FRAME;
  if (process_id == GetCurrentProcessId())
    flags |= RDW_UPDATENOW;
  RedrawWindow(hwnd, NULL, NULL, flags);
  return TRUE;
}

bool GetMonitorAndRects(const RECT& rect,
                        HMONITOR* monitor,
                        gfx::Rect* monitor_rect,
                        gfx::Rect* work_area) {
  DCHECK(monitor);
  DCHECK(monitor_rect);
  DCHECK(work_area);
  *monitor = MonitorFromRect(&rect, MONITOR_DEFAULTTONULL);
  if (!*monitor)
    return false;
  MONITORINFO monitor_info = { 0 };
  monitor_info.cbSize = sizeof(monitor_info);
  GetMonitorInfo(*monitor, &monitor_info);
  *monitor_rect = gfx::Rect(monitor_info.rcMonitor);
  *work_area = gfx::Rect(monitor_info.rcWork);
  return true;
}

struct FindOwnedWindowsData {
  HWND window;
  std::vector<Widget*> owned_widgets;
};

BOOL CALLBACK FindOwnedWindowsCallback(HWND hwnd, LPARAM param) {
  // TODO(beng): resolve wrt aura.
#if !defined(USE_AURA)
  FindOwnedWindowsData* data = reinterpret_cast<FindOwnedWindowsData*>(param);
  if (GetWindow(hwnd, GW_OWNER) == data->window) {
    Widget* widget = Widget::GetWidgetForNativeView(hwnd);
    if (widget)
      data->owned_widgets.push_back(widget);
  }
#endif
  return TRUE;
}

// Enables or disables the menu item for the specified command and menu.
void EnableMenuItemByCommand(HMENU menu, UINT command, bool enabled) {
  UINT flags = MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_DISABLED | MF_GRAYED);
  EnableMenuItem(menu, command, flags);
}

// Callback used to notify child windows that the top level window received a
// DWMCompositionChanged message.
BOOL CALLBACK SendDwmCompositionChanged(HWND window, LPARAM param) {
  SendMessage(window, WM_DWMCOMPOSITIONCHANGED, 0, 0);
  return TRUE;
}

// See comments in OnNCPaint() for details of this struct.
struct ClipState {
  // The window being painted.
  HWND parent;

  // DC painting to.
  HDC dc;

  // Origin of the window in terms of the screen.
  int x;
  int y;
};

// See comments in OnNCPaint() for details of this function.
static BOOL CALLBACK ClipDCToChild(HWND window, LPARAM param) {
  ClipState* clip_state = reinterpret_cast<ClipState*>(param);
  if (GetParent(window) == clip_state->parent && IsWindowVisible(window)) {
    RECT bounds;
    GetWindowRect(window, &bounds);
    ExcludeClipRect(clip_state->dc,
      bounds.left - clip_state->x,
      bounds.top - clip_state->y,
      bounds.right - clip_state->x,
      bounds.bottom - clip_state->y);
  }
  return TRUE;
}

#if !defined(USE_AURA)

// Get the source HWND of the specified message. Depending on the message, the
// source HWND is encoded in either the WPARAM or the LPARAM value.
HWND GetControlHWNDForMessage(UINT message, WPARAM w_param, LPARAM l_param) {
  // Each of the following messages can be sent by a child HWND and must be
  // forwarded to its associated NativeControlWin for handling.
  switch (message) {
    case WM_NOTIFY:
      return reinterpret_cast<NMHDR*>(l_param)->hwndFrom;
    case WM_COMMAND:
      return reinterpret_cast<HWND>(l_param);
    case WM_CONTEXTMENU:
      return reinterpret_cast<HWND>(w_param);
    case WM_CTLCOLORBTN:
    case WM_CTLCOLORSTATIC:
      return reinterpret_cast<HWND>(l_param);
  }
  return NULL;
}

// Some messages may be sent to us by a child HWND. If this is the case, this
// function will forward those messages on to the object associated with the
// source HWND and return true, in which case the window procedure must not do
// any further processing of the message. If there is no associated
// ChildWindowMessageProcessor, the return value will be false and the WndProc
// can continue processing the message normally.  |l_result| contains the result
// of the message processing by the control and must be returned by the WndProc
// if the return value is true.
bool ProcessChildWindowMessage(UINT message,
                               WPARAM w_param,
                               LPARAM l_param,
                               LRESULT* l_result) {
  *l_result = 0;

  HWND control_hwnd = GetControlHWNDForMessage(message, w_param, l_param);
  if (IsWindow(control_hwnd)) {
    ChildWindowMessageProcessor* processor =
        ChildWindowMessageProcessor::Get(control_hwnd);
    if (processor)
      return processor->ProcessMessage(message, w_param, l_param, l_result);
  }

  return false;
}

#endif

// The thickness of an auto-hide taskbar in pixels.
const int kAutoHideTaskbarThicknessPx = 2;

// For windows with the standard frame removed, the client area needs to be
// different from the window area to avoid a "feature" in Windows's handling of
// WM_NCCALCSIZE data. See the comment near the bottom of GetClientAreaInsets
// for more details.
const int kClientAreaBottomInsetHack = -1;

}  // namespace

// A scoping class that prevents a window from being able to redraw in response
// to invalidations that may occur within it for the lifetime of the object.
//
// Why would we want such a thing? Well, it turns out Windows has some
// "unorthodox" behavior when it comes to painting its non-client areas.
// Occasionally, Windows will paint portions of the default non-client area
// right over the top of the custom frame. This is not simply fixed by handling
// WM_NCPAINT/WM_PAINT, with some investigation it turns out that this
// rendering is being done *inside* the default implementation of some message
// handlers and functions:
//  . WM_SETTEXT
//  . WM_SETICON
//  . WM_NCLBUTTONDOWN
//  . EnableMenuItem, called from our WM_INITMENU handler
// The solution is to handle these messages and call DefWindowProc ourselves,
// but prevent the window from being able to update itself for the duration of
// the call. We do this with this class, which automatically calls its
// associated Window's lock and unlock functions as it is created and destroyed.
// See documentation in those methods for the technique used.
//
// The lock only has an effect if the window was visible upon lock creation, as
// it doesn't guard against direct visiblility changes, and multiple locks may
// exist simultaneously to handle certain nested Windows messages.
//
// IMPORTANT: Do not use this scoping object for large scopes or periods of
//            time! IT WILL PREVENT THE WINDOW FROM BEING REDRAWN! (duh).
//
// I would love to hear Raymond Chen's explanation for all this. And maybe a
// list of other messages that this applies to ;-)
class HWNDMessageHandler::ScopedRedrawLock {
 public:
  explicit ScopedRedrawLock(HWNDMessageHandler* owner)
    : owner_(owner),
      hwnd_(owner_->hwnd()),
      was_visible_(owner_->IsVisible()),
      cancel_unlock_(false),
      force_(!(GetWindowLong(hwnd_, GWL_STYLE) & WS_CAPTION)) {
    if (was_visible_ && ::IsWindow(hwnd_))
      owner_->LockUpdates(force_);
  }

  ~ScopedRedrawLock() {
    if (!cancel_unlock_ && was_visible_ && ::IsWindow(hwnd_))
      owner_->UnlockUpdates(force_);
  }

  // Cancel the unlock operation, call this if the Widget is being destroyed.
  void CancelUnlockOperation() { cancel_unlock_ = true; }

 private:
  // The owner having its style changed.
  HWNDMessageHandler* owner_;
  // The owner's HWND, cached to avoid action after window destruction.
  HWND hwnd_;
  // Records the HWND visibility at the time of creation.
  bool was_visible_;
  // A flag indicating that the unlock operation was canceled.
  bool cancel_unlock_;
  // If true, perform the redraw lock regardless of Aero state.
  bool force_;

  DISALLOW_COPY_AND_ASSIGN(ScopedRedrawLock);
};

////////////////////////////////////////////////////////////////////////////////
// HWNDMessageHandler, public:

HWNDMessageHandler::HWNDMessageHandler(HWNDMessageHandlerDelegate* delegate)
    : delegate_(delegate),
      fullscreen_handler_(new FullscreenHandler),
      close_widget_factory_(this),
      remove_standard_frame_(false),
      use_system_default_icon_(false),
      restore_focus_when_enabled_(false),
      restored_enabled_(false),
      previous_cursor_(NULL),
      active_mouse_tracking_flags_(0),
      is_right_mouse_pressed_on_caption_(false),
      lock_updates_count_(0),
      destroyed_(NULL),
      ignore_window_pos_changes_(false),
      ignore_pos_changes_factory_(this),
      last_monitor_(NULL),
      use_layered_buffer_(false),
      layered_alpha_(255),
      paint_layered_window_factory_(this),
      can_update_layered_window_(true),
      is_first_nccalc_(true) {
}

HWNDMessageHandler::~HWNDMessageHandler() {
  delegate_ = NULL;
  if (destroyed_ != NULL)
    *destroyed_ = true;
  // Prevent calls back into this class via WNDPROC now that we've been
  // destroyed.
  ClearUserData();
}

void HWNDMessageHandler::Init(HWND parent, const gfx::Rect& bounds) {
  TRACE_EVENT0("views", "HWNDMessageHandler::Init");
  GetMonitorAndRects(bounds.ToRECT(), &last_monitor_, &last_monitor_rect_,
                     &last_work_area_);

  // Create the window.
  WindowImpl::Init(parent, bounds);
}

void HWNDMessageHandler::InitModalType(ui::ModalType modal_type) {
  if (modal_type == ui::MODAL_TYPE_NONE)
    return;
  // We implement modality by crawling up the hierarchy of windows starting
  // at the owner, disabling all of them so that they don't receive input
  // messages.
  HWND start = ::GetWindow(hwnd(), GW_OWNER);
  while (start) {
    ::EnableWindow(start, FALSE);
    start = ::GetParent(start);
  }
}

void HWNDMessageHandler::Close() {
  if (!IsWindow(hwnd()))
    return;  // No need to do anything.

  // Let's hide ourselves right away.
  Hide();

  // Modal dialog windows disable their owner windows; re-enable them now so
  // they can activate as foreground windows upon this window's destruction.
  RestoreEnabledIfNecessary();

  if (!close_widget_factory_.HasWeakPtrs()) {
    // And we delay the close so that if we are called from an ATL callback,
    // we don't destroy the window before the callback returned (as the caller
    // may delete ourselves on destroy and the ATL callback would still
    // dereference us when the callback returns).
    base::MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(&HWNDMessageHandler::CloseNow,
                   close_widget_factory_.GetWeakPtr()));
  }
}

void HWNDMessageHandler::CloseNow() {
  // We may already have been destroyed if the selection resulted in a tab
  // switch which will have reactivated the browser window and closed us, so
  // we need to check to see if we're still a window before trying to destroy
  // ourself.
  if (IsWindow(hwnd()))
    DestroyWindow(hwnd());
}

gfx::Rect HWNDMessageHandler::GetWindowBoundsInScreen() const {
  RECT r;
  GetWindowRect(hwnd(), &r);
  return gfx::Rect(r);
}

gfx::Rect HWNDMessageHandler::GetClientAreaBoundsInScreen() const {
  RECT r;
  GetClientRect(hwnd(), &r);
  POINT point = { r.left, r.top };
  ClientToScreen(hwnd(), &point);
  return gfx::Rect(point.x, point.y, r.right - r.left, r.bottom - r.top);
}

gfx::Rect HWNDMessageHandler::GetRestoredBounds() const {
  // If we're in fullscreen mode, we've changed the normal bounds to the monitor
  // rect, so return the saved bounds instead.
  if (fullscreen_handler_->fullscreen())
    return fullscreen_handler_->GetRestoreBounds();

  gfx::Rect bounds;
  GetWindowPlacement(&bounds, NULL);
  return bounds;
}

void HWNDMessageHandler::GetWindowPlacement(
    gfx::Rect* bounds,
    ui::WindowShowState* show_state) const {
  WINDOWPLACEMENT wp;
  wp.length = sizeof(wp);
  const bool succeeded = !!::GetWindowPlacement(hwnd(), &wp);
  DCHECK(succeeded);

  if (bounds != NULL) {
    if (wp.showCmd == SW_SHOWNORMAL) {
      // GetWindowPlacement can return misleading position if a normalized
      // window was resized using Aero Snap feature (see comment 9 in bug
      // 36421). As a workaround, using GetWindowRect for normalized windows.
      const bool succeeded = GetWindowRect(hwnd(), &wp.rcNormalPosition) != 0;
      DCHECK(succeeded);

      *bounds = gfx::Rect(wp.rcNormalPosition);
    } else {
      MONITORINFO mi;
      mi.cbSize = sizeof(mi);
      const bool succeeded = GetMonitorInfo(
          MonitorFromWindow(hwnd(), MONITOR_DEFAULTTONEAREST), &mi) != 0;
      DCHECK(succeeded);

      *bounds = gfx::Rect(wp.rcNormalPosition);
      // Convert normal position from workarea coordinates to screen
      // coordinates.
      bounds->Offset(mi.rcWork.left - mi.rcMonitor.left,
                     mi.rcWork.top - mi.rcMonitor.top);
    }
  }

  if (show_state) {
    if (wp.showCmd == SW_SHOWMAXIMIZED)
      *show_state = ui::SHOW_STATE_MAXIMIZED;
    else if (wp.showCmd == SW_SHOWMINIMIZED)
      *show_state = ui::SHOW_STATE_MINIMIZED;
    else
      *show_state = ui::SHOW_STATE_NORMAL;
  }
}

void HWNDMessageHandler::SetBounds(const gfx::Rect& bounds) {
  LONG style = GetWindowLong(hwnd(), GWL_STYLE);
  if (style & WS_MAXIMIZE)
    SetWindowLong(hwnd(), GWL_STYLE, style & ~WS_MAXIMIZE);
  SetWindowPos(hwnd(), NULL, bounds.x(), bounds.y(), bounds.width(),
               bounds.height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

void HWNDMessageHandler::SetSize(const gfx::Size& size) {
  SetWindowPos(hwnd(), NULL, 0, 0, size.width(), size.height(),
               SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
}

void HWNDMessageHandler::CenterWindow(const gfx::Size& size) {
  HWND parent = GetParent(hwnd());
  if (!IsWindow(hwnd()))
    parent = ::GetWindow(hwnd(), GW_OWNER);
  ui::CenterAndSizeWindow(parent, hwnd(), size);
}

void HWNDMessageHandler::SetRegion(HRGN region) {
  SetWindowRgn(hwnd(), region, TRUE);
}

void HWNDMessageHandler::StackAbove(HWND other_hwnd) {
  SetWindowPos(hwnd(), other_hwnd, 0, 0, 0, 0,
               SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
}

void HWNDMessageHandler::StackAtTop() {
  SetWindowPos(hwnd(), HWND_TOP, 0, 0, 0, 0,
               SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);
}

void HWNDMessageHandler::Show() {
  if (IsWindow(hwnd()))
    ShowWindowWithState(ui::SHOW_STATE_INACTIVE);
}

void HWNDMessageHandler::ShowWindowWithState(ui::WindowShowState show_state) {
  TRACE_EVENT0("views", "HWNDMessageHandler::ShowWindowWithState");
  DWORD native_show_state;
  switch (show_state) {
    case ui::SHOW_STATE_INACTIVE:
      native_show_state = SW_SHOWNOACTIVATE;
      break;
    case ui::SHOW_STATE_MAXIMIZED:
      native_show_state = SW_SHOWMAXIMIZED;
      break;
    case ui::SHOW_STATE_MINIMIZED:
      native_show_state = SW_SHOWMINIMIZED;
      break;
    default:
      native_show_state = delegate_->GetInitialShowState();
      break;
  }
  Show(native_show_state);
}

void HWNDMessageHandler::Show(int show_state) {
  ShowWindow(hwnd(), show_state);
  // When launched from certain programs like bash and Windows Live Messenger,
  // show_state is set to SW_HIDE, so we need to correct that condition. We
  // don't just change show_state to SW_SHOWNORMAL because MSDN says we must
  // always first call ShowWindow with the specified value from STARTUPINFO,
  // otherwise all future ShowWindow calls will be ignored (!!#@@#!). Instead,
  // we call ShowWindow again in this case.
  if (show_state == SW_HIDE) {
    show_state = SW_SHOWNORMAL;
    ShowWindow(hwnd(), show_state);
  }

  // We need to explicitly activate the window if we've been shown with a state
  // that should activate, because if we're opened from a desktop shortcut while
  // an existing window is already running it doesn't seem to be enough to use
  // one of these flags to activate the window.
  if (show_state == SW_SHOWNORMAL || show_state == SW_SHOWMAXIMIZED)
    Activate();

  if (!delegate_->HandleInitialFocus())
    SetInitialFocus();
}

void HWNDMessageHandler::ShowMaximizedWithBounds(const gfx::Rect& bounds) {
  WINDOWPLACEMENT placement = { 0 };
  placement.length = sizeof(WINDOWPLACEMENT);
  placement.showCmd = SW_SHOWMAXIMIZED;
  placement.rcNormalPosition = bounds.ToRECT();
  SetWindowPlacement(hwnd(), &placement);
}

void HWNDMessageHandler::Hide() {
  if (IsWindow(hwnd())) {
    // NOTE: Be careful not to activate any windows here (for example, calling
    // ShowWindow(SW_HIDE) will automatically activate another window).  This
    // code can be called while a window is being deactivated, and activating
    // another window will screw up the activation that is already in progress.
    SetWindowPos(hwnd(), NULL, 0, 0, 0, 0,
                 SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOMOVE |
                 SWP_NOREPOSITION | SWP_NOSIZE | SWP_NOZORDER);

    if (!GetParent(hwnd()))
      NotifyOwnedWindowsParentClosing();
  }
}

void HWNDMessageHandler::Maximize() {
  ExecuteSystemMenuCommand(SC_MAXIMIZE);
}

void HWNDMessageHandler::Minimize() {
  ExecuteSystemMenuCommand(SC_MINIMIZE);
  delegate_->HandleNativeBlur(NULL);
}

void HWNDMessageHandler::Restore() {
  ExecuteSystemMenuCommand(SC_RESTORE);
}

void HWNDMessageHandler::Activate() {
  if (IsMinimized())
    ::ShowWindow(hwnd(), SW_RESTORE);
  ::SetWindowPos(hwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
  SetForegroundWindow(hwnd());
}

void HWNDMessageHandler::Deactivate() {
  HWND next_hwnd = ::GetNextWindow(hwnd(), GW_HWNDNEXT);
  if (next_hwnd)
    ::SetForegroundWindow(next_hwnd);
}

void HWNDMessageHandler::SetAlwaysOnTop(bool on_top) {
  ::SetWindowPos(hwnd(), on_top ? HWND_TOPMOST : HWND_NOTOPMOST,
                 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

bool HWNDMessageHandler::IsVisible() const {
  return !!::IsWindowVisible(hwnd());
}

bool HWNDMessageHandler::IsActive() const {
  return GetActiveWindow() == hwnd();
}

bool HWNDMessageHandler::IsMinimized() const {
  return !!::IsIconic(hwnd());
}

bool HWNDMessageHandler::IsMaximized() const {
  return !!::IsZoomed(hwnd());
}

bool HWNDMessageHandler::RunMoveLoop(const gfx::Vector2d& drag_offset) {
  ReleaseCapture();
  MoveLoopMouseWatcher watcher(this);
  SendMessage(hwnd(), WM_SYSCOMMAND, SC_MOVE | 0x0002, GetMessagePos());
  // Windows doesn't appear to offer a way to determine whether the user
  // canceled the move or not. We assume if the user released the mouse it was
  // successful.
  return watcher.got_mouse_up();
}

void HWNDMessageHandler::EndMoveLoop() {
  SendMessage(hwnd(), WM_CANCELMODE, 0, 0);
}

void HWNDMessageHandler::SendFrameChanged() {
  SetWindowPos(hwnd(), NULL, 0, 0, 0, 0,
      SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOCOPYBITS |
      SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREPOSITION |
      SWP_NOSENDCHANGING | SWP_NOSIZE | SWP_NOZORDER);
}

void HWNDMessageHandler::FlashFrame(bool flash) {
  FLASHWINFO fwi;
  fwi.cbSize = sizeof(fwi);
  fwi.hwnd = hwnd();
  if (flash) {
    fwi.dwFlags = FLASHW_ALL;
    fwi.uCount = 4;
    fwi.dwTimeout = 0;
  } else {
    fwi.dwFlags = FLASHW_STOP;
  }
  FlashWindowEx(&fwi);
}

void HWNDMessageHandler::ClearNativeFocus() {
  ::SetFocus(hwnd());
}

void HWNDMessageHandler::SetCapture() {
  DCHECK(!HasCapture());
  ::SetCapture(hwnd());
}

void HWNDMessageHandler::ReleaseCapture() {
  if (HasCapture())
    ::ReleaseCapture();
}

bool HWNDMessageHandler::HasCapture() const {
  return ::GetCapture() == hwnd();
}

void HWNDMessageHandler::SetVisibilityChangedAnimationsEnabled(bool enabled) {
  if (base::win::GetVersion() >= base::win::VERSION_VISTA) {
    int dwm_value = enabled ? FALSE : TRUE;
    DwmSetWindowAttribute(
        hwnd(), DWMWA_TRANSITIONS_FORCEDISABLED, &dwm_value, sizeof(dwm_value));
  }
}

void HWNDMessageHandler::SetTitle(const string16& title) {
  SetWindowText(hwnd(), title.c_str());
}

void HWNDMessageHandler::SetCursor(HCURSOR cursor) {
  if (cursor) {
    previous_cursor_ = ::SetCursor(cursor);
  } else if (previous_cursor_) {
    ::SetCursor(previous_cursor_);
    previous_cursor_ = NULL;
  }
}

void HWNDMessageHandler::FrameTypeChanged() {
  // Called when the frame type could possibly be changing (theme change or
  // DWM composition change).
  if (base::win::GetVersion() >= base::win::VERSION_VISTA) {
    // We need to toggle the rendering policy of the DWM/glass frame as we
    // change from opaque to glass. "Non client rendering enabled" means that
    // the DWM's glass non-client rendering is enabled, which is why
    // DWMNCRP_ENABLED is used for the native frame case. _DISABLED means the
    // DWM doesn't render glass, and so is used in the custom frame case.
    DWMNCRENDERINGPOLICY policy = !delegate_->IsUsingCustomFrame() ?
        DWMNCRP_ENABLED : DWMNCRP_DISABLED;
    DwmSetWindowAttribute(hwnd(), DWMWA_NCRENDERING_POLICY,
                          &policy, sizeof(DWMNCRENDERINGPOLICY));
  }

  ResetWindowRegion(true);

  // The non-client view needs to update too.
  delegate_->HandleFrameChanged();

  // WM_DWMCOMPOSITIONCHANGED is only sent to top level windows, however we want
  // to notify our children too, since we can have MDI child windows who need to
  // update their appearance.
  EnumChildWindows(hwnd(), &SendDwmCompositionChanged, NULL);
}

void HWNDMessageHandler::SchedulePaintInRect(const gfx::Rect& rect) {
  if (use_layered_buffer_) {
    // We must update the back-buffer immediately, since Windows' handling of
    // invalid rects is somewhat mysterious.
    invalid_rect_.Union(rect);

    // In some situations, such as drag and drop, when Windows itself runs a
    // nested message loop our message loop appears to be starved and we don't
    // receive calls to DidProcessMessage(). This only seems to affect layered
    // windows, so we schedule a redraw manually using a task, since those never
    // seem to be starved. Also, wtf.
    if (!paint_layered_window_factory_.HasWeakPtrs()) {
      base::MessageLoop::current()->PostTask(
          FROM_HERE,
          base::Bind(&HWNDMessageHandler::RedrawLayeredWindowContents,
                     paint_layered_window_factory_.GetWeakPtr()));
    }
  } else {
    // InvalidateRect() expects client coordinates.
    RECT r = rect.ToRECT();
    InvalidateRect(hwnd(), &r, FALSE);
  }
}

void HWNDMessageHandler::SetOpacity(BYTE opacity) {
  layered_alpha_ = opacity;
}

void HWNDMessageHandler::SetWindowIcons(const gfx::ImageSkia& window_icon,
                                        const gfx::ImageSkia& app_icon) {
  if (!window_icon.isNull()) {
    HICON windows_icon = IconUtil::CreateHICONFromSkBitmap(
        *window_icon.bitmap());
    // We need to make sure to destroy the previous icon, otherwise we'll leak
    // these GDI objects until we crash!
    HICON old_icon = reinterpret_cast<HICON>(
        SendMessage(hwnd(), WM_SETICON, ICON_SMALL,
                    reinterpret_cast<LPARAM>(windows_icon)));
    if (old_icon)
      DestroyIcon(old_icon);
  }
  if (!app_icon.isNull()) {
    HICON windows_icon = IconUtil::CreateHICONFromSkBitmap(*app_icon.bitmap());
    HICON old_icon = reinterpret_cast<HICON>(
        SendMessage(hwnd(), WM_SETICON, ICON_BIG,
                    reinterpret_cast<LPARAM>(windows_icon)));
    if (old_icon)
      DestroyIcon(old_icon);
  }
}

////////////////////////////////////////////////////////////////////////////////
// HWNDMessageHandler, InputMethodDelegate implementation:

void HWNDMessageHandler::DispatchKeyEventPostIME(const ui::KeyEvent& key) {
  SetMsgHandled(delegate_->HandleKeyEvent(key));
}

////////////////////////////////////////////////////////////////////////////////
// HWNDMessageHandler, ui::WindowImpl overrides:

HICON HWNDMessageHandler::GetDefaultWindowIcon() const {
  if (use_system_default_icon_)
    return NULL;
  return ViewsDelegate::views_delegate ?
      ViewsDelegate::views_delegate->GetDefaultWindowIcon() : NULL;
}

LRESULT HWNDMessageHandler::OnWndProc(UINT message,
                                      WPARAM w_param,
                                      LPARAM l_param) {
  HWND window = hwnd();
  LRESULT result = 0;

  if (delegate_ && delegate_->PreHandleMSG(message, w_param, l_param, &result))
    return result;

#if !defined(USE_AURA)
  // First allow messages sent by child controls to be processed directly by
  // their associated views. If such a view is present, it will handle the
  // message *instead of* this NativeWidgetWin.
  if (ProcessChildWindowMessage(message, w_param, l_param, &result))
    return result;
#endif

  // Otherwise we handle everything else.
  if (!ProcessWindowMessage(window, message, w_param, l_param, result))
    result = DefWindowProc(window, message, w_param, l_param);

  // DefWindowProc() may have destroyed the window in a nested message loop.
  if (!::IsWindow(window))
    return result;

  if (delegate_)
    delegate_->PostHandleMSG(message, w_param, l_param);
  if (message == WM_NCDESTROY) {
    base::MessageLoopForUI::current()->RemoveObserver(this);
    if (delegate_)
      delegate_->HandleDestroyed();
  }

  // Only top level widget should store/restore focus.
  if (message == WM_ACTIVATE && delegate_->CanSaveFocus())
    PostProcessActivateMessage(LOWORD(w_param));
  if (message == WM_ENABLE && restore_focus_when_enabled_) {
    // This path should be executed only for top level as
    // restore_focus_when_enabled_ is set in PostProcessActivateMessage.
    DCHECK(delegate_->CanSaveFocus());
    restore_focus_when_enabled_ = false;
    delegate_->RestoreFocusOnEnable();
  }
  return result;
}

////////////////////////////////////////////////////////////////////////////////
// HWNDMessageHandler, MessageLoopForUI::Observer implementation:

base::EventStatus HWNDMessageHandler::WillProcessEvent(
      const base::NativeEvent& event) {
  return base::EVENT_CONTINUE;
}

void HWNDMessageHandler::DidProcessEvent(const base::NativeEvent& event) {
  RedrawInvalidRect();
}

////////////////////////////////////////////////////////////////////////////////
// HWNDMessageHandler, private:

void HWNDMessageHandler::SetInitialFocus() {
  if (!(GetWindowLong(hwnd(), GWL_EXSTYLE) & WS_EX_TRANSPARENT) &&
      !(GetWindowLong(hwnd(), GWL_EXSTYLE) & WS_EX_NOACTIVATE)) {
    // The window does not get keyboard messages unless we focus it.
    SetFocus(hwnd());
  }
}

void HWNDMessageHandler::PostProcessActivateMessage(int activation_state) {
  DCHECK(delegate_->CanSaveFocus());
  if (WA_INACTIVE == activation_state) {
    // We might get activated/inactivated without being enabled, so we need to
    // clear restore_focus_when_enabled_.
    restore_focus_when_enabled_ = false;
    delegate_->SaveFocusOnDeactivate();
  } else {
    // We must restore the focus after the message has been DefProc'ed as it
    // does set the focus to the last focused HWND.
    // Note that if the window is not enabled, we cannot restore the focus as
    // calling ::SetFocus on a child of the non-enabled top-window would fail.
    // This is the case when showing a modal dialog (such as 'open file',
    // 'print'...) from a different thread.
    // In that case we delay the focus restoration to when the window is enabled
    // again.
    if (!IsWindowEnabled(hwnd())) {
      DCHECK(!restore_focus_when_enabled_);
      restore_focus_when_enabled_ = true;
      return;
    }
    delegate_->RestoreFocusOnActivate();
  }
}

void HWNDMessageHandler::RestoreEnabledIfNecessary() {
  if (delegate_->IsModal() && !restored_enabled_) {
    restored_enabled_ = true;
    // If we were run modally, we need to undo the disabled-ness we inflicted on
    // the owner's parent hierarchy.
    HWND start = ::GetWindow(hwnd(), GW_OWNER);
    while (start) {
      ::EnableWindow(start, TRUE);
      start = ::GetParent(start);
    }
  }
}

void HWNDMessageHandler::ExecuteSystemMenuCommand(int command) {
  if (command)
    SendMessage(hwnd(), WM_SYSCOMMAND, command, 0);
}

void HWNDMessageHandler::TrackMouseEvents(DWORD mouse_tracking_flags) {
  // Begin tracking mouse events for this HWND so that we get WM_MOUSELEAVE
  // when the user moves the mouse outside this HWND's bounds.
  if (active_mouse_tracking_flags_ == 0 || mouse_tracking_flags & TME_CANCEL) {
    if (mouse_tracking_flags & TME_CANCEL) {
      // We're about to cancel active mouse tracking, so empty out the stored
      // state.
      active_mouse_tracking_flags_ = 0;
    } else {
      active_mouse_tracking_flags_ = mouse_tracking_flags;
    }

    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.dwFlags = mouse_tracking_flags;
    tme.hwndTrack = hwnd();
    tme.dwHoverTime = 0;
    TrackMouseEvent(&tme);
  } else if (mouse_tracking_flags != active_mouse_tracking_flags_) {
    TrackMouseEvents(active_mouse_tracking_flags_ | TME_CANCEL);
    TrackMouseEvents(mouse_tracking_flags);
  }
}

void HWNDMessageHandler::ClientAreaSizeChanged() {
  RECT r = {0, 0, 0, 0};
  // In case of minimized window GetWindowRect can return normally unexpected
  // coordinates.
  if (!IsMinimized()) {
    if (delegate_->WidgetSizeIsClientSize()) {
      GetClientRect(hwnd(), &r);
      // This is needed due to a hack that works around a "feature" in
      // Windows's handling of WM_NCCALCSIZE. See the comment near the end of
      // GetClientAreaInsets for more details.
      if (remove_standard_frame_ && !IsMaximized())
        r.bottom += kClientAreaBottomInsetHack;
    } else {
      GetWindowRect(hwnd(), &r);
    }
  }
  gfx::Size s(std::max(0, static_cast<int>(r.right - r.left)),
              std::max(0, static_cast<int>(r.bottom - r.top)));
  delegate_->HandleClientSizeChanged(s);
  if (use_layered_buffer_) {
    layered_window_contents_.reset(
        new gfx::Canvas(s, ui::SCALE_FACTOR_100P, false));
  }
}

gfx::Insets HWNDMessageHandler::GetClientAreaInsets() const {
  gfx::Insets insets;
  if (delegate_->GetClientAreaInsets(&insets))
    return insets;
  DCHECK(insets.empty());

  // Returning an empty Insets object causes the default handling in
  // NativeWidgetWin::OnNCCalcSize() to be invoked.
  if (!delegate_->IsWidgetWindow() ||
      (!delegate_->IsUsingCustomFrame() && !remove_standard_frame_)) {
    return insets;
  }

  if (IsMaximized()) {
    // Windows automatically adds a standard width border to all sides when a
    // window is maximized.
    int border_thickness = GetSystemMetrics(SM_CXSIZEFRAME);
    if (remove_standard_frame_)
      border_thickness -= 1;
    return gfx::Insets(border_thickness, border_thickness, border_thickness,
                       border_thickness);
  }

  // Returning empty insets for a window with the standard frame removed seems
  // to cause Windows to treat the window specially, treating black as
  // transparent and changing around some of the painting logic. I suspect it's
  // some sort of horrible backwards-compatability hack, but the upshot of it
  // is that if the insets are empty then in certain conditions (it seems to
  // be subtly related to timing), the contents of windows with the standard
  // frame removed will flicker to transparent during resize.
  //
  // To work around this, we increase the size of the client area by 1px
  // *beyond* the bottom of the window. This prevents Windows from having a
  // hissy fit and flashing the window incessantly during resizes, but it also
  // means that the client area is reported 1px larger than it really is, so
  // user code has to compensate by making its content shorter if it wants
  // everything to appear inside the window.
  if (remove_standard_frame_)
    return gfx::Insets(0, 0, IsMaximized() ? 0 : kClientAreaBottomInsetHack, 0);
  // This is weird, but highly essential. If we don't offset the bottom edge
  // of the client rect, the window client area and window area will match,
  // and when returning to glass rendering mode from non-glass, the client
  // area will not paint black as transparent. This is because (and I don't
  // know why) the client area goes from matching the window rect to being
  // something else. If the client area is not the window rect in both
  // modes, the blackness doesn't occur. Because of this, we need to tell
  // the RootView to lay out to fit the window rect, rather than the client
  // rect when using the opaque frame.
  // Note: this is only required for non-fullscreen windows. Note that
  // fullscreen windows are in restored state, not maximized.
  return gfx::Insets(0, 0, fullscreen_handler_->fullscreen() ? 0 : 1, 0);
}

void HWNDMessageHandler::ResetWindowRegion(bool force) {
  // A native frame uses the native window region, and we don't want to mess
  // with it.
  if (!delegate_->IsUsingCustomFrame() || !delegate_->IsWidgetWindow()) {
    if (force)
      SetWindowRgn(hwnd(), NULL, TRUE);
    return;
  }

  // Changing the window region is going to force a paint. Only change the
  // window region if the region really differs.
  HRGN current_rgn = CreateRectRgn(0, 0, 0, 0);
  int current_rgn_result = GetWindowRgn(hwnd(), current_rgn);

  CRect window_rect;
  GetWindowRect(hwnd(), &window_rect);
  HRGN new_region;
  if (IsMaximized()) {
    HMONITOR monitor = MonitorFromWindow(hwnd(), MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi;
    mi.cbSize = sizeof mi;
    GetMonitorInfo(monitor, &mi);
    CRect work_rect = mi.rcWork;
    work_rect.OffsetRect(-window_rect.left, -window_rect.top);
    new_region = CreateRectRgnIndirect(&work_rect);
  } else {
    gfx::Path window_mask;
    delegate_->GetWindowMask(
        gfx::Size(window_rect.Width(), window_rect.Height()), &window_mask);
    new_region = gfx::CreateHRGNFromSkPath(window_mask);
  }

  if (current_rgn_result == ERROR || !EqualRgn(current_rgn, new_region)) {
    // SetWindowRgn takes ownership of the HRGN created by CreateNativeRegion.
    SetWindowRgn(hwnd(), new_region, TRUE);
  } else {
    DeleteObject(new_region);
  }

  DeleteObject(current_rgn);
}

LRESULT HWNDMessageHandler::DefWindowProcWithRedrawLock(UINT message,
                                                        WPARAM w_param,
                                                        LPARAM l_param) {
  ScopedRedrawLock lock(this);
  // The Widget and HWND can be destroyed in the call to DefWindowProc, so use
  // the |destroyed_| flag to avoid unlocking (and crashing) after destruction.
  bool destroyed = false;
  destroyed_ = &destroyed;
  LRESULT result = DefWindowProc(hwnd(), message, w_param, l_param);
  if (destroyed)
    lock.CancelUnlockOperation();
  else
    destroyed_ = NULL;
  return result;
}

void HWNDMessageHandler::NotifyOwnedWindowsParentClosing() {
  FindOwnedWindowsData data;
  data.window = hwnd();
  EnumThreadWindows(GetCurrentThreadId(), FindOwnedWindowsCallback,
                    reinterpret_cast<LPARAM>(&data));
  for (size_t i = 0; i < data.owned_widgets.size(); ++i)
    data.owned_widgets[i]->OnOwnerClosing();
}

void HWNDMessageHandler::LockUpdates(bool force) {
  // We skip locked updates when Aero is on for two reasons:
  // 1. Because it isn't necessary
  // 2. Because toggling the WS_VISIBLE flag may occur while the GPU process is
  //    attempting to present a child window's backbuffer onscreen. When these
  //    two actions race with one another, the child window will either flicker
  //    or will simply stop updating entirely.
  if ((force || !ui::win::IsAeroGlassEnabled()) && ++lock_updates_count_ == 1) {
    SetWindowLong(hwnd(), GWL_STYLE,
                  GetWindowLong(hwnd(), GWL_STYLE) & ~WS_VISIBLE);
  }
}

void HWNDMessageHandler::UnlockUpdates(bool force) {
  if ((force || !ui::win::IsAeroGlassEnabled()) && --lock_updates_count_ <= 0) {
    SetWindowLong(hwnd(), GWL_STYLE,
                  GetWindowLong(hwnd(), GWL_STYLE) | WS_VISIBLE);
    lock_updates_count_ = 0;
  }
}

void HWNDMessageHandler::RedrawInvalidRect() {
  if (!use_layered_buffer_) {
    RECT r = { 0, 0, 0, 0 };
    if (GetUpdateRect(hwnd(), &r, FALSE) && !IsRectEmpty(&r)) {
      RedrawWindow(hwnd(), &r, NULL,
                   RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOCHILDREN);
    }
  }
}

void HWNDMessageHandler::RedrawLayeredWindowContents() {
  if (invalid_rect_.IsEmpty())
    return;

  // We need to clip to the dirty rect ourselves.
  layered_window_contents_->sk_canvas()->save(SkCanvas::kClip_SaveFlag);
  layered_window_contents_->ClipRect(invalid_rect_);
  delegate_->PaintLayeredWindow(layered_window_contents_.get());
  layered_window_contents_->sk_canvas()->restore();

  RECT wr;
  GetWindowRect(hwnd(), &wr);
  SIZE size = {wr.right - wr.left, wr.bottom - wr.top};
  POINT position = {wr.left, wr.top};
  HDC dib_dc = skia::BeginPlatformPaint(layered_window_contents_->sk_canvas());
  POINT zero = {0, 0};
  BLENDFUNCTION blend = {AC_SRC_OVER, 0, layered_alpha_, AC_SRC_ALPHA};
  UpdateLayeredWindow(hwnd(), NULL, &position, &size, dib_dc, &zero,
                      RGB(0xFF, 0xFF, 0xFF), &blend, ULW_ALPHA);
  invalid_rect_.SetRect(0, 0, 0, 0);
  skia::EndPlatformPaint(layered_window_contents_->sk_canvas());
}

// Message handlers ------------------------------------------------------------

void HWNDMessageHandler::OnActivateApp(BOOL active, DWORD thread_id) {
  if (delegate_->IsWidgetWindow() && !active &&
      thread_id != GetCurrentThreadId()) {
    delegate_->HandleAppDeactivated();
    // Also update the native frame if it is rendering the non-client area.
    if (!remove_standard_frame_ && !delegate_->IsUsingCustomFrame())
      DefWindowProcWithRedrawLock(WM_NCACTIVATE, FALSE, 0);
  }
}

BOOL HWNDMessageHandler::OnAppCommand(HWND window,
                                      short command,
                                      WORD device,
                                      int keystate) {
  BOOL handled = !!delegate_->HandleAppCommand(command);
  SetMsgHandled(handled);
  // Make sure to return TRUE if the event was handled or in some cases the
  // system will execute the default handler which can cause bugs like going
  // forward or back two pages instead of one.
  return handled;
}

void HWNDMessageHandler::OnCancelMode() {
  delegate_->HandleCancelMode();
  // Need default handling, otherwise capture and other things aren't canceled.
  SetMsgHandled(FALSE);
}

void HWNDMessageHandler::OnCaptureChanged(HWND window) {
  delegate_->HandleCaptureLost();
}

void HWNDMessageHandler::OnClose() {
  delegate_->HandleClose();
}

void HWNDMessageHandler::OnCommand(UINT notification_code,
                                   int command,
                                   HWND window) {
  // If the notification code is > 1 it means it is control specific and we
  // should ignore it.
  if (notification_code > 1 || delegate_->HandleAppCommand(command))
    SetMsgHandled(FALSE);
}

LRESULT HWNDMessageHandler::OnCreate(CREATESTRUCT* create_struct) {
  use_layered_buffer_ = !!(window_ex_style() & WS_EX_LAYERED);

#if defined(USE_AURA)
  if (window_ex_style() &  WS_EX_COMPOSITED) {
    if (base::win::GetVersion() >= base::win::VERSION_VISTA) {
      // This is part of the magic to emulate layered windows with Aura
      // see the explanation elsewere when we set WS_EX_COMPOSITED style.
      MARGINS margins = {-1,-1,-1,-1};
      DwmExtendFrameIntoClientArea(hwnd(), &margins);
    }
  }
#endif

  fullscreen_handler_->set_hwnd(hwnd());

  // This message initializes the window so that focus border are shown for
  // windows.
  SendMessage(hwnd(),
              WM_CHANGEUISTATE,
              MAKELPARAM(UIS_CLEAR, UISF_HIDEFOCUS),
              0);

  // Bug 964884: detach the IME attached to this window.
  // We should attach IMEs only when we need to input CJK strings.
  ImmAssociateContextEx(hwnd(), NULL, 0);

  if (remove_standard_frame_) {
    SetWindowLong(hwnd(), GWL_STYLE,
                  GetWindowLong(hwnd(), GWL_STYLE) & ~WS_CAPTION);
    SendFrameChanged();
  }

  // Get access to a modifiable copy of the system menu.
  GetSystemMenu(hwnd(), false);

  if (base::win::GetVersion() >= base::win::VERSION_WIN7)
    RegisterTouchWindow(hwnd(), 0);

  // We need to allow the delegate to size its contents since the window may not
  // receive a size notification when its initial bounds are specified at window
  // creation time.
  ClientAreaSizeChanged();

  // We need to add ourselves as a message loop observer so that we can repaint
  // aggressively if the contents of our window become invalid. Unfortunately
  // WM_PAINT messages are starved and we get flickery redrawing when resizing
  // if we do not do this.
  base::MessageLoopForUI::current()->AddObserver(this);

  delegate_->HandleCreate();

  // TODO(beng): move more of NWW::OnCreate here.
  return 0;
}

void HWNDMessageHandler::OnDestroy() {
  delegate_->HandleDestroying();
}

void HWNDMessageHandler::OnDisplayChange(UINT bits_per_pixel,
                                         const CSize& screen_size) {
  delegate_->HandleDisplayChange();
}

LRESULT HWNDMessageHandler::OnDwmCompositionChanged(UINT msg,
                                                    WPARAM w_param,
                                                    LPARAM l_param) {
  if (!delegate_->IsWidgetWindow()) {
    SetMsgHandled(FALSE);
    return 0;
  }
  // For some reason, we need to hide the window while we're changing the frame
  // type only when we're changing it in response to WM_DWMCOMPOSITIONCHANGED.
  // If we don't, the client area will be filled with black. I'm suspecting
  // something skia-ey.
  // Frame type toggling caused by the user (e.g. switching theme) doesn't seem
  // to have this requirement.
  FrameTypeChanged();
  return 0;
}

void HWNDMessageHandler::OnEnterSizeMove() {
  delegate_->HandleBeginWMSizeMove();
  SetMsgHandled(FALSE);
}

LRESULT HWNDMessageHandler::OnEraseBkgnd(HDC dc) {
  // Needed to prevent resize flicker.
  return 1;
}

void HWNDMessageHandler::OnExitSizeMove() {
  delegate_->HandleEndWMSizeMove();
  SetMsgHandled(FALSE);
}

void HWNDMessageHandler::OnGetMinMaxInfo(MINMAXINFO* minmax_info) {
  gfx::Size min_window_size;
  gfx::Size max_window_size;
  delegate_->GetMinMaxSize(&min_window_size, &max_window_size);

  // Add the native frame border size to the minimum and maximum size if the
  // view reports its size as the client size.
  if (delegate_->WidgetSizeIsClientSize()) {
    CRect client_rect, window_rect;
    GetClientRect(hwnd(), &client_rect);
    GetWindowRect(hwnd(), &window_rect);
    // Due to the client area bottom inset hack (detailed elsewhere), adjust
    // the reported size of the client area in the case that the standard frame
    // has been removed.
    if (remove_standard_frame_)
      client_rect.bottom += kClientAreaBottomInsetHack;
    window_rect -= client_rect;
    min_window_size.Enlarge(window_rect.Width(), window_rect.Height());
    if (!max_window_size.IsEmpty())
      max_window_size.Enlarge(window_rect.Width(), window_rect.Height());
  }
  minmax_info->ptMinTrackSize.x = min_window_size.width();
  minmax_info->ptMinTrackSize.y = min_window_size.height();
  if (max_window_size.width() || max_window_size.height()) {
    if (!max_window_size.width())
      max_window_size.set_width(GetSystemMetrics(SM_CXMAXTRACK));
    if (!max_window_size.height())
      max_window_size.set_height(GetSystemMetrics(SM_CYMAXTRACK));
    minmax_info->ptMaxTrackSize.x = max_window_size.width();
    minmax_info->ptMaxTrackSize.y = max_window_size.height();
  }
  SetMsgHandled(FALSE);
}

LRESULT HWNDMessageHandler::OnGetObject(UINT message,
                                        WPARAM w_param,
                                        LPARAM l_param) {
  LRESULT reference_result = static_cast<LRESULT>(0L);

  // Accessibility readers will send an OBJID_CLIENT message
  if (OBJID_CLIENT == l_param) {
    // Retrieve MSAA dispatch object for the root view.
    base::win::ScopedComPtr<IAccessible> root(
        delegate_->GetNativeViewAccessible());

    // Create a reference that MSAA will marshall to the client.
    reference_result = LresultFromObject(IID_IAccessible, w_param,
        static_cast<IAccessible*>(root.Detach()));
  }

  return reference_result;
}

LRESULT HWNDMessageHandler::OnImeMessages(UINT message,
                                          WPARAM w_param,
                                          LPARAM l_param) {
  LRESULT result = 0;
  SetMsgHandled(delegate_->HandleIMEMessage(
      message, w_param, l_param, &result));
  return result;
}

void HWNDMessageHandler::OnInitMenu(HMENU menu) {
  bool is_fullscreen = fullscreen_handler_->fullscreen();
  bool is_minimized = IsMinimized();
  bool is_maximized = IsMaximized();
  bool is_restored = !is_fullscreen && !is_minimized && !is_maximized;

  ScopedRedrawLock lock(this);
  EnableMenuItemByCommand(menu, SC_RESTORE, is_minimized || is_maximized);
  EnableMenuItemByCommand(menu, SC_MOVE, is_restored);
  EnableMenuItemByCommand(menu, SC_SIZE, delegate_->CanResize() && is_restored);
  EnableMenuItemByCommand(menu, SC_MAXIMIZE, delegate_->CanMaximize() &&
                          !is_fullscreen && !is_maximized);
  EnableMenuItemByCommand(menu, SC_MINIMIZE, delegate_->CanMaximize() &&
                          !is_minimized);
}

void HWNDMessageHandler::OnInputLangChange(DWORD character_set,
                                           HKL input_language_id) {
  delegate_->HandleInputLanguageChange(character_set, input_language_id);
}

LRESULT HWNDMessageHandler::OnKeyEvent(UINT message,
                                       WPARAM w_param,
                                       LPARAM l_param) {
  MSG msg = { hwnd(), message, w_param, l_param, GetMessageTime() };
  ui::KeyEvent key(msg, message == WM_CHAR);
  if (!delegate_->HandleUntranslatedKeyEvent(key))
    DispatchKeyEventPostIME(key);
  return 0;
}

void HWNDMessageHandler::OnKillFocus(HWND focused_window) {
  delegate_->HandleNativeBlur(focused_window);
  SetMsgHandled(FALSE);
}

LRESULT HWNDMessageHandler::OnMouseActivate(UINT message,
                                            WPARAM w_param,
                                            LPARAM l_param) {
  // TODO(beng): resolve this with the GetWindowLong() check on the subsequent
  //             line.
  if (delegate_->IsWidgetWindow())
    return delegate_->CanActivate() ? MA_ACTIVATE : MA_NOACTIVATEANDEAT;
  if (GetWindowLong(hwnd(), GWL_EXSTYLE) & WS_EX_NOACTIVATE)
    return MA_NOACTIVATE;
  SetMsgHandled(FALSE);
  return MA_ACTIVATE;
}

LRESULT HWNDMessageHandler::OnMouseRange(UINT message,
                                         WPARAM w_param,
                                         LPARAM l_param) {
#if defined(USE_AURA)
  // We handle touch events on Windows Aura. Ignore synthesized mouse messages
  // from Windows.
  if (!touch_ids_.empty() || ui::IsMouseEventFromTouch(message))
    return 0;
#endif
  if (message == WM_RBUTTONUP && is_right_mouse_pressed_on_caption_) {
    is_right_mouse_pressed_on_caption_ = false;
    ReleaseCapture();
    // |point| is in window coordinates, but WM_NCHITTEST and TrackPopupMenu()
    // expect screen coordinates.
    CPoint screen_point(l_param);
    MapWindowPoints(hwnd(), HWND_DESKTOP, &screen_point, 1);
    w_param = SendMessage(hwnd(), WM_NCHITTEST, 0,
                          MAKELPARAM(screen_point.x, screen_point.y));
    if (w_param == HTCAPTION || w_param == HTSYSMENU) {
      ui::ShowSystemMenuAtPoint(hwnd(), gfx::Point(screen_point));
      return 0;
    }
  } else if (message == WM_NCLBUTTONDOWN && delegate_->IsUsingCustomFrame()) {
    switch (w_param) {
      case HTCLOSE:
      case HTMINBUTTON:
      case HTMAXBUTTON: {
        // When the mouse is pressed down in these specific non-client areas,
        // we need to tell the RootView to send the mouse pressed event (which
        // sets capture, allowing subsequent WM_LBUTTONUP (note, _not_
        // WM_NCLBUTTONUP) to fire so that the appropriate WM_SYSCOMMAND can be
        // sent by the applicable button's ButtonListener. We _have_ to do this
        // way rather than letting Windows just send the syscommand itself (as
        // would happen if we never did this dance) because for some insane
        // reason DefWindowProc for WM_NCLBUTTONDOWN also renders the pressed
        // window control button appearance, in the Windows classic style, over
        // our view! Ick! By handling this message we prevent Windows from
        // doing this undesirable thing, but that means we need to roll the
        // sys-command handling ourselves.
        // Combine |w_param| with common key state message flags.
        w_param |= base::win::IsCtrlPressed() ? MK_CONTROL : 0;
        w_param |= base::win::IsShiftPressed() ? MK_SHIFT : 0;
      }
    }
  } else if (message == WM_NCRBUTTONDOWN &&
      (w_param == HTCAPTION || w_param == HTSYSMENU)) {
    is_right_mouse_pressed_on_caption_ = true;
    // We SetCapture() to ensure we only show the menu when the button
    // down and up are both on the caption. Note: this causes the button up to
    // be WM_RBUTTONUP instead of WM_NCRBUTTONUP.
    SetCapture();
  }

  MSG msg = { hwnd(), message, w_param, l_param, GetMessageTime(),
              { GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param) } };
  ui::MouseEvent event(msg);
  if (!touch_ids_.empty() || ui::IsMouseEventFromTouch(message))
    event.set_flags(event.flags() | ui::EF_FROM_TOUCH);

  if (!(event.flags() & ui::EF_IS_NON_CLIENT))
    delegate_->HandleTooltipMouseMove(message, w_param, l_param);

  if (event.type() == ui::ET_MOUSE_MOVED && !HasCapture()) {
    // Windows only fires WM_MOUSELEAVE events if the application begins
    // "tracking" mouse events for a given HWND during WM_MOUSEMOVE events.
    // We need to call |TrackMouseEvents| to listen for WM_MOUSELEAVE.
    TrackMouseEvents((message == WM_NCMOUSEMOVE) ?
        TME_NONCLIENT | TME_LEAVE : TME_LEAVE);
  } else if (event.type() == ui::ET_MOUSE_EXITED) {
    // Reset our tracking flags so future mouse movement over this
    // NativeWidgetWin results in a new tracking session. Fall through for
    // OnMouseEvent.
    active_mouse_tracking_flags_ = 0;
  } else if (event.type() == ui::ET_MOUSEWHEEL) {
    // Reroute the mouse wheel to the window under the pointer if applicable.
    return (ui::RerouteMouseWheel(hwnd(), w_param, l_param) ||
            delegate_->HandleMouseEvent(ui::MouseWheelEvent(msg))) ? 0 : 1;
  }

  bool handled = delegate_->HandleMouseEvent(event);
  if (!handled && message == WM_NCLBUTTONDOWN && w_param != HTSYSMENU &&
      delegate_->IsUsingCustomFrame()) {
    // TODO(msw): Eliminate undesired painting, or re-evaluate this workaround.
    // DefWindowProc for WM_NCLBUTTONDOWN does weird non-client painting, so we
    // need to call it inside a ScopedRedrawLock. This may cause other negative
    // side-effects (ex/ stifling non-client mouse releases).
    DefWindowProcWithRedrawLock(message, w_param, l_param);
    handled = true;
  }

  SetMsgHandled(handled);
  return 0;
}

void HWNDMessageHandler::OnMove(const CPoint& point) {
  delegate_->HandleMove();
  SetMsgHandled(FALSE);
}

void HWNDMessageHandler::OnMoving(UINT param, const RECT* new_bounds) {
  delegate_->HandleMove();
}

LRESULT HWNDMessageHandler::OnNCActivate(BOOL active) {
  if (delegate_->CanActivate())
    delegate_->HandleActivationChanged(!!active);

  if (!delegate_->IsWidgetWindow()) {
    SetMsgHandled(FALSE);
    return 0;
  }

  if (!delegate_->CanActivate())
    return TRUE;

  // On activation, lift any prior restriction against rendering as inactive.
  bool inactive_rendering_disabled = delegate_->IsInactiveRenderingDisabled();
  if (active && inactive_rendering_disabled)
    delegate_->EnableInactiveRendering();

  if (delegate_->IsUsingCustomFrame()) {
    // TODO(beng, et al): Hack to redraw this window and child windows
    //     synchronously upon activation. Not all child windows are redrawing
    //     themselves leading to issues like http://crbug.com/74604
    //     We redraw out-of-process HWNDs asynchronously to avoid hanging the
    //     whole app if a child HWND belonging to a hung plugin is encountered.
    RedrawWindow(hwnd(), NULL, NULL,
                 RDW_NOCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW);
    EnumChildWindows(hwnd(), EnumChildWindowsForRedraw, NULL);
  }

  // The frame may need to redraw as a result of the activation change.
  // We can get WM_NCACTIVATE before we're actually visible. If we're not
  // visible, no need to paint.
  if (IsVisible())
    delegate_->SchedulePaint();

  // Avoid DefWindowProc non-client rendering over our custom frame on newer
  // Windows versions only (breaks taskbar activation indication on XP/Vista).
  if (delegate_->IsUsingCustomFrame() &&
      base::win::GetVersion() > base::win::VERSION_VISTA) {
    SetMsgHandled(TRUE);
    return TRUE;
  }

  return DefWindowProcWithRedrawLock(
      WM_NCACTIVATE, inactive_rendering_disabled || active, 0);
}

LRESULT HWNDMessageHandler::OnNCCalcSize(BOOL mode, LPARAM l_param) {
  // We only override the default handling if we need to specify a custom
  // non-client edge width. Note that in most cases "no insets" means no
  // custom width, but in fullscreen mode or when the NonClientFrameView
  // requests it, we want a custom width of 0.

  // Let User32 handle the first nccalcsize for captioned windows
  // so it updates its internal structures (specifically caption-present)
  // Without this Tile & Cascade windows won't work.
  // See http://code.google.com/p/chromium/issues/detail?id=900
  if (is_first_nccalc_) {
    is_first_nccalc_ = false;
    if (GetWindowLong(hwnd(), GWL_STYLE) & WS_CAPTION) {
      SetMsgHandled(FALSE);
      return 0;
    }
  }

  gfx::Insets insets = GetClientAreaInsets();
  if (insets.empty() && !fullscreen_handler_->fullscreen() &&
      !(mode && remove_standard_frame_)) {
    SetMsgHandled(FALSE);
    return 0;
  }

  RECT* client_rect = mode ?
      &(reinterpret_cast<NCCALCSIZE_PARAMS*>(l_param)->rgrc[0]) :
      reinterpret_cast<RECT*>(l_param);
  client_rect->left += insets.left();
  client_rect->top += insets.top();
  client_rect->bottom -= insets.bottom();
  client_rect->right -= insets.right();
  if (IsMaximized()) {
    // Find all auto-hide taskbars along the screen edges and adjust in by the
    // thickness of the auto-hide taskbar on each such edge, so the window isn't
    // treated as a "fullscreen app", which would cause the taskbars to
    // disappear.
    HMONITOR monitor = MonitorFromWindow(hwnd(), MONITOR_DEFAULTTONULL);
    if (!monitor) {
      // We might end up here if the window was previously minimized and the
      // user clicks on the taskbar button to restore it in the previously
      // maximized position. In that case WM_NCCALCSIZE is sent before the
      // window coordinates are restored to their previous values, so our
      // (left,top) would probably be (-32000,-32000) like all minimized
      // windows. So the above MonitorFromWindow call fails, but if we check
      // the window rect given with WM_NCCALCSIZE (which is our previous
      // restored window position) we will get the correct monitor handle.
      monitor = MonitorFromRect(client_rect, MONITOR_DEFAULTTONULL);
      if (!monitor) {
        // This is probably an extreme case that we won't hit, but if we don't
        // intersect any monitor, let us not adjust the client rect since our
        // window will not be visible anyway.
        return 0;
      }
    }
    if (GetTopmostAutoHideTaskbarForEdge(ABE_LEFT, monitor))
      client_rect->left += kAutoHideTaskbarThicknessPx;
    if (GetTopmostAutoHideTaskbarForEdge(ABE_TOP, monitor)) {
      if (!delegate_->IsUsingCustomFrame()) {
        // Tricky bit.  Due to a bug in DwmDefWindowProc()'s handling of
        // WM_NCHITTEST, having any nonclient area atop the window causes the
        // caption buttons to draw onscreen but not respond to mouse
        // hover/clicks.
        // So for a taskbar at the screen top, we can't push the
        // client_rect->top down; instead, we move the bottom up by one pixel,
        // which is the smallest change we can make and still get a client area
        // less than the screen size. This is visibly ugly, but there seems to
        // be no better solution.
        --client_rect->bottom;
      } else {
        client_rect->top += kAutoHideTaskbarThicknessPx;
      }
    }
    if (GetTopmostAutoHideTaskbarForEdge(ABE_RIGHT, monitor))
      client_rect->right -= kAutoHideTaskbarThicknessPx;
    if (GetTopmostAutoHideTaskbarForEdge(ABE_BOTTOM, monitor))
      client_rect->bottom -= kAutoHideTaskbarThicknessPx;

    // We cannot return WVR_REDRAW when there is nonclient area, or Windows
    // exhibits bugs where client pixels and child HWNDs are mispositioned by
    // the width/height of the upper-left nonclient area.
    return 0;
  }

  // If the window bounds change, we're going to relayout and repaint anyway.
  // Returning WVR_REDRAW avoids an extra paint before that of the old client
  // pixels in the (now wrong) location, and thus makes actions like resizing a
  // window from the left edge look slightly less broken.
  // We special case when left or top insets are 0, since these conditions
  // actually require another repaint to correct the layout after glass gets
  // turned on and off.
  if (insets.left() == 0 || insets.top() == 0)
    return 0;
  return mode ? WVR_REDRAW : 0;
}

LRESULT HWNDMessageHandler::OnNCHitTest(const CPoint& point) {
  if (!delegate_->IsWidgetWindow()) {
    SetMsgHandled(FALSE);
    return 0;
  }

  // If the DWM is rendering the window controls, we need to give the DWM's
  // default window procedure first chance to handle hit testing.
  if (!remove_standard_frame_ && !delegate_->IsUsingCustomFrame()) {
    LRESULT result;
    if (DwmDefWindowProc(hwnd(), WM_NCHITTEST, 0,
                         MAKELPARAM(point.x, point.y), &result)) {
      return result;
    }
  }

  // First, give the NonClientView a chance to test the point to see if it
  // provides any of the non-client area.
  POINT temp = point;
  MapWindowPoints(HWND_DESKTOP, hwnd(), &temp, 1);
  int component = delegate_->GetNonClientComponent(gfx::Point(temp));
  if (component != HTNOWHERE)
    return component;

  // Otherwise, we let Windows do all the native frame non-client handling for
  // us.
  SetMsgHandled(FALSE);
  return 0;
}

void HWNDMessageHandler::OnNCPaint(HRGN rgn) {
  // We only do non-client painting if we're not using the native frame.
  // It's required to avoid some native painting artifacts from appearing when
  // the window is resized.
  if (!delegate_->IsWidgetWindow() || !delegate_->IsUsingCustomFrame()) {
    SetMsgHandled(FALSE);
    return;
  }

  // We have an NC region and need to paint it. We expand the NC region to
  // include the dirty region of the root view. This is done to minimize
  // paints.
  CRect window_rect;
  GetWindowRect(hwnd(), &window_rect);

  gfx::Size root_view_size = delegate_->GetRootViewSize();
  if (gfx::Size(window_rect.Width(), window_rect.Height()) != root_view_size) {
    // If the size of the window differs from the size of the root view it
    // means we're being asked to paint before we've gotten a WM_SIZE. This can
    // happen when the user is interactively resizing the window. To avoid
    // mass flickering we don't do anything here. Once we get the WM_SIZE we'll
    // reset the region of the window which triggers another WM_NCPAINT and
    // all is well.
    return;
  }

  CRect dirty_region;
  // A value of 1 indicates paint all.
  if (!rgn || rgn == reinterpret_cast<HRGN>(1)) {
    dirty_region = CRect(0, 0, window_rect.Width(), window_rect.Height());
  } else {
    RECT rgn_bounding_box;
    GetRgnBox(rgn, &rgn_bounding_box);
    if (!IntersectRect(&dirty_region, &rgn_bounding_box, &window_rect))
      return;  // Dirty region doesn't intersect window bounds, bale.

    // rgn_bounding_box is in screen coordinates. Map it to window coordinates.
    OffsetRect(&dirty_region, -window_rect.left, -window_rect.top);
  }

  // In theory GetDCEx should do what we want, but I couldn't get it to work.
  // In particular the docs mentiond DCX_CLIPCHILDREN, but as far as I can tell
  // it doesn't work at all. So, instead we get the DC for the window then
  // manually clip out the children.
  HDC dc = GetWindowDC(hwnd());
  ClipState clip_state;
  clip_state.x = window_rect.left;
  clip_state.y = window_rect.top;
  clip_state.parent = hwnd();
  clip_state.dc = dc;
  EnumChildWindows(hwnd(), &ClipDCToChild,
                   reinterpret_cast<LPARAM>(&clip_state));

  gfx::Rect old_paint_region = invalid_rect_;
  if (!old_paint_region.IsEmpty()) {
    // The root view has a region that needs to be painted. Include it in the
    // region we're going to paint.

    CRect old_paint_region_crect = old_paint_region.ToRECT();
    CRect tmp = dirty_region;
    UnionRect(&dirty_region, &tmp, &old_paint_region_crect);
  }

  SchedulePaintInRect(gfx::Rect(dirty_region));

  // gfx::CanvasSkiaPaint's destructor does the actual painting. As such, wrap
  // the following in a block to force paint to occur so that we can release
  // the dc.
  if (!delegate_->HandlePaintAccelerated(gfx::Rect(dirty_region))) {
    gfx::CanvasSkiaPaint canvas(dc, true, dirty_region.left,
                                dirty_region.top, dirty_region.Width(),
                                dirty_region.Height());
    delegate_->HandlePaint(&canvas);
  }

  ReleaseDC(hwnd(), dc);
  // When using a custom frame, we want to avoid calling DefWindowProc() since
  // that may render artifacts.
  SetMsgHandled(delegate_->IsUsingCustomFrame());
}

LRESULT HWNDMessageHandler::OnNCUAHDrawCaption(UINT message,
                                               WPARAM w_param,
                                               LPARAM l_param) {
  // See comment in widget_win.h at the definition of WM_NCUAHDRAWCAPTION for
  // an explanation about why we need to handle this message.
  SetMsgHandled(delegate_->IsUsingCustomFrame());
  return 0;
}

LRESULT HWNDMessageHandler::OnNCUAHDrawFrame(UINT message,
                                             WPARAM w_param,
                                             LPARAM l_param) {
  // See comment in widget_win.h at the definition of WM_NCUAHDRAWCAPTION for
  // an explanation about why we need to handle this message.
  SetMsgHandled(delegate_->IsUsingCustomFrame());
  return 0;
}

LRESULT HWNDMessageHandler::OnNotify(int w_param, NMHDR* l_param) {
  LRESULT l_result = 0;
  SetMsgHandled(delegate_->HandleTooltipNotify(w_param, l_param, &l_result));
  return l_result;
}

void HWNDMessageHandler::OnPaint(HDC dc) {
  RECT dirty_rect;
  // Try to paint accelerated first.
  if (GetUpdateRect(hwnd(), &dirty_rect, FALSE) &&
      !IsRectEmpty(&dirty_rect)) {
    if (delegate_->HandlePaintAccelerated(gfx::Rect(dirty_rect))) {
      ValidateRect(hwnd(), NULL);
    } else {
#if defined(USE_AURA)
      delegate_->HandlePaint(NULL);
#else
      scoped_ptr<gfx::CanvasPaint> canvas(
          gfx::CanvasPaint::CreateCanvasPaint(hwnd()));
      delegate_->HandlePaint(canvas->AsCanvas());
#endif
    }
  } else {
    // TODO(msw): Find a better solution for this crbug.com/93530 workaround.
    // Some scenarios otherwise fail to validate minimized app/popup windows.
    ValidateRect(hwnd(), NULL);
  }
}

LRESULT HWNDMessageHandler::OnReflectedMessage(UINT message,
                                               WPARAM w_param,
                                               LPARAM l_param) {
  SetMsgHandled(FALSE);
  return 0;
}

LRESULT HWNDMessageHandler::OnSetCursor(UINT message,
                                        WPARAM w_param,
                                        LPARAM l_param) {
  // Reimplement the necessary default behavior here. Calling DefWindowProc can
  // trigger weird non-client painting for non-glass windows with custom frames.
  // Using a ScopedRedrawLock to prevent caption rendering artifacts may allow
  // content behind this window to incorrectly paint in front of this window.
  // Invalidating the window to paint over either set of artifacts is not ideal.
  wchar_t* cursor = IDC_ARROW;
  switch (LOWORD(l_param)) {
    case HTSIZE:
      cursor = IDC_SIZENWSE;
      break;
    case HTLEFT:
    case HTRIGHT:
      cursor = IDC_SIZEWE;
      break;
    case HTTOP:
    case HTBOTTOM:
      cursor = IDC_SIZENS;
      break;
    case HTTOPLEFT:
    case HTBOTTOMRIGHT:
      cursor = IDC_SIZENWSE;
      break;
    case HTTOPRIGHT:
    case HTBOTTOMLEFT:
      cursor = IDC_SIZENESW;
      break;
    case HTCLIENT:
      // Client-area mouse events set the proper cursor from View::GetCursor.
      return 0;
    default:
      // Use the default value, IDC_ARROW.
      break;
  }
  SetCursor(LoadCursor(NULL, cursor));
  return 0;
}

void HWNDMessageHandler::OnSetFocus(HWND last_focused_window) {
  delegate_->HandleNativeFocus(last_focused_window);
  SetMsgHandled(FALSE);
}

LRESULT HWNDMessageHandler::OnSetIcon(UINT size_type, HICON new_icon) {
  // Use a ScopedRedrawLock to avoid weird non-client painting.
  return DefWindowProcWithRedrawLock(WM_SETICON, size_type,
                                     reinterpret_cast<LPARAM>(new_icon));
}

LRESULT HWNDMessageHandler::OnSetText(const wchar_t* text) {
  // Use a ScopedRedrawLock to avoid weird non-client painting.
  return DefWindowProcWithRedrawLock(WM_SETTEXT, NULL,
                                     reinterpret_cast<LPARAM>(text));
}

void HWNDMessageHandler::OnSettingChange(UINT flags, const wchar_t* section) {
  if (!GetParent(hwnd()) && (flags == SPI_SETWORKAREA) &&
      !delegate_->WillProcessWorkAreaChange()) {
    // Fire a dummy SetWindowPos() call, so we'll trip the code in
    // OnWindowPosChanging() below that notices work area changes.
    ::SetWindowPos(hwnd(), 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE |
        SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
    SetMsgHandled(TRUE);
  } else {
    if (flags == SPI_SETWORKAREA)
      delegate_->HandleWorkAreaChanged();
    SetMsgHandled(FALSE);
  }
}

void HWNDMessageHandler::OnSize(UINT param, const CSize& size) {
  RedrawWindow(hwnd(), NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
  // ResetWindowRegion is going to trigger WM_NCPAINT. By doing it after we've
  // invoked OnSize we ensure the RootView has been laid out.
  ResetWindowRegion(false);
}

void HWNDMessageHandler::OnSysCommand(UINT notification_code,
                                      const CPoint& point) {
  if (!delegate_->ShouldHandleSystemCommands())
    return;

  // Windows uses the 4 lower order bits of |notification_code| for type-
  // specific information so we must exclude this when comparing.
  static const int sc_mask = 0xFFF0;
  // Ignore size/move/maximize in fullscreen mode.
  if (fullscreen_handler_->fullscreen() &&
      (((notification_code & sc_mask) == SC_SIZE) ||
       ((notification_code & sc_mask) == SC_MOVE) ||
       ((notification_code & sc_mask) == SC_MAXIMIZE)))
    return;
  if (delegate_->IsUsingCustomFrame()) {
    if ((notification_code & sc_mask) == SC_MINIMIZE ||
        (notification_code & sc_mask) == SC_MAXIMIZE ||
        (notification_code & sc_mask) == SC_RESTORE) {
      delegate_->ResetWindowControls();
    } else if ((notification_code & sc_mask) == SC_MOVE ||
               (notification_code & sc_mask) == SC_SIZE) {
      if (!IsVisible()) {
        // Circumvent ScopedRedrawLocks and force visibility before entering a
        // resize or move modal loop to get continuous sizing/moving feedback.
        SetWindowLong(hwnd(), GWL_STYLE,
                      GetWindowLong(hwnd(), GWL_STYLE) | WS_VISIBLE);
      }
    }
  }

  // Handle SC_KEYMENU, which means that the user has pressed the ALT
  // key and released it, so we should focus the menu bar.
  if ((notification_code & sc_mask) == SC_KEYMENU && point.x == 0) {
    int modifiers = ui::EF_NONE;
    if (base::win::IsShiftPressed())
      modifiers |= ui::EF_SHIFT_DOWN;
    if (base::win::IsCtrlPressed())
      modifiers |= ui::EF_CONTROL_DOWN;
    // Retrieve the status of shift and control keys to prevent consuming
    // shift+alt keys, which are used by Windows to change input languages.
    ui::Accelerator accelerator(ui::KeyboardCodeForWindowsKeyCode(VK_MENU),
                                modifiers);
    delegate_->HandleAccelerator(accelerator);
    return;
  }

  // If the delegate can't handle it, the system implementation will be called.
  if (!delegate_->HandleCommand(notification_code)) {
    DefWindowProc(hwnd(), WM_SYSCOMMAND, notification_code,
                  MAKELPARAM(point.x, point.y));
  }
}

void HWNDMessageHandler::OnThemeChanged() {
  ui::NativeThemeWin::instance()->CloseHandles();
}

LRESULT HWNDMessageHandler::OnTouchEvent(UINT message,
                                         WPARAM w_param,
                                         LPARAM l_param) {
  int num_points = LOWORD(w_param);
  scoped_ptr<TOUCHINPUT[]> input(new TOUCHINPUT[num_points]);
  if (ui::GetTouchInputInfoWrapper(reinterpret_cast<HTOUCHINPUT>(l_param),
                                   num_points, input.get(),
                                   sizeof(TOUCHINPUT))) {
    for (int i = 0; i < num_points; ++i) {
      ui::EventType touch_event_type = ui::ET_UNKNOWN;

      if (input[i].dwFlags & TOUCHEVENTF_DOWN) {
        touch_ids_.insert(input[i].dwID);
        touch_event_type = ui::ET_TOUCH_PRESSED;
      } else if (input[i].dwFlags & TOUCHEVENTF_UP) {
        touch_ids_.erase(input[i].dwID);
        touch_event_type = ui::ET_TOUCH_RELEASED;
      } else if (input[i].dwFlags & TOUCHEVENTF_MOVE) {
        touch_event_type = ui::ET_TOUCH_MOVED;
      }
      // Handle touch events only on Aura for now.
#if defined(USE_AURA)
      if (touch_event_type != ui::ET_UNKNOWN) {
        POINT point;
        point.x = TOUCH_COORD_TO_PIXEL(input[i].x);
        point.y = TOUCH_COORD_TO_PIXEL(input[i].y);

        ScreenToClient(hwnd(), &point);

        ui::TouchEvent event(
            touch_event_type,
            gfx::Point(point.x, point.y),
            input[i].dwID % ui::GestureSequence::kMaxGesturePoints,
            base::TimeDelta::FromMilliseconds(input[i].dwTime));
        delegate_->HandleTouchEvent(event);
      }
#endif
    }
  }
  CloseTouchInputHandle(reinterpret_cast<HTOUCHINPUT>(l_param));
  SetMsgHandled(FALSE);
  return 0;
}

void HWNDMessageHandler::OnWindowPosChanging(WINDOWPOS* window_pos) {
  if (ignore_window_pos_changes_) {
    // If somebody's trying to toggle our visibility, change the nonclient area,
    // change our Z-order, or activate us, we should probably let it go through.
    if (!(window_pos->flags & ((IsVisible() ? SWP_HIDEWINDOW : SWP_SHOWWINDOW) |
        SWP_FRAMECHANGED)) &&
        (window_pos->flags & (SWP_NOZORDER | SWP_NOACTIVATE))) {
      // Just sizing/moving the window; ignore.
      window_pos->flags |= SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW;
      window_pos->flags &= ~(SWP_SHOWWINDOW | SWP_HIDEWINDOW);
    }
  } else if (!GetParent(hwnd())) {
    CRect window_rect;
    HMONITOR monitor;
    gfx::Rect monitor_rect, work_area;
    if (GetWindowRect(hwnd(), &window_rect) &&
        GetMonitorAndRects(window_rect, &monitor, &monitor_rect, &work_area)) {
      bool work_area_changed = (monitor_rect == last_monitor_rect_) &&
                               (work_area != last_work_area_);
      if (monitor && (monitor == last_monitor_) &&
          ((fullscreen_handler_->fullscreen() &&
            !fullscreen_handler_->metro_snap()) ||
            work_area_changed)) {
        // A rect for the monitor we're on changed.  Normally Windows notifies
        // us about this (and thus we're reaching here due to the SetWindowPos()
        // call in OnSettingChange() above), but with some software (e.g.
        // nVidia's nView desktop manager) the work area can change asynchronous
        // to any notification, and we're just sent a SetWindowPos() call with a
        // new (frequently incorrect) position/size.  In either case, the best
        // response is to throw away the existing position/size information in
        // |window_pos| and recalculate it based on the new work rect.
        gfx::Rect new_window_rect;
        if (fullscreen_handler_->fullscreen()) {
          new_window_rect = monitor_rect;
        } else if (IsMaximized()) {
          new_window_rect = work_area;
          int border_thickness = GetSystemMetrics(SM_CXSIZEFRAME);
          new_window_rect.Inset(-border_thickness, -border_thickness);
        } else {
          new_window_rect = gfx::Rect(window_rect);
          new_window_rect.AdjustToFit(work_area);
        }
        window_pos->x = new_window_rect.x();
        window_pos->y = new_window_rect.y();
        window_pos->cx = new_window_rect.width();
        window_pos->cy = new_window_rect.height();
        // WARNING!  Don't set SWP_FRAMECHANGED here, it breaks moving the child
        // HWNDs for some reason.
        window_pos->flags &= ~(SWP_NOSIZE | SWP_NOMOVE | SWP_NOREDRAW);
        window_pos->flags |= SWP_NOCOPYBITS;

        // Now ignore all immediately-following SetWindowPos() changes.  Windows
        // likes to (incorrectly) recalculate what our position/size should be
        // and send us further updates.
        ignore_window_pos_changes_ = true;
        DCHECK(!ignore_pos_changes_factory_.HasWeakPtrs());
        base::MessageLoop::current()->PostTask(
            FROM_HERE,
            base::Bind(&HWNDMessageHandler::StopIgnoringPosChanges,
                       ignore_pos_changes_factory_.GetWeakPtr()));
      }
      last_monitor_ = monitor;
      last_monitor_rect_ = monitor_rect;
      last_work_area_ = work_area;
    }
  }

  if (ScopedFullscreenVisibility::IsHiddenForFullscreen(hwnd())) {
    // Prevent the window from being made visible if we've been asked to do so.
    // See comment in header as to why we might want this.
    window_pos->flags &= ~SWP_SHOWWINDOW;
  }

  SetMsgHandled(FALSE);
}

void HWNDMessageHandler::OnWindowPosChanged(WINDOWPOS* window_pos) {
  if (DidClientAreaSizeChange(window_pos))
    ClientAreaSizeChanged();
  if (remove_standard_frame_ && window_pos->flags & SWP_FRAMECHANGED &&
      ui::win::IsAeroGlassEnabled()) {
    MARGINS m = {10, 10, 10, 10};
    DwmExtendFrameIntoClientArea(hwnd(), &m);
  }
  if (window_pos->flags & SWP_SHOWWINDOW)
    delegate_->HandleVisibilityChanged(true);
  else if (window_pos->flags & SWP_HIDEWINDOW)
    delegate_->HandleVisibilityChanged(false);
  SetMsgHandled(FALSE);
}

}  // namespace views
