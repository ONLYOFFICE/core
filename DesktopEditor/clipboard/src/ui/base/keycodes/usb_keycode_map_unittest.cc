// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <map>

#include "base/basictypes.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {

#if defined(OS_WIN)
const size_t kExpectedMappedKeyCount = 138;
#define USB_KEYMAP(usb, xkb, win, mac) {usb, win}
#elif defined(OS_LINUX)
const size_t kExpectedMappedKeyCount = 145;
#define USB_KEYMAP(usb, xkb, win, mac) {usb, xkb}
#elif defined(OS_MACOSX)
const size_t kExpectedMappedKeyCount = 118;
#define USB_KEYMAP(usb, xkb, win, mac) {usb, mac}
#else
const size_t kExpectedMappedKeyCount = 0;
#define USB_KEYMAP(usb, xkb, win, mac) {usb, 0}
#endif
#include "ui/base/keycodes/usb_keycode_map.h"
#undef USB_KEYMAP

const uint32_t kUsbNonExistentKeycode = 0xffffff;
const uint32_t kUsbUsBackslash =        0x070031;
const uint32_t kUsbNonUsHash =          0x070032;

TEST(UsbKeycodeMap, Basic) {
  // Verify that the first element in the table is the "invalid" code.
  EXPECT_EQ(InvalidUsbKeycode(), usb_keycode_map[0].usb_keycode);
  EXPECT_EQ(InvalidNativeKeycode(), usb_keycode_map[0].native_keycode);

  // Verify that there are no duplicate entries in the mapping.
  std::map<uint32_t, uint16_t> usb_to_native;
  std::map<uint16_t, uint32_t> native_to_usb;
  for (size_t i = 0; i < arraysize(usb_keycode_map); ++i) {
    // Don't test keys with no native keycode mapping on this platform.
    if (usb_keycode_map[i].native_keycode == InvalidNativeKeycode())
      continue;

    // Verify UsbKeycodeToNativeKeycode works for this key.
    EXPECT_EQ(usb_keycode_map[i].native_keycode,
              UsbKeycodeToNativeKeycode(usb_keycode_map[i].usb_keycode));

    // Verify that the USB or native codes aren't duplicated.
    EXPECT_EQ(0U, usb_to_native.count(usb_keycode_map[i].usb_keycode))
        << " duplicate of USB code 0x" << std::hex << std::setfill('0')
        << std::setw(6) << usb_keycode_map[i].usb_keycode
        << " to native 0x"
        << std::setw(4) << usb_keycode_map[i].native_keycode
        << " (previous was 0x"
        << std::setw(4) << usb_to_native[usb_keycode_map[i].usb_keycode]
        << ")";
    usb_to_native[usb_keycode_map[i].usb_keycode] =
        usb_keycode_map[i].native_keycode;
    EXPECT_EQ(0U, native_to_usb.count(usb_keycode_map[i].native_keycode))
        << " duplicate of native code 0x" << std::hex << std::setfill('0')
        << std::setw(4) << usb_keycode_map[i].native_keycode
        << " to USB 0x"
        << std::setw(6) << usb_keycode_map[i].usb_keycode
        << " (previous was 0x"
        << std::setw(6) << native_to_usb[usb_keycode_map[i].native_keycode]
        << ")";
    native_to_usb[usb_keycode_map[i].native_keycode] =
        usb_keycode_map[i].usb_keycode;
  }
  ASSERT_EQ(usb_to_native.size(), native_to_usb.size());

  // Verify that the number of mapped keys is what we expect, i.e. we haven't
  // lost any, and if we've added some then the expectation has been updated.
  EXPECT_EQ(kExpectedMappedKeyCount, usb_to_native.size());
}

TEST(UsbKeycodeMap, NonExistent) {
  // Verify that UsbKeycodeToNativeKeycode works for a non-existent USB keycode.
  EXPECT_EQ(InvalidNativeKeycode(),
      UsbKeycodeToNativeKeycode(kUsbNonExistentKeycode));
}

TEST(UsbKeycodeMap, UsBackslashIsNonUsHash) {
  // Verify that UsbKeycodeToNativeKeycode treats the non-US "hash" key
  // as equivalent to the US "backslash" key.
  EXPECT_EQ(UsbKeycodeToNativeKeycode(kUsbUsBackslash),
            UsbKeycodeToNativeKeycode(kUsbNonUsHash));

}

}  // namespace
