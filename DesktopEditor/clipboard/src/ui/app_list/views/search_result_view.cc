// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/search_result_view.h"

#include "ui/app_list/app_list_constants.h"
#include "ui/app_list/search_result.h"
#include "ui/app_list/views/search_result_list_view.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font.h"
#include "ui/gfx/image/image_skia_operations.h"
#include "ui/gfx/render_text.h"
#include "ui/views/controls/button/image_button.h"
#include "ui/views/controls/image_view.h"

namespace {

const int kPreferredWidth = 300;
const int kPreferredHeight = 52;
const int kIconDimension = 32;
const int kIconPadding = 14;
const int kIconViewWidth = kIconDimension + 2 * kIconPadding;
const int kTextTrailPadding = kIconPadding;
const int kBorderSize = 1;

// Maximum dimensions of a search result's action icons.
const int kActionIconDimension = 24;

// Total space (including padding) used for each action icon's button.
const int kActionButtonWidth = 32;

// Extra margin at the right of the rightmost action icon.
const int kActionButtonRightMargin = 8;

const SkColor kBorderColor = SkColorSetRGB(0xE5, 0xE5, 0xE5);

const SkColor kDefaultTextColor = SkColorSetRGB(0x33, 0x33, 0x33);
const SkColor kDimmedTextColor = SkColorSetRGB(0x96, 0x96, 0x96);
const SkColor kURLTextColor = SkColorSetRGB(0x00, 0x99, 0x33);

// A non-interactive image view to display result icon.
class IconView : public views::ImageView {
 public:
  IconView() : ImageView() {}
  virtual ~IconView() {}

 private:
  // views::View overrides:
  virtual bool HitTestRect(const gfx::Rect& rect) const OVERRIDE {
    return false;
  }

  DISALLOW_COPY_AND_ASSIGN(IconView);
};

// Creates a RenderText of given |text| and |styles|. Caller takes ownership
// of returned RenderText.
gfx::RenderText* CreateRenderText(const string16& text,
                                  const app_list::SearchResult::Tags& tags) {
  gfx::RenderText* render_text = gfx::RenderText::CreateInstance();
  render_text->SetText(text);
  render_text->SetColor(kDefaultTextColor);

  for (app_list::SearchResult::Tags::const_iterator it = tags.begin();
       it != tags.end();
       ++it) {
    // NONE means default style so do nothing.
    if (it->styles == app_list::SearchResult::Tag::NONE)
      continue;

    if (it->styles & app_list::SearchResult::Tag::MATCH)
      render_text->ApplyStyle(gfx::BOLD, true, it->range);
    if (it->styles & app_list::SearchResult::Tag::DIM)
      render_text->ApplyColor(kDimmedTextColor, it->range);
    else if (it->styles & app_list::SearchResult::Tag::URL)
      render_text->ApplyColor(kURLTextColor, it->range);
  }

  return render_text;
}

}  // namespace

namespace app_list {

// static
const char SearchResultView::kViewClassName[] = "ui/app_list/SearchResultView";

SearchResultView::SearchResultView(SearchResultListView* list_view,
                                   SearchResultViewDelegate* delegate)
    : views::CustomButton(this),
      result_(NULL),
      list_view_(list_view),
      delegate_(delegate),
      icon_(new IconView) {
  AddChildView(icon_);
}

SearchResultView::~SearchResultView() {
  ClearResultNoRepaint();
}

void SearchResultView::SetResult(SearchResult* result) {
  ClearResultNoRepaint();

  result_ = result;
  if (result_)
    result_->AddObserver(this);

  OnIconChanged();
  OnActionIconsChanged();
  UpdateTitleText();
  UpdateDetailsText();
  SchedulePaint();
}

void SearchResultView::ClearResultNoRepaint() {
  if (result_)
    result_->RemoveObserver(this);
  result_ = NULL;
}

void SearchResultView::UpdateTitleText() {
  if (!result_ || result_->title().empty()) {
    title_text_.reset();
  } else {
    title_text_.reset(CreateRenderText(result_->title(),
                                       result_->title_tags()));
  }
}

void SearchResultView::UpdateDetailsText() {
  if (!result_ || result_->details().empty()) {
    details_text_.reset();
  } else {
    details_text_.reset(CreateRenderText(result_->details(),
                                         result_->details_tags()));
  }
}

std::string SearchResultView::GetClassName() const {
  return kViewClassName;
}

gfx::Size SearchResultView::GetPreferredSize() {
  return gfx::Size(kPreferredWidth, kPreferredHeight);
}

void SearchResultView::Layout() {
  gfx::Rect rect(GetContentsBounds());
  if (rect.IsEmpty())
    return;

  gfx::Rect icon_bounds(rect);
  icon_bounds.set_width(kIconViewWidth);
  icon_bounds.Inset(kIconPadding, (rect.height() - kIconDimension) / 2);
  icon_bounds.Intersect(rect);
  icon_->SetBoundsRect(icon_bounds);

  size_t num_buttons = action_buttons_.size();
  for (size_t i = 0; i < num_buttons; ++i) {
    views::ImageButton* button = action_buttons_[i];
    gfx::Rect button_bounds(
        rect.right() - kActionButtonRightMargin -
            (num_buttons - i) * kActionButtonWidth,
        (rect.y() + rect.height() - kActionIconDimension) / 2,
        kActionButtonWidth, kActionIconDimension);
    button->SetBoundsRect(button_bounds);
  }
}

void SearchResultView::OnPaint(gfx::Canvas* canvas) {
  gfx::Rect rect(GetContentsBounds());
  if (rect.IsEmpty())
    return;

  gfx::Rect content_rect(rect);
  content_rect.set_height(rect.height() - kBorderSize);

  const bool selected = list_view_->IsResultViewSelected(this);
  const bool hover = state() == STATE_HOVERED || state() == STATE_PRESSED;
  if (hover && selected)
    canvas->FillRect(content_rect, kHoverAndPushedColor);
  else if (selected || hover)
    canvas->FillRect(content_rect, kSelectedColor);
  else
    canvas->FillRect(content_rect, kContentsBackgroundColor);

  gfx::Rect border_bottom = gfx::SubtractRects(rect, content_rect);
  canvas->FillRect(border_bottom, kBorderColor);

  gfx::Rect text_bounds(rect);
  text_bounds.set_x(kIconViewWidth);
  text_bounds.set_width(
      rect.width() - kIconViewWidth - kTextTrailPadding -
      action_buttons_.size() * kActionButtonWidth -
      (!action_buttons_.empty() ? kActionButtonRightMargin : 0));
  text_bounds.set_x(GetMirroredXWithWidthInView(text_bounds.x(),
                                                text_bounds.width()));

  if (title_text_ && details_text_) {
    gfx::Size title_size(text_bounds.width(),
                         title_text_->GetStringSize().height());
    gfx::Size details_size(text_bounds.width(),
                           details_text_->GetStringSize().height());
    int total_height = title_size.height() + + details_size.height();
    int y = text_bounds.y() + (text_bounds.height() - total_height) / 2;

    title_text_->SetDisplayRect(gfx::Rect(gfx::Point(text_bounds.x(), y),
                                          title_size));
    title_text_->Draw(canvas);

    y += title_size.height();
    details_text_->SetDisplayRect(gfx::Rect(gfx::Point(text_bounds.x(), y),
                                            details_size));
    details_text_->Draw(canvas);
  } else if (title_text_) {
    gfx::Size title_size(text_bounds.width(),
                         title_text_->GetStringSize().height());
    gfx::Rect centered_title_rect(text_bounds);
    centered_title_rect.ClampToCenteredSize(title_size);
    title_text_->SetDisplayRect(centered_title_rect);
    title_text_->Draw(canvas);
  }
}

void SearchResultView::ButtonPressed(views::Button* sender,
                                     const ui::Event& event) {
  if (sender == this) {
    delegate_->SearchResultActivated(this, event);
    return;
  }

  for (size_t i = 0; i < action_buttons_.size(); ++i) {
    if (sender == action_buttons_[i]) {
      delegate_->SearchResultActionActivated(this, i, event);
      return;
    }
  }
  NOTREACHED() << "Got unexpected button press on " << sender;
}

void SearchResultView::OnIconChanged() {
  gfx::ImageSkia image(result_ ? result_->icon() : gfx::ImageSkia());
  // Note this might leave the view with an old icon. But it is needed to avoid
  // flash when a SearchResult's icon is loaded asynchronously. In this case, it
  // looks nicer to keep the stale icon for a little while on screen instead of
  // clearing it out. It should work correctly as long as the SearchResult does
  // not forget to SetIcon when it's ready.
  if (image.isNull())
    return;

  // Scales down big icons but leave small ones unchanged.
  if (image.width() > kIconDimension || image.height() > kIconDimension) {
    image = gfx::ImageSkiaOperations::CreateResizedImage(
        image,
        skia::ImageOperations::RESIZE_BEST,
        gfx::Size(kIconDimension, kIconDimension));
  } else {
    icon_->ResetImageSize();
  }

  icon_->SetImage(image);
}

void SearchResultView::OnActionIconsChanged() {
  while (action_buttons_.size() >
         (result_ ? result_->action_icons().size() : 0)) {
    RemoveChildView(action_buttons_.back());
    action_buttons_.pop_back();
  }

  if (result_) {
    while (action_buttons_.size() < result_->action_icons().size()) {
      views::ImageButton* button = new views::ImageButton(this);
      button->SetImageAlignment(views::ImageButton::ALIGN_CENTER,
                                views::ImageButton::ALIGN_MIDDLE);
      AddChildView(button);
      action_buttons_.push_back(button);
    }

    const SearchResult::ActionIconSets& icons = result_->action_icons();
    for (size_t i = 0; i < icons.size(); ++i) {
      const SearchResult::ActionIconSet& icon = icons.at(i);
      views::ImageButton* button = action_buttons_[i];
      button->SetImage(views::CustomButton::STATE_NORMAL, &icon.base_image);
      button->SetImage(views::CustomButton::STATE_HOVERED, &icon.hover_image);
      button->SetImage(views::CustomButton::STATE_PRESSED, &icon.pressed_image);
      button->SetTooltipText(icon.tooltip_text);
    }
  }
}

}  // namespace app_list
