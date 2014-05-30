// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_TOOLTIP_MANAGER_AURA_H_
#define UI_VIEWS_WIDGET_TOOLTIP_MANAGER_AURA_H_

#include "base/compiler_specific.h"
#include "base/string16.h"
#include "ui/gfx/point.h"
#include "ui/views/widget/tooltip_manager.h"

namespace aura {
class RootWindow;
class Window;
}

namespace views {

class Widget;

// TooltipManager implementation for Aura.
class TooltipManagerAura : public TooltipManager {
 public:
  TooltipManagerAura(aura::Window* window, Widget* widget);
  virtual ~TooltipManagerAura();

  // TooltipManager.
  virtual void UpdateTooltip() OVERRIDE;
  virtual void TooltipTextChanged(View* view) OVERRIDE;
  virtual void ShowKeyboardTooltip(View* view) OVERRIDE;
  virtual void HideKeyboardTooltip() OVERRIDE;

 private:
  View* GetViewUnderPoint(const gfx::Point& point);
  void UpdateTooltipForTarget(View* target,
                              const gfx::Point& point,
                              aura::RootWindow* root_window);

  aura::Window* window_;
  Widget* widget_;
  string16 tooltip_text_;

  DISALLOW_COPY_AND_ASSIGN(TooltipManagerAura);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_TOOLTIP_MANAGER_AURA_H_
