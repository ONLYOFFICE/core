// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/remote_root_window_host_win.h"

#include <windows.h>

#include <algorithm>

#include "base/message_loop.h"
#include "ipc/ipc_message.h"
#include "ipc/ipc_sender.h"
#include "ui/aura/client/capture_client.h"
#include "ui/aura/root_window.h"
#include "ui/base/cursor/cursor_loader_win.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_utils.h"
#include "ui/base/keycodes/keyboard_code_conversion_win.h"
#include "ui/base/view_prop.h"
#include "ui/gfx/insets.h"
#include "ui/metro_viewer/metro_viewer_messages.h"

namespace aura {

namespace {

const char* kRootWindowHostWinKey = "__AURA_REMOTE_ROOT_WINDOW_HOST_WIN__";

// Sets the keystate for the virtual key passed in to down or up.
void SetKeyState(uint8* key_states, bool key_down, uint32 virtual_key_code) {
  DCHECK(key_states);

  if (key_down)
    key_states[virtual_key_code] |= 0x80;
  else
    key_states[virtual_key_code] &= 0x7F;
}

// Sets the keyboard states for the Shift/Control/Alt/Caps lock keys.
void SetVirtualKeyStates(uint32 flags) {
  uint8 keyboard_state[256] = {0};
  ::GetKeyboardState(keyboard_state);

  SetKeyState(keyboard_state, !!(flags & ui::EF_SHIFT_DOWN), VK_SHIFT);
  SetKeyState(keyboard_state, !!(flags & ui::EF_CONTROL_DOWN), VK_CONTROL);
  SetKeyState(keyboard_state, !!(flags & ui::EF_ALT_DOWN), VK_MENU);
  SetKeyState(keyboard_state, !!(flags & ui::EF_CAPS_LOCK_DOWN), VK_CAPITAL);

  ::SetKeyboardState(keyboard_state);
}

}  // namespace

void HandleOpenFile(
    const string16& title,
    const base::FilePath& default_path,
    const string16& filter,
    const OpenFileCompletion& callback) {
  DCHECK(aura::RemoteRootWindowHostWin::Instance());
  aura::RemoteRootWindowHostWin::Instance()->HandleOpenFile(title,
                                                            default_path,
                                                            filter,
                                                            callback);
}

void HandleOpenMultipleFiles(
    const string16& title,
    const base::FilePath& default_path,
    const string16& filter,
    const OpenMultipleFilesCompletion& callback) {
  DCHECK(aura::RemoteRootWindowHostWin::Instance());
  aura::RemoteRootWindowHostWin::Instance()->HandleOpenMultipleFiles(
      title,
      default_path,
      filter,
      callback);
}

void HandleSaveFile(
    const string16& title,
    const base::FilePath& default_path,
    const string16& filter,
    int filter_index,
    const string16& default_extension,
    const SaveFileCompletion& callback) {
  DCHECK(aura::RemoteRootWindowHostWin::Instance());
  aura::RemoteRootWindowHostWin::Instance()->HandleSaveFile(title,
                                                            default_path,
                                                            filter,
                                                            filter_index,
                                                            default_extension,
                                                            callback);
}

void HandleSelectFolder(const string16& title,
                        const SelectFolderCompletion& callback) {
  DCHECK(aura::RemoteRootWindowHostWin::Instance());
  aura::RemoteRootWindowHostWin::Instance()->HandleSelectFolder(title,
                                                                callback);
}

RemoteRootWindowHostWin* g_instance = NULL;

RemoteRootWindowHostWin* RemoteRootWindowHostWin::Instance() {
  return g_instance;
}

RemoteRootWindowHostWin* RemoteRootWindowHostWin::Create(
    const gfx::Rect& bounds) {
  g_instance = new RemoteRootWindowHostWin(bounds);
  return g_instance;
}

RemoteRootWindowHostWin::RemoteRootWindowHostWin(const gfx::Rect& bounds)
    : delegate_(NULL), host_(NULL) {
  prop_.reset(new ui::ViewProp(NULL, kRootWindowHostWinKey, this));
}

RemoteRootWindowHostWin::~RemoteRootWindowHostWin() {
}

void RemoteRootWindowHostWin::Connected(IPC::Sender* host) {
  CHECK(host_ == NULL);
  host_ = host;
}

void RemoteRootWindowHostWin::Disconnected() {
  CHECK(host_ != NULL);
  host_ = NULL;
}

bool RemoteRootWindowHostWin::OnMessageReceived(const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(RemoteRootWindowHostWin, message)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_MouseMoved, OnMouseMoved)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_MouseButton, OnMouseButton)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_KeyDown, OnKeyDown)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_KeyUp, OnKeyUp)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_Character, OnChar)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_VisibilityChanged,
                        OnVisibilityChanged)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_TouchDown,
                        OnTouchDown)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_TouchUp,
                        OnTouchUp)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_TouchMoved,
                        OnTouchMoved)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_FileSaveAsDone,
                        OnFileSaveAsDone)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_FileOpenDone,
                        OnFileOpenDone)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_MultiFileOpenDone,
                        OnMultiFileOpenDone)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_SelectFolderDone,
                        OnSelectFolderDone)
    IPC_MESSAGE_HANDLER(MetroViewerHostMsg_WindowActivated,
                        OnWindowActivated)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}

void RemoteRootWindowHostWin::HandleOpenFile(
    const string16& title,
    const base::FilePath& default_path,
    const string16& filter,
    const OpenFileCompletion& callback) {
  if (!host_)
    return;

  // Can only have one of these operations in flight.
  DCHECK(file_open_completion_callback_.is_null());
  file_open_completion_callback_ = callback;

  host_->Send(new MetroViewerHostMsg_DisplayFileOpen(title,
                                                     filter,
                                                     default_path,
                                                     false));
}

void RemoteRootWindowHostWin::HandleOpenMultipleFiles(
      const string16& title,
      const base::FilePath& default_path,
      const string16& filter,
      const OpenMultipleFilesCompletion& callback) {
  if (!host_)
    return;

  // Can only have one of these operations in flight.
  DCHECK(multi_file_open_completion_callback_.is_null());
  multi_file_open_completion_callback_ = callback;

  host_->Send(new MetroViewerHostMsg_DisplayFileOpen(title,
                                                     filter,
                                                     default_path,
                                                     true));
}

void RemoteRootWindowHostWin::HandleSaveFile(
    const string16& title,
    const base::FilePath& default_path,
    const string16& filter,
    int filter_index,
    const string16& default_extension,
    const SaveFileCompletion& callback) {
  if (!host_)
    return;

  MetroViewerHostMsg_SaveAsDialogParams params;
  params.title = title;
  params.default_extension = default_extension;
  params.filter = filter;
  params.filter_index = filter_index;

  // Can only have one of these operations in flight.
  DCHECK(file_saveas_completion_callback_.is_null());
  file_saveas_completion_callback_ = callback;

  host_->Send(new MetroViewerHostMsg_DisplayFileSaveAs(params));
}

void RemoteRootWindowHostWin::HandleSelectFolder(
    const string16& title,
    const SelectFolderCompletion& callback) {
  if (!host_)
    return;

  // Can only have one of these operations in flight.
  DCHECK(select_folder_completion_callback_.is_null());
  select_folder_completion_callback_ = callback;

  host_->Send(new MetroViewerHostMsg_DisplaySelectFolder(title));
}

void RemoteRootWindowHostWin::SetDelegate(RootWindowHostDelegate* delegate) {
  delegate_ = delegate;
}

RootWindow* RemoteRootWindowHostWin::GetRootWindow() {
  return delegate_->AsRootWindow();
}

gfx::AcceleratedWidget RemoteRootWindowHostWin::GetAcceleratedWidget() {
  // TODO(cpu): This is bad. Chrome's compositor needs a valid window
  // initially and then later on we swap it. Since the compositor never
  // uses this initial window we tell ourselves this hack is ok to get
  // thing off the ground.
  return ::GetDesktopWindow();
}

void RemoteRootWindowHostWin::Show() {
}

void RemoteRootWindowHostWin::Hide() {
  NOTIMPLEMENTED();
}

void RemoteRootWindowHostWin::ToggleFullScreen() {
}

gfx::Rect RemoteRootWindowHostWin::GetBounds() const {
  gfx::Rect r(gfx::Point(0, 0), aura::RootWindowHost::GetNativeScreenSize());
  return r;
}

void RemoteRootWindowHostWin::SetBounds(const gfx::Rect& bounds) {
  delegate_->OnHostResized(bounds.size());
}

gfx::Insets RemoteRootWindowHostWin::GetInsets() const {
  return gfx::Insets();
}

void RemoteRootWindowHostWin::SetInsets(const gfx::Insets& insets) {
}

gfx::Point RemoteRootWindowHostWin::GetLocationOnNativeScreen() const {
  return gfx::Point(0, 0);
}

void RemoteRootWindowHostWin::SetCursor(gfx::NativeCursor native_cursor) {
  if (!host_)
    return;
  host_->Send(
      new MetroViewerHostMsg_SetCursor(uint64(native_cursor.platform())));
}

void RemoteRootWindowHostWin::SetCapture() {
}

void RemoteRootWindowHostWin::ReleaseCapture() {
}

bool RemoteRootWindowHostWin::QueryMouseLocation(gfx::Point* location_return) {
  POINT pt;
  GetCursorPos(&pt);
  *location_return =
      gfx::Point(static_cast<int>(pt.x), static_cast<int>(pt.y));
  return true;
}

bool RemoteRootWindowHostWin::ConfineCursorToRootWindow() {
  return true;
}

bool RemoteRootWindowHostWin::CopyAreaToSkCanvas(const gfx::Rect& source_bounds,
                                                 const gfx::Point& dest_offset,
                                                 SkCanvas* canvas) {
  NOTIMPLEMENTED();
  return false;
}

void RemoteRootWindowHostWin::UnConfineCursor() {
}

void RemoteRootWindowHostWin::OnCursorVisibilityChanged(bool show) {
  NOTIMPLEMENTED();
}

void RemoteRootWindowHostWin::MoveCursorTo(const gfx::Point& location) {
}

void RemoteRootWindowHostWin::SetFocusWhenShown(bool focus_when_shown) {
  NOTIMPLEMENTED();
}

void RemoteRootWindowHostWin::PostNativeEvent(
    const base::NativeEvent& native_event) {
}

void RemoteRootWindowHostWin::OnDeviceScaleFactorChanged(
    float device_scale_factor) {
  NOTIMPLEMENTED();
}

void RemoteRootWindowHostWin::PrepareForShutdown() {
}

void RemoteRootWindowHostWin::OnMouseMoved(int32 x, int32 y, int32 flags) {
  gfx::Point location(x, y);
  ui::MouseEvent event(ui::ET_MOUSE_MOVED, location, location, flags);
  delegate_->OnHostMouseEvent(&event);
}

void RemoteRootWindowHostWin::OnMouseButton(
    int32 x, int32 y, int32 extra, ui::EventType type, ui::EventFlags flags) {
  gfx::Point location(x, y);
  ui::MouseEvent mouse_event(type, location, location, 0);
  mouse_event.set_flags(flags);

  if (type == ui::ET_MOUSEWHEEL) {
    ui::MouseWheelEvent wheel_event(mouse_event, 0, extra);
    delegate_->OnHostMouseEvent(&wheel_event);
  } else {
    mouse_event.SetClickCount(1);
    delegate_->OnHostMouseEvent(&mouse_event);
  }
}

void RemoteRootWindowHostWin::OnKeyDown(uint32 vkey,
                                        uint32 repeat_count,
                                        uint32 scan_code,
                                        uint32 flags) {
  DispatchKeyboardMessage(ui::ET_KEY_PRESSED, vkey, repeat_count, scan_code,
                          flags, false);
}

void RemoteRootWindowHostWin::OnKeyUp(uint32 vkey,
                                      uint32 repeat_count,
                                      uint32 scan_code,
                                      uint32 flags) {
  DispatchKeyboardMessage(ui::ET_KEY_RELEASED, vkey, repeat_count, scan_code,
                          flags, false);
}

void RemoteRootWindowHostWin::OnChar(uint32 key_code,
                                     uint32 repeat_count,
                                     uint32 scan_code,
                                     uint32 flags) {
  DispatchKeyboardMessage(ui::ET_KEY_PRESSED, key_code, repeat_count,
                          scan_code, flags, true);
}

void RemoteRootWindowHostWin::OnVisibilityChanged(bool visible) {
  if (visible)
    delegate_->OnHostActivated();
}

void RemoteRootWindowHostWin::OnTouchDown(int32 x,
                                          int32 y,
                                          uint64 timestamp,
                                          uint32 pointer_id) {
  ui::TouchEvent event(ui::ET_TOUCH_PRESSED,
                       gfx::Point(x, y),
                       pointer_id,
                       base::TimeDelta::FromMicroseconds(timestamp));
  delegate_->OnHostTouchEvent(&event);
}

void RemoteRootWindowHostWin::OnTouchUp(int32 x,
                                        int32 y,
                                        uint64 timestamp,
                                        uint32 pointer_id) {
  ui::TouchEvent event(ui::ET_TOUCH_RELEASED,
                       gfx::Point(x, y),
                       pointer_id,
                       base::TimeDelta::FromMicroseconds(timestamp));
  delegate_->OnHostTouchEvent(&event);
}

void RemoteRootWindowHostWin::OnTouchMoved(int32 x,
                                           int32 y,
                                           uint64 timestamp,
                                           uint32 pointer_id) {
  ui::TouchEvent event(ui::ET_TOUCH_MOVED,
                       gfx::Point(x, y),
                       pointer_id,
                       base::TimeDelta::FromMicroseconds(timestamp));
  delegate_->OnHostTouchEvent(&event);
}

void RemoteRootWindowHostWin::OnFileSaveAsDone(bool success,
                                               const base::FilePath& filename,
                                               int filter_index) {
  if (success) {
    file_saveas_completion_callback_.Run(filename, filter_index, NULL);
  }
  file_saveas_completion_callback_.Reset();
}


void RemoteRootWindowHostWin::OnFileOpenDone(bool success,
                                             const base::FilePath& filename) {
  if (success) {
    file_open_completion_callback_.Run(
        base::FilePath(filename), 0, NULL);
  }
  file_open_completion_callback_.Reset();
}

void RemoteRootWindowHostWin::OnMultiFileOpenDone(
    bool success,
    const std::vector<base::FilePath>& files) {
  if (success) {
    multi_file_open_completion_callback_.Run(files, NULL);
  }
  multi_file_open_completion_callback_.Reset();
}

void RemoteRootWindowHostWin::OnSelectFolderDone(
    bool success,
    const base::FilePath& folder) {
  if (success) {
    select_folder_completion_callback_.Run(base::FilePath(folder), 0, NULL);
  }
  select_folder_completion_callback_.Reset();
}

void RemoteRootWindowHostWin::OnWindowActivated(bool active) {
  active ? GetRootWindow()->Focus() : GetRootWindow()->Blur();
}

void RemoteRootWindowHostWin::DispatchKeyboardMessage(ui::EventType type,
                                                      uint32 vkey,
                                                      uint32 repeat_count,
                                                      uint32 scan_code,
                                                      uint32 flags,
                                                      bool is_character) {
  if (base::MessageLoop::current()->IsNested()) {
    SetVirtualKeyStates(flags);

    uint32 message = is_character ? WM_CHAR :
        (type == ui::ET_KEY_PRESSED ? WM_KEYDOWN : WM_KEYUP);
    ::PostThreadMessage(::GetCurrentThreadId(),
                        message,
                        vkey,
                        repeat_count | scan_code >> 15);
  } else {
    ui::KeyEvent event(type,
                       ui::KeyboardCodeForWindowsKeyCode(vkey),
                       flags,
                       is_character);
    delegate_->OnHostKeyEvent(&event);
  }
}

}  // namespace aura
