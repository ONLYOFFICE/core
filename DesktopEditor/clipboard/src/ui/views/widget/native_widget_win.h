// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_NATIVE_WIDGET_WIN_H_
#define UI_VIEWS_WIDGET_NATIVE_WIDGET_WIN_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/win/scoped_comptr.h"
#include "base/win/win_util.h"
#include "ui/base/win/window_impl.h"
#include "ui/views/widget/native_widget_private.h"
#include "ui/views/win/hwnd_message_handler_delegate.h"

namespace ui {
class Compositor;
class ViewProp;
}

namespace gfx {
class Canvas;
class Font;
class Rect;
}

namespace views {

class DropTargetWin;
class HWNDMessageHandler;
class InputMethodDelegate;
class RootView;
class TooltipManagerWin;

///////////////////////////////////////////////////////////////////////////////
//
// NativeWidgetWin
//  A Widget for a views hierarchy used to represent anything that can be
//  contained within an HWND, e.g. a control, a window, etc. Specializations
//  suitable for specific tasks, e.g. top level window, are derived from this.
//
//  This Widget contains a RootView which owns the hierarchy of views within it.
//  As long as views are part of this tree, they will be deleted automatically
//  when the RootView is destroyed. If you remove a view from the tree, you are
//  then responsible for cleaning up after it.
//
///////////////////////////////////////////////////////////////////////////////
class VIEWS_EXPORT NativeWidgetWin : public internal::NativeWidgetPrivate,
                                     public HWNDMessageHandlerDelegate {
 public:
  explicit NativeWidgetWin(internal::NativeWidgetDelegate* delegate);
  virtual ~NativeWidgetWin();

  // Returns the system set window title font.
  static gfx::Font GetWindowTitleFont();

  // Show the window with the specified show command.
  void Show(int show_state);

  // Places the window in a pseudo-fullscreen mode where it looks and acts as
  // like a fullscreen window except that it remains within the boundaries
  // of the metro snap divider.
  void SetMetroSnapFullscreen(bool metro_snap);
  bool IsInMetroSnapMode() const;

  void SetCanUpdateLayeredWindow(bool can_update);

  // Overridden from internal::NativeWidgetPrivate:
  virtual void InitNativeWidget(const Widget::InitParams& params) OVERRIDE;
  virtual NonClientFrameView* CreateNonClientFrameView() OVERRIDE;
  virtual bool ShouldUseNativeFrame() const OVERRIDE;
  virtual void FrameTypeChanged() OVERRIDE;
  virtual Widget* GetWidget() OVERRIDE;
  virtual const Widget* GetWidget() const OVERRIDE;
  virtual gfx::NativeView GetNativeView() const OVERRIDE;
  virtual gfx::NativeWindow GetNativeWindow() const OVERRIDE;
  virtual Widget* GetTopLevelWidget() OVERRIDE;
  virtual const ui::Compositor* GetCompositor() const OVERRIDE;
  virtual ui::Compositor* GetCompositor() OVERRIDE;
  virtual gfx::Vector2d CalculateOffsetToAncestorWithLayer(
      ui::Layer** layer_parent) OVERRIDE;
  virtual void ViewRemoved(View* view) OVERRIDE;
  virtual void SetNativeWindowProperty(const char* name, void* value) OVERRIDE;
  virtual void* GetNativeWindowProperty(const char* name) const OVERRIDE;
  virtual TooltipManager* GetTooltipManager() const OVERRIDE;
  virtual void SetCapture() OVERRIDE;
  virtual void ReleaseCapture() OVERRIDE;
  virtual bool HasCapture() const OVERRIDE;
  virtual InputMethod* CreateInputMethod() OVERRIDE;
  virtual internal::InputMethodDelegate* GetInputMethodDelegate() OVERRIDE;
  virtual void CenterWindow(const gfx::Size& size) OVERRIDE;
  virtual void GetWindowPlacement(
     gfx::Rect* bounds,
     ui::WindowShowState* show_state) const OVERRIDE;
  virtual void SetWindowTitle(const string16& title) OVERRIDE;
  virtual void SetWindowIcons(const gfx::ImageSkia& window_icon,
                              const gfx::ImageSkia& app_icon) OVERRIDE;
  virtual void InitModalType(ui::ModalType modal_type) OVERRIDE;
  virtual gfx::Rect GetWindowBoundsInScreen() const OVERRIDE;
  virtual gfx::Rect GetClientAreaBoundsInScreen() const OVERRIDE;
  virtual gfx::Rect GetRestoredBounds() const OVERRIDE;
  virtual void SetBounds(const gfx::Rect& bounds) OVERRIDE;
  virtual void SetSize(const gfx::Size& size) OVERRIDE;
  virtual void StackAbove(gfx::NativeView native_view) OVERRIDE;
  virtual void StackAtTop() OVERRIDE;
  virtual void StackBelow(gfx::NativeView native_view) OVERRIDE;
  virtual void SetShape(gfx::NativeRegion shape) OVERRIDE;
  virtual void Close() OVERRIDE;
  virtual void CloseNow() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual void ShowMaximizedWithBounds(
      const gfx::Rect& restored_bounds) OVERRIDE;
  virtual void ShowWithWindowState(ui::WindowShowState show_state) OVERRIDE;
  virtual bool IsVisible() const OVERRIDE;
  virtual void Activate() OVERRIDE;
  virtual void Deactivate() OVERRIDE;
  virtual bool IsActive() const OVERRIDE;
  virtual void SetAlwaysOnTop(bool always_on_top) OVERRIDE;
  virtual void Maximize() OVERRIDE;
  virtual void Minimize() OVERRIDE;
  virtual bool IsMaximized() const OVERRIDE;
  virtual bool IsMinimized() const OVERRIDE;
  virtual void Restore() OVERRIDE;
  virtual void SetFullscreen(bool fullscreen) OVERRIDE;
  virtual bool IsFullscreen() const OVERRIDE;
  virtual void SetOpacity(unsigned char opacity) OVERRIDE;
  virtual void SetUseDragFrame(bool use_drag_frame) OVERRIDE;
  virtual void FlashFrame(bool flash) OVERRIDE;
  virtual void RunShellDrag(View* view,
                            const ui::OSExchangeData& data,
                            const gfx::Point& location,
                            int operation,
                            ui::DragDropTypes::DragEventSource source) OVERRIDE;
  virtual void SchedulePaintInRect(const gfx::Rect& rect) OVERRIDE;
  virtual void SetCursor(gfx::NativeCursor cursor) OVERRIDE;
  virtual void ClearNativeFocus() OVERRIDE;
  virtual gfx::Rect GetWorkAreaBoundsInScreen() const OVERRIDE;
  virtual void SetInactiveRenderingDisabled(bool value) OVERRIDE;
  virtual Widget::MoveLoopResult RunMoveLoop(
      const gfx::Vector2d& drag_offset,
      Widget::MoveLoopSource source) OVERRIDE;
  virtual void EndMoveLoop() OVERRIDE;
  virtual void SetVisibilityChangedAnimationsEnabled(bool value) OVERRIDE;
  virtual ui::NativeTheme* GetNativeTheme() const OVERRIDE;

 protected:
  // Deletes this window as it is destroyed, override to provide different
  // behavior.
  virtual void OnFinalMessage(HWND window);

  HWNDMessageHandler* GetMessageHandler();

  // Overridden from HWNDMessageHandlerDelegate:
  virtual bool IsWidgetWindow() const OVERRIDE;
  virtual bool IsUsingCustomFrame() const OVERRIDE;
  virtual void SchedulePaint() OVERRIDE;
  virtual void EnableInactiveRendering() OVERRIDE;
  virtual bool IsInactiveRenderingDisabled() OVERRIDE;
  virtual bool CanResize() const OVERRIDE;
  virtual bool CanMaximize() const OVERRIDE;
  virtual bool CanActivate() const OVERRIDE;
  virtual bool WidgetSizeIsClientSize() const OVERRIDE;
  virtual bool CanSaveFocus() const OVERRIDE;
  virtual void SaveFocusOnDeactivate() OVERRIDE;
  virtual void RestoreFocusOnActivate() OVERRIDE;
  virtual void RestoreFocusOnEnable() OVERRIDE;
  virtual bool IsModal() const OVERRIDE;
  virtual int GetInitialShowState() const OVERRIDE;
  virtual bool WillProcessWorkAreaChange() const OVERRIDE;
  virtual int GetNonClientComponent(const gfx::Point& point) const OVERRIDE;
  virtual void GetWindowMask(const gfx::Size& size, gfx::Path* path) OVERRIDE;
  virtual bool GetClientAreaInsets(gfx::Insets* insets) const OVERRIDE;
  virtual void GetMinMaxSize(gfx::Size* min_size,
                             gfx::Size* max_size) const OVERRIDE;
  virtual gfx::Size GetRootViewSize() const OVERRIDE;
  virtual void ResetWindowControls() OVERRIDE;
  virtual void PaintLayeredWindow(gfx::Canvas* canvas) OVERRIDE;
  virtual InputMethod* GetInputMethod() OVERRIDE;
  virtual gfx::NativeViewAccessible GetNativeViewAccessible() OVERRIDE;
  virtual bool ShouldHandleSystemCommands() const OVERRIDE;
  virtual void HandleAppDeactivated() OVERRIDE;
  virtual void HandleActivationChanged(bool active) OVERRIDE;
  virtual bool HandleAppCommand(short command) OVERRIDE;
  virtual void HandleCancelMode() OVERRIDE;
  virtual void HandleCaptureLost() OVERRIDE;
  virtual void HandleClose() OVERRIDE;
  virtual bool HandleCommand(int command) OVERRIDE;
  virtual void HandleAccelerator(const ui::Accelerator& accelerator) OVERRIDE;
  virtual void HandleCreate() OVERRIDE;
  virtual void HandleDestroying() OVERRIDE;
  virtual void HandleDestroyed() OVERRIDE;
  virtual bool HandleInitialFocus() OVERRIDE;
  virtual void HandleDisplayChange() OVERRIDE;
  virtual void HandleBeginWMSizeMove() OVERRIDE;
  virtual void HandleEndWMSizeMove() OVERRIDE;
  virtual void HandleMove() OVERRIDE;
  virtual void HandleWorkAreaChanged() OVERRIDE;
  virtual void HandleVisibilityChanged(bool visible) OVERRIDE;
  virtual void HandleClientSizeChanged(const gfx::Size& new_size) OVERRIDE;
  virtual void HandleFrameChanged() OVERRIDE;
  virtual void HandleNativeFocus(HWND last_focused_window) OVERRIDE;
  virtual void HandleNativeBlur(HWND focused_window) OVERRIDE;
  virtual bool HandleMouseEvent(const ui::MouseEvent& event) OVERRIDE;
  virtual bool HandleKeyEvent(const ui::KeyEvent& event) OVERRIDE;
  virtual bool HandleUntranslatedKeyEvent(const ui::KeyEvent& event) OVERRIDE;
  virtual bool HandleTouchEvent(const ui::TouchEvent& event) OVERRIDE;
  virtual bool HandleIMEMessage(UINT message,
                                WPARAM w_param,
                                LPARAM l_param,
                                LRESULT* result) OVERRIDE;
  virtual void HandleInputLanguageChange(DWORD character_set,
                                         HKL input_language_id) OVERRIDE;
  virtual bool HandlePaintAccelerated(const gfx::Rect& invalid_rect) OVERRIDE;
  virtual void HandlePaint(gfx::Canvas* canvas) OVERRIDE;
  virtual bool HandleTooltipNotify(int w_param,
                                   NMHDR* l_param,
                                   LRESULT* l_result) OVERRIDE;
  virtual void HandleTooltipMouseMove(UINT message,
                                      WPARAM w_param,
                                      LPARAM l_param) OVERRIDE;
  virtual bool PreHandleMSG(UINT message,
                            WPARAM w_param,
                            LPARAM l_param,
                            LRESULT* result) OVERRIDE;
  virtual void PostHandleMSG(UINT message,
                             WPARAM w_param,
                             LPARAM l_param) OVERRIDE;

  // The TooltipManager. This is NULL if there is a problem creating the
  // underlying tooltip window.
  // WARNING: RootView's destructor calls into the TooltipManager. As such, this
  // must be destroyed AFTER root_view_.
  scoped_ptr<TooltipManagerWin> tooltip_manager_;

  scoped_refptr<DropTargetWin> drop_target_;

 private:
  typedef ScopedVector<ui::ViewProp> ViewProps;

  void SetInitParams(const Widget::InitParams& params);

  // A delegate implementation that handles events received here.
  // See class documentation for Widget in widget.h for a note about ownership.
  internal::NativeWidgetDelegate* delegate_;

  // See class documentation for Widget in widget.h for a note about ownership.
  Widget::InitParams::Ownership ownership_;

  ViewProps props_;

  // The window styles before we modified them for the drag frame appearance.
  DWORD drag_frame_saved_window_style_;
  DWORD drag_frame_saved_window_ex_style_;

  // True if the widget is going to have a non_client_view. We cache this value
  // rather than asking the Widget for the non_client_view so that we know at
  // Init time, before the Widget has created the NonClientView.
  bool has_non_client_view_;

  scoped_ptr<HWNDMessageHandler> message_handler_;

  DISALLOW_COPY_AND_ASSIGN(NativeWidgetWin);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_NATIVE_WIDGET_WIN_H_
