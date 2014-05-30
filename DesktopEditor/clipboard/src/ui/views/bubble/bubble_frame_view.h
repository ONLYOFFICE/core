// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_BUBBLE_BUBBLE_FRAME_VIEW_H_
#define UI_VIEWS_BUBBLE_BUBBLE_FRAME_VIEW_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "ui/gfx/insets.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/window/non_client_view.h"

namespace views {

class Label;
class LabelButton;
class BubbleBorder;

// The non-client frame view of bubble-styled widgets.
class VIEWS_EXPORT BubbleFrameView : public NonClientFrameView,
                                     public ButtonListener {
 public:
  explicit BubbleFrameView(const gfx::Insets& content_margins);
  virtual ~BubbleFrameView();

  // NonClientFrameView overrides:
  virtual gfx::Rect GetBoundsForClientView() const OVERRIDE;
  virtual gfx::Rect GetWindowBoundsForClientBounds(
      const gfx::Rect& client_bounds) const OVERRIDE;
  virtual int NonClientHitTest(const gfx::Point& point) OVERRIDE;
  virtual void GetWindowMask(const gfx::Size& size,
                             gfx::Path* window_mask) OVERRIDE;
  virtual void ResetWindowControls() OVERRIDE;
  virtual void UpdateWindowIcon() OVERRIDE;
  virtual void UpdateWindowTitle() OVERRIDE;

  // View overrides:
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void Layout() OVERRIDE;
  virtual std::string GetClassName() const OVERRIDE;
  virtual void ChildPreferredSizeChanged(View* child) OVERRIDE;

  // Overridden from ButtonListener:
  virtual void ButtonPressed(Button* sender, const ui::Event& event) OVERRIDE;

  // Use bubble_border() and SetBubbleBorder(), not border() and set_border().
  BubbleBorder* bubble_border() const { return bubble_border_; }
  void SetBubbleBorder(BubbleBorder* border);

  gfx::Insets content_margins() const { return content_margins_; }

  void SetTitle(const string16& title);
  void SetShowCloseButton(bool show);
  void SetTitlebarExtraView(View* view);

  void set_can_drag(bool can_drag) { can_drag_ = can_drag; }

  // Given the size of the contents and the rect to point at, returns the bounds
  // of the bubble window. The bubble's arrow location may change if the bubble
  // does not fit on the monitor and |adjust_if_offscreen| is true.
  gfx::Rect GetUpdatedWindowBounds(const gfx::Rect& anchor_rect,
                                   gfx::Size client_size,
                                   bool adjust_if_offscreen);

 protected:
  // Returns the bounds for the monitor showing the specified |rect|.
  // This function is virtual to support testing environments.
  virtual gfx::Rect GetMonitorBounds(const gfx::Rect& rect);

 private:
  FRIEND_TEST_ALL_PREFIXES(BubbleFrameViewTest, GetBoundsForClientView);

  // Mirrors the bubble's arrow location on the |vertical| or horizontal axis,
  // if the generated window bounds don't fit in the monitor bounds.
  void MirrorArrowIfOffScreen(bool vertical,
                              const gfx::Rect& anchor_rect,
                              const gfx::Size& client_size);

  // Adjust the bubble's arrow offsets if the generated window bounds don't fit
  // in the monitor bounds.
  void OffsetArrowIfOffScreen(const gfx::Rect& anchor_rect,
                              const gfx::Size& client_size);

  // The bubble border.
  BubbleBorder* bubble_border_;

  // Margins between the content and the inside of the border, in pixels.
  gfx::Insets content_margins_;

  // The optional title and (x) close button.
  Label* title_;
  LabelButton* close_;

  // When supplied, this view is placed in the titlebar between the title and
  // (x) close button.
  View* titlebar_extra_view_;

  // A flag controlling the ability to drag this frame.
  bool can_drag_;

  DISALLOW_COPY_AND_ASSIGN(BubbleFrameView);
};

}  // namespace views

#endif  // UI_VIEWS_BUBBLE_BUBBLE_FRAME_VIEW_H_
