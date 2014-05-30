// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_POWER_MONITOR_POWER_MONITOR_H_
#define BASE_POWER_MONITOR_POWER_MONITOR_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list_threadsafe.h"
#include "base/power_monitor/power_observer.h"

#if defined(OS_WIN)
#include <windows.h>

// Windows HiRes timers drain the battery faster so we need to know the battery
// status.  This isn't true for other platforms.
#define ENABLE_BATTERY_MONITORING 1
#else
#undef ENABLE_BATTERY_MONITORING
#endif  // !OS_WIN

#if defined(ENABLE_BATTERY_MONITORING)
#include "base/timer.h"
#endif  // defined(ENABLE_BATTERY_MONITORING)

#if defined(OS_IOS)
#include <objc/runtime.h>
#endif  // OS_IOS

namespace base {

// A class used to monitor the power state change and notify the observers about
// the change event.
class BASE_EXPORT PowerMonitor {
 public:
  // Normalized list of power events.
  enum PowerEvent {
    POWER_STATE_EVENT,  // The Power status of the system has changed.
    SUSPEND_EVENT,      // The system is being suspended.
    RESUME_EVENT        // The system is being resumed.
  };

  PowerMonitor();
  ~PowerMonitor();

  // Get the application-wide PowerMonitor (if not present, returns NULL).
  static PowerMonitor* Get();

#if defined(OS_MACOSX)
  // Allocate system resources needed by the PowerMonitor class.
  //
  // This function must be called before instantiating an instance of the class
  // and before the Sandbox is initialized.
#if !defined(OS_IOS)
  static void AllocateSystemIOPorts();
#else
  static void AllocateSystemIOPorts() {}
#endif  // OS_IOS
#endif  // OS_MACOSX

  // Add and remove an observer.
  // Can be called from any thread.
  // Must not be called from within a notification callback.
  void AddObserver(PowerObserver* observer);
  void RemoveObserver(PowerObserver* observer);

  // Is the computer currently on battery power. Can be called on any thread.
  bool BatteryPower() const {
    // Using a lock here is not necessary for just a bool.
    return battery_in_use_;
  }

 private:
  friend class PowerMonitorTest;
  // A friend function that is allowed to access the private ProcessPowerEvent.
  friend void ProcessPowerEventHelper(PowerEvent);

#if defined(OS_WIN)
  // Represents a message-only window for power message handling on Windows.
  // Only allow PowerMonitor to create it.
  class PowerMessageWindow {
   public:
    PowerMessageWindow();
    ~PowerMessageWindow();

   private:
    void ProcessWmPowerBroadcastMessage(int event_id);
    LRESULT CALLBACK WndProc(HWND hwnd, UINT message,
                             WPARAM wparam, LPARAM lparam);
    static LRESULT CALLBACK WndProcThunk(HWND hwnd,
                                         UINT message,
                                         WPARAM wparam,
                                         LPARAM lparam);
    // Instance of the module containing the window procedure.
    HMODULE instance_;
    // A hidden message-only window.
    HWND message_hwnd_;
  };
#endif  // OS_WIN

#if defined(OS_MACOSX)
  void PlatformInit();
  void PlatformDestroy();
#endif

  // Cross-platform handling of a power event.
  void ProcessPowerEvent(PowerEvent event_id);

  // Platform-specific method to check whether the system is currently
  // running on battery power.  Returns true if running on batteries,
  // false otherwise.
  bool IsBatteryPower();

  // Checks the battery status and notifies observers if the battery
  // status has changed.
  void BatteryCheck();

  void NotifyPowerStateChange();
  void NotifySuspend();
  void NotifyResume();

#if defined(OS_IOS)
  // Holds pointers to system event notification observers.
  std::vector<id> notification_observers_;
#endif

#if defined(ENABLE_BATTERY_MONITORING)
  base::OneShotTimer<PowerMonitor> delayed_battery_check_;
#endif

  scoped_refptr<ObserverListThreadSafe<PowerObserver> > observers_;
  bool battery_in_use_;
  bool suspended_;

#if defined(OS_WIN)
  PowerMessageWindow power_message_window_;
#endif

  DISALLOW_COPY_AND_ASSIGN(PowerMonitor);
};

}  // namespace base

#endif  // BASE_POWER_MONITOR_POWER_MONITOR_H_
