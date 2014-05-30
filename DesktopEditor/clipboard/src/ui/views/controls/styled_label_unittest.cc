// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/utf_string_conversions.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/views/controls/link.h"
#include "ui/views/controls/styled_label.h"
#include "ui/views/controls/styled_label_listener.h"

namespace views {

class StyledLabelTest : public testing::Test, public StyledLabelListener {
 public:
  StyledLabelTest() {}
  virtual ~StyledLabelTest() {}

  // StyledLabelListener implementation.
  virtual void StyledLabelLinkClicked(const ui::Range& range,
                                      int event_flags) OVERRIDE {}

 protected:
  StyledLabel* styled() { return styled_.get(); }

  void InitStyledLabel(const std::string& ascii_text) {
    styled_.reset(new StyledLabel(ASCIIToUTF16(ascii_text), this));
  }

  int StyledLabelContentHeightForWidth(int w) {
    return styled_->GetHeightForWidth(w) - styled_->GetInsets().height();
  }

 private:
  scoped_ptr<StyledLabel> styled_;

  DISALLOW_COPY_AND_ASSIGN(StyledLabelTest);
};

TEST_F(StyledLabelTest, NoWrapping) {
  const std::string text("This is a test block of text");
  InitStyledLabel(text);
  Label label(ASCIIToUTF16(text));
  const gfx::Size label_preferred_size = label.GetPreferredSize();
  EXPECT_EQ(label_preferred_size.height(),
            StyledLabelContentHeightForWidth(label_preferred_size.width() * 2));
}

TEST_F(StyledLabelTest, TrailingWhitespaceiIgnored) {
  const std::string text("This is a test block of text   ");
  InitStyledLabel(text);

  styled()->SetBounds(0, 0, 1000, 1000);
  styled()->Layout();

  ASSERT_EQ(1, styled()->child_count());
  ASSERT_EQ(std::string(Label::kViewClassName),
            styled()->child_at(0)->GetClassName());
  EXPECT_EQ(ASCIIToUTF16("This is a test block of text"),
            static_cast<Label*>(styled()->child_at(0))->text());
}

TEST_F(StyledLabelTest, RespectLeadingWhitespace) {
  const std::string text("   This is a test block of text");
  InitStyledLabel(text);

  styled()->SetBounds(0, 0, 1000, 1000);
  styled()->Layout();

  ASSERT_EQ(1, styled()->child_count());
  ASSERT_EQ(std::string(Label::kViewClassName),
            styled()->child_at(0)->GetClassName());
  EXPECT_EQ(ASCIIToUTF16("   This is a test block of text"),
            static_cast<Label*>(styled()->child_at(0))->text());
}

TEST_F(StyledLabelTest, FirstLineNotEmptyWhenLeadingWhitespaceTooLong) {
  const std::string text("                                     a");
  InitStyledLabel(text);

  Label label(ASCIIToUTF16(text));
  gfx::Size label_preferred_size = label.GetPreferredSize();

  styled()->SetBounds(0, 0, label_preferred_size.width() / 2, 1000);
  styled()->Layout();

  ASSERT_EQ(1, styled()->child_count());
  ASSERT_EQ(std::string(Label::kViewClassName),
            styled()->child_at(0)->GetClassName());
  EXPECT_EQ(ASCIIToUTF16("a"),
            static_cast<Label*>(styled()->child_at(0))->text());
}

TEST_F(StyledLabelTest, BasicWrapping) {
  const std::string text("This is a test block of text");
  InitStyledLabel(text);
  Label label(ASCIIToUTF16(text.substr(0, text.size() * 2 / 3)));
  gfx::Size label_preferred_size = label.GetPreferredSize();
  EXPECT_EQ(label_preferred_size.height() * 2,
            StyledLabelContentHeightForWidth(label_preferred_size.width()));

  // Also respect the border.
  styled()->set_border(Border::CreateEmptyBorder(3, 3, 3, 3));
  styled()->SetBounds(
      0,
      0,
      styled()->GetInsets().width() + label_preferred_size.width(),
      styled()->GetInsets().height() + 2 * label_preferred_size.height());
  styled()->Layout();
  ASSERT_EQ(2, styled()->child_count());
  EXPECT_EQ(3, styled()->child_at(0)->bounds().x());
  EXPECT_EQ(3, styled()->child_at(0)->bounds().y());
  EXPECT_EQ(styled()->bounds().height() - 3,
            styled()->child_at(1)->bounds().bottom());
}

TEST_F(StyledLabelTest, CreateLinks) {
  const std::string text("This is a test block of text.");
  InitStyledLabel(text);
  styled()->AddStyleRange(ui::Range(0, 1),
                          StyledLabel::RangeStyleInfo::CreateForLink());
  styled()->AddStyleRange(ui::Range(1, 2),
                          StyledLabel::RangeStyleInfo::CreateForLink());
  styled()->AddStyleRange(ui::Range(10, 11),
                          StyledLabel::RangeStyleInfo::CreateForLink());
  styled()->AddStyleRange(ui::Range(12, 13),
                          StyledLabel::RangeStyleInfo::CreateForLink());

  styled()->SetBounds(0, 0, 1000, 1000);
  styled()->Layout();
  ASSERT_EQ(7, styled()->child_count());
}

TEST_F(StyledLabelTest, DontBreakLinks) {
  const std::string text("This is a test block of text, ");
  const std::string link_text("and this should be a link");
  InitStyledLabel(text + link_text);
  styled()->AddStyleRange(
      ui::Range(text.size(), text.size() + link_text.size()),
      StyledLabel::RangeStyleInfo::CreateForLink());

  Label label(ASCIIToUTF16(text + link_text.substr(0, link_text.size() / 2)));
  gfx::Size label_preferred_size = label.GetPreferredSize();
  int pref_height = styled()->GetHeightForWidth(label_preferred_size.width());
  EXPECT_EQ(label_preferred_size.height() * 2,
            pref_height - styled()->GetInsets().height());

  styled()->SetBounds(0, 0, label_preferred_size.width(), pref_height);
  styled()->Layout();
  ASSERT_EQ(2, styled()->child_count());
  EXPECT_EQ(0, styled()->child_at(0)->bounds().x());
  EXPECT_EQ(0, styled()->child_at(1)->bounds().x());
}

TEST_F(StyledLabelTest, StyledRangeWithDisabledLineWrapping) {
  const std::string text("This is a test block of text, ");
  const std::string unbreakable_text("and this should not be breaked");
  InitStyledLabel(text + unbreakable_text);
  StyledLabel::RangeStyleInfo style_info;
  style_info.disable_line_wrapping = true;
  styled()->AddStyleRange(
      ui::Range(text.size(), text.size() + unbreakable_text.size()),
      style_info);

  Label label(ASCIIToUTF16(
      text + unbreakable_text.substr(0, unbreakable_text.size() / 2)));
  gfx::Size label_preferred_size = label.GetPreferredSize();
  int pref_height = styled()->GetHeightForWidth(label_preferred_size.width());
  EXPECT_EQ(label_preferred_size.height() * 2,
            pref_height - styled()->GetInsets().height());

  styled()->SetBounds(0, 0, label_preferred_size.width(), pref_height);
  styled()->Layout();
  ASSERT_EQ(2, styled()->child_count());
  EXPECT_EQ(0, styled()->child_at(0)->bounds().x());
  EXPECT_EQ(0, styled()->child_at(1)->bounds().x());
}

TEST_F(StyledLabelTest, StyledRangeUnderlined) {
  const std::string text("This is a test block of text, ");
  const std::string underlined_text("and this should be undelined");
  InitStyledLabel(text + underlined_text);
  StyledLabel::RangeStyleInfo style_info;
  style_info.font_style = gfx::Font::UNDERLINE;
  styled()->AddStyleRange(
      ui::Range(text.size(), text.size() + underlined_text.size()),
      style_info);

  styled()->SetBounds(0, 0, 1000, 1000);
  styled()->Layout();

  ASSERT_EQ(2, styled()->child_count());
  ASSERT_EQ(std::string(Label::kViewClassName),
            styled()->child_at(1)->GetClassName());
  EXPECT_EQ(gfx::Font::UNDERLINE,
            static_cast<Label*>(styled()->child_at(1))->font().GetStyle());
}

TEST_F(StyledLabelTest, StyledRangeBold) {
  const std::string bold_text(
      "This is a block of text whose style will be set to BOLD in the test");
  const std::string text(" normal text");
  InitStyledLabel(bold_text + text);

  StyledLabel::RangeStyleInfo style_info;
  style_info.font_style = gfx::Font::BOLD;
  styled()->AddStyleRange(ui::Range(0, bold_text.size()), style_info);

  // Calculate the bold text width if it were a pure label view, both with bold
  // and normal style.
  Label label(ASCIIToUTF16(bold_text));
  const gfx::Size normal_label_size = label.GetPreferredSize();
  label.SetFont(label.font().DeriveFont(0, gfx::Font::BOLD));
  const gfx::Size bold_label_size = label.GetPreferredSize();

  ASSERT_GE(bold_label_size.width(), normal_label_size.width());

  // Set the width so |bold_text| doesn't fit on a single line with bold style,
  // but does with normal font style.
  int styled_width = (normal_label_size.width() + bold_label_size.width()) / 2;
  int pref_height = styled()->GetHeightForWidth(styled_width);

  // Sanity check that |bold_text| with normal font style would fit on a single
  // line in a styled label with width |styled_width|.
  StyledLabel unstyled(ASCIIToUTF16(bold_text), this);
  unstyled.SetBounds(0, 0, styled_width, pref_height);
  unstyled.Layout();
  ASSERT_EQ(1, unstyled.child_count());

  styled()->SetBounds(0, 0, styled_width, pref_height);
  styled()->Layout();

  ASSERT_EQ(3, styled()->child_count());

  // The bold text should be broken up into two parts.
  ASSERT_EQ(std::string(Label::kViewClassName),
            styled()->child_at(0)->GetClassName());
  EXPECT_EQ(gfx::Font::BOLD,
            static_cast<Label*>(styled()->child_at(0))->font().GetStyle());
  ASSERT_EQ(std::string(Label::kViewClassName),
            styled()->child_at(1)->GetClassName());
  EXPECT_EQ(gfx::Font::BOLD,
            static_cast<Label*>(styled()->child_at(1))->font().GetStyle());
  ASSERT_EQ(std::string(Label::kViewClassName),
            styled()->child_at(2)->GetClassName());
  EXPECT_EQ(gfx::Font::NORMAL,
            static_cast<Label*>(styled()->child_at(2))->font().GetStyle());

  // The second bold part should start on a new line.
  EXPECT_EQ(0, styled()->child_at(0)->bounds().x());
  EXPECT_EQ(0, styled()->child_at(1)->bounds().x());
  EXPECT_EQ(styled()->child_at(1)->bounds().right() - 2,
            styled()->child_at(2)->bounds().x());
}

TEST_F(StyledLabelTest, StyledRangeWithTooltip) {
  const std::string text("This is a test block of text, ");
  const std::string tooltip_text("this should have a tooltip,");
  const std::string normal_text(" this should not have a tooltip, ");
  const std::string link_text("and this should be a link");

  const size_t tooltip_start = text.size();
  const size_t link_start =
      text.size() + tooltip_text.size() + normal_text.size();

  InitStyledLabel(text + tooltip_text + normal_text + link_text);
  StyledLabel::RangeStyleInfo tooltip_style;
  tooltip_style.tooltip = ASCIIToUTF16("tooltip");
  styled()->AddStyleRange(
      ui::Range(tooltip_start, tooltip_start + tooltip_text.size()),
      tooltip_style);
  styled()->AddStyleRange(ui::Range(link_start, link_start + link_text.size()),
                          StyledLabel::RangeStyleInfo::CreateForLink());

  // Break line inside the range with the tooltip.
  Label label(ASCIIToUTF16(
       text + tooltip_text.substr(0, tooltip_text.size() - 3)));
  gfx::Size label_preferred_size = label.GetPreferredSize();
  int pref_height = styled()->GetHeightForWidth(label_preferred_size.width());
  EXPECT_EQ(label_preferred_size.height() * 3,
            pref_height - styled()->GetInsets().height());

  styled()->SetBounds(0, 0, label_preferred_size.width(), pref_height);
  styled()->Layout();

  EXPECT_EQ(label_preferred_size.width(), styled()->width());

  ASSERT_EQ(5, styled()->child_count());
  EXPECT_EQ(0, styled()->child_at(0)->bounds().x());
  EXPECT_EQ(styled()->child_at(0)->bounds().right() - 2,
            styled()->child_at(1)->bounds().x());
  EXPECT_EQ(0, styled()->child_at(2)->bounds().x());
  EXPECT_EQ(styled()->child_at(2)->bounds().right() - 2,
            styled()->child_at(3)->bounds().x());
  EXPECT_EQ(0, styled()->child_at(4)->bounds().x());

  string16 tooltip;
  EXPECT_TRUE(
      styled()->child_at(1)->GetTooltipText(gfx::Point(1, 1), &tooltip));
  EXPECT_EQ(ASCIIToUTF16("tooltip"), tooltip);
  EXPECT_TRUE(
      styled()->child_at(2)->GetTooltipText(gfx::Point(1, 1), &tooltip));
  EXPECT_EQ(ASCIIToUTF16("tooltip"), tooltip);
}

TEST_F(StyledLabelTest, HandleEmptyLayout) {
  const std::string text("This is a test block of text, ");
  InitStyledLabel(text);
  styled()->Layout();
  ASSERT_EQ(0, styled()->child_count());
}

}  // namespace
