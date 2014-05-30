// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/events/event_utils.h"
#include "ui/base/test/cocoa_test_event_utils.h"
#import "ui/base/test/ui_cocoa_test_helper.h"
#include "ui/gfx/point.h"

using namespace cocoa_test_event_utils;

namespace {

class EventsMacTest : public ui::CocoaTest {
 public:
  EventsMacTest() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(EventsMacTest);
};

TEST_F(EventsMacTest, EventTypeFromNative) {
  NSEvent* native_event = nil;
  ui::EventType type = ui::ET_UNKNOWN;

  native_event = MouseEventWithType(NSLeftMouseDown, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_MOUSE_PRESSED, type);

  native_event = MouseEventWithType(NSLeftMouseUp, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_MOUSE_RELEASED, type);

  native_event = MouseEventWithType(NSRightMouseDown, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_MOUSE_PRESSED, type);

  native_event = MouseEventWithType(NSRightMouseUp, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_MOUSE_RELEASED, type);

  native_event = MouseEventWithType(NSMouseMoved, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_MOUSE_MOVED, type);

  native_event = MouseEventWithType(NSLeftMouseDragged, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_MOUSE_DRAGGED, type);

  native_event = MouseEventWithType(NSRightMouseDragged, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_MOUSE_DRAGGED, type);

  native_event = EnterExitEventWithType(NSMouseEntered);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_MOUSE_ENTERED, type);

  native_event = EnterExitEventWithType(NSMouseExited);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_MOUSE_EXITED, type);

  native_event = KeyEventWithType(NSKeyDown, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_KEY_PRESSED, type);

  native_event = KeyEventWithType(NSKeyUp, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_KEY_RELEASED, type);

  native_event = KeyEventWithType(NSFlagsChanged, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_KEY_PRESSED, type);

  native_event = OtherEventWithType(NSAppKitDefined);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_UNKNOWN, type);

  native_event = OtherEventWithType(NSSystemDefined);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_UNKNOWN, type);

  native_event = OtherEventWithType(NSApplicationDefined);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_UNKNOWN, type);

  native_event = OtherEventWithType(NSPeriodic);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_UNKNOWN, type);

  native_event = EnterExitEventWithType(NSCursorUpdate);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_UNKNOWN, type);

  native_event = MouseEventWithType(NSOtherMouseDown, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_MOUSE_PRESSED, type);

  native_event = MouseEventWithType(NSOtherMouseUp, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_MOUSE_RELEASED, type);

  native_event = MouseEventWithType(NSOtherMouseDragged, 0);
  type = ui::EventTypeFromNative(native_event);
  EXPECT_EQ(ui::ET_MOUSE_DRAGGED, type);
}

TEST_F(EventsMacTest, EventFlagsFromNative) {
  NSEvent* native_event = nil;
  int flags = 0;

  // No key flags.
  native_event = KeyEventWithType(NSKeyDown, 0);
  flags = ui::EventFlagsFromNative(native_event);
  EXPECT_EQ(0, flags);

  // All key flags.
  native_event = KeyEventWithType(NSKeyDown,
                                  NSAlphaShiftKeyMask |
                                      NSShiftKeyMask |
                                      NSControlKeyMask |
                                      NSAlternateKeyMask |
                                      NSCommandKeyMask);
  flags = ui::EventFlagsFromNative(native_event);
  EXPECT_EQ(ui::EF_CAPS_LOCK_DOWN |
                ui::EF_SHIFT_DOWN |
                ui::EF_CONTROL_DOWN |
                ui::EF_ALT_DOWN |
                ui::EF_COMMAND_DOWN,
            flags);

  // Left mouse flags.
  native_event = MouseEventWithType(NSLeftMouseDown, 0);
  flags = ui::EventFlagsFromNative(native_event);
  EXPECT_EQ(ui::EF_LEFT_MOUSE_BUTTON, flags);

  // Right mouse flags.
  native_event = MouseEventWithType(NSRightMouseDown, 0);
  flags = ui::EventFlagsFromNative(native_event);
  EXPECT_EQ(ui::EF_RIGHT_MOUSE_BUTTON, flags);

  // Center mouse flags.
  native_event = MouseEventWithType(NSOtherMouseDown, 0);
  flags = ui::EventFlagsFromNative(native_event);
  EXPECT_EQ(ui::EF_MIDDLE_MOUSE_BUTTON, flags);
}

TEST_F(EventsMacTest, EventLocationFromNative) {
  NSWindow* window = [[NSWindow alloc]
                       initWithContentRect:NSMakeRect(10, 25, 200, 205)
                                 styleMask:NSTitledWindowMask
                                   backing:NSBackingStoreBuffered
                                     defer:NO];
  [window orderFront:nil];

  // Construct mouse event from window number having position at bottom-left.
  // Cocoa origin is at bottom-left.  ui::Event origin is top-left.
  NSEvent* native_event =
      [NSEvent mouseEventWithType:NSLeftMouseDown
                         location:NSMakePoint(10, 15)
                    modifierFlags:0
                        timestamp:[NSDate timeIntervalSinceReferenceDate]
                     windowNumber:[window windowNumber]
                          context:[window graphicsContext]
                      eventNumber:0
                       clickCount:1
                         pressure:0.0f];

  // Expect resulting event to be positioned relative to top-left.
  gfx::Point location = ui::EventLocationFromNative(native_event);
  EXPECT_EQ(gfx::Point(10, 190), location);

  [window orderOut:nil];
  [window close];
}

TEST_F(EventsMacTest, KeyboardCodeFromNative) {
  NSEvent* native_event = nil;
  ui::KeyboardCode code = ui::VKEY_UNKNOWN;

  // Simple "x".  No modifiers.
  native_event =
      [NSEvent keyEventWithType:NSKeyDown
                       location:NSZeroPoint
                  modifierFlags:0
                      timestamp:[NSDate timeIntervalSinceReferenceDate]
                   windowNumber:0
                        context:nil
                     characters:@"x"
               charactersIgnoringModifiers:@"x"
                      isARepeat:NO
                        keyCode:0x58];
  code = ui::KeyboardCodeFromNative(native_event);
  EXPECT_EQ(ui::VKEY_X, code);

  // Option/Alt "x".
  native_event =
      [NSEvent keyEventWithType:NSKeyDown
                       location:NSZeroPoint
                  modifierFlags:NSAlternateKeyMask
                      timestamp:[NSDate timeIntervalSinceReferenceDate]
                   windowNumber:0
                        context:nil
                     characters:@"\0x2248" // opt-x
               charactersIgnoringModifiers:@"x"
                      isARepeat:NO
                        keyCode:0x58];
  code = ui::KeyboardCodeFromNative(native_event);
  EXPECT_EQ(ui::VKEY_X, code);

  // Option/Alt (alone).
  native_event =
      [NSEvent keyEventWithType:NSFlagsChanged
                       location:NSZeroPoint
                  modifierFlags:NSAlternateKeyMask
                      timestamp:[NSDate timeIntervalSinceReferenceDate]
                   windowNumber:0
                        context:nil
                     characters:@""
               charactersIgnoringModifiers:@""
                      isARepeat:NO
                        keyCode:0x3a];
  code = ui::KeyboardCodeFromNative(native_event);
  EXPECT_EQ(ui::VKEY_MENU, code);
}

TEST_F(EventsMacTest, IsMouseEvent) {
  NSEvent* native_event = nil;

  native_event = MouseEventWithType(NSLeftMouseDown, 0);
  EXPECT_TRUE(ui::IsMouseEvent(native_event));

  native_event = MouseEventWithType(NSLeftMouseUp, 0);
  EXPECT_TRUE(ui::IsMouseEvent(native_event));

  native_event = MouseEventWithType(NSRightMouseDown, 0);
  EXPECT_TRUE(ui::IsMouseEvent(native_event));

  native_event = MouseEventWithType(NSRightMouseUp, 0);
  EXPECT_TRUE(ui::IsMouseEvent(native_event));

  native_event = MouseEventWithType(NSMouseMoved, 0);
  EXPECT_TRUE(ui::IsMouseEvent(native_event));

  native_event = MouseEventWithType(NSLeftMouseDragged, 0);
  EXPECT_TRUE(ui::IsMouseEvent(native_event));

  native_event = MouseEventWithType(NSRightMouseDragged, 0);
  EXPECT_TRUE(ui::IsMouseEvent(native_event));

  native_event = EnterExitEventWithType(NSMouseEntered);
  EXPECT_TRUE(ui::IsMouseEvent(native_event));

  native_event = EnterExitEventWithType(NSMouseExited);
  EXPECT_TRUE(ui::IsMouseEvent(native_event));

  native_event = KeyEventWithType(NSKeyDown, 0);
  EXPECT_FALSE(ui::IsMouseEvent(native_event));

  native_event = KeyEventWithType(NSKeyUp, 0);
  EXPECT_FALSE(ui::IsMouseEvent(native_event));

  native_event = KeyEventWithType(NSFlagsChanged, 0);
  EXPECT_FALSE(ui::IsMouseEvent(native_event));

  native_event = OtherEventWithType(NSAppKitDefined);
  EXPECT_FALSE(ui::IsMouseEvent(native_event));

  native_event = OtherEventWithType(NSSystemDefined);
  EXPECT_FALSE(ui::IsMouseEvent(native_event));

  native_event = OtherEventWithType(NSApplicationDefined);
  EXPECT_FALSE(ui::IsMouseEvent(native_event));

  native_event = OtherEventWithType(NSPeriodic);
  EXPECT_FALSE(ui::IsMouseEvent(native_event));

  native_event = EnterExitEventWithType(NSCursorUpdate);
  EXPECT_FALSE(ui::IsMouseEvent(native_event));

  native_event = MouseEventWithType(NSOtherMouseDown, 0);
  EXPECT_TRUE(ui::IsMouseEvent(native_event));

  native_event = MouseEventWithType(NSOtherMouseUp, 0);
  EXPECT_TRUE(ui::IsMouseEvent(native_event));

  native_event = MouseEventWithType(NSOtherMouseDragged, 0);
  EXPECT_TRUE(ui::IsMouseEvent(native_event));
}

TEST_F(EventsMacTest, CreateNoopEvent) {
  NSEvent* native_event = ui::CreateNoopEvent();
  EXPECT_TRUE(native_event != nil);
}

}  // namespace
