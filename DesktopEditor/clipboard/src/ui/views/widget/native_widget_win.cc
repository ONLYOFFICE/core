// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/native_widget_win.h"

#include <dwmapi.h>
#include <shellapi.h>

#include <algorithm>

#include "base/bind.h"
#include "base/string_util.h"
#include "base/win/scoped_gdi_object.h"
#include "base/win/win_util.h"
#include "base/win/windows_version.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/dragdrop/drag_source_win.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/base/dragdrop/os_exchange_data_provider_win.h"
#include "ui/base/events/event.h"
#include "ui/base/keycodes/keyboard_code_conversion_win.h"
#include "ui/base/l10n/l10n_util_win.h"
#include "ui/base/theme_provider.h"
#include "ui/base/view_prop.h"
#include "ui/base/win/hwnd_util.h"
#include "ui/base/win/mouse_wheel_util.h"
#include "ui/base/win/shell.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/canvas_paint.h"
#include "ui/gfx/canvas_skia_paint.h"
#include "ui/gfx/path.h"
#include "ui/gfx/screen.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/controls/native_control_win.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/drag_utils.h"
#include "ui/views/focus/accelerator_handler.h"
#include "ui/views/focus/view_storage.h"
#include "ui/views/focus/widget_focus_manager.h"
#include "ui/views/ime/input_method_win.h"
#include "ui/views/widget/aero_tooltip_manager.h"
#include "ui/views/widget/drop_target_win.h"
#include "ui/views/widget/monitor_win.h"
#include "ui/views/widget/native_widget_delegate.h"
#include "ui/views/widget/root_view.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/views/widget/widget_hwnd_utils.h"
#include "ui/views/win/fullscreen_handler.h"
#include "ui/views/win/hwnd_message_handler.h"
#include "ui/views/window/native_frame_view.h"

#pragma comment(lib, "dwmapi.lib")

using ui::ViewProp;

namespace views {

namespace {

// Enumeration callback for NativeWidget::GetAllChildWidgets(). Called for each
// child HWND beneath the original HWND.
BOOL CALLBACK EnumerateChildWindowsForNativeWidgets(HWND hwnd, LPARAM l_param) {
  Widget* widget = Widget::GetWidgetForNativeView(hwnd);
  if (widget) {
    Widget::Widgets* widgets = reinterpret_cast<Widget::Widgets*>(l_param);
    widgets->insert(widget);
  }
  return TRUE;
}

// Links the HWND to its NativeWidget.
const char* const kNativeWidgetKey = "__VIEWS_NATIVE_WIDGET__";

const int kDragFrameWindowAlpha = 200;

}  // namespace

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetWin, public:

NativeWidgetWin::NativeWidgetWin(internal::NativeWidgetDelegate* delegate)
    : delegate_(delegate),
      ownership_(Widget::InitParams::NATIVE_WIDGET_OWNS_WIDGET),
      has_non_client_view_(false),
      message_handler_(new HWNDMessageHandler(this)) {
}

NativeWidgetWin::~NativeWidgetWin() {
  if (ownership_ == Widget::InitParams::NATIVE_WIDGET_OWNS_WIDGET)
    delete delegate_;
  else
    CloseNow();
  message_handler_.reset();
}

// static
gfx::Font NativeWidgetWin::GetWindowTitleFont() {
  NONCLIENTMETRICS ncm;
  base::win::GetNonClientMetrics(&ncm);
  l10n_util::AdjustUIFont(&(ncm.lfCaptionFont));
  base::win::ScopedHFONT caption_font(CreateFontIndirect(&(ncm.lfCaptionFont)));
  return gfx::Font(caption_font);
}

void NativeWidgetWin::Show(int show_state) {
  message_handler_->Show(show_state);
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetWin, NativeWidget implementation:

void NativeWidgetWin::InitNativeWidget(const Widget::InitParams& params) {
  SetInitParams(params);
  message_handler_->Init(params.parent, params.bounds);
}

NonClientFrameView* NativeWidgetWin::CreateNonClientFrameView() {
  return GetWidget()->ShouldUseNativeFrame() ?
      new NativeFrameView(GetWidget()) : NULL;
}

bool NativeWidgetWin::ShouldUseNativeFrame() const {
  return ui::win::IsAeroGlassEnabled();
}

void NativeWidgetWin::FrameTypeChanged() {
  message_handler_->FrameTypeChanged();
}

Widget* NativeWidgetWin::GetWidget() {
  return delegate_->AsWidget();
}

const Widget* NativeWidgetWin::GetWidget() const {
  return delegate_->AsWidget();
}

gfx::NativeView NativeWidgetWin::GetNativeView() const {
  return message_handler_->hwnd();
}

gfx::NativeWindow NativeWidgetWin::GetNativeWindow() const {
  return message_handler_->hwnd();
}

Widget* NativeWidgetWin::GetTopLevelWidget() {
  NativeWidgetPrivate* native_widget = GetTopLevelNativeWidget(GetNativeView());
  return native_widget ? native_widget->GetWidget() : NULL;
}

const ui::Compositor* NativeWidgetWin::GetCompositor() const {
  return NULL;
}

ui::Compositor* NativeWidgetWin::GetCompositor() {
  return NULL;
}

gfx::Vector2d NativeWidgetWin::CalculateOffsetToAncestorWithLayer(
    ui::Layer** layer_parent) {
  return gfx::Vector2d();
}

void NativeWidgetWin::ViewRemoved(View* view) {
  if (drop_target_.get())
    drop_target_->ResetTargetViewIfEquals(view);
}

void NativeWidgetWin::SetNativeWindowProperty(const char* name, void* value) {
  // Remove the existing property (if any).
  for (ViewProps::iterator i = props_.begin(); i != props_.end(); ++i) {
    if ((*i)->Key() == name) {
      props_.erase(i);
      break;
    }
  }

  if (value)
    props_.push_back(new ViewProp(GetNativeView(), name, value));
}

void* NativeWidgetWin::GetNativeWindowProperty(const char* name) const {
  return ViewProp::GetValue(GetNativeView(), name);
}

TooltipManager* NativeWidgetWin::GetTooltipManager() const {
  return tooltip_manager_.get();
}

void NativeWidgetWin::SetCapture() {
  message_handler_->SetCapture();
}

void NativeWidgetWin::ReleaseCapture() {
  message_handler_->ReleaseCapture();
}

bool NativeWidgetWin::HasCapture() const {
  return message_handler_->HasCapture();
}

InputMethod* NativeWidgetWin::CreateInputMethod() {
  return new InputMethodWin(GetMessageHandler(), GetNativeWindow(), NULL);
}

internal::InputMethodDelegate* NativeWidgetWin::GetInputMethodDelegate() {
  return message_handler_.get();
}

void NativeWidgetWin::CenterWindow(const gfx::Size& size) {
  message_handler_->CenterWindow(size);
}

void NativeWidgetWin::GetWindowPlacement(
    gfx::Rect* bounds,
    ui::WindowShowState* show_state) const {
  message_handler_->GetWindowPlacement(bounds, show_state);
}

void NativeWidgetWin::SetWindowTitle(const string16& title) {
  message_handler_->SetTitle(title);
}

void NativeWidgetWin::SetWindowIcons(const gfx::ImageSkia& window_icon,
                                     const gfx::ImageSkia& app_icon) {
  message_handler_->SetWindowIcons(window_icon, app_icon);
}

void NativeWidgetWin::InitModalType(ui::ModalType modal_type) {
  message_handler_->InitModalType(modal_type);
}

gfx::Rect NativeWidgetWin::GetWindowBoundsInScreen() const {
  return message_handler_->GetWindowBoundsInScreen();
}

gfx::Rect NativeWidgetWin::GetClientAreaBoundsInScreen() const {
  return message_handler_->GetClientAreaBoundsInScreen();
}

gfx::Rect NativeWidgetWin::GetRestoredBounds() const {
  return message_handler_->GetRestoredBounds();
}

void NativeWidgetWin::SetBounds(const gfx::Rect& bounds) {
  message_handler_->SetBounds(bounds);
}

void NativeWidgetWin::SetSize(const gfx::Size& size) {
  message_handler_->SetSize(size);
}

void NativeWidgetWin::StackAbove(gfx::NativeView native_view) {
  message_handler_->StackAbove(native_view);
}

void NativeWidgetWin::StackAtTop() {
  message_handler_->StackAtTop();
}

void NativeWidgetWin::StackBelow(gfx::NativeView native_view) {
  NOTIMPLEMENTED();
}

void NativeWidgetWin::SetShape(gfx::NativeRegion region) {
  message_handler_->SetRegion(region);
}

void NativeWidgetWin::Close() {
  message_handler_->Close();
}

void NativeWidgetWin::CloseNow() {
  message_handler_->CloseNow();
}

void NativeWidgetWin::Show() {
  message_handler_->Show();
}

void NativeWidgetWin::Hide() {
  message_handler_->Hide();
}

void NativeWidgetWin::ShowMaximizedWithBounds(
    const gfx::Rect& restored_bounds) {
  message_handler_->ShowMaximizedWithBounds(restored_bounds);
}

void NativeWidgetWin::ShowWithWindowState(ui::WindowShowState show_state) {
  message_handler_->ShowWindowWithState(show_state);
}

bool NativeWidgetWin::IsVisible() const {
  return message_handler_->IsVisible();
}

void NativeWidgetWin::Activate() {
  message_handler_->Activate();
}

void NativeWidgetWin::Deactivate() {
  message_handler_->Deactivate();
}

bool NativeWidgetWin::IsActive() const {
  return message_handler_->IsActive();
}

void NativeWidgetWin::SetAlwaysOnTop(bool on_top) {
  message_handler_->SetAlwaysOnTop(on_top);
}

void NativeWidgetWin::Maximize() {
  message_handler_->Maximize();
}

void NativeWidgetWin::Minimize() {
  message_handler_->Minimize();
}

bool NativeWidgetWin::IsMaximized() const {
  return message_handler_->IsMaximized();
}

bool NativeWidgetWin::IsMinimized() const {
  return message_handler_->IsMinimized();
}

void NativeWidgetWin::Restore() {
  message_handler_->Restore();
}

void NativeWidgetWin::SetFullscreen(bool fullscreen) {
  message_handler_->fullscreen_handler()->SetFullscreen(fullscreen);
}

void NativeWidgetWin::SetMetroSnapFullscreen(bool metro_snap) {
  message_handler_->fullscreen_handler()->SetMetroSnap(metro_snap);
}

bool NativeWidgetWin::IsFullscreen() const {
  return message_handler_->fullscreen_handler()->fullscreen();
}

bool NativeWidgetWin::IsInMetroSnapMode() const {
  return message_handler_->fullscreen_handler()->metro_snap();
}

void NativeWidgetWin::SetCanUpdateLayeredWindow(bool can_update) {
  message_handler_->set_can_update_layered_window(can_update);
}

void NativeWidgetWin::SetOpacity(unsigned char opacity) {
  message_handler_->SetOpacity(static_cast<BYTE>(opacity));
  GetWidget()->GetRootView()->SchedulePaint();
}

void NativeWidgetWin::SetUseDragFrame(bool use_drag_frame) {
  if (use_drag_frame) {
    // Make the frame slightly transparent during the drag operation.
    drag_frame_saved_window_style_ = GetWindowLong(GetNativeView(), GWL_STYLE);
    drag_frame_saved_window_ex_style_ =
        GetWindowLong(GetNativeView(), GWL_EXSTYLE);
    SetWindowLong(GetNativeView(), GWL_EXSTYLE,
                  drag_frame_saved_window_ex_style_ | WS_EX_LAYERED);
    // Remove the captions tyle so the window doesn't have window controls for a
    // more "transparent" look.
    SetWindowLong(GetNativeView(), GWL_STYLE,
                  drag_frame_saved_window_style_ & ~WS_CAPTION);
    SetLayeredWindowAttributes(GetNativeView(), RGB(0xFF, 0xFF, 0xFF),
                               kDragFrameWindowAlpha, LWA_ALPHA);
  } else {
    SetWindowLong(GetNativeView(), GWL_STYLE, drag_frame_saved_window_style_);
    SetWindowLong(GetNativeView(), GWL_EXSTYLE,
                  drag_frame_saved_window_ex_style_);
  }
}

void NativeWidgetWin::FlashFrame(bool flash) {
  message_handler_->FlashFrame(flash);
}

void NativeWidgetWin::RunShellDrag(View* view,
                                   const ui::OSExchangeData& data,
                                   const gfx::Point& location,
                                   int operation,
                                   ui::DragDropTypes::DragEventSource source) {
  views::RunShellDrag(NULL, data, location, operation, source);
}

void NativeWidgetWin::SchedulePaintInRect(const gfx::Rect& rect) {
  message_handler_->SchedulePaintInRect(rect);
}

void NativeWidgetWin::SetCursor(gfx::NativeCursor cursor) {
  message_handler_->SetCursor(cursor);
}

void NativeWidgetWin::ClearNativeFocus() {
  message_handler_->ClearNativeFocus();
}

gfx::Rect NativeWidgetWin::GetWorkAreaBoundsInScreen() const {
  return gfx::Screen::GetNativeScreen()->GetDisplayNearestWindow(
      GetNativeView()).work_area();
}

void NativeWidgetWin::SetInactiveRenderingDisabled(bool value) {
}

Widget::MoveLoopResult NativeWidgetWin::RunMoveLoop(
    const gfx::Vector2d& drag_offset,
    Widget::MoveLoopSource source) {
  return message_handler_->RunMoveLoop(drag_offset) ?
      Widget::MOVE_LOOP_SUCCESSFUL : Widget::MOVE_LOOP_CANCELED;
}

void NativeWidgetWin::EndMoveLoop() {
  message_handler_->EndMoveLoop();
}

void NativeWidgetWin::SetVisibilityChangedAnimationsEnabled(bool value) {
  message_handler_->SetVisibilityChangedAnimationsEnabled(value);
}

ui::NativeTheme* NativeWidgetWin::GetNativeTheme() const {
  return ui::NativeTheme::instance();
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetWin, protected:

void NativeWidgetWin::OnFinalMessage(HWND window) {
  // We don't destroy props in WM_DESTROY as we may still get messages after
  // WM_DESTROY that assume the properties are still valid (such as WM_CLOSE).
  props_.clear();
  delegate_->OnNativeWidgetDestroyed();
  if (ownership_ == Widget::InitParams::NATIVE_WIDGET_OWNS_WIDGET)
    delete this;
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetWin, protected:

HWNDMessageHandler* NativeWidgetWin::GetMessageHandler() {
  return message_handler_.get();
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetWin, HWNDMessageHandlerDelegate implementation:

bool NativeWidgetWin::IsWidgetWindow() const {
  // We don't NULL check GetWidget()->non_client_view() here because this
  // function can be called before the widget is fully constructed.
  return has_non_client_view_;
}

bool NativeWidgetWin::IsUsingCustomFrame() const {
  return !GetWidget()->ShouldUseNativeFrame();
}

void NativeWidgetWin::SchedulePaint() {
  GetWidget()->GetRootView()->SchedulePaint();
}

void NativeWidgetWin::EnableInactiveRendering() {
  delegate_->EnableInactiveRendering();
}

bool NativeWidgetWin::IsInactiveRenderingDisabled() {
  return delegate_->IsInactiveRenderingDisabled();
}

bool NativeWidgetWin::CanResize() const {
  return GetWidget()->widget_delegate()->CanResize();
}

bool NativeWidgetWin::CanMaximize() const {
  return GetWidget()->widget_delegate()->CanMaximize();
}

bool NativeWidgetWin::CanActivate() const {
  return delegate_->CanActivate();
}

bool NativeWidgetWin::WidgetSizeIsClientSize() const {
  const Widget* widget = GetWidget()->GetTopLevelWidget();
  return IsZoomed(GetNativeView()) ||
         (widget && widget->ShouldUseNativeFrame());
}

bool NativeWidgetWin::CanSaveFocus() const {
  return GetWidget()->is_top_level();
}

void NativeWidgetWin::SaveFocusOnDeactivate() {
  GetWidget()->GetFocusManager()->StoreFocusedView(true);
}

void NativeWidgetWin::RestoreFocusOnActivate() {
  // Mysteriously, this only appears to be needed support restoration of focus
  // to a child hwnd when restoring its top level window from the minimized
  // state. If we don't do this, then ::SetFocus() to that child HWND returns
  // ERROR_INVALID_PARAMETER, despite both HWNDs being of the same thread.
  // See http://crbug.com/125976 and
  // chrome/browser/ui/views/native_widget_win_interactive_uitest.cc .
  {
    // Since this is a synthetic reset, we don't need to tell anyone about it.
    AutoNativeNotificationDisabler disabler;
    GetWidget()->GetFocusManager()->ClearFocus();
  }
  RestoreFocusOnEnable();
}

void NativeWidgetWin::RestoreFocusOnEnable() {
  GetWidget()->GetFocusManager()->RestoreFocusedView();
}

bool NativeWidgetWin::IsModal() const {
  return delegate_->IsModal();
}

int NativeWidgetWin::GetInitialShowState() const {
  return SW_SHOWNORMAL;
}

bool NativeWidgetWin::WillProcessWorkAreaChange() const {
  return GetWidget()->widget_delegate()->WillProcessWorkAreaChange();
}

int NativeWidgetWin::GetNonClientComponent(const gfx::Point& point) const {
  return delegate_->GetNonClientComponent(point);
}

void NativeWidgetWin::GetWindowMask(const gfx::Size& size, gfx::Path* path) {
  if (GetWidget()->non_client_view())
    GetWidget()->non_client_view()->GetWindowMask(size, path);
}

bool NativeWidgetWin::GetClientAreaInsets(gfx::Insets* insets) const {
  return false;
}

void NativeWidgetWin::GetMinMaxSize(gfx::Size* min_size,
                                    gfx::Size* max_size) const {
  *min_size = delegate_->GetMinimumSize();
  *max_size = delegate_->GetMaximumSize();
}

gfx::Size NativeWidgetWin::GetRootViewSize() const {
  return GetWidget()->GetRootView()->size();
}

void NativeWidgetWin::ResetWindowControls() {
  GetWidget()->non_client_view()->ResetWindowControls();
}

void NativeWidgetWin::PaintLayeredWindow(gfx::Canvas* canvas) {
  GetWidget()->GetRootView()->Paint(canvas);
}

InputMethod* NativeWidgetWin::GetInputMethod() {
  return GetWidget()->GetInputMethodDirect();
}

gfx::NativeViewAccessible NativeWidgetWin::GetNativeViewAccessible() {
  return GetWidget()->GetRootView()->GetNativeViewAccessible();
}

bool NativeWidgetWin::ShouldHandleSystemCommands() const {
  return GetWidget()->widget_delegate()->ShouldHandleSystemCommands();
}

void NativeWidgetWin::HandleAppDeactivated() {
  if (IsInactiveRenderingDisabled()) {
    delegate_->EnableInactiveRendering();
  } else {
    // TODO(pkotwicz): Remove need for SchedulePaint(). crbug.com/165841
    View* non_client_view = GetWidget()->non_client_view();
    if (non_client_view)
      non_client_view->SchedulePaint();
  }
}

void NativeWidgetWin::HandleActivationChanged(bool active) {
  delegate_->OnNativeWidgetActivationChanged(active);
}

bool NativeWidgetWin::HandleAppCommand(short command) {
  // We treat APPCOMMAND ids as an extension of our command namespace, and just
  // let the delegate figure out what to do...
  return GetWidget()->widget_delegate() &&
      GetWidget()->widget_delegate()->ExecuteWindowsCommand(command);
}

void NativeWidgetWin::HandleCancelMode() {
}

void NativeWidgetWin::HandleCaptureLost() {
  delegate_->OnMouseCaptureLost();
}

void NativeWidgetWin::HandleClose() {
  GetWidget()->Close();
}

bool NativeWidgetWin::HandleCommand(int command) {
  return GetWidget()->widget_delegate()->ExecuteWindowsCommand(command);
}

void NativeWidgetWin::HandleAccelerator(const ui::Accelerator& accelerator) {
  GetWidget()->GetFocusManager()->ProcessAccelerator(accelerator);
}

void NativeWidgetWin::HandleCreate() {
  // TODO(beng): much of this could/should maybe move to HWNDMessageHandler.

  SetNativeWindowProperty(kNativeWidgetKey, this);
  CHECK_EQ(this, GetNativeWidgetForNativeView(GetNativeView()));

  props_.push_back(ui::SetWindowSupportsRerouteMouseWheel(GetNativeView()));

  drop_target_ = new DropTargetWin(
      static_cast<internal::RootView*>(GetWidget()->GetRootView()));

  // Windows special DWM window frame requires a special tooltip manager so
  // that window controls in Chrome windows don't flicker when you move your
  // mouse over them. See comment in aero_tooltip_manager.h.
  Widget* widget = GetWidget()->GetTopLevelWidget();
  if (widget && widget->ShouldUseNativeFrame()) {
    tooltip_manager_.reset(new AeroTooltipManager(GetWidget()));
  } else {
    tooltip_manager_.reset(new TooltipManagerWin(GetWidget()));
  }
  if (!tooltip_manager_->Init()) {
    // There was a problem creating the TooltipManager. Common error is 127.
    // See 82193 for details.
    LOG_GETLASTERROR(WARNING) << "tooltip creation failed, disabling tooltips";
    tooltip_manager_.reset();
  }

  delegate_->OnNativeWidgetCreated();
}

void NativeWidgetWin::HandleDestroying() {
  delegate_->OnNativeWidgetDestroying();
  if (drop_target_.get()) {
    RevokeDragDrop(GetNativeView());
    drop_target_ = NULL;
  }
}

void NativeWidgetWin::HandleDestroyed() {
  OnFinalMessage(GetNativeView());
}

bool NativeWidgetWin::HandleInitialFocus() {
  return GetWidget()->SetInitialFocus();
}

void NativeWidgetWin::HandleDisplayChange() {
  GetWidget()->widget_delegate()->OnDisplayChanged();
}

void NativeWidgetWin::HandleBeginWMSizeMove() {
  delegate_->OnNativeWidgetBeginUserBoundsChange();
}

void NativeWidgetWin::HandleEndWMSizeMove() {
  delegate_->OnNativeWidgetEndUserBoundsChange();
}

void NativeWidgetWin::HandleMove() {
  delegate_->OnNativeWidgetMove();
}

void NativeWidgetWin::HandleWorkAreaChanged() {
  GetWidget()->widget_delegate()->OnWorkAreaChanged();
}

void NativeWidgetWin::HandleVisibilityChanged(bool visible) {
  delegate_->OnNativeWidgetVisibilityChanged(visible);
}

void NativeWidgetWin::HandleClientSizeChanged(const gfx::Size& new_size) {
  delegate_->OnNativeWidgetSizeChanged(new_size);
}

void NativeWidgetWin::HandleFrameChanged() {
  // Replace the frame and layout the contents.
  GetWidget()->non_client_view()->UpdateFrame(true);
}

void NativeWidgetWin::HandleNativeFocus(HWND last_focused_window) {
  delegate_->OnNativeFocus(last_focused_window);
  InputMethod* input_method = GetInputMethod();
  if (input_method)
    input_method->OnFocus();
}

void NativeWidgetWin::HandleNativeBlur(HWND focused_window) {
  delegate_->OnNativeBlur(focused_window);
  InputMethod* input_method = GetInputMethod();
  if (input_method)
    input_method->OnBlur();
}

bool NativeWidgetWin::HandleMouseEvent(const ui::MouseEvent& event) {
  delegate_->OnMouseEvent(const_cast<ui::MouseEvent*>(&event));
  return event.handled();
}

bool NativeWidgetWin::HandleKeyEvent(const ui::KeyEvent& event) {
  delegate_->OnKeyEvent(const_cast<ui::KeyEvent*>(&event));
  return event.handled();
}

bool NativeWidgetWin::HandleUntranslatedKeyEvent(const ui::KeyEvent& event) {
  InputMethod* input_method = GetInputMethod();
  if (input_method)
    input_method->DispatchKeyEvent(event);
  return !!input_method;
}

bool NativeWidgetWin::HandleTouchEvent(const ui::TouchEvent& event) {
  NOTREACHED() << "Touch events are not supported";
  return false;
}

bool NativeWidgetWin::HandleIMEMessage(UINT message,
                                       WPARAM w_param,
                                       LPARAM l_param,
                                       LRESULT* result) {
  InputMethod* input_method = GetInputMethod();
  if (!input_method || input_method->IsMock()) {
    *result = 0;
    return false;
  }

  InputMethodWin* ime_win = static_cast<InputMethodWin*>(input_method);
  BOOL handled = FALSE;
  *result = ime_win->OnImeMessages(message, w_param, l_param, &handled);
  return !!handled;
}

void NativeWidgetWin::HandleInputLanguageChange(DWORD character_set,
                                                HKL input_language_id) {
  InputMethod* input_method = GetInputMethod();
  if (input_method && !input_method->IsMock()) {
    static_cast<InputMethodWin*>(input_method)->OnInputLangChange(
        character_set, input_language_id);
  }
}

bool NativeWidgetWin::HandlePaintAccelerated(const gfx::Rect& invalid_rect) {
  return delegate_->OnNativeWidgetPaintAccelerated(gfx::Rect(invalid_rect));
}

void NativeWidgetWin::HandlePaint(gfx::Canvas* canvas) {
  delegate_->OnNativeWidgetPaint(canvas);
}

bool NativeWidgetWin::HandleTooltipNotify(int w_param,
                                          NMHDR* l_param,
                                          LRESULT* l_result) {
  // We can be sent this message before the tooltip manager is created, if a
  // subclass overrides OnCreate and creates some kind of Windows control there
  // that sends WM_NOTIFY messages.
  if (tooltip_manager_.get()) {
    bool handled;
    *l_result = tooltip_manager_->OnNotify(w_param, l_param, &handled);
    return handled;
  }
  return false;
}

void NativeWidgetWin::HandleTooltipMouseMove(UINT message,
                                             WPARAM w_param,
                                             LPARAM l_param) {
  if (tooltip_manager_.get())
    tooltip_manager_->OnMouse(message, w_param, l_param);
}

bool NativeWidgetWin::PreHandleMSG(UINT message,
                                   WPARAM w_param,
                                   LPARAM l_param,
                                   LRESULT* result) {
  return false;
}

void NativeWidgetWin::PostHandleMSG(UINT message,
                                    WPARAM w_param,
                                    LPARAM l_param) {
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetWin, private:

void NativeWidgetWin::SetInitParams(const Widget::InitParams& params) {
  // Set non-style attributes.
  ownership_ = params.ownership;

  ConfigureWindowStyles(message_handler_.get(), params,
                        GetWidget()->widget_delegate(), delegate_);

  has_non_client_view_ = Widget::RequiresNonClientView(params.type);
  message_handler_->set_remove_standard_frame(params.remove_standard_frame);
  message_handler_->set_use_system_default_icon(params.use_system_default_icon);
}

////////////////////////////////////////////////////////////////////////////////
// Widget, public:

// static
void Widget::NotifyLocaleChanged() {
  NOTIMPLEMENTED();
}

namespace {
BOOL CALLBACK WindowCallbackProc(HWND hwnd, LPARAM lParam) {
  Widget* widget = Widget::GetWidgetForNativeView(hwnd);
  if (widget && widget->is_secondary_widget())
    widget->Close();
  return TRUE;
}
}  // namespace

// static
void Widget::CloseAllSecondaryWidgets() {
  EnumThreadWindows(GetCurrentThreadId(), WindowCallbackProc, 0);
}

bool Widget::ConvertRect(const Widget* source,
                         const Widget* target,
                         gfx::Rect* rect) {
  DCHECK(source);
  DCHECK(target);
  DCHECK(rect);

  HWND source_hwnd = source->GetNativeView();
  HWND target_hwnd = target->GetNativeView();
  if (source_hwnd == target_hwnd)
    return true;

  RECT win_rect = rect->ToRECT();
  if (::MapWindowPoints(source_hwnd, target_hwnd,
                        reinterpret_cast<LPPOINT>(&win_rect),
                        sizeof(RECT)/sizeof(POINT))) {
    *rect = gfx::Rect(win_rect);
    return true;
  }
  return false;
}

namespace internal {

////////////////////////////////////////////////////////////////////////////////
// internal::NativeWidgetPrivate, public:

// static
NativeWidgetPrivate* NativeWidgetPrivate::CreateNativeWidget(
    internal::NativeWidgetDelegate* delegate) {
  return new NativeWidgetWin(delegate);
}

// static
NativeWidgetPrivate* NativeWidgetPrivate::GetNativeWidgetForNativeView(
    gfx::NativeView native_view) {
  return reinterpret_cast<NativeWidgetWin*>(
      ViewProp::GetValue(native_view, kNativeWidgetKey));
}

// static
NativeWidgetPrivate* NativeWidgetPrivate::GetNativeWidgetForNativeWindow(
    gfx::NativeWindow native_window) {
  return GetNativeWidgetForNativeView(native_window);
}

// static
NativeWidgetPrivate* NativeWidgetPrivate::GetTopLevelNativeWidget(
    gfx::NativeView native_view) {
  if (!native_view)
    return NULL;

  // First, check if the top-level window is a Widget.
  HWND root = ::GetAncestor(native_view, GA_ROOT);
  if (!root)
    return NULL;

  NativeWidgetPrivate* widget = GetNativeWidgetForNativeView(root);
  if (widget)
    return widget;

  // Second, try to locate the last Widget window in the parent hierarchy.
  HWND parent_hwnd = native_view;
  // If we fail to find the native widget pointer for the root then it probably
  // means that the root belongs to a different process in which case we walk up
  // the native view chain looking for a parent window which corresponds to a
  // valid native widget. We only do this if we fail to find the native widget
  // for the current native view which means it is being destroyed.
  if (!widget && !GetNativeWidgetForNativeView(native_view)) {
    parent_hwnd = ::GetAncestor(parent_hwnd, GA_PARENT);
    if (!parent_hwnd)
      return NULL;
  }
  NativeWidgetPrivate* parent_widget;
  do {
    parent_widget = GetNativeWidgetForNativeView(parent_hwnd);
    if (parent_widget) {
      widget = parent_widget;
      parent_hwnd = ::GetAncestor(parent_hwnd, GA_PARENT);
    }
  } while (parent_hwnd != NULL && parent_widget != NULL);

  return widget;
}

// static
void NativeWidgetPrivate::GetAllChildWidgets(gfx::NativeView native_view,
                                             Widget::Widgets* children) {
  if (!native_view)
    return;

  Widget* widget = Widget::GetWidgetForNativeView(native_view);
  if (widget)
    children->insert(widget);
  EnumChildWindows(native_view, EnumerateChildWindowsForNativeWidgets,
                   reinterpret_cast<LPARAM>(children));
}

// static
void NativeWidgetPrivate::ReparentNativeView(gfx::NativeView native_view,
                                             gfx::NativeView new_parent) {
  if (!native_view)
    return;

  HWND previous_parent = ::GetParent(native_view);
  if (previous_parent == new_parent)
    return;

  Widget::Widgets widgets;
  GetAllChildWidgets(native_view, &widgets);

  // First notify all the widgets that they are being disassociated
  // from their previous parent.
  for (Widget::Widgets::iterator it = widgets.begin();
       it != widgets.end(); ++it) {
    // TODO(beng): Rename this notification to NotifyNativeViewChanging()
    // and eliminate the bool parameter.
    (*it)->NotifyNativeViewHierarchyChanged(false, previous_parent);
  }

  ::SetParent(native_view, new_parent);

  // And now, notify them that they have a brand new parent.
  for (Widget::Widgets::iterator it = widgets.begin();
       it != widgets.end(); ++it) {
    (*it)->NotifyNativeViewHierarchyChanged(true, new_parent);
  }
}

// static
bool NativeWidgetPrivate::IsMouseButtonDown() {
  return (GetKeyState(VK_LBUTTON) & 0x80) ||
    (GetKeyState(VK_RBUTTON) & 0x80) ||
    (GetKeyState(VK_MBUTTON) & 0x80) ||
    (GetKeyState(VK_XBUTTON1) & 0x80) ||
    (GetKeyState(VK_XBUTTON2) & 0x80);
}

// static
bool NativeWidgetPrivate::IsTouchDown() {
  // This currently isn't necessary because we're not generating touch events on
  // windows.  When we do, this will need to be updated.
  return false;
}

}  // namespace internal

}  // namespace views
