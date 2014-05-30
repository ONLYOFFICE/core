// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_COMBOBOX_COMBOBOX_H_
#define UI_VIEWS_CONTROLS_COMBOBOX_COMBOBOX_H_

#include <string>

#include "ui/gfx/native_widget_types.h"
#include "ui/views/controls/combobox/native_combobox_wrapper.h"
#include "ui/views/view.h"

namespace gfx {
class Font;
}

namespace ui {
class ComboboxModel;
}

namespace views {

class ComboboxListener;

// A non-editable combobox (aka a drop-down list).
class VIEWS_EXPORT Combobox : public View {
 public:
  // The combobox's class name.
  static const char kViewClassName[];

  // |model| is not owned by the combobox.
  explicit Combobox(ui::ComboboxModel* model);
  virtual ~Combobox();

  static const gfx::Font& GetFont();

  // Sets the listener which will be called when a selection has been made.
  void set_listener(ComboboxListener* listener) {
    listener_ = listener;
  }

  // Informs the combobox that its model changed.
  void ModelChanged();

  // Gets/Sets the selected index.
  int selected_index() const { return selected_index_; }
  void SetSelectedIndex(int index);

  // Called when the combobox's selection is changed by the user.
  void SelectionChanged();

  ui::ComboboxModel* model() const { return model_; }

  // Set the accessible name of the combobox.
  void SetAccessibleName(const string16& name);

  // Provided only for testing:
  gfx::NativeView GetTestingHandle() const {
    return native_wrapper_ ? native_wrapper_->GetTestingHandle() : NULL;
  }
  NativeComboboxWrapper* GetNativeWrapperForTesting() const {
    return native_wrapper_;
  }

  // Visually marks the combobox as having an invalid value selected. The caller
  // is responsible for flipping it back to valid if the selection changes.
  void SetInvalid(bool invalid);

  bool invalid() const {
    return invalid_;
  }

  // Overridden from View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnEnabledChanged() OVERRIDE;
  virtual bool SkipDefaultKeyEventProcessing(const ui::KeyEvent& e) OVERRIDE;
  virtual void OnPaintFocusBorder(gfx::Canvas* canvas) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& e) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& e) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 protected:
  // Overridden from View:
  virtual void ViewHierarchyChanged(bool is_add,
                                    View* parent,
                                    View* child) OVERRIDE;
  virtual std::string GetClassName() const OVERRIDE;

  // The object that actually implements the native combobox.
  NativeComboboxWrapper* native_wrapper_;

 private:
  // Our model. Not owned.
  ui::ComboboxModel* model_;

  // Our listener. Not owned. Notified when the selected index change.
  ComboboxListener* listener_;

  // The current selected index.
  int selected_index_;

  // True when the selection is visually denoted as invalid.
  bool invalid_;

  // The accessible name of this combobox.
  string16 accessible_name_;

  DISALLOW_COPY_AND_ASSIGN(Combobox);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_COMBOBOX_COMBOBOX_H_
