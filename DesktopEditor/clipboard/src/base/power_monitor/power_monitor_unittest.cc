// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/power_monitor/power_monitor.h"

#include "base/message_loop.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

class PowerTest : public PowerObserver {
 public:
  PowerTest()
      : power_state_changes_(0),
        suspends_(0),
        resumes_(0) {
  }

  // PowerObserver callbacks.
  virtual void OnPowerStateChange(bool on_battery_power) OVERRIDE {
    power_state_changes_++;
  }

  virtual void OnSuspend() OVERRIDE {
    suspends_++;
  }

  virtual void OnResume() OVERRIDE {
    resumes_++;
  }

  // Test status counts.
  int power_state_changes() { return power_state_changes_; }
  int suspends() { return suspends_; }
  int resumes() { return resumes_; }

 private:
  int power_state_changes_;  // Count of OnPowerStateChange notifications.
  int suspends_;  // Count of OnSuspend notifications.
  int resumes_;  // Count of OnResume notifications.
};

class PowerMonitorTest : public testing::Test {
 protected:
  PowerMonitorTest() {
#if defined(OS_MACOSX)
    // This needs to happen before PowerMonitor's ctor.
    PowerMonitor::AllocateSystemIOPorts();
#endif
    power_monitor_.reset(new PowerMonitor);
  }

  void ProcessPowerEvent(PowerMonitor::PowerEvent event_id) {
    power_monitor_->ProcessPowerEvent(event_id);
  }

  virtual ~PowerMonitorTest() {}

  MessageLoop message_loop_;
  scoped_ptr<PowerMonitor> power_monitor_;

  DISALLOW_COPY_AND_ASSIGN(PowerMonitorTest);
};

TEST_F(PowerMonitorTest, PowerNotifications) {
  const int kObservers = 5;

  PowerTest test[kObservers];
  for (int index = 0; index < kObservers; ++index)
    power_monitor_->AddObserver(&test[index]);

  // Send a bunch of power changes.  Since the battery power hasn't
  // actually changed, we shouldn't get notifications.
  for (int index = 0; index < 5; index++) {
    ProcessPowerEvent(PowerMonitor::POWER_STATE_EVENT);
    EXPECT_EQ(test[0].power_state_changes(), 0);
  }

  // Sending resume when not suspended should have no effect.
  ProcessPowerEvent(PowerMonitor::RESUME_EVENT);
  message_loop_.RunUntilIdle();
  EXPECT_EQ(test[0].resumes(), 0);

  // Pretend we suspended.
  ProcessPowerEvent(PowerMonitor::SUSPEND_EVENT);
  message_loop_.RunUntilIdle();
  EXPECT_EQ(test[0].suspends(), 1);

  // Send a second suspend notification.  This should be suppressed.
  ProcessPowerEvent(PowerMonitor::SUSPEND_EVENT);
  message_loop_.RunUntilIdle();
  EXPECT_EQ(test[0].suspends(), 1);

  // Pretend we were awakened.
  ProcessPowerEvent(PowerMonitor::RESUME_EVENT);
  message_loop_.RunUntilIdle();
  EXPECT_EQ(test[0].resumes(), 1);

  // Send a duplicate resume notification.  This should be suppressed.
  ProcessPowerEvent(PowerMonitor::RESUME_EVENT);
  message_loop_.RunUntilIdle();
  EXPECT_EQ(test[0].resumes(), 1);
}

}  // namespace base
