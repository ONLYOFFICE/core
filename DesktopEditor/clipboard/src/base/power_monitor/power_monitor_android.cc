// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/power_monitor/power_monitor.h"
#include "jni/PowerMonitor_jni.h"

namespace base {

// A helper function which is a friend of PowerMonitor.
void ProcessPowerEventHelper(PowerMonitor::PowerEvent event) {
  if (base::PowerMonitor::Get())
    base::PowerMonitor::Get()->ProcessPowerEvent(event);
}

namespace android {

// Native implementation of PowerMonitor.java.
void OnBatteryChargingChanged(JNIEnv* env, jclass clazz) {
  ProcessPowerEventHelper(PowerMonitor::POWER_STATE_EVENT);
}

void OnMainActivityResumed(JNIEnv* env, jclass clazz) {
  ProcessPowerEventHelper(PowerMonitor::RESUME_EVENT);
}

void OnMainActivitySuspended(JNIEnv* env, jclass clazz) {
  ProcessPowerEventHelper(PowerMonitor::SUSPEND_EVENT);
}

}  // namespace android

bool PowerMonitor::IsBatteryPower() {
  JNIEnv* env = base::android::AttachCurrentThread();
  return base::android::Java_PowerMonitor_isBatteryPower(env);
}

bool RegisterPowerMonitor(JNIEnv* env) {
  return base::android::RegisterNativesImpl(env);
}

}  // namespace base
