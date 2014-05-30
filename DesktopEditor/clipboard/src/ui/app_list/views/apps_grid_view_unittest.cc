// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/apps_grid_view.h"

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/timer.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/app_list/app_list_item_model.h"
#include "ui/app_list/app_list_model.h"
#include "ui/app_list/pagination_model.h"
#include "ui/app_list/test/app_list_test_model.h"
#include "ui/app_list/views/app_list_item_view.h"
#include "ui/app_list/views/test/apps_grid_view_test_api.h"

namespace app_list {
namespace test {

namespace {

const int kIconDimension = 48;
const int kCols = 2;
const int kRows = 2;
const int kTilesPerPage = kCols * kRows;

const int kWidth = 320;
const int kHeight = 240;

class PageFlipWaiter : public PaginationModelObserver {
 public:
  PageFlipWaiter(base::MessageLoopForUI* ui_loop, PaginationModel* model)
      : ui_loop_(ui_loop), model_(model), wait_(false), page_changed_(false) {
    model_->AddObserver(this);
  }

  virtual ~PageFlipWaiter() {
    model_->RemoveObserver(this);
  }

  bool Wait(int time_out_ms) {
    DCHECK(!wait_);
    wait_ = true;
    page_changed_ = false;
    wait_timer_.Stop();
    wait_timer_.Start(FROM_HERE,
                      base::TimeDelta::FromMilliseconds(time_out_ms),
                      this, &PageFlipWaiter::OnWaitTimeOut);
    ui_loop_->Run();
    wait_ = false;
    return page_changed_;
  }

 private:
  void OnWaitTimeOut() {
    ui_loop_->Quit();
  }

  // PaginationModelObserver overrides:
  virtual void TotalPagesChanged() OVERRIDE {
  }
  virtual void SelectedPageChanged(int old_selected,
                                   int new_selected) OVERRIDE {
    page_changed_ = true;
    if (wait_)
      ui_loop_->Quit();
  }
  virtual void TransitionChanged() OVERRIDE {
  }

  base::MessageLoopForUI* ui_loop_;
  PaginationModel* model_;
  bool wait_;
  bool page_changed_;
  base::OneShotTimer<PageFlipWaiter> wait_timer_;

  DISALLOW_COPY_AND_ASSIGN(PageFlipWaiter);
};

}  // namespace

class AppsGridViewTest : public testing::Test {
 public:
  AppsGridViewTest() {}
  virtual ~AppsGridViewTest() {}

  // testing::Test overrides:
  virtual void SetUp() OVERRIDE {
    model_.reset(new AppListTestModel);
    pagination_model_.reset(new PaginationModel);

    apps_grid_view_.reset(new AppsGridView(NULL, pagination_model_.get()));
    apps_grid_view_->SetLayout(kIconDimension, kCols, kRows);
    apps_grid_view_->SetBoundsRect(gfx::Rect(gfx::Size(kWidth, kHeight)));
    apps_grid_view_->SetModel(model_.get());

    test_api_.reset(new AppsGridViewTestApi(apps_grid_view_.get()));
  }
  virtual void TearDown() OVERRIDE {
    apps_grid_view_.reset();  // Release apps grid view before models.
  }

 protected:
  AppListItemView* GetItemViewAt(int index) {
    return static_cast<AppListItemView*>(
        test_api_->GetViewAtModelIndex(index));
  }

  AppListItemView* GetItemViewForPoint(const gfx::Point& point) {
    for (size_t i = 0; i < model_->apps()->item_count(); ++i) {
      AppListItemView* view = GetItemViewAt(i);
      if (view->bounds().Contains(point))
        return view;
    }
    return NULL;
  }

  gfx::Rect GetItemTileRectAt(int row, int col) {
    DCHECK_GT(model_->apps()->item_count(), 0u);

    gfx::Insets insets(apps_grid_view_->GetInsets());
    gfx::Rect rect(gfx::Point(insets.left(), insets.top()),
                   GetItemViewAt(0)->bounds().size());
    rect.Offset(col * rect.width(), row * rect.height());
    return rect;
  }

  // Points are in |apps_grid_view_|'s coordinates.
  void SimulateDrag(AppsGridView::Pointer pointer,
                    const gfx::Point& from,
                    const gfx::Point& to) {
    AppListItemView* view = GetItemViewForPoint(from);
    DCHECK(view);

    gfx::Point translated_from = gfx::PointAtOffsetFromOrigin(
        from - view->bounds().origin());
    gfx::Point translated_to = gfx::PointAtOffsetFromOrigin(
        to - view->bounds().origin());

    ui::MouseEvent pressed_event(ui::ET_MOUSE_PRESSED,
                                 translated_from, from, 0);
    apps_grid_view_->InitiateDrag(view, pointer, pressed_event);

    ui::MouseEvent drag_event(ui::ET_MOUSE_DRAGGED,
                              translated_to, to, 0);
    apps_grid_view_->UpdateDrag(view, pointer, drag_event);
  }

  void SimulateKeyPress(ui::KeyboardCode key_code) {
    ui::KeyEvent key_event(ui::ET_KEY_PRESSED, key_code, 0, false);
    apps_grid_view_->OnKeyPressed(key_event);
  }

  scoped_ptr<AppListTestModel> model_;
  scoped_ptr<PaginationModel> pagination_model_;
  scoped_ptr<AppsGridView> apps_grid_view_;
  scoped_ptr<AppsGridViewTestApi> test_api_;

  base::MessageLoopForUI message_loop_;

 private:
  DISALLOW_COPY_AND_ASSIGN(AppsGridViewTest);
};

TEST_F(AppsGridViewTest, CreatePage) {
  // Fully populates a page.
  const int kPages = 1;
  model_->PopulateApps(kPages * kTilesPerPage);
  EXPECT_EQ(kPages, pagination_model_->total_pages());

  // Adds one more and gets a new page created.
  model_->AddItem(std::string("Extra"));
  EXPECT_EQ(kPages + 1, pagination_model_->total_pages());
}

TEST_F(AppsGridViewTest, EnsureHighlightedVisible) {
  const int kPages = 3;
  model_->PopulateApps(kPages * kTilesPerPage);
  EXPECT_EQ(kPages, pagination_model_->total_pages());
  EXPECT_EQ(0, pagination_model_->selected_page());

  // Highlight first one and last one one first page and first page should be
  // selected.
  model_->HighlightItemAt(0);
  EXPECT_EQ(0, pagination_model_->selected_page());
  model_->HighlightItemAt(kTilesPerPage - 1);
  EXPECT_EQ(0, pagination_model_->selected_page());

  // Highlight first one on 2nd page and 2nd page should be selected.
  model_->HighlightItemAt(kTilesPerPage + 1);
  EXPECT_EQ(1, pagination_model_->selected_page());

  // Highlight last one in the model and last page should be selected.
  model_->HighlightItemAt(model_->apps()->item_count() - 1);
  EXPECT_EQ(kPages - 1, pagination_model_->selected_page());
}

TEST_F(AppsGridViewTest, RemoveSelectedLastApp) {
  const int kTotalItems = 2;
  const int kLastItemIndex = kTotalItems - 1;

  model_->PopulateApps(kTotalItems);

  AppListItemView* last_view = GetItemViewAt(kLastItemIndex);
  apps_grid_view_->SetSelectedView(last_view);
  model_->apps()->DeleteAt(kLastItemIndex);

  EXPECT_FALSE(apps_grid_view_->IsSelectedView(last_view));

  // No crash happens.
  AppListItemView* view = GetItemViewAt(0);
  apps_grid_view_->SetSelectedView(view);
  EXPECT_TRUE(apps_grid_view_->IsSelectedView(view));
}

TEST_F(AppsGridViewTest, MouseDrag) {
  const int kTotalItems = 4;
  model_->PopulateApps(kTotalItems);
  EXPECT_EQ(std::string("Item 0,Item 1,Item 2,Item 3"),
            model_->GetModelContent());

  gfx::Point from = GetItemTileRectAt(0, 0).CenterPoint();
  gfx::Point to = GetItemTileRectAt(0, 1).CenterPoint();

  // Dragging changes model order.
  SimulateDrag(AppsGridView::MOUSE, from, to);
  apps_grid_view_->EndDrag(false);
  EXPECT_EQ(std::string("Item 1,Item 0,Item 2,Item 3"),
            model_->GetModelContent());
  test_api_->LayoutToIdealBounds();

  // Canceling drag should keep existing order.
  SimulateDrag(AppsGridView::MOUSE, from, to);
  apps_grid_view_->EndDrag(true);
  EXPECT_EQ(std::string("Item 1,Item 0,Item 2,Item 3"),
            model_->GetModelContent());
  test_api_->LayoutToIdealBounds();

  // Deleting an item keeps remaining intact.
  SimulateDrag(AppsGridView::MOUSE, from, to);
  model_->apps()->DeleteAt(1);
  apps_grid_view_->EndDrag(false);
  EXPECT_EQ(std::string("Item 1,Item 2,Item 3"),
            model_->GetModelContent());
  test_api_->LayoutToIdealBounds();

  // Adding a launcher item cancels the drag and respects the order.
  SimulateDrag(AppsGridView::MOUSE, from, to);
  model_->AddItem(std::string("Extra"));
  apps_grid_view_->EndDrag(false);
  EXPECT_EQ(std::string("Item 1,Item 2,Item 3,Extra"),
            model_->GetModelContent());
  test_api_->LayoutToIdealBounds();
}

TEST_F(AppsGridViewTest, MouseDragFlipPage) {
  test_api_->SetPageFlipDelay(10);
  pagination_model_->SetTransitionDurations(10, 10);

  PageFlipWaiter page_flip_waiter(&message_loop_,
                                  pagination_model_.get());

  const int kPages = 3;
  model_->PopulateApps(kPages * kTilesPerPage);
  EXPECT_EQ(kPages, pagination_model_->total_pages());
  EXPECT_EQ(0, pagination_model_->selected_page());

  gfx::Point from = GetItemTileRectAt(0, 0).CenterPoint();
  gfx::Point to = gfx::Point(apps_grid_view_->width(),
                             apps_grid_view_->height() / 2);

  // Drag to right edge.
  SimulateDrag(AppsGridView::MOUSE, from, to);

  // Page should be flipped after sometime.
  EXPECT_TRUE(page_flip_waiter.Wait(100));
  EXPECT_EQ(1, pagination_model_->selected_page());

  // Stay there and page should be flipped again.
  EXPECT_TRUE(page_flip_waiter.Wait(100));
  EXPECT_EQ(2, pagination_model_->selected_page());

  // Stay there longer and no page flip happen since we are at the last page.
  EXPECT_FALSE(page_flip_waiter.Wait(100));
  EXPECT_EQ(2, pagination_model_->selected_page());

  apps_grid_view_->EndDrag(true);

  // Now drag to the left edge and test the other direction.
  to.set_x(0);

  SimulateDrag(AppsGridView::MOUSE, from, to);

  EXPECT_TRUE(page_flip_waiter.Wait(100));
  EXPECT_EQ(1, pagination_model_->selected_page());

  EXPECT_TRUE(page_flip_waiter.Wait(100));
  EXPECT_EQ(0, pagination_model_->selected_page());

  EXPECT_FALSE(page_flip_waiter.Wait(100));
  EXPECT_EQ(0, pagination_model_->selected_page());
  apps_grid_view_->EndDrag(true);
}

TEST_F(AppsGridViewTest, SimultaneousDrag) {
  const int kTotalItems = 4;
  model_->PopulateApps(kTotalItems);
  EXPECT_EQ(std::string("Item 0,Item 1,Item 2,Item 3"),
            model_->GetModelContent());

  gfx::Point mouse_from = GetItemTileRectAt(0, 0).CenterPoint();
  gfx::Point mouse_to = GetItemTileRectAt(0, 1).CenterPoint();

  gfx::Point touch_from = GetItemTileRectAt(1, 0).CenterPoint();
  gfx::Point touch_to = GetItemTileRectAt(1, 1).CenterPoint();

  // Starts a mouse drag first then a touch drag.
  SimulateDrag(AppsGridView::MOUSE, mouse_from, mouse_to);
  SimulateDrag(AppsGridView::TOUCH, touch_from, touch_to);
  // Finishes the drag and mouse drag wins.
  apps_grid_view_->EndDrag(false);
  EXPECT_EQ(std::string("Item 1,Item 0,Item 2,Item 3"),
            model_->GetModelContent());
  test_api_->LayoutToIdealBounds();

  // Starts a touch drag first then a mouse drag.
  SimulateDrag(AppsGridView::TOUCH, touch_from, touch_to);
  SimulateDrag(AppsGridView::MOUSE, mouse_from, mouse_to);
  // Finishes the drag and touch drag wins.
  apps_grid_view_->EndDrag(false);
  EXPECT_EQ(std::string("Item 1,Item 0,Item 3,Item 2"),
            model_->GetModelContent());
  test_api_->LayoutToIdealBounds();
}

TEST_F(AppsGridViewTest, HighlightWithKeyboard) {
  const int kPages = 2;
  model_->PopulateApps(kPages * kTilesPerPage);

  const int first_index = 0;
  const int last_index = kPages * kTilesPerPage - 1;
  const int last_index_on_page1 = kTilesPerPage - 1;
  const int first_index_on_page2 = kTilesPerPage;

  // Try moving off the item beyond the first one.
  apps_grid_view_->SetSelectedView(GetItemViewAt(first_index));
  SimulateKeyPress(ui::VKEY_UP);
  EXPECT_TRUE(apps_grid_view_->IsSelectedView(GetItemViewAt(first_index)));
  SimulateKeyPress(ui::VKEY_LEFT);
  EXPECT_TRUE(apps_grid_view_->IsSelectedView(GetItemViewAt(first_index)));

  // Move to the last item and try to go past it.
  apps_grid_view_->SetSelectedView(GetItemViewAt(last_index));
  SimulateKeyPress(ui::VKEY_DOWN);
  EXPECT_TRUE(apps_grid_view_->IsSelectedView(GetItemViewAt(last_index)));
  SimulateKeyPress(ui::VKEY_RIGHT);
  EXPECT_TRUE(apps_grid_view_->IsSelectedView(GetItemViewAt(last_index)));

  // Move right on last item on page 1 should get to first item on page 2
  // and vice vesa.
  apps_grid_view_->SetSelectedView(GetItemViewAt(last_index_on_page1));
  SimulateKeyPress(ui::VKEY_RIGHT);
  EXPECT_TRUE(apps_grid_view_->IsSelectedView(GetItemViewAt(
      first_index_on_page2)));
  SimulateKeyPress(ui::VKEY_LEFT);
  EXPECT_TRUE(apps_grid_view_->IsSelectedView(GetItemViewAt(
      last_index_on_page1)));

  // Up/down changes page similarly to the above left/right cases.
  apps_grid_view_->SetSelectedView(GetItemViewAt(last_index_on_page1));
  SimulateKeyPress(ui::VKEY_DOWN);
  EXPECT_TRUE(apps_grid_view_->IsSelectedView(GetItemViewAt(
      last_index_on_page1 + kCols)));
  SimulateKeyPress(ui::VKEY_UP);
  EXPECT_TRUE(apps_grid_view_->IsSelectedView(GetItemViewAt(
      last_index_on_page1)));

  // After page switch, arrow keys select first item on current page.
  apps_grid_view_->SetSelectedView(GetItemViewAt(first_index));
  pagination_model_->SelectPage(1, false);
  SimulateKeyPress(ui::VKEY_UP);
  EXPECT_TRUE(apps_grid_view_->IsSelectedView(GetItemViewAt(
      first_index_on_page2)));
}

}  // namespace test
}  // namespace app_list
