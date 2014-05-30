// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/touch/touch_factory_x11.h"

#include <X11/cursorfont.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/XIproto.h>

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/compiler_specific.h"
#include "base/logging.h"
#include "base/memory/singleton.h"
#include "base/message_loop.h"
#include "base/string_number_conversions.h"
#include "base/strings/string_split.h"
#include "ui/base/ui_base_switches.h"
#include "ui/base/x/device_list_cache_x.h"
#include "ui/base/x/x11_util.h"

namespace ui {

TouchFactory::TouchFactory()
    : pointer_device_lookup_(),
      touch_device_available_(false),
      touch_events_disabled_(false),
      touch_device_list_(),
#if defined(USE_XI2_MT)
      min_available_slot_(0),
#endif
      slots_used_() {
#if defined(USE_AURA)
  if (!base::MessagePumpForUI::HasXInput2())
    return;
#endif

  Display* display = GetXDisplay();
  UpdateDeviceList(display);

  CommandLine* cmdline = CommandLine::ForCurrentProcess();
  touch_events_disabled_ = cmdline->HasSwitch(switches::kTouchEvents) &&
      cmdline->GetSwitchValueASCII(switches::kTouchEvents) ==
          switches::kTouchEventsDisabled;
}

TouchFactory::~TouchFactory() {
}

// static
TouchFactory* TouchFactory::GetInstance() {
  return Singleton<TouchFactory>::get();
}

// static
void TouchFactory::SetTouchDeviceListFromCommandLine() {
#if defined(TOOLKIT_VIEWS)
  // Get a list of pointer-devices that should be treated as touch-devices.
  // This is primarily used for testing/debugging touch-event processing when a
  // touch-device isn't available.
  std::string touch_devices =
      CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
          switches::kTouchDevices);

  if (!touch_devices.empty()) {
    std::vector<std::string> devs;
    std::vector<unsigned int> device_ids;
    unsigned int devid;
    base::SplitString(touch_devices, ',', &devs);
    for (std::vector<std::string>::iterator iter = devs.begin();
        iter != devs.end(); ++iter) {
      if (base::StringToInt(*iter, reinterpret_cast<int*>(&devid)))
        device_ids.push_back(devid);
      else
        DLOG(WARNING) << "Invalid touch-device id: " << *iter;
    }
    ui::TouchFactory::GetInstance()->SetTouchDeviceList(device_ids);
  }
#endif
}

void TouchFactory::UpdateDeviceList(Display* display) {
  // Detect touch devices.
  touch_device_available_ = false;
  touch_device_lookup_.reset();
  touch_device_list_.clear();

#if !defined(USE_XI2_MT)
  // NOTE: The new API for retrieving the list of devices (XIQueryDevice) does
  // not provide enough information to detect a touch device. As a result, the
  // old version of query function (XListInputDevices) is used instead.
  // If XInput2 is not supported, this will return null (with count of -1) so
  // we assume there cannot be any touch devices.
  // With XI2.1 or older, we allow only single touch devices.
  XDeviceList dev_list =
      DeviceListCacheX::GetInstance()->GetXDeviceList(display);
  for (int i = 0; i < dev_list.count; i++) {
    if (dev_list[i].type) {
      XScopedString devtype(XGetAtomName(display, dev_list[i].type));
      if (devtype.string() && !strcmp(devtype.string(), XI_TOUCHSCREEN)) {
        touch_device_lookup_[dev_list[i].id] = true;
        touch_device_list_[dev_list[i].id] = false;
        touch_device_available_ = true;
      }
    }
  }
#endif

  // Instead of asking X for the list of devices all the time, let's maintain a
  // list of pointer devices we care about.
  // It should not be necessary to select for slave devices. XInput2 provides
  // enough information to the event callback to decide which slave device
  // triggered the event, thus decide whether the 'pointer event' is a
  // 'mouse event' or a 'touch event'.
  // However, on some desktops, some events from a master pointer are
  // not delivered to the client. So we select for slave devices instead.
  // If the touch device has 'GrabDevice' set and 'SendCoreEvents' unset (which
  // is possible), then the device is detected as a floating device, and a
  // floating device is not connected to a master device. So it is necessary to
  // also select on the floating devices.
  pointer_device_lookup_.reset();
  XIDeviceList xi_dev_list =
      DeviceListCacheX::GetInstance()->GetXI2DeviceList(display);
  for (int i = 0; i < xi_dev_list.count; i++) {
    XIDeviceInfo* devinfo = xi_dev_list.devices + i;
    if (devinfo->use == XIFloatingSlave || devinfo->use == XIMasterPointer) {
#if defined(USE_XI2_MT)
      for (int k = 0; k < devinfo->num_classes; ++k) {
        XIAnyClassInfo* xiclassinfo = devinfo->classes[k];
        if (xiclassinfo->type == XITouchClass) {
          XITouchClassInfo* tci =
              reinterpret_cast<XITouchClassInfo *>(xiclassinfo);
          // Only care direct touch device (such as touch screen) right now
          if (tci->mode == XIDirectTouch) {
            touch_device_lookup_[devinfo->deviceid] = true;
            touch_device_list_[devinfo->deviceid] = true;
            touch_device_available_ = true;
          }
        }
      }
#endif
      pointer_device_lookup_[devinfo->deviceid] = true;
    }
  }
}

bool TouchFactory::ShouldProcessXI2Event(XEvent* xev) {
  DCHECK_EQ(GenericEvent, xev->type);
  XIEvent* event = static_cast<XIEvent*>(xev->xcookie.data);
  XIDeviceEvent* xiev = reinterpret_cast<XIDeviceEvent*>(event);

#if defined(USE_XI2_MT)
  if (event->evtype == XI_TouchBegin ||
      event->evtype == XI_TouchUpdate ||
      event->evtype == XI_TouchEnd) {
    return !touch_events_disabled_ && IsTouchDevice(xiev->deviceid);
  }
#endif
  if (event->evtype != XI_ButtonPress &&
      event->evtype != XI_ButtonRelease &&
      event->evtype != XI_Motion)
    return true;

  if (!pointer_device_lookup_[xiev->deviceid])
    return false;

  return IsTouchDevice(xiev->deviceid) ? !touch_events_disabled_ : true;
}

void TouchFactory::SetupXI2ForXWindow(Window window) {
  // Setup mask for mouse events. It is possible that a device is loaded/plugged
  // in after we have setup XInput2 on a window. In such cases, we need to
  // either resetup XInput2 for the window, so that we get events from the new
  // device, or we need to listen to events from all devices, and then filter
  // the events from uninteresting devices. We do the latter because that's
  // simpler.

  Display* display = ui::GetXDisplay();

  unsigned char mask[XIMaskLen(XI_LASTEVENT)];
  memset(mask, 0, sizeof(mask));

#if defined(USE_XI2_MT)
  XISetMask(mask, XI_TouchBegin);
  XISetMask(mask, XI_TouchUpdate);
  XISetMask(mask, XI_TouchEnd);
#endif
  XISetMask(mask, XI_ButtonPress);
  XISetMask(mask, XI_ButtonRelease);
  XISetMask(mask, XI_Motion);

  XIEventMask evmask;
  evmask.deviceid = XIAllDevices;
  evmask.mask_len = sizeof(mask);
  evmask.mask = mask;
  XISelectEvents(display, window, &evmask, 1);
  XFlush(display);
}

void TouchFactory::SetTouchDeviceList(
    const std::vector<unsigned int>& devices) {
  touch_device_lookup_.reset();
  touch_device_list_.clear();
  for (std::vector<unsigned int>::const_iterator iter = devices.begin();
       iter != devices.end(); ++iter) {
    DCHECK(*iter < touch_device_lookup_.size());
    touch_device_lookup_[*iter] = true;
    touch_device_list_[*iter] = false;
  }
}

bool TouchFactory::IsTouchDevice(unsigned deviceid) const {
  return deviceid < touch_device_lookup_.size() ?
      touch_device_lookup_[deviceid] : false;
}

bool TouchFactory::IsMultiTouchDevice(unsigned int deviceid) const {
  return (deviceid < touch_device_lookup_.size() &&
          touch_device_lookup_[deviceid]) ?
          touch_device_list_.find(deviceid)->second :
          false;
}

#if defined(USE_XI2_MT)
int TouchFactory::GetSlotForTrackingID(uint32 tracking_id) {
  TrackingIdMap::iterator itr = tracking_id_map_.find(tracking_id);
  if (itr != tracking_id_map_.end())
    return itr->second;

  int slot = min_available_slot_;
  if (slot == kMaxTouchPoints) {
    LOG(ERROR) << "Could not find available slot for touch point";
    return 0;
  }
  SetSlotUsed(slot, true);
  tracking_id_map_.insert(std::make_pair(tracking_id, slot));

  // Updates the minium available slot ID
  while (++min_available_slot_ < kMaxTouchPoints &&
         IsSlotUsed(min_available_slot_))
    continue;

  return slot;
}

void TouchFactory::ReleaseSlotForTrackingID(uint32 tracking_id) {
  TrackingIdMap::iterator itr = tracking_id_map_.find(tracking_id);
  if (itr != tracking_id_map_.end()) {
    int slot = itr->second;
    SetSlotUsed(slot, false);
    tracking_id_map_.erase(itr);
    if (slot < min_available_slot_)
      min_available_slot_ = slot;
  } else {
    NOTREACHED() << "Cannot find slot mapping to tracking ID " << tracking_id;
  }
}
#endif

bool TouchFactory::IsSlotUsed(int slot) const {
  CHECK_LT(slot, kMaxTouchPoints);
  return slots_used_[slot];
}

void TouchFactory::SetSlotUsed(int slot, bool used) {
  CHECK_LT(slot, kMaxTouchPoints);
  slots_used_[slot] = used;
}

bool TouchFactory::IsTouchDevicePresent() {
  return !touch_events_disabled_ && touch_device_available_;
}

}  // namespace ui
