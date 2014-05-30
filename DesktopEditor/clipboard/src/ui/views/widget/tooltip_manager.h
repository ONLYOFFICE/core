// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_TOOLTIP_MANAGER_H_
#define UI_VIEWS_WIDGET_TOOLTIP_MANAGER_H_

#include <string>

#include "base/basictypes.h"
#include "base/string16.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/views/views_export.h"

namespace gfx {
class Font;
}  // namespace gfx

namespace views {

class View;

// TooltipManager takes care of the wiring to support tooltips for Views. You
// almost never need to interact directly with TooltipManager, rather look to
// the various tooltip methods on View.
class VIEWS_EXPORT TooltipManager {
 public:
  // Returns the height of tooltips. This should only be invoked from within
  // GetTooltipTextOrigin.
  static int GetTooltipHeight();

  // Returns the default font used by tooltips.
  static gfx::Font GetDefaultFont();

  // Returns the maximum width of the tooltip. |x| and |y| give the location
  // the tooltip is to be displayed on in screen coordinates. |context| is
  // used to determine which gfx::Screen should be used.
  static int GetMaxWidth(int x, int y, gfx::NativeView context);

  TooltipManager() {}
  virtual ~TooltipManager() {}

  // Notification that the view hierarchy has changed in some way.
  virtual void UpdateTooltip() = 0;

  // Invoked when the tooltip text changes for the specified views.
  virtual void TooltipTextChanged(View* view) = 0;

  // Invoked when toolbar icon gets focus.
  virtual void ShowKeyboardTooltip(View* view) = 0;

  // Invoked when toolbar loses focus.
  virtual void HideKeyboardTooltip() = 0;

 protected:
  // Trims the tooltip to fit, setting |text| to the clipped result,
  // |max_width| to the width (in pixels) of the clipped text and |line_count|
  // to the number of lines of text in the tooltip. |x| and |y| give the
  // location of the tooltip in screen coordinates. |context| is used to
  // determine which gfx::Screen should be used.
  static void TrimTooltipToFit(string16* text,
                               int* max_width,
                               int* line_count,
                               int x,
                               int y,
                               gfx::NativeView context);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_TOOLTIP_MANAGER_H_
