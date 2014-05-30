// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_EVENTS_EVENT_H_
#define UI_BASE_EVENTS_EVENT_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/event_types.h"
#include "base/logging.h"
#include "base/time.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/gestures/gesture_types.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/point.h"

namespace gfx {
class Transform;
}

namespace ui {
class EventTarget;

class UI_EXPORT Event {
 public:
  virtual ~Event();

  class DispatcherApi {
   public:
    explicit DispatcherApi(Event* event) : event_(event) {}

    void set_target(EventTarget* target) {
      event_->target_ = target;
    }

    void set_phase(EventPhase phase) { event_->phase_ = phase; }
    void set_result(int result) {
      event_->result_ = static_cast<EventResult>(result);
    }

   private:
    DispatcherApi();
    Event* event_;

    DISALLOW_COPY_AND_ASSIGN(DispatcherApi);
  };

  // For testing.
  class TestApi {
   public:
    explicit TestApi(Event* event) : event_(event) {}

    void set_time_stamp(base::TimeDelta time_stamp) {
      event_->time_stamp_ = time_stamp;
    }

   private:
    TestApi();
    Event* event_;
  };

  const base::NativeEvent& native_event() const { return native_event_; }
  EventType type() const { return type_; }
  const std::string& name() const { return name_; }
  // time_stamp represents time since machine was booted.
  const base::TimeDelta& time_stamp() const { return time_stamp_; }
  int flags() const { return flags_; }

  // This is only intended to be used externally by classes that are modifying
  // events in EventFilter::PreHandleKeyEvent().
  void set_flags(int flags) { flags_ = flags; }

  EventTarget* target() const { return target_; }
  EventPhase phase() const { return phase_; }
  EventResult result() const { return result_; }
  bool dispatch_to_hidden_targets() const {
    return dispatch_to_hidden_targets_;
  }

  // By default, events are "cancelable", this means any default processing that
  // the containing abstraction layer may perform can be prevented by calling
  // SetHandled(). SetHandled() or StopPropagation() must not be called for
  // events that are not cancelable.
  bool cancelable() const { return cancelable_; }

  // The following methods return true if the respective keys were pressed at
  // the time the event was created.
  bool IsShiftDown() const { return (flags_ & EF_SHIFT_DOWN) != 0; }
  bool IsControlDown() const { return (flags_ & EF_CONTROL_DOWN) != 0; }
  bool IsCapsLockDown() const { return (flags_ & EF_CAPS_LOCK_DOWN) != 0; }
  bool IsAltDown() const { return (flags_ & EF_ALT_DOWN) != 0; }

  bool IsKeyEvent() const {
    return type_ == ET_KEY_PRESSED ||
           type_ == ET_KEY_RELEASED ||
           type_ == ET_TRANSLATED_KEY_PRESS ||
           type_ == ET_TRANSLATED_KEY_RELEASE;
  }

  bool IsMouseEvent() const {
    return type_ == ET_MOUSE_PRESSED ||
           type_ == ET_MOUSE_DRAGGED ||
           type_ == ET_MOUSE_RELEASED ||
           type_ == ET_MOUSE_MOVED ||
           type_ == ET_MOUSE_ENTERED ||
           type_ == ET_MOUSE_EXITED ||
           type_ == ET_MOUSEWHEEL ||
           type_ == ET_MOUSE_CAPTURE_CHANGED;
  }

  bool IsTouchEvent() const {
    return type_ == ET_TOUCH_RELEASED ||
           type_ == ET_TOUCH_PRESSED ||
           type_ == ET_TOUCH_MOVED ||
           type_ == ET_TOUCH_STATIONARY ||
           type_ == ET_TOUCH_CANCELLED;
  }

  bool IsGestureEvent() const {
    switch (type_) {
      case ET_GESTURE_SCROLL_BEGIN:
      case ET_GESTURE_SCROLL_END:
      case ET_GESTURE_SCROLL_UPDATE:
      case ET_GESTURE_TAP:
      case ET_GESTURE_TAP_CANCEL:
      case ET_GESTURE_TAP_DOWN:
      case ET_GESTURE_BEGIN:
      case ET_GESTURE_END:
      case ET_GESTURE_TWO_FINGER_TAP:
      case ET_GESTURE_PINCH_BEGIN:
      case ET_GESTURE_PINCH_END:
      case ET_GESTURE_PINCH_UPDATE:
      case ET_GESTURE_LONG_PRESS:
      case ET_GESTURE_LONG_TAP:
      case ET_GESTURE_MULTIFINGER_SWIPE:
        return true;

      case ET_SCROLL_FLING_CANCEL:
      case ET_SCROLL_FLING_START:
        // These can be ScrollEvents too. EF_FROM_TOUCH determines if they're
        // Gesture or Scroll events.
        return (flags_ & EF_FROM_TOUCH) == EF_FROM_TOUCH;

      default:
        break;
    }
    return false;
  }

  bool IsScrollEvent() const {
    // Flings can be GestureEvents too. EF_FROM_TOUCH determins if they're
    // Gesture or Scroll events.
    return type_ == ET_SCROLL ||
           ((type_ == ET_SCROLL_FLING_START ||
           type_ == ET_SCROLL_FLING_CANCEL) &&
           !(flags() & EF_FROM_TOUCH));
  }

  bool IsScrollGestureEvent() const {
    return type_ == ET_GESTURE_SCROLL_BEGIN ||
           type_ == ET_GESTURE_SCROLL_UPDATE ||
           type_ == ET_GESTURE_SCROLL_END;
  }

  bool IsFlingScrollEvent() const {
    return type_ == ET_SCROLL_FLING_CANCEL ||
           type_ == ET_SCROLL_FLING_START;
  }

  // Returns true if the event has a valid |native_event_|.
  bool HasNativeEvent() const;

  // Immediately stops the propagation of the event. This must be called only
  // from an EventHandler during an event-dispatch. Any event handler that may
  // be in the list will not receive the event after this is called.
  // Note that StopPropagation() can be called only for cancelable events.
  void StopPropagation();
  bool stopped_propagation() const { return !!(result_ & ER_CONSUMED); }

  // Marks the event as having been handled. A handled event does not reach the
  // next event phase. For example, if an event is handled during the pre-target
  // phase, then the event is dispatched to all pre-target handlers, but not to
  // the target or post-target handlers.
  // Note that SetHandled() can be called only for cancelable events.
  void SetHandled();
  bool handled() const { return result_ != ER_UNHANDLED; }

 protected:
  Event(EventType type, base::TimeDelta time_stamp, int flags);
  Event(const base::NativeEvent& native_event, EventType type, int flags);
  Event(const Event& copy);
  void SetType(EventType type);
  void set_delete_native_event(bool delete_native_event) {
    delete_native_event_ = delete_native_event;
  }
  void set_cancelable(bool cancelable) { cancelable_ = cancelable; }
  void set_dispatch_to_hidden_targets(bool dispatch_to_hidden_targets) {
    dispatch_to_hidden_targets_ = dispatch_to_hidden_targets;
  }

  void set_time_stamp(const base::TimeDelta& time_stamp) {
    time_stamp_ = time_stamp;
  }

  void set_name(const std::string& name) { name_ = name; }

 private:
  void operator=(const Event&);

  // Safely initializes the native event members of this class.
  void Init();
  void InitWithNativeEvent(const base::NativeEvent& native_event);

  EventType type_;
  std::string name_;
  base::TimeDelta time_stamp_;
  int flags_;
  bool dispatch_to_hidden_targets_;
  base::NativeEvent native_event_;
  bool delete_native_event_;
  bool cancelable_;
  EventTarget* target_;
  EventPhase phase_;
  EventResult result_;
};

class UI_EXPORT CancelModeEvent : public Event {
 public:
  CancelModeEvent();
  virtual ~CancelModeEvent();
};

class UI_EXPORT LocatedEvent : public Event {
 public:
  // For testing.
  class TestApi : public Event::TestApi {
   public:
    explicit TestApi(LocatedEvent* located_event)
        : Event::TestApi(located_event),
          located_event_(located_event) {}

    void set_location(const gfx::Point& location) {
      located_event_->location_ = location;
    }

   private:
    TestApi();
    LocatedEvent* located_event_;
  };

  virtual ~LocatedEvent();

  int x() const { return location_.x(); }
  int y() const { return location_.y(); }
  void set_location(const gfx::Point& location) { location_ = location; }
  gfx::Point location() const { return location_; }
  void set_root_location(const gfx::Point& root_location) {
    root_location_ = root_location;
  }
  gfx::Point root_location() const { return root_location_; }

  // Transform the locations using |inverted_root_transform|.
  // This is applied to both |location_| and |root_location_|.
  virtual void UpdateForRootTransform(
      const gfx::Transform& inverted_root_transform);

  template <class T> void ConvertLocationToTarget(T* source, T* target) {
    if (target && target != source)
      T::ConvertPointToTarget(source, target, &location_);
  }

 protected:
  explicit LocatedEvent(const base::NativeEvent& native_event);

  // Create a new LocatedEvent which is identical to the provided model.
  // If source / target windows are provided, the model location will be
  // converted from |source| coordinate system to |target| coordinate system.
  template <class T>
  LocatedEvent(const LocatedEvent& model, T* source, T* target)
      : Event(model),
        location_(model.location_),
        root_location_(model.root_location_) {
    ConvertLocationToTarget(source, target);
  }

  // Used for synthetic events in testing.
  LocatedEvent(EventType type,
               const gfx::Point& location,
               const gfx::Point& root_location,
               base::TimeDelta time_stamp,
               int flags);

  gfx::Point location_;

  // |location_| multiplied by an optional transformation matrix for
  // rotations, animations and skews.
  gfx::Point root_location_;
};

class UI_EXPORT MouseEvent : public LocatedEvent {
 public:
  explicit MouseEvent(const base::NativeEvent& native_event);

  // Create a new MouseEvent based on the provided model.
  // Uses the provided |type| and |flags| for the new event.
  // If source / target windows are provided, the model location will be
  // converted from |source| coordinate system to |target| coordinate system.
  template <class T>
  MouseEvent(const MouseEvent& model, T* source, T* target)
      : LocatedEvent(model, source, target),
        changed_button_flags_(model.changed_button_flags_) {
  }

  template <class T>
  MouseEvent(const MouseEvent& model,
             T* source,
             T* target,
             EventType type,
             int flags)
      : LocatedEvent(model, source, target),
        changed_button_flags_(model.changed_button_flags_) {
    SetType(type);
    set_flags(flags);
  }

  // Used for synthetic events in testing and by the gesture recognizer.
  MouseEvent(EventType type,
             const gfx::Point& location,
             const gfx::Point& root_location,
             int flags);

  // Conveniences to quickly test what button is down
  bool IsOnlyLeftMouseButton() const {
    return (flags() & EF_LEFT_MOUSE_BUTTON) &&
      !(flags() & (EF_MIDDLE_MOUSE_BUTTON | EF_RIGHT_MOUSE_BUTTON));
  }

  bool IsLeftMouseButton() const {
    return (flags() & EF_LEFT_MOUSE_BUTTON) != 0;
  }

  bool IsOnlyMiddleMouseButton() const {
    return (flags() & EF_MIDDLE_MOUSE_BUTTON) &&
      !(flags() & (EF_LEFT_MOUSE_BUTTON | EF_RIGHT_MOUSE_BUTTON));
  }

  bool IsMiddleMouseButton() const {
    return (flags() & EF_MIDDLE_MOUSE_BUTTON) != 0;
  }

  bool IsOnlyRightMouseButton() const {
    return (flags() & EF_RIGHT_MOUSE_BUTTON) &&
      !(flags() & (EF_LEFT_MOUSE_BUTTON | EF_MIDDLE_MOUSE_BUTTON));
  }

  bool IsRightMouseButton() const {
    return (flags() & EF_RIGHT_MOUSE_BUTTON) != 0;
  }

  bool IsAnyButton() const {
    return (flags() & (EF_LEFT_MOUSE_BUTTON | EF_MIDDLE_MOUSE_BUTTON |
                       EF_RIGHT_MOUSE_BUTTON)) != 0;
  }

  // Compares two mouse down events and returns true if the second one should
  // be considered a repeat of the first.
  static bool IsRepeatedClickEvent(
      const MouseEvent& event1,
      const MouseEvent& event2);

  // Get the click count. Can be 1, 2 or 3 for mousedown messages, 0 otherwise.
  int GetClickCount() const;

  // Set the click count for a mousedown message. Can be 1, 2 or 3.
  void SetClickCount(int click_count);

  // Identifies the button that changed. During a press this corresponds to the
  // button that was pressed and during a release this corresponds to the button
  // that was released.
  // NOTE: during a press and release flags() contains the complete set of
  // flags. Use this to determine the button that was pressed or released.
  int changed_button_flags() const { return changed_button_flags_; }

 private:
  // Returns the repeat count based on the previous mouse click, if it is
  // recent enough and within a small enough distance.
  static int GetRepeatCount(const MouseEvent& click_event);

  gfx::Point root_location_;

  // See description above getter for details.
  int changed_button_flags_;

  static MouseEvent* last_click_event_;
};

class ScrollEvent;

class UI_EXPORT MouseWheelEvent : public MouseEvent {
 public:
  // See |offset| for details.
  static const int kWheelDelta;

  explicit MouseWheelEvent(const base::NativeEvent& native_event);
  explicit MouseWheelEvent(const ScrollEvent& scroll_event);
  MouseWheelEvent(const MouseEvent& mouse_event, int x_offset, int y_offset);

  template <class T>
  MouseWheelEvent(const MouseWheelEvent& model,
                  T* source,
                  T* target,
                  EventType type,
                  int flags)
      : MouseEvent(model, source, target, type, flags),
        offset_(model.x_offset(), model.y_offset()){
  }

  // The amount to scroll. This is in multiples of kWheelDelta.
  // Note: x_offset() > 0/y_offset() > 0 means scroll left/up.
  int x_offset() const { return offset_.x(); }
  int y_offset() const { return offset_.y(); }

 private:
  gfx::Vector2d offset_;

  DISALLOW_COPY_AND_ASSIGN(MouseWheelEvent);
};

class UI_EXPORT TouchEvent : public LocatedEvent {
 public:
  explicit TouchEvent(const base::NativeEvent& native_event);

  // Create a new TouchEvent which is identical to the provided model.
  // If source / target windows are provided, the model location will be
  // converted from |source| coordinate system to |target| coordinate system.
  template <class T>
  TouchEvent(const TouchEvent& model, T* source, T* target)
      : LocatedEvent(model, source, target),
        touch_id_(model.touch_id_),
        radius_x_(model.radius_x_),
        radius_y_(model.radius_y_),
        rotation_angle_(model.rotation_angle_),
        force_(model.force_) {
  }

  TouchEvent(EventType type,
             const gfx::Point& root_location,
             int touch_id,
             base::TimeDelta time_stamp);

  TouchEvent(EventType type,
             const gfx::Point& location,
             int flags,
             int touch_id,
             base::TimeDelta timestamp,
             float radius_x,
             float radius_y,
             float angle,
             float force);

  virtual ~TouchEvent();

  int touch_id() const { return touch_id_; }
  float radius_x() const { return radius_x_; }
  float radius_y() const { return radius_y_; }
  float rotation_angle() const { return rotation_angle_; }
  float force() const { return force_; }

  // Relocate the touch-point to a new |origin|.
  // This is useful when touch event is in X Root Window coordinates,
  // and it needs to be mapped into Aura Root Window coordinates.
  void Relocate(const gfx::Point& origin);

  // Used for unit tests.
  void set_radius_x(const float r) { radius_x_ = r; }
  void set_radius_y(const float r) { radius_y_ = r; }

  // Overridden from LocatedEvent.
  virtual void UpdateForRootTransform(
      const gfx::Transform& inverted_root_transform) OVERRIDE;

 protected:
  void set_radius(float radius_x, float radius_y) {
    radius_x_ = radius_x;
    radius_y_ = radius_y;
  }

  void set_rotation_angle(float rotation_angle) {
    rotation_angle_ = rotation_angle;
  }

  void set_force(float force) { force_ = force; }

 private:
  // The identity (typically finger) of the touch starting at 0 and incrementing
  // for each separable additional touch that the hardware can detect.
  const int touch_id_;

  // Radius of the X (major) axis of the touch ellipse. 0.0 if unknown.
  float radius_x_;

  // Radius of the Y (minor) axis of the touch ellipse. 0.0 if unknown.
  float radius_y_;

  // Angle of the major axis away from the X axis. Default 0.0.
  float rotation_angle_;

  // Force (pressure) of the touch. Normalized to be [0, 1]. Default to be 0.0.
  float force_;
};

class UI_EXPORT KeyEvent : public Event {
 public:
  KeyEvent(const base::NativeEvent& native_event, bool is_char);

  // Used for synthetic events.
  KeyEvent(EventType type, KeyboardCode key_code, int flags, bool is_char);

  // These setters allow an I18N virtual keyboard to fabricate a keyboard event
  // which does not have a corresponding KeyboardCode (example: U+00E1 Latin
  // small letter A with acute, U+0410 Cyrillic capital letter A.)
  // GetCharacter() and GetUnmodifiedCharacter() return the character.
  void set_character(uint16 character) { character_ = character; }
  void set_unmodified_character(uint16 unmodified_character) {
    unmodified_character_ = unmodified_character;
  }

  // Gets the character generated by this key event. It only supports Unicode
  // BMP characters.
  uint16 GetCharacter() const;

  // Gets the character generated by this key event ignoring concurrently-held
  // modifiers (except shift).
  uint16 GetUnmodifiedCharacter() const;

  // Returns the copy of this key event. Used in NativeWebKeyboardEvent.
  KeyEvent* Copy() const;

  KeyboardCode key_code() const { return key_code_; }
  bool is_char() const { return is_char_; }

  // This is only intended to be used externally by classes that are modifying
  // events in EventFilter::PreHandleKeyEvent().  set_character() should also be
  // called.
  void set_key_code(KeyboardCode key_code) { key_code_ = key_code; }

  // Returns true for [Alt]+<num-pad digit> Unicode alt key codes used by Win.
  // TODO(msw): Additional work may be needed for analogues on other platforms.
  bool IsUnicodeKeyCode() const;

  // Normalizes flags_ to make it Windows/Mac compatible. Since the way
  // of setting modifier mask on X is very different than Windows/Mac as shown
  // in http://crbug.com/127142#c8, the normalization is necessary.
  void NormalizeFlags();

 private:
  KeyboardCode key_code_;
  // True if this is a translated character event (vs. a raw key down). Both
  // share the same type: ET_KEY_PRESSED.
  bool is_char_;

  uint16 character_;
  uint16 unmodified_character_;
};

// A key event which is translated by an input method (IME).
// For example, if an IME receives a KeyEvent(VKEY_SPACE), and it does not
// consume the key, the IME usually generates and dispatches a
// TranslatedKeyEvent(VKEY_SPACE) event. If the IME receives a KeyEvent and
// it does consume the event, it might dispatch a
// TranslatedKeyEvent(VKEY_PROCESSKEY) event as defined in the DOM spec.
class UI_EXPORT TranslatedKeyEvent : public KeyEvent {
 public:
  TranslatedKeyEvent(const base::NativeEvent& native_event, bool is_char);

  // Used for synthetic events such as a VKEY_PROCESSKEY key event.
  TranslatedKeyEvent(bool is_press, KeyboardCode key_code, int flags);

  // Changes the type() of the object from ET_TRANSLATED_KEY_* to ET_KEY_* so
  // that RenderWidgetHostViewAura and NativeWidgetAura could handle the event.
  void ConvertToKeyEvent();

 private:
  DISALLOW_COPY_AND_ASSIGN(TranslatedKeyEvent);
};

class UI_EXPORT DropTargetEvent : public LocatedEvent {
 public:
  DropTargetEvent(const OSExchangeData& data,
                  const gfx::Point& location,
                  const gfx::Point& root_location,
                  int source_operations);

  const OSExchangeData& data() const { return data_; }
  int source_operations() const { return source_operations_; }

 private:
  // Data associated with the drag/drop session.
  const OSExchangeData& data_;

  // Bitmask of supported DragDropTypes::DragOperation by the source.
  int source_operations_;

  DISALLOW_COPY_AND_ASSIGN(DropTargetEvent);
};

class UI_EXPORT ScrollEvent : public MouseEvent {
 public:
  explicit ScrollEvent(const base::NativeEvent& native_event);
  template <class T>
  ScrollEvent(const ScrollEvent& model,
              T* source,
              T* target)
      : MouseEvent(model, source, target),
        x_offset_(model.x_offset_),
        y_offset_(model.y_offset_),
        x_offset_ordinal_(model.x_offset_ordinal_),
        y_offset_ordinal_(model.y_offset_ordinal_),
        finger_count_(model.finger_count_){
  }

  // Used for tests.
  ScrollEvent(EventType type,
              const gfx::Point& location,
              base::TimeDelta time_stamp,
              int flags,
              float x_offset,
              float y_offset,
              float x_offset_ordinal,
              float y_offset_ordinal,
              int finger_count);

  // Scale the scroll event's offset value.
  // This is useful in the multi-monitor setup where it needs to be scaled
  // to provide a consistent user experience.
  void Scale(const float factor);

  float x_offset() const { return x_offset_; }
  float y_offset() const { return y_offset_; }
  float x_offset_ordinal() const { return x_offset_ordinal_; }
  float y_offset_ordinal() const { return y_offset_ordinal_; }
  int finger_count() const { return finger_count_; }

  // Overridden from LocatedEvent.
  virtual void UpdateForRootTransform(
      const gfx::Transform& inverted_root_transform) OVERRIDE;

 private:
  // Potential accelerated offsets.
  float x_offset_;
  float y_offset_;
  // Unaccelerated offsets.
  float x_offset_ordinal_;
  float y_offset_ordinal_;
  // Number of fingers on the pad.
  int finger_count_;
};

class UI_EXPORT GestureEvent : public LocatedEvent {
 public:
  GestureEvent(EventType type,
               int x,
               int y,
               int flags,
               base::TimeDelta time_stamp,
               const GestureEventDetails& details,
               unsigned int touch_ids_bitfield);

  // Create a new GestureEvent which is identical to the provided model.
  // If source / target windows are provided, the model location will be
  // converted from |source| coordinate system to |target| coordinate system.
  template <typename T>
  GestureEvent(const GestureEvent& model, T* source, T* target)
      : LocatedEvent(model, source, target),
        details_(model.details_),
        touch_ids_bitfield_(model.touch_ids_bitfield_) {
  }

  virtual ~GestureEvent();

  const GestureEventDetails& details() const { return details_; }

  // Returns the lowest touch-id of any of the touches which make up this
  // gesture. If there are no touches associated with this gesture, returns -1.
  int GetLowestTouchId() const;

 private:
  GestureEventDetails details_;

  // The set of indices of ones in the binary representation of
  // touch_ids_bitfield_ is the set of touch_ids associate with this gesture.
  // This value is stored as a bitfield because the number of touch ids varies,
  // but we currently don't need more than 32 touches at a time.
  const unsigned int touch_ids_bitfield_;

  DISALLOW_COPY_AND_ASSIGN(GestureEvent);
};

}  // namespace ui

#endif  // UI_BASE_EVENTS_EVENT_H_
