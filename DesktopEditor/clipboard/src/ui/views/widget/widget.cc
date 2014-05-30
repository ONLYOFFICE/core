// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/widget/widget.h"

#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "base/message_loop.h"
#include "base/utf_string_conversions.h"
#include "ui/base/events/event.h"
#include "ui/base/hit_test.h"
#include "ui/base/l10n/l10n_font_util.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/compositor/compositor.h"
#include "ui/compositor/layer.h"
#include "ui/gfx/screen.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/focus/focus_manager_factory.h"
#include "ui/views/focus/view_storage.h"
#include "ui/views/focus/widget_focus_manager.h"
#include "ui/views/ime/input_method.h"
#include "ui/views/views_delegate.h"
#include "ui/views/widget/default_theme_provider.h"
#include "ui/views/widget/native_widget_private.h"
#include "ui/views/widget/root_view.h"
#include "ui/views/widget/tooltip_manager.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/views/widget/widget_deletion_observer.h"
#include "ui/views/widget/widget_observer.h"
#include "ui/views/window/custom_frame_view.h"

#if !defined(OS_MACOSX)
#include "ui/views/controls/menu/menu_controller.h"
#endif

namespace views {

namespace {

// If |view| has a layer the layer is added to |layers|. Else this recurses
// through the children. This is used to build a list of the layers created by
// views that are direct children of the Widgets layer.
void BuildRootLayers(View* view, std::vector<ui::Layer*>* layers) {
  if (view->layer()) {
    layers->push_back(view->layer());
  } else {
    for (int i = 0; i < view->child_count(); ++i)
      BuildRootLayers(view->child_at(i), layers);
  }
}

// Create a native widget implementation.
// First, use the supplied one if non-NULL.
// Finally, make a default one.
NativeWidget* CreateNativeWidget(NativeWidget* native_widget,
                                 internal::NativeWidgetDelegate* delegate) {
  if (!native_widget) {
    native_widget =
        internal::NativeWidgetPrivate::CreateNativeWidget(delegate);
  }
  return native_widget;
}

}  // namespace

// This class is used to keep track of the event a Widget is processing, and
// restore any previously active event afterwards.
class ScopedEvent {
 public:
  ScopedEvent(Widget* widget, const ui::Event& event)
      : widget_(widget),
        event_(&event) {
    widget->event_stack_.push(this);
  }

  ~ScopedEvent() {
    if (widget_)
      widget_->event_stack_.pop();
  }

  void reset() {
    widget_ = NULL;
  }

  const ui::Event* event() {
    return event_;
  }

 private:
  Widget* widget_;
  const ui::Event* event_;

  DISALLOW_COPY_AND_ASSIGN(ScopedEvent);
};

// A default implementation of WidgetDelegate, used by Widget when no
// WidgetDelegate is supplied.
class DefaultWidgetDelegate : public WidgetDelegate {
 public:
  DefaultWidgetDelegate(Widget* widget, const Widget::InitParams& params)
      : widget_(widget),
        can_activate_(!params.child &&
                      params.type != Widget::InitParams::TYPE_POPUP) {
  }
  virtual ~DefaultWidgetDelegate() {}

  // Overridden from WidgetDelegate:
  virtual void DeleteDelegate() OVERRIDE {
    delete this;
  }
  virtual Widget* GetWidget() OVERRIDE {
    return widget_;
  }
  virtual const Widget* GetWidget() const OVERRIDE {
    return widget_;
  }

  virtual bool CanActivate() const OVERRIDE {
    return can_activate_;
  }

 private:
  Widget* widget_;
  bool can_activate_;

  DISALLOW_COPY_AND_ASSIGN(DefaultWidgetDelegate);
};

////////////////////////////////////////////////////////////////////////////////
// Widget, InitParams:

Widget::InitParams::InitParams()
    : type(TYPE_WINDOW),
      delegate(NULL),
      child(false),
      transient(false),
      transparent(ViewsDelegate::views_delegate &&
                  ViewsDelegate::views_delegate->UseTransparentWindows()),
      accept_events(true),
      can_activate(true),
      keep_on_top(false),
      ownership(NATIVE_WIDGET_OWNS_WIDGET),
      mirror_origin_in_rtl(false),
      has_dropshadow(false),
      remove_standard_frame(false),
      use_system_default_icon(false),
      show_state(ui::SHOW_STATE_DEFAULT),
      double_buffer(false),
      parent(NULL),
      native_widget(NULL),
      desktop_root_window_host(NULL),
      top_level(false),
      layer_type(ui::LAYER_TEXTURED),
      context(NULL) {
}

Widget::InitParams::InitParams(Type type)
    : type(type),
      delegate(NULL),
      child(type == TYPE_CONTROL),
      transient(type == TYPE_BUBBLE || type == TYPE_POPUP || type == TYPE_MENU),
      transparent(type == TYPE_WINDOW &&
                  ViewsDelegate::views_delegate &&
                  ViewsDelegate::views_delegate->UseTransparentWindows()),
      accept_events(true),
      can_activate(type != TYPE_POPUP && type != TYPE_MENU),
      keep_on_top(type == TYPE_MENU),
      ownership(NATIVE_WIDGET_OWNS_WIDGET),
      mirror_origin_in_rtl(false),
      has_dropshadow(false),
      remove_standard_frame(false),
      use_system_default_icon(false),
      show_state(ui::SHOW_STATE_DEFAULT),
      double_buffer(false),
      parent(NULL),
      native_widget(NULL),
      desktop_root_window_host(NULL),
      top_level(false),
      layer_type(ui::LAYER_TEXTURED),
      context(NULL) {
}

////////////////////////////////////////////////////////////////////////////////
// Widget, public:

Widget::Widget()
    : native_widget_(NULL),
      widget_delegate_(NULL),
      non_client_view_(NULL),
      dragged_view_(NULL),
      event_stack_(),
      ownership_(InitParams::NATIVE_WIDGET_OWNS_WIDGET),
      is_secondary_widget_(true),
      frame_type_(FRAME_TYPE_DEFAULT),
      disable_inactive_rendering_(false),
      widget_closed_(false),
      saved_show_state_(ui::SHOW_STATE_DEFAULT),
      focus_on_creation_(true),
      is_top_level_(false),
      native_widget_initialized_(false),
      native_widget_destroyed_(false),
      is_mouse_button_pressed_(false),
      is_touch_down_(false),
      last_mouse_event_was_move_(false),
      root_layers_dirty_(false),
      movement_disabled_(false) {
}

Widget::~Widget() {
  while (!event_stack_.empty()) {
    event_stack_.top()->reset();
    event_stack_.pop();
  }

  DestroyRootView();
  if (ownership_ == InitParams::WIDGET_OWNS_NATIVE_WIDGET) {
    delete native_widget_;
  } else {
    DCHECK(native_widget_destroyed_)
        << "Destroying a widget with a live native widget. "
        << "Widget probably should use WIDGET_OWNS_NATIVE_WIDGET ownership.";
  }
}

// static
Widget* Widget::CreateWindow(WidgetDelegate* delegate) {
  return CreateWindowWithBounds(delegate, gfx::Rect());
}

// static
Widget* Widget::CreateWindowWithBounds(WidgetDelegate* delegate,
                                       const gfx::Rect& bounds) {
  Widget* widget = new Widget;
  Widget::InitParams params;
  params.bounds = bounds;
  params.delegate = delegate;
  params.top_level = true;
  widget->Init(params);
  return widget;
}

// static
Widget* Widget::CreateWindowWithParent(WidgetDelegate* delegate,
                                       gfx::NativeWindow parent) {
  return CreateWindowWithParentAndBounds(delegate, parent, gfx::Rect());
}

// static
Widget* Widget::CreateWindowWithParentAndBounds(WidgetDelegate* delegate,
                                                gfx::NativeWindow parent,
                                                const gfx::Rect& bounds) {
  Widget* widget = new Widget;
  Widget::InitParams params;
  params.delegate = delegate;
  params.parent = parent;
  params.bounds = bounds;
  widget->Init(params);
  return widget;
}

// static
Widget* Widget::CreateWindowWithContext(WidgetDelegate* delegate,
                                        gfx::NativeView context) {
  return CreateWindowWithContextAndBounds(delegate, context, gfx::Rect());
}

// static
Widget* Widget::CreateWindowWithContextAndBounds(WidgetDelegate* delegate,
                                                 gfx::NativeView context,
                                                 const gfx::Rect& bounds) {
  Widget* widget = new Widget;
  Widget::InitParams params;
  params.delegate = delegate;
  params.context = context;
  params.bounds = bounds;
  widget->Init(params);
  return widget;
}

// static
Widget* Widget::GetWidgetForNativeView(gfx::NativeView native_view) {
  internal::NativeWidgetPrivate* native_widget =
      internal::NativeWidgetPrivate::GetNativeWidgetForNativeView(native_view);
  return native_widget ? native_widget->GetWidget() : NULL;
}

// static
Widget* Widget::GetWidgetForNativeWindow(gfx::NativeWindow native_window) {
  internal::NativeWidgetPrivate* native_widget =
      internal::NativeWidgetPrivate::GetNativeWidgetForNativeWindow(
          native_window);
  return native_widget ? native_widget->GetWidget() : NULL;
}

// static
Widget* Widget::GetTopLevelWidgetForNativeView(gfx::NativeView native_view) {
  internal::NativeWidgetPrivate* native_widget =
      internal::NativeWidgetPrivate::GetTopLevelNativeWidget(native_view);
  return native_widget ? native_widget->GetWidget() : NULL;
}


// static
void Widget::GetAllChildWidgets(gfx::NativeView native_view,
                                Widgets* children) {
  internal::NativeWidgetPrivate::GetAllChildWidgets(native_view, children);
}

// static
void Widget::ReparentNativeView(gfx::NativeView native_view,
                                gfx::NativeView new_parent) {
  internal::NativeWidgetPrivate::ReparentNativeView(native_view, new_parent);
}

// static
int Widget::GetLocalizedContentsWidth(int col_resource_id) {
  return ui::GetLocalizedContentsWidthForFont(col_resource_id,
      ResourceBundle::GetSharedInstance().GetFont(ResourceBundle::BaseFont));
}

// static
int Widget::GetLocalizedContentsHeight(int row_resource_id) {
  return ui::GetLocalizedContentsHeightForFont(row_resource_id,
      ResourceBundle::GetSharedInstance().GetFont(ResourceBundle::BaseFont));
}

// static
gfx::Size Widget::GetLocalizedContentsSize(int col_resource_id,
                                           int row_resource_id) {
  return gfx::Size(GetLocalizedContentsWidth(col_resource_id),
                   GetLocalizedContentsHeight(row_resource_id));
}

// static
bool Widget::RequiresNonClientView(InitParams::Type type) {
  return type == InitParams::TYPE_WINDOW ||
         type == InitParams::TYPE_PANEL ||
         type == InitParams::TYPE_BUBBLE;
}

void Widget::Init(const InitParams& in_params) {
  TRACE_EVENT0("views", "Widget::Init");
  InitParams params = in_params;

  is_top_level_ = params.top_level ||
      (!params.child &&
       params.type != InitParams::TYPE_CONTROL &&
       params.type != InitParams::TYPE_TOOLTIP);
  params.top_level = is_top_level_;
#if defined(OS_WIN) && defined(USE_AURA)
  // It'll need to be faded in if it's top level and not the main window.
  // Maintain transparent if the creator of the Widget specified transparent
  // already.
  params.transparent =
      params.transparent ||
      (is_top_level_ && params.type != InitParams::TYPE_WINDOW);
#endif

  if (ViewsDelegate::views_delegate)
    ViewsDelegate::views_delegate->OnBeforeWidgetInit(&params, this);

  widget_delegate_ = params.delegate ?
      params.delegate : new DefaultWidgetDelegate(this, params);
  ownership_ = params.ownership;
  native_widget_ = CreateNativeWidget(params.native_widget, this)->
                   AsNativeWidgetPrivate();
  root_view_.reset(CreateRootView());
  default_theme_provider_.reset(new DefaultThemeProvider);
  if (params.type == InitParams::TYPE_MENU) {
    is_mouse_button_pressed_ =
        internal::NativeWidgetPrivate::IsMouseButtonDown();
  }
  native_widget_->InitNativeWidget(params);
  if (RequiresNonClientView(params.type)) {
    non_client_view_ = new NonClientView;
    non_client_view_->SetFrameView(CreateNonClientFrameView());
    // Create the ClientView, add it to the NonClientView and add the
    // NonClientView to the RootView. This will cause everything to be parented.
    non_client_view_->set_client_view(widget_delegate_->CreateClientView(this));
    SetContentsView(non_client_view_);
    SetInitialBounds(params.bounds);
    if (params.show_state == ui::SHOW_STATE_MAXIMIZED)
      Maximize();
    else if (params.show_state == ui::SHOW_STATE_MINIMIZED)
      Minimize();
    UpdateWindowTitle();
  } else if (params.delegate) {
    SetContentsView(params.delegate->GetContentsView());
    SetInitialBoundsForFramelessWindow(params.bounds);
  }
  native_widget_initialized_ = true;
}

// Unconverted methods (see header) --------------------------------------------

gfx::NativeView Widget::GetNativeView() const {
  return native_widget_->GetNativeView();
}

gfx::NativeWindow Widget::GetNativeWindow() const {
  return native_widget_->GetNativeWindow();
}

void Widget::AddObserver(WidgetObserver* observer) {
  observers_.AddObserver(observer);
}

void Widget::RemoveObserver(WidgetObserver* observer) {
  observers_.RemoveObserver(observer);
}

bool Widget::HasObserver(WidgetObserver* observer) {
  return observers_.HasObserver(observer);
}

bool Widget::GetAccelerator(int cmd_id, ui::Accelerator* accelerator) {
  return false;
}

void Widget::ViewHierarchyChanged(bool is_add, View* parent, View* child) {
  if (!is_add) {
    if (child == dragged_view_)
      dragged_view_ = NULL;
    FocusManager* focus_manager = GetFocusManager();
    if (focus_manager)
      focus_manager->ViewRemoved(child);
    ViewStorage::GetInstance()->ViewRemoved(child);
    native_widget_->ViewRemoved(child);
  }
}

void Widget::NotifyNativeViewHierarchyChanged(bool attached,
                                              gfx::NativeView native_view) {
  if (!attached) {
    FocusManager* focus_manager = GetFocusManager();
    // We are being removed from a window hierarchy.  Treat this as
    // the root_view_ being removed.
    if (focus_manager)
      focus_manager->ViewRemoved(root_view_.get());
  }
  root_view_->NotifyNativeViewHierarchyChanged(attached, native_view);
}

// Converted methods (see header) ----------------------------------------------

Widget* Widget::GetTopLevelWidget() {
  return const_cast<Widget*>(
      static_cast<const Widget*>(this)->GetTopLevelWidget());
}

const Widget* Widget::GetTopLevelWidget() const {
  // GetTopLevelNativeWidget doesn't work during destruction because
  // property is gone after gobject gets deleted. Short circuit here
  // for toplevel so that InputMethod can remove itself from
  // focus manager.
  return is_top_level() ? this : native_widget_->GetTopLevelWidget();
}

void Widget::SetContentsView(View* view) {
  // Do not SetContentsView() again if it is already set to the same view.
  if (view == GetContentsView())
    return;
  root_view_->SetContentsView(view);
  if (non_client_view_ != view) {
    // |non_client_view_| can only be non-NULL here if RequiresNonClientView()
    // was true when the widget was initialized. Creating widgets with non
    // client views and then setting the contents view can cause subtle
    // problems on Windows, where the native widget thinks there is still a
    // |non_client_view_|. If you get this error, either use a different type
    // when initializing the widget, or don't call SetContentsView().
    DCHECK(!non_client_view_);
    non_client_view_ = NULL;
  }
}

View* Widget::GetContentsView() {
  return root_view_->GetContentsView();
}

gfx::Rect Widget::GetWindowBoundsInScreen() const {
  return native_widget_->GetWindowBoundsInScreen();
}

gfx::Rect Widget::GetClientAreaBoundsInScreen() const {
  return native_widget_->GetClientAreaBoundsInScreen();
}

gfx::Rect Widget::GetRestoredBounds() const {
  return native_widget_->GetRestoredBounds();
}

void Widget::SetBounds(const gfx::Rect& bounds) {
  native_widget_->SetBounds(bounds);
}

void Widget::SetSize(const gfx::Size& size) {
  native_widget_->SetSize(size);
}

void Widget::CenterWindow(const gfx::Size& size) {
  native_widget_->CenterWindow(size);
}

void Widget::SetBoundsConstrained(const gfx::Rect& bounds) {
  gfx::Rect work_area =
      gfx::Screen::GetScreenFor(GetNativeView())->GetDisplayNearestPoint(
          bounds.origin()).work_area();
  if (work_area.IsEmpty()) {
    SetBounds(bounds);
  } else {
    // Inset the work area slightly.
    work_area.Inset(10, 10, 10, 10);
    work_area.AdjustToFit(bounds);
    SetBounds(work_area);
  }
}

void Widget::SetVisibilityChangedAnimationsEnabled(bool value) {
  native_widget_->SetVisibilityChangedAnimationsEnabled(value);
}

Widget::MoveLoopResult Widget::RunMoveLoop(const gfx::Vector2d& drag_offset,
                                           MoveLoopSource source) {
  return native_widget_->RunMoveLoop(drag_offset, source);
}

void Widget::EndMoveLoop() {
  native_widget_->EndMoveLoop();
}

void Widget::StackAboveWidget(Widget* widget) {
  native_widget_->StackAbove(widget->GetNativeView());
}

void Widget::StackAbove(gfx::NativeView native_view) {
  native_widget_->StackAbove(native_view);
}

void Widget::StackAtTop() {
  native_widget_->StackAtTop();
}

void Widget::StackBelow(gfx::NativeView native_view) {
  native_widget_->StackBelow(native_view);
}

void Widget::SetShape(gfx::NativeRegion shape) {
  native_widget_->SetShape(shape);
}

void Widget::Close() {
  if (widget_closed_) {
    // It appears we can hit this code path if you close a modal dialog then
    // close the last browser before the destructor is hit, which triggers
    // invoking Close again.
    return;
  }

  bool can_close = true;
  if (non_client_view_)
    can_close = non_client_view_->CanClose();
  if (can_close) {
    SaveWindowPlacement();

    // During tear-down the top-level focus manager becomes unavailable to
    // GTK tabbed panes and their children, so normal deregistration via
    // |FormManager::ViewRemoved()| calls are fouled.  We clear focus here
    // to avoid these redundant steps and to avoid accessing deleted views
    // that may have been in focus.
    if (is_top_level() && focus_manager_.get())
      focus_manager_->SetFocusedView(NULL);

    FOR_EACH_OBSERVER(WidgetObserver, observers_, OnWidgetClosing(this));
    native_widget_->Close();
    widget_closed_ = true;
  }
}

void Widget::CloseNow() {
  FOR_EACH_OBSERVER(WidgetObserver, observers_, OnWidgetClosing(this));
  native_widget_->CloseNow();
}

void Widget::Show() {
  TRACE_EVENT0("views", "Widget::Show");
  if (non_client_view_) {
#if defined(OS_MACOSX)
    // On the Mac the FullScreenBookmarkBar test is different then for any other
    // OS. Since the new maximize logic from ash does not apply to the mac, we
    // continue to ignore the fullscreen mode here.
    if (saved_show_state_ == ui::SHOW_STATE_MAXIMIZED &&
        !initial_restored_bounds_.IsEmpty()) {
      native_widget_->ShowMaximizedWithBounds(initial_restored_bounds_);
    } else {
      native_widget_->ShowWithWindowState(saved_show_state_);
    }
#else
    // While initializing, the kiosk mode will go to full screen before the
    // widget gets shown. In that case we stay in full screen mode, regardless
    // of the |saved_show_state_| member.
    if (saved_show_state_ == ui::SHOW_STATE_MAXIMIZED &&
        !initial_restored_bounds_.IsEmpty() &&
        !IsFullscreen()) {
      native_widget_->ShowMaximizedWithBounds(initial_restored_bounds_);
    } else {
      native_widget_->ShowWithWindowState(
          IsFullscreen() ? ui::SHOW_STATE_FULLSCREEN : saved_show_state_);
    }
#endif
    // |saved_show_state_| only applies the first time the window is shown.
    // If we don't reset the value the window may be shown maximized every time
    // it is subsequently shown after being hidden.
    saved_show_state_ = ui::SHOW_STATE_NORMAL;
  } else {
    native_widget_->Show();
  }
}

void Widget::Hide() {
  native_widget_->Hide();
}

void Widget::ShowInactive() {
  // If this gets called with saved_show_state_ == ui::SHOW_STATE_MAXIMIZED,
  // call SetBounds()with the restored bounds to set the correct size. This
  // normally should not happen, but if it does we should avoid showing unsized
  // windows.
  if (saved_show_state_ == ui::SHOW_STATE_MAXIMIZED &&
      !initial_restored_bounds_.IsEmpty()) {
    SetBounds(initial_restored_bounds_);
    saved_show_state_ = ui::SHOW_STATE_NORMAL;
  }
  native_widget_->ShowWithWindowState(ui::SHOW_STATE_INACTIVE);
}

void Widget::Activate() {
  native_widget_->Activate();
}

void Widget::Deactivate() {
  native_widget_->Deactivate();
}

bool Widget::IsActive() const {
  return native_widget_->IsActive();
}

void Widget::DisableInactiveRendering() {
  SetInactiveRenderingDisabled(true);
}

void Widget::SetAlwaysOnTop(bool on_top) {
  native_widget_->SetAlwaysOnTop(on_top);
}

void Widget::Maximize() {
  native_widget_->Maximize();
}

void Widget::Minimize() {
  native_widget_->Minimize();
}

void Widget::Restore() {
  native_widget_->Restore();
}

bool Widget::IsMaximized() const {
  return native_widget_->IsMaximized();
}

bool Widget::IsMinimized() const {
  return native_widget_->IsMinimized();
}

void Widget::SetFullscreen(bool fullscreen) {
  native_widget_->SetFullscreen(fullscreen);
}

bool Widget::IsFullscreen() const {
  return native_widget_->IsFullscreen();
}

void Widget::SetOpacity(unsigned char opacity) {
  native_widget_->SetOpacity(opacity);
}

void Widget::SetUseDragFrame(bool use_drag_frame) {
  native_widget_->SetUseDragFrame(use_drag_frame);
}

void Widget::FlashFrame(bool flash) {
  native_widget_->FlashFrame(flash);
}

View* Widget::GetRootView() {
  return root_view_.get();
}

const View* Widget::GetRootView() const {
  return root_view_.get();
}

bool Widget::IsVisible() const {
  return native_widget_->IsVisible();
}

ui::ThemeProvider* Widget::GetThemeProvider() const {
  const Widget* root_widget = GetTopLevelWidget();
  if (root_widget && root_widget != this) {
    // Attempt to get the theme provider, and fall back to the default theme
    // provider if not found.
    ui::ThemeProvider* provider = root_widget->GetThemeProvider();
    if (provider)
      return provider;

    provider = root_widget->default_theme_provider_.get();
    if (provider)
      return provider;
  }
  return default_theme_provider_.get();
}

const ui::NativeTheme* Widget::GetNativeTheme() const {
  return native_widget_->GetNativeTheme();
}

FocusManager* Widget::GetFocusManager() {
  Widget* toplevel_widget = GetTopLevelWidget();
  return toplevel_widget ? toplevel_widget->focus_manager_.get() : NULL;
}

const FocusManager* Widget::GetFocusManager() const {
  const Widget* toplevel_widget = GetTopLevelWidget();
  return toplevel_widget ? toplevel_widget->focus_manager_.get() : NULL;
}

InputMethod* Widget::GetInputMethod() {
  if (is_top_level()) {
    if (!input_method_.get()) {
      input_method_.reset(native_widget_->CreateInputMethod());
      if (input_method_.get())
        input_method_->Init(this);
    }
    return input_method_.get();
  } else {
    Widget* toplevel = GetTopLevelWidget();
    // If GetTopLevelWidget() returns itself which is not toplevel,
    // the widget is detached from toplevel widget.
    // TODO(oshima): Fix GetTopLevelWidget() to return NULL
    // if there is no toplevel. We probably need to add GetTopMostWidget()
    // to replace some use cases.
    return (toplevel && toplevel != this) ? toplevel->GetInputMethod() : NULL;
  }
}

void Widget::RunShellDrag(View* view,
                          const ui::OSExchangeData& data,
                          const gfx::Point& location,
                          int operation,
                          ui::DragDropTypes::DragEventSource source) {
  dragged_view_ = view;
  native_widget_->RunShellDrag(view, data, location, operation, source);
  // If the view is removed during the drag operation, dragged_view_ is set to
  // NULL.
  if (view && dragged_view_ == view) {
    dragged_view_ = NULL;
    view->OnDragDone();
  }
}

void Widget::SchedulePaintInRect(const gfx::Rect& rect) {
  native_widget_->SchedulePaintInRect(rect);
}

void Widget::SetCursor(gfx::NativeCursor cursor) {
  native_widget_->SetCursor(cursor);
}

void Widget::SetNativeWindowProperty(const char* name, void* value) {
  native_widget_->SetNativeWindowProperty(name, value);
}

void* Widget::GetNativeWindowProperty(const char* name) const {
  return native_widget_->GetNativeWindowProperty(name);
}

void Widget::UpdateWindowTitle() {
  if (!non_client_view_)
    return;

  // Update the native frame's text. We do this regardless of whether or not
  // the native frame is being used, since this also updates the taskbar, etc.
  string16 window_title = widget_delegate_->GetWindowTitle();
  base::i18n::AdjustStringForLocaleDirection(&window_title);
  native_widget_->SetWindowTitle(window_title);
  non_client_view_->UpdateWindowTitle();

  // If the non-client view is rendering its own title, it'll need to relayout
  // now and to get a paint update later on.
  non_client_view_->Layout();
}

void Widget::UpdateWindowIcon() {
  if (non_client_view_)
    non_client_view_->UpdateWindowIcon();
  native_widget_->SetWindowIcons(widget_delegate_->GetWindowIcon(),
                                 widget_delegate_->GetWindowAppIcon());
}

FocusTraversable* Widget::GetFocusTraversable() {
  return static_cast<internal::RootView*>(root_view_.get());
}

void Widget::ThemeChanged() {
  root_view_->ThemeChanged();
}

void Widget::LocaleChanged() {
  root_view_->LocaleChanged();
}

void Widget::SetFocusTraversableParent(FocusTraversable* parent) {
  root_view_->SetFocusTraversableParent(parent);
}

void Widget::SetFocusTraversableParentView(View* parent_view) {
  root_view_->SetFocusTraversableParentView(parent_view);
}

void Widget::ClearNativeFocus() {
  native_widget_->ClearNativeFocus();
}

NonClientFrameView* Widget::CreateNonClientFrameView() {
  NonClientFrameView* frame_view =
      widget_delegate_->CreateNonClientFrameView(this);
  if (!frame_view)
    frame_view = native_widget_->CreateNonClientFrameView();
  if (!frame_view && ViewsDelegate::views_delegate) {
    frame_view =
        ViewsDelegate::views_delegate->CreateDefaultNonClientFrameView(this);
  }
  if (frame_view)
    return frame_view;

  CustomFrameView* custom_frame_view = new CustomFrameView;
  custom_frame_view->Init(this);
  return custom_frame_view;
}

bool Widget::ShouldUseNativeFrame() const {
  if (frame_type_ != FRAME_TYPE_DEFAULT)
    return frame_type_ == FRAME_TYPE_FORCE_NATIVE;
  return native_widget_->ShouldUseNativeFrame();
}

void Widget::DebugToggleFrameType() {
  if (frame_type_ == FRAME_TYPE_DEFAULT) {
    frame_type_ = ShouldUseNativeFrame() ? FRAME_TYPE_FORCE_CUSTOM :
        FRAME_TYPE_FORCE_NATIVE;
  } else {
    frame_type_ = frame_type_ == FRAME_TYPE_FORCE_CUSTOM ?
        FRAME_TYPE_FORCE_NATIVE : FRAME_TYPE_FORCE_CUSTOM;
  }
  FrameTypeChanged();
}

void Widget::FrameTypeChanged() {
  native_widget_->FrameTypeChanged();
}

const ui::Compositor* Widget::GetCompositor() const {
  return native_widget_->GetCompositor();
}

ui::Compositor* Widget::GetCompositor() {
  return native_widget_->GetCompositor();
}

gfx::Vector2d Widget::CalculateOffsetToAncestorWithLayer(
    ui::Layer** layer_parent) {
  return native_widget_->CalculateOffsetToAncestorWithLayer(layer_parent);
}

void Widget::ReorderLayers() {
  ui::Layer* layer = NULL;
  CalculateOffsetToAncestorWithLayer(&layer);
  if (layer)
    root_view_->ReorderChildLayers(layer);
}

void Widget::UpdateRootLayers() {
  // Calculate the layers requires traversing the tree, and since nearly any
  // mutation of the tree can trigger this call we delay until absolutely
  // necessary.
  root_layers_dirty_ = true;
}

const NativeWidget* Widget::native_widget() const {
  return native_widget_;
}

NativeWidget* Widget::native_widget() {
  return native_widget_;
}

void Widget::SetCapture(View* view) {
  if (internal::NativeWidgetPrivate::IsMouseButtonDown())
    is_mouse_button_pressed_ = true;
  if (internal::NativeWidgetPrivate::IsTouchDown())
    is_touch_down_ = true;
  root_view_->SetMouseHandler(view);
  if (!native_widget_->HasCapture())
    native_widget_->SetCapture();
}

void Widget::ReleaseCapture() {
  if (native_widget_->HasCapture())
    native_widget_->ReleaseCapture();
}

bool Widget::HasCapture() {
  return native_widget_->HasCapture();
}

const ui::Event* Widget::GetCurrentEvent() {
  return event_stack_.empty() ? NULL : event_stack_.top()->event();
}

void Widget::TooltipTextChanged(View* view) {
  TooltipManager* manager = native_widget_private()->GetTooltipManager();
  if (manager)
    manager->TooltipTextChanged(view);
}

bool Widget::SetInitialFocus() {
  View* v = widget_delegate_->GetInitiallyFocusedView();
  if (!focus_on_creation_) {
    // If not focusing the window now, tell the focus manager which view to
    // focus when the window is restored.
    if (v)
      focus_manager_->SetStoredFocusView(v);
    return true;
  }
  if (v)
    v->RequestFocus();
  return !!v;
}

View* Widget::GetChildViewParent() {
  return GetContentsView() ? GetContentsView() : GetRootView();
}

gfx::Rect Widget::GetWorkAreaBoundsInScreen() const {
  return native_widget_->GetWorkAreaBoundsInScreen();
}

void Widget::SynthesizeMouseMoveEvent() {
  last_mouse_event_was_move_ = false;
  ui::MouseEvent mouse_event(ui::ET_MOUSE_MOVED,
                             last_mouse_event_position_,
                             last_mouse_event_position_,
                             ui::EF_IS_SYNTHESIZED);
  root_view_->OnMouseMoved(mouse_event);
}

void Widget::OnOwnerClosing() {
}

////////////////////////////////////////////////////////////////////////////////
// Widget, NativeWidgetDelegate implementation:

bool Widget::IsModal() const {
  return widget_delegate_->GetModalType() != ui::MODAL_TYPE_NONE;
}

bool Widget::IsDialogBox() const {
  return !!widget_delegate_->AsDialogDelegate();
}

bool Widget::CanActivate() const {
  return widget_delegate_->CanActivate();
}

bool Widget::IsInactiveRenderingDisabled() const {
  return disable_inactive_rendering_;
}

void Widget::EnableInactiveRendering() {
  SetInactiveRenderingDisabled(false);
}

void Widget::OnNativeWidgetActivationChanged(bool active) {
  if (!active)
    SaveWindowPlacement();

  FOR_EACH_OBSERVER(WidgetObserver, observers_,
                    OnWidgetActivationChanged(this, active));
}

void Widget::OnNativeFocus(gfx::NativeView old_focused_view) {
  WidgetFocusManager::GetInstance()->OnWidgetFocusEvent(old_focused_view,
                                                        GetNativeView());
}

void Widget::OnNativeBlur(gfx::NativeView new_focused_view) {
  WidgetFocusManager::GetInstance()->OnWidgetFocusEvent(GetNativeView(),
                                                        new_focused_view);
}

void Widget::OnNativeWidgetVisibilityChanged(bool visible) {
  View* root = GetRootView();
  if (root)
    root->PropagateVisibilityNotifications(root, visible);
  FOR_EACH_OBSERVER(WidgetObserver, observers_,
                    OnWidgetVisibilityChanged(this, visible));
  if (GetCompositor() && root && root->layer())
    root->layer()->SetVisible(visible);
}

void Widget::OnNativeWidgetCreated() {
  if (is_top_level())
    focus_manager_.reset(FocusManagerFactory::Create(this));

  native_widget_->InitModalType(widget_delegate_->GetModalType());

  FOR_EACH_OBSERVER(WidgetObserver, observers_, OnWidgetCreated(this));
}

void Widget::OnNativeWidgetDestroying() {
  // Tell the focus manager (if any) that root_view is being removed
  // in case that the focused view is under this root view.
  if (GetFocusManager())
    GetFocusManager()->ViewRemoved(root_view_.get());
  FOR_EACH_OBSERVER(WidgetObserver, observers_, OnWidgetDestroying(this));
  if (non_client_view_)
    non_client_view_->WindowClosing();
  widget_delegate_->WindowClosing();
}

void Widget::OnNativeWidgetDestroyed() {
  FOR_EACH_OBSERVER(WidgetObserver, observers_, OnWidgetDestroyed(this));
  widget_delegate_->DeleteDelegate();
  widget_delegate_ = NULL;
  native_widget_destroyed_ = true;
}

gfx::Size Widget::GetMinimumSize() {
  return non_client_view_ ? non_client_view_->GetMinimumSize() : gfx::Size();
}

gfx::Size Widget::GetMaximumSize() {
  return non_client_view_ ? non_client_view_->GetMaximumSize() : gfx::Size();
}

void Widget::OnNativeWidgetMove() {
  widget_delegate_->OnWidgetMove();
  FOR_EACH_OBSERVER(WidgetObserver, observers_, OnWidgetBoundsChanged(
    this,
    GetWindowBoundsInScreen()));
}

void Widget::OnNativeWidgetSizeChanged(const gfx::Size& new_size) {
  root_view_->SetSize(new_size);

  // Size changed notifications can fire prior to full initialization
  // i.e. during session restore.  Avoid saving session state during these
  // startup procedures.
  if (native_widget_initialized_)
    SaveWindowPlacement();

  FOR_EACH_OBSERVER(WidgetObserver, observers_, OnWidgetBoundsChanged(
    this,
    GetWindowBoundsInScreen()));
}

void Widget::OnNativeWidgetBeginUserBoundsChange() {
  widget_delegate_->OnWindowBeginUserBoundsChange();
}

void Widget::OnNativeWidgetEndUserBoundsChange() {
  widget_delegate_->OnWindowEndUserBoundsChange();
}

bool Widget::HasFocusManager() const {
  return !!focus_manager_.get();
}

bool Widget::OnNativeWidgetPaintAccelerated(const gfx::Rect& dirty_region) {
  ui::Compositor* compositor = GetCompositor();
  if (!compositor)
    return false;

  compositor->ScheduleRedrawRect(dirty_region);
  return true;
}

void Widget::OnNativeWidgetPaint(gfx::Canvas* canvas) {
  // On Linux Aura, we can get here during Init() because of the
  // SetInitialBounds call.
  if (native_widget_initialized_)
  GetRootView()->Paint(canvas);
}

int Widget::GetNonClientComponent(const gfx::Point& point) {
  int component = non_client_view_ ?
      non_client_view_->NonClientHitTest(point) :
      HTNOWHERE;

  if (movement_disabled_ && (component == HTCAPTION || component == HTSYSMENU))
    return HTNOWHERE;

  return component;
}

void Widget::OnKeyEvent(ui::KeyEvent* event) {
  ScopedEvent scoped(this, *event);
  static_cast<internal::RootView*>(GetRootView())->
      DispatchKeyEvent(event);
}

void Widget::OnMouseEvent(ui::MouseEvent* event) {
  ScopedEvent scoped(this, *event);
  View* root_view = GetRootView();
  switch (event->type()) {
    case ui::ET_MOUSE_PRESSED: {
      last_mouse_event_was_move_ = false;

      // We may get deleted by the time we return from OnMousePressed. So we
      // use an observer to make sure we are still alive.
      WidgetDeletionObserver widget_deletion_observer(this);
      // Make sure we're still visible before we attempt capture as the mouse
      // press processing may have made the window hide (as happens with menus).
      if (root_view && root_view->OnMousePressed(*event) &&
          widget_deletion_observer.IsWidgetAlive() && IsVisible()) {
        is_mouse_button_pressed_ = true;
        if (!native_widget_->HasCapture())
          native_widget_->SetCapture();
        event->SetHandled();
      }
      return;
    }
    case ui::ET_MOUSE_RELEASED:
      last_mouse_event_was_move_ = false;
      is_mouse_button_pressed_ = false;
      // Release capture first, to avoid confusion if OnMouseReleased blocks.
      if (native_widget_->HasCapture() &&
          ShouldReleaseCaptureOnMouseReleased()) {
        native_widget_->ReleaseCapture();
      }
      if (root_view)
        root_view->OnMouseReleased(*event);
      if ((event->flags() & ui::EF_IS_NON_CLIENT) == 0)
        event->SetHandled();
      return;
    case ui::ET_MOUSE_MOVED:
    case ui::ET_MOUSE_DRAGGED:
      if (native_widget_->HasCapture() && is_mouse_button_pressed_) {
        last_mouse_event_was_move_ = false;
        if (root_view)
          root_view->OnMouseDragged(*event);
      } else if (!last_mouse_event_was_move_ ||
                 last_mouse_event_position_ != event->location()) {
        last_mouse_event_position_ = event->location();
        last_mouse_event_was_move_ = true;
        if (root_view)
          root_view->OnMouseMoved(*event);
      }
      return;
    case ui::ET_MOUSE_EXITED:
      last_mouse_event_was_move_ = false;
      if (root_view)
        root_view->OnMouseExited(*event);
      return;
    case ui::ET_MOUSEWHEEL:
      if (root_view && root_view->OnMouseWheel(
          static_cast<const ui::MouseWheelEvent&>(*event)))
        event->SetHandled();
      return;
    default:
      return;
  }
  event->SetHandled();
}

void Widget::OnMouseCaptureLost() {
  if (is_mouse_button_pressed_ || is_touch_down_) {
    View* root_view = GetRootView();
    if (root_view)
      root_view->OnMouseCaptureLost();
  }
  is_touch_down_ = false;
  is_mouse_button_pressed_ = false;
}

void Widget::OnTouchEvent(ui::TouchEvent* event) {
  ScopedEvent scoped(this, *event);
  static_cast<internal::RootView*>(GetRootView())->
      DispatchTouchEvent(event);
}

void Widget::OnScrollEvent(ui::ScrollEvent* event) {
  ScopedEvent scoped(this, *event);
  static_cast<internal::RootView*>(GetRootView())->
      DispatchScrollEvent(event);
}

void Widget::OnGestureEvent(ui::GestureEvent* event) {
  ScopedEvent scoped(this, *event);
  switch (event->type()) {
    case ui::ET_GESTURE_TAP_DOWN:
      is_touch_down_ = true;
      // We explicitly don't capture here. Not capturing enables multiple
      // widgets to get tap events at the same time. Views (such as tab
      // dragging) may explicitly capture.
      break;

    case ui::ET_GESTURE_END:
      if (event->details().touch_points() == 1) {
        is_touch_down_ = false;
        if (ShouldReleaseCaptureOnMouseReleased())
          ReleaseCapture();
      }
      break;

    default:
      break;
  }
  static_cast<internal::RootView*>(GetRootView())->DispatchGestureEvent(event);
}

bool Widget::ExecuteCommand(int command_id) {
  return widget_delegate_->ExecuteWindowsCommand(command_id);
}

InputMethod* Widget::GetInputMethodDirect() {
  return input_method_.get();
}

const std::vector<ui::Layer*>& Widget::GetRootLayers() {
  if (root_layers_dirty_) {
    root_layers_dirty_ = false;
    root_layers_.clear();
    BuildRootLayers(GetRootView(), &root_layers_);
  }
  return root_layers_;
}

bool Widget::HasHitTestMask() const {
  return widget_delegate_->WidgetHasHitTestMask();
}

void Widget::GetHitTestMask(gfx::Path* mask) const {
  DCHECK(mask);
  widget_delegate_->GetWidgetHitTestMask(mask);
}

Widget* Widget::AsWidget() {
  return this;
}

const Widget* Widget::AsWidget() const {
  return this;
}

////////////////////////////////////////////////////////////////////////////////
// Widget, FocusTraversable implementation:

FocusSearch* Widget::GetFocusSearch() {
  return root_view_->GetFocusSearch();
}

FocusTraversable* Widget::GetFocusTraversableParent() {
  // We are a proxy to the root view, so we should be bypassed when traversing
  // up and as a result this should not be called.
  NOTREACHED();
  return NULL;
}

View* Widget::GetFocusTraversableParentView() {
  // We are a proxy to the root view, so we should be bypassed when traversing
  // up and as a result this should not be called.
  NOTREACHED();
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////
// Widget, protected:

internal::RootView* Widget::CreateRootView() {
  return new internal::RootView(this);
}

void Widget::DestroyRootView() {
  non_client_view_ = NULL;
  root_view_.reset();
  // Input method has to be destroyed before focus manager.
  input_method_.reset();
}

////////////////////////////////////////////////////////////////////////////////
// Widget, private:

bool Widget::ShouldReleaseCaptureOnMouseReleased() const {
  return true;
}

void Widget::SetInactiveRenderingDisabled(bool value) {
  if (value == disable_inactive_rendering_)
    return;

  disable_inactive_rendering_ = value;
  if (non_client_view_)
    non_client_view_->SetInactiveRenderingDisabled(value);
  native_widget_->SetInactiveRenderingDisabled(value);
}

void Widget::SaveWindowPlacement() {
  // The window delegate does the actual saving for us. It seems like (judging
  // by go/crash) that in some circumstances we can end up here after
  // WM_DESTROY, at which point the window delegate is likely gone. So just
  // bail.
  if (!widget_delegate_)
    return;

  ui::WindowShowState show_state = ui::SHOW_STATE_NORMAL;
  gfx::Rect bounds;
  native_widget_->GetWindowPlacement(&bounds, &show_state);
  widget_delegate_->SaveWindowPlacement(bounds, show_state);
}

void Widget::SetInitialBounds(const gfx::Rect& bounds) {
  if (!non_client_view_)
    return;

  gfx::Rect saved_bounds;
  if (GetSavedWindowPlacement(&saved_bounds, &saved_show_state_)) {
    if (saved_show_state_ == ui::SHOW_STATE_MAXIMIZED) {
      // If we're going to maximize, wait until Show is invoked to set the
      // bounds. That way we avoid a noticeable resize.
      initial_restored_bounds_ = saved_bounds;
    } else if (!saved_bounds.IsEmpty()) {
      // If the saved bounds are valid, use them.
      SetBounds(saved_bounds);
    }
  } else {
    if (bounds.IsEmpty()) {
      // No initial bounds supplied, so size the window to its content and
      // center over its parent.
      native_widget_->CenterWindow(non_client_view_->GetPreferredSize());
    } else {
      // Use the supplied initial bounds.
      SetBoundsConstrained(bounds);
    }
  }
}

void Widget::SetInitialBoundsForFramelessWindow(const gfx::Rect& bounds) {
  if (bounds.IsEmpty()) {
    View* contents_view = GetContentsView();
    DCHECK(contents_view);
    // No initial bounds supplied, so size the window to its content and
    // center over its parent if preferred size is provided.
    gfx::Size size = contents_view->GetPreferredSize();
    if (!size.IsEmpty())
      native_widget_->CenterWindow(size);
  } else {
    // Use the supplied initial bounds.
    SetBoundsConstrained(bounds);
  }
}

bool Widget::GetSavedWindowPlacement(gfx::Rect* bounds,
                                     ui::WindowShowState* show_state) {
  // First we obtain the window's saved show-style and store it. We need to do
  // this here, rather than in Show() because by the time Show() is called,
  // the window's size will have been reset (below) and the saved maximized
  // state will have been lost. Sadly there's no way to tell on Windows when
  // a window is restored from maximized state, so we can't more accurately
  // track maximized state independently of sizing information.

  // Restore the window's placement from the controller.
  if (widget_delegate_->GetSavedWindowPlacement(bounds, show_state)) {
    if (!widget_delegate_->ShouldRestoreWindowSize()) {
      bounds->set_size(non_client_view_->GetPreferredSize());
    } else {
      gfx::Size minimum_size = GetMinimumSize();
      // Make sure the bounds are at least the minimum size.
      if (bounds->width() < minimum_size.width())
        bounds->set_width(minimum_size.width());

      if (bounds->height() < minimum_size.height())
        bounds->set_height(minimum_size.height());
    }
    return true;
  }
  return false;
}

void Widget::ReplaceInputMethod(InputMethod* input_method) {
  input_method_.reset(input_method);
  input_method->SetDelegate(native_widget_->GetInputMethodDelegate());
  input_method->Init(this);
}

namespace internal {

////////////////////////////////////////////////////////////////////////////////
// internal::NativeWidgetPrivate, NativeWidget implementation:

internal::NativeWidgetPrivate* NativeWidgetPrivate::AsNativeWidgetPrivate() {
  return this;
}

}  // namespace internal
}  // namespace views
