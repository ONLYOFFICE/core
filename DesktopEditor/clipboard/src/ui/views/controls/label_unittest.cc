// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/i18n/rtl.h"
#include "base/utf_string_conversions.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/accessibility/accessible_view_state.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/gfx/canvas.h"
#include "ui/views/border.h"
#include "ui/views/controls/label.h"

namespace views {

// All text sizing measurements (width and height) should be greater than this.
const int kMinTextDimension = 4;

TEST(LabelTest, FontPropertyCourier) {
  Label label;
  std::string font_name("courier");
  // Note: This test is size dependent since Courier does not support all sizes.
  gfx::Font font(font_name, 26);
  label.SetFont(font);
  gfx::Font font_used = label.font();
#if defined(OS_WIN)
  // On Linux, this results in "Sans" instead of "courier".
  EXPECT_EQ(font_name, font_used.GetFontName());
#endif
  EXPECT_EQ(26, font_used.GetFontSize());
}

TEST(LabelTest, FontPropertyArial) {
  Label label;
  std::string font_name("arial");
  gfx::Font font(font_name, 30);
  label.SetFont(font);
  gfx::Font font_used = label.font();
  EXPECT_EQ(font_name, font_used.GetFontName());
  EXPECT_EQ(30, font_used.GetFontSize());
}

TEST(LabelTest, TextProperty) {
  Label label;
  string16 test_text(ASCIIToUTF16("A random string."));
  label.SetText(test_text);
  EXPECT_EQ(test_text, label.text());
}

TEST(LabelTest, ColorProperty) {
  Label label;
  SkColor color = SkColorSetARGB(20, 40, 10, 5);
  label.SetAutoColorReadabilityEnabled(false);
  label.SetEnabledColor(color);
  EXPECT_EQ(color, label.enabled_color());
}

TEST(LabelTest, AlignmentProperty) {
  Label label;
  bool reverse_alignment = base::i18n::IsRTL();

  label.SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  EXPECT_EQ(reverse_alignment ? gfx::ALIGN_LEFT : gfx::ALIGN_RIGHT,
            label.horizontal_alignment());
  label.SetHorizontalAlignment(gfx::ALIGN_LEFT);
  EXPECT_EQ(reverse_alignment ? gfx::ALIGN_RIGHT : gfx::ALIGN_LEFT,
            label.horizontal_alignment());
  label.SetHorizontalAlignment(gfx::ALIGN_CENTER);
  EXPECT_EQ(gfx::ALIGN_CENTER, label.horizontal_alignment());

  // The label's alignment should not be flipped if the directionality mode is
  // AUTO_DETECT_DIRECTIONALITY.
  label.set_directionality_mode(Label::AUTO_DETECT_DIRECTIONALITY);
  label.SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  EXPECT_EQ(gfx::ALIGN_RIGHT, label.horizontal_alignment());
  label.SetHorizontalAlignment(gfx::ALIGN_LEFT);
  EXPECT_EQ(gfx::ALIGN_LEFT, label.horizontal_alignment());
  label.SetHorizontalAlignment(gfx::ALIGN_CENTER);
  EXPECT_EQ(gfx::ALIGN_CENTER, label.horizontal_alignment());
}

TEST(LabelTest, DirectionalityModeProperty) {
  Label label;
  EXPECT_EQ(Label::USE_UI_DIRECTIONALITY, label.directionality_mode());

  label.set_directionality_mode(Label::AUTO_DETECT_DIRECTIONALITY);
  EXPECT_EQ(Label::AUTO_DETECT_DIRECTIONALITY, label.directionality_mode());

  label.set_directionality_mode(Label::USE_UI_DIRECTIONALITY);
  EXPECT_EQ(Label::USE_UI_DIRECTIONALITY, label.directionality_mode());
}

TEST(LabelTest, MultiLineProperty) {
  Label label;
  EXPECT_FALSE(label.is_multi_line());
  label.SetMultiLine(true);
  EXPECT_TRUE(label.is_multi_line());
  label.SetMultiLine(false);
  EXPECT_FALSE(label.is_multi_line());
}

TEST(LabelTest, TooltipProperty) {
  Label label;
  string16 test_text(ASCIIToUTF16("My cool string."));
  label.SetText(test_text);

  string16 tooltip;
  EXPECT_TRUE(label.GetTooltipText(gfx::Point(), &tooltip));
  EXPECT_EQ(test_text, tooltip);

  string16 tooltip_text(ASCIIToUTF16("The tooltip!"));
  label.SetTooltipText(tooltip_text);
  EXPECT_TRUE(label.GetTooltipText(gfx::Point(), &tooltip));
  EXPECT_EQ(tooltip_text, tooltip);

  string16 empty_text;
  label.SetTooltipText(empty_text);
  EXPECT_TRUE(label.GetTooltipText(gfx::Point(), &tooltip));
  EXPECT_EQ(test_text, tooltip);

  // Make the label big enough to hold the text
  // and expect there to be no tooltip.
  label.SetBounds(0, 0, 1000, 40);
  EXPECT_FALSE(label.GetTooltipText(gfx::Point(), &tooltip));

  // Verify that setting the tooltip still shows it.
  label.SetTooltipText(tooltip_text);
  EXPECT_TRUE(label.GetTooltipText(gfx::Point(), &tooltip));
  EXPECT_EQ(tooltip_text, tooltip);
  // Clear out the tooltip.
  label.SetTooltipText(empty_text);

  // Shrink the bounds and the tooltip should come back.
  label.SetBounds(0, 0, 1, 1);
  EXPECT_TRUE(label.GetTooltipText(gfx::Point(), &tooltip));

  // Make the label multiline and there is no tooltip again.
  label.SetMultiLine(true);
  EXPECT_FALSE(label.GetTooltipText(gfx::Point(), &tooltip));

  // Verify that setting the tooltip still shows it.
  label.SetTooltipText(tooltip_text);
  EXPECT_TRUE(label.GetTooltipText(gfx::Point(), &tooltip));
  EXPECT_EQ(tooltip_text, tooltip);
  // Clear out the tooltip.
  label.SetTooltipText(empty_text);
}

TEST(LabelTest, Accessibility) {
  Label label;
  string16 test_text(ASCIIToUTF16("My special text."));
  label.SetText(test_text);

  ui::AccessibleViewState state;
  label.GetAccessibleState(&state);
  EXPECT_EQ(ui::AccessibilityTypes::ROLE_STATICTEXT, state.role);
  EXPECT_EQ(test_text, state.name);
  EXPECT_TRUE(ui::AccessibilityTypes::STATE_READONLY & state.state);
}

TEST(LabelTest, SingleLineSizing) {
  Label label;
  string16 test_text(ASCIIToUTF16("A not so random string in one line."));
  label.SetText(test_text);

  // GetPreferredSize
  gfx::Size required_size = label.GetPreferredSize();
  EXPECT_GT(required_size.height(), kMinTextDimension);
  EXPECT_GT(required_size.width(), kMinTextDimension);

  // Test everything with borders.
  gfx::Insets border(10, 20, 30, 40);
  label.set_border(Border::CreateEmptyBorder(border.top(),
                                             border.left(),
                                             border.bottom(),
                                             border.right()));

  // GetPreferredSize and borders.
  label.SetBounds(0, 0, 0, 0);
  gfx::Size required_size_with_border = label.GetPreferredSize();
  EXPECT_EQ(required_size_with_border.height(),
            required_size.height() + border.height());
  EXPECT_EQ(required_size_with_border.width(),
            required_size.width() + border.width());
}

TEST(LabelTest, MultilineSmallAvailableWidthSizing) {
  Label label;
  string16 test_text(ASCIIToUTF16("Too Wide."));

  label.SetMultiLine(true);
  label.SetAllowCharacterBreak(true);
  label.SetElideBehavior(Label::ELIDE_AT_END);
  label.SetText(test_text);

  // Check that Label can be laid out at a variety of small sizes,
  // splitting the words into up to one character per line if necessary.
  // Incorrect word splitting may cause infinite loops in text layout.
  gfx::Size required_size = label.GetPreferredSize();
  for (int i = 1; i < required_size.width(); ++i) {
    EXPECT_GT(label.GetHeightForWidth(i), 0);
  }
}

TEST(LabelTest, MultiLineSizing) {
  Label label;
  label.set_focusable(false);
  string16 test_text(
      ASCIIToUTF16("A random string\nwith multiple lines\nand returns!"));
  label.SetText(test_text);
  label.SetMultiLine(true);

  // GetPreferredSize
  gfx::Size required_size = label.GetPreferredSize();
  EXPECT_GT(required_size.height(), kMinTextDimension);
  EXPECT_GT(required_size.width(), kMinTextDimension);

  // SizeToFit with unlimited width.
  label.SizeToFit(0);
  int required_width = label.GetLocalBounds().width();
  EXPECT_GT(required_width, kMinTextDimension);

  // SizeToFit with limited width.
  label.SizeToFit(required_width - 1);
  int constrained_width = label.GetLocalBounds().width();
#if defined(OS_WIN)
  // Canvas::SizeStringInt (in ui/gfx/canvas_linux.cc)
  // has to be fixed to return the size that fits to given width/height.
  EXPECT_LT(constrained_width, required_width);
#endif
  EXPECT_GT(constrained_width, kMinTextDimension);

  // Change the width back to the desire width.
  label.SizeToFit(required_width);
  EXPECT_EQ(required_width, label.GetLocalBounds().width());

  // General tests for GetHeightForWidth.
  int required_height = label.GetHeightForWidth(required_width);
  EXPECT_GT(required_height, kMinTextDimension);
  int height_for_constrained_width = label.GetHeightForWidth(constrained_width);
#if defined(OS_WIN)
  // Canvas::SizeStringInt (in ui/gfx/canvas_linux.cc)
  // has to be fixed to return the size that fits to given width/height.
  EXPECT_GT(height_for_constrained_width, required_height);
#endif
  // Using the constrained width or the required_width - 1 should give the
  // same result for the height because the constrainted width is the tight
  // width when given "required_width - 1" as the max width.
  EXPECT_EQ(height_for_constrained_width,
            label.GetHeightForWidth(required_width - 1));

  // Test everything with borders.
  gfx::Insets border(10, 20, 30, 40);
  label.set_border(Border::CreateEmptyBorder(border.top(),
                                             border.left(),
                                             border.bottom(),
                                             border.right()));

  // SizeToFit and borders.
  label.SizeToFit(0);
  int required_width_with_border = label.GetLocalBounds().width();
  EXPECT_EQ(required_width_with_border, required_width + border.width());

  // GetHeightForWidth and borders.
  int required_height_with_border =
      label.GetHeightForWidth(required_width_with_border);
  EXPECT_EQ(required_height_with_border, required_height + border.height());

  // Test that the border width is subtracted before doing the height
  // calculation.  If it is, then the height will grow when width
  // is shrunk.
  int height1 = label.GetHeightForWidth(required_width_with_border - 1);
#if defined(OS_WIN)
  // Canvas::SizeStringInt (in ui/gfx/canvas_linux.cc)
  // has to be fixed to return the size that fits to given width/height.
  EXPECT_GT(height1, required_height_with_border);
#endif
  EXPECT_EQ(height1, height_for_constrained_width + border.height());

  // GetPreferredSize and borders.
  label.SetBounds(0, 0, 0, 0);
  gfx::Size required_size_with_border = label.GetPreferredSize();
  EXPECT_EQ(required_size_with_border.height(),
            required_size.height() + border.height());
  EXPECT_EQ(required_size_with_border.width(),
            required_size.width() + border.width());
}

TEST(LabelTest, AutoDetectDirectionality) {
  Label label;
  label.set_directionality_mode(Label::AUTO_DETECT_DIRECTIONALITY);

  // Test text starts with RTL character.
  string16 test_text(WideToUTF16(L"  \x5d0\x5d1\x5d2 abc"));
  label.SetText(test_text);
  gfx::Size required_size(label.GetPreferredSize());
  gfx::Size extra(22, 8);
  label.SetBounds(0,
                  0,
                  required_size.width() + extra.width(),
                  required_size.height() + extra.height());

  string16 paint_text;
  gfx::Rect text_bounds;
  int flags;
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(gfx::Canvas::FORCE_RTL_DIRECTIONALITY |
            gfx::Canvas::TEXT_ALIGN_CENTER, flags);

  // Test text starts with LTR character.
  test_text = (WideToUTF16(L"ltr \x5d0\x5d1\x5d2 abc"));
  label.SetText(test_text);
  required_size = label.GetPreferredSize();
  label.SetBounds(0,
                  0,
                  required_size.width() + extra.width(),
                  required_size.height() + extra.height());

  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(gfx::Canvas::FORCE_LTR_DIRECTIONALITY |
            gfx::Canvas::TEXT_ALIGN_CENTER, flags);
}

TEST(LabelTest, DrawSingleLineString) {
  Label label;
  label.set_focusable(false);

  // Turn off mirroring so that we don't need to figure out if
  // align right really means align left.
  label.set_directionality_mode(Label::AUTO_DETECT_DIRECTIONALITY);

  string16 test_text(ASCIIToUTF16("Here's a string with no returns."));
  label.SetText(test_text);
  gfx::Size required_size(label.GetPreferredSize());
  gfx::Size extra(22, 8);
  label.SetBounds(0,
                  0,
                  required_size.width() + extra.width(),
                  required_size.height() + extra.height());

  // Do some basic verifications for all three alignments.
  string16 paint_text;
  gfx::Rect text_bounds;
  int flags;

  // Centered text.
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  // The text should be centered horizontally and vertically.
  EXPECT_EQ(extra.width() / 2, text_bounds.x());
  EXPECT_EQ(extra.height() / 2 , text_bounds.y());
  EXPECT_EQ(required_size.width(), text_bounds.width());
  EXPECT_EQ(required_size.height(), text_bounds.height());
  EXPECT_EQ(gfx::Canvas::FORCE_LTR_DIRECTIONALITY |
            gfx::Canvas::TEXT_ALIGN_CENTER, flags);

  // Left aligned text.
  label.SetHorizontalAlignment(gfx::ALIGN_LEFT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  // The text should be left aligned horizontally and centered vertically.
  EXPECT_EQ(0, text_bounds.x());
  EXPECT_EQ(extra.height() / 2 , text_bounds.y());
  EXPECT_EQ(required_size.width(), text_bounds.width());
  EXPECT_EQ(required_size.height(), text_bounds.height());
  EXPECT_EQ(gfx::Canvas::FORCE_LTR_DIRECTIONALITY |
            gfx::Canvas::TEXT_ALIGN_LEFT, flags);

  // Right aligned text.
  label.SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  // The text should be right aligned horizontally and centered vertically.
  EXPECT_EQ(extra.width(), text_bounds.x());
  EXPECT_EQ(extra.height() / 2 , text_bounds.y());
  EXPECT_EQ(required_size.width(), text_bounds.width());
  EXPECT_EQ(required_size.height(), text_bounds.height());
  EXPECT_EQ(gfx::Canvas::FORCE_LTR_DIRECTIONALITY |
            gfx::Canvas::TEXT_ALIGN_RIGHT, flags);

  // Test single line drawing with a border.
  gfx::Insets border(39, 34, 8, 96);
  label.set_border(Border::CreateEmptyBorder(border.top(),
                                             border.left(),
                                             border.bottom(),
                                             border.right()));

  gfx::Size required_size_with_border(label.GetPreferredSize());
  EXPECT_EQ(required_size.width() + border.width(),
            required_size_with_border.width());
  EXPECT_EQ(required_size.height() + border.height(),
            required_size_with_border.height());
  label.SetBounds(0,
                  0,
                  required_size_with_border.width() + extra.width(),
                  required_size_with_border.height() + extra.height());

  // Centered text with border.
  label.SetHorizontalAlignment(gfx::ALIGN_CENTER);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  // The text should be centered horizontally and vertically within the border.
  EXPECT_EQ(border.left() + extra.width() / 2, text_bounds.x());
  EXPECT_EQ(border.top() + extra.height() / 2 , text_bounds.y());
  EXPECT_EQ(required_size.width(), text_bounds.width());
  EXPECT_EQ(required_size.height(), text_bounds.height());
  EXPECT_EQ(gfx::Canvas::FORCE_LTR_DIRECTIONALITY |
            gfx::Canvas::TEXT_ALIGN_CENTER, flags);

  // Left aligned text with border.
  label.SetHorizontalAlignment(gfx::ALIGN_LEFT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  // The text should be left aligned horizontally and centered vertically.
  EXPECT_EQ(border.left(), text_bounds.x());
  EXPECT_EQ(border.top() + extra.height() / 2 , text_bounds.y());
  EXPECT_EQ(required_size.width(), text_bounds.width());
  EXPECT_EQ(required_size.height(), text_bounds.height());
  EXPECT_EQ(gfx::Canvas::FORCE_LTR_DIRECTIONALITY |
            gfx::Canvas::TEXT_ALIGN_LEFT, flags);

  // Right aligned text.
  label.SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  // The text should be right aligned horizontally and centered vertically.
  EXPECT_EQ(border.left() + extra.width(), text_bounds.x());
  EXPECT_EQ(border.top() + extra.height() / 2 , text_bounds.y());
  EXPECT_EQ(required_size.width(), text_bounds.width());
  EXPECT_EQ(required_size.height(), text_bounds.height());
  EXPECT_EQ(gfx::Canvas::FORCE_LTR_DIRECTIONALITY |
            gfx::Canvas::TEXT_ALIGN_RIGHT, flags);
}

// On Linux the underlying pango routines require a max height in order to
// ellide multiline text. So until that can be resolved, we set all
// multiline lables to not ellide in Linux only.
TEST(LabelTest, DrawMultiLineString) {
  Label label;
  label.set_focusable(false);

  // Turn off mirroring so that we don't need to figure out if
  // align right really means align left.
  label.set_directionality_mode(Label::AUTO_DETECT_DIRECTIONALITY);

  string16 test_text(ASCIIToUTF16("Another string\nwith returns\n\n!"));
  label.SetText(test_text);
  label.SetMultiLine(true);
  label.SizeToFit(0);
  gfx::Size extra(50, 10);
  label.SetBounds(label.x(),
                  label.y(),
                  label.width() + extra.width(),
                  label.height() + extra.height());

  // Do some basic verifications for all three alignments.
  string16 paint_text;
  gfx::Rect text_bounds;
  int flags;
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  EXPECT_EQ(extra.width() / 2, text_bounds.x());
  EXPECT_EQ(extra.height() / 2, text_bounds.y());
  EXPECT_GT(text_bounds.width(), kMinTextDimension);
  EXPECT_GT(text_bounds.height(), kMinTextDimension);
  int expected_flags = gfx::Canvas::MULTI_LINE |
                       gfx::Canvas::TEXT_ALIGN_CENTER |
                       gfx::Canvas::FORCE_LTR_DIRECTIONALITY;
#if defined(OS_WIN)
  EXPECT_EQ(expected_flags, flags);
#else
  EXPECT_EQ(expected_flags | gfx::Canvas::NO_ELLIPSIS, flags);
#endif
  gfx::Rect center_bounds(text_bounds);

  label.SetHorizontalAlignment(gfx::ALIGN_LEFT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  EXPECT_EQ(0, text_bounds.x());
  EXPECT_EQ(extra.height() / 2, text_bounds.y());
  EXPECT_GT(text_bounds.width(), kMinTextDimension);
  EXPECT_GT(text_bounds.height(), kMinTextDimension);
  expected_flags = gfx::Canvas::MULTI_LINE |
                   gfx::Canvas::TEXT_ALIGN_LEFT |
                   gfx::Canvas::FORCE_LTR_DIRECTIONALITY;
#if defined(OS_WIN)
  EXPECT_EQ(expected_flags, flags);
#else
  EXPECT_EQ(expected_flags | gfx::Canvas::NO_ELLIPSIS, flags);
#endif

  label.SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  EXPECT_EQ(extra.width(), text_bounds.x());
  EXPECT_EQ(extra.height() / 2, text_bounds.y());
  EXPECT_GT(text_bounds.width(), kMinTextDimension);
  EXPECT_GT(text_bounds.height(), kMinTextDimension);
  expected_flags = gfx::Canvas::MULTI_LINE |
                   gfx::Canvas::TEXT_ALIGN_RIGHT |
                   gfx::Canvas::FORCE_LTR_DIRECTIONALITY;
#if defined(OS_WIN)
  EXPECT_EQ(expected_flags, flags);
#else
  EXPECT_EQ(expected_flags | gfx::Canvas::NO_ELLIPSIS, flags);
#endif

  // Test multiline drawing with a border.
  gfx::Insets border(19, 92, 23, 2);
  label.set_border(Border::CreateEmptyBorder(border.top(),
                                             border.left(),
                                             border.bottom(),
                                             border.right()));
  label.SizeToFit(0);
  label.SetBounds(label.x(),
                  label.y(),
                  label.width() + extra.width(),
                  label.height() + extra.height());

  label.SetHorizontalAlignment(gfx::ALIGN_CENTER);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  EXPECT_EQ(border.left() + extra.width() / 2, text_bounds.x());
  EXPECT_EQ(border.top() + extra.height() / 2, text_bounds.y());
  EXPECT_EQ(center_bounds.width(), text_bounds.width());
  EXPECT_EQ(center_bounds.height(), text_bounds.height());
  expected_flags = gfx::Canvas::MULTI_LINE |
                   gfx::Canvas::TEXT_ALIGN_CENTER |
                   gfx::Canvas::FORCE_LTR_DIRECTIONALITY;
#if defined(OS_WIN)
  EXPECT_EQ(expected_flags, flags);
#else
  EXPECT_EQ(expected_flags | gfx::Canvas::NO_ELLIPSIS, flags);
#endif

  label.SetHorizontalAlignment(gfx::ALIGN_LEFT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  EXPECT_EQ(border.left(), text_bounds.x());
  EXPECT_EQ(border.top() + extra.height() / 2, text_bounds.y());
  EXPECT_EQ(center_bounds.width(), text_bounds.width());
  EXPECT_EQ(center_bounds.height(), text_bounds.height());
  expected_flags = gfx::Canvas::MULTI_LINE |
                   gfx::Canvas::TEXT_ALIGN_LEFT |
                   gfx::Canvas::FORCE_LTR_DIRECTIONALITY;
#if defined(OS_WIN)
  EXPECT_EQ(expected_flags, flags);
#else
  EXPECT_EQ(expected_flags | gfx::Canvas::NO_ELLIPSIS, flags);
#endif

  label.SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  EXPECT_EQ(extra.width() + border.left(), text_bounds.x());
  EXPECT_EQ(border.top() + extra.height() / 2, text_bounds.y());
  EXPECT_EQ(center_bounds.width(), text_bounds.width());
  EXPECT_EQ(center_bounds.height(), text_bounds.height());
  expected_flags = gfx::Canvas::MULTI_LINE |
                   gfx::Canvas::TEXT_ALIGN_RIGHT |
                   gfx::Canvas::FORCE_LTR_DIRECTIONALITY;
#if defined(OS_WIN)
  EXPECT_EQ(expected_flags, flags);
#else
  EXPECT_EQ(expected_flags | gfx::Canvas::NO_ELLIPSIS, flags);
#endif
}

TEST(LabelTest, DrawSingleLineStringInRTL) {
  Label label;
  label.set_focusable(false);

  std::string locale = l10n_util::GetApplicationLocale("");
  base::i18n::SetICUDefaultLocale("he");

  string16 test_text(ASCIIToUTF16("Here's a string with no returns."));
  label.SetText(test_text);
  gfx::Size required_size(label.GetPreferredSize());
  gfx::Size extra(22, 8);
  label.SetBounds(0,
                  0,
                  required_size.width() + extra.width(),
                  required_size.height() + extra.height());

  // Do some basic verifications for all three alignments.
  string16 paint_text;
  gfx::Rect text_bounds;
  int flags;

  // Centered text.
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  // The text should be centered horizontally and vertically.
  EXPECT_EQ(extra.width() / 2, text_bounds.x());
  EXPECT_EQ(extra.height() / 2 , text_bounds.y());
  EXPECT_EQ(required_size.width(), text_bounds.width());
  EXPECT_EQ(required_size.height(), text_bounds.height());
  EXPECT_EQ(gfx::Canvas::TEXT_ALIGN_CENTER, flags);

  // ALIGN_LEFT label.
  label.SetHorizontalAlignment(gfx::ALIGN_LEFT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  // The text should be right aligned horizontally and centered vertically.
  EXPECT_EQ(extra.width(), text_bounds.x());
  EXPECT_EQ(extra.height() / 2 , text_bounds.y());
  EXPECT_EQ(required_size.width(), text_bounds.width());
  EXPECT_EQ(required_size.height(), text_bounds.height());
  EXPECT_EQ(gfx::Canvas::TEXT_ALIGN_RIGHT, flags);

  // ALIGN_RIGHT label.
  label.SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  // The text should be left aligned horizontally and centered vertically.
  EXPECT_EQ(0, text_bounds.x());
  EXPECT_EQ(extra.height() / 2 , text_bounds.y());
  EXPECT_EQ(required_size.width(), text_bounds.width());
  EXPECT_EQ(required_size.height(), text_bounds.height());
  EXPECT_EQ(gfx::Canvas::TEXT_ALIGN_LEFT, flags);


  // Test single line drawing with a border.
  gfx::Insets border(39, 34, 8, 96);
  label.set_border(Border::CreateEmptyBorder(border.top(),
                                             border.left(),
                                             border.bottom(),
                                             border.right()));

  gfx::Size required_size_with_border(label.GetPreferredSize());
  EXPECT_EQ(required_size.width() + border.width(),
            required_size_with_border.width());
  EXPECT_EQ(required_size.height() + border.height(),
            required_size_with_border.height());
  label.SetBounds(0,
                  0,
                  required_size_with_border.width() + extra.width(),
                  required_size_with_border.height() + extra.height());

  // Centered text with border.
  label.SetHorizontalAlignment(gfx::ALIGN_CENTER);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  // The text should be centered horizontally and vertically within the border.
  EXPECT_EQ(border.left() + extra.width() / 2, text_bounds.x());
  EXPECT_EQ(border.top() + extra.height() / 2 , text_bounds.y());
  EXPECT_EQ(required_size.width(), text_bounds.width());
  EXPECT_EQ(required_size.height(), text_bounds.height());
  EXPECT_EQ(gfx::Canvas::TEXT_ALIGN_CENTER, flags);

  // ALIGN_LEFT text with border.
  label.SetHorizontalAlignment(gfx::ALIGN_LEFT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  // The text should be right aligned horizontally and centered vertically.
  EXPECT_EQ(border.left() + extra.width(), text_bounds.x());
  EXPECT_EQ(border.top() + extra.height() / 2 , text_bounds.y());
  EXPECT_EQ(required_size.width(), text_bounds.width());
  EXPECT_EQ(required_size.height(), text_bounds.height());
  EXPECT_EQ(gfx::Canvas::TEXT_ALIGN_RIGHT, flags);

  // ALIGN_RIGHT text.
  label.SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  // The text should be left aligned horizontally and centered vertically.
  EXPECT_EQ(border.left(), text_bounds.x());
  EXPECT_EQ(border.top() + extra.height() / 2 , text_bounds.y());
  EXPECT_EQ(required_size.width(), text_bounds.width());
  EXPECT_EQ(required_size.height(), text_bounds.height());
  EXPECT_EQ(gfx::Canvas::TEXT_ALIGN_LEFT, flags);

  // Reset locale.
  base::i18n::SetICUDefaultLocale(locale);
}

// On Linux the underlying pango routines require a max height in order to
// ellide multiline text. So until that can be resolved, we set all
// multiline lables to not ellide in Linux only.
TEST(LabelTest, DrawMultiLineStringInRTL) {
  Label label;
  label.set_focusable(false);

  // Test for RTL.
  std::string locale = l10n_util::GetApplicationLocale("");
  base::i18n::SetICUDefaultLocale("he");

  string16 test_text(ASCIIToUTF16("Another string\nwith returns\n\n!"));
  label.SetText(test_text);
  label.SetMultiLine(true);
  label.SizeToFit(0);
  gfx::Size extra(50, 10);
  label.SetBounds(label.x(),
                  label.y(),
                  label.width() + extra.width(),
                  label.height() + extra.height());

  // Do some basic verifications for all three alignments.
  string16 paint_text;
  gfx::Rect text_bounds;
  int flags;
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  EXPECT_EQ(extra.width() / 2, text_bounds.x());
  EXPECT_EQ(extra.height() / 2, text_bounds.y());
  EXPECT_GT(text_bounds.width(), kMinTextDimension);
  EXPECT_GT(text_bounds.height(), kMinTextDimension);
#if defined(OS_WIN)
  EXPECT_EQ(gfx::Canvas::MULTI_LINE | gfx::Canvas::TEXT_ALIGN_CENTER, flags);
#else
  EXPECT_EQ(
      gfx::Canvas::MULTI_LINE |
      gfx::Canvas::TEXT_ALIGN_CENTER |
      gfx::Canvas::NO_ELLIPSIS,
      flags);
#endif
  gfx::Rect center_bounds(text_bounds);

  label.SetHorizontalAlignment(gfx::ALIGN_LEFT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  EXPECT_EQ(extra.width(), text_bounds.x());
  EXPECT_EQ(extra.height() / 2, text_bounds.y());
  EXPECT_GT(text_bounds.width(), kMinTextDimension);
  EXPECT_GT(text_bounds.height(), kMinTextDimension);
#if defined(OS_WIN)
  EXPECT_EQ(gfx::Canvas::MULTI_LINE | gfx::Canvas::TEXT_ALIGN_RIGHT, flags);
#else
  EXPECT_EQ(
      gfx::Canvas::MULTI_LINE |
      gfx::Canvas::TEXT_ALIGN_RIGHT |
      gfx::Canvas::NO_ELLIPSIS,
      flags);
#endif

  label.SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  EXPECT_EQ(0, text_bounds.x());
  EXPECT_EQ(extra.height() / 2, text_bounds.y());
  EXPECT_GT(text_bounds.width(), kMinTextDimension);
  EXPECT_GT(text_bounds.height(), kMinTextDimension);
#if defined(OS_WIN)
  EXPECT_EQ(gfx::Canvas::MULTI_LINE | gfx::Canvas::TEXT_ALIGN_LEFT, flags);
#else
  EXPECT_EQ(
      gfx::Canvas::MULTI_LINE |
      gfx::Canvas::TEXT_ALIGN_LEFT |
      gfx::Canvas::NO_ELLIPSIS,
      flags);
#endif

  // Test multiline drawing with a border.
  gfx::Insets border(19, 92, 23, 2);
  label.set_border(Border::CreateEmptyBorder(border.top(),
                                             border.left(),
                                             border.bottom(),
                                             border.right()));
  label.SizeToFit(0);
  label.SetBounds(label.x(),
                  label.y(),
                  label.width() + extra.width(),
                  label.height() + extra.height());

  label.SetHorizontalAlignment(gfx::ALIGN_CENTER);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  EXPECT_EQ(border.left() + extra.width() / 2, text_bounds.x());
  EXPECT_EQ(border.top() + extra.height() / 2, text_bounds.y());
  EXPECT_EQ(center_bounds.width(), text_bounds.width());
  EXPECT_EQ(center_bounds.height(), text_bounds.height());
#if defined(OS_WIN)
  EXPECT_EQ(gfx::Canvas::MULTI_LINE | gfx::Canvas::TEXT_ALIGN_CENTER, flags);
#else
  EXPECT_EQ(
      gfx::Canvas::MULTI_LINE |
      gfx::Canvas::TEXT_ALIGN_CENTER |
      gfx::Canvas::NO_ELLIPSIS,
      flags);
#endif

  label.SetHorizontalAlignment(gfx::ALIGN_LEFT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  EXPECT_EQ(border.left() + extra.width(), text_bounds.x());
  EXPECT_EQ(border.top() + extra.height() / 2, text_bounds.y());
  EXPECT_EQ(center_bounds.width(), text_bounds.width());
  EXPECT_EQ(center_bounds.height(), text_bounds.height());
#if defined(OS_WIN)
  EXPECT_EQ(gfx::Canvas::MULTI_LINE | gfx::Canvas::TEXT_ALIGN_RIGHT, flags);
#else
  EXPECT_EQ(
      gfx::Canvas::MULTI_LINE |
      gfx::Canvas::TEXT_ALIGN_RIGHT |
      gfx::Canvas::NO_ELLIPSIS,
      flags);
#endif

  label.SetHorizontalAlignment(gfx::ALIGN_RIGHT);
  paint_text.clear();
  text_bounds.SetRect(0, 0, 0, 0);
  label.CalculateDrawStringParams(&paint_text, &text_bounds, &flags);
  EXPECT_EQ(test_text, paint_text);
  EXPECT_EQ(border.left(), text_bounds.x());
  EXPECT_EQ(border.top() + extra.height() / 2, text_bounds.y());
  EXPECT_EQ(center_bounds.width(), text_bounds.width());
  EXPECT_EQ(center_bounds.height(), text_bounds.height());
#if defined(OS_WIN)
  EXPECT_EQ(gfx::Canvas::MULTI_LINE | gfx::Canvas::TEXT_ALIGN_LEFT, flags);
#else
  EXPECT_EQ(
      gfx::Canvas::MULTI_LINE |
      gfx::Canvas::TEXT_ALIGN_LEFT |
      gfx::Canvas::NO_ELLIPSIS,
      flags);
#endif

  // Reset Locale
  base::i18n::SetICUDefaultLocale(locale);
}

// Check that we disable subpixel rendering when a transparent background is
// being used.
TEST(LabelTest, DisableSubpixelRendering) {
  Label label;
  label.SetBackgroundColor(SK_ColorWHITE);
  EXPECT_EQ(
      0, label.ComputeDrawStringFlags() & gfx::Canvas::NO_SUBPIXEL_RENDERING);

  label.SetBackgroundColor(SkColorSetARGB(64, 255, 255, 255));
  EXPECT_EQ(
      gfx::Canvas::NO_SUBPIXEL_RENDERING,
      label.ComputeDrawStringFlags() & gfx::Canvas::NO_SUBPIXEL_RENDERING);
}

// Check that labels support GetTooltipHandlerForPoint.
TEST(LabelTest, GetTooltipHandlerForPoint) {
  Label label;
  label.SetText(
      ASCIIToUTF16("A string that's long enough to exceed the bounds"));
  label.SetBounds(0, 0, 10, 10);
  // There's a default tooltip if the text is too big to fit.
  EXPECT_EQ(&label, label.GetTooltipHandlerForPoint(gfx::Point(2, 2)));

  // If there's no default tooltip, this should return NULL.
  label.SetBounds(0, 0, 500, 50);
  EXPECT_FALSE(label.GetTooltipHandlerForPoint(gfx::Point(2, 2)));

  label.SetTooltipText(ASCIIToUTF16("a tooltip"));
  // If the point hits the label, and tooltip is set, the label should be
  // returned as its tooltip handler.
  EXPECT_EQ(&label, label.GetTooltipHandlerForPoint(gfx::Point(2, 2)));

  // Additionally, GetTooltipHandlerForPoint should verify that the label
  // actually contains the point.
  EXPECT_FALSE(label.GetTooltipHandlerForPoint(gfx::Point(2, 51)));
  EXPECT_FALSE(label.GetTooltipHandlerForPoint(gfx::Point(-1, 20)));

  // GetTooltipHandlerForPoint works should work in child bounds.
  label.SetBounds(2, 2, 10, 10);
  EXPECT_EQ(&label, label.GetTooltipHandlerForPoint(gfx::Point(1, 5)));
  EXPECT_FALSE(label.GetTooltipHandlerForPoint(gfx::Point(3, 11)));
}

}  // namespace views
