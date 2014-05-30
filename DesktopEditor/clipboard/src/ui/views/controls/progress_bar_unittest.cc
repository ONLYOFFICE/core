// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/progress_bar.h"

#include "base/utf_string_conversions.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/accessibility/accessible_view_state.h"

namespace views {

TEST(ProgressBarTest, TooltipTextProperty) {
  ProgressBar bar;
  string16 tooltip = ASCIIToUTF16("Some text");
  EXPECT_FALSE(bar.GetTooltipText(gfx::Point(), &tooltip));
  EXPECT_EQ(string16(), tooltip);
  string16 tooltip_text = ASCIIToUTF16("My progress");
  bar.SetTooltipText(tooltip_text);
  EXPECT_TRUE(bar.GetTooltipText(gfx::Point(), &tooltip));
  EXPECT_EQ(tooltip_text, tooltip);
}

TEST(ProgressBarTest, Accessibility) {
  ProgressBar bar;
  bar.SetValue(62);

  ui::AccessibleViewState state;
  bar.GetAccessibleState(&state);
  EXPECT_EQ(ui::AccessibilityTypes::ROLE_PROGRESSBAR, state.role);
  EXPECT_EQ(string16(), state.name);
  EXPECT_TRUE(ui::AccessibilityTypes::STATE_READONLY & state.state);
}

}  // namespace views
