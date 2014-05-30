// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/desktop_screen.h"

#include <X11/Xlib.h>

// It clashes with out RootWindow.
#undef RootWindow

#include "base/logging.h"
#include "ui/aura/root_window.h"
#include "ui/aura/root_window_host.h"
#include "ui/base/x/x11_util.h"
#include "ui/gfx/display.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/screen.h"

namespace {

// TODO(erg): This method is a temporary hack, until we can reliably extract
// location data out of XRandR.
gfx::Size GetPrimaryDisplaySize() {
  ::Display* display = ui::GetXDisplay();
  ::Screen* screen = DefaultScreenOfDisplay(display);
  int width = WidthOfScreen(screen);
  int height = HeightOfScreen(screen);

  return gfx::Size(width, height);
}

class DesktopScreenX11 : public gfx::Screen {
 public:
  DesktopScreenX11();
  virtual ~DesktopScreenX11();

  // Overridden from gfx::Screen:
  virtual bool IsDIPEnabled() OVERRIDE;
  virtual gfx::Point GetCursorScreenPoint() OVERRIDE;
  virtual gfx::NativeWindow GetWindowAtCursorScreenPoint() OVERRIDE;
  virtual int GetNumDisplays() OVERRIDE;
  virtual gfx::Display GetDisplayNearestWindow(
      gfx::NativeView window) const OVERRIDE;
  virtual gfx::Display GetDisplayNearestPoint(
      const gfx::Point& point) const OVERRIDE;
  virtual gfx::Display GetDisplayMatching(
      const gfx::Rect& match_rect) const OVERRIDE;
  virtual gfx::Display GetPrimaryDisplay() const OVERRIDE;
  virtual void AddObserver(gfx::DisplayObserver* observer) OVERRIDE;
  virtual void RemoveObserver(gfx::DisplayObserver* observer) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(DesktopScreenX11);
};

////////////////////////////////////////////////////////////////////////////////
// DesktopScreenX11, public:

DesktopScreenX11::DesktopScreenX11() {
}

DesktopScreenX11::~DesktopScreenX11() {
}

////////////////////////////////////////////////////////////////////////////////
// DesktopScreenX11, gfx::Screen implementation:

bool DesktopScreenX11::IsDIPEnabled() {
  return false;
}

gfx::Point DesktopScreenX11::GetCursorScreenPoint() {
  Display* display = ui::GetXDisplay();

  ::Window root, child;
  int root_x, root_y, win_x, win_y;
  unsigned int mask;
  XQueryPointer(display,
                DefaultRootWindow(display),
                &root,
                &child,
                &root_x,
                &root_y,
                &win_x,
                &win_y,
                &mask);

  return gfx::Point(root_x, root_y);
}

gfx::NativeWindow DesktopScreenX11::GetWindowAtCursorScreenPoint() {
  // TODO(erg): Implement using the discussion at
  // http://codereview.chromium.org/10279005/
  return NULL;
}

int DesktopScreenX11::GetNumDisplays() {
  // TODO(erg): Figure this out with oshima or piman because I have no clue
  // about the XRandR implications here.
  return 1;
}

gfx::Display DesktopScreenX11::GetDisplayNearestWindow(
    gfx::NativeView window) const {
  // TODO(erg): Do the right thing once we know what that is.
  return gfx::Display(0, gfx::Rect(GetPrimaryDisplaySize()));
}

gfx::Display DesktopScreenX11::GetDisplayNearestPoint(
    const gfx::Point& point) const {
  // TODO(erg): Do the right thing once we know what that is.
  return gfx::Display(0, gfx::Rect(GetPrimaryDisplaySize()));
}

gfx::Display DesktopScreenX11::GetDisplayMatching(
    const gfx::Rect& match_rect) const {
  // TODO(erg): Do the right thing once we know what that is.
  return gfx::Display(0, gfx::Rect(GetPrimaryDisplaySize()));
}

gfx::Display DesktopScreenX11::GetPrimaryDisplay() const {
  // TODO(erg): Do the right thing once we know what that is.
  return gfx::Display(0, gfx::Rect(GetPrimaryDisplaySize()));
}

void DesktopScreenX11::AddObserver(gfx::DisplayObserver* observer) {
  // TODO(erg|oshima): Do the right thing once we know what that is.
  // crbug.com/122863
}
void DesktopScreenX11::RemoveObserver(gfx::DisplayObserver* observer) {
  // TODO(erg|oshima): Do the right thing once we know what that is.
  // crbug.com/122863
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////

namespace views {

gfx::Screen* CreateDesktopScreen() {
  return new DesktopScreenX11;
}

}  // namespace views
