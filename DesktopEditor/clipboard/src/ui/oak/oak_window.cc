// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/oak/oak_window.h"

#include "base/utf_string_conversions.h"
#include "grit/ui_resources.h"
#include "ui/aura/root_window.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/image/image.h"
#include "ui/oak/oak.h"
#include "ui/oak/oak_aura_window_display.h"
#include "ui/views/controls/table/table_view.h"
#include "ui/views/controls/tree/tree_view.h"
#include "ui/views/layout/layout_constants.h"
#include "ui/views/widget/widget.h"

namespace oak {
namespace internal {
namespace {
const SkColor kBorderColor = SkColorSetRGB(0xCC, 0xCC, 0xCC);
}  // namespace

// static
views::Widget* OakWindow::instance = NULL;

////////////////////////////////////////////////////////////////////////////////
// OakWindow, public:

OakWindow::OakWindow() : tree_(NULL), tree_container_(NULL), details_(NULL) {
}

OakWindow::~OakWindow() {
  // The tree/table need to be destroyed before the model.
  tree_.reset();
  details_.reset();
}

////////////////////////////////////////////////////////////////////////////////
// OakWindow, views::WidgetDelegateView implementation:

bool OakWindow::CanResize() const {
  return true;
}

bool OakWindow::CanMaximize() const {
  return true;
}

base::string16 OakWindow::GetWindowTitle() const {
  return ASCIIToUTF16("Oak");
}

views::View* OakWindow::GetContentsView() {
  return this;
}

gfx::ImageSkia OakWindow::GetWindowIcon() {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  return *rb.GetImageNamed(IDR_OAK).ToImageSkia();
}

bool OakWindow::ShouldShowWindowIcon() const {
  return true;
}

void OakWindow::DeleteDelegate() {
  instance = NULL;
  delete this;
}

////////////////////////////////////////////////////////////////////////////////
// OakWindow, views::View overrides:

void OakWindow::OnPaint(gfx::Canvas* canvas) {
  canvas->DrawColor(SK_ColorWHITE);
  canvas->FillRect(separator_rect_, kBorderColor);
}

void OakWindow::ViewHierarchyChanged(bool is_add,
                                     views::View* parent,
                                     views::View* child) {
  if (is_add && child == this)
    Init();
}

void OakWindow::Layout() {
  gfx::Rect content_bounds = GetLocalBounds();
  content_bounds.Inset(views::kPanelHorizMargin, views::kPanelVertMargin);

  int tree_height =
      (content_bounds.height() / 2) - views::kUnrelatedControlVerticalSpacing;
  gfx::Rect tree_bounds = content_bounds;
  tree_bounds.set_height(tree_height);
  tree_container_->SetBoundsRect(tree_bounds);

  separator_rect_ = content_bounds;
  separator_rect_.set_y(
      tree_bounds.bottom() + views::kRelatedControlVerticalSpacing);
  separator_rect_.set_height(1);

  gfx::Rect details_bounds = content_bounds;
  details_bounds.set_y(
      separator_rect_.bottom() + views::kRelatedControlVerticalSpacing);
  details_bounds.set_height(content_bounds.bottom() - details_bounds.y());
  details_container_->SetBoundsRect(details_bounds);
}

////////////////////////////////////////////////////////////////////////////////
// OakWindow, views::TreeViewController implementation:

void OakWindow::OnTreeViewSelectionChanged(views::TreeView* tree) {
  details_model_->SetValue(tree_model_->AsNode(tree->GetSelectedNode())->value);
}

////////////////////////////////////////////////////////////////////////////////
// OakWindow, private:

void OakWindow::Init() {
  tree_model_.reset(
      GenerateModel(GetWidget()->GetNativeView()->GetRootWindow()));
  tree_.reset(new views::TreeView);
  tree_->set_owned_by_client();
  tree_->SetController(this);
  tree_->SetModel(tree_model_.get());
  tree_container_ = tree_->CreateParentIfNecessary();
  AddChildView(tree_container_);

  details_model_.reset(new OakAuraWindowDisplay);
  std::vector<ui::TableColumn> columns;
  columns.push_back(ui::TableColumn());
  details_.reset(new views::TableView(details_model_.get(),
                                      columns,
                                      views::TEXT_ONLY,
                                      true));
  details_->set_owned_by_client();
  details_container_ = details_->CreateParentIfNecessary();
  details_->SetModel(details_model_.get());
  AddChildView(details_container_);

  OnTreeViewSelectionChanged(tree_.get());
}

}  // namespace internal

void ShowOakWindowWithContext(gfx::NativeView context) {
  if (!internal::OakWindow::instance) {
    // TODO(erg): Do we want to reuse this window in times with a different
    // context? For now, this is OK, but if we ever use Oak outside of the ash
    // shell, we run into crbug.com/165759.
    internal::OakWindow::instance =
        views::Widget::CreateWindowWithContextAndBounds(
            new internal::OakWindow, context, gfx::Rect(10, 10, 500, 500));
  }
  internal::OakWindow::instance->Show();
}

}  // namespace oak
