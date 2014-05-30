// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_X_DEVICE_LIST_CACHE_X_H_
#define UI_BASE_X_DEVICE_LIST_CACHE_X_H_

#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>

#include <map>

#include "base/basictypes.h"
#include "ui/base/ui_export.h"

template <typename T> struct DefaultSingletonTraits;

typedef struct _XDisplay Display;

template <typename T>
struct DeviceList {
  DeviceList() : devices(NULL), count(0) {
  }
  T& operator[] (int x) {
    return devices[x];
  }
  T* devices;
  int count;
};

typedef struct DeviceList<XDeviceInfo> XDeviceList;
typedef struct DeviceList<XIDeviceInfo> XIDeviceList;

namespace ui {

// A class to cache the current XInput device list. This minimized the
// round-trip time to the X server whenever such a device list is needed. The
// update function will be called on each incoming XI_HierarchyChanged event.
class UI_EXPORT DeviceListCacheX {
 public:
  static DeviceListCacheX* GetInstance();

  void UpdateDeviceList(Display* display);

  const XDeviceList& GetXDeviceList(Display* display);
  const XIDeviceList& GetXI2DeviceList(Display* display);

 private:
  friend struct DefaultSingletonTraits<DeviceListCacheX>;

  DeviceListCacheX();
  ~DeviceListCacheX();

  std::map<Display*, XDeviceList> x_dev_list_map_;
  std::map<Display*, XIDeviceList> xi_dev_list_map_;

  DISALLOW_COPY_AND_ASSIGN(DeviceListCacheX);
};

}  // namespace ui

#endif  // UI_BASE_X_DEVICE_LIST_CACHE_X_H_

