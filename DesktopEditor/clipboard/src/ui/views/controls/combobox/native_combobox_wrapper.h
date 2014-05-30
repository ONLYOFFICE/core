// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_COMBOBOX_NATIVE_COMBOBOX_WRAPPER_H_
#define UI_VIEWS_CONTROLS_COMBOBOX_NATIVE_COMBOBOX_WRAPPER_H_

#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"

namespace gfx {
class Size;
}

namespace ui {
class KeyEvent;
}

namespace views {

class Combobox;
class View;

class VIEWS_EXPORT NativeComboboxWrapper {
 public:
  // Updates the combobox's content from its model.
  virtual void UpdateFromModel() = 0;

  // Updates the selected index from the associated combobox.
  virtual void UpdateSelectedIndex() = 0;

  // Updates the enabled state of the combobox from the associated view.
  virtual void UpdateEnabled() = 0;

  // Returns the selected index.
  virtual int GetSelectedIndex() const = 0;

  // Returns true if the combobox dropdown is open.
  virtual bool IsDropdownOpen() const = 0;

  // Returns the preferred size of the combobox.
  virtual gfx::Size GetPreferredSize() = 0;

  // Retrieves the view that hosts the native control.
  virtual View* GetView() = 0;

  // Sets the focus to the button.
  virtual void SetFocus() = 0;

  // Called when the combobox flipped validity. When invalid, it paints
  // with white text on a red background.
  virtual void ValidityStateChanged() = 0;

  // Invoked when a key is pressed/release on combobox. Subclasser should
  // return true if the event has been processed and false otherwise.
  // See also View::OnKeyPressed/OnKeyReleased.
  virtual bool HandleKeyPressed(const ui::KeyEvent& e) = 0;
  virtual bool HandleKeyReleased(const ui::KeyEvent& e) = 0;

  // Invoked when focus is being moved from or to the combobox.
  // See also View::OnFocus/OnBlur.
  virtual void HandleFocus() = 0;
  virtual void HandleBlur() = 0;

  // Returns a handle to the underlying native view for testing.
  virtual gfx::NativeView GetTestingHandle() const = 0;

  static NativeComboboxWrapper* CreateWrapper(Combobox* combobox);

 protected:
  virtual ~NativeComboboxWrapper() {}
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_COMBOBOX_NATIVE_COMBOBOX_WRAPPER_H_
