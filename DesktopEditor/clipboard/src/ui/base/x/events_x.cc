// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/events/event_constants.h"

#include <string.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>
#include <X11/Xlib.h>

#include "base/logging.h"
#include "base/memory/singleton.h"
#include "base/message_pump_aurax11.h"
#include "ui/base/events/event_utils.h"
#include "ui/base/keycodes/keyboard_code_conversion_x.h"
#include "ui/base/touch/touch_factory_x11.h"
#include "ui/base/x/device_list_cache_x.h"
#include "ui/base/x/valuators.h"
#include "ui/base/x/x11_atom_cache.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/display.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/screen.h"

// Copied from xserver-properties.h
#define AXIS_LABEL_PROP_REL_HWHEEL "Rel Horiz Wheel"
#define AXIS_LABEL_PROP_REL_WHEEL "Rel Vert Wheel"

// CMT specific timings
#define AXIS_LABEL_PROP_ABS_START_TIME "Abs Start Timestamp"
#define AXIS_LABEL_PROP_ABS_END_TIME "Abs End Timestamp"

// Ordinal values
#define AXIS_LABEL_PROP_ABS_DBL_ORDINAL_X   "Abs Dbl Ordinal X"
#define AXIS_LABEL_PROP_ABS_DBL_ORDINAL_Y   "Abs Dbl Ordinal Y"

// Fling properties
#define AXIS_LABEL_PROP_ABS_FLING_X       "Abs Fling X Velocity"
#define AXIS_LABEL_PROP_ABS_FLING_Y       "Abs Fling Y Velocity"
#define AXIS_LABEL_PROP_ABS_FLING_STATE   "Abs Fling State"

#define AXIS_LABEL_PROP_ABS_FINGER_COUNT   "Abs Finger Count"

// New versions of the valuators, with double values instead of fixed point.
#define AXIS_LABEL_PROP_ABS_DBL_START_TIME "Abs Dbl Start Timestamp"
#define AXIS_LABEL_PROP_ABS_DBL_END_TIME   "Abs Dbl End Timestamp"
#define AXIS_LABEL_PROP_ABS_DBL_FLING_VX   "Abs Dbl Fling X Velocity"
#define AXIS_LABEL_PROP_ABS_DBL_FLING_VY   "Abs Dbl Fling Y Velocity"

namespace {

// Scroll amount for each wheelscroll event. 53 is also the value used for GTK+.
const int kWheelScrollAmount = 53;

const int kMinWheelButton = 4;
const int kMaxWheelButton = 7;

const char* kCMTCachedAtoms[] = {
  AXIS_LABEL_PROP_ABS_DBL_ORDINAL_X,
  AXIS_LABEL_PROP_ABS_DBL_ORDINAL_Y,
  AXIS_LABEL_PROP_REL_HWHEEL,
  AXIS_LABEL_PROP_REL_WHEEL,
  AXIS_LABEL_PROP_ABS_START_TIME,
  AXIS_LABEL_PROP_ABS_DBL_START_TIME,
  AXIS_LABEL_PROP_ABS_END_TIME,
  AXIS_LABEL_PROP_ABS_DBL_END_TIME,
  AXIS_LABEL_PROP_ABS_FLING_X,
  AXIS_LABEL_PROP_ABS_FLING_Y,
  AXIS_LABEL_PROP_ABS_DBL_FLING_VX,
  AXIS_LABEL_PROP_ABS_DBL_FLING_VY,
  AXIS_LABEL_PROP_ABS_FLING_STATE,
  AXIS_LABEL_PROP_ABS_FINGER_COUNT,
  NULL
};

// A class to support the detection of scroll events, using X11 valuators.
class CMTEventData {
 public:
  // Returns the ScrollEventData singleton.
  static CMTEventData* GetInstance() {
    return Singleton<CMTEventData>::get();
  }

  // Updates the list of devices.
  void UpdateDeviceList(Display* display) {
    cmt_devices_.reset();
    touchpads_.reset();
    device_to_valuators_.clear();

#if defined(USE_XI2_MT)
    // Find all the touchpad devices.
    XDeviceList dev_list =
        ui::DeviceListCacheX::GetInstance()->GetXDeviceList(display);
    Atom xi_touchpad = XInternAtom(display, XI_TOUCHPAD, false);
    for (int i = 0; i < dev_list.count; ++i)
      if (dev_list[i].type == xi_touchpad)
        touchpads_[dev_list[i].id] = true;

    XIDeviceList info_list =
        ui::DeviceListCacheX::GetInstance()->GetXI2DeviceList(display);
    Atom x_axis = atom_cache_.GetAtom(AXIS_LABEL_PROP_REL_HWHEEL);
    Atom y_axis = atom_cache_.GetAtom(AXIS_LABEL_PROP_REL_WHEEL);
    Atom x_ordinal = atom_cache_.GetAtom(AXIS_LABEL_PROP_ABS_DBL_ORDINAL_X);
    Atom y_ordinal = atom_cache_.GetAtom(AXIS_LABEL_PROP_ABS_DBL_ORDINAL_Y);
    Atom start_time = atom_cache_.GetAtom(AXIS_LABEL_PROP_ABS_START_TIME);
    Atom start_time_dbl =
        atom_cache_.GetAtom(AXIS_LABEL_PROP_ABS_DBL_START_TIME);
    Atom end_time = atom_cache_.GetAtom(AXIS_LABEL_PROP_ABS_END_TIME);
    Atom end_time_dbl = atom_cache_.GetAtom(AXIS_LABEL_PROP_ABS_DBL_END_TIME);
    Atom fling_vx = atom_cache_.GetAtom(AXIS_LABEL_PROP_ABS_FLING_X);
    Atom fling_vx_dbl = atom_cache_.GetAtom(AXIS_LABEL_PROP_ABS_DBL_FLING_VX);
    Atom fling_vy = atom_cache_.GetAtom(AXIS_LABEL_PROP_ABS_FLING_Y);
    Atom fling_vy_dbl = atom_cache_.GetAtom(AXIS_LABEL_PROP_ABS_DBL_FLING_VY);
    Atom fling_state = atom_cache_.GetAtom(AXIS_LABEL_PROP_ABS_FLING_STATE);
    Atom finger_count = atom_cache_.GetAtom(AXIS_LABEL_PROP_ABS_FINGER_COUNT);

    for (int i = 0; i < info_list.count; ++i) {
      XIDeviceInfo* info = info_list.devices + i;

      if (info->use != XISlavePointer && info->use != XIFloatingSlave)
        continue;

      Valuators valuators;
      bool is_cmt = false;
      for (int j = 0; j < info->num_classes; ++j) {
        if (info->classes[j]->type == XIScrollClass) {
          is_cmt = false;
          break;
        }
        if (info->classes[j]->type != XIValuatorClass)
          continue;

        XIValuatorClassInfo* v =
            reinterpret_cast<XIValuatorClassInfo*>(info->classes[j]);
        int number = v->number;
        if (number > valuators.max)
          valuators.max = number;
        if (v->label == x_axis) {
          valuators.scroll_x = number;
          is_cmt = true;
        } else if (v->label == y_axis) {
          valuators.scroll_y = number;
          is_cmt = true;
        } else if (v->label == x_ordinal) {
          valuators.ordinal_x = number;
          is_cmt = true;
        } else if (v->label == y_ordinal) {
          valuators.ordinal_y = number;
          is_cmt = true;
        } else if (v->label == finger_count) {
          valuators.finger_count = number;
          is_cmt = true;
        } else if (v->label == start_time) {
          valuators.start_time = number;
          is_cmt = true;
        } else if (v->label == start_time_dbl) {
          valuators.start_time_dbl = number;
          is_cmt = true;
        } else if (v->label == end_time) {
          valuators.end_time = number;
          is_cmt = true;
        } else if (v->label == end_time_dbl) {
          valuators.end_time_dbl = number;
          is_cmt = true;
        } else if (v->label == fling_vx) {
          valuators.fling_vx = number;
          is_cmt = true;
        } else if (v->label == fling_vx_dbl) {
          valuators.fling_vx_dbl = number;
          is_cmt = true;
        } else if (v->label == fling_vy) {
          valuators.fling_vy = number;
          is_cmt = true;
        } else if (v->label == fling_vy_dbl) {
          valuators.fling_vy_dbl = number;
          is_cmt = true;
        } else if (v->label == fling_state) {
          valuators.fling_state = number;
          is_cmt = true;
        }
      }
      if (is_cmt) {
        // Double valuators override fixed point ones.
        if (valuators.start_time_dbl >= 0)
          valuators.start_time = -1;
        if (valuators.end_time_dbl >= 0)
          valuators.end_time = -1;
        if (valuators.fling_vx_dbl >= 0)
          valuators.fling_vx = -1;
        if (valuators.fling_vy_dbl >= 0)
          valuators.fling_vy = -1;
        device_to_valuators_[info->deviceid] = valuators;
        cmt_devices_[info->deviceid] = true;
      }
    }
#endif  // defined(USE_XI2_MT)
  }

  bool natural_scroll_enabled() const { return natural_scroll_enabled_; }
  void set_natural_scroll_enabled(bool enabled) {
    natural_scroll_enabled_ = enabled;
  }

  bool IsTouchpadXInputEvent(const base::NativeEvent& native_event) {
    if (native_event->type != GenericEvent)
      return false;

    XIDeviceEvent* xievent =
        static_cast<XIDeviceEvent*>(native_event->xcookie.data);
    return touchpads_[xievent->sourceid];
  }

  float GetNaturalScrollFactor(int sourceid) {
    // Natural scroll is touchpad-only.
    if (!touchpads_[sourceid])
      return -1.0f;

    return natural_scroll_enabled_ ? 1.0f : -1.0f;
  }

  // Returns true if this is a scroll event (a motion event with the necessary
  // valuators. Also returns the offsets. |x_offset| and |y_offset| can be
  // NULL.
  bool GetScrollOffsets(const XEvent& xev,
                        float* x_offset,
                        float* y_offset,
                        float* x_offset_ordinal,
                        float* y_offset_ordinal,
                        int* finger_count) {
    XIDeviceEvent* xiev = static_cast<XIDeviceEvent*>(xev.xcookie.data);

    if (x_offset)
      *x_offset = 0;
    if (y_offset)
      *y_offset = 0;
    if (x_offset_ordinal)
      *x_offset_ordinal = 0;
    if (y_offset_ordinal)
      *y_offset_ordinal = 0;
   if (finger_count)
      *finger_count = 2;

    const int sourceid = xiev->sourceid;
    if (!cmt_devices_[sourceid])
      return false;

    const float natural_scroll_factor = GetNaturalScrollFactor(sourceid);
    const Valuators v = device_to_valuators_[sourceid];
    const bool has_x_offset = XIMaskIsSet(xiev->valuators.mask, v.scroll_x);
    const bool has_y_offset = XIMaskIsSet(xiev->valuators.mask, v.scroll_y);
    const bool is_scroll = has_x_offset || has_y_offset;

    if (!is_scroll || (!x_offset && !y_offset))
      return is_scroll;

    double* valuators = xiev->valuators.values;
    for (int i = 0; i <= v.max; ++i) {
      if (XIMaskIsSet(xiev->valuators.mask, i)) {
        if (x_offset && v.scroll_x == i)
          *x_offset = *valuators * natural_scroll_factor;
        else if (y_offset && v.scroll_y == i)
          *y_offset = *valuators * natural_scroll_factor;
        else if (x_offset_ordinal && v.ordinal_x == i)
          *x_offset_ordinal = *valuators * natural_scroll_factor;
        else if (y_offset_ordinal && v.ordinal_y == i)
          *y_offset_ordinal = *valuators * natural_scroll_factor;
        else if (finger_count && v.finger_count == i)
          *finger_count = static_cast<int>(*valuators);
        valuators++;
      }
    }

    return true;
  }

  bool GetFlingData(const XEvent& xev,
                    float* vx, float* vy,
                    float* vx_ordinal, float* vy_ordinal,
                    bool* is_cancel) {
    XIDeviceEvent* xiev = static_cast<XIDeviceEvent*>(xev.xcookie.data);

    if (vx)
      *vx = 0;
    if (vy)
      *vy = 0;
    if (vx_ordinal)
      *vx_ordinal = 0;
    if (vy_ordinal)
      *vy_ordinal = 0;
    if (is_cancel)
      *is_cancel = false;

    const int sourceid = xiev->sourceid;
    if (!cmt_devices_[sourceid])
      return false;

    const float natural_scroll_factor = GetNaturalScrollFactor(sourceid);
    const Valuators v = device_to_valuators_[sourceid];
    if ((!XIMaskIsSet(xiev->valuators.mask, v.fling_vx) &&
            !XIMaskIsSet(xiev->valuators.mask, v.fling_vx_dbl)) ||
        (!XIMaskIsSet(xiev->valuators.mask, v.fling_vy) &&
            !XIMaskIsSet(xiev->valuators.mask, v.fling_vy_dbl)) ||
        !XIMaskIsSet(xiev->valuators.mask, v.fling_state))
      return false;

    double* valuators = xiev->valuators.values;
    for (int i = 0; i <= v.max; ++i) {
      if (XIMaskIsSet(xiev->valuators.mask, i)) {
        // Convert values to unsigned ints representing ms before storing them,
        // as that is how they were encoded before conversion to doubles.
        if (vx && v.fling_vx_dbl == i) {
          *vx = natural_scroll_factor * *valuators;
        } else if (vx && v.fling_vx == i) {
          *vx = natural_scroll_factor *
              static_cast<double>(static_cast<int>(*valuators)) / 1000.0f;
        } else if (vy && v.fling_vy_dbl == i) {
          *vy = natural_scroll_factor * *valuators;
        } else if (vy && v.fling_vy == i) {
          *vy = natural_scroll_factor *
              static_cast<double>(static_cast<int>(*valuators)) / 1000.0f;
        } else if (is_cancel && v.fling_state == i) {
          *is_cancel = !!static_cast<unsigned int>(*valuators);
        } else if (vx_ordinal && v.ordinal_x == i) {
          *vx_ordinal = *valuators * natural_scroll_factor;
        } else if (vy_ordinal && v.ordinal_y == i) {
          *vy_ordinal = *valuators * natural_scroll_factor;
        }
        valuators++;
      }
    }

    return true;
  }

  bool GetGestureTimes(const XEvent& xev,
                       double* start_time,
                       double* end_time) {
    *start_time = 0;
    *end_time = 0;

    XIDeviceEvent* xiev = static_cast<XIDeviceEvent*>(xev.xcookie.data);
    if (!cmt_devices_[xiev->sourceid])
      return false;

    Valuators v = device_to_valuators_[xiev->sourceid];
    if ((!XIMaskIsSet(xiev->valuators.mask, v.start_time) &&
            !XIMaskIsSet(xiev->valuators.mask, v.start_time_dbl)) ||
        (!XIMaskIsSet(xiev->valuators.mask, v.end_time) &&
            !XIMaskIsSet(xiev->valuators.mask, v.end_time_dbl)))
      return false;

    double* valuators = xiev->valuators.values;
    for (int i = 0; i <= v.max; ++i) {
      if (XIMaskIsSet(xiev->valuators.mask, i)) {
        if (v.start_time_dbl == i) {
          *start_time = *valuators;
        } else if (v.start_time == i) {
          // Convert values to unsigned ints representing ms before storing
          // them, as that is how they were encoded before conversion
          // to doubles.
          *start_time =
              static_cast<double>(
                  static_cast<unsigned int>(*valuators)) / 1000;
        } else if (v.end_time_dbl == i) {
          *end_time = *valuators;
        } else if (v.end_time == i) {
          // Convert values to unsigned ints representing ms before storing
          // them, as that is how they were encoded before conversion
          // to doubles.
          *end_time =
              static_cast<double>(
                  static_cast<unsigned int>(*valuators)) / 1000;
        }
        valuators++;
      }
    }

    return true;
  }

 private:
  // Requirement for Singleton
  friend struct DefaultSingletonTraits<CMTEventData>;

  struct Valuators {
    int max;
    int scroll_x;
    int scroll_y;
    int ordinal_x;
    int ordinal_y;
    int finger_count;
    int start_time;
    int end_time;
    int fling_vx;
    int fling_vy;
    int fling_state;
    // *_dbl valuators take precedence over the fixed precision versions.
    int start_time_dbl;
    int end_time_dbl;
    int fling_vx_dbl;
    int fling_vy_dbl;

    Valuators()
        : max(-1),
          scroll_x(-1),
          scroll_y(-1),
          ordinal_x(-1),
          ordinal_y(-1),
          finger_count(-1),
          start_time(-1),
          end_time(-1),
          fling_vx(-1),
          fling_vy(-1),
          fling_state(-1),
          start_time_dbl(-1),
          end_time_dbl(-1),
          fling_vx_dbl(-1),
          fling_vy_dbl(-1) {
    }

  };

  CMTEventData()
      : natural_scroll_enabled_(false),
        atom_cache_(ui::GetXDisplay(), kCMTCachedAtoms) {
    UpdateDeviceList(ui::GetXDisplay());
  }

  ~CMTEventData() {}

  // A quick lookup table for determining if events from the pointer device
  // should be processed.
  static const int kMaxDeviceNum = 128;
  bool natural_scroll_enabled_;
  std::bitset<kMaxDeviceNum> cmt_devices_;
  std::bitset<kMaxDeviceNum> touchpads_;
  std::map<int, Valuators> device_to_valuators_;
  ui::X11AtomCache atom_cache_;

  DISALLOW_COPY_AND_ASSIGN(CMTEventData);
};

// A class to track current modifier state on master device. Only track ctrl,
// alt, shift and caps lock keys currently. The tracked state can then be used
// by floating device.
class XModifierStateWatcher{
 public:
  static XModifierStateWatcher* GetInstance() {
    return Singleton<XModifierStateWatcher>::get();
  }

  void UpdateStateFromEvent(const base::NativeEvent& native_event) {
    // Floating device can't access the modifer state from master device.
    // We need to track the states of modifier keys in a singleton for
    // floating devices such as touch screen. Issue 106426 is one example
    // of why we need the modifier states for floating device.
    state_ = native_event->xkey.state;
    // master_state is the state before key press. We need to track the
    // state after key press for floating device. Currently only ctrl,
    // shift, alt and caps lock keys are tracked.
    ui::KeyboardCode keyboard_code = ui::KeyboardCodeFromNative(native_event);
    unsigned int mask = 0;

    switch (keyboard_code) {
      case ui::VKEY_CONTROL: {
        mask = ControlMask;
        break;
      }
      case ui::VKEY_SHIFT: {
        mask = ShiftMask;
        break;
      }
      case ui::VKEY_MENU: {
        mask = Mod1Mask;
        break;
      }
      case ui::VKEY_CAPITAL: {
        mask = LockMask;
        break;
      }
      default:
        break;
    }

    if (native_event->type == KeyPress)
      state_ |= mask;
    else
      state_ &= ~mask;
  }

  // Returns the current modifer state in master device. It only contains the
  // state of ctrl, shift, alt and caps lock keys.
  unsigned int state() { return state_; }

 private:
  friend struct DefaultSingletonTraits<XModifierStateWatcher>;

  XModifierStateWatcher() : state_(0) { }

  unsigned int state_;

  DISALLOW_COPY_AND_ASSIGN(XModifierStateWatcher);
};

#if defined(USE_XI2_MT)
// Detects if a touch event is a driver-generated 'special event'.
// A 'special event' is a touch release or move event with maximum radius and
// pressure at location (0, 0).
// This needs to be done in a cleaner way: http://crbug.com/169256
bool TouchEventIsGeneratedHack(const base::NativeEvent& native_event) {
  XIDeviceEvent* event =
      static_cast<XIDeviceEvent*>(native_event->xcookie.data);
  CHECK(event->evtype == XI_TouchUpdate ||
        event->evtype == XI_TouchEnd);

  // Force is normalized to [0, 1].
  if (ui::GetTouchForce(native_event) < 1.0f)
    return false;

  if (ui::EventLocationFromNative(native_event) != gfx::Point())
    return false;

  // Radius is in pixels, and the valuator is the diameter in pixels.
  float radius = ui::GetTouchRadiusX(native_event), min, max;
  unsigned int deviceid =
      static_cast<XIDeviceEvent*>(native_event->xcookie.data)->sourceid;
  if (!ui::ValuatorTracker::GetInstance()->GetValuatorRange(
      deviceid, ui::ValuatorTracker::VAL_TOUCH_MAJOR, &min, &max)) {
    return false;
  }

  return radius * 2 == max;
}
#endif

int GetEventFlagsFromXState(unsigned int state) {
  int flags = 0;
  if (state & ControlMask)
    flags |= ui::EF_CONTROL_DOWN;
  if (state & ShiftMask)
    flags |= ui::EF_SHIFT_DOWN;
  if (state & Mod1Mask)
    flags |= ui::EF_ALT_DOWN;
  if (state & LockMask)
    flags |= ui::EF_CAPS_LOCK_DOWN;
  if (state & Button1Mask)
    flags |= ui::EF_LEFT_MOUSE_BUTTON;
  if (state & Button2Mask)
    flags |= ui::EF_MIDDLE_MOUSE_BUTTON;
  if (state & Button3Mask)
    flags |= ui::EF_RIGHT_MOUSE_BUTTON;
  return flags;
}

// Get the event flag for the button in XButtonEvent. During a ButtonPress
// event, |state| in XButtonEvent does not include the button that has just been
// pressed. Instead |state| contains flags for the buttons (if any) that had
// already been pressed before the current button, and |button| stores the most
// current pressed button. So, if you press down left mouse button, and while
// pressing it down, press down the right mouse button, then for the latter
// event, |state| would have Button1Mask set but not Button3Mask, and |button|
// would be 3.
int GetEventFlagsForButton(int button) {
  switch (button) {
    case 1:
      return ui::EF_LEFT_MOUSE_BUTTON;
    case 2:
      return ui::EF_MIDDLE_MOUSE_BUTTON;
    case 3:
      return ui::EF_RIGHT_MOUSE_BUTTON;
    default:
      return 0;
  }
}

int GetButtonMaskForX2Event(XIDeviceEvent* xievent) {
  int buttonflags = 0;
  for (int i = 0; i < 8 * xievent->buttons.mask_len; i++) {
    if (XIMaskIsSet(xievent->buttons.mask, i)) {
      int button = (xievent->sourceid == xievent->deviceid) ?
                   ui::GetMappedButton(i) : i;
      buttonflags |= GetEventFlagsForButton(button);
    }
  }
  return buttonflags;
}

ui::EventType GetTouchEventType(const base::NativeEvent& native_event) {
  XIDeviceEvent* event =
      static_cast<XIDeviceEvent*>(native_event->xcookie.data);
#if defined(USE_XI2_MT)
  switch(event->evtype) {
    case XI_TouchBegin:
      return ui::ET_TOUCH_PRESSED;
    case XI_TouchUpdate:
      return TouchEventIsGeneratedHack(native_event) ? ui::ET_UNKNOWN :
                                                       ui::ET_TOUCH_MOVED;
    case XI_TouchEnd:
      return TouchEventIsGeneratedHack(native_event) ? ui::ET_TOUCH_CANCELLED :
                                                       ui::ET_TOUCH_RELEASED;
  }

  return ui::ET_UNKNOWN;
#else
  ui::TouchFactory* factory = ui::TouchFactory::GetInstance();

  // If this device doesn't support multi-touch, then just use the normal
  // pressed/release events to indicate touch start/end.  With multi-touch,
  // these events are sent only for the first (pressed) or last (released)
  // touch point, and so we must infer start/end from motion events.
  if (!factory->IsMultiTouchDevice(event->sourceid)) {
    switch (event->evtype) {
      case XI_ButtonPress:
        return ui::ET_TOUCH_PRESSED;
      case XI_ButtonRelease:
        return ui::ET_TOUCH_RELEASED;
      case XI_Motion:
        if (GetButtonMaskForX2Event(event))
          return ui::ET_TOUCH_MOVED;
        return ui::ET_UNKNOWN;
      default:
        NOTREACHED();
    }
  }

  DCHECK_EQ(event->evtype, XI_Motion);

  // Note: We will not generate a _STATIONARY event here. It will be created,
  // when necessary, by a RWHVV.
  // TODO(sad): When should _CANCELLED be generated?

  ui::ValuatorTracker* valuators = ui::ValuatorTracker::GetInstance();

  float slot;
  if (!valuators->ExtractValuator(
      *native_event, ui::ValuatorTracker::VAL_SLOT_ID, &slot))
    return ui::ET_UNKNOWN;

  if (!factory->IsSlotUsed(slot)) {
    // This is a new touch point.
    return ui::ET_TOUCH_PRESSED;
  }

  float tracking;
  if (!valuators->ExtractValuator(
      *native_event, ui::ValuatorTracker::VAL_TRACKING_ID, &tracking))
    return ui::ET_UNKNOWN;

  if (tracking == 0l) {
    // The touch point has been released.
    return ui::ET_TOUCH_RELEASED;
  }

  return ui::ET_TOUCH_MOVED;
#endif  // defined(USE_XI2_MT)
}

float GetTouchParamFromXEvent(XEvent* xev,
                              ui::ValuatorTracker::Valuator val,
                              float default_value) {
  ui::ValuatorTracker::GetInstance()->ExtractValuator(
      *xev, val, &default_value);
  return default_value;
}

Atom GetNoopEventAtom() {
  return XInternAtom(
      base::MessagePumpAuraX11::GetDefaultXDisplay(),
      "noop", False);
}

}  // namespace

namespace ui {

void UpdateDeviceList() {
  Display* display = GetXDisplay();
  DeviceListCacheX::GetInstance()->UpdateDeviceList(display);
  CMTEventData::GetInstance()->UpdateDeviceList(display);
  TouchFactory::GetInstance()->UpdateDeviceList(display);
  ValuatorTracker::GetInstance()->SetupValuator();
}

EventType EventTypeFromNative(const base::NativeEvent& native_event) {
  switch (native_event->type) {
    case KeyPress:
      return ET_KEY_PRESSED;
    case KeyRelease:
      return ET_KEY_RELEASED;
    case ButtonPress:
      if (static_cast<int>(native_event->xbutton.button) >= kMinWheelButton &&
          static_cast<int>(native_event->xbutton.button) <= kMaxWheelButton)
        return ET_MOUSEWHEEL;
      return ET_MOUSE_PRESSED;
    case ButtonRelease:
      // Drop wheel events; we should've already scrolled on the press.
      if (static_cast<int>(native_event->xbutton.button) >= kMinWheelButton &&
          static_cast<int>(native_event->xbutton.button) <= kMaxWheelButton)
        return ET_UNKNOWN;
      return ET_MOUSE_RELEASED;
    case MotionNotify:
      if (native_event->xmotion.state &
          (Button1Mask | Button2Mask | Button3Mask))
        return ET_MOUSE_DRAGGED;
      return ET_MOUSE_MOVED;
    case EnterNotify:
      return ET_MOUSE_ENTERED;
    case LeaveNotify:
      return ET_MOUSE_EXITED;
    case GenericEvent: {
      TouchFactory* factory = TouchFactory::GetInstance();
      if (!factory->ShouldProcessXI2Event(native_event))
        return ET_UNKNOWN;

      XIDeviceEvent* xievent =
          static_cast<XIDeviceEvent*>(native_event->xcookie.data);

      if (factory->IsTouchDevice(xievent->sourceid))
        return GetTouchEventType(native_event);

      switch (xievent->evtype) {
        case XI_ButtonPress: {
          int button = EventButtonFromNative(native_event);
          if (button >= kMinWheelButton && button <= kMaxWheelButton)
            return ET_MOUSEWHEEL;
          return ET_MOUSE_PRESSED;
        }
        case XI_ButtonRelease: {
          int button = EventButtonFromNative(native_event);
          // Drop wheel events; we should've already scrolled on the press.
          if (button >= kMinWheelButton && button <= kMaxWheelButton)
            return ET_UNKNOWN;
          return ET_MOUSE_RELEASED;
        }
        case XI_Motion: {
          bool is_cancel;
          if (GetFlingData(native_event, NULL, NULL, NULL, NULL, &is_cancel)) {
            return is_cancel ? ET_SCROLL_FLING_CANCEL : ET_SCROLL_FLING_START;
          } else if (GetScrollOffsets(
              native_event, NULL, NULL, NULL, NULL, NULL)) {
            return IsTouchpadEvent(native_event) ? ET_SCROLL : ET_MOUSEWHEEL;
          } else if (GetButtonMaskForX2Event(xievent)) {
            return ET_MOUSE_DRAGGED;
          } else {
            return ET_MOUSE_MOVED;
          }
        }
      }
    }
    default:
      break;
  }
  return ET_UNKNOWN;
}

int EventFlagsFromNative(const base::NativeEvent& native_event) {
  switch (native_event->type) {
    case KeyPress:
    case KeyRelease: {
      XModifierStateWatcher::GetInstance()->UpdateStateFromEvent(native_event);
      return GetEventFlagsFromXState(native_event->xkey.state);
    }
    case ButtonPress:
    case ButtonRelease: {
      int flags = GetEventFlagsFromXState(native_event->xbutton.state);
      const EventType type = EventTypeFromNative(native_event);
      if (type == ET_MOUSE_PRESSED || type == ET_MOUSE_RELEASED)
        flags |= GetEventFlagsForButton(native_event->xbutton.button);
      return flags;
    }
    case MotionNotify:
      return GetEventFlagsFromXState(native_event->xmotion.state);
    case GenericEvent: {
      XIDeviceEvent* xievent =
          static_cast<XIDeviceEvent*>(native_event->xcookie.data);

      switch (xievent->evtype) {
#if defined(USE_XI2_MT)
        case XI_TouchBegin:
        case XI_TouchUpdate:
        case XI_TouchEnd:
          return GetButtonMaskForX2Event(xievent) |
                 GetEventFlagsFromXState(xievent->mods.effective) |
                 GetEventFlagsFromXState(
                     XModifierStateWatcher::GetInstance()->state());
          break;
#endif
        case XI_ButtonPress:
        case XI_ButtonRelease: {
          const bool touch =
              TouchFactory::GetInstance()->IsTouchDevice(xievent->sourceid);
          int flags = GetButtonMaskForX2Event(xievent) |
              GetEventFlagsFromXState(xievent->mods.effective);
          if (touch) {
            flags |= GetEventFlagsFromXState(
                XModifierStateWatcher::GetInstance()->state());
          }

          const EventType type = EventTypeFromNative(native_event);
          int button = EventButtonFromNative(native_event);
          if ((type == ET_MOUSE_PRESSED || type == ET_MOUSE_RELEASED) && !touch)
            flags |= GetEventFlagsForButton(button);
          return flags;
        }
        case XI_Motion:
           return GetButtonMaskForX2Event(xievent) |
                  GetEventFlagsFromXState(xievent->mods.effective);
      }
    }
  }
  return 0;
}

base::TimeDelta EventTimeFromNative(const base::NativeEvent& native_event) {
  switch(native_event->type) {
    case KeyPress:
    case KeyRelease:
      return base::TimeDelta::FromMilliseconds(native_event->xkey.time);
    case ButtonPress:
    case ButtonRelease:
      return base::TimeDelta::FromMilliseconds(native_event->xbutton.time);
      break;
    case MotionNotify:
      return base::TimeDelta::FromMilliseconds(native_event->xmotion.time);
      break;
    case EnterNotify:
    case LeaveNotify:
      return base::TimeDelta::FromMilliseconds(native_event->xcrossing.time);
      break;
    case GenericEvent: {
      double start, end;
      float touch_timestamp;
      if (GetGestureTimes(native_event, &start, &end)) {
        // If the driver supports gesture times, use them.
        return base::TimeDelta::FromMicroseconds(end * 1000000);
      } else if (ValuatorTracker::GetInstance()->ExtractValuator(*native_event,
                 ValuatorTracker::VAL_TOUCH_RAW_TIMESTAMP, &touch_timestamp)) {
        return base::TimeDelta::FromMicroseconds(touch_timestamp * 1000000);
      } else {
        XIDeviceEvent* xide =
            static_cast<XIDeviceEvent*>(native_event->xcookie.data);
        return base::TimeDelta::FromMilliseconds(xide->time);
      }
      break;
    }
  }
  NOTREACHED();
  return base::TimeDelta();
}

gfx::Point EventLocationFromNative(const base::NativeEvent& native_event) {
  switch (native_event->type) {
    case EnterNotify:
    case LeaveNotify:
      return gfx::Point(native_event->xcrossing.x, native_event->xcrossing.y);
    case ButtonPress:
    case ButtonRelease:
      return gfx::Point(native_event->xbutton.x, native_event->xbutton.y);
    case MotionNotify:
      return gfx::Point(native_event->xmotion.x, native_event->xmotion.y);
    case GenericEvent: {
      XIDeviceEvent* xievent =
          static_cast<XIDeviceEvent*>(native_event->xcookie.data);

#if defined(USE_XI2_MT)
      // Touch event valuators aren't coordinates.
      // Return the |event_x|/|event_y| directly as event's position.
      if (xievent->evtype == XI_TouchBegin ||
          xievent->evtype == XI_TouchUpdate ||
          xievent->evtype == XI_TouchEnd)
        // Note: Touch events are always touch screen events.
        return gfx::Point(static_cast<int>(xievent->event_x),
                          static_cast<int>(xievent->event_y));
#endif
      // Read the position from the valuators, because the location reported in
      // event_x/event_y seems to be different (and doesn't match for events
      // coming from slave device and master device) from the values in the
      // valuators. See more on crbug.com/103981. The position in the valuators
      // is in the global screen coordinates. But it is necessary to convert it
      // into the window's coordinates. If the valuator is not set, that means
      // the value hasn't changed, and so we can use the value from
      // event_x/event_y (which are in the window's coordinates).
      double* valuators = xievent->valuators.values;

      double x = xievent->event_x;
      if (XIMaskIsSet(xievent->valuators.mask, 0))
        x = *valuators++ - (xievent->root_x - xievent->event_x);

      double y = xievent->event_y;
      if (XIMaskIsSet(xievent->valuators.mask, 1))
        y = *valuators++ - (xievent->root_y - xievent->event_y);

      return gfx::Point(static_cast<int>(x), static_cast<int>(y));
    }
  }
  return gfx::Point();
}

gfx::Point EventSystemLocationFromNative(
    const base::NativeEvent& native_event) {
  switch (native_event->type) {
    case EnterNotify:
    case LeaveNotify: {
      return gfx::Point(native_event->xcrossing.x_root,
                        native_event->xcrossing.y_root);
    }
    case ButtonPress:
    case ButtonRelease: {
      return gfx::Point(native_event->xbutton.x_root,
                        native_event->xbutton.y_root);
    }
    case MotionNotify: {
      return gfx::Point(native_event->xmotion.x_root,
                        native_event->xmotion.y_root);
    }
    case GenericEvent: {
      XIDeviceEvent* xievent =
          static_cast<XIDeviceEvent*>(native_event->xcookie.data);
      return gfx::Point(xievent->root_x, xievent->root_y);
    }
  }

  return gfx::Point();
}

int EventButtonFromNative(const base::NativeEvent& native_event) {
  CHECK_EQ(GenericEvent, native_event->type);
  XIDeviceEvent* xievent =
      static_cast<XIDeviceEvent*>(native_event->xcookie.data);
  int button = xievent->detail;

  return (xievent->sourceid == xievent->deviceid) ?
         ui::GetMappedButton(button) : button;
}

KeyboardCode KeyboardCodeFromNative(const base::NativeEvent& native_event) {
  return KeyboardCodeFromXKeyEvent(native_event);
}

bool IsMouseEvent(const base::NativeEvent& native_event) {
  if (native_event->type == EnterNotify ||
      native_event->type == LeaveNotify ||
      native_event->type == ButtonPress ||
      native_event->type == ButtonRelease ||
      native_event->type == MotionNotify)
    return true;
  if (native_event->type == GenericEvent) {
    XIDeviceEvent* xievent =
        static_cast<XIDeviceEvent*>(native_event->xcookie.data);
    return xievent->evtype == XI_ButtonPress ||
           xievent->evtype == XI_ButtonRelease ||
           xievent->evtype == XI_Motion;
  }
  return false;
}

int GetChangedMouseButtonFlagsFromNative(
    const base::NativeEvent& native_event) {
  switch (native_event->type) {
    case ButtonPress:
    case ButtonRelease:
      return GetEventFlagsFromXState(native_event->xbutton.state);
    case GenericEvent: {
      XIDeviceEvent* xievent =
          static_cast<XIDeviceEvent*>(native_event->xcookie.data);
      switch (xievent->evtype) {
        case XI_ButtonPress:
        case XI_ButtonRelease:
          return GetEventFlagsForButton(EventButtonFromNative(native_event));
        default:
          break;
      }
    }
    default:
      break;
  }
  return 0;
}

gfx::Vector2d GetMouseWheelOffset(const base::NativeEvent& native_event) {
  float x_offset = 0;
  float y_offset = 0;
  if (native_event->type == GenericEvent &&
      GetScrollOffsets(native_event, &x_offset, &y_offset, NULL, NULL, NULL)) {
    return gfx::Vector2d(static_cast<int>(x_offset),
                         static_cast<int>(y_offset));
  }

  int button = native_event->type == GenericEvent ?
      EventButtonFromNative(native_event) : native_event->xbutton.button;

  switch (button) {
    case 4:
      return gfx::Vector2d(0, kWheelScrollAmount);
    case 5:
      return gfx::Vector2d(0, -kWheelScrollAmount);
    default:
      // TODO(derat): Do something for horizontal scrolls (buttons 6 and 7)?
      return gfx::Vector2d();
  }
}

int GetTouchId(const base::NativeEvent& xev) {
  float slot = 0;
  ui::TouchFactory* factory = ui::TouchFactory::GetInstance();
  XIDeviceEvent* xievent = static_cast<XIDeviceEvent*>(xev->xcookie.data);
  if (!factory->IsMultiTouchDevice(xievent->sourceid)) {
    // TODO(sad): Come up with a way to generate touch-ids for multi-touch
    // events when touch-events are generated from a single-touch device.
    return slot;
  }

  ui::ValuatorTracker* valuators = ui::ValuatorTracker::GetInstance();

#if defined(USE_XI2_MT)
  float tracking_id;
  if (!valuators->ExtractValuator(
      *xev, ui::ValuatorTracker::VAL_TRACKING_ID, &tracking_id)) {
    LOG(ERROR) << "Could not get the slot ID for the event. Using 0.";
  } else {
    slot = factory->GetSlotForTrackingID(tracking_id);
    ui::EventType type = ui::EventTypeFromNative(xev);
    if (type == ui::ET_TOUCH_CANCELLED ||
        type == ui::ET_TOUCH_RELEASED) {
      factory->ReleaseSlotForTrackingID(tracking_id);
    }
  }
#else
  if (!valuators->ExtractValuator(
      *xev, ui::ValuatorTracker::VAL_SLOT_ID, &slot))
    LOG(ERROR) << "Could not get the slot ID for the event. Using 0.";
#endif
  return slot;
}

float GetTouchRadiusX(const base::NativeEvent& native_event) {
  return GetTouchParamFromXEvent(native_event,
      ui::ValuatorTracker::VAL_TOUCH_MAJOR, 0.0) / 2.0;
}

float GetTouchRadiusY(const base::NativeEvent& native_event) {
  return GetTouchParamFromXEvent(native_event,
      ui::ValuatorTracker::VAL_TOUCH_MINOR, 0.0) / 2.0;
}

float GetTouchAngle(const base::NativeEvent& native_event) {
  return GetTouchParamFromXEvent(native_event,
      ui::ValuatorTracker::VAL_ORIENTATION, 0.0) / 2.0;
}

float GetTouchForce(const base::NativeEvent& native_event) {
  float force = 0.0;
  force = GetTouchParamFromXEvent(native_event,
      ui::ValuatorTracker::VAL_PRESSURE, 0.0);
  unsigned int deviceid =
      static_cast<XIDeviceEvent*>(native_event->xcookie.data)->sourceid;
  // Force is normalized to fall into [0, 1]
  if (!ui::ValuatorTracker::GetInstance()->NormalizeValuator(
      deviceid, ui::ValuatorTracker::VAL_PRESSURE, &force))
    force = 0.0;
  return force;
}

bool GetScrollOffsets(const base::NativeEvent& native_event,
                      float* x_offset,
                      float* y_offset,
                      float* x_offset_ordinal,
                      float* y_offset_ordinal,
                      int* finger_count) {
  return CMTEventData::GetInstance()->GetScrollOffsets(
      *native_event,
      x_offset, y_offset,
      x_offset_ordinal, y_offset_ordinal,
      finger_count);
}

bool GetFlingData(const base::NativeEvent& native_event,
                  float* vx,
                  float* vy,
                  float* vx_ordinal,
                  float* vy_ordinal,
                  bool* is_cancel) {
  return CMTEventData::GetInstance()->GetFlingData(
      *native_event, vx, vy, vx_ordinal, vy_ordinal, is_cancel);
}

bool GetGestureTimes(const base::NativeEvent& native_event,
                     double* start_time,
                     double* end_time) {
  return CMTEventData::GetInstance()->GetGestureTimes(
      *native_event, start_time, end_time);
}

void SetNaturalScroll(bool enabled) {
  CMTEventData::GetInstance()->set_natural_scroll_enabled(enabled);
}

bool IsNaturalScrollEnabled() {
  return CMTEventData::GetInstance()->natural_scroll_enabled();
}

bool IsTouchpadEvent(const base::NativeEvent& event) {
  return CMTEventData::GetInstance()->IsTouchpadXInputEvent(event);
}

bool IsNoopEvent(const base::NativeEvent& event) {
  return (event->type == ClientMessage &&
      event->xclient.message_type == GetNoopEventAtom());
}

base::NativeEvent CreateNoopEvent() {
  static XEvent* noop = NULL;
  if (!noop) {
    noop = new XEvent();
    memset(noop, 0, sizeof(XEvent));
    noop->xclient.type = ClientMessage;
    noop->xclient.window = None;
    noop->xclient.format = 8;
    DCHECK(!noop->xclient.display);
  }
  // Make sure we use atom from current xdisplay, which may
  // change during the test.
  noop->xclient.message_type = GetNoopEventAtom();
  return noop;
}

}  // namespace ui
