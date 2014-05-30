// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/screen.h"

#import <ApplicationServices/ApplicationServices.h>
#import <Cocoa/Cocoa.h>

#include "base/logging.h"
#include "ui/gfx/display.h"

#if !defined(MAC_OS_X_VERSION_10_7) || \
    MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_7

@interface NSScreen (LionAPI)
- (CGFloat)backingScaleFactor;
@end

#endif  // 10.7

namespace {

gfx::Rect ConvertCoordinateSystem(NSRect ns_rect) {
  // Primary monitor is defined as the monitor with the menubar,
  // which is always at index 0.
  NSScreen* primary_screen = [[NSScreen screens] objectAtIndex:0];
  float primary_screen_height = [primary_screen frame].size.height;
  gfx::Rect rect(NSRectToCGRect(ns_rect));
  rect.set_y(primary_screen_height - rect.y() - rect.height());
  return rect;
}

NSScreen* GetMatchingScreen(const gfx::Rect& match_rect) {
  // Default to the monitor with the current keyboard focus, in case
  // |match_rect| is not on any screen at all.
  NSScreen* max_screen = [NSScreen mainScreen];
  int max_area = 0;

  for (NSScreen* screen in [NSScreen screens]) {
    gfx::Rect monitor_area = ConvertCoordinateSystem([screen frame]);
    gfx::Rect intersection = gfx::IntersectRects(monitor_area, match_rect);
    int area = intersection.width() * intersection.height();
    if (area > max_area) {
      max_area = area;
      max_screen = screen;
    }
  }

  return max_screen;
}

gfx::Display GetDisplayForScreen(NSScreen* screen, bool is_primary) {
  NSRect frame = [screen frame];
  // TODO(oshima): Implement ID and Observer.
  gfx::Display display(0, gfx::Rect(NSRectToCGRect(frame)));

  NSRect visible_frame = [screen visibleFrame];

  // Convert work area's coordinate systems.
  if (is_primary) {
    gfx::Rect work_area = gfx::Rect(NSRectToCGRect(visible_frame));
    work_area.set_y(frame.size.height - visible_frame.origin.y -
                    visible_frame.size.height);
    display.set_work_area(work_area);
  } else {
    display.set_bounds(ConvertCoordinateSystem(frame));
    display.set_work_area(ConvertCoordinateSystem(visible_frame));
  }
  CGFloat scale;
  if ([screen respondsToSelector:@selector(backingScaleFactor)])
    scale = [screen backingScaleFactor];
  else
    scale = [screen userSpaceScaleFactor];
  display.set_device_scale_factor(scale);
  return display;
}

class ScreenMac : public gfx::Screen {
 public:
  ScreenMac() {}

  virtual bool IsDIPEnabled() OVERRIDE {
    return true;
  }

  virtual gfx::Point GetCursorScreenPoint() OVERRIDE {
    NSPoint mouseLocation  = [NSEvent mouseLocation];
    // Flip coordinates to gfx (0,0 in top-left corner) using primary screen.
    NSScreen* screen = [[NSScreen screens] objectAtIndex:0];
    mouseLocation.y = NSMaxY([screen frame]) - mouseLocation.y;
    return gfx::Point(mouseLocation.x, mouseLocation.y);
  }

  virtual gfx::NativeWindow GetWindowAtCursorScreenPoint() OVERRIDE {
    NOTIMPLEMENTED();
    return gfx::NativeWindow();
  }

  virtual int GetNumDisplays() OVERRIDE {
    // Don't just return the number of online displays.  It includes displays
    // that mirror other displays, which are not desired in the count.  It's
    // tempting to use the count returned by CGGetActiveDisplayList, but active
    // displays exclude sleeping displays, and those are desired in the count.

    // It would be ridiculous to have this many displays connected, but
    // CGDirectDisplayID is just an integer, so supporting up to this many
    // doesn't hurt.
    CGDirectDisplayID online_displays[128];
    CGDisplayCount online_display_count = 0;
    if (CGGetOnlineDisplayList(arraysize(online_displays),
                              online_displays,
                              &online_display_count) != kCGErrorSuccess) {
      // 1 is a reasonable assumption.
      return 1;
    }

    int display_count = 0;
    for (CGDisplayCount online_display_index = 0;
        online_display_index < online_display_count;
        ++online_display_index) {
      CGDirectDisplayID online_display = online_displays[online_display_index];
      if (CGDisplayMirrorsDisplay(online_display) == kCGNullDirectDisplay) {
        // If this display doesn't mirror any other, include it in the count.
        // The primary display in a mirrored set will be counted, but those that
        // mirror it will not be.
        ++display_count;
      }
    }

    return display_count;
  }

  virtual gfx::Display GetDisplayNearestWindow(
      gfx::NativeView view) const OVERRIDE {
    NSWindow* window = [view window];
    if (!window)
      return GetPrimaryDisplay();
    NSScreen* match_screen = [window screen];
    return GetDisplayForScreen(match_screen, false /* may not be primary */);
  }

  virtual gfx::Display GetDisplayNearestPoint(
      const gfx::Point& point) const OVERRIDE {
    NSPoint ns_point = NSPointFromCGPoint(point.ToCGPoint());

    NSArray* screens = [NSScreen screens];
    NSScreen* primary = [screens objectAtIndex:0];
    ns_point.y = NSMaxY([primary frame]) - ns_point.y;
    for (NSScreen* screen in screens) {
      if (NSMouseInRect(ns_point, [screen frame], NO))
        return GetDisplayForScreen(screen, screen == primary);
    }
    return GetPrimaryDisplay();
  }

  // Returns the display that most closely intersects the provided bounds.
  virtual gfx::Display GetDisplayMatching(
      const gfx::Rect& match_rect) const OVERRIDE {
    NSScreen* match_screen = GetMatchingScreen(match_rect);
    return GetDisplayForScreen(match_screen, false /* may not be primary */);
  }

  // Returns the primary display.
  virtual gfx::Display GetPrimaryDisplay() const OVERRIDE {
    // Primary display is defined as the display with the menubar,
    // which is always at index 0.
    NSScreen* primary = [[NSScreen screens] objectAtIndex:0];
    gfx::Display display = GetDisplayForScreen(primary, true /* primary */);
    return display;
  }

  virtual void AddObserver(gfx::DisplayObserver* observer) OVERRIDE {
    // TODO(oshima): crbug.com/122863.
  }

  virtual void RemoveObserver(gfx::DisplayObserver* observer) OVERRIDE {
    // TODO(oshima): crbug.com/122863.
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ScreenMac);
};

}  // namespace

namespace gfx {

Screen* CreateNativeScreen() {
  return new ScreenMac;
}

}
