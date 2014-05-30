// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_TOOLTIP_CONTROLLER_H_
#define UI_VIEWS_COREWM_TOOLTIP_CONTROLLER_H_

#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/timer.h"
#include "ui/aura/client/tooltip_client.h"
#include "ui/aura/window_observer.h"
#include "ui/base/events/event_handler.h"
#include "ui/gfx/point.h"
#include "ui/gfx/screen_type_delegate.h"
#include "ui/views/views_export.h"

namespace aura {
class Window;
}

namespace views {
namespace corewm {

namespace test {
class TooltipControllerTestHelper;
}  // namespace test

// TooltipController provides tooltip functionality for aura shell.
class VIEWS_EXPORT TooltipController : public aura::client::TooltipClient,
                                       public ui::EventHandler,
                                       public aura::WindowObserver {
 public:
  explicit TooltipController(gfx::ScreenType screen_type);
  virtual ~TooltipController();

  // Overridden from aura::client::TooltipClient.
  virtual void UpdateTooltip(aura::Window* target) OVERRIDE;
  virtual void SetTooltipsEnabled(bool enable) OVERRIDE;

  // Overridden from ui::EventHandler.
  virtual void OnKeyEvent(ui::KeyEvent* event) OVERRIDE;
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnCancelMode(ui::CancelModeEvent* event) OVERRIDE;

  // Overridden from aura::WindowObserver.
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;

  const gfx::Point& mouse_location() const { return curr_mouse_loc_; }

 private:
  friend class test::TooltipControllerTestHelper;

  class Tooltip;

  // Returns the max width of the tooltip when shown at the specified location.
  int GetMaxWidth(const gfx::Point& location) const;

  // Returns the bounds to fit the tooltip in.
  gfx::Rect GetBoundsForTooltip(const gfx::Point& origin) const;

  // Trims the tooltip to fit in the width |max_width|, setting |text| to the
  // clipped result, |width| to the width (in pixels) of the clipped text
  // and |line_count| to the number of lines of text in the tooltip. |x| and |y|
  // give the location of the tooltip in screen coordinates. |max_width| comes
  // from GetMaxWidth().
  static void TrimTooltipToFit(int max_width,
                               string16* text,
                               int* width,
                               int* line_count);

  void TooltipTimerFired();
  void TooltipShownTimerFired();

  // Updates the tooltip if required (if there is any change in the tooltip
  // text or the aura::Window.
  void UpdateIfRequired();

  // Only used in tests.
  bool IsTooltipVisible();

  bool IsDragDropInProgress();

  // This lazily creates the Tooltip instance so that the tooltip window will
  // be initialized with appropriate drop shadows.
  Tooltip* GetTooltip();

  // Returns true if the cursor is visible.
  bool IsCursorVisible();

  const gfx::ScreenType screen_type_;

  aura::Window* tooltip_window_;
  string16 tooltip_text_;

  // These fields are for tracking state when the user presses a mouse button.
  aura::Window* tooltip_window_at_mouse_press_;
  string16 tooltip_text_at_mouse_press_;
  bool mouse_pressed_;

  scoped_ptr<Tooltip> tooltip_;

  base::RepeatingTimer<TooltipController> tooltip_timer_;

  // Timer to timeout the life of an on-screen tooltip. We hide the tooltip when
  // this timer fires.
  base::OneShotTimer<TooltipController> tooltip_shown_timer_;

  gfx::Point curr_mouse_loc_;

  bool tooltips_enabled_;

  DISALLOW_COPY_AND_ASSIGN(TooltipController);
};

}  // namespace corewm
}  // namespace views

#endif  // UI_VIEWS_COREWM_TOOLTIP_CONTROLLER_H_
