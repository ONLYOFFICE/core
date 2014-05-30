// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Cocoa/Cocoa.h>

#include "ui/base/events/event_constants.h"

#include "base/event_types.h"
#include "base/logging.h"
#include "base/time.h"
#include "ui/base/events/event_utils.h"
#import "ui/base/keycodes/keyboard_code_conversion_mac.h"
#include "ui/gfx/point.h"

namespace ui {

EventType EventTypeFromNative(const base::NativeEvent& native_event) {
  NSEventType native_type = [native_event type];
  switch (native_type) {
    case NSLeftMouseDown:
    case NSRightMouseDown:
    case NSOtherMouseDown:
      return ET_MOUSE_PRESSED;

    case NSLeftMouseUp:
    case NSRightMouseUp:
    case NSOtherMouseUp:
      return ET_MOUSE_RELEASED;

    case NSMouseMoved:
      return ET_MOUSE_MOVED;

    case NSLeftMouseDragged:
    case NSRightMouseDragged:
    case NSOtherMouseDragged:
      return ET_MOUSE_DRAGGED;

    case NSMouseEntered:
      return ET_MOUSE_ENTERED;

    case NSMouseExited:
      return ET_MOUSE_EXITED;

    case NSKeyDown:
      return ET_KEY_PRESSED;

    case NSKeyUp:
      return ET_KEY_RELEASED;

    case NSFlagsChanged:
      return ET_KEY_PRESSED;

    case NSScrollWheel:
      return ET_MOUSEWHEEL;

    case NSAppKitDefined:
    case NSSystemDefined:
    case NSApplicationDefined:
    case NSPeriodic:
    case NSCursorUpdate:
    case NSTabletPoint:
    case NSTabletProximity:
    default:
      return ET_UNKNOWN;
  }
}

int EventFlagsFromNative(const base::NativeEvent& native_event) {
  int event_flags = 0;
  NSUInteger modifiers = [native_event modifierFlags];

  if (modifiers & NSAlphaShiftKeyMask)
    event_flags = event_flags | EF_CAPS_LOCK_DOWN;

  if (modifiers & NSShiftKeyMask)
    event_flags = event_flags | EF_SHIFT_DOWN;

  if (modifiers & NSControlKeyMask)
    event_flags = event_flags | EF_CONTROL_DOWN;

  if (modifiers & NSAlternateKeyMask)
    event_flags = event_flags | EF_ALT_DOWN;

  if (modifiers & NSCommandKeyMask)
    event_flags = event_flags | EF_COMMAND_DOWN;

  NSEventType type = [native_event type];

  if (type == NSLeftMouseDown ||
      type == NSLeftMouseUp ||
      type == NSLeftMouseDragged) {
    event_flags = event_flags | EF_LEFT_MOUSE_BUTTON;
  }

  if (type == NSRightMouseDown ||
      type == NSRightMouseUp ||
      type == NSRightMouseDragged) {
    event_flags = event_flags | EF_RIGHT_MOUSE_BUTTON;
  }

  if (type == NSOtherMouseDown ||
      type == NSOtherMouseUp ||
      type == NSOtherMouseDragged) {
    event_flags = event_flags | EF_MIDDLE_MOUSE_BUTTON;
  }

  return event_flags;
}

base::TimeDelta EventTimeFromNative(const base::NativeEvent& native_event) {
  return base::TimeDelta::FromMicroseconds(
      [native_event timestamp] * 1000000.0f);
}

gfx::Point EventLocationFromNative(const base::NativeEvent& native_event) {
  NSWindow* window = [native_event window];
  NSPoint location = [native_event locationInWindow];

  // Convert |location| to be relative to coordinate system of |contentView|.
  // Note: this assumes that ui::Event coordinates are rooted in the top-level
  // view (with flipped coordinates).  A more general (but costly) approach
  // would be to hit-test the view of the event and use the found view's
  // coordinate system.  Currently there is no need for this generality, and
  // speed is preferred.  Flipped views are not suppported.
  DCHECK([[window contentView] isFlipped] == NO);
  location = [[window contentView] convertPoint:location fromView:nil];
  location.y = [[window contentView] bounds].size.height - location.y;

  return gfx::Point(NSPointToCGPoint(location));
}

gfx::Point EventSystemLocationFromNative(
    const base::NativeEvent& native_event) {
  // TODO(port): Needs to always return screen position here. Returning normal
  // origin for now since that's obviously wrong.
  return gfx::Point(0, 0);
}

KeyboardCode KeyboardCodeFromNative(const base::NativeEvent& native_event) {
  return ui::KeyboardCodeFromNSEvent(native_event);
}

bool IsMouseEvent(const base::NativeEvent& native_event) {
  EventType type = EventTypeFromNative(native_event);
  return type == ET_MOUSE_PRESSED ||
         type == ET_MOUSE_DRAGGED ||
         type == ET_MOUSE_RELEASED ||
         type == ET_MOUSE_MOVED ||
         type == ET_MOUSE_ENTERED ||
         type == ET_MOUSE_EXITED;
}

gfx::Vector2d GetMouseWheelOffset(const base::NativeEvent& native_event) {
  // TODO(dhollowa): Come back to this once comparisons can be made with other
  // platforms.
  return gfx::Vector2d([native_event deltaX], [native_event deltaY]);
}

int GetTouchId(const base::NativeEvent& native_event) {
  // Touch is currently unsupported.
  return 0;
}

float GetTouchRadiusX(const base::NativeEvent& native_event) {
  // Touch is currently unsupported.
  return 1.0;
}

float GetTouchRadiusY(const base::NativeEvent& native_event) {
  // Touch is currently unsupported.
  return 1.0;
}

float GetTouchAngle(const base::NativeEvent& native_event) {
  // Touch is currently unsupported.
  return 0.0;
}

float GetTouchForce(const base::NativeEvent& native_event) {
  // Touch is currently unsupported.
  return 0.0;
}

bool GetScrollOffsets(const base::NativeEvent& native_event,
                      float* x_offset,
                      float* y_offset,
                      int* finger_count) {
  return false;
}

bool IsNoopEvent(const base::NativeEvent& event) {
  return ([event type] == NSApplicationDefined && [event subtype] == 0);
}

base::NativeEvent CreateNoopEvent() {
  return [NSEvent otherEventWithType:NSApplicationDefined
                            location:NSZeroPoint
                       modifierFlags:0
                           timestamp:[NSDate timeIntervalSinceReferenceDate]
                        windowNumber:0
                             context:nil
                             subtype:0
                               data1:0
                               data2:0];
}

}  // namespace ui
