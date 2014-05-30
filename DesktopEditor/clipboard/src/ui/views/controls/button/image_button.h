// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_IMAGE_BUTTON_H_
#define UI_VIEWS_CONTROLS_BUTTON_IMAGE_BUTTON_H_

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/controls/button/custom_button.h"

namespace views {

// An image button.

// Note that this type of button is not focusable by default and will not be
// part of the focus chain.  Call set_focusable(true) to make it part of the
// focus chain.

class VIEWS_EXPORT ImageButton : public CustomButton {
 public:
  enum HorizontalAlignment {
    ALIGN_LEFT = 0,
    ALIGN_CENTER,
    ALIGN_RIGHT
  };

  enum VerticalAlignment {
    ALIGN_TOP = 0,
    ALIGN_MIDDLE,
    ALIGN_BOTTOM
  };

  explicit ImageButton(ButtonListener* listener);
  virtual ~ImageButton();

  // Returns the image for a given |state|.
  virtual const gfx::ImageSkia& GetImage(ButtonState state) const;

  // Set the image the button should use for the provided state.
  virtual void SetImage(ButtonState state, const gfx::ImageSkia* image);

  // Set the background details.
  void SetBackground(SkColor color,
                     const gfx::ImageSkia* image,
                     const gfx::ImageSkia* mask);

  // Set an |image| to draw on top of the normal / hot / pushed image.
  // Pass NULL for no image.
  void SetOverlayImage(const gfx::ImageSkia* image);

  // Sets how the image is laid out within the button's bounds.
  void SetImageAlignment(HorizontalAlignment h_align,
                         VerticalAlignment v_align);

  // Overridden from View:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  // Sets preferred size, so it could be correctly positioned in layout even if
  // it is NULL.
  void SetPreferredSize(const gfx::Size& preferred_size) {
    preferred_size_ = preferred_size;
  }

 protected:
  // Returns the image to paint. This is invoked from paint and returns a value
  // from images.
  virtual gfx::ImageSkia GetImageToPaint();

  // Updates button background for |scale_factor|.
  void UpdateButtonBackground(ui::ScaleFactor scale_factor);

  // The images used to render the different states of this button.
  gfx::ImageSkia images_[STATE_COUNT];

  gfx::ImageSkia background_image_;

  // Image to draw on top of normal / hot / pushed image.  Usually empty.
  gfx::ImageSkia overlay_image_;

 private:
  FRIEND_TEST_ALL_PREFIXES(ImageButtonTest, Basics);
  FRIEND_TEST_ALL_PREFIXES(ImageButtonTest, ImagePositionWithBorder);

  // Returns the correct position of the image for painting.
  gfx::Point ComputeImagePaintPosition(const gfx::ImageSkia& image);

  // Image alignment.
  HorizontalAlignment h_alignment_;
  VerticalAlignment v_alignment_;
  gfx::Size preferred_size_;

  DISALLOW_COPY_AND_ASSIGN(ImageButton);
};

////////////////////////////////////////////////////////////////////////////////
//
// ToggleImageButton
//
// A toggle-able ImageButton.  It swaps out its graphics when toggled.
//
////////////////////////////////////////////////////////////////////////////////
class VIEWS_EXPORT ToggleImageButton : public ImageButton {
 public:
  explicit ToggleImageButton(ButtonListener* listener);
  virtual ~ToggleImageButton();

  // Change the toggled state.
  void SetToggled(bool toggled);

  // Like ImageButton::SetImage(), but to set the graphics used for the
  // "has been toggled" state.  Must be called for each button state
  // before the button is toggled.
  void SetToggledImage(ButtonState state, const gfx::ImageSkia* image);

  // Set the tooltip text displayed when the button is toggled.
  void SetToggledTooltipText(const string16& tooltip);

  // Overridden from ImageButton:
  virtual const gfx::ImageSkia& GetImage(ButtonState state) const OVERRIDE;
  virtual void SetImage(ButtonState state,
                        const gfx::ImageSkia* image) OVERRIDE;

  // Overridden from View:
  virtual bool GetTooltipText(const gfx::Point& p,
                              string16* tooltip) const OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;

 private:
  // The parent class's images_ member is used for the current images,
  // and this array is used to hold the alternative images.
  // We swap between the two when toggling.
  gfx::ImageSkia alternate_images_[STATE_COUNT];

  // True if the button is currently toggled.
  bool toggled_;

  // The parent class's tooltip_text_ is displayed when not toggled, and
  // this one is shown when toggled.
  string16 toggled_tooltip_text_;

  DISALLOW_COPY_AND_ASSIGN(ToggleImageButton);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_BUTTON_IMAGE_BUTTON_H_
