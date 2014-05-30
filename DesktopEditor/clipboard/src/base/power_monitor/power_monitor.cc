// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/power_monitor/power_monitor.h"

#include "base/time.h"

namespace base {

static PowerMonitor* g_power_monitor = NULL;

#if defined(ENABLE_BATTERY_MONITORING)
// The amount of time (in ms) to wait before running the initial
// battery check.
static int kDelayedBatteryCheckMs = 10 * 1000;
#endif  // defined(ENABLE_BATTERY_MONITORING)

PowerMonitor::PowerMonitor()
    : observers_(new ObserverListThreadSafe<PowerObserver>()),
      battery_in_use_(false),
      suspended_(false) {
  DCHECK(!g_power_monitor);
  g_power_monitor = this;

  DCHECK(MessageLoop::current());
#if defined(ENABLE_BATTERY_MONITORING)
  delayed_battery_check_.Start(FROM_HERE,
      base::TimeDelta::FromMilliseconds(kDelayedBatteryCheckMs), this,
      &PowerMonitor::BatteryCheck);
#endif  // defined(ENABLE_BATTERY_MONITORING)
#if defined(OS_MACOSX)
  PlatformInit();
#endif
}

PowerMonitor::~PowerMonitor() {
#if defined(OS_MACOSX)
  PlatformDestroy();
#endif
  DCHECK_EQ(this, g_power_monitor);
  g_power_monitor = NULL;
}

// static
PowerMonitor* PowerMonitor::Get() {
  return g_power_monitor;
}

void PowerMonitor::AddObserver(PowerObserver* obs) {
  observers_->AddObserver(obs);
}

void PowerMonitor::RemoveObserver(PowerObserver* obs) {
  observers_->RemoveObserver(obs);
}

void PowerMonitor::ProcessPowerEvent(PowerEvent event_id) {
  // Suppress duplicate notifications.  Some platforms may
  // send multiple notifications of the same event.
  switch (event_id) {
    case POWER_STATE_EVENT:
      {
        bool on_battery = IsBatteryPower();
        if (on_battery != battery_in_use_) {
          battery_in_use_ = on_battery;
          NotifyPowerStateChange();
        }
      }
      break;
    case RESUME_EVENT:
      if (suspended_) {
        suspended_ = false;
        NotifyResume();
      }
      break;
    case SUSPEND_EVENT:
      if (!suspended_) {
        suspended_ = true;
        NotifySuspend();
      }
      break;
  }
}

void PowerMonitor::BatteryCheck() {
  ProcessPowerEvent(PowerMonitor::POWER_STATE_EVENT);
}

void PowerMonitor::NotifyPowerStateChange() {
  DVLOG(1) << "PowerStateChange: " << (BatteryPower() ? "On" : "Off")
           << " battery";
  observers_->Notify(&PowerObserver::OnPowerStateChange, BatteryPower());
}

void PowerMonitor::NotifySuspend() {
  DVLOG(1) << "Power Suspending";
  observers_->Notify(&PowerObserver::OnSuspend);
}

void PowerMonitor::NotifyResume() {
  DVLOG(1) << "Power Resuming";
  observers_->Notify(&PowerObserver::OnResume);
}

}  // namespace base
