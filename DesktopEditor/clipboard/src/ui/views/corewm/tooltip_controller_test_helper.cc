// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/corewm/tooltip_controller_test_helper.h"

#include "ui/aura/window.h"
#include "ui/views/corewm/tooltip_controller.h"

namespace views {
namespace corewm {
namespace test {

TooltipControllerTestHelper::TooltipControllerTestHelper(
    TooltipController* controller)
    : controller_(controller) {
}

TooltipControllerTestHelper::~TooltipControllerTestHelper() {
}

string16 TooltipControllerTestHelper::GetTooltipText() {
  return controller_->tooltip_text_;
}

aura::Window* TooltipControllerTestHelper::GetTooltipWindow() {
  return controller_->tooltip_window_;
}

void TooltipControllerTestHelper::FireTooltipTimer() {
  controller_->TooltipTimerFired();
}

bool TooltipControllerTestHelper::IsTooltipTimerRunning() {
  return controller_->tooltip_timer_.IsRunning();
}

void TooltipControllerTestHelper::FireTooltipShownTimer() {
  controller_->tooltip_shown_timer_.Stop();
  controller_->TooltipShownTimerFired();
}

bool TooltipControllerTestHelper::IsTooltipShownTimerRunning() {
  return controller_->tooltip_shown_timer_.IsRunning();
}

bool TooltipControllerTestHelper::IsTooltipVisible() {
  return controller_->IsTooltipVisible();
}

// static
void TooltipControllerTestHelper::TrimTooltipToFit(int max_width,
                                                   string16* text,
                                                   int* width,
                                                   int* line_count) {
  TooltipController::TrimTooltipToFit(max_width, text, width, line_count);
}

TooltipTestView::TooltipTestView() {
}

TooltipTestView::~TooltipTestView() {
}

bool TooltipTestView::GetTooltipText(const gfx::Point& p,
                                     string16* tooltip) const {
  *tooltip = tooltip_text_;
  return true;
}

}  // namespace test
}  // namespace corewm
}  // namespace views
