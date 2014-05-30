// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_ACCESSIBILITY_ACCESSIBILITY_TYPES_H_
#define UI_BASE_ACCESSIBILITY_ACCESSIBILITY_TYPES_H_

#include "base/basictypes.h"

namespace ui {

////////////////////////////////////////////////////////////////////////////////
//
// AccessibilityTypes
//
// Provides enumerations used to preserve platform-independence in
// accessibility functions.
//
////////////////////////////////////////////////////////////////////////////////
class AccessibilityTypes {
 public:

  // This defines states of the supported accessibility roles in our
  // Views (e.g. used in View::GetAccessibleState). Any interface using states
  // must provide a conversion to its own states (see e.g.
  // NativeViewAccessibilityWin::get_accState).
  typedef uint32 State;
  enum StateFlag {
    STATE_CHECKED     = 1 << 0,
    STATE_COLLAPSED   = 1 << 1,
    STATE_DEFAULT     = 1 << 2,
    STATE_EDITABLE    = 1 << 3,
    STATE_EXPANDED    = 1 << 4,
    STATE_FOCUSED     = 1 << 5,
    STATE_HASPOPUP    = 1 << 6,
    STATE_HOTTRACKED  = 1 << 7,
    STATE_INVISIBLE   = 1 << 8,
    STATE_LINKED      = 1 << 9,
    STATE_OFFSCREEN   = 1 << 10,
    STATE_PRESSED     = 1 << 11,
    STATE_PROTECTED   = 1 << 12,
    STATE_READONLY    = 1 << 13,
    STATE_SELECTED    = 1 << 14,
    STATE_UNAVAILABLE = 1 << 15
  };

  // This defines an enumeration of the supported accessibility roles in our
  // Views (e.g. used in View::GetAccessibleRole). Any interface using roles
  // must provide a conversion to its own roles (see e.g.
  // NativeViewAccessibilityWin::get_accRole).
  enum Role {
    ROLE_ALERT,
    ROLE_APPLICATION,
    ROLE_BUTTONDROPDOWN,
    ROLE_BUTTONMENU,
    ROLE_CHECKBUTTON,
    ROLE_CLIENT,
    ROLE_COMBOBOX,
    ROLE_DIALOG,
    ROLE_GRAPHIC,
    ROLE_GROUPING,
    ROLE_LINK,
    ROLE_LOCATION_BAR,
    ROLE_MENUBAR,
    ROLE_MENUITEM,
    ROLE_MENUPOPUP,
    ROLE_OUTLINE,
    ROLE_OUTLINEITEM,
    ROLE_PAGETAB,
    ROLE_PAGETABLIST,
    ROLE_PANE,
    ROLE_PROGRESSBAR,
    ROLE_PUSHBUTTON,
    ROLE_RADIOBUTTON,
    ROLE_SCROLLBAR,
    ROLE_SEPARATOR,
    ROLE_SLIDER,
    ROLE_STATICTEXT,
    ROLE_TEXT,
    ROLE_TITLEBAR,
    ROLE_TOOLBAR,
    ROLE_WINDOW
  };

  // This defines an enumeration of the supported accessibility events in our
  // Views (e.g. used in View::NotifyAccessibilityEvent). Any interface using
  // events must provide a conversion to its own events (see e.g.
  // ViewAccessibility::MSAAEvent).
  enum Event {
    EVENT_ALERT,
    EVENT_FOCUS,
    EVENT_MENUSTART,
    EVENT_MENUEND,
    EVENT_MENUPOPUPSTART,
    EVENT_MENUPOPUPEND,
    EVENT_NAME_CHANGED,
    EVENT_TEXT_CHANGED,
    EVENT_SELECTION_CHANGED,
    EVENT_VALUE_CHANGED
  };

 private:
  // Do not instantiate this class.
  AccessibilityTypes() {}
  ~AccessibilityTypes() {}
};

}  // namespace ui

#endif  // UI_BASE_ACCESSIBILITY_ACCESSIBILITY_TYPES_H_
