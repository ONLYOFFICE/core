// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"

#include "base/bind.h"
#include "ui/aura/client/activation_client.h"
#include "ui/aura/client/aura_constants.h"
#include "ui/aura/client/stacking_client.h"
#include "ui/aura/focus_manager.h"
#include "ui/aura/root_window.h"
#include "ui/aura/root_window_host.h"
#include "ui/aura/window.h"
#include "ui/aura/window_observer.h"
#include "ui/aura/window_property.h"
#include "ui/base/hit_test.h"
#include "ui/compositor/layer.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/display.h"
#include "ui/gfx/point_conversions.h"
#include "ui/gfx/screen.h"
#include "ui/gfx/size_conversions.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/corewm/compound_event_filter.h"
#include "ui/views/corewm/corewm_switches.h"
#include "ui/views/corewm/input_method_event_filter.h"
#include "ui/views/corewm/shadow_controller.h"
#include "ui/views/corewm/shadow_types.h"
#include "ui/views/corewm/tooltip_controller.h"
#include "ui/views/corewm/visibility_controller.h"
#include "ui/views/corewm/window_modality_controller.h"
#include "ui/views/drag_utils.h"
#include "ui/views/ime/input_method.h"
#include "ui/views/ime/input_method_bridge.h"
#include "ui/views/widget/desktop_aura/desktop_root_window_host.h"
#include "ui/views/widget/drop_helper.h"
#include "ui/views/widget/native_widget_aura_window_observer.h"
#include "ui/views/widget/root_view.h"
#include "ui/views/widget/tooltip_manager_aura.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_aura_utils.h"
#include "ui/views/widget/widget_delegate.h"

DECLARE_EXPORTED_WINDOW_PROPERTY_TYPE(VIEWS_EXPORT,
                                      views::DesktopNativeWidgetAura*);

namespace views {

DEFINE_WINDOW_PROPERTY_KEY(DesktopNativeWidgetAura*,
                           kDesktopNativeWidgetAuraKey, NULL);

namespace {

// This class provides functionality to create a top level fullscreen widget to
// host a child window.
class DesktopNativeWidgetFullscreenHandler : public aura::WindowObserver {
 public:
  // This function creates a full screen widget with the bounds passed in
  // which eventually becomes the parent of the child window passed in.
  static aura::Window* CreateParentWindow(aura::Window* child_window,
                                          const gfx::Rect& bounds) {
    // This instance will get deleted when the fullscreen widget is destroyed.
    DesktopNativeWidgetFullscreenHandler* full_screen_handler =
        new DesktopNativeWidgetFullscreenHandler;

    child_window->SetBounds(gfx::Rect(bounds.size()));

    Widget::InitParams init_params;
    init_params.type = Widget::InitParams::TYPE_WINDOW;
    init_params.bounds = bounds;
    init_params.ownership = Widget::InitParams::NATIVE_WIDGET_OWNS_WIDGET;
    init_params.layer_type = ui::LAYER_NOT_DRAWN;

    // This widget instance will get deleted when the fullscreen window is
    // destroyed.
    full_screen_handler->full_screen_widget_ = new Widget();
    full_screen_handler->full_screen_widget_->Init(init_params);

    full_screen_handler->full_screen_widget_->SetFullscreen(true);
    full_screen_handler->full_screen_widget_->Show();

    aura::Window* native_window =
        full_screen_handler->full_screen_widget_->GetNativeView();
    child_window->AddObserver(full_screen_handler);
    native_window->AddObserver(full_screen_handler);
    return native_window;
  }

  // aura::WindowObserver overrides
  virtual void OnWindowDestroying(aura::Window* window) OVERRIDE {
    window->RemoveObserver(this);

    // If the widget is being destroyed by the OS then we should not try and
    // destroy it again.
    if (full_screen_widget_ &&
        window == full_screen_widget_->GetNativeView()) {
      full_screen_widget_ = NULL;
      return;
    }

    if (full_screen_widget_) {
      DCHECK(full_screen_widget_->GetNativeView());
      full_screen_widget_->GetNativeView()->RemoveObserver(this);
      // When we receive a notification that the child of the fullscreen window
      // created above is being destroyed we go ahead and initiate the
      // destruction of the corresponding widget.
      full_screen_widget_->Close();
      full_screen_widget_ = NULL;
    }
    delete this;
  }

 private:
  DesktopNativeWidgetFullscreenHandler()
      : full_screen_widget_(NULL) {}

  virtual ~DesktopNativeWidgetFullscreenHandler() {}

  Widget* full_screen_widget_;

  DISALLOW_COPY_AND_ASSIGN(DesktopNativeWidgetFullscreenHandler);
};

class DesktopNativeWidgetAuraStackingClient :
    public aura::client::StackingClient {
 public:
  explicit DesktopNativeWidgetAuraStackingClient(aura::RootWindow* root_window)
      : root_window_(root_window) {
    aura::client::SetStackingClient(root_window_, this);
  }
  virtual ~DesktopNativeWidgetAuraStackingClient() {
    aura::client::SetStackingClient(root_window_, NULL);
  }

  // Overridden from client::StackingClient:
  virtual aura::Window* GetDefaultParent(aura::Window* context,
                                         aura::Window* window,
                                         const gfx::Rect& bounds) OVERRIDE {
    if (window->GetProperty(aura::client::kShowStateKey) ==
            ui::SHOW_STATE_FULLSCREEN) {
      return DesktopNativeWidgetFullscreenHandler::CreateParentWindow(window,
                                                                      bounds);
    }
    return root_window_;
  }

 private:
  aura::RootWindow* root_window_;

  DISALLOW_COPY_AND_ASSIGN(DesktopNativeWidgetAuraStackingClient);
};

}  // namespace

////////////////////////////////////////////////////////////////////////////////
// DesktopNativeWidgetAura, public:

DesktopNativeWidgetAura::DesktopNativeWidgetAura(
    internal::NativeWidgetDelegate* delegate)
    : ownership_(Widget::InitParams::NATIVE_WIDGET_OWNS_WIDGET),
      close_widget_factory_(this),
      can_activate_(true),
      desktop_root_window_host_(NULL),
      window_(new aura::Window(this)),
      native_widget_delegate_(delegate),
      last_drop_operation_(ui::DragDropTypes::DRAG_NONE),
      restore_focus_on_activate_(false) {
  window_->SetProperty(kDesktopNativeWidgetAuraKey, this);
  aura::client::SetFocusChangeObserver(window_, this);
  aura::client::SetActivationChangeObserver(window_, this);
}

DesktopNativeWidgetAura::~DesktopNativeWidgetAura() {
  if (ownership_ == Widget::InitParams::NATIVE_WIDGET_OWNS_WIDGET)
    delete native_widget_delegate_;
  else
    CloseNow();

  stacking_client_.reset();  // Uses root_window_ at destruction.
  root_window_.reset();  // Uses input_method_event_filter_ at destruction.
  input_method_event_filter_.reset();
}

// static
DesktopNativeWidgetAura* DesktopNativeWidgetAura::ForWindow(
    aura::Window* window) {
  return window->GetProperty(kDesktopNativeWidgetAuraKey);
}

void DesktopNativeWidgetAura::OnHostClosed() {
  root_window_event_filter_->RemoveHandler(input_method_event_filter_.get());
  // This will, through a long list of callbacks, trigger |root_window_| going
  // away. See OnWindowDestroyed()
  delete window_;
}

void DesktopNativeWidgetAura::InstallInputMethodEventFilter(
    aura::RootWindow* root) {
  DCHECK(!input_method_event_filter_.get());

  // CEF sets focus to the window the user clicks down on.
  // TODO(beng): see if we can't do this some other way. CEF seems a heavy-
  //             handed way of accomplishing focus.
  // No event filter for aura::Env. Create CompoundEvnetFilter per RootWindow.
  root_window_event_filter_ = new corewm::CompoundEventFilter;
  // Pass ownership of the filter to the root_window.
  root->SetEventFilter(root_window_event_filter_);

  input_method_event_filter_.reset(
      new corewm::InputMethodEventFilter(root->GetAcceleratedWidget()));
  input_method_event_filter_->SetInputMethodPropertyInRootWindow(root);
  root_window_event_filter_->AddHandler(input_method_event_filter_.get());
}

////////////////////////////////////////////////////////////////////////////////
// DesktopNativeWidgetAura, internal::NativeWidgetPrivate implementation:

void DesktopNativeWidgetAura::InitNativeWidget(
    const Widget::InitParams& params) {
  ownership_ = params.ownership;

  window_->set_user_data(this);
  window_->SetType(GetAuraWindowTypeForWidgetType(params.type));
  window_->SetTransparent(true);
  window_->Init(params.layer_type);
  corewm::SetShadowType(window_, corewm::SHADOW_TYPE_NONE);
#if defined(OS_LINUX)  // TODO(scottmg): http://crbug.com/180071
  window_->Show();
#endif

  desktop_root_window_host_ = params.desktop_root_window_host ?
      params.desktop_root_window_host :
      DesktopRootWindowHost::Create(native_widget_delegate_,
                                    this, params.bounds);
  root_window_.reset(
      desktop_root_window_host_->Init(window_, params));
  stacking_client_.reset(
      new DesktopNativeWidgetAuraStackingClient(root_window_.get()));
  drop_helper_.reset(new DropHelper(
      static_cast<internal::RootView*>(GetWidget()->GetRootView())));
  aura::client::SetDragDropDelegate(window_, this);

  tooltip_manager_.reset(new views::TooltipManagerAura(window_, GetWidget()));
  tooltip_controller_.reset(
      new corewm::TooltipController(gfx::SCREEN_TYPE_NATIVE));
  aura::client::SetTooltipClient(root_window_.get(),
                                 tooltip_controller_.get());
  root_window_->AddPreTargetHandler(tooltip_controller_.get());

  if (params.type != Widget::InitParams::TYPE_WINDOW) {
    visibility_controller_.reset(new views::corewm::VisibilityController);
    aura::client::SetVisibilityClient(GetNativeView()->GetRootWindow(),
                                      visibility_controller_.get());
    views::corewm::SetChildWindowVisibilityChangesAnimated(
        GetNativeView()->GetRootWindow());
  }

  if (params.type == Widget::InitParams::TYPE_WINDOW) {
    window_modality_controller_.reset(
        new views::corewm::WindowModalityController);
    root_window_->AddPreTargetHandler(window_modality_controller_.get());
  }

  window_->Show();
  desktop_root_window_host_->InitFocus(window_);

  aura::client::SetActivationDelegate(window_, this);

  shadow_controller_.reset(
      new corewm::ShadowController(
          aura::client::GetActivationClient(root_window_.get())));
}

NonClientFrameView* DesktopNativeWidgetAura::CreateNonClientFrameView() {
  return desktop_root_window_host_->CreateNonClientFrameView();
}

bool DesktopNativeWidgetAura::ShouldUseNativeFrame() const {
  return desktop_root_window_host_->ShouldUseNativeFrame();
}

void DesktopNativeWidgetAura::FrameTypeChanged() {
  desktop_root_window_host_->FrameTypeChanged();
}

Widget* DesktopNativeWidgetAura::GetWidget() {
  return native_widget_delegate_->AsWidget();
}

const Widget* DesktopNativeWidgetAura::GetWidget() const {
  return native_widget_delegate_->AsWidget();
}

gfx::NativeView DesktopNativeWidgetAura::GetNativeView() const {
  return window_;
}

gfx::NativeWindow DesktopNativeWidgetAura::GetNativeWindow() const {
  return window_;
}

Widget* DesktopNativeWidgetAura::GetTopLevelWidget() {
  return GetWidget();
}

const ui::Compositor* DesktopNativeWidgetAura::GetCompositor() const {
  return window_->layer()->GetCompositor();
}

ui::Compositor* DesktopNativeWidgetAura::GetCompositor() {
  return window_->layer()->GetCompositor();
}

gfx::Vector2d DesktopNativeWidgetAura::CalculateOffsetToAncestorWithLayer(
      ui::Layer** layer_parent) {
  if (layer_parent)
    *layer_parent = window_->layer();
  return gfx::Vector2d();
}

void DesktopNativeWidgetAura::ViewRemoved(View* view) {
}

void DesktopNativeWidgetAura::SetNativeWindowProperty(const char* name,
                                                      void* value) {
  window_->SetNativeWindowProperty(name, value);
}

void* DesktopNativeWidgetAura::GetNativeWindowProperty(const char* name) const {
  return window_->GetNativeWindowProperty(name);
}

TooltipManager* DesktopNativeWidgetAura::GetTooltipManager() const {
  return tooltip_manager_.get();
}

void DesktopNativeWidgetAura::SetCapture() {
  window_->SetCapture();
  // aura::Window doesn't implicitly update capture on the RootWindowHost, so
  // we have to do that manually.
  if (!desktop_root_window_host_->HasCapture())
    window_->GetRootWindow()->SetNativeCapture();
}

void DesktopNativeWidgetAura::ReleaseCapture() {
  window_->ReleaseCapture();
  // aura::Window doesn't implicitly update capture on the RootWindowHost, so
  // we have to do that manually.
  if (desktop_root_window_host_->HasCapture())
    window_->GetRootWindow()->ReleaseNativeCapture();
}

bool DesktopNativeWidgetAura::HasCapture() const {
  return window_->HasCapture() && desktop_root_window_host_->HasCapture();
}

InputMethod* DesktopNativeWidgetAura::CreateInputMethod() {
  ui::InputMethod* host = input_method_event_filter_->input_method();
  return new InputMethodBridge(this, host, false);
}

internal::InputMethodDelegate*
    DesktopNativeWidgetAura::GetInputMethodDelegate() {
  return this;
}

void DesktopNativeWidgetAura::CenterWindow(const gfx::Size& size) {
  desktop_root_window_host_->CenterWindow(size);
}

void DesktopNativeWidgetAura::GetWindowPlacement(
      gfx::Rect* bounds,
      ui::WindowShowState* maximized) const {
  desktop_root_window_host_->GetWindowPlacement(bounds, maximized);
}

void DesktopNativeWidgetAura::SetWindowTitle(const string16& title) {
  desktop_root_window_host_->SetWindowTitle(title);
}

void DesktopNativeWidgetAura::SetWindowIcons(const gfx::ImageSkia& window_icon,
                                             const gfx::ImageSkia& app_icon) {
}

void DesktopNativeWidgetAura::InitModalType(ui::ModalType modal_type) {
  // 99% of the time, we should not be asked to create a
  // DesktopNativeWidgetAura that is modal. The case where this breaks down is
  // when there are no browser windows and a background extension tries to
  // display a simple alert dialog. (This case was masked because we used to
  // have a hidden RootWindow which was the parent of these modal dialogs; they
  // weren't displayed to the user.)
}

gfx::Rect DesktopNativeWidgetAura::GetWindowBoundsInScreen() const {
  return desktop_root_window_host_->GetWindowBoundsInScreen();
}

gfx::Rect DesktopNativeWidgetAura::GetClientAreaBoundsInScreen() const {
  return desktop_root_window_host_->GetClientAreaBoundsInScreen();
}

gfx::Rect DesktopNativeWidgetAura::GetRestoredBounds() const {
  return desktop_root_window_host_->GetRestoredBounds();
}

void DesktopNativeWidgetAura::SetBounds(const gfx::Rect& bounds) {
  float scale = 1;
  aura::RootWindow* root = root_window_.get();
  if (root) {
    scale = gfx::Screen::GetScreenFor(root)->
        GetDisplayNearestWindow(root).device_scale_factor();
  }
  gfx::Rect bounds_in_pixels(
      gfx::ToCeiledPoint(gfx::ScalePoint(bounds.origin(), scale)),
      gfx::ToFlooredSize(gfx::ScaleSize(bounds.size(), scale)));
  desktop_root_window_host_->AsRootWindowHost()->SetBounds(bounds_in_pixels);
}

void DesktopNativeWidgetAura::SetSize(const gfx::Size& size) {
  desktop_root_window_host_->SetSize(size);
}

void DesktopNativeWidgetAura::StackAbove(gfx::NativeView native_view) {
}

void DesktopNativeWidgetAura::StackAtTop() {
}

void DesktopNativeWidgetAura::StackBelow(gfx::NativeView native_view) {
}

void DesktopNativeWidgetAura::SetShape(gfx::NativeRegion shape) {
  desktop_root_window_host_->SetShape(shape);
}

void DesktopNativeWidgetAura::Close() {
  desktop_root_window_host_->Close();
  if (window_)
    window_->SuppressPaint();
}

void DesktopNativeWidgetAura::CloseNow() {
  desktop_root_window_host_->CloseNow();
}

void DesktopNativeWidgetAura::Show() {
  desktop_root_window_host_->AsRootWindowHost()->Show();
  window_->Show();
}

void DesktopNativeWidgetAura::Hide() {
  desktop_root_window_host_->AsRootWindowHost()->Hide();
  if (window_)
    window_->Hide();
}

void DesktopNativeWidgetAura::ShowMaximizedWithBounds(
      const gfx::Rect& restored_bounds) {
  desktop_root_window_host_->ShowMaximizedWithBounds(restored_bounds);
  window_->Show();
}

void DesktopNativeWidgetAura::ShowWithWindowState(ui::WindowShowState state) {
  desktop_root_window_host_->ShowWindowWithState(state);
  window_->Show();
}

bool DesktopNativeWidgetAura::IsVisible() const {
  return desktop_root_window_host_->IsVisible();
}

void DesktopNativeWidgetAura::Activate() {
  desktop_root_window_host_->Activate();
}

void DesktopNativeWidgetAura::Deactivate() {
  desktop_root_window_host_->Deactivate();
}

bool DesktopNativeWidgetAura::IsActive() const {
  return desktop_root_window_host_->IsActive();
}

void DesktopNativeWidgetAura::SetAlwaysOnTop(bool always_on_top) {
  desktop_root_window_host_->SetAlwaysOnTop(always_on_top);
}

void DesktopNativeWidgetAura::Maximize() {
  desktop_root_window_host_->Maximize();
}

void DesktopNativeWidgetAura::Minimize() {
  desktop_root_window_host_->Minimize();
}

bool DesktopNativeWidgetAura::IsMaximized() const {
  return desktop_root_window_host_->IsMaximized();
}

bool DesktopNativeWidgetAura::IsMinimized() const {
  return desktop_root_window_host_->IsMinimized();
}

void DesktopNativeWidgetAura::Restore() {
  desktop_root_window_host_->Restore();
}

void DesktopNativeWidgetAura::SetFullscreen(bool fullscreen) {
  desktop_root_window_host_->SetFullscreen(fullscreen);
}

bool DesktopNativeWidgetAura::IsFullscreen() const {
  return desktop_root_window_host_->IsFullscreen();
}

void DesktopNativeWidgetAura::SetOpacity(unsigned char opacity) {
  desktop_root_window_host_->SetOpacity(opacity);
}

void DesktopNativeWidgetAura::SetUseDragFrame(bool use_drag_frame) {
}

void DesktopNativeWidgetAura::FlashFrame(bool flash_frame) {
  desktop_root_window_host_->FlashFrame(flash_frame);
}

void DesktopNativeWidgetAura::RunShellDrag(
    View* view,
    const ui::OSExchangeData& data,
    const gfx::Point& location,
    int operation,
    ui::DragDropTypes::DragEventSource source) {
  views::RunShellDrag(window_, data, location, operation, source);
}

void DesktopNativeWidgetAura::SchedulePaintInRect(const gfx::Rect& rect) {
  if (window_)
    window_->SchedulePaintInRect(rect);
}

void DesktopNativeWidgetAura::SetCursor(gfx::NativeCursor cursor) {
  desktop_root_window_host_->AsRootWindowHost()->SetCursor(cursor);
}

void DesktopNativeWidgetAura::ClearNativeFocus() {
  desktop_root_window_host_->ClearNativeFocus();

  if (ShouldActivate()) {
    aura::client::GetFocusClient(window_)->
        ResetFocusWithinActiveWindow(window_);
  }
}

gfx::Rect DesktopNativeWidgetAura::GetWorkAreaBoundsInScreen() const {
  return desktop_root_window_host_->GetWorkAreaBoundsInScreen();
}

void DesktopNativeWidgetAura::SetInactiveRenderingDisabled(bool value) {
  if (!value) {
    active_window_observer_.reset();
  } else {
    active_window_observer_.reset(
        new NativeWidgetAuraWindowObserver(window_, native_widget_delegate_));
  }
  desktop_root_window_host_->SetInactiveRenderingDisabled(value);
}

Widget::MoveLoopResult DesktopNativeWidgetAura::RunMoveLoop(
    const gfx::Vector2d& drag_offset,
    Widget::MoveLoopSource source) {
  return desktop_root_window_host_->RunMoveLoop(drag_offset, source);
}

void DesktopNativeWidgetAura::EndMoveLoop() {
  desktop_root_window_host_->EndMoveLoop();
}

void DesktopNativeWidgetAura::SetVisibilityChangedAnimationsEnabled(
    bool value) {
  desktop_root_window_host_->SetVisibilityChangedAnimationsEnabled(value);
}

ui::NativeTheme* DesktopNativeWidgetAura::GetNativeTheme() const {
  return DesktopRootWindowHost::GetNativeTheme(window_);
}

////////////////////////////////////////////////////////////////////////////////
// DesktopNativeWidgetAura, aura::WindowDelegate implementation:

gfx::Size DesktopNativeWidgetAura::GetMinimumSize() const {
  return native_widget_delegate_->GetMinimumSize();
}

gfx::Size DesktopNativeWidgetAura::GetMaximumSize() const {
  return native_widget_delegate_->GetMaximumSize();
}

void DesktopNativeWidgetAura::OnBoundsChanged(const gfx::Rect& old_bounds,
                                              const gfx::Rect& new_bounds) {
  if (old_bounds.origin() != new_bounds.origin())
    native_widget_delegate_->OnNativeWidgetMove();
  if (old_bounds.size() != new_bounds.size())
    native_widget_delegate_->OnNativeWidgetSizeChanged(new_bounds.size());
}

gfx::NativeCursor DesktopNativeWidgetAura::GetCursor(const gfx::Point& point) {
  return gfx::kNullCursor;
}

int DesktopNativeWidgetAura::GetNonClientComponent(
    const gfx::Point& point) const {
  return native_widget_delegate_->GetNonClientComponent(point);
}

bool DesktopNativeWidgetAura::ShouldDescendIntoChildForEventHandling(
      aura::Window* child,
      const gfx::Point& location) {
  views::WidgetDelegate* widget_delegate = GetWidget()->widget_delegate();
  return !widget_delegate ||
      widget_delegate->ShouldDescendIntoChildForEventHandling(child, location);
}

bool DesktopNativeWidgetAura::CanFocus() {
  return true;
}

void DesktopNativeWidgetAura::OnCaptureLost() {
  native_widget_delegate_->OnMouseCaptureLost();
}

void DesktopNativeWidgetAura::OnPaint(gfx::Canvas* canvas) {
  native_widget_delegate_->OnNativeWidgetPaint(canvas);
}

void DesktopNativeWidgetAura::OnDeviceScaleFactorChanged(
    float device_scale_factor) {
}

void DesktopNativeWidgetAura::OnWindowDestroying() {
  // DesktopRootWindowHost owns the ActivationController which ShadowController
  // references. Make sure we destroy ShadowController early on.
  shadow_controller_.reset();
  // The DesktopRootWindowHost implementation sends OnNativeWidgetDestroying().
  tooltip_manager_.reset();
  if (tooltip_controller_.get()) {
    root_window_->RemovePreTargetHandler(tooltip_controller_.get());
    tooltip_controller_.reset();
    aura::client::SetTooltipClient(root_window_.get(), NULL);
  }
  if (window_modality_controller_) {
    root_window_->RemovePreTargetHandler(window_modality_controller_.get());
    window_modality_controller_.reset();
  }
}

void DesktopNativeWidgetAura::OnWindowDestroyed() {
  window_ = NULL;
  native_widget_delegate_->OnNativeWidgetDestroyed();
  if (ownership_ == Widget::InitParams::NATIVE_WIDGET_OWNS_WIDGET)
    delete this;
}

void DesktopNativeWidgetAura::OnWindowTargetVisibilityChanged(bool visible) {
}

bool DesktopNativeWidgetAura::HasHitTestMask() const {
  return native_widget_delegate_->HasHitTestMask();
}

void DesktopNativeWidgetAura::GetHitTestMask(gfx::Path* mask) const {
  native_widget_delegate_->GetHitTestMask(mask);
}

scoped_refptr<ui::Texture> DesktopNativeWidgetAura::CopyTexture() {
  // The layer we create doesn't have an external texture, so this should never
  // get invoked.
  NOTREACHED();
  return scoped_refptr<ui::Texture>();
}

////////////////////////////////////////////////////////////////////////////////
// DesktopNativeWidgetAura, ui::EventHandler implementation:

void DesktopNativeWidgetAura::OnKeyEvent(ui::KeyEvent* event) {
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

  native_widget_delegate_->OnKeyEvent(event);
  if (event->handled())
    return;

  if (GetWidget()->HasFocusManager() &&
      !GetWidget()->GetFocusManager()->OnKeyEvent(*event))
    event->SetHandled();
}

void DesktopNativeWidgetAura::OnMouseEvent(ui::MouseEvent* event) {
  DCHECK(window_->IsVisible());
  if (tooltip_manager_.get())
    tooltip_manager_->UpdateTooltip();
  native_widget_delegate_->OnMouseEvent(event);
  // WARNING: we may have been deleted.
}

void DesktopNativeWidgetAura::OnScrollEvent(ui::ScrollEvent* event) {
  if (event->type() == ui::ET_SCROLL) {
    native_widget_delegate_->OnScrollEvent(event);
    if (event->handled())
      return;

    // Convert unprocessed scroll events into wheel events.
    ui::MouseWheelEvent mwe(*static_cast<ui::ScrollEvent*>(event));
    native_widget_delegate_->OnMouseEvent(&mwe);
    if (mwe.handled())
      event->SetHandled();
  } else {
    native_widget_delegate_->OnScrollEvent(event);
  }
}

void DesktopNativeWidgetAura::OnTouchEvent(ui::TouchEvent* event) {
  native_widget_delegate_->OnTouchEvent(event);
}

void DesktopNativeWidgetAura::OnGestureEvent(ui::GestureEvent* event) {
  native_widget_delegate_->OnGestureEvent(event);
}

////////////////////////////////////////////////////////////////////////////////
// DesktopNativeWidgetAura, aura::client::ActivationDelegate implementation:

bool DesktopNativeWidgetAura::ShouldActivate() const {
  return can_activate_ && native_widget_delegate_->CanActivate();
}

////////////////////////////////////////////////////////////////////////////////
// DesktopNativeWidgetAura, aura::client::ActivationChangeObserver
//    implementation:

void DesktopNativeWidgetAura::OnWindowActivated(aura::Window* gained_active,
                                                aura::Window* lost_active) {
  DCHECK(window_ == gained_active || window_ == lost_active);
  if ((window_ == gained_active || window_ == lost_active) &&
      IsVisible() && GetWidget()->non_client_view()) {
    GetWidget()->non_client_view()->SchedulePaint();
  }
  if (gained_active == window_ && restore_focus_on_activate_) {
    restore_focus_on_activate_ = false;
    GetWidget()->GetFocusManager()->RestoreFocusedView();
  } else if (lost_active == window_ && GetWidget()->HasFocusManager()) {
    bool store_focused_view = corewm::UseFocusControllerOnDesktop();
    if (!store_focused_view) {
      // If we're losing focus to a window that is a top level (such as a
      // bubble) store the focus. Such a window shares the same
      // RootWindowHost, so that such a change won't trigger an activation
      // change (which calls StoreFocusedView()). Without this the focused
      // view is never told it lost focus.
      aura::Window* focused_window =
        aura::client::GetFocusClient(window_)->GetFocusedWindow();
      if (focused_window && focused_window != window_) {
        Widget* focused_widget =
            Widget::GetWidgetForNativeWindow(focused_window);
        store_focused_view = focused_widget && focused_widget != GetWidget() &&
            focused_widget->is_top_level();
      }
    }
    if (store_focused_view) {
      DCHECK(!restore_focus_on_activate_);
      restore_focus_on_activate_ = true;
      // Pass in false so that ClearNativeFocus() isn't invoked.
      GetWidget()->GetFocusManager()->StoreFocusedView(false);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// DesktopNativeWidgetAura, aura::client::FocusChangeObserver implementation:

void DesktopNativeWidgetAura::OnWindowFocused(aura::Window* gained_focus,
                                              aura::Window* lost_focus) {
  if (window_ == gained_focus) {
    desktop_root_window_host_->OnNativeWidgetFocus();
    native_widget_delegate_->OnNativeFocus(lost_focus);

    // If focus is moving from a descendant Window to |window_| then native
    // activation hasn't changed. We still need to inform the InputMethod we've
    // been focused though.
    InputMethod* input_method = GetWidget()->GetInputMethod();
    if (input_method)
      input_method->OnFocus();
  } else if (window_ == lost_focus) {
    desktop_root_window_host_->OnNativeWidgetBlur();
    native_widget_delegate_->OnNativeBlur(
        aura::client::GetFocusClient(window_)->GetFocusedWindow());
  }
}

////////////////////////////////////////////////////////////////////////////////
// DesktopNativeWidgetAura, views::internal::InputMethodDelegate:

void DesktopNativeWidgetAura::DispatchKeyEventPostIME(const ui::KeyEvent& key) {
  FocusManager* focus_manager =
      native_widget_delegate_->AsWidget()->GetFocusManager();
  native_widget_delegate_->OnKeyEvent(const_cast<ui::KeyEvent*>(&key));
  if (key.handled() || !focus_manager)
    return;
  focus_manager->OnKeyEvent(key);
}

////////////////////////////////////////////////////////////////////////////////
// DesktopNativeWidgetAura, aura::WindowDragDropDelegate implementation:

void DesktopNativeWidgetAura::OnDragEntered(const ui::DropTargetEvent& event) {
  DCHECK(drop_helper_.get() != NULL);
  last_drop_operation_ = drop_helper_->OnDragOver(event.data(),
      event.location(), event.source_operations());
}

int DesktopNativeWidgetAura::OnDragUpdated(const ui::DropTargetEvent& event) {
  DCHECK(drop_helper_.get() != NULL);
  last_drop_operation_ = drop_helper_->OnDragOver(event.data(),
      event.location(), event.source_operations());
  return last_drop_operation_;
}

void DesktopNativeWidgetAura::OnDragExited() {
  DCHECK(drop_helper_.get() != NULL);
  drop_helper_->OnDragExit();
}

int DesktopNativeWidgetAura::OnPerformDrop(const ui::DropTargetEvent& event) {
  DCHECK(drop_helper_.get() != NULL);
  return drop_helper_->OnDrop(event.data(), event.location(),
      last_drop_operation_);
}

}  // namespace views
