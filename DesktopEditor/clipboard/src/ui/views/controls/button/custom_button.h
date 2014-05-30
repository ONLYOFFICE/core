// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_BUTTON_CUSTOM_BUTTON_H_
#define UI_VIEWS_CONTROLS_BUTTON_CUSTOM_BUTTON_H_

#include "base/memory/scoped_ptr.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/events/event_constants.h"
#include "ui/views/controls/button/button.h"

namespace ui {
class ThrobAnimation;
}

namespace views {

class CustomButtonStateChangedDelegate;

// A button with custom rendering. The common base class of ImageButton and
// TextButton.
// Note that this type of button is not focusable by default and will not be
// part of the focus chain.  Call set_focusable(true) to make it part of the
// focus chain.
class VIEWS_EXPORT CustomButton : public Button,
                                  public ui::AnimationDelegate {
 public:
  // The menu button's class name.
  static const char kViewClassName[];

  virtual ~CustomButton();

  // Get/sets the current display state of the button.
  ButtonState state() const { return state_; }
  void SetState(ButtonState state);

  // Starts throbbing. See HoverAnimation for a description of cycles_til_stop.
  void StartThrobbing(int cycles_til_stop);

  // Stops throbbing immediately.
  void StopThrobbing();

  // Set how long the hover animation will last for.
  void SetAnimationDuration(int duration);

  void set_triggerable_event_flags(int triggerable_event_flags) {
    triggerable_event_flags_ = triggerable_event_flags;
  }
  int triggerable_event_flags() const { return triggerable_event_flags_; }

  // Sets whether |RequestFocus| should be invoked on a mouse press. The default
  // is true.
  void set_request_focus_on_press(bool value) {
    request_focus_on_press_ = value;
  }
  bool request_focus_on_press() const { return request_focus_on_press_; }

  // See description above field.
  void set_animate_on_state_change(bool value) {
    animate_on_state_change_ = value;
  }

  // Returns true if the mouse pointer is over this control.  Note that this
  // isn't the same as IsHotTracked() because the mouse may be over the control
  // when it's disabled.
  bool IsMouseHovered() const;

  void SetHotTracked(bool is_hot_tracked);
  bool IsHotTracked() const;

  // Overridden from View:
  virtual void OnEnabledChanged() OVERRIDE;
  virtual std::string GetClassName() const OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnMouseEntered(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseExited(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseMoved(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;
  virtual void ShowContextMenu(const gfx::Point& p,
                               bool is_mouse_gesture) OVERRIDE;
  virtual void OnDragDone() OVERRIDE;
  virtual void GetAccessibleState(ui::AccessibleViewState* state) OVERRIDE;
  virtual void VisibilityChanged(View* starting_from, bool is_visible) OVERRIDE;

  // Overridden from ui::AnimationDelegate:
  virtual void AnimationProgressed(const ui::Animation* animation) OVERRIDE;

  // Takes ownership of the delegate.
  void set_state_changed_delegate(CustomButtonStateChangedDelegate* delegate) {
    state_changed_delegate_.reset(delegate);
  }

 protected:
  // Construct the Button with a Listener. See comment for Button's ctor.
  explicit CustomButton(ButtonListener* listener);

  // Invoked from SetState() when SetState() is passed a value that differs from
  // the current state. CustomButton's implementation of StateChanged() does
  // nothing; this method is provided for subclasses that wish to do something
  // on state changes.
  virtual void StateChanged();

  // Returns true if the event is one that can trigger notifying the listener.
  // This implementation returns true if the left mouse button is down.
  virtual bool IsTriggerableEvent(const ui::Event& event);

  // Returns true if the button should become pressed when the user
  // holds the mouse down over the button. For this implementation,
  // we simply return IsTriggerableEvent(event).
  virtual bool ShouldEnterPushedState(const ui::Event& event);

  // Overridden from View:
  virtual void ViewHierarchyChanged(bool is_add,
                                    View* parent,
                                    View* child) OVERRIDE;
  virtual void OnBlur() OVERRIDE;

  // The button state (defined in implementation)
  ButtonState state_;

  // Hover animation.
  scoped_ptr<ui::ThrobAnimation> hover_animation_;

 private:
  // Should we animate when the state changes? Defaults to true.
  bool animate_on_state_change_;

  // Is the hover animation running because StartThrob was invoked?
  bool is_throbbing_;

  // Mouse event flags which can trigger button actions.
  int triggerable_event_flags_;

  // See description above setter.
  bool request_focus_on_press_;

  scoped_ptr<CustomButtonStateChangedDelegate> state_changed_delegate_;

  DISALLOW_COPY_AND_ASSIGN(CustomButton);
};

// Delegate for actions taken on state changes by CustomButton.
class VIEWS_EXPORT CustomButtonStateChangedDelegate {
public:
  virtual ~CustomButtonStateChangedDelegate() {}
  virtual void StateChanged(Button::ButtonState state) = 0;

protected:
  CustomButtonStateChangedDelegate() {}

private:
  DISALLOW_COPY_AND_ASSIGN(CustomButtonStateChangedDelegate);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_BUTTON_CUSTOM_BUTTON_H_
