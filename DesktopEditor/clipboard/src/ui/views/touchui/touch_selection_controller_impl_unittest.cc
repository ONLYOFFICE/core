// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/command_line.h"
#include "base/utf_string_conversions.h"
#include "ui/base/touch/touch_editing_controller.h"
#include "ui/base/ui_base_switches.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/render_text.h"
#include "ui/views/controls/textfield/native_textfield_views.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/test/views_test_base.h"
#include "ui/views/touchui/touch_selection_controller_impl.h"
#include "ui/views/widget/widget.h"

#if defined(USE_AURA)
#include "ui/aura/test/event_generator.h"
#include "ui/aura/window.h"
#endif

namespace views {

class TouchSelectionControllerImplTest : public ViewsTestBase {
 public:
  TouchSelectionControllerImplTest()
      : widget_(NULL),
        textfield_(NULL),
        textfield_view_(NULL),
        views_tsc_factory_(new ViewsTouchSelectionControllerFactory) {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kEnableTouchEditing);
    ui::TouchSelectionControllerFactory::SetInstance(views_tsc_factory_.get());
  }

  virtual ~TouchSelectionControllerImplTest() {
    ui::TouchSelectionControllerFactory::SetInstance(NULL);
  }

  virtual void TearDown() {
    if (widget_)
      widget_->Close();
    ViewsTestBase::TearDown();
  }

  void CreateTextfield() {
    textfield_ = new Textfield();
    widget_ = new Widget;
    Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
    params.bounds = gfx::Rect(0, 0, 200, 200);
    widget_->Init(params);
    View* container = new View();
    widget_->SetContentsView(container);
    container->AddChildView(textfield_);

    textfield_view_ = static_cast<NativeTextfieldViews*>(
        textfield_->GetNativeWrapperForTesting());
    textfield_->SetBoundsRect(params.bounds);
    textfield_view_->SetBoundsRect(params.bounds);
    textfield_->set_id(1);
    widget_->Show();

    DCHECK(textfield_view_);
    textfield_->RequestFocus();
  }

 protected:
  gfx::Point GetCursorPosition(const gfx::SelectionModel& sel) {
    gfx::RenderText* render_text = textfield_view_->GetRenderText();
    gfx::Rect cursor_bounds = render_text->GetCursorBounds(sel, true);
    return gfx::Point(cursor_bounds.x(), cursor_bounds.y());
  }

  TouchSelectionControllerImpl* GetSelectionController() {
    return static_cast<TouchSelectionControllerImpl*>(
        textfield_view_->touch_selection_controller_.get());
  }

  void SimulateSelectionHandleDrag(gfx::Point p, int selection_handle) {
    TouchSelectionControllerImpl* controller = GetSelectionController();
    // Do the work of OnMousePressed().
    if (selection_handle == 1)
      controller->SetDraggingHandle(controller->selection_handle_1_.get());
    else
      controller->SetDraggingHandle(controller->selection_handle_2_.get());

    // Offset the drag position by the selection handle radius since it is
    // supposed to be in the coordinate system of the handle.
    p.Offset(10, 0);
    controller->SelectionHandleDragged(p);

    // Do the work of OnMouseReleased().
    controller->dragging_handle_ = NULL;
  }

  gfx::Point GetSelectionHandle1Position() {
    return GetSelectionController()->GetSelectionHandle1Position();
  }

  gfx::Point GetSelectionHandle2Position() {
    return GetSelectionController()->GetSelectionHandle2Position();
  }

  gfx::Point GetCursorHandlePosition() {
    return GetSelectionController()->GetCursorHandlePosition();
  }

  bool IsSelectionHandle1Visible() {
    return GetSelectionController()->IsSelectionHandle1Visible();
  }

  bool IsSelectionHandle2Visible() {
    return GetSelectionController()->IsSelectionHandle2Visible();
  }

  bool IsCursorHandleVisible() {
    return GetSelectionController()->IsCursorHandleVisible();
  }

  gfx::RenderText* GetRenderText() {
    return textfield_view_->GetRenderText();
  }

  Widget* widget_;

  Textfield* textfield_;
  NativeTextfieldViews* textfield_view_;
  scoped_ptr<ViewsTouchSelectionControllerFactory> views_tsc_factory_;

 private:
  DISALLOW_COPY_AND_ASSIGN(TouchSelectionControllerImplTest);
};

// If textfield has selection, this macro verifies that the selection handles
// are visible and at the correct positions (at the end points of selection).
// |cursor_at_selection_handle_1| is used to decide whether selection
// handle 1's position is matched against the start of selection or the end.
#define VERIFY_HANDLE_POSITIONS(cursor_at_selection_handle_1)                  \
{                                                                              \
    gfx::SelectionModel sel = textfield_view_->GetSelectionModel();            \
    if (textfield_->HasSelection()) {                                          \
      EXPECT_TRUE(IsSelectionHandle1Visible());                                \
      EXPECT_TRUE(IsSelectionHandle2Visible());                                \
      EXPECT_FALSE(IsCursorHandleVisible());                                   \
      gfx::SelectionModel sel_start = GetRenderText()->                        \
                                      GetSelectionModelForSelectionStart();    \
      gfx::Point selection_start = GetCursorPosition(sel_start);               \
      gfx::Point selection_end = GetCursorPosition(sel);                       \
      gfx::Point sh1 = GetSelectionHandle1Position();                          \
      gfx::Point sh2 = GetSelectionHandle2Position();                          \
      sh1.Offset(10, 0);                                                       \
      sh2.Offset(10, 0);                                                       \
      if (cursor_at_selection_handle_1) {                                      \
        EXPECT_EQ(sh1, selection_end);                                         \
        EXPECT_EQ(sh2, selection_start);                                       \
      } else {                                                                 \
        EXPECT_EQ(sh1, selection_start);                                       \
        EXPECT_EQ(sh2, selection_end);                                         \
      }                                                                        \
    } else {                                                                   \
      EXPECT_FALSE(IsSelectionHandle1Visible());                               \
      EXPECT_FALSE(IsSelectionHandle2Visible());                               \
      EXPECT_TRUE(IsCursorHandleVisible());                                    \
      gfx::Point cursor_pos = GetCursorPosition(sel);                          \
      gfx::Point ch_pos = GetCursorHandlePosition();                           \
      ch_pos.Offset(10, 0);                                                    \
      EXPECT_EQ(ch_pos, cursor_pos);                                           \
    }                                                                          \
}

// Tests that the selection handles are placed appropriately when selection in
// a Textfield changes.
TEST_F(TouchSelectionControllerImplTest, SelectionInTextfieldTest) {
  CreateTextfield();
  textfield_->SetText(ASCIIToUTF16("some text"));
  // Tap the textfield to invoke touch selection.
  ui::GestureEvent tap(ui::ET_GESTURE_TAP, 0, 0, 0, base::TimeDelta(),
      ui::GestureEventDetails(ui::ET_GESTURE_TAP, 1.0f, 0.0f), 0);
  textfield_view_->OnGestureEvent(&tap);

  // Test selecting a range.
  textfield_->SelectRange(ui::Range(3, 7));
  VERIFY_HANDLE_POSITIONS(false);

  // Test selecting everything.
  textfield_->SelectAll(false);
  VERIFY_HANDLE_POSITIONS(false);

  // Test with no selection.
  textfield_->ClearSelection();
  VERIFY_HANDLE_POSITIONS(false);

  // Test with lost focus.
  widget_->GetFocusManager()->ClearFocus();
  EXPECT_FALSE(GetSelectionController());

  // Test with focus re-gained.
  widget_->GetFocusManager()->SetFocusedView(textfield_);
  EXPECT_FALSE(GetSelectionController());
  textfield_view_->OnGestureEvent(&tap);
  VERIFY_HANDLE_POSITIONS(false);
}

// Tests that the selection handles are placed appropriately in bidi text.
TEST_F(TouchSelectionControllerImplTest, SelectionInBidiTextfieldTest) {
  CreateTextfield();
  textfield_->SetText(WideToUTF16(L"abc\x05d0\x05d1\x05d2"));
  // Tap the textfield to invoke touch selection.
  ui::GestureEvent tap(ui::ET_GESTURE_TAP, 0, 0, 0, base::TimeDelta(),
      ui::GestureEventDetails(ui::ET_GESTURE_TAP, 1.0f, 0.0f), 0);
  textfield_view_->OnGestureEvent(&tap);

  // Test cursor at run boundary and with empty selection.
  textfield_->SelectSelectionModel(
      gfx::SelectionModel(3, gfx::CURSOR_BACKWARD));
  VERIFY_HANDLE_POSITIONS(false);

  // Test selection range inside one run and starts or ends at run boundary.
  textfield_->SelectRange(ui::Range(2, 3));
  VERIFY_HANDLE_POSITIONS(false);

  textfield_->SelectRange(ui::Range(3, 2));
  VERIFY_HANDLE_POSITIONS(false);

  textfield_->SelectRange(ui::Range(3, 4));
  VERIFY_HANDLE_POSITIONS(false);

  textfield_->SelectRange(ui::Range(4, 3));
  VERIFY_HANDLE_POSITIONS(false);

  textfield_->SelectRange(ui::Range(3, 6));
  VERIFY_HANDLE_POSITIONS(false);

  textfield_->SelectRange(ui::Range(6, 3));
  VERIFY_HANDLE_POSITIONS(false);

  // Test selection range accross runs.
  textfield_->SelectRange(ui::Range(0, 6));
  VERIFY_HANDLE_POSITIONS(false);

  textfield_->SelectRange(ui::Range(6, 0));
  VERIFY_HANDLE_POSITIONS(false);

  textfield_->SelectRange(ui::Range(1, 4));
  VERIFY_HANDLE_POSITIONS(false);

  textfield_->SelectRange(ui::Range(4, 1));
  VERIFY_HANDLE_POSITIONS(false);
}

// Tests if the SelectRect callback is called appropriately when selection
// handles are moved.
TEST_F(TouchSelectionControllerImplTest, SelectRectCallbackTest) {
  CreateTextfield();
  textfield_->SetText(ASCIIToUTF16("textfield with selected text"));
  // Tap the textfield to invoke touch selection.
  ui::GestureEvent tap(ui::ET_GESTURE_TAP, 0, 0, 0, base::TimeDelta(),
      ui::GestureEventDetails(ui::ET_GESTURE_TAP, 1.0f, 0.0f), 0);
  textfield_view_->OnGestureEvent(&tap);
  textfield_->SelectRange(ui::Range(3, 7));

  EXPECT_EQ(UTF16ToUTF8(textfield_->GetSelectedText()), "tfie");
  VERIFY_HANDLE_POSITIONS(false);

  // Drag selection handle 2 to right by 3 chars.
  int x = textfield_->font().GetStringWidth(ASCIIToUTF16("ld "));
  SimulateSelectionHandleDrag(gfx::Point(x, 0), 2);
  EXPECT_EQ(UTF16ToUTF8(textfield_->GetSelectedText()), "tfield ");
  VERIFY_HANDLE_POSITIONS(false);

  // Drag selection handle 1 to the left by a large amount (selection should
  // just stick to the beginning of the textfield).
  SimulateSelectionHandleDrag(gfx::Point(-50, 0), 1);
  EXPECT_EQ(UTF16ToUTF8(textfield_->GetSelectedText()), "textfield ");
  VERIFY_HANDLE_POSITIONS(true);

  // Drag selection handle 1 across selection handle 2.
  x = textfield_->font().GetStringWidth(ASCIIToUTF16("textfield with "));
  SimulateSelectionHandleDrag(gfx::Point(x, 0), 1);
  EXPECT_EQ(UTF16ToUTF8(textfield_->GetSelectedText()), "with ");
  VERIFY_HANDLE_POSITIONS(true);

  // Drag selection handle 2 across selection handle 1.
  x = textfield_->font().GetStringWidth(ASCIIToUTF16("with selected "));
  SimulateSelectionHandleDrag(gfx::Point(x, 0), 2);
  EXPECT_EQ(UTF16ToUTF8(textfield_->GetSelectedText()), "selected ");
  VERIFY_HANDLE_POSITIONS(false);
}

TEST_F(TouchSelectionControllerImplTest, SelectRectInBidiCallbackTest) {
  CreateTextfield();
  textfield_->SetText(WideToUTF16(L"abc\x05e1\x05e2\x05e3"L"def"));
  // Tap the textfield to invoke touch selection.
  ui::GestureEvent tap(ui::ET_GESTURE_TAP, 0, 0, 0, base::TimeDelta(),
      ui::GestureEventDetails(ui::ET_GESTURE_TAP, 1.0f, 0.0f), 0);
  textfield_view_->OnGestureEvent(&tap);

  // Select [c] from left to right.
  textfield_->SelectRange(ui::Range(2, 3));
  EXPECT_EQ(WideToUTF16(L"c"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(false);

  // Drag selection handle 2 to right by 1 char.
  int x = textfield_->font().GetStringWidth(WideToUTF16(L"\x05e3"));
  SimulateSelectionHandleDrag(gfx::Point(x, 0), 2);
  EXPECT_EQ(WideToUTF16(L"c\x05e1\x05e2"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(false);

  // Drag selection handle 1 to left by 1 char.
  x = textfield_->font().GetStringWidth(WideToUTF16(L"b"));
  SimulateSelectionHandleDrag(gfx::Point(-x, 0), 1);
  EXPECT_EQ(WideToUTF16(L"bc\x05e1\x05e2"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(true);

  // Select [c] from right to left.
  textfield_->SelectRange(ui::Range(3, 2));
  EXPECT_EQ(WideToUTF16(L"c"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(false);

  // Drag selection handle 1 to right by 1 char.
  x = textfield_->font().GetStringWidth(WideToUTF16(L"\x05e3"));
  SimulateSelectionHandleDrag(gfx::Point(x, 0), 1);
  EXPECT_EQ(WideToUTF16(L"c\x05e1\x05e2"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(true);

  // Drag selection handle 2 to left by 1 char.
  x = textfield_->font().GetStringWidth(WideToUTF16(L"b"));
  SimulateSelectionHandleDrag(gfx::Point(-x, 0), 2);
  EXPECT_EQ(WideToUTF16(L"bc\x05e1\x05e2"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(false);

  // Select [\x5e1] from right to left.
  textfield_->SelectRange(ui::Range(3, 4));
  EXPECT_EQ(WideToUTF16(L"\x05e1"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(false);

  /* TODO(xji): for bidi text "abcDEF" whose display is "abcFEDhij", when click
     right of 'D' and select [D] then move the left selection handle to left
     by one character, it should select [ED], instead it selects [F].
     Reason: click right of 'D' and left of 'h' return the same x-axis position,
     pass this position to FindCursorPosition() returns index of 'h'. which
     means the selection start changed from 3 to 6.
     Need further investigation on whether this is a bug in Pango and how to
     work around it.
  // Drag selection handle 2 to left by 1 char.
  x = textfield_->font().GetStringWidth(WideToUTF16(L"\x05e2"));
  SimulateSelectionHandleDrag(gfx::Point(-x, 0), 2);
  EXPECT_EQ(WideToUTF16(L"\x05e1\x05e2"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(false);
  */

  // Drag selection handle 1 to right by 1 char.
  x = textfield_->font().GetStringWidth(WideToUTF16(L"d"));
  SimulateSelectionHandleDrag(gfx::Point(x, 0), 1);
  EXPECT_EQ(WideToUTF16(L"\x05e2\x05e3"L"d"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(true);

  // Select [\x5e1] from left to right.
  textfield_->SelectRange(ui::Range(4, 3));
  EXPECT_EQ(WideToUTF16(L"\x05e1"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(false);

  /* TODO(xji): see detail of above commented out test case.
  // Drag selection handle 1 to left by 1 char.
  x = textfield_->font().GetStringWidth(WideToUTF16(L"\x05e2"));
  SimulateSelectionHandleDrag(gfx::Point(-x, 0), 1);
  EXPECT_EQ(WideToUTF16(L"\x05e1\x05e2"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(true);
  */

  // Drag selection handle 2 to right by 1 char.
  x = textfield_->font().GetStringWidth(WideToUTF16(L"d"));
  SimulateSelectionHandleDrag(gfx::Point(x, 0), 2);
  EXPECT_EQ(WideToUTF16(L"\x05e2\x05e3"L"d"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(false);

  // Select [\x05r3] from right to left.
  textfield_->SelectRange(ui::Range(5, 6));
  EXPECT_EQ(WideToUTF16(L"\x05e3"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(false);

  // Drag selection handle 2 to left by 1 char.
  x = textfield_->font().GetStringWidth(WideToUTF16(L"c"));
  SimulateSelectionHandleDrag(gfx::Point(-x, 0), 2);
  EXPECT_EQ(WideToUTF16(L"c\x05e1\x05e2"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(false);

  // Drag selection handle 1 to right by 1 char.
  x = textfield_->font().GetStringWidth(WideToUTF16(L"\x05e2"));
  SimulateSelectionHandleDrag(gfx::Point(x, 0), 1);
  EXPECT_EQ(WideToUTF16(L"c\x05e1"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(true);

  // Select [\x05r3] from left to right.
  textfield_->SelectRange(ui::Range(6, 5));
  EXPECT_EQ(WideToUTF16(L"\x05e3"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(false);

  // Drag selection handle 1 to left by 1 char.
  x = textfield_->font().GetStringWidth(WideToUTF16(L"c"));
  SimulateSelectionHandleDrag(gfx::Point(-x, 0), 1);
  EXPECT_EQ(WideToUTF16(L"c\x05e1\x05e2"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(true);

  // Drag selection handle 2 to right by 1 char.
  x = textfield_->font().GetStringWidth(WideToUTF16(L"\x05e2"));
  SimulateSelectionHandleDrag(gfx::Point(x, 0), 2);
  EXPECT_EQ(WideToUTF16(L"c\x05e1"), textfield_->GetSelectedText());
  VERIFY_HANDLE_POSITIONS(false);
}

#if defined(USE_AURA)
TEST_F(TouchSelectionControllerImplTest,
       DoubleTapInTextfieldWithCursorHandleShouldSelectWord) {
  CreateTextfield();
  textfield_->SetText(ASCIIToUTF16("some text"));
  aura::test::EventGenerator generator(
      textfield_->GetWidget()->GetNativeView()->GetRootWindow());

  // Tap the textfield to invoke touch selection.
  generator.GestureTapAt(gfx::Point(10, 10));

  // Cursor handle should be visible.
  EXPECT_FALSE(textfield_->HasSelection());
  VERIFY_HANDLE_POSITIONS(false);

  // Double tap on the cursor handle position. We want to check that the cursor
  // handle is not eating the event and that the event is falling through to the
  // textfield.
  gfx::Point cursor_pos = GetCursorHandlePosition();
  generator.GestureTapAt(cursor_pos);
  generator.GestureTapAt(cursor_pos);
  EXPECT_TRUE(textfield_->HasSelection());
  VERIFY_HANDLE_POSITIONS(false);
}
#endif

}  // namespace views
