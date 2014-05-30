// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_ACCESSIBILITY_ACCESSIBLE_VIEW_STATE_H_
#define UI_BASE_ACCESSIBILITY_ACCESSIBLE_VIEW_STATE_H_

#include "base/basictypes.h"
#include "base/string16.h"
#include "ui/base/accessibility/accessibility_types.h"
#include "ui/base/ui_export.h"

namespace ui {

////////////////////////////////////////////////////////////////////////////////
//
// AccessibleViewState
//
//   A cross-platform struct for storing the core accessibility information
//   that should be provided about any UI view to assistive technology (AT).
//
////////////////////////////////////////////////////////////////////////////////
struct UI_EXPORT AccessibleViewState {
 public:
  AccessibleViewState();
  ~AccessibleViewState();

  // The view's role, like button or list box.
  AccessibilityTypes::Role role;

  // The view's state, a bitmask containing fields such as checked
  // (for a checkbox) and protected (for a password text box).
  AccessibilityTypes::State state;

  // The view's name / label.
  string16 name;

  // The view's value, for example the text content.
  string16 value;

  // The name of the default action if the user clicks on this view.
  string16 default_action;

  // The keyboard shortcut to activate this view, if any.
  string16 keyboard_shortcut;

  // The selection start and end. Only applies to views with text content,
  // such as a text box or combo box; start and end should be -1 otherwise.
  int selection_start;
  int selection_end;

  // The selected item's index and the count of the number of items.
  // Only applies to views with multiple choices like a listbox; both
  // index and count should be -1 otherwise.
  int index;
  int count;
};

}  // namespace ui

#endif  // UI_BASE_ACCESSIBILITY_ACCESSIBLE_VIEW_STATE_H_
