// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_COMBOBOX_NATIVE_COMBOBOX_WIN_H_
#define UI_VIEWS_CONTROLS_COMBOBOX_NATIVE_COMBOBOX_WIN_H_

#include "ui/views/controls/combobox/native_combobox_wrapper.h"
#include "ui/views/controls/native_control_win.h"

namespace views {

class NativeComboboxWin : public NativeControlWin,
                          public NativeComboboxWrapper {
 public:
  explicit NativeComboboxWin(Combobox* combobox);
  virtual ~NativeComboboxWin();

  // Overridden from NativeComboboxWrapper:
  virtual void UpdateFromModel() OVERRIDE;
  virtual void UpdateSelectedIndex() OVERRIDE;
  virtual void UpdateEnabled() OVERRIDE;
  virtual int GetSelectedIndex() const OVERRIDE;
  virtual bool IsDropdownOpen() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual View* GetView() OVERRIDE;
  virtual void SetFocus() OVERRIDE;
  virtual void ValidityStateChanged() OVERRIDE;
  virtual bool HandleKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool HandleKeyReleased(const ui::KeyEvent& event) OVERRIDE;
  virtual void HandleFocus() OVERRIDE;
  virtual void HandleBlur() OVERRIDE;
  virtual gfx::NativeView GetTestingHandle() const OVERRIDE;

 protected:
  // Overridden from NativeControlWin:
  virtual bool ProcessMessage(UINT message,
                              WPARAM w_param,
                              LPARAM l_param,
                              LRESULT* result) OVERRIDE;
  virtual void CreateNativeControl() OVERRIDE;
  virtual void NativeControlCreated(HWND native_control) OVERRIDE;

 private:
  void UpdateFont();

  // The combobox we are bound to.
  Combobox* combobox_;

  // The min width, in pixels, for the text content.
  int content_width_;

  DISALLOW_COPY_AND_ASSIGN(NativeComboboxWin);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_COMBOBOX_NATIVE_COMBOBOX_WIN_H_
