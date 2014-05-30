// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_FOCUS_RULES_H_
#define UI_VIEWS_WIDGET_DESKTOP_FOCUS_RULES_H_

#include "ui/views/corewm/base_focus_rules.h"

namespace views {

class DesktopFocusRules : public corewm::BaseFocusRules {
 public:
  DesktopFocusRules();
  virtual ~DesktopFocusRules();

 private:
  // Overridden from corewm::BaseFocusRules:
  virtual bool SupportsChildActivation(aura::Window* window) const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(DesktopFocusRules);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DESKTOP_FOCUS_RULES_H_
