// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_COMBOBOX_NATIVE_COMBOBOX_VIEWS_H_
#define UI_VIEWS_CONTROLS_COMBOBOX_NATIVE_COMBOBOX_VIEWS_H_

#include "ui/views/controls/combobox/native_combobox_wrapper.h"
#include "ui/views/controls/menu/menu_delegate.h"
#include "ui/views/view.h"

namespace gfx {
class Canvas;
class Font;
}

namespace views {

class FocusableBorder;
class MenuRunner;

// A views/skia only implementation of NativeComboboxWrapper.
// No platform specific code is used.
class NativeComboboxViews : public views::View,
                            public NativeComboboxWrapper,
                            public views::MenuDelegate {
 public:
  static const char kViewClassName[];

  explicit NativeComboboxViews(Combobox* combobox);
  virtual ~NativeComboboxViews();

  // views::View overrides:
  virtual bool OnMousePressed(const ui::MouseEvent& mouse_event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& mouse_event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& key_event) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& key_event) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;

  // ui::EventHandler overrides:
  virtual void OnGestureEvent(ui::GestureEvent* gesture) OVERRIDE;

  // NativeComboboxWrapper overrides:
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

  // MenuDelegate overrides:
  virtual bool IsItemChecked(int id) const OVERRIDE;
  virtual bool IsCommandEnabled(int id) const OVERRIDE;
  virtual void ExecuteCommand(int id) OVERRIDE;
  virtual bool GetAccelerator(int id, ui::Accelerator* accelerator) OVERRIDE;

 private:
  // Given bounds within our View, this helper routine mirrors the bounds if
  // necessary.
  void AdjustBoundsForRTLUI(gfx::Rect* rect) const;

  // Draw the selected value of the drop down list
  void PaintText(gfx::Canvas* canvas);

  // Show the drop down list
  void ShowDropDownMenu();

  // The parent combobox, the owner of this object.
  Combobox* combobox_;

  // The reference to the border class. The object is owned by View::border_.
  FocusableBorder* text_border_;

  // The disclosure arrow next to the currently selected item from the list.
  const gfx::ImageSkia* disclosure_arrow_;

  // Responsible for showing the context menu.
  scoped_ptr<MenuRunner> dropdown_list_menu_runner_;

  // Is the drop down list showing
  bool dropdown_open_;

  // The selected index in the model. The default value is -1, which means no
  // selection.
  int selected_index_;

  // The maximum dimensions of the content in the dropdown
  int content_width_;
  int content_height_;

  DISALLOW_COPY_AND_ASSIGN(NativeComboboxViews);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_COMBOBOX_NATIVE_COMBOBOX_VIEWS_H_
