// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SLIDER_H_
#define UI_VIEWS_CONTROLS_SLIDER_H_

#include "ui/base/animation/animation_delegate.h"
#include "ui/views/view.h"
#include "ui/views/views_export.h"

typedef unsigned int SkColor;

namespace gfx {
class ImageSkia;
}

namespace ui {
class SlideAnimation;
}

namespace views {

class Slider;

enum SliderChangeReason {
  VALUE_CHANGED_BY_USER,  // value was changed by the user (by clicking, e.g.)
  VALUE_CHANGED_BY_API,   // value was changed by a call to SetValue.
};

class VIEWS_EXPORT SliderListener {
 public:
  virtual void SliderValueChanged(Slider* sender,
                                  float value,
                                  float old_value,
                                  SliderChangeReason reason) = 0;

  // Invoked when a drag starts or ends (more specifically, when the mouse
  // button is pressed or released).
  virtual void SliderDragStarted(Slider* sender) {}
  virtual void SliderDragEnded(Slider* sender) {}

 protected:
  virtual ~SliderListener() {}
};

class VIEWS_EXPORT Slider : public View,
                            public ui::AnimationDelegate {
 public:
  enum Orientation {
    HORIZONTAL,
    VERTICAL
  };

  Slider(SliderListener* listener, Orientation orientation);
  virtual ~Slider();

  float value() const { return value_; }
  void SetValue(float value);

  // Set the delta used for changing the value via keyboard.
  void SetKeyboardIncrement(float increment);

  void SetAccessibleName(const string16& name);

  void set_enable_accessibility_events(bool enabled) {
    accessibility_events_enabled_ = enabled;
  }

  void set_focus_border_color(SkColor color) { focus_border_color_ = color; }

  // Update UI based on control on/off state.
  void UpdateState(bool control_on);

 private:
  void SetValueInternal(float value, SliderChangeReason reason);

  // Should be called on the Mouse Down event. Used to calculate relative
  // position of the mouse cursor (or the touch point) on the button to
  // accurately move the button using the MoveButtonTo() method.
  void PrepareForMove(const gfx::Point& point);

  // Moves the button to the specified point and updates the value accordingly.
  void MoveButtonTo(const gfx::Point& point);

  // views::View overrides:
  virtual gfx::Size GetPreferredSize() OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void OnPaintFocusBorder(gfx::Canvas* canvas) OVERRIDE;

  // ui::EventHandler overrides:
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  // ui::AnimationDelegate overrides:
  virtual void AnimationProgressed(const ui::Animation* animation) OVERRIDE;

  SliderListener* listener_;
  Orientation orientation_;

  scoped_ptr<ui::SlideAnimation> move_animation_;

  float value_;
  float keyboard_increment_;
  float animating_value_;
  bool value_is_valid_;
  string16 accessible_name_;
  bool accessibility_events_enabled_;
  SkColor focus_border_color_;

  // Relative position of the mouse cursor (or the touch point) on the slider's
  // button.
  gfx::Point initial_button_offset_;

  const int* bar_active_images_;
  const int* bar_disabled_images_;
  const gfx::ImageSkia* thumb_;
  const gfx::ImageSkia* images_[4];
  int bar_height_;

  DISALLOW_COPY_AND_ASSIGN(Slider);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_SLIDER_H_
