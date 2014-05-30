// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/scrollbar/bitmap_scroll_bar.h"

#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/message_loop.h"
#include "base/string16.h"
#include "base/utf_string_conversions.h"
#include "build/build_config.h"
#include "grit/ui_strings.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/controls/menu/menu.h"
#include "ui/views/controls/scroll_view.h"
#include "ui/views/controls/scrollbar/base_scroll_bar_thumb.h"
#include "ui/views/widget/widget.h"

#if defined(OS_LINUX)
#include "ui/views/screen.h"
#endif

#undef min
#undef max

namespace views {

namespace {

// The distance the mouse can be dragged outside the bounds of the thumb during
// dragging before the scrollbar will snap back to its regular position.
const int kScrollThumbDragOutSnap = 100;

///////////////////////////////////////////////////////////////////////////////
//
// AutorepeatButton
//
//  A button that activates on mouse pressed rather than released, and that
//  continues to fire the clicked action as the mouse button remains pressed
//  down on the button.
//
///////////////////////////////////////////////////////////////////////////////
class AutorepeatButton : public ImageButton {
 public:
  explicit AutorepeatButton(ButtonListener* listener)
      : ImageButton(listener),
        repeater_(base::Bind(&AutorepeatButton::NotifyClick,
                             base::Unretained(this))) {
  }
  virtual ~AutorepeatButton() {}

 protected:
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE {
    Button::NotifyClick(event);
    repeater_.Start();
    return true;
  }

  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE {
    OnMouseCaptureLost();
  }

  virtual void OnMouseCaptureLost() OVERRIDE {
    repeater_.Stop();
  }

 private:
  void NotifyClick() {
#if defined(OS_WIN)
    gfx::Point cursor_point(GetMessagePos());
#elif defined(OS_LINUX)
    gfx::Point cursor_point = gfx::Screen::GetCursorScreenPoint();
#endif
    ui::MouseEvent event(ui::ET_MOUSE_RELEASED,
                         cursor_point, cursor_point,
                         ui::EF_LEFT_MOUSE_BUTTON);
    Button::NotifyClick(event);
  }

  // The repeat controller that we use to repeatedly click the button when the
  // mouse button is down.
  RepeatController repeater_;

  DISALLOW_COPY_AND_ASSIGN(AutorepeatButton);
};

///////////////////////////////////////////////////////////////////////////////
//
// BitmapScrollBarThumb
//
//  A view that acts as the thumb in the scroll bar track that the user can
//  drag to scroll the associated contents view within the viewport.
//
///////////////////////////////////////////////////////////////////////////////
class BitmapScrollBarThumb : public BaseScrollBarThumb {
 public:
  explicit BitmapScrollBarThumb(BitmapScrollBar* scroll_bar)
      : BaseScrollBarThumb(scroll_bar),
        scroll_bar_(scroll_bar) {
  }
  virtual ~BitmapScrollBarThumb() { }

  // View overrides:
  virtual gfx::Size GetPreferredSize() OVERRIDE {
    return gfx::Size(background_image()->width(),
                     start_cap_image()->height() +
                         end_cap_image()->height() +
                         grippy_image()->height());
  }

 protected:
  // View overrides:
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE {
    canvas->DrawImageInt(*start_cap_image(), 0, 0);
    int top_cap_height = start_cap_image()->height();
    int bottom_cap_height = end_cap_image()->height();
    int thumb_body_height = height() - top_cap_height - bottom_cap_height;
    canvas->TileImageInt(*background_image(), 0, top_cap_height,
                         background_image()->width(), thumb_body_height);
    canvas->DrawImageInt(*end_cap_image(), 0,
                         height() - bottom_cap_height);

    // Paint the grippy over the track.
    int grippy_x = (width() - grippy_image()->width()) / 2;
    int grippy_y = (thumb_body_height - grippy_image()->height()) / 2;
    canvas->DrawImageInt(*grippy_image(), grippy_x, grippy_y);
  }

 private:
  // Returns the image rendered at the start of the thumb.
  gfx::ImageSkia* start_cap_image() const {
    return scroll_bar_->images_[BitmapScrollBar::THUMB_START_CAP][GetState()];
  }

  // Returns the image rendered at the end of the thumb.
  gfx::ImageSkia* end_cap_image() const {
    return scroll_bar_->images_[BitmapScrollBar::THUMB_END_CAP][GetState()];
  }

  // Returns the image that is tiled in the background of the thumb between
  // the start and the end caps.
  gfx::ImageSkia* background_image() const {
    return scroll_bar_->images_[BitmapScrollBar::THUMB_MIDDLE][GetState()];
  }

  // Returns the image that is rendered in the middle of the thumb
  // transparently over the background image.
  gfx::ImageSkia* grippy_image() const {
    return scroll_bar_->images_[BitmapScrollBar::THUMB_GRIPPY]
        [CustomButton::STATE_NORMAL];
  }

  // The BitmapScrollBar that owns us.
  BitmapScrollBar* scroll_bar_;

  DISALLOW_COPY_AND_ASSIGN(BitmapScrollBarThumb);
};

}  // namespace

///////////////////////////////////////////////////////////////////////////////
// BitmapScrollBar, public:

BitmapScrollBar::BitmapScrollBar(bool horizontal, bool show_scroll_buttons)
    : BaseScrollBar(horizontal, new BitmapScrollBarThumb(this)),
      prev_button_(new AutorepeatButton(this)),
      next_button_(new AutorepeatButton(this)),
      show_scroll_buttons_(show_scroll_buttons) {
  if (!show_scroll_buttons_) {
    prev_button_->SetVisible(false);
    next_button_->SetVisible(false);
  }

  AddChildView(prev_button_);
  AddChildView(next_button_);

  set_context_menu_controller(this);
  prev_button_->set_context_menu_controller(this);
  next_button_->set_context_menu_controller(this);
}

void BitmapScrollBar::SetImage(ScrollBarPart part,
                               CustomButton::ButtonState state,
                               gfx::ImageSkia* image_skia) {
  DCHECK(part < PART_COUNT);
  DCHECK(state < CustomButton::STATE_COUNT);
  switch (part) {
    case PREV_BUTTON:
      prev_button_->SetImage(state, image_skia);
      break;
    case NEXT_BUTTON:
      next_button_->SetImage(state, image_skia);
      break;
    case THUMB_START_CAP:
    case THUMB_MIDDLE:
    case THUMB_END_CAP:
    case THUMB_GRIPPY:
    case THUMB_TRACK:
      images_[part][state] = image_skia;
      break;
  }
}

int BitmapScrollBar::GetLayoutSize() const {
  gfx::Size prefsize = prev_button_->GetPreferredSize();
  return IsHorizontal() ? prefsize.height() : prefsize.width();
}

gfx::Rect BitmapScrollBar::GetTrackBounds() const {
  gfx::Size prefsize = prev_button_->GetPreferredSize();
  if (IsHorizontal()) {
    if (!show_scroll_buttons_)
      prefsize.set_width(0);
    int new_width =
        std::max(0, width() - (prefsize.width() * 2));
    gfx::Rect track_bounds(prefsize.width(), 0, new_width, prefsize.height());
    return track_bounds;
  }
  if (!show_scroll_buttons_)
    prefsize.set_height(0);
  gfx::Rect track_bounds(0, prefsize.height(), prefsize.width(),
                         std::max(0, height() - (prefsize.height() * 2)));
  return track_bounds;
}

///////////////////////////////////////////////////////////////////////////////
// BitmapScrollBar, View implementation:

gfx::Size BitmapScrollBar::GetPreferredSize() {
  // In this case, we're returning the desired width of the scrollbar and its
  // minimum allowable height.
  gfx::Size button_prefsize = prev_button_->GetPreferredSize();
  return gfx::Size(button_prefsize.width(), button_prefsize.height() * 2);
}

void BitmapScrollBar::Layout() {
  // Size and place the two scroll buttons.
  if (show_scroll_buttons_) {
    gfx::Size prefsize = prev_button_->GetPreferredSize();
    prev_button_->SetBounds(0, 0, prefsize.width(), prefsize.height());
    prefsize = next_button_->GetPreferredSize();
    if (IsHorizontal()) {
      next_button_->SetBounds(width() - prefsize.width(), 0, prefsize.width(),
                              prefsize.height());
    } else {
      next_button_->SetBounds(0, height() - prefsize.height(), prefsize.width(),
                              prefsize.height());
    }
  } else {
    prev_button_->SetBounds(0, 0, 0, 0);
    next_button_->SetBounds(0, 0, 0, 0);
  }

  BaseScrollBarThumb* thumb = GetThumb();
  // Size and place the thumb
  gfx::Size thumb_prefsize = thumb->GetPreferredSize();
  gfx::Rect track_bounds = GetTrackBounds();

  // Preserve the height/width of the thumb (depending on orientation) as set
  // by the last call to |Update|, but coerce the width/height to be the
  // appropriate value for the images provided.
  if (IsHorizontal()) {
    thumb->SetBounds(thumb->x(), thumb->y(), thumb->width(),
                      thumb_prefsize.height());
  } else {
    thumb->SetBounds(thumb->x(), thumb->y(), thumb_prefsize.width(),
                     thumb->height());
  }

  // Hide the thumb if the track isn't tall enough to display even a tiny
  // thumb. The user can only use the mousewheel, scroll buttons or keyboard
  // in this scenario.
  if ((IsHorizontal() && (track_bounds.width() < thumb_prefsize.width()) ||
      (!IsHorizontal() && (track_bounds.height() < thumb_prefsize.height())))) {
    thumb->SetVisible(false);
  } else if (!thumb->visible()) {
    thumb->SetVisible(true);
  }
}

///////////////////////////////////////////////////////////////////////////////
// BitmapScrollBar, View implementation:

void BitmapScrollBar::OnPaint(gfx::Canvas* canvas) {
  // Paint the track.
  gfx::Rect track_bounds = GetTrackBounds();
  canvas->TileImageInt(*images_[THUMB_TRACK][GetThumbTrackState()],
                       track_bounds.x(), track_bounds.y(),
                       track_bounds.width(), track_bounds.height());
}

///////////////////////////////////////////////////////////////////////////////
// BitmapScrollBar, ButtonListener implementation:

void BitmapScrollBar::ButtonPressed(Button* sender, const ui::Event& event) {
  if (sender == prev_button_) {
    ScrollByAmount(SCROLL_PREV_LINE);
  } else if (sender == next_button_) {
    ScrollByAmount(SCROLL_NEXT_LINE);
  }
}

}  // namespace views
