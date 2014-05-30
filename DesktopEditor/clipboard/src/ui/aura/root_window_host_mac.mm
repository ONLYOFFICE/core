// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/root_window_host_mac.h"

#import <Cocoa/Cocoa.h>

#include "base/compiler_specific.h"
#include "base/mac/bundle_locations.h"
#include "base/memory/scoped_nsobject.h"
#include "ui/aura/event.h"
#include "ui/aura/root_window.h"
#include "ui/aura/root_window_host.h"
#include "ui/aura/root_window_mac.h"
#include "ui/aura/root_window_view_mac.h"
#include "ui/base/events/event_utils.h"
#include "ui/gfx/point.h"

namespace aura {

// The Mac-specific implementation of the RootWindowHost interface.  This class
// acts at an intermediary between the Aura shell and an NSWindow.  The
// association between the Aura compositor and the native window's view is
// established with this class as is the association between the native window's
// event dispatch and the Aura event processing.
class RootWindowHostMac : public RootWindowHost,
                          public RootWindowHostMacDelegate {
 public:
  explicit RootWindowHostMac(const gfx::Rect& bounds);
  virtual ~RootWindowHostMac();

  // RootWindowHost:
  virtual void SetRootWindow(RootWindow* root_window) OVERRIDE;
  virtual gfx::AcceleratedWidget GetAcceleratedWidget() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void ToggleFullScreen() OVERRIDE;
  virtual gfx::Size GetSize() const OVERRIDE;
  virtual void SetSize(const gfx::Size& size) OVERRIDE;
  virtual gfx::Point GetLocationOnNativeScreen() const OVERRIDE;
  virtual void SetCapture() OVERRIDE;
  virtual void ReleaseCapture() OVERRIDE;
  virtual void SetCursor(gfx::NativeCursor cursor) OVERRIDE;
  virtual void ShowCursor(bool show) OVERRIDE;
  virtual bool QueryMouseLocation(gfx::Point* location_return) OVERRIDE;
  virtual void MoveCursorTo(const gfx::Point& location) OVERRIDE;
  virtual bool ConfineCursorToRootWindow() OVERRIDE;
  virtual void UnConfineCursor() OVERRIDE;
  virtual bool CopyAreaToSkCanvas(const gfx::Rect& source_bounds,
                                  const gfx::Point& dest_offset,
                                  SkCanvas* canvas) OVERRIDE;
  // RootWindowHostMacDelegate:
  virtual void SendEvent(const base::NativeEvent& native_event) OVERRIDE;

  // Set the initial location of the root window.  The origin of |bounds| is
  // top-left.  This gets converted to bottom-left to match Mac coordinates on
  // the main screen.
  void SetLocation(const gfx::Rect& bounds);

 private:
  // Weak reference.
  RootWindow* root_window_;

  // The bounds of the Aura desktop.  Relative to Aura's coordinate system.
  // This is currently used only for size information, not location.
  gfx::Rect bounds_;

  // An NSWindowController for the root window.  Controls the actual Cocoa
  // window on Mac.
  scoped_nsobject<NSWindowController> controller_;

  DISALLOW_COPY_AND_ASSIGN(RootWindowHostMac);
};

RootWindowHostMacDelegate::RootWindowHostMacDelegate() {
}

RootWindowHostMacDelegate::~RootWindowHostMacDelegate() {
}

RootWindowHostMac::RootWindowHostMac(const gfx::Rect& bounds)
    : root_window_(NULL), bounds_(bounds) {
  NSString* nibpath = [base::mac::FrameworkBundle()
                           pathForResource:@"RootWindow"
                                    ofType:@"nib"];
  NSWindowController* controller = [NSWindowController alloc];
  controller_.reset([controller initWithWindowNibPath:nibpath
                                                owner:controller]);
  SetSize(bounds.size());
  SetLocation(bounds);
}

RootWindowHostMac::~RootWindowHostMac() {
  RootWindowView* view = [[controller_ window] contentView];
  [view setCompositor:NULL];
  [controller_ close];
}

// static
RootWindowHost* RootWindowHost::Create(const gfx::Rect& bounds) {
  return new RootWindowHostMac(bounds);
}

// static
gfx::Size RootWindowHost::GetNativeScreenSize() {
  NSRect screen = [[NSScreen mainScreen] visibleFrame];
  return gfx::Size(NSSizeToCGSize(screen.size));
}

void RootWindowHostMac::SetRootWindow(RootWindow* root_window) {
  root_window_ = root_window;

  RootWindowView* view = [[controller_ window] contentView];
  DCHECK([view respondsToSelector:@selector(setCompositor:)]);
  [view setCompositor:root_window->compositor()];

  RootWindowMac* window = static_cast<RootWindowMac*>([controller_ window]);
  DCHECK([window respondsToSelector:@selector(setHostDelegate:)]);
  [window setHostDelegate:this];
}

gfx::AcceleratedWidget RootWindowHostMac::GetAcceleratedWidget() {
  return [[controller_ window] contentView];
}

void RootWindowHostMac::Show() {
  [controller_ showWindow:controller_];
}

void RootWindowHostMac::ToggleFullScreen() {
}

gfx::Size RootWindowHostMac::GetSize() const {
  NSSize size = [[[controller_ window] contentView] bounds].size;
  return gfx::Size(NSSizeToCGSize(size));
}

void RootWindowHostMac::SetSize(const gfx::Size& size) {
  NSSize nssize = NSSizeFromCGSize(size.ToCGSize());
  [[controller_ window] setContentSize:nssize];
  [[controller_ window] setContentMaxSize:nssize];
  [[controller_ window] setContentMinSize:nssize];
}

gfx::Point RootWindowHostMac::GetLocationOnNativeScreen() const {
  return gfx::Point();
}

void RootWindowHostMac::SetCapture() {
}

void RootWindowHostMac::ReleaseCapture() {
}

void RootWindowHostMac::SetCursor(gfx::NativeCursor cursor) {
}

void RootWindowHostMac::ShowCursor(bool show) {
}

bool RootWindowHostMac::QueryMouseLocation(gfx::Point* location_return) {
  *location_return = gfx::Point();
  return true;
}

void RootWindowHostMac::MoveCursorTo(const gfx::Point& location) {
}

bool RootWindowHostMac::ConfineCursorToRootWindow() {
  return false;
}

void RootWindowHostMac::UnConfineCursor() {
}

void RootWindowHostMac::SendEvent(const base::NativeEvent& native_event) {
  ui::EventType type = ui::EventTypeFromNative(native_event);
  switch (type) {
    case ui::ET_MOUSE_PRESSED:
    case ui::ET_MOUSE_DRAGGED:
    case ui::ET_MOUSE_RELEASED:
    case ui::ET_MOUSE_MOVED:
    case ui::ET_MOUSE_ENTERED:
    case ui::ET_MOUSE_EXITED: {
      MouseEvent mouse_event(native_event);
      root_window_->DispatchMouseEvent(&mouse_event);
      break;
    }
    case ui::ET_KEY_PRESSED:
    case ui::ET_KEY_RELEASED: {
      KeyEvent key_event(native_event, false);
      root_window_->DispatchKeyEvent(&key_event);
      break;
    }
    case ui::ET_MOUSEWHEEL:
    case ui::ET_TOUCH_RELEASED:
    case ui::ET_TOUCH_PRESSED:
    case ui::ET_TOUCH_MOVED:
    case ui::ET_TOUCH_STATIONARY:
    case ui::ET_TOUCH_CANCELLED:
    case ui::ET_DROP_TARGET_EVENT:
    case ui::ET_FOCUS_CHANGE:
    case ui::ET_SCROLL:
    case ui::ET_UNKNOWN:
    default:
      break;
  }
}

void RootWindowHostMac::SetLocation(const gfx::Rect& bounds) {
  NSRect screen = [[NSScreen mainScreen] visibleFrame];
  NSPoint origin = NSMakePoint(screen.origin.x + bounds.x(),
                               screen.origin.y + screen.size.height -
                                   bounds.y() - bounds.height());
  [[controller_ window] setFrameOrigin:origin];
}

}  // namespace aura
