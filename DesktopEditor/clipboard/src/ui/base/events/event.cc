// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/events/event.h"

#if defined(USE_X11)
#include <X11/Xlib.h>
#endif

#include <cmath>
#include <cstring>

#include "base/metrics/histogram.h"
#include "base/stringprintf.h"
#include "ui/base/events/event_utils.h"
#include "ui/base/keycodes/keyboard_code_conversion.h"
#include "ui/gfx/point3_f.h"
#include "ui/gfx/point_conversions.h"
#include "ui/gfx/transform.h"
#include "ui/gfx/transform_util.h"

#if defined(USE_X11)
#include "ui/base/keycodes/keyboard_code_conversion_x.h"
#endif

namespace {

base::NativeEvent CopyNativeEvent(const base::NativeEvent& event) {
#if defined(USE_X11)
  if (!event || event->type == GenericEvent)
    return NULL;
  XEvent* copy = new XEvent;
  *copy = *event;
  return copy;
#elif defined(OS_WIN)
  return event;
#else
  NOTREACHED() <<
      "Don't know how to copy base::NativeEvent for this platform";
  return NULL;
#endif
}

gfx::Point CalibratePoint(const gfx::Point& point,
                          const gfx::Size& from,
                          const gfx::Size& to) {
  float calibrated_x =
      static_cast<float>(point.x()) * to.width() / from.width();
  float calibrated_y =
      static_cast<float>(point.y()) * to.height() / from.height();
  return gfx::Point(static_cast<int>(floorf(calibrated_x + 0.5f)),
                    static_cast<int>(floorf(calibrated_y + 0.5f)));
}

std::string EventTypeName(ui::EventType type) {
#define RETURN_IF_TYPE(t) if (type == ui::t)  return #t
#define CASE_TYPE(t) case ui::t:  return #t
  switch (type) {
    CASE_TYPE(ET_UNKNOWN);
    CASE_TYPE(ET_MOUSE_PRESSED);
    CASE_TYPE(ET_MOUSE_DRAGGED);
    CASE_TYPE(ET_MOUSE_RELEASED);
    CASE_TYPE(ET_MOUSE_MOVED);
    CASE_TYPE(ET_MOUSE_ENTERED);
    CASE_TYPE(ET_MOUSE_EXITED);
    CASE_TYPE(ET_KEY_PRESSED);
    CASE_TYPE(ET_KEY_RELEASED);
    CASE_TYPE(ET_MOUSEWHEEL);
    CASE_TYPE(ET_MOUSE_CAPTURE_CHANGED);
    CASE_TYPE(ET_TOUCH_RELEASED);
    CASE_TYPE(ET_TOUCH_PRESSED);
    CASE_TYPE(ET_TOUCH_MOVED);
    CASE_TYPE(ET_TOUCH_STATIONARY);
    CASE_TYPE(ET_TOUCH_CANCELLED);
    CASE_TYPE(ET_DROP_TARGET_EVENT);
    CASE_TYPE(ET_TRANSLATED_KEY_PRESS);
    CASE_TYPE(ET_TRANSLATED_KEY_RELEASE);
    CASE_TYPE(ET_GESTURE_SCROLL_BEGIN);
    CASE_TYPE(ET_GESTURE_SCROLL_END);
    CASE_TYPE(ET_GESTURE_SCROLL_UPDATE);
    CASE_TYPE(ET_GESTURE_TAP);
    CASE_TYPE(ET_GESTURE_TAP_DOWN);
    CASE_TYPE(ET_GESTURE_TAP_CANCEL);
    CASE_TYPE(ET_GESTURE_BEGIN);
    CASE_TYPE(ET_GESTURE_END);
    CASE_TYPE(ET_GESTURE_TWO_FINGER_TAP);
    CASE_TYPE(ET_GESTURE_PINCH_BEGIN);
    CASE_TYPE(ET_GESTURE_PINCH_END);
    CASE_TYPE(ET_GESTURE_PINCH_UPDATE);
    CASE_TYPE(ET_GESTURE_LONG_PRESS);
    CASE_TYPE(ET_GESTURE_LONG_TAP);
    CASE_TYPE(ET_GESTURE_MULTIFINGER_SWIPE);
    CASE_TYPE(ET_SCROLL);
    CASE_TYPE(ET_SCROLL_FLING_START);
    CASE_TYPE(ET_SCROLL_FLING_CANCEL);
    CASE_TYPE(ET_CANCEL_MODE);
    case ui::ET_LAST: NOTREACHED(); return std::string();
    // Don't include default, so that we get an error when new type is added.
  }
#undef CASE_TYPE

  NOTREACHED();
  return std::string();
}

bool IsX11SendEventTrue(const base::NativeEvent& event) {
#if defined(USE_X11)
  if (event && event->xany.send_event)
    return true;
#endif
  return false;
}

}  // namespace

namespace ui {

////////////////////////////////////////////////////////////////////////////////
// Event

Event::~Event() {
#if defined(USE_X11)
  if (delete_native_event_)
    delete native_event_;
#endif
}

bool Event::HasNativeEvent() const {
  base::NativeEvent null_event;
  std::memset(&null_event, 0, sizeof(null_event));
  return !!std::memcmp(&native_event_, &null_event, sizeof(null_event));
}

void Event::StopPropagation() {
  // TODO(sad): Re-enable these checks once View uses dispatcher to dispatch
  // events.
  // CHECK(phase_ != EP_PREDISPATCH && phase_ != EP_POSTDISPATCH);
  CHECK(cancelable_);
  result_ = static_cast<EventResult>(result_ | ER_CONSUMED);
}

void Event::SetHandled() {
  // TODO(sad): Re-enable these checks once View uses dispatcher to dispatch
  // events.
  // CHECK(phase_ != EP_PREDISPATCH && phase_ != EP_POSTDISPATCH);
  CHECK(cancelable_);
  result_ = static_cast<EventResult>(result_ | ER_HANDLED);
}

Event::Event(EventType type, base::TimeDelta time_stamp, int flags)
    : type_(type),
      time_stamp_(time_stamp),
      flags_(flags),
      dispatch_to_hidden_targets_(false),
#if defined(USE_X11)
      native_event_(NULL),
#endif
      delete_native_event_(false),
      cancelable_(true),
      target_(NULL),
      phase_(EP_PREDISPATCH),
      result_(ER_UNHANDLED) {
  if (type_ < ET_LAST)
    name_ = EventTypeName(type_);
  Init();
}

Event::Event(const base::NativeEvent& native_event,
             EventType type,
             int flags)
    : type_(type),
      time_stamp_(EventTimeFromNative(native_event)),
      flags_(flags),
      dispatch_to_hidden_targets_(false),
      delete_native_event_(false),
      cancelable_(true),
      target_(NULL),
      phase_(EP_PREDISPATCH),
      result_(ER_UNHANDLED) {
  base::TimeDelta delta = EventTimeForNow() - time_stamp_;
  if (type_ < ET_LAST)
    name_ = EventTypeName(type_);
  UMA_HISTOGRAM_CUSTOM_COUNTS("Event.Latency.Browser",
                              delta.InMicroseconds(), 0, 1000000, 100);
  std::string name_for_event =
      base::StringPrintf("Event.Latency.Browser.%s", name_.c_str());
  base::HistogramBase* counter_for_type =
      base::Histogram::FactoryGet(
          name_for_event,
          0,
          1000000,
          100,
          base::HistogramBase::kUmaTargetedHistogramFlag);
  counter_for_type->Add(delta.InMicroseconds());
  InitWithNativeEvent(native_event);
}

Event::Event(const Event& copy)
    : type_(copy.type_),
      time_stamp_(copy.time_stamp_),
      flags_(copy.flags_),
      dispatch_to_hidden_targets_(false),
      native_event_(::CopyNativeEvent(copy.native_event_)),
      delete_native_event_(false),
      cancelable_(true),
      target_(NULL),
      phase_(EP_PREDISPATCH),
      result_(ER_UNHANDLED) {
  if (type_ < ET_LAST)
    name_ = EventTypeName(type_);
#if defined(USE_X11)
  if (native_event_)
    delete_native_event_ = true;
#endif
}

void Event::SetType(EventType type) {
  if (type_ < ET_LAST)
    name_ = std::string();
  type_ = type;
  if (type_ < ET_LAST)
    name_ = EventTypeName(type_);
}

void Event::Init() {
  std::memset(&native_event_, 0, sizeof(native_event_));
}

void Event::InitWithNativeEvent(const base::NativeEvent& native_event) {
  native_event_ = native_event;
}

////////////////////////////////////////////////////////////////////////////////
// CancelModeEvent

CancelModeEvent::CancelModeEvent()
    : Event(ET_CANCEL_MODE, base::TimeDelta(), 0) {
  set_cancelable(false);
}

CancelModeEvent::~CancelModeEvent() {
}

////////////////////////////////////////////////////////////////////////////////
// LocatedEvent

LocatedEvent::~LocatedEvent() {
}

LocatedEvent::LocatedEvent(const base::NativeEvent& native_event)
    : Event(native_event,
            EventTypeFromNative(native_event),
            EventFlagsFromNative(native_event)),
      location_(EventLocationFromNative(native_event)),
      root_location_(location_) {
}

LocatedEvent::LocatedEvent(EventType type,
                           const gfx::Point& location,
                           const gfx::Point& root_location,
                           base::TimeDelta time_stamp,
                           int flags)
    : Event(type, time_stamp, flags),
      location_(location),
      root_location_(root_location) {
}

void LocatedEvent::UpdateForRootTransform(
    const gfx::Transform& reversed_root_transform) {
  // Transform has to be done at root level.
  gfx::Point3F p(location_);
  reversed_root_transform.TransformPoint(p);
  root_location_ = location_ = gfx::ToFlooredPoint(p.AsPointF());
}

////////////////////////////////////////////////////////////////////////////////
// MouseEvent

MouseEvent::MouseEvent(const base::NativeEvent& native_event)
    : LocatedEvent(native_event),
      changed_button_flags_(
          GetChangedMouseButtonFlagsFromNative(native_event)) {
  if (type() == ET_MOUSE_PRESSED)
    SetClickCount(GetRepeatCount(*this));
}

MouseEvent::MouseEvent(EventType type,
                       const gfx::Point& location,
                       const gfx::Point& root_location,
                       int flags)
    : LocatedEvent(type, location, root_location, EventTimeForNow(), flags),
      changed_button_flags_(0) {
  if (this->type() == ET_MOUSE_MOVED && IsAnyButton())
    SetType(ET_MOUSE_DRAGGED);
}

// static
bool MouseEvent::IsRepeatedClickEvent(
    const MouseEvent& event1,
    const MouseEvent& event2) {
  // These values match the Windows defaults.
  static const int kDoubleClickTimeMS = 500;
  static const int kDoubleClickWidth = 4;
  static const int kDoubleClickHeight = 4;

  if (event1.type() != ET_MOUSE_PRESSED ||
      event2.type() != ET_MOUSE_PRESSED)
    return false;

  // Compare flags, but ignore EF_IS_DOUBLE_CLICK to allow triple clicks.
  if ((event1.flags() & ~EF_IS_DOUBLE_CLICK) !=
      (event2.flags() & ~EF_IS_DOUBLE_CLICK))
    return false;

  base::TimeDelta time_difference = event2.time_stamp() - event1.time_stamp();

  if (time_difference.InMilliseconds() > kDoubleClickTimeMS)
    return false;

  if (abs(event2.x() - event1.x()) > kDoubleClickWidth / 2)
    return false;

  if (abs(event2.y() - event1.y()) > kDoubleClickHeight / 2)
    return false;

  return true;
}

// static
int MouseEvent::GetRepeatCount(const MouseEvent& event) {
  int click_count = 1;
  if (last_click_event_) {
    if (IsX11SendEventTrue(event.native_event()))
      click_count = last_click_event_->GetClickCount();
    else if (IsRepeatedClickEvent(*last_click_event_, event))
      click_count = last_click_event_->GetClickCount() + 1;
    delete last_click_event_;
  }
  last_click_event_ = new MouseEvent(event);
  if (click_count > 3)
    click_count = 3;
  last_click_event_->SetClickCount(click_count);
  return click_count;
}

// static
MouseEvent* MouseEvent::last_click_event_ = NULL;

int MouseEvent::GetClickCount() const {
  if (type() != ET_MOUSE_PRESSED)
    return 0;

  if (flags() & EF_IS_TRIPLE_CLICK)
    return 3;
  else if (flags() & EF_IS_DOUBLE_CLICK)
    return 2;
  else
    return 1;
}

void MouseEvent::SetClickCount(int click_count) {
  if (type() != ET_MOUSE_PRESSED)
    return;

  DCHECK(click_count > 0);
  DCHECK(click_count <= 3);

  int f = flags();
  switch (click_count) {
    case 1:
      f &= ~EF_IS_DOUBLE_CLICK;
      f &= ~EF_IS_TRIPLE_CLICK;
      break;
    case 2:
      f |= EF_IS_DOUBLE_CLICK;
      f &= ~EF_IS_TRIPLE_CLICK;
      break;
    case 3:
      f &= ~EF_IS_DOUBLE_CLICK;
      f |= EF_IS_TRIPLE_CLICK;
      break;
  }
  set_flags(f);
}

////////////////////////////////////////////////////////////////////////////////
// MouseWheelEvent

MouseWheelEvent::MouseWheelEvent(const base::NativeEvent& native_event)
    : MouseEvent(native_event),
      offset_(GetMouseWheelOffset(native_event)) {
}

MouseWheelEvent::MouseWheelEvent(const ScrollEvent& scroll_event)
    : MouseEvent(scroll_event),
      offset_(scroll_event.x_offset(), scroll_event.y_offset()){
  SetType(ET_MOUSEWHEEL);
}

MouseWheelEvent::MouseWheelEvent(const MouseEvent& mouse_event,
                                 int x_offset,
                                 int y_offset)
    : MouseEvent(mouse_event), offset_(x_offset, y_offset) {
  DCHECK(type() == ET_MOUSEWHEEL);
}

#if defined(OS_WIN)
// This value matches windows WHEEL_DELTA.
// static
const int MouseWheelEvent::kWheelDelta = 120;
#else
// This value matches GTK+ wheel scroll amount.
const int MouseWheelEvent::kWheelDelta = 53;
#endif

////////////////////////////////////////////////////////////////////////////////
// TouchEvent

TouchEvent::TouchEvent(const base::NativeEvent& native_event)
    : LocatedEvent(native_event),
      touch_id_(GetTouchId(native_event)),
      radius_x_(GetTouchRadiusX(native_event)),
      radius_y_(GetTouchRadiusY(native_event)),
      rotation_angle_(GetTouchAngle(native_event)),
      force_(GetTouchForce(native_event)) {
}

TouchEvent::TouchEvent(EventType type,
                       const gfx::Point& location,
                       int touch_id,
                       base::TimeDelta time_stamp)
    : LocatedEvent(type, location, location, time_stamp, 0),
      touch_id_(touch_id),
      radius_x_(0.0f),
      radius_y_(0.0f),
      rotation_angle_(0.0f),
      force_(0.0f) {
}

TouchEvent::TouchEvent(EventType type,
                       const gfx::Point& location,
                       int flags,
                       int touch_id,
                       base::TimeDelta time_stamp,
                       float radius_x,
                       float radius_y,
                       float angle,
                       float force)
    : LocatedEvent(type, location, location, time_stamp, flags),
      touch_id_(touch_id),
      radius_x_(radius_x),
      radius_y_(radius_y),
      rotation_angle_(angle),
      force_(force) {
}

TouchEvent::~TouchEvent() {
}

void TouchEvent::Relocate(const gfx::Point& origin) {
  location_ -= origin.OffsetFromOrigin();
  root_location_ -= origin.OffsetFromOrigin();
}

void TouchEvent::UpdateForRootTransform(
    const gfx::Transform& inverted_root_transform) {
  LocatedEvent::UpdateForRootTransform(inverted_root_transform);
  gfx::DecomposedTransform decomp;
  bool success = gfx::DecomposeTransform(&decomp, inverted_root_transform);
  DCHECK(success);
  if (decomp.scale[0])
    radius_x_ *= decomp.scale[0];
  if (decomp.scale[1])
    radius_y_ *= decomp.scale[1];
}

////////////////////////////////////////////////////////////////////////////////
// KeyEvent

KeyEvent::KeyEvent(const base::NativeEvent& native_event, bool is_char)
    : Event(native_event,
            EventTypeFromNative(native_event),
            EventFlagsFromNative(native_event)),
      key_code_(KeyboardCodeFromNative(native_event)),
      is_char_(is_char),
      character_(0),
      unmodified_character_(0) {
#if defined(USE_X11)
  NormalizeFlags();
#endif
}

KeyEvent::KeyEvent(EventType type,
                   KeyboardCode key_code,
                   int flags,
                   bool is_char)
    : Event(type, EventTimeForNow(), flags),
      key_code_(key_code),
      is_char_(is_char),
      character_(GetCharacterFromKeyCode(key_code, flags)),
      unmodified_character_(0) {
}

uint16 KeyEvent::GetCharacter() const {
  if (character_)
    return character_;

#if defined(OS_WIN)
  return (native_event().message == WM_CHAR) ? key_code_ :
      GetCharacterFromKeyCode(key_code_, flags());
#elif defined(USE_X11)
  if (!native_event())
    return GetCharacterFromKeyCode(key_code_, flags());

  DCHECK(native_event()->type == KeyPress ||
         native_event()->type == KeyRelease);

  uint16 ch = 0;
  if (!IsControlDown())
    ch = GetCharacterFromXEvent(native_event());
  return ch ? ch : GetCharacterFromKeyCode(key_code_, flags());
#else
  NOTIMPLEMENTED();
  return 0;
#endif
}

uint16 KeyEvent::GetUnmodifiedCharacter() const {
  if (unmodified_character_)
    return unmodified_character_;

#if defined(OS_WIN)
  // Looks like there is no way to get unmodified character on Windows.
  return (native_event().message == WM_CHAR) ? key_code_ :
      GetCharacterFromKeyCode(key_code_, flags() & EF_SHIFT_DOWN);
#elif defined(USE_X11)
  if (!native_event())
    return GetCharacterFromKeyCode(key_code_, flags() & EF_SHIFT_DOWN);

  DCHECK(native_event()->type == KeyPress ||
         native_event()->type == KeyRelease);

  static const unsigned int kIgnoredModifiers = ControlMask | LockMask |
      Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | Mod5Mask;

  XKeyEvent copy = native_event()->xkey;  // bit-wise copy is safe.
  // We can't use things like (native_event()->xkey.state & ShiftMask), as it
  // may mask out bits used by X11 internally.
  copy.state &= ~kIgnoredModifiers;
  uint16 ch = GetCharacterFromXEvent(reinterpret_cast<XEvent*>(&copy));
  return ch ? ch : GetCharacterFromKeyCode(key_code_, flags() & EF_SHIFT_DOWN);
#else
  NOTIMPLEMENTED();
  return 0;
#endif
}

KeyEvent* KeyEvent::Copy() const {
  KeyEvent* copy = new KeyEvent(::CopyNativeEvent(native_event()), is_char());
#if defined(USE_X11)
  copy->set_delete_native_event(true);
#endif
  return copy;
}

bool KeyEvent::IsUnicodeKeyCode() const {
  if (!IsAltDown())
    return false;
  const int key = key_code();
  if (key >= VKEY_NUMPAD0 && key <= VKEY_NUMPAD9)
    return true;
  // Check whether the user is using the numeric keypad with num-lock off.
  // In that case, EF_EXTENDED will not be set; if it is set, the key event
  // originated from the relevant non-numpad dedicated key, e.g. [Insert].
  return (!(flags() & EF_EXTENDED) &&
          (key == VKEY_INSERT || key == VKEY_END  || key == VKEY_DOWN ||
           key == VKEY_NEXT   || key == VKEY_LEFT || key == VKEY_CLEAR ||
           key == VKEY_RIGHT  || key == VKEY_HOME || key == VKEY_UP ||
           key == VKEY_PRIOR));
}

void KeyEvent::NormalizeFlags() {
  int mask = 0;
  switch (key_code()) {
    case VKEY_CONTROL:
      mask = EF_CONTROL_DOWN;
      break;
    case VKEY_SHIFT:
      mask = EF_SHIFT_DOWN;
      break;
    case VKEY_MENU:
      mask = EF_ALT_DOWN;
      break;
    case VKEY_CAPITAL:
      mask = EF_CAPS_LOCK_DOWN;
      break;
    default:
      return;
  }
  if (type() == ET_KEY_PRESSED)
    set_flags(flags() | mask);
  else
    set_flags(flags() & ~mask);
}

////////////////////////////////////////////////////////////////////////////////
// TranslatedKeyEvent

TranslatedKeyEvent::TranslatedKeyEvent(const base::NativeEvent& native_event,
                                       bool is_char)
    : KeyEvent(native_event, is_char) {
  SetType(type() == ET_KEY_PRESSED ?
          ET_TRANSLATED_KEY_PRESS : ET_TRANSLATED_KEY_RELEASE);
}

TranslatedKeyEvent::TranslatedKeyEvent(bool is_press,
                                       KeyboardCode key_code,
                                       int flags)
    : KeyEvent((is_press ? ET_TRANSLATED_KEY_PRESS : ET_TRANSLATED_KEY_RELEASE),
               key_code,
               flags,
               false) {
}

void TranslatedKeyEvent::ConvertToKeyEvent() {
  SetType(type() == ET_TRANSLATED_KEY_PRESS ?
          ET_KEY_PRESSED : ET_KEY_RELEASED);
}

////////////////////////////////////////////////////////////////////////////////
// DropTargetEvent

DropTargetEvent::DropTargetEvent(const OSExchangeData& data,
                                 const gfx::Point& location,
                                 const gfx::Point& root_location,
                                 int source_operations)
    : LocatedEvent(ET_DROP_TARGET_EVENT,
                   location,
                   root_location,
                   EventTimeForNow(),
                   0),
      data_(data),
      source_operations_(source_operations) {
}

////////////////////////////////////////////////////////////////////////////////
// ScrollEvent

ScrollEvent::ScrollEvent(const base::NativeEvent& native_event)
    : MouseEvent(native_event) {
  if (type() == ET_SCROLL) {
    GetScrollOffsets(native_event,
                     &x_offset_, &y_offset_,
                     &x_offset_ordinal_, &y_offset_ordinal_,
                     &finger_count_);
  } else if (type() == ET_SCROLL_FLING_START ||
             type() == ET_SCROLL_FLING_CANCEL) {
    GetFlingData(native_event,
                 &x_offset_, &y_offset_,
                 &x_offset_ordinal_, &y_offset_ordinal_,
                 NULL);
  } else {
    NOTREACHED() << "Unexpected event type " << type()
        << " when constructing a ScrollEvent.";
  }
}

ScrollEvent::ScrollEvent(EventType type,
                         const gfx::Point& location,
                         base::TimeDelta time_stamp,
                         int flags,
                         float x_offset,
                         float y_offset,
                         float x_offset_ordinal,
                         float y_offset_ordinal,
                         int finger_count)
    : MouseEvent(type, location, location, flags),
      x_offset_(x_offset),
      y_offset_(y_offset),
      x_offset_ordinal_(x_offset_ordinal),
      y_offset_ordinal_(y_offset_ordinal),
      finger_count_(finger_count) {
  set_time_stamp(time_stamp);
  CHECK(IsScrollEvent());
}

void ScrollEvent::Scale(const float factor) {
  x_offset_ *= factor;
  y_offset_ *= factor;
  x_offset_ordinal_ *= factor;
  y_offset_ordinal_ *= factor;
}

void ScrollEvent::UpdateForRootTransform(
    const gfx::Transform& inverted_root_transform) {
  LocatedEvent::UpdateForRootTransform(inverted_root_transform);
  gfx::DecomposedTransform decomp;
  bool success = gfx::DecomposeTransform(&decomp, inverted_root_transform);
  DCHECK(success);
  if (decomp.scale[0])
    x_offset_ordinal_ *= decomp.scale[0];
  if (decomp.scale[1])
    y_offset_ordinal_ *= decomp.scale[1];
}

////////////////////////////////////////////////////////////////////////////////
// GestureEvent

GestureEvent::GestureEvent(EventType type,
                           int x,
                           int y,
                           int flags,
                           base::TimeDelta time_stamp,
                           const GestureEventDetails& details,
                           unsigned int touch_ids_bitfield)
    : LocatedEvent(type,
                   gfx::Point(x, y),
                   gfx::Point(x, y),
                   time_stamp,
                   flags | EF_FROM_TOUCH),
      details_(details),
      touch_ids_bitfield_(touch_ids_bitfield) {
}

GestureEvent::~GestureEvent() {
}

int GestureEvent::GetLowestTouchId() const {
  if (touch_ids_bitfield_ == 0)
    return -1;
  int i = -1;
  // Find the index of the least significant 1 bit
  while (!(1 << ++i & touch_ids_bitfield_));
  return i;
}

}  // namespace ui
