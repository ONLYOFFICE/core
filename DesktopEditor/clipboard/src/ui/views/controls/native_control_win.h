// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_NATIVE_CONTROL_WIN_H_
#define UI_VIEWS_CONTROLS_NATIVE_CONTROL_WIN_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_vector.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/widget/child_window_message_processor.h"

namespace ui {
class ViewProp;
}

namespace views {

// A View that hosts a native Windows control.
class NativeControlWin : public ChildWindowMessageProcessor,
                         public NativeViewHost {
 public:
  NativeControlWin();
  virtual ~NativeControlWin();

  // Called by our subclassed window procedure when a WM_KEYDOWN message is
  // received by the HWND created by an object derived from NativeControlWin.
  // Returns true if the key was processed, false otherwise.
  virtual bool OnKeyDown(int vkey) { return false; }

  // Overridden from ChildWindowMessageProcessor:
  virtual bool ProcessMessage(UINT message,
                              WPARAM w_param,
                              LPARAM l_param,
                              LRESULT* result) OVERRIDE;

  // Overridden from View:
  virtual void OnEnabledChanged() OVERRIDE;

 protected:
  virtual void ViewHierarchyChanged(bool is_add,
                                    View* parent,
                                    View* child) OVERRIDE;
  virtual void VisibilityChanged(View* starting_from, bool is_visible) OVERRIDE;
  virtual void OnFocus() OVERRIDE;

  // Called by the containing NativeWidgetWin when a WM_CONTEXTMENU message is
  // received from the HWND created by an object derived from NativeControlWin.
  virtual void ShowContextMenu(const gfx::Point& location);

  // Called when the NativeControlWin is attached to a View hierarchy with a
  // valid Widget. The NativeControlWin should use this opportunity to create
  // its associated HWND.
  virtual void CreateNativeControl() = 0;

  // MUST be called by the subclass implementation of |CreateNativeControl|
  // immediately after creating the control HWND, otherwise it won't be attached
  // to the NativeViewHost and will be effectively orphaned.
  virtual void NativeControlCreated(HWND native_control);

  // Returns additional extended style flags. When subclasses call
  // CreateWindowEx in order to create the underlying control, they must OR the
  // ExStyle parameter with the value returned by this function.
  //
  // We currently use this method in order to add flags such as WS_EX_LAYOUTRTL
  // to the HWND for views with right-to-left UI layout.
  DWORD GetAdditionalExStyle() const;

  // TODO(xji): we use the following temporary function as we transition the
  // various native controls to use the right set of RTL flags. This function
  // will go away (and be replaced by GetAdditionalExStyle()) once all the
  // controls are properly transitioned.
  DWORD GetAdditionalRTLStyle() const;

 private:
  typedef ScopedVector<ui::ViewProp> ViewProps;

  // Called by the containing NativeWidgetWin when a message of type
  // WM_CTLCOLORBTN or WM_CTLCOLORSTATIC is sent from the HWND created by an
  // object derived from NativeControlWin.
  LRESULT GetControlColor(UINT message, HDC dc, HWND sender);

  // Our subclass window procedure for the attached control.
  static LRESULT CALLBACK NativeControlWndProc(HWND window,
                                               UINT message,
                                               WPARAM w_param,
                                               LPARAM l_param);

  // The window procedure before we subclassed.
  WNDPROC original_wndproc_;

  ViewProps props_;

  DISALLOW_COPY_AND_ASSIGN(NativeControlWin);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_NATIVE_CONTROL_WIN_H_
