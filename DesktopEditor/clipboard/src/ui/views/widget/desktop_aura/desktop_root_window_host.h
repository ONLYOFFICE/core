// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_ROOT_WINDOW_HOST_H_
#define UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_ROOT_WINDOW_HOST_H_

#include "ui/base/ui_base_types.h"
#include "ui/views/views_export.h"
#include "ui/views/widget/widget.h"

namespace aura {
class RootWindowHost;
class Window;
}

namespace gfx {
class ImageSkia;
class Rect;
}

namespace ui {
class NativeTheme;
}

namespace views {
class DesktopNativeWidgetAura;
namespace internal {
class NativeWidgetDelegate;
}

class VIEWS_EXPORT DesktopRootWindowHost {
 public:
  virtual ~DesktopRootWindowHost() {}

  static DesktopRootWindowHost* Create(
      internal::NativeWidgetDelegate* native_widget_delegate,
      DesktopNativeWidgetAura* desktop_native_widget_aura,
      const gfx::Rect& initial_bounds);

  // Return the NativeTheme to use for |window|.
  static ui::NativeTheme* GetNativeTheme(aura::Window* window);

  // Creates the aura resources associated with the native window we built.
  // Caller takes ownership of returned RootWindow.
  virtual aura::RootWindow* Init(aura::Window* content_window,
                                 const Widget::InitParams& params) = 0;
  virtual void InitFocus(aura::Window* window) = 0;

  virtual void Close() = 0;
  virtual void CloseNow() = 0;

  virtual aura::RootWindowHost* AsRootWindowHost() = 0;

  virtual void ShowWindowWithState(ui::WindowShowState show_state) = 0;
  virtual void ShowMaximizedWithBounds(const gfx::Rect& restored_bounds) = 0;

  virtual bool IsVisible() const = 0;

  virtual void SetSize(const gfx::Size& size) = 0;
  virtual void CenterWindow(const gfx::Size& size) = 0;
  virtual void GetWindowPlacement(gfx::Rect* bounds,
                                  ui::WindowShowState* show_state) const = 0;
  virtual gfx::Rect GetWindowBoundsInScreen() const = 0;
  virtual gfx::Rect GetClientAreaBoundsInScreen() const = 0;
  virtual gfx::Rect GetRestoredBounds() const = 0;

  virtual gfx::Rect GetWorkAreaBoundsInScreen() const = 0;

  virtual void SetShape(gfx::NativeRegion native_region) = 0;

  virtual void Activate() = 0;
  virtual void Deactivate() = 0;
  virtual bool IsActive() const = 0;
  virtual void Maximize() = 0;
  virtual void Minimize() = 0;
  virtual void Restore() = 0;
  virtual bool IsMaximized() const = 0;
  virtual bool IsMinimized() const = 0;

  virtual bool HasCapture() const = 0;

  virtual void SetAlwaysOnTop(bool always_on_top) = 0;

  virtual void SetWindowTitle(const string16& title) = 0;

  virtual void ClearNativeFocus() = 0;

  virtual Widget::MoveLoopResult RunMoveLoop(
      const gfx::Vector2d& drag_offset,
      Widget::MoveLoopSource source) = 0;
  virtual void EndMoveLoop() = 0;

  virtual void SetVisibilityChangedAnimationsEnabled(bool value) = 0;

  virtual bool ShouldUseNativeFrame() = 0;
  virtual void FrameTypeChanged() = 0;
  virtual NonClientFrameView* CreateNonClientFrameView() = 0;

  virtual void SetFullscreen(bool fullscreen) = 0;
  virtual bool IsFullscreen() const = 0;

  virtual void SetOpacity(unsigned char opacity) = 0;

  virtual void SetWindowIcons(const gfx::ImageSkia& window_icon,
                              const gfx::ImageSkia& app_icon) = 0;

  virtual void InitModalType(ui::ModalType modal_type) = 0;

  virtual void FlashFrame(bool flash_frame) = 0;

  // Called when the DesktopNativeWidgetAura's aura::Window is focused and
  // blurred.
  virtual void OnNativeWidgetFocus() = 0;
  virtual void OnNativeWidgetBlur() = 0;

  // Paints the host window as activated depending on the bool passed in.
  virtual void SetInactiveRenderingDisabled(bool disable_inactive) = 0;
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DESKTOP_AURA_DESKTOP_ROOT_WINDOW_HOST_H_
