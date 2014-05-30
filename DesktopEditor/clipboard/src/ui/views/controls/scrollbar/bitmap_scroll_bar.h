// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SCROLLBAR_BITMAP_SCROLL_BAR_H_
#define UI_VIEWS_CONTROLS_SCROLLBAR_BITMAP_SCROLL_BAR_H_

#include "ui/views/controls/scrollbar/base_scroll_bar.h"

namespace views {

namespace {
class BitmapScrollBarThumb;
}

///////////////////////////////////////////////////////////////////////////////
//
// BitmapScrollBar
//
//  A ScrollBar subclass that implements a scroll bar rendered using images
//  that the user provides. There are images for the up and down buttons, as
//  well as for the thumb and track. This is intended for creating UIs that
//  have customized, non-native appearances, like floating HUDs etc.
//
///////////////////////////////////////////////////////////////////////////////
class VIEWS_EXPORT BitmapScrollBar : public BaseScrollBar,
                                     public ButtonListener {
 public:
  BitmapScrollBar(bool horizontal, bool show_scroll_buttons);
  virtual ~BitmapScrollBar() { }

  // A list of parts that the user may supply images for.
  enum ScrollBarPart {
    // The button used to represent scrolling up/left by 1 line.
    PREV_BUTTON = 0,
    // The button used to represent scrolling down/right by 1 line.
    // IMPORTANT: The code assumes the prev and next
    // buttons have equal width and equal height.
    NEXT_BUTTON,
    // The top/left segment of the thumb on the scrollbar.
    THUMB_START_CAP,
    // The tiled background image of the thumb.
    THUMB_MIDDLE,
    // The bottom/right segment of the thumb on the scrollbar.
    THUMB_END_CAP,
    // The grippy that is rendered in the center of the thumb.
    THUMB_GRIPPY,
    // The tiled background image of the thumb track.
    THUMB_TRACK,
    PART_COUNT
  };

  // Sets the image to be rendered for the specified part and state.
  void SetImage(ScrollBarPart part,
                CustomButton::ButtonState state,
                gfx::ImageSkia* image_skia);


  gfx::Rect GetTrackBounds() const;

 protected:
  // View overrides:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  // ScrollBar overrides:
  virtual int GetLayoutSize() const OVERRIDE;

  // BaseButton::ButtonListener overrides:
  virtual void ButtonPressed(Button* sender,
                             const ui::Event& event) OVERRIDE;

 private:
  // Up/Down/Left/Right buttons.
  ImageButton* prev_button_;
  ImageButton* next_button_;

  // The thumb needs to be able to access the part images.
  friend BitmapScrollBarThumb;
  gfx::ImageSkia* images_[PART_COUNT][CustomButton::STATE_COUNT];

  // True if the scroll buttons at each end of the scroll bar should be shown.
  bool show_scroll_buttons_;

  DISALLOW_COPY_AND_ASSIGN(BitmapScrollBar);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_SCROLLBAR_BITMAP_SCROLL_BAR_H_
