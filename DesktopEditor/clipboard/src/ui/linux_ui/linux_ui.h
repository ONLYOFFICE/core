// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_LINUX_UI_LINUX_UI_H_
#define UI_LINUX_UI_LINUX_UI_H_

#include "third_party/skia/include/core/SkColor.h"
#include "ui/linux_ui/linux_ui_export.h"
#include "ui/shell_dialogs/linux_shell_dialog.h"

// The main entrypoint into Linux toolkit specific code. GTK code should only
// be executed behind this interface.

namespace gfx {
class Image;
}

namespace ui {
class NativeTheme;

// Adapter class with targets to render like different toolkits. Set by any
// project that wants to do linux desktop native rendering.
//
// TODO(erg): We're hardcoding GTK2, when we'll need to have backends for (at
// minimum) GTK2 and GTK3. LinuxUI::instance() should actually be a very
// complex method that pokes around with dlopen against a libuigtk2.so, a
// liuigtk3.so, etc.
//
// TODO(erg): This class should be folded into ui/views/ instead of living in
// its own component once we've deleted out the GTK+ port.
class LINUX_UI_EXPORT LinuxUI : public LinuxShellDialog {
 public:
  virtual ~LinuxUI() {}

  // Sets the dynamically loaded singleton that draws the desktop native UI.
  static void SetInstance(LinuxUI* instance);

  // Returns a LinuxUI instance for the toolkit used in the user's desktop
  // environment.
  //
  // Can return NULL, in case no toolkit has been set. (For example, if we're
  // running with the "--ash" flag.)
  static const LinuxUI* instance();

  // Returns an themed image per theme_provider.h
  virtual bool UseNativeTheme() const = 0;
  virtual gfx::Image GetThemeImageNamed(int id) const = 0;
  virtual bool GetColor(int id, SkColor* color) const = 0;

  // Returns a NativeTheme that will provide system colors and draw system
  // style widgets.
  virtual NativeTheme* GetNativeTheme() const = 0;
};

}  // namespace ui

#endif  // UI_LINUX_UI_LINUX_UI_H_
