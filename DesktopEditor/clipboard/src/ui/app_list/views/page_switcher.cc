// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/page_switcher.h"

#include <algorithm>

#include "third_party/skia/include/core/SkPath.h"
#include "ui/app_list/pagination_model.h"
#include "ui/base/animation/throb_animation.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/skia_util.h"
#include "ui/views/controls/button/custom_button.h"
#include "ui/views/layout/box_layout.h"

namespace {

const int kPreferredHeight = 57;

const int kMaxButtonSpacing = 18;
const int kMinButtonSpacing = 4;
const int kMaxButtonWidth = 68;
const int kMinButtonWidth = 28;
const int kButtonHeight = 6;
const int kButtonCornerRadius = 2;
const int kButtonStripPadding = 20;

const SkColor kHoverColor = SkColorSetRGB(0xB4, 0xB4, 0xB4);

const SkColor kNormalColor = SkColorSetRGB(0xE2, 0xE2, 0xE2);

const SkColor kSelectedColor = SkColorSetRGB(0x46, 0x8F, 0xFC);

class PageSwitcherButton : public views::CustomButton {
 public:
  explicit PageSwitcherButton(views::ButtonListener* listener)
      : views::CustomButton(listener),
        button_width_(kMaxButtonWidth),
        selected_range_(0) {
  }
  virtual ~PageSwitcherButton() {}

  void SetSelectedRange(double selected_range) {
    if (selected_range_ == selected_range)
      return;

    selected_range_ = selected_range;
    SchedulePaint();
  }

  void set_button_width(int button_width) { button_width_ = button_width; }

  // Overridden from views::View:
  virtual gfx::Size GetPreferredSize() OVERRIDE {
    return gfx::Size(button_width_, kButtonHeight);
  }

  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE {
    if (state() == STATE_HOVERED) {
      PaintButton(canvas, kHoverColor);
    } else {
      PaintButton(canvas, kNormalColor);
    }
  }

 private:
  // Paints a button that has two rounded corner at bottom.
  void PaintButton(gfx::Canvas* canvas, SkColor base_color) {
    gfx::Rect rect(GetContentsBounds());
    rect.ClampToCenteredSize(gfx::Size(button_width_, kButtonHeight));

    SkPath path;
    path.addRoundRect(gfx::RectToSkRect(rect),
                      SkIntToScalar(kButtonCornerRadius),
                      SkIntToScalar(kButtonCornerRadius));

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor(base_color);
    canvas->DrawPath(path, paint);

    int selected_start_x = 0;
    int selected_width = 0;
    if (selected_range_ > 0) {
      selected_width = selected_range_ * rect.width();
    } else if (selected_range_ < 0) {
      selected_width =  -selected_range_ * rect.width();
      selected_start_x = rect.right() - selected_width;
    }

    if (selected_width) {
      gfx::Rect selected_rect(rect);
      selected_rect.set_x(selected_start_x);
      selected_rect.set_width(selected_width);

      SkPath selected_path;
      selected_path.addRoundRect(gfx::RectToSkRect(selected_rect),
                                 SkIntToScalar(kButtonCornerRadius),
                                 SkIntToScalar(kButtonCornerRadius));
      paint.setColor(kSelectedColor);
      canvas->DrawPath(selected_path, paint);
    }
  }

  int button_width_;

  // [-1, 1] range that represents the portion of the button that should be
  // painted with kSelectedColor. Positive range starts from left side and
  // negative range starts from the right side.
  double selected_range_;

  DISALLOW_COPY_AND_ASSIGN(PageSwitcherButton);
};

// Gets PageSwitcherButton at |index| in |buttons|.
PageSwitcherButton* GetButtonByIndex(views::View* buttons, int index) {
  return static_cast<PageSwitcherButton*>(buttons->child_at(index));
}

}  // namespace

namespace app_list {

PageSwitcher::PageSwitcher(PaginationModel* model)
    : model_(model),
      buttons_(new views::View) {
  AddChildView(buttons_);

  TotalPagesChanged();
  SelectedPageChanged(-1, model->selected_page());
  model_->AddObserver(this);
}

PageSwitcher::~PageSwitcher() {
  model_->RemoveObserver(this);
}

int PageSwitcher::GetPageForPoint(const gfx::Point& point) const {
  if (!buttons_->bounds().Contains(point))
    return -1;

  gfx::Point buttons_point(point);
  views::View::ConvertPointToTarget(this, buttons_, &buttons_point);

  for (int i = 0; i < buttons_->child_count(); ++i) {
    const views::View* button = buttons_->child_at(i);
    if (button->bounds().Contains(buttons_point))
      return i;
  }

  return -1;
}

void PageSwitcher::UpdateUIForDragPoint(const gfx::Point& point) {
  int page = GetPageForPoint(point);

  const int button_count = buttons_->child_count();
  if (page >= 0 && page < button_count) {
    PageSwitcherButton* button =
        static_cast<PageSwitcherButton*>(buttons_->child_at(page));
    button->SetState(views::CustomButton::STATE_HOVERED);
    return;
  }

  for (int i = 0; i < button_count; ++i) {
    PageSwitcherButton* button =
        static_cast<PageSwitcherButton*>(buttons_->child_at(i));
    button->SetState(views::CustomButton::STATE_NORMAL);
  }
}

gfx::Size PageSwitcher::GetPreferredSize() {
  // Always return a size with correct height so that container resize is not
  // needed when more pages are added.
  return gfx::Size(buttons_->GetPreferredSize().width(),
                   kPreferredHeight);
}

void PageSwitcher::Layout() {
  gfx::Rect rect(GetContentsBounds());

  CalculateButtonWidthAndSpacing(rect.width());

  // Makes |buttons_| horizontally center and vertically fill.
  gfx::Size buttons_size(buttons_->GetPreferredSize());
  gfx::Rect buttons_bounds(rect.CenterPoint().x() - buttons_size.width() / 2,
                           rect.y(),
                           buttons_size.width(),
                           rect.height());
  buttons_->SetBoundsRect(gfx::IntersectRects(rect, buttons_bounds));
}

void PageSwitcher::CalculateButtonWidthAndSpacing(int contents_width) {
  const int button_count = buttons_->child_count();
  if (!button_count)
    return;

  contents_width -= 2 * kButtonStripPadding;

  int button_width = kMinButtonWidth;
  int button_spacing = kMinButtonSpacing;
  if (button_count > 1) {
    button_spacing = (contents_width - button_width * button_count) /
        (button_count - 1);
    button_spacing = std::min(kMaxButtonSpacing,
                              std::max(kMinButtonSpacing, button_spacing));
  }

  button_width = (contents_width - (button_count - 1) * button_spacing) /
      button_count;
  button_width = std::min(kMaxButtonWidth,
                          std::max(kMinButtonWidth, button_width));

  buttons_->SetLayoutManager(new views::BoxLayout(
      views::BoxLayout::kHorizontal, kButtonStripPadding, 0, button_spacing));
  for (int i = 0; i < button_count; ++i) {
    PageSwitcherButton* button =
        static_cast<PageSwitcherButton*>(buttons_->child_at(i));
    button->set_button_width(button_width);
  }
}

void PageSwitcher::ButtonPressed(views::Button* sender,
                                 const ui::Event& event) {
  for (int i = 0; i < buttons_->child_count(); ++i) {
    if (sender == static_cast<views::Button*>(buttons_->child_at(i))) {
      model_->SelectPage(i, true /* animate */);
      break;
    }
  }
}

void PageSwitcher::TotalPagesChanged() {
  buttons_->RemoveAllChildViews(true);
  for (int i = 0; i < model_->total_pages(); ++i) {
    PageSwitcherButton* button = new PageSwitcherButton(this);
    button->SetSelectedRange(i == model_->selected_page() ? 1 : 0);
    buttons_->AddChildView(button);
  }
  buttons_->SetVisible(model_->total_pages() > 1);
  Layout();
}

void PageSwitcher::SelectedPageChanged(int old_selected, int new_selected) {
  if (old_selected >= 0 && old_selected < buttons_->child_count())
    GetButtonByIndex(buttons_, old_selected)->SetSelectedRange(0);
  if (new_selected >= 0 && new_selected < buttons_->child_count())
    GetButtonByIndex(buttons_, new_selected)->SetSelectedRange(1);
}

void PageSwitcher::TransitionChanged() {
  const int current_page = model_->selected_page();
  const int target_page = model_->transition().target_page;

  double progress = model_->transition().progress;
  double remaining = progress - 1;

  if (current_page > target_page) {
    remaining = -remaining;
    progress = -progress;
  }

  GetButtonByIndex(buttons_, current_page)->SetSelectedRange(remaining);
  if (model_->is_valid_page(target_page))
    GetButtonByIndex(buttons_, target_page)->SetSelectedRange(progress);
}

}  // namespace app_list
