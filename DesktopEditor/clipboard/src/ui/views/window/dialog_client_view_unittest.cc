// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/basictypes.h"
#include "base/utf_string_conversions.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/ui_base_types.h"
#include "ui/views/test/test_views.h"
#include "ui/views/window/dialog_client_view.h"
#include "ui/views/window/dialog_delegate.h"

namespace views {

class TestDialogClientView : public DialogClientView {
 public:
  TestDialogClientView(View* contents_view,
                       DialogDelegate* delegate)
      : DialogClientView(contents_view),
        delegate_(delegate) {}
  virtual ~TestDialogClientView() {}

  // DialogClientView implementation.
  virtual DialogDelegate* GetDialogDelegate() const OVERRIDE {
    return delegate_;
  }

  View* GetContentsView() {
    return contents_view();
  }

  void CreateExtraViews() {
    CreateExtraView();
    CreateFootnoteView();
  }

 private:
  DialogDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(TestDialogClientView);
};

class DialogClientViewTest : public testing::Test,
                             public DialogDelegateView {
 public:
  DialogClientViewTest()
      : dialog_buttons_(ui::DIALOG_BUTTON_NONE),
        extra_view_(NULL),
        footnote_view_(NULL) {}
  virtual ~DialogClientViewTest() {}

  // testing::Test implementation.
  virtual void SetUp() OVERRIDE {
    dialog_buttons_ = ui::DIALOG_BUTTON_NONE;
    contents_.reset(new StaticSizedView(gfx::Size(100, 200)));
    client_view_.reset(new TestDialogClientView(contents_.get(), this));
  }

  // DialogDelegateView implementation.
  virtual View* GetContentsView() OVERRIDE {
    return contents_.get();
  }
  virtual View* CreateExtraView() OVERRIDE {
    return extra_view_;
  }
  virtual View* CreateFootnoteView() OVERRIDE {
    return footnote_view_;
  }
  virtual int GetDialogButtons() const OVERRIDE {
    return dialog_buttons_;
  }

 protected:
  void ResizeAndLayoutClientView() {
    client_view_->SizeToPreferredSize();
    client_view_->Layout();
  }

  // Maes sure that the content view is sized correctly. Width must be at least
  // the requested amount, but height should always match exactly.
  void CheckContentsIsSetToPreferredSize() {
    ResizeAndLayoutClientView();
    gfx::Size preferred_size = contents_->GetPreferredSize();
    EXPECT_EQ(preferred_size.height(), contents_->bounds().height());
    EXPECT_LE(preferred_size.width(), contents_->bounds().width());
    EXPECT_EQ(contents_->bounds().x(), client_view()->bounds().x());
    EXPECT_EQ(contents_->bounds().y(), client_view()->bounds().y());
    EXPECT_EQ(contents_->bounds().right(), client_view()->bounds().right());
  }

  // Sets the buttons to show in the dialog and refreshes the dialog.
  void SetDialogButtons(int dialog_buttons) {
    dialog_buttons_ = dialog_buttons;
    client_view_->UpdateDialogButtons();
  }

  // Sets the extra view.
  void SetExtraView(View* view) {
    DCHECK(!extra_view_);
    extra_view_ = view;
    client_view_->CreateExtraViews();
  }

  // Sets the footnote view.
  void SetFootnoteView(View* view) {
    DCHECK(!footnote_view_);
    footnote_view_ = view;
    client_view_->CreateExtraViews();
  }

  TestDialogClientView* client_view() { return client_view_.get(); }

 private:
  // The contents of the dialog.
  scoped_ptr<View> contents_;
  // The DialogClientView that's being tested.
  scoped_ptr<TestDialogClientView> client_view_;
  // The bitmask of buttons to show in the dialog.
  int dialog_buttons_;
  View* extra_view_;  // weak
  View* footnote_view_;  // weak

  DISALLOW_COPY_AND_ASSIGN(DialogClientViewTest);
};

TEST_F(DialogClientViewTest, ButtonStateCanChange) {
  ResizeAndLayoutClientView();
  int no_button_height = client_view()->bounds().height();

  SetDialogButtons(ui::DIALOG_BUTTON_OK | ui::DIALOG_BUTTON_CANCEL);
  ResizeAndLayoutClientView();
  EXPECT_GT(client_view()->bounds().height(), no_button_height);
}

// Test that the contents view gets its preferred size in the basic dialog
// configuration.
TEST_F(DialogClientViewTest, ContentsSize) {
  CheckContentsIsSetToPreferredSize();
  EXPECT_EQ(GetContentsView()->bounds().bottom(),
            client_view()->bounds().bottom());
}

// Test the effect of the button strip on layout.
TEST_F(DialogClientViewTest, LayoutWithButtons) {
  SetDialogButtons(ui::DIALOG_BUTTON_OK | ui::DIALOG_BUTTON_CANCEL);
  CheckContentsIsSetToPreferredSize();
  EXPECT_LT(GetContentsView()->bounds().bottom(),
            client_view()->bounds().bottom());
  gfx::Size no_extra_view_size = client_view()->bounds().size();

  View* extra_view = new StaticSizedView(gfx::Size(200, 200));
  SetExtraView(extra_view);
  CheckContentsIsSetToPreferredSize();
  EXPECT_GT(client_view()->bounds().height(), no_extra_view_size.height());
  int width_of_extra_view = extra_view->bounds().width();

  // Visibility of extra view is respected.
  extra_view->SetVisible(false);
  CheckContentsIsSetToPreferredSize();
  EXPECT_EQ(no_extra_view_size.height(), client_view()->bounds().height());
  EXPECT_EQ(no_extra_view_size.width(), client_view()->bounds().width());

  // Try with a reduced-size dialog.
  extra_view->SetVisible(true);
  client_view()->SetBoundsRect(gfx::Rect(gfx::Point(0, 0), no_extra_view_size));
  client_view()->Layout();
  DCHECK_GT(width_of_extra_view, extra_view->bounds().width());
}

// Test the effect of the footnote view on layout.
TEST_F(DialogClientViewTest, LayoutWithFootnote) {
  CheckContentsIsSetToPreferredSize();
  gfx::Size no_footnote_size = client_view()->bounds().size();

  View* footnote_view = new StaticSizedView(gfx::Size(200, 200));
  SetFootnoteView(footnote_view);
  CheckContentsIsSetToPreferredSize();
  EXPECT_GT(client_view()->bounds().height(), no_footnote_size.height());
  EXPECT_EQ(200, footnote_view->bounds().height());
  gfx::Size with_footnote_size = client_view()->bounds().size();
  EXPECT_EQ(with_footnote_size.width(), footnote_view->bounds().width());

  SetDialogButtons(ui::DIALOG_BUTTON_CANCEL);
  CheckContentsIsSetToPreferredSize();
  EXPECT_LE(with_footnote_size.height(), client_view()->bounds().height());
  EXPECT_LE(with_footnote_size.width(), client_view()->bounds().width());
  gfx::Size with_footnote_and_button_size = client_view()->bounds().size();

  SetDialogButtons(ui::DIALOG_BUTTON_NONE);
  footnote_view->SetVisible(false);
  CheckContentsIsSetToPreferredSize();
  EXPECT_EQ(no_footnote_size.height(), client_view()->bounds().height());
  EXPECT_EQ(no_footnote_size.width(), client_view()->bounds().width());
}

// Test that GetHeightForWidth is respected for the footnote view.
TEST_F(DialogClientViewTest, LayoutWithFootnoteHeightForWidth) {
  CheckContentsIsSetToPreferredSize();
  gfx::Size no_footnote_size = client_view()->bounds().size();

  View* footnote_view = new ProportionallySizedView(3);
  SetFootnoteView(footnote_view);
  CheckContentsIsSetToPreferredSize();
  EXPECT_GT(client_view()->bounds().height(), no_footnote_size.height());
  EXPECT_EQ(footnote_view->bounds().width() * 3,
            footnote_view->bounds().height());
}

}  // namespace views
