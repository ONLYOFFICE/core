// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_OAK_OAK_WINDOW_H_
#define UI_OAK_OAK_WINDOW_H_

#include "ui/oak/oak_tree_model.h"
#include "ui/views/controls/tree/tree_view_controller.h"
#include "ui/views/widget/widget_delegate.h"

namespace views {
class TableView;
}

namespace oak {
namespace internal {

class OakTreeModel;
class OakDetailsModel;

class OakWindow : public views::WidgetDelegateView,
                  public views::TreeViewController {
 public:
  OakWindow();
  virtual ~OakWindow();

  static views::Widget* instance;

  // Overridden from views::WidgetDelegateView:
  virtual bool CanResize() const OVERRIDE;
  virtual bool CanMaximize() const OVERRIDE;
  virtual base::string16 GetWindowTitle() const OVERRIDE;
  virtual views::View* GetContentsView() OVERRIDE;
  virtual gfx::ImageSkia GetWindowIcon() OVERRIDE;
  virtual bool ShouldShowWindowIcon() const OVERRIDE;
  virtual void DeleteDelegate() OVERRIDE;

 private:
  // Overridden from views::View:
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void ViewHierarchyChanged(bool is_add,
                                    views::View* parent,
                                    views::View* child) OVERRIDE;
  virtual void Layout() OVERRIDE;

  // Overridden from views::TreeViewController:
  virtual void OnTreeViewSelectionChanged(views::TreeView* tree) OVERRIDE;

  void Init();

  scoped_ptr<views::TreeView> tree_;
  scoped_ptr<TreeOfWindows> tree_model_;
  views::View* tree_container_;

  gfx::Rect separator_rect_;

  scoped_ptr<views::TableView> details_;
  scoped_ptr<OakDetailsModel> details_model_;
  views::View* details_container_;

  DISALLOW_COPY_AND_ASSIGN(OakWindow);
};

}  // namespace internal
}  // namespace oak

#endif  // UI_OAK_OAK_WINDOW_H_
