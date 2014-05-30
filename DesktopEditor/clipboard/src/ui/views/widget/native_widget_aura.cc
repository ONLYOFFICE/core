// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/native_widget_aura.h"

#include "base/bind.h"
#include "base/string_util.h"
#include "third_party/skia/include/core/SkRegion.h"
#include "ui/aura/client/activation_client.h"
#include "ui/aura/client/aura_constants.h"
#include "ui/aura/client/cursor_client.h"
#include "ui/aura/client/drag_drop_client.h"
#include "ui/aura/client/screen_position_client.h"
#include "ui/aura/client/stacking_client.h"
#include "ui/aura/client/window_move_client.h"
#include "ui/aura/client/window_types.h"
#include "ui/aura/env.h"
#include "ui/aura/focus_manager.h"
#include "ui/aura/root_window.h"
#include "ui/aura/window.h"
#include "ui/aura/window_observer.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/base/events/event.h"
#include "ui/base/ui_base_types.h"
#include "ui/compositor/layer.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font.h"
#include "ui/gfx/screen.h"
#include "ui/native_theme/native_theme_aura.h"
#include "ui/views/drag_utils.h"
#include "ui/views/ime/input_method_bridge.h"
#include "ui/views/views_delegate.h"
#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"
#include "ui/views/widget/drop_helper.h"
#include "ui/views/widget/native_widget_aura_window_observer.h"
#include "ui/views/widget/native_widget_delegate.h"
#include "ui/views/widget/root_view.h"
#include "ui/views/widget/tooltip_manager_aura.h"
#include "ui/views/widget/widget_aura_utils.h"
#include "ui/views/widget/widget_delegate.h"

#if defined(OS_WIN)
#include "base/win/scoped_gdi_object.h"
#include "base/win/win_util.h"
#include "ui/base/l10n/l10n_util_win.h"
#include "ui/views/widget/desktop_aura/desktop_root_window_host_win.h"
#endif

#if !defined(OS_CHROMEOS)
#include "ui/views/widget/desktop_aura/desktop_root_window_host.h"
#endif

namespace views {

namespace {

void SetRestoreBounds(aura::Window* window, const gfx::Rect& bounds) {
  window->SetProperty(aura::client::kRestoreBoundsKey, new gfx::Rect(bounds));
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetAura, public:

NativeWidgetAura::NativeWidgetAura(internal::NativeWidgetDelegate* delegate)
    : delegate_(delegate),
      window_(new aura::Window(this)),
      ownership_(Widget::InitParams::NATIVE_WIDGET_OWNS_WIDGET),
      close_widget_factory_(this),
      can_activate_(true),
      destroying_(false),
      cursor_(gfx::kNullCursor),
      saved_window_state_(ui::SHOW_STATE_DEFAULT) {
  aura::client::SetFocusChangeObserver(window_, this);
  aura::client::SetActivationChangeObserver(window_, this);
}

// static
gfx::Font NativeWidgetAura::GetWindowTitleFont() {
#if defined(OS_WIN)
  NONCLIENTMETRICS ncm;
  base::win::GetNonClientMetrics(&ncm);
  l10n_util::AdjustUIFont(&(ncm.lfCaptionFont));
  base::win::ScopedHFONT caption_font(CreateFontIndirect(&(ncm.lfCaptionFont)));
  return gfx::Font(caption_font);
#else
  return gfx::Font();
#endif
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetAura, internal::NativeWidgetPrivate implementation:

void NativeWidgetAura::InitNativeWidget(const Widget::InitParams& params) {
  // Aura needs to know which desktop (Ash or regular) will manage this widget.
  // See Widget::InitParams::context for details.
  DCHECK(params.parent || params.context);

  ownership_ = params.ownership;

  window_->set_user_data(this);
  window_->SetType(GetAuraWindowTypeForWidgetType(params.type));
  window_->SetProperty(aura::client::kShowStateKey, params.show_state);
  if (params.type == Widget::InitParams::TYPE_BUBBLE)
    aura::client::SetHideOnDeactivate(window_, true);
  window_->SetTransparent(params.transparent);
  window_->Init(params.layer_type);
  if (params.type == Widget::InitParams::TYPE_CONTROL)
    window_->Show();

  delegate_->OnNativeWidgetCreated();

  gfx::Rect window_bounds = params.bounds;
  gfx::NativeView parent = params.parent;
  gfx::NativeView context = params.context;
  if (!params.child) {
    // Set up the transient child before the window is added. This way the
    // LayoutManager knows the window has a transient parent.
    if (parent && parent->type() != aura::client::WINDOW_TYPE_UNKNOWN) {
      parent->AddTransientChild(window_);
      if (!context)
        context = parent;
      parent = NULL;
    }
    // SetAlwaysOnTop before SetParent so that always-on-top container is used.
    SetAlwaysOnTop(params.keep_on_top);
    // Make sure we have a real |window_bounds|.
    if (parent && window_bounds == gfx::Rect()) {
      // If a parent is specified but no bounds are given,
      // use the origin of the parent's display so that the widget
      // will be added to the same display as the parent.
      gfx::Rect bounds = gfx::Screen::GetScreenFor(parent)->
          GetDisplayNearestWindow(parent).bounds();
      window_bounds.set_origin(bounds.origin());
    }
  }

  if (parent) {
    parent->AddChild(window_);
  } else {
    window_->SetDefaultParentByRootWindow(context->GetRootWindow(),
                                          window_bounds);
  }

  // Wait to set the bounds until we have a parent. That way we can know our
  // true state/bounds (the LayoutManager may enforce a particular
  // state/bounds).
  if (IsMaximized())
    SetRestoreBounds(window_, window_bounds);
  else
    SetBounds(window_bounds);
  window_->set_ignore_events(!params.accept_events);
  can_activate_ =
      params.can_activate && params.type != Widget::InitParams::TYPE_CONTROL;
  DCHECK(GetWidget()->GetRootView());
#if !defined(OS_MACOSX)
  if (params.type != Widget::InitParams::TYPE_TOOLTIP)
    tooltip_manager_.reset(new views::TooltipManagerAura(window_, GetWidget()));
#endif  // !defined(OS_MACOSX)

  drop_helper_.reset(new DropHelper(GetWidget()->GetRootView()));
  if (params.type != Widget::InitParams::TYPE_TOOLTIP &&
      params.type != Widget::InitParams::TYPE_POPUP) {
    aura::client::SetDragDropDelegate(window_, this);
  }

  aura::client::SetActivationDelegate(window_, this);

  window_->SetProperty(aura::client::kCanMaximizeKey,
                       GetWidget()->widget_delegate()->CanMaximize());
  window_->SetProperty(aura::client::kCanResizeKey,
                       GetWidget()->widget_delegate()->CanResize());
}

NonClientFrameView* NativeWidgetAura::CreateNonClientFrameView() {
  return NULL;
}

bool NativeWidgetAura::ShouldUseNativeFrame() const {
  // There is only one frame type for aura.
  return false;
}

void NativeWidgetAura::FrameTypeChanged() {
  // This is called when the Theme has changed; forward the event to the root
  // widget.
  GetWidget()->ThemeChanged();
  GetWidget()->GetRootView()->SchedulePaint();
}

Widget* NativeWidgetAura::GetWidget() {
  return delegate_->AsWidget();
}

const Widget* NativeWidgetAura::GetWidget() const {
  return delegate_->AsWidget();
}

gfx::NativeView NativeWidgetAura::GetNativeView() const {
  return window_;
}

gfx::NativeWindow NativeWidgetAura::GetNativeWindow() const {
  return window_;
}

Widget* NativeWidgetAura::GetTopLevelWidget() {
  NativeWidgetPrivate* native_widget = GetTopLevelNativeWidget(GetNativeView());
  return native_widget ? native_widget->GetWidget() : NULL;
}

const ui::Compositor* NativeWidgetAura::GetCompositor() const {
  return window_->layer()->GetCompositor();
}

ui::Compositor* NativeWidgetAura::GetCompositor() {
  return window_->layer()->GetCompositor();
}

gfx::Vector2d NativeWidgetAura::CalculateOffsetToAncestorWithLayer(
    ui::Layer** layer_parent) {
  if (layer_parent)
    *layer_parent = window_->layer();
  return gfx::Vector2d();
}

void NativeWidgetAura::ViewRemoved(View* view) {
  DCHECK(drop_helper_.get() != NULL);
  drop_helper_->ResetTargetViewIfEquals(view);
}

void NativeWidgetAura::SetNativeWindowProperty(const char* name, void* value) {
  if (window_)
    window_->SetNativeWindowProperty(name, value);
}

void* NativeWidgetAura::GetNativeWindowProperty(const char* name) const {
  return window_ ? window_->GetNativeWindowProperty(name) : NULL;
}

TooltipManager* NativeWidgetAura::GetTooltipManager() const {
  return tooltip_manager_.get();
}

void NativeWidgetAura::SetCapture() {
  window_->SetCapture();
}

void NativeWidgetAura::ReleaseCapture() {
  window_->ReleaseCapture();
}

bool NativeWidgetAura::HasCapture() const {
  return window_->HasCapture();
}

InputMethod* NativeWidgetAura::CreateInputMethod() {
  aura::RootWindow* root_window = window_->GetRootWindow();
  ui::InputMethod* host =
      root_window->GetProperty(aura::client::kRootWindowInputMethodKey);
  return new InputMethodBridge(this, host, true);
}

internal::InputMethodDelegate* NativeWidgetAura::GetInputMethodDelegate() {
  return this;
}

void NativeWidgetAura::CenterWindow(const gfx::Size& size) {
  gfx::Rect parent_bounds(window_->parent()->GetBoundsInRootWindow());
  // When centering window, we take the intersection of the host and
  // the parent. We assume the root window represents the visible
  // rect of a single screen.
  gfx::Rect work_area = gfx::Screen::GetScreenFor(window_)->
      GetDisplayNearestWindow(window_).work_area();

  aura::client::ScreenPositionClient* screen_position_client =
      aura::client::GetScreenPositionClient(window_->GetRootWindow());
  if (screen_position_client) {
    gfx::Point origin = work_area.origin();
    screen_position_client->ConvertPointFromScreen(window_->GetRootWindow(),
                                                   &origin);
    work_area.set_origin(origin);
  }

  parent_bounds.Intersect(work_area);

  // If |window_|'s transient parent's bounds are big enough to fit it, then we
  // center it with respect to the transient parent.
  if (window_->transient_parent()) {
    gfx::Rect transient_parent_rect = window_->transient_parent()->
        GetBoundsInRootWindow();
    transient_parent_rect.Intersect(work_area);
    if (transient_parent_rect.height() >= size.height() &&
        transient_parent_rect.width() >= size.width())
      parent_bounds = transient_parent_rect;
  }

  gfx::Rect window_bounds(
      parent_bounds.x() + (parent_bounds.width() - size.width()) / 2,
      parent_bounds.y() + (parent_bounds.height() - size.height()) / 2,
      size.width(),
      size.height());
  // Don't size the window bigger than the parent, otherwise the user may not be
  // able to close or move it.
  window_bounds.AdjustToFit(parent_bounds);

  // Convert the bounds back relative to the parent.
  gfx::Point origin = window_bounds.origin();
  aura::Window::ConvertPointToTarget(window_->GetRootWindow(),
      window_->parent(), &origin);
  window_bounds.set_origin(origin);
  window_->SetBounds(window_bounds);
}

void NativeWidgetAura::GetWindowPlacement(
    gfx::Rect* bounds,
    ui::WindowShowState* show_state) const {
  // The interface specifies returning restored bounds, not current bounds.
  *bounds = GetRestoredBounds();
  *show_state = window_->GetProperty(aura::client::kShowStateKey);
}

void NativeWidgetAura::SetWindowTitle(const string16& title) {
  window_->set_title(title);
}

void NativeWidgetAura::SetWindowIcons(const gfx::ImageSkia& window_icon,
                                      const gfx::ImageSkia& app_icon) {
  // Aura doesn't have window icons.
}

void NativeWidgetAura::InitModalType(ui::ModalType modal_type) {
  if (modal_type != ui::MODAL_TYPE_NONE)
    window_->SetProperty(aura::client::kModalKey, modal_type);
}

gfx::Rect NativeWidgetAura::GetWindowBoundsInScreen() const {
  return window_->GetBoundsInScreen();
}

gfx::Rect NativeWidgetAura::GetClientAreaBoundsInScreen() const {
  // View-to-screen coordinate system transformations depend on this returning
  // the full window bounds, for example View::ConvertPointToScreen().
  return window_->GetBoundsInScreen();
}

gfx::Rect NativeWidgetAura::GetRestoredBounds() const {
  // Restored bounds should only be relevant if the window is minimized or
  // maximized. However, in some places the code expects GetRestoredBounds()
  // to return the current window bounds if the window is not in either state.
  if (IsMinimized() || IsMaximized() || IsFullscreen()) {
    // Restore bounds are in screen coordinates, no need to convert.
    gfx::Rect* restore_bounds =
        window_->GetProperty(aura::client::kRestoreBoundsKey);
    if (restore_bounds)
      return *restore_bounds;
  }
  return window_->GetBoundsInScreen();
}

void NativeWidgetAura::SetBounds(const gfx::Rect& bounds) {
  aura::RootWindow* root = window_->GetRootWindow();
  if (root) {
    aura::client::ScreenPositionClient* screen_position_client =
        aura::client::GetScreenPositionClient(root);
    if (screen_position_client) {
      gfx::Display dst_display =
          gfx::Screen::GetScreenFor(window_)->GetDisplayMatching(bounds);
      screen_position_client->SetBounds(window_, bounds, dst_display);
      return;
    }
  }
  window_->SetBounds(bounds);
}

void NativeWidgetAura::SetSize(const gfx::Size& size) {
  window_->SetBounds(gfx::Rect(window_->bounds().origin(), size));
}

void NativeWidgetAura::StackAbove(gfx::NativeView native_view) {
  if (window_->parent() && window_->parent() == native_view->parent())
    window_->parent()->StackChildAbove(window_, native_view);
}

void NativeWidgetAura::StackAtTop() {
  window_->parent()->StackChildAtTop(window_);
}

void NativeWidgetAura::StackBelow(gfx::NativeView native_view) {
  if (window_->parent() && window_->parent() == native_view->parent())
    window_->parent()->StackChildBelow(window_, native_view);
}

void NativeWidgetAura::SetShape(gfx::NativeRegion region) {
  // No need for this. Just delete and ignore.
  delete region;
}

void NativeWidgetAura::Close() {
  // |window_| may already be deleted by parent window. This can happen
  // when this widget is child widget or has transient parent
  // and ownership is WIDGET_OWNS_NATIVE_WIDGET.
  DCHECK(window_ ||
         ownership_ == Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET);
  if (window_) {
    window_->SuppressPaint();
    Hide();
    window_->SetProperty(aura::client::kModalKey, ui::MODAL_TYPE_NONE);
  }

  if (!close_widget_factory_.HasWeakPtrs()) {
    base::MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(&NativeWidgetAura::CloseNow,
                   close_widget_factory_.GetWeakPtr()));
  }
}

void NativeWidgetAura::CloseNow() {
  delete window_;
}

void NativeWidgetAura::Show() {
  ShowWithWindowState(ui::SHOW_STATE_INACTIVE);
}

void NativeWidgetAura::Hide() {
  window_->Hide();
}

void NativeWidgetAura::ShowMaximizedWithBounds(
    const gfx::Rect& restored_bounds) {
  ShowWithWindowState(ui::SHOW_STATE_MAXIMIZED);
  SetRestoreBounds(window_, restored_bounds);
}

void NativeWidgetAura::ShowWithWindowState(ui::WindowShowState state) {
  if (state == ui::SHOW_STATE_MAXIMIZED || state == ui::SHOW_STATE_FULLSCREEN)
    window_->SetProperty(aura::client::kShowStateKey, state);
  window_->Show();
  if (can_activate_) {
    if (state != ui::SHOW_STATE_INACTIVE)
      Activate();
    // SetInitialFocus() should be always be called, even for
    // SHOW_STATE_INACTIVE. When a frameless modal dialog is created by
    // a widget of TYPE_WINDOW_FRAMELESS, Widget::Show() will call into
    // this function with the window state SHOW_STATE_INACTIVE,
    // SetInitialFoucs() has to be called so that the dialog can get focus.
    // This also matches NativeWidgetWin which invokes SetInitialFocus
    // regardless of show state.
    SetInitialFocus();
  }
}

bool NativeWidgetAura::IsVisible() const {
  return window_->IsVisible();
}

void NativeWidgetAura::Activate() {
  // We don't necessarily have a root window yet. This can happen with
  // constrained windows.
  if (window_->GetRootWindow()) {
    aura::client::GetActivationClient(window_->GetRootWindow())->ActivateWindow(
        window_);
  }
  if (window_->GetProperty(aura::client::kDrawAttentionKey))
    window_->SetProperty(aura::client::kDrawAttentionKey, false);
}

void NativeWidgetAura::Deactivate() {
  aura::client::GetActivationClient(window_->GetRootWindow())->DeactivateWindow(
      window_);
}

bool NativeWidgetAura::IsActive() const {
  return aura::client::GetActivationClient(window_->GetRootWindow())->
      GetActiveWindow() == window_;
}

void NativeWidgetAura::SetAlwaysOnTop(bool on_top) {
  window_->SetProperty(aura::client::kAlwaysOnTopKey, on_top);
}

void NativeWidgetAura::Maximize() {
  window_->SetProperty(aura::client::kShowStateKey, ui::SHOW_STATE_MAXIMIZED);
}

void NativeWidgetAura::Minimize() {
  window_->SetProperty(aura::client::kShowStateKey, ui::SHOW_STATE_MINIMIZED);
}

bool NativeWidgetAura::IsMaximized() const {
  return window_->GetProperty(aura::client::kShowStateKey) ==
      ui::SHOW_STATE_MAXIMIZED;
}

bool NativeWidgetAura::IsMinimized() const {
  return window_->GetProperty(aura::client::kShowStateKey) ==
      ui::SHOW_STATE_MINIMIZED;
}

void NativeWidgetAura::Restore() {
  window_->SetProperty(aura::client::kShowStateKey, ui::SHOW_STATE_NORMAL);
}

void NativeWidgetAura::SetFullscreen(bool fullscreen) {
  if (IsFullscreen() == fullscreen)
    return;  // Nothing to do.

  // Save window state before entering full screen so that it could restored
  // when exiting full screen.
  if (fullscreen)
    saved_window_state_ = window_->GetProperty(aura::client::kShowStateKey);

  window_->SetProperty(
      aura::client::kShowStateKey,
      fullscreen ? ui::SHOW_STATE_FULLSCREEN : saved_window_state_);
}

bool NativeWidgetAura::IsFullscreen() const {
  return window_->GetProperty(aura::client::kShowStateKey) ==
      ui::SHOW_STATE_FULLSCREEN;
}

void NativeWidgetAura::SetOpacity(unsigned char opacity) {
  window_->layer()->SetOpacity(opacity / 255.0);
}

void NativeWidgetAura::SetUseDragFrame(bool use_drag_frame) {
  NOTIMPLEMENTED();
}

void NativeWidgetAura::FlashFrame(bool flash) {
  window_->SetProperty(aura::client::kDrawAttentionKey, flash);
}

void NativeWidgetAura::RunShellDrag(View* view,
                                    const ui::OSExchangeData& data,
                                    const gfx::Point& location,
                                    int operation,
                                    ui::DragDropTypes::DragEventSource source) {
  views::RunShellDrag(window_, data, location, operation, source);
}

void NativeWidgetAura::SchedulePaintInRect(const gfx::Rect& rect) {
  if (window_)
    window_->SchedulePaintInRect(rect);
}

void NativeWidgetAura::SetCursor(gfx::NativeCursor cursor) {
  cursor_ = cursor;
  aura::client::CursorClient* cursor_client =
      aura::client::GetCursorClient(window_->GetRootWindow());
  if (cursor_client)
    cursor_client->SetCursor(cursor);
}

void NativeWidgetAura::ClearNativeFocus() {
  aura::client::FocusClient* client = aura::client::GetFocusClient(window_);
  if (window_ && client && window_->Contains(client->GetFocusedWindow()))
    client->ResetFocusWithinActiveWindow(window_);
}

gfx::Rect NativeWidgetAura::GetWorkAreaBoundsInScreen() const {
  return gfx::Screen::GetScreenFor(GetNativeView())->
      GetDisplayNearestWindow(GetNativeView()).work_area();
}

void NativeWidgetAura::SetInactiveRenderingDisabled(bool value) {
  if (!value) {
    active_window_observer_.reset();
  } else {
    active_window_observer_.reset(
        new NativeWidgetAuraWindowObserver(window_, delegate_));
  }
}

Widget::MoveLoopResult NativeWidgetAura::RunMoveLoop(
    const gfx::Vector2d& drag_offset,
    Widget::MoveLoopSource source) {
  if (window_->parent() &&
      aura::client::GetWindowMoveClient(window_->parent())) {
    SetCapture();
    aura::client::WindowMoveSource window_move_source =
        source == Widget::MOVE_LOOP_SOURCE_MOUSE ?
        aura::client::WINDOW_MOVE_SOURCE_MOUSE :
        aura::client::WINDOW_MOVE_SOURCE_TOUCH;
    if (aura::client::GetWindowMoveClient(window_->parent())->RunMoveLoop(
            window_, drag_offset, window_move_source) ==
        aura::client::MOVE_SUCCESSFUL) {
      return Widget::MOVE_LOOP_SUCCESSFUL;
    }
  }
  return Widget::MOVE_LOOP_CANCELED;
}

void NativeWidgetAura::EndMoveLoop() {
  if (window_->parent() &&
      aura::client::GetWindowMoveClient(window_->parent())) {
    aura::client::GetWindowMoveClient(window_->parent())->EndMoveLoop();
  }
}

void NativeWidgetAura::SetVisibilityChangedAnimationsEnabled(bool value) {
  window_->SetProperty(aura::client::kAnimationsDisabledKey, !value);
}

ui::NativeTheme* NativeWidgetAura::GetNativeTheme() const {
#if !defined(OS_CHROMEOS)
  if (window_)
    return DesktopRootWindowHost::GetNativeTheme(window_);
#endif
  return ui::NativeThemeAura::instance();
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetAura, views::InputMethodDelegate implementation:

void NativeWidgetAura::DispatchKeyEventPostIME(const ui::KeyEvent& key) {
  FocusManager* focus_manager = GetWidget()->GetFocusManager();
  delegate_->OnKeyEvent(const_cast<ui::KeyEvent*>(&key));
  if (key.handled() || !focus_manager)
    return;
  focus_manager->OnKeyEvent(key);
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetAura, aura::WindowDelegate implementation:

gfx::Size NativeWidgetAura::GetMinimumSize() const {
  return delegate_->GetMinimumSize();
}

gfx::Size NativeWidgetAura::GetMaximumSize() const {
  return delegate_->GetMaximumSize();
}

void NativeWidgetAura::OnBoundsChanged(const gfx::Rect& old_bounds,
                                       const gfx::Rect& new_bounds) {
  if (old_bounds.origin() != new_bounds.origin())
    delegate_->OnNativeWidgetMove();
  if (old_bounds.size() != new_bounds.size())
    delegate_->OnNativeWidgetSizeChanged(new_bounds.size());
}

gfx::NativeCursor NativeWidgetAura::GetCursor(const gfx::Point& point) {
  return cursor_;
}

int NativeWidgetAura::GetNonClientComponent(const gfx::Point& point) const {
  return delegate_->GetNonClientComponent(point);
}

bool NativeWidgetAura::ShouldDescendIntoChildForEventHandling(
      aura::Window* child,
      const gfx::Point& location) {
  views::WidgetDelegate* widget_delegate = GetWidget()->widget_delegate();
  if (widget_delegate &&
      !widget_delegate->ShouldDescendIntoChildForEventHandling(child, location))
    return false;

  // Don't descend into |child| if there is a view with a Layer that contains
  // the point and is stacked above |child|s layer.
  typedef std::vector<ui::Layer*> Layers;
  const Layers& root_layers(delegate_->GetRootLayers());
  if (root_layers.empty())
    return true;

  Layers::const_iterator child_layer_iter(
      std::find(window_->layer()->children().begin(),
                window_->layer()->children().end(), child->layer()));
  if (child_layer_iter == window_->layer()->children().end())
    return true;

  for (std::vector<ui::Layer*>::const_reverse_iterator i = root_layers.rbegin();
       i != root_layers.rend(); ++i) {
    ui::Layer* layer = *i;
    if (layer->visible() && layer->bounds().Contains(location)) {
      Layers::const_iterator root_layer_iter(
          std::find(window_->layer()->children().begin(),
                    window_->layer()->children().end(), layer));
      if (root_layer_iter > child_layer_iter)
        return false;
    }
  }
  return true;
}

bool NativeWidgetAura::CanFocus() {
  return can_activate_;
}

void NativeWidgetAura::OnCaptureLost() {
  delegate_->OnMouseCaptureLost();
}

void NativeWidgetAura::OnPaint(gfx::Canvas* canvas) {
  delegate_->OnNativeWidgetPaint(canvas);
}

void NativeWidgetAura::OnDeviceScaleFactorChanged(float device_scale_factor) {
  // Repainting with new scale factor will paint the content at the right scale.
}

void NativeWidgetAura::OnWindowDestroying() {
  delegate_->OnNativeWidgetDestroying();

  // If the aura::Window is destroyed, we can no longer show tooltips.
  tooltip_manager_.reset();
}

void NativeWidgetAura::OnWindowDestroyed() {
  window_ = NULL;
  delegate_->OnNativeWidgetDestroyed();
  if (ownership_ == Widget::InitParams::NATIVE_WIDGET_OWNS_WIDGET)
    delete this;
}

void NativeWidgetAura::OnWindowTargetVisibilityChanged(bool visible) {
  delegate_->OnNativeWidgetVisibilityChanged(visible);
}

bool NativeWidgetAura::HasHitTestMask() const {
  return delegate_->HasHitTestMask();
}

void NativeWidgetAura::GetHitTestMask(gfx::Path* mask) const {
  DCHECK(mask);
  delegate_->GetHitTestMask(mask);
}

scoped_refptr<ui::Texture> NativeWidgetAura::CopyTexture() {
  // The layer we create doesn't have an external texture, so this should never
  // get invoked.
  NOTREACHED();
  return scoped_refptr<ui::Texture>();
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetAura, ui::EventHandler implementation:

void NativeWidgetAura::OnKeyEvent(ui::KeyEvent* event) {
  if (event->is_char()) {
    // If a ui::InputMethod object is attached to the root window, character
    // events are handled inside the object and are not passed to this function.
    // If such object is not attached, character events might be sent (e.g. on
    // Windows). In this case, we just skip these.
    return;
  }
  // Renderer may send a key event back to us if the key event wasn't handled,
  // and the window may be invisible by that time.
  if (!window_->IsVisible())
    return;
  GetWidget()->GetInputMethod()->DispatchKeyEvent(*event);
  event->SetHandled();
}

void NativeWidgetAura::OnMouseEvent(ui::MouseEvent* event) {
  DCHECK(window_->IsVisible());
  if (event->type() == ui::ET_MOUSEWHEEL) {
    delegate_->OnMouseEvent(event);
    if (event->handled())
      return;
  }

  if (tooltip_manager_.get())
    tooltip_manager_->UpdateTooltip();
  delegate_->OnMouseEvent(event);
}

void NativeWidgetAura::OnScrollEvent(ui::ScrollEvent* event) {
  delegate_->OnScrollEvent(event);
}

void NativeWidgetAura::OnTouchEvent(ui::TouchEvent* event) {
  DCHECK(window_->IsVisible());
  delegate_->OnTouchEvent(event);
}

void NativeWidgetAura::OnGestureEvent(ui::GestureEvent* event) {
  DCHECK(window_->IsVisible());
  delegate_->OnGestureEvent(event);
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetAura, aura::client::ActivationDelegate implementation:

bool NativeWidgetAura::ShouldActivate() const {
  return can_activate_ && delegate_->CanActivate();
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetAura, aura::client::ActivationChangeObserver implementation:

void NativeWidgetAura::OnWindowActivated(aura::Window* gained_active,
                                         aura::Window* lost_active) {
  DCHECK(window_ == gained_active || window_ == lost_active);
  if (GetWidget()->GetFocusManager()) {
    if (window_ == gained_active)
      GetWidget()->GetFocusManager()->RestoreFocusedView();
    else if (window_ == lost_active)
      GetWidget()->GetFocusManager()->StoreFocusedView(true);
  }
  delegate_->OnNativeWidgetActivationChanged(window_ == gained_active);
  if (IsVisible() && GetWidget()->non_client_view())
    GetWidget()->non_client_view()->SchedulePaint();
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetAura, aura::client::FocusChangeObserver:

void NativeWidgetAura::OnWindowFocused(aura::Window* gained_focus,
                                       aura::Window* lost_focus) {
  if (window_ == gained_focus) {
    // In aura, it is possible for child native widgets to take input and focus,
    // this differs from the behavior on windows.
    if (GetWidget()->GetInputMethod())  // Null in tests.
      GetWidget()->GetInputMethod()->OnFocus();
    delegate_->OnNativeFocus(lost_focus);
  } else if (window_ == lost_focus) {
    // GetInputMethod() recreates the input method if it's previously been
    // destroyed.  If we get called during destruction, the input method will be
    // gone, and creating a new one and telling it that we lost the focus will
    // trigger a DCHECK (the new input method doesn't think that we have the
    // focus and doesn't expect a blur).  OnBlur() shouldn't be called during
    // destruction unless WIDGET_OWNS_NATIVE_WIDGET is set (which is just the
    // case in tests).
    if (!destroying_) {
      if (GetWidget()->GetInputMethod())
        GetWidget()->GetInputMethod()->OnBlur();
    } else {
      DCHECK_EQ(ownership_, Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET);
    }

    aura::client::FocusClient* client = aura::client::GetFocusClient(window_);
    if (client)  // NULL during destruction of aura::Window.
      delegate_->OnNativeBlur(client->GetFocusedWindow());
  }
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetAura, aura::WindowDragDropDelegate implementation:

void NativeWidgetAura::OnDragEntered(const ui::DropTargetEvent& event) {
  DCHECK(drop_helper_.get() != NULL);
  last_drop_operation_ = drop_helper_->OnDragOver(event.data(),
      event.location(), event.source_operations());
}

int NativeWidgetAura::OnDragUpdated(const ui::DropTargetEvent& event) {
  DCHECK(drop_helper_.get() != NULL);
  last_drop_operation_ = drop_helper_->OnDragOver(event.data(),
      event.location(), event.source_operations());
  return last_drop_operation_;
}

void NativeWidgetAura::OnDragExited() {
  DCHECK(drop_helper_.get() != NULL);
  drop_helper_->OnDragExit();
}

int NativeWidgetAura::OnPerformDrop(const ui::DropTargetEvent& event) {
  DCHECK(drop_helper_.get() != NULL);
  return drop_helper_->OnDrop(event.data(), event.location(),
      last_drop_operation_);
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetAura, protected:

NativeWidgetAura::~NativeWidgetAura() {
  destroying_ = true;
  if (ownership_ == Widget::InitParams::NATIVE_WIDGET_OWNS_WIDGET)
    delete delegate_;
  else
    CloseNow();
}

////////////////////////////////////////////////////////////////////////////////
// NativeWidgetAura, private:

void NativeWidgetAura::SetInitialFocus() {
  // The window does not get keyboard messages unless we focus it.
  if (!GetWidget()->SetInitialFocus())
    window_->Focus();
}

////////////////////////////////////////////////////////////////////////////////
// Widget, public:

// static
void Widget::NotifyLocaleChanged() {
  // Deliberately not implemented.
}

#if defined(OS_WIN)
namespace {
BOOL CALLBACK WindowCallbackProc(HWND hwnd, LPARAM lParam) {
  aura::Window* root_window =
      DesktopRootWindowHostWin::GetContentWindowForHWND(hwnd);
  if (root_window) {
    Widget* widget = Widget::GetWidgetForNativeView(root_window);
    if (widget && widget->is_secondary_widget())
      // To avoid the delay in shutdown caused by using Close which may wait
      // for animations, use CloseNow. Because this is only used on secondary
      // widgets it seems relatively safe to skip the extra processing of
      // Close.
      widget->CloseNow();
  }
  return TRUE;
}
}  // namespace
#endif

// static
void Widget::CloseAllSecondaryWidgets() {
#if defined(OS_WIN)
  EnumThreadWindows(GetCurrentThreadId(), WindowCallbackProc, 0);
#endif
}

bool Widget::ConvertRect(const Widget* source,
                         const Widget* target,
                         gfx::Rect* rect) {
  return false;
}

namespace internal {

////////////////////////////////////////////////////////////////////////////////
// internal::NativeWidgetPrivate, public:

// static
NativeWidgetPrivate* NativeWidgetPrivate::CreateNativeWidget(
    internal::NativeWidgetDelegate* delegate) {
  return new NativeWidgetAura(delegate);
}

// static
NativeWidgetPrivate* NativeWidgetPrivate::GetNativeWidgetForNativeView(
    gfx::NativeView native_view) {
  return reinterpret_cast<NativeWidgetPrivate*>(native_view->user_data());
}

// static
NativeWidgetPrivate* NativeWidgetPrivate::GetNativeWidgetForNativeWindow(
    gfx::NativeWindow native_window) {
  return reinterpret_cast<NativeWidgetPrivate*>(native_window->user_data());
}

// static
NativeWidgetPrivate* NativeWidgetPrivate::GetTopLevelNativeWidget(
    gfx::NativeView native_view) {
  aura::Window* window = native_view;
  NativeWidgetPrivate* top_level_native_widget = NULL;
  while (window) {
    NativeWidgetPrivate* native_widget = GetNativeWidgetForNativeView(window);
    if (native_widget)
      top_level_native_widget = native_widget;
    window = window->parent();
  }
  return top_level_native_widget;
}

// static
void NativeWidgetPrivate::GetAllChildWidgets(gfx::NativeView native_view,
                                             Widget::Widgets* children) {
  {
    // Code expects widget for |native_view| to be added to |children|.
    NativeWidgetPrivate* native_widget = static_cast<NativeWidgetPrivate*>(
        GetNativeWidgetForNativeView(native_view));
    if (native_widget && native_widget->GetWidget())
      children->insert(native_widget->GetWidget());
  }

  const aura::Window::Windows& child_windows = native_view->children();
  for (aura::Window::Windows::const_iterator i = child_windows.begin();
       i != child_windows.end(); ++i) {
    NativeWidgetAura* native_widget =
        static_cast<NativeWidgetAura*>(GetNativeWidgetForNativeView(*i));
    if (native_widget)
      children->insert(native_widget->GetWidget());
  }
}

// static
void NativeWidgetPrivate::ReparentNativeView(gfx::NativeView native_view,
                                             gfx::NativeView new_parent) {
  DCHECK(native_view != new_parent);

  gfx::NativeView previous_parent = native_view->parent();
  if (previous_parent == new_parent)
    return;

  Widget::Widgets widgets;
  GetAllChildWidgets(native_view, &widgets);

  // First notify all the widgets that they are being disassociated
  // from their previous parent.
  for (Widget::Widgets::iterator it = widgets.begin();
      it != widgets.end(); ++it) {
    (*it)->NotifyNativeViewHierarchyChanged(false, previous_parent);
  }

  if (new_parent) {
    new_parent->AddChild(native_view);
  } else {
    // The following looks weird, but it's the equivalent of what aura has
    // always done. (The previous behaviour of aura::Window::SetParent() used
    // NULL as a special value that meant ask the StackingClient where things
    // should go.)
    //
    // This probably isn't strictly correct, but its an invariant that a Window
    // in use will be attached to a RootWindow, so we can't just call
    // RemoveChild here. The only possible thing that could assign a RootWindow
    // in this case is the stacking client of the current RootWindow. This
    // matches our previous behaviour; the global stacking client would almost
    // always reattach the window to the same RootWindow.
    aura::RootWindow* root_window = native_view->GetRootWindow();
    native_view->SetDefaultParentByRootWindow(
        root_window, root_window->GetBoundsInScreen());
  }

  // And now, notify them that they have a brand new parent.
  for (Widget::Widgets::iterator it = widgets.begin();
      it != widgets.end(); ++it) {
    (*it)->NotifyNativeViewHierarchyChanged(true, new_parent);
  }
}

// static
bool NativeWidgetPrivate::IsMouseButtonDown() {
  return aura::Env::GetInstance()->is_mouse_button_down();
}

// static
bool NativeWidgetPrivate::IsTouchDown() {
  return aura::Env::GetInstance()->is_touch_down();
}

}  // namespace internal
}  // namespace views
