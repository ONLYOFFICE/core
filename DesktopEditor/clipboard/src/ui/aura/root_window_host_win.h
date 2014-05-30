// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ROOT_WINDOW_HOST_WIN_H_
#define UI_AURA_ROOT_WINDOW_HOST_WIN_H_

#include "base/compiler_specific.h"
#include "ui/aura/root_window_host.h"
#include "ui/base/ui_export.h"
#include "ui/base/win/window_impl.h"

namespace aura {

class RootWindowHostWin : public RootWindowHost, public ui::WindowImpl {
 public:
  RootWindowHostWin(const gfx::Rect& bounds);
  virtual ~RootWindowHostWin();
  // RootWindowHost:
  virtual void SetDelegate(RootWindowHostDelegate* delegate) OVERRIDE;
  virtual RootWindow* GetRootWindow() OVERRIDE;
  virtual gfx::AcceleratedWidget GetAcceleratedWidget() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void ToggleFullScreen() OVERRIDE;
  virtual gfx::Rect GetBounds() const OVERRIDE;
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual void SetInsets(const gfx::Insets& insets) OVERRIDE;
  virtual gfx::Point GetLocationOnNativeScreen() const OVERRIDE;
  virtual void SetCapture() OVERRIDE;
  virtual void ReleaseCapture() OVERRIDE;
  virtual void SetCursor(gfx::NativeCursor cursor) OVERRIDE;
  virtual bool QueryMouseLocation(gfx::Point* location_return) OVERRIDE;
  virtual bool ConfineCursorToRootWindow() OVERRIDE;
  virtual void UnConfineCursor() OVERRIDE;
  virtual void OnCursorVisibilityChanged(bool show) OVERRIDE;
  virtual void MoveCursorTo(const gfx::Point& location) OVERRIDE;
  virtual void SetFocusWhenShown(bool focus_when_shown) OVERRIDE;
  virtual bool CopyAreaToSkCanvas(const gfx::Rect& source_bounds,
                                  const gfx::Point& dest_offset,
                                  SkCanvas* canvas) OVERRIDE;
  virtual void PostNativeEvent(const base::NativeEvent& native_event) OVERRIDE;
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;
  virtual void PrepareForShutdown() OVERRIDE;

 private:
  BEGIN_MSG_MAP_EX(RootWindowHostWin)
    // Range handlers must go first!
    MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseRange)
    MESSAGE_RANGE_HANDLER_EX(WM_NCMOUSEMOVE, WM_NCXBUTTONDBLCLK, OnMouseRange)

    // Mouse capture events.
    MESSAGE_HANDLER_EX(WM_CAPTURECHANGED, OnCaptureChanged)

    // Key events.
    MESSAGE_HANDLER_EX(WM_KEYDOWN, OnKeyEvent)
    MESSAGE_HANDLER_EX(WM_KEYUP, OnKeyEvent)
    MESSAGE_HANDLER_EX(WM_SYSKEYDOWN, OnKeyEvent)
    MESSAGE_HANDLER_EX(WM_SYSKEYUP, OnKeyEvent)
    MESSAGE_HANDLER_EX(WM_CHAR, OnKeyEvent)
    MESSAGE_HANDLER_EX(WM_SYSCHAR, OnKeyEvent)
    MESSAGE_HANDLER_EX(WM_IME_CHAR, OnKeyEvent)
    MESSAGE_HANDLER_EX(WM_NCACTIVATE, OnNCActivate)

    MSG_WM_CLOSE(OnClose)
    MSG_WM_MOVE(OnMove)
    MSG_WM_PAINT(OnPaint)
    MSG_WM_SIZE(OnSize)
  END_MSG_MAP()

  void OnClose();
  LRESULT OnKeyEvent(UINT message, WPARAM w_param, LPARAM l_param);
  LRESULT OnMouseRange(UINT message, WPARAM w_param, LPARAM l_param);
  LRESULT OnCaptureChanged(UINT message, WPARAM w_param, LPARAM l_param);
  LRESULT OnNCActivate(UINT message, WPARAM w_param, LPARAM l_param);
  void OnMove(const CPoint& point);
  void OnPaint(HDC dc);
  void OnSize(UINT param, const CSize& size);

  RootWindowHostDelegate* delegate_;

  bool fullscreen_;
  bool has_capture_;
  RECT saved_window_rect_;
  DWORD saved_window_style_;
  DWORD saved_window_ex_style_;

  DISALLOW_COPY_AND_ASSIGN(RootWindowHostWin);
};

namespace test {

// Set true to let RootWindowHostWin use a popup window
// with no frame/title so that the window size and test's
// expectations matches.
AURA_EXPORT void SetUsePopupAsRootWindowForTest(bool use);

}  // namespace

}  // namespace aura

#endif  // UI_AURA_ROOT_WINDOW_HOST_WIN_H_
