// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/corewm/tooltip_controller.h"

#include "base/utf_string_conversions.h"
#include "ui/aura/client/cursor_client.h"
#include "ui/aura/client/tooltip_client.h"
#include "ui/aura/env.h"
#include "ui/aura/root_window.h"
#include "ui/aura/test/aura_test_base.h"
#include "ui/aura/test/event_generator.h"
#include "ui/aura/window.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/text/text_elider.h"
#include "ui/gfx/font.h"
#include "ui/gfx/point.h"
#include "ui/views/corewm/tooltip_controller_test_helper.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget.h"

#if defined(OS_WIN)
#include "ui/base/win/scoped_ole_initializer.h"
#endif
#if !defined(OS_CHROMEOS)
#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"
#endif

namespace views {
namespace corewm {
namespace test {
namespace {

views::Widget* CreateWidget(aura::RootWindow* root) {
  views::Widget* widget = new views::Widget;
  views::Widget::InitParams params;
  params.type = views::Widget::InitParams::TYPE_WINDOW_FRAMELESS;
  params.accept_events = true;
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
#if defined(OS_CHROMEOS)
  params.parent = root;
#else
  params.native_widget = new DesktopNativeWidgetAura(widget);
#endif
  params.bounds = gfx::Rect(0, 0, 200, 100);
  widget->Init(params);
  widget->Show();
  return widget;
}

gfx::Font GetDefaultFont() {
  return ui::ResourceBundle::GetSharedInstance().GetFont(
      ui::ResourceBundle::BaseFont);
}

TooltipController* GetController(Widget* widget) {
  return static_cast<TooltipController*>(
      aura::client::GetTooltipClient(
          widget->GetNativeWindow()->GetRootWindow()));
}

}  // namespace

class TooltipControllerTest : public aura::test::AuraTestBase {
 public:
  TooltipControllerTest() : view_(NULL) {}
  virtual ~TooltipControllerTest() {}

  virtual void SetUp() OVERRIDE {
    aura::test::AuraTestBase::SetUp();
#if defined(OS_CHROMEOS)
    controller_.reset(new TooltipController(gfx::SCREEN_TYPE_ALTERNATE));
    root_window()->AddPreTargetHandler(controller_.get());
    SetTooltipClient(root_window(), controller_.get());
#endif
    widget_.reset(CreateWidget(root_window()));
    widget_->SetContentsView(new View);
    view_ = new TooltipTestView;
    widget_->GetContentsView()->AddChildView(view_);
    view_->SetBoundsRect(widget_->GetContentsView()->GetLocalBounds());
    helper_.reset(new TooltipControllerTestHelper(
                      GetController(widget_.get())));
    generator_.reset(new aura::test::EventGenerator(GetRootWindow()));
  }

  virtual void TearDown() OVERRIDE {
#if defined(OS_CHROMEOS)
    root_window()->RemovePreTargetHandler(controller_.get());
    SetTooltipClient(root_window(), NULL);
    controller_.reset();
#endif
    generator_.reset();
    helper_.reset();
    widget_.reset();
    aura::test::AuraTestBase::TearDown();
  }

 protected:
  aura::Window* GetWindow() {
    return widget_->GetNativeWindow();
  }

  aura::RootWindow* GetRootWindow() {
    return GetWindow()->GetRootWindow();
  }

  TooltipTestView* PrepareSecondView() {
    TooltipTestView* view2 = new TooltipTestView;
    widget_->GetContentsView()->AddChildView(view2);
    view_->SetBounds(0, 0, 100, 100);
    view2->SetBounds(100, 0, 100, 100);
    return view2;
  }

  scoped_ptr<views::Widget> widget_;
  TooltipTestView* view_;
  scoped_ptr<TooltipControllerTestHelper> helper_;
  scoped_ptr<aura::test::EventGenerator> generator_;

 private:
  scoped_ptr<TooltipController> controller_;
#if defined(OS_WIN)
  ui::ScopedOleInitializer ole_initializer_;
#endif

  DISALLOW_COPY_AND_ASSIGN(TooltipControllerTest);
};

TEST_F(TooltipControllerTest, ViewTooltip) {
  view_->set_tooltip_text(ASCIIToUTF16("Tooltip Text"));
  EXPECT_EQ(string16(), helper_->GetTooltipText());
  EXPECT_EQ(NULL, helper_->GetTooltipWindow());
  generator_->MoveMouseToCenterOf(GetWindow());

  EXPECT_EQ(GetWindow(), GetRootWindow()->GetEventHandlerForPoint(
      generator_->current_location()));
  string16 expected_tooltip = ASCIIToUTF16("Tooltip Text");
  EXPECT_EQ(expected_tooltip, aura::client::GetTooltipText(GetWindow()));
  EXPECT_EQ(string16(), helper_->GetTooltipText());
  EXPECT_EQ(GetWindow(), helper_->GetTooltipWindow());

  // Fire tooltip timer so tooltip becomes visible.
  helper_->FireTooltipTimer();

  EXPECT_TRUE(helper_->IsTooltipVisible());
  generator_->MoveMouseBy(1, 0);

  EXPECT_TRUE(helper_->IsTooltipVisible());
  EXPECT_EQ(expected_tooltip, aura::client::GetTooltipText(GetWindow()));
  EXPECT_EQ(expected_tooltip, helper_->GetTooltipText());
  EXPECT_EQ(GetWindow(), helper_->GetTooltipWindow());
}

TEST_F(TooltipControllerTest, TooltipsInMultipleViews) {
  view_->set_tooltip_text(ASCIIToUTF16("Tooltip Text"));
  EXPECT_EQ(string16(), helper_->GetTooltipText());
  EXPECT_EQ(NULL, helper_->GetTooltipWindow());

  PrepareSecondView();
  aura::Window* window = GetWindow();
  aura::RootWindow* root_window = GetRootWindow();

  // Fire tooltip timer so tooltip becomes visible.
  generator_->MoveMouseRelativeTo(window, view_->bounds().CenterPoint());
  helper_->FireTooltipTimer();
  EXPECT_TRUE(helper_->IsTooltipVisible());
  for (int i = 0; i < 49; ++i) {
    generator_->MoveMouseBy(1, 0);
    EXPECT_TRUE(helper_->IsTooltipVisible());
    EXPECT_EQ(window, root_window->GetEventHandlerForPoint(
            generator_->current_location()));
    string16 expected_tooltip = ASCIIToUTF16("Tooltip Text");
    EXPECT_EQ(expected_tooltip, aura::client::GetTooltipText(window));
    EXPECT_EQ(expected_tooltip, helper_->GetTooltipText());
    EXPECT_EQ(window, helper_->GetTooltipWindow());
  }
  for (int i = 0; i < 49; ++i) {
    generator_->MoveMouseBy(1, 0);
    EXPECT_FALSE(helper_->IsTooltipVisible());
    EXPECT_EQ(window, root_window->GetEventHandlerForPoint(
            generator_->current_location()));
    string16 expected_tooltip;  // = ""
    EXPECT_EQ(expected_tooltip, aura::client::GetTooltipText(window));
    EXPECT_EQ(expected_tooltip, helper_->GetTooltipText());
    EXPECT_EQ(window, helper_->GetTooltipWindow());
  }
}

TEST_F(TooltipControllerTest, EnableOrDisableTooltips) {
  view_->set_tooltip_text(ASCIIToUTF16("Tooltip Text"));
  EXPECT_EQ(string16(), helper_->GetTooltipText());
  EXPECT_EQ(NULL, helper_->GetTooltipWindow());

  generator_->MoveMouseRelativeTo(GetWindow(), view_->bounds().CenterPoint());
  string16 expected_tooltip = ASCIIToUTF16("Tooltip Text");

  // Fire tooltip timer so tooltip becomes visible.
  helper_->FireTooltipTimer();
  EXPECT_TRUE(helper_->IsTooltipVisible());

  // Diable tooltips and check again.
  helper_->controller()->SetTooltipsEnabled(false);
  EXPECT_FALSE(helper_->IsTooltipVisible());
  helper_->FireTooltipTimer();
  EXPECT_FALSE(helper_->IsTooltipVisible());

  // Enable tooltips back and check again.
  helper_->controller()->SetTooltipsEnabled(true);
  EXPECT_FALSE(helper_->IsTooltipVisible());
  helper_->FireTooltipTimer();
  EXPECT_TRUE(helper_->IsTooltipVisible());
}

TEST_F(TooltipControllerTest, TrimTooltipToFitTests) {
  const int max_width = 4000;
  string16 tooltip;
  int width, line_count, expect_lines;
  int max_pixel_width = 400;  // copied from constants in tooltip_controller.cc
  int max_lines = 10;  // copied from constants in tooltip_controller.cc
  gfx::Font font = GetDefaultFont();
  size_t tooltip_len;

  // Error in computed size vs. expected size should not be greater than the
  // size of the longest word.
  int error_in_pixel_width = font.GetStringWidth(ASCIIToUTF16("tooltip"));

  // Long tooltips should wrap to next line
  tooltip.clear();
  width = line_count = -1;
  expect_lines = 3;
  for (; font.GetStringWidth(tooltip) <= (expect_lines - 1) * max_pixel_width;)
    tooltip.append(ASCIIToUTF16("This is part of the tooltip"));
  tooltip_len = tooltip.length();
  TooltipControllerTestHelper::TrimTooltipToFit(
      max_width, &tooltip, &width, &line_count);
  EXPECT_NEAR(max_pixel_width, width, error_in_pixel_width);
  EXPECT_EQ(expect_lines, line_count);
  EXPECT_EQ(tooltip_len + expect_lines - 1, tooltip.length());

  // More than |max_lines| lines should get truncated at 10 lines.
  tooltip.clear();
  width = line_count = -1;
  expect_lines = 13;
  for (; font.GetStringWidth(tooltip) <= (expect_lines - 1) * max_pixel_width;)
    tooltip.append(ASCIIToUTF16("This is part of the tooltip"));
  TooltipControllerTestHelper::TrimTooltipToFit(
      max_width, &tooltip, &width, &line_count);
  EXPECT_NEAR(max_pixel_width, width, error_in_pixel_width);
  EXPECT_EQ(max_lines, line_count);

  // Long multi line tooltips should wrap individual lines.
  tooltip.clear();
  width = line_count = -1;
  expect_lines = 4;
  for (; font.GetStringWidth(tooltip) <= (expect_lines - 2) * max_pixel_width;)
    tooltip.append(ASCIIToUTF16("This is part of the tooltip"));
  tooltip.insert(tooltip.length() / 2, ASCIIToUTF16("\n"));
  tooltip_len = tooltip.length();
  TooltipControllerTestHelper::TrimTooltipToFit(
      max_width, &tooltip, &width, &line_count);
  EXPECT_NEAR(max_pixel_width, width, error_in_pixel_width);
  EXPECT_EQ(expect_lines, line_count);
  // We may have inserted the line break above near a space which will get
  // trimmed. Hence we may be off by 1 in the final tooltip length calculation.
  EXPECT_NEAR(tooltip_len + expect_lines - 2, tooltip.length(), 1);

#if !defined(OS_WIN)
  // Tooltip with really long word gets elided.
  tooltip.clear();
  width = line_count = -1;
  tooltip = UTF8ToUTF16(std::string('a', max_pixel_width));
  TooltipControllerTestHelper::TrimTooltipToFit(
      max_width, &tooltip, &width, &line_count);
  EXPECT_NEAR(max_pixel_width, width, 5);
  EXPECT_EQ(1, line_count);
  EXPECT_EQ(ui::ElideText(UTF8ToUTF16(std::string('a', max_pixel_width)), font,
                          max_pixel_width, ui::ELIDE_AT_END), tooltip);
#endif

  // Normal small tooltip should stay as is.
  tooltip.clear();
  width = line_count = -1;
  tooltip = ASCIIToUTF16("Small Tooltip");
  TooltipControllerTestHelper::TrimTooltipToFit(
      max_width, &tooltip, &width, &line_count);
  EXPECT_EQ(font.GetStringWidth(ASCIIToUTF16("Small Tooltip")), width);
  EXPECT_EQ(1, line_count);
  EXPECT_EQ(ASCIIToUTF16("Small Tooltip"), tooltip);

  // Normal small multi-line tooltip should stay as is.
  tooltip.clear();
  width = line_count = -1;
  tooltip = ASCIIToUTF16("Multi line\nTooltip");
  TooltipControllerTestHelper::TrimTooltipToFit(
      max_width, &tooltip, &width, &line_count);
  int expected_width = font.GetStringWidth(ASCIIToUTF16("Multi line"));
  expected_width = std::max(expected_width,
                            font.GetStringWidth(ASCIIToUTF16("Tooltip")));
  EXPECT_EQ(expected_width, width);
  EXPECT_EQ(2, line_count);
  EXPECT_EQ(ASCIIToUTF16("Multi line\nTooltip"), tooltip);

  // Whitespaces in tooltips are preserved.
  tooltip.clear();
  width = line_count = -1;
  tooltip = ASCIIToUTF16("Small  Tool  t\tip");
  TooltipControllerTestHelper::TrimTooltipToFit(
      max_width, &tooltip, &width, &line_count);
  EXPECT_EQ(font.GetStringWidth(ASCIIToUTF16("Small  Tool  t\tip")), width);
  EXPECT_EQ(1, line_count);
  EXPECT_EQ(ASCIIToUTF16("Small  Tool  t\tip"), tooltip);
}

TEST_F(TooltipControllerTest, TooltipHidesOnKeyPressAndStaysHiddenUntilChange) {
  view_->set_tooltip_text(ASCIIToUTF16("Tooltip Text for view 1"));
  EXPECT_EQ(string16(), helper_->GetTooltipText());
  EXPECT_EQ(NULL, helper_->GetTooltipWindow());

  TooltipTestView* view2 = PrepareSecondView();
  view2->set_tooltip_text(ASCIIToUTF16("Tooltip Text for view 2"));

  aura::Window* window = GetWindow();

  // Fire tooltip timer so tooltip becomes visible.
  generator_->MoveMouseRelativeTo(window, view_->bounds().CenterPoint());
  helper_->FireTooltipTimer();
  EXPECT_TRUE(helper_->IsTooltipVisible());
  EXPECT_TRUE(helper_->IsTooltipShownTimerRunning());

  generator_->PressKey(ui::VKEY_1, 0);
  EXPECT_FALSE(helper_->IsTooltipVisible());
  EXPECT_FALSE(helper_->IsTooltipTimerRunning());
  EXPECT_FALSE(helper_->IsTooltipShownTimerRunning());

  // Moving the mouse inside |view1| should not change the state of the tooltip
  // or the timers.
  for (int i = 0; i < 49; i++) {
    generator_->MoveMouseBy(1, 0);
    EXPECT_FALSE(helper_->IsTooltipVisible());
    EXPECT_FALSE(helper_->IsTooltipTimerRunning());
    EXPECT_FALSE(helper_->IsTooltipShownTimerRunning());
    EXPECT_EQ(window,
              GetRootWindow()->GetEventHandlerForPoint(
                  generator_->current_location()));
    string16 expected_tooltip = ASCIIToUTF16("Tooltip Text for view 1");
    EXPECT_EQ(expected_tooltip, aura::client::GetTooltipText(window));
    EXPECT_EQ(expected_tooltip, helper_->GetTooltipText());
    EXPECT_EQ(window, helper_->GetTooltipWindow());
  }

  // Now we move the mouse on to |view2|. It should re-start the tooltip timer.
  generator_->MoveMouseBy(1, 0);
  EXPECT_TRUE(helper_->IsTooltipTimerRunning());
  helper_->FireTooltipTimer();
  EXPECT_TRUE(helper_->IsTooltipVisible());
  EXPECT_TRUE(helper_->IsTooltipShownTimerRunning());
  string16 expected_tooltip = ASCIIToUTF16("Tooltip Text for view 2");
  EXPECT_EQ(expected_tooltip, aura::client::GetTooltipText(window));
  EXPECT_EQ(expected_tooltip, helper_->GetTooltipText());
  EXPECT_EQ(window, helper_->GetTooltipWindow());
}

TEST_F(TooltipControllerTest, TooltipHidesOnTimeoutAndStaysHiddenUntilChange) {
  view_->set_tooltip_text(ASCIIToUTF16("Tooltip Text for view 1"));
  EXPECT_EQ(string16(), helper_->GetTooltipText());
  EXPECT_EQ(NULL, helper_->GetTooltipWindow());

  TooltipTestView* view2 = PrepareSecondView();
  view2->set_tooltip_text(ASCIIToUTF16("Tooltip Text for view 2"));

  aura::Window* window = GetWindow();

  // Fire tooltip timer so tooltip becomes visible.
  generator_->MoveMouseRelativeTo(window, view_->bounds().CenterPoint());
  helper_->FireTooltipTimer();
  EXPECT_TRUE(helper_->IsTooltipVisible());
  EXPECT_TRUE(helper_->IsTooltipShownTimerRunning());

  helper_->FireTooltipShownTimer();
  EXPECT_FALSE(helper_->IsTooltipVisible());
  EXPECT_FALSE(helper_->IsTooltipTimerRunning());
  EXPECT_FALSE(helper_->IsTooltipShownTimerRunning());

  // Moving the mouse inside |view1| should not change the state of the tooltip
  // or the timers.
  for (int i = 0; i < 49; ++i) {
    generator_->MoveMouseBy(1, 0);
    EXPECT_FALSE(helper_->IsTooltipVisible());
    EXPECT_FALSE(helper_->IsTooltipTimerRunning());
    EXPECT_FALSE(helper_->IsTooltipShownTimerRunning());
    EXPECT_EQ(window, GetRootWindow()->GetEventHandlerForPoint(
                  generator_->current_location()));
    string16 expected_tooltip = ASCIIToUTF16("Tooltip Text for view 1");
    EXPECT_EQ(expected_tooltip, aura::client::GetTooltipText(window));
    EXPECT_EQ(expected_tooltip, helper_->GetTooltipText());
    EXPECT_EQ(window, helper_->GetTooltipWindow());
  }

  // Now we move the mouse on to |view2|. It should re-start the tooltip timer.
  generator_->MoveMouseBy(1, 0);
  EXPECT_TRUE(helper_->IsTooltipTimerRunning());
  helper_->FireTooltipTimer();
  EXPECT_TRUE(helper_->IsTooltipVisible());
  EXPECT_TRUE(helper_->IsTooltipShownTimerRunning());
  string16 expected_tooltip = ASCIIToUTF16("Tooltip Text for view 2");
  EXPECT_EQ(expected_tooltip, aura::client::GetTooltipText(window));
  EXPECT_EQ(expected_tooltip, helper_->GetTooltipText());
  EXPECT_EQ(window, helper_->GetTooltipWindow());
}

}  // namespace test
}  // namespace corewm
}  // namespace views
