// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/combobox/combobox.h"

#include "base/logging.h"
#include "base/utf_string_conversions.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/events/event.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/base/models/combobox_model.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/views/controls/combobox/combobox_listener.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/widget/widget.h"

namespace views {

// static
const char Combobox::kViewClassName[] = "views/Combobox";

////////////////////////////////////////////////////////////////////////////////
// Combobox, public:

Combobox::Combobox(ui::ComboboxModel* model)
    : native_wrapper_(NULL),
      model_(model),
      listener_(NULL),
      selected_index_(model_->GetDefaultIndex()),
      invalid_(false) {
  set_focusable(true);
}

Combobox::~Combobox() {
}

// static
const gfx::Font& Combobox::GetFont() {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  return rb.GetFont(ui::ResourceBundle::BaseFont);
}

void Combobox::ModelChanged() {
  selected_index_ = std::min(0, model_->GetItemCount());
  if (native_wrapper_)
    native_wrapper_->UpdateFromModel();
  PreferredSizeChanged();
}

void Combobox::SetSelectedIndex(int index) {
  selected_index_ = index;
  if (native_wrapper_)
    native_wrapper_->UpdateSelectedIndex();
}

void Combobox::SelectionChanged() {
  selected_index_ = native_wrapper_->GetSelectedIndex();
  if (listener_)
    listener_->OnSelectedIndexChanged(this);
  NotifyAccessibilityEvent(ui::AccessibilityTypes::EVENT_VALUE_CHANGED, false);
}

void Combobox::SetAccessibleName(const string16& name) {
  accessible_name_ = name;
}

void Combobox::SetInvalid(bool invalid) {
  invalid_ = invalid;
  if (native_wrapper_)
    native_wrapper_->ValidityStateChanged();
}

////////////////////////////////////////////////////////////////////////////////
// Combobox, View overrides:

gfx::Size Combobox::GetPreferredSize() {
  if (native_wrapper_)
    return native_wrapper_->GetPreferredSize();
  return gfx::Size();
}

void Combobox::Layout() {
  if (native_wrapper_) {
    native_wrapper_->GetView()->SetBounds(0, 0, width(), height());
    native_wrapper_->GetView()->Layout();
  }
}

void Combobox::OnEnabledChanged() {
  View::OnEnabledChanged();
  if (native_wrapper_)
    native_wrapper_->UpdateEnabled();
}

// VKEY_ESCAPE should be handled by this view when the drop down list is active.
// In other words, the list should be closed instead of the dialog.
bool Combobox::SkipDefaultKeyEventProcessing(const ui::KeyEvent& e) {
  if (e.key_code() != ui::VKEY_ESCAPE ||
      e.IsShiftDown() || e.IsControlDown() || e.IsAltDown()) {
    return false;
  }
  return native_wrapper_ && native_wrapper_->IsDropdownOpen();
}

void Combobox::OnPaintFocusBorder(gfx::Canvas* canvas) {
  if (NativeViewHost::kRenderNativeControlFocus)
    View::OnPaintFocusBorder(canvas);
}

bool Combobox::OnKeyPressed(const ui::KeyEvent& e) {
  return native_wrapper_ && native_wrapper_->HandleKeyPressed(e);
}

bool Combobox::OnKeyReleased(const ui::KeyEvent& e) {
  return native_wrapper_ && native_wrapper_->HandleKeyReleased(e);
}

void Combobox::OnFocus() {
  // Forward the focus to the wrapper.
  if (native_wrapper_) {
    native_wrapper_->SetFocus();
    NotifyAccessibilityEvent(ui::AccessibilityTypes::EVENT_FOCUS, true);
  } else {
    View::OnFocus();  // Will focus the RootView window (so we still get
                      // keyboard messages).
  }
}

void Combobox::OnBlur() {
  if (native_wrapper_)
    native_wrapper_->HandleBlur();
}

void Combobox::GetAccessibleState(ui::AccessibleViewState* state) {
  state->role = ui::AccessibilityTypes::ROLE_COMBOBOX;
  state->name = accessible_name_;
  state->value = model_->GetItemAt(selected_index_);
  state->index = selected_index_;
  state->count = model_->GetItemCount();
}

void Combobox::ViewHierarchyChanged(bool is_add, View* parent, View* child) {
  if (is_add && !native_wrapper_ && GetWidget()) {
    // The native wrapper's lifetime will be managed by the view hierarchy after
    // we call AddChildView.
    native_wrapper_ = NativeComboboxWrapper::CreateWrapper(this);
    AddChildView(native_wrapper_->GetView());
    // The underlying native widget may not be created until the wrapper is
    // parented. For this reason the wrapper is only updated after adding its
    // view.
    native_wrapper_->UpdateFromModel();
    native_wrapper_->UpdateSelectedIndex();
    native_wrapper_->UpdateEnabled();
  }
}

std::string Combobox::GetClassName() const {
  return kViewClassName;
}

}  // namespace views
