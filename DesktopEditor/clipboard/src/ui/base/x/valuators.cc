// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/x/valuators.h"

#include <X11/extensions/XInput2.h>

#include "base/memory/singleton.h"
#include "ui/base/touch/touch_factory_x11.h"
#include "ui/base/x/device_list_cache_x.h"
#include "ui/base/x/x11_util.h"

namespace {

#define AXIS_LABEL_ABS_MT_TOUCH_MAJOR "Abs MT Touch Major"
#define AXIS_LABEL_ABS_MT_TOUCH_MINOR "Abs MT Touch Minor"
#define AXIS_LABEL_ABS_MT_ORIENTATION "Abs MT Orientation"
#define AXIS_LABEL_ABS_MT_PRESSURE    "Abs MT Pressure"
#define AXIS_LABEL_ABS_MT_SLOT_ID     "Abs MT Slot ID"
#define AXIS_LABEL_ABS_MT_TRACKING_ID "Abs MT Tracking ID"
#define AXIS_LABEL_TOUCH_TIMESTAMP    "Touch Timestamp"

// Given the Valuator, return the correspoding XIValuatorClassInfo using
// the X device information through Atom name matching.
XIValuatorClassInfo* FindValuator(Display* display,
                                  XIDeviceInfo* info,
                                  ui::ValuatorTracker::Valuator val) {
  // Lookup table for mapping Valuator to Atom string used in X.
  // A full set of Atom strings can be found at xserver-properties.h.
  static struct {
    ui::ValuatorTracker::Valuator val;
    Atom atom;
  } kValuatorAtom[] = {
    { ui::ValuatorTracker::VAL_TOUCH_MAJOR,
      XInternAtom(ui::GetXDisplay(), AXIS_LABEL_ABS_MT_TOUCH_MAJOR, false) },
    { ui::ValuatorTracker::VAL_TOUCH_MINOR,
      XInternAtom(ui::GetXDisplay(), AXIS_LABEL_ABS_MT_TOUCH_MINOR, false) },
    { ui::ValuatorTracker::VAL_ORIENTATION,
      XInternAtom(ui::GetXDisplay(), AXIS_LABEL_ABS_MT_ORIENTATION, false) },
    { ui::ValuatorTracker::VAL_PRESSURE,
      XInternAtom(ui::GetXDisplay(), AXIS_LABEL_ABS_MT_PRESSURE, false) },
#if !defined(USE_XI2_MT)
    // For Slot ID, See this chromeos revision: http://git.chromium.org/gitweb/?
    //        p=chromiumos/overlays/chromiumos-overlay.git;
    //        a=commit;h=9164d0a75e48c4867e4ef4ab51f743ae231c059a
    { ui::ValuatorTracker::VAL_SLOT_ID,
      XInternAtom(ui::GetXDisplay(), AXIS_LABEL_ABS_MT_SLOT_ID, false) },
#endif
    { ui::ValuatorTracker::VAL_TRACKING_ID,
      XInternAtom(ui::GetXDisplay(), AXIS_LABEL_ABS_MT_TRACKING_ID, false) },
    { ui::ValuatorTracker::VAL_TOUCH_RAW_TIMESTAMP,
      XInternAtom(ui::GetXDisplay(), AXIS_LABEL_TOUCH_TIMESTAMP, false) },
    { ui::ValuatorTracker::VAL_LAST_ENTRY, None },
  };

  Atom atom = None;

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(kValuatorAtom); i++) {
    if (val == kValuatorAtom[i].val) {
      atom = kValuatorAtom[i].atom;
      break;
    }
  }

  if (atom == None)
    return NULL;

  for (int i = 0; i < info->num_classes; i++) {
    if (info->classes[i]->type != XIValuatorClass)
      continue;
    XIValuatorClassInfo* v =
        reinterpret_cast<XIValuatorClassInfo*>(info->classes[i]);
    if (v->label && atom == v->label)
      return v;
  }

  return NULL;
}

}  // namespace

namespace ui {

ValuatorTracker::ValuatorTracker() {
  SetupValuator();
}

ValuatorTracker::~ValuatorTracker() {
}

// static
ValuatorTracker* ValuatorTracker::GetInstance() {
  return Singleton<ValuatorTracker>::get();
}

bool ValuatorTracker::ExtractValuator(const XEvent& xev,
                                      Valuator val,
                                      float* value) {
  XIDeviceEvent* xiev = static_cast<XIDeviceEvent*>(xev.xcookie.data);
  if (xiev->sourceid >= kMaxDeviceNum || xiev->deviceid >= kMaxDeviceNum)
    return false;
  int val_index = valuator_lookup_[xiev->sourceid][val];
  if (val_index >= 0) {
    if (XIMaskIsSet(xiev->valuators.mask, val_index)) {
      double* valuators = xiev->valuators.values;
      while (val_index--) {
        if (XIMaskIsSet(xiev->valuators.mask, val_index))
          ++valuators;
      }
      *value = *valuators;
      last_seen_valuator_[xiev->deviceid][val] = *value;
      return true;
    } else {
      *value = last_seen_valuator_[xiev->deviceid][val];
    }
  }

#if defined(USE_XI2_MT)
  // With XInput2 MT, Tracking ID is provided in the detail field.
  if (val == VAL_TRACKING_ID) {
    *value = xiev->detail;
    return true;
  }
#endif

  return false;
}

bool ValuatorTracker::NormalizeValuator(unsigned int deviceid,
                                        Valuator val,
                                        float* value) {
  float max_value;
  float min_value;
  if (GetValuatorRange(deviceid, val, &min_value, &max_value)) {
    *value = (*value - min_value) / (max_value - min_value);
    DCHECK(*value >= 0.0 && *value <= 1.0);
    return true;
  }
  return false;
}

bool ValuatorTracker::GetValuatorRange(unsigned int deviceid,
                                       Valuator val,
                                       float* min,
                                       float* max) {
  if (valuator_lookup_[deviceid][val] >= 0) {
    *min = valuator_min_[deviceid][val];
    *max = valuator_max_[deviceid][val];
    return true;
  }
  return false;
}

void ValuatorTracker::SetupValuator() {
  memset(valuator_lookup_, -1, sizeof(valuator_lookup_));
  memset(valuator_min_, 0, sizeof(valuator_min_));
  memset(valuator_max_, 0, sizeof(valuator_max_));
  memset(last_seen_valuator_, 0, sizeof(last_seen_valuator_));

  Display* display = GetXDisplay();
  XIDeviceList info_list =
      DeviceListCacheX::GetInstance()->GetXI2DeviceList(display);
  TouchFactory* factory = TouchFactory::GetInstance();

  for (int i = 0; i < info_list.count; i++) {
    XIDeviceInfo* info = info_list.devices + i;

    if (!factory->IsTouchDevice(info->deviceid))
      continue;

    for (int j = 0; j < VAL_LAST_ENTRY; j++) {
      Valuator val = static_cast<Valuator>(j);
      XIValuatorClassInfo* valuator = FindValuator(display, info, val);
      if (valuator) {
        valuator_lookup_[info->deviceid][j] = valuator->number;
        valuator_min_[info->deviceid][j] = valuator->min;
        valuator_max_[info->deviceid][j] = valuator->max;
      }
    }
  }
}

}  // namespace ui
