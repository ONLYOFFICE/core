// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/device_list_updater_aurax11.h"

#include <X11/extensions/XInput2.h>

#include "ui/base/events/event_utils.h"

namespace aura {

DeviceListUpdaterAuraX11::DeviceListUpdaterAuraX11() {}

DeviceListUpdaterAuraX11::~DeviceListUpdaterAuraX11() {}

base::EventStatus DeviceListUpdaterAuraX11::WillProcessEvent(
    const base::NativeEvent& event) {
  // XI_HierarchyChanged events are special. There is no window associated with
  // these events. So process them directly from here.
  if (event->type == GenericEvent &&
      event->xgeneric.evtype == XI_HierarchyChanged) {
    ui::UpdateDeviceList();
  }

  return base::EVENT_CONTINUE;
}

void DeviceListUpdaterAuraX11::DidProcessEvent(const base::NativeEvent& event) {
}

}  // namespace aura
