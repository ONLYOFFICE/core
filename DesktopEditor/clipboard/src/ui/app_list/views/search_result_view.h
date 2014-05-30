// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_SEARCH_RESULT_VIEW_H_
#define UI_APP_LIST_VIEWS_SEARCH_RESULT_VIEW_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/app_list/search_result_observer.h"
#include "ui/views/controls/button/custom_button.h"

namespace gfx {
class RenderText;
}

namespace views {
class ImageButton;
class ImageView;
}

namespace app_list {

class SearchResult;
class SearchResultListView;
class SearchResultViewDelegate;

// SearchResultView displays a SearchResult.
class SearchResultView : public views::CustomButton,
                         public views::ButtonListener,
                         public SearchResultObserver {
 public:
  // Internal class name.
  static const char kViewClassName[];

  SearchResultView(SearchResultListView* list_view,
                   SearchResultViewDelegate* delegate);
  virtual ~SearchResultView();

  // Sets/gets SearchResult displayed by this view.
  void SetResult(SearchResult* result);
  const SearchResult* result() const { return result_; }

  // Clears reference to SearchResult but don't schedule repaint.
  void ClearResultNoRepaint();

 private:
  void UpdateTitleText();
  void UpdateDetailsText();

  // views::View overrides:
  virtual std::string GetClassName() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  // views::ButtonListener overrides:
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  // SearchResultObserver overrides:
  virtual void OnIconChanged() OVERRIDE;
  virtual void OnActionIconsChanged() OVERRIDE;

  SearchResult* result_;  // Owned by AppListModel::SearchResults.

  // Parent list view. Owned by views hierarchy.
  SearchResultListView* list_view_;

  // Not owned by us.
  SearchResultViewDelegate* delegate_;

  views::ImageView* icon_;  // Owned by views hierarchy.
  scoped_ptr<gfx::RenderText> title_text_;
  scoped_ptr<gfx::RenderText> details_text_;

  // Owned by the views hierarchy.
  std::vector<views::ImageButton*> action_buttons_;

  DISALLOW_COPY_AND_ASSIGN(SearchResultView);
};

}  // namespace app_list

#endif  // UI_APP_LIST_VIEWS_SEARCH_RESULT_VIEW_H_
