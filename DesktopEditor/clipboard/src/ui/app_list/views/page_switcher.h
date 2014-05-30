// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_PAGE_SWITCHER_H_
#define UI_APP_LIST_VIEWS_PAGE_SWITCHER_H_

#include "base/basictypes.h"
#include "ui/app_list/pagination_model_observer.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/view.h"

namespace app_list {

class PaginationModel;

// PageSwitcher represents its underlying PaginationModel with a button strip.
// Each page in the PageinationModel has a button in the strip and when the
// button is clicked, the corresponding page becomes selected.
class PageSwitcher : public views::View,
                     public views::ButtonListener,
                     public PaginationModelObserver {
 public:
  explicit PageSwitcher(PaginationModel* model);
  virtual ~PageSwitcher();

  // Returns the page index of the page switcher button under the point. If no
  // page switcher button is under the point, -1 is return. |point| is in
  // PageSwitcher's coordinates.
  int GetPageForPoint(const gfx::Point& point) const;

  // Shows hover for button under the point. |point| is in PageSwitcher's
  // coordinates.
  void UpdateUIForDragPoint(const gfx::Point& point);

  // Overridden from views::View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;

 private:
  void CalculateButtonWidthAndSpacing(int contents_width);

  // Overridden from views::ButtonListener:
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  // Overridden from PaginationModelObserver:
  virtual void TotalPagesChanged() OVERRIDE;
  virtual void SelectedPageChanged(int old_selected, int new_selected) OVERRIDE;
  virtual void TransitionChanged() OVERRIDE;

  PaginationModel* model_;  // Owned by parent AppListView.
  views::View* buttons_;  // Owned by views hierarchy.

  DISALLOW_COPY_AND_ASSIGN(PageSwitcher);
};

}  // namespace app_list

#endif  // UI_APP_LIST_VIEWS_PAGE_SWITCHER_H_
