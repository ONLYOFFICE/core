// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/ime/text_input_test_support.h"

#if defined(OS_CHROMEOS)
#include "base/chromeos/chromeos_version.h"
#include "base/logging.h"
#include "chromeos/dbus/dbus_thread_manager.h"
#include "chromeos/ime/ibus_daemon_controller.h"
#endif  // OS_CHROMEOS

#if defined(OS_CHROMEOS)
namespace {
bool dbus_thread_manager_was_initialized = false;
}
#endif  // OS_CHROMEOS

namespace ui {

void TextInputTestSupport::Initialize() {
#if defined(OS_CHROMEOS)
  if (!chromeos::DBusThreadManager::IsInitialized()) {
    chromeos::DBusThreadManager::InitializeWithStub();
    dbus_thread_manager_was_initialized = true;
  }
  if (!chromeos::IBusDaemonController::GetInstance()) {
    // Passing NULL is okay because IBusDaemonController will be initialized
    // with stub implementation on non-ChromeOS device.
    DCHECK(!base::chromeos::IsRunningOnChromeOS());
    chromeos::IBusDaemonController::Initialize(NULL, NULL);
  }
#endif  // OS_CHROMEOS
}

void TextInputTestSupport::Shutdown() {
#if defined(OS_CHROMEOS)
  if (dbus_thread_manager_was_initialized) {
    chromeos::DBusThreadManager::Shutdown();
    dbus_thread_manager_was_initialized = false;
  }
  if (chromeos::IBusDaemonController::GetInstance())
    chromeos::IBusDaemonController::Shutdown();
#endif  // OS_CHROMEOS
}

}  // namespace ui
