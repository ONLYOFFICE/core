// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_X_VALUATORS_H_
#define UI_BASE_X_VALUATORS_H_

#include "base/basictypes.h"
#include "ui/base/ui_export.h"

template <typename T> struct DefaultSingletonTraits;

typedef union _XEvent XEvent;

namespace ui {

// A valuator is not reported in an XEvent if it hasn't changed from the
// previous event from the same device. As a result, it is necessary to track
// the last reported valuator values for a specific device.
// Right now, this is only used to keep track of valuators for events coming in
// from touch-devices, but it can be used for other devices as well.
class UI_EXPORT ValuatorTracker {
 private:
  ValuatorTracker();
  ~ValuatorTracker();

 public:
  // Define the valuators following the Multi-touch Protocol.
  enum Valuator {
    VAL_TOUCH_MAJOR = 0,   // Length of the touch area.
    VAL_TOUCH_MINOR,       // Width of the touch area.
    VAL_ORIENTATION,       // Angle between the X-axis and the major axis of the
                           // touch area.
    VAL_PRESSURE,          // Pressure of the touch contact.

    // NOTE: A touch event can have multiple touch points. So when we receive a
    // touch event, we need to determine which point triggered the event.
    // A touch point can have both a 'Slot ID' and a 'Tracking ID', and they can
    // be (in fact, usually are) different. The 'Slot ID' ranges between 0 and
    // (X - 1), where X is the maximum touch points supported by the device. The
    // 'Tracking ID' can be any 16-bit value. With XInput 2.0, an XI_Motion
    // event that comes from a currently-unused 'Slot ID' indicates the creation
    // of a new touch point, and any event that comes with a 0 value for
    // 'Tracking ID' marks the removal of a touch point. During the lifetime of
    // a touchpoint, we use the 'Slot ID' as its identifier. The XI_ButtonPress
    // and XI_ButtonRelease events are ignored.
#if !defined(USE_XI2_MT)
    VAL_SLOT_ID,           // ID of the finger that triggered a touch event
                           // (useful when tracking multiple simultaneous
                           // touches)
#endif
    // NOTE for XInput MT: 'Tracking ID' is provided in every touch event to
    // track individual touch. 'Tracking ID' is an unsigned 32-bit value and
    // is increased for each new touch. It will wrap back to 0 when reaching
    // the numerical limit.
    VAL_TRACKING_ID,       // ID of the touch point.

    // Kernel timestamp from touch screen (if available).
    VAL_TOUCH_RAW_TIMESTAMP,

    VAL_LAST_ENTRY
  };

  // Returns the ValuatorTracker singleton.
  static ValuatorTracker* GetInstance();

  // Extract the Valuator from the XEvent. Return true and the value is set
  // if the Valuator is found, false and value unchanged if the Valuator
  // is not found.
  bool ExtractValuator(const XEvent& xev, Valuator val, float* value);

  // Normalize the Valuator with value on deviceid to fall into [0, 1].
  // *value = (*value - min_value_of_tp) / (max_value_of_tp - min_value_of_tp)
  // Returns true and sets the normalized value in|value| if normalization is
  // successful. Returns false and |value| is unchanged otherwise.
  bool NormalizeValuator(unsigned int deviceid, Valuator val, float* value);

  // Extract the range of the Valuator. Return true if the range is available
  // and written into min & max, false if the range is not available.
  bool GetValuatorRange(unsigned int deviceid,
                        Valuator val,
                        float* min,
                        float* max);

  // Sets up the internal bookkeeping of the valuator information. It currently
  // does this for touch devices only.
  void SetupValuator();

 private:
  // Requirement for Singleton.
  friend struct DefaultSingletonTraits<ValuatorTracker>;

  static const int kMaxDeviceNum = 128;

  // Index table to find the valuator for the Valuator on the specific device
  // by valuator_lookup_[device_id][valuator]. Use 2-D array to get fast
  // index at the expense of space. If the kMaxDeviceNum grows larger so that
  // the space waste becomes a concern, the 2D lookup table can be replaced by a
  // hash map.
  signed char valuator_lookup_[kMaxDeviceNum][VAL_LAST_ENTRY];

  // Index table to find the min & max value of the Valuator on a specific
  // device.
  int valuator_min_[kMaxDeviceNum][VAL_LAST_ENTRY];
  int valuator_max_[kMaxDeviceNum][VAL_LAST_ENTRY];

  // Table to keep track of the last seen value for the specified valuator for
  // a device. Defaults to 0 if the valuator was not specified in an earlier
  // event.
  float last_seen_valuator_[kMaxDeviceNum][VAL_LAST_ENTRY];

  DISALLOW_COPY_AND_ASSIGN(ValuatorTracker);
};

}  // namespace ui

#endif  // UI_BASE_X_VALUATORS_H_
