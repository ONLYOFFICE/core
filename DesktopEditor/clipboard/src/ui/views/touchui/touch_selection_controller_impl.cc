// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/touchui/touch_selection_controller_impl.h"

#include "base/time.h"
#include "grit/ui_strings.h"
#include "ui/base/ui_base_switches_util.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/path.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/screen.h"
#include "ui/gfx/size.h"
#include "ui/views/corewm/shadow_types.h"
#include "ui/views/widget/widget.h"

namespace {

// Constants defining the visual attributes of selection handles
const int kSelectionHandleRadius = 10;
const int kSelectionHandleAlpha = 0x7F;
const SkColor kSelectionHandleColor =
    SkColorSetA(SK_ColorBLACK, kSelectionHandleAlpha);

// The minimum selection size to trigger selection controller.
const int kMinSelectionSize = 4;

const int kContextMenuTimoutMs = 200;

// Convenience struct to represent a circle shape.
struct Circle {
  int radius;
  gfx::Point center;
  SkColor color;
};

// Creates a widget to host SelectionHandleView.
views::Widget* CreateTouchSelectionPopupWidget(
    gfx::NativeView context,
    views::WidgetDelegate* widget_delegate) {
  views::Widget* widget = new views::Widget;
  views::Widget::InitParams params(views::Widget::InitParams::TYPE_TOOLTIP);
  params.can_activate = false;
  params.transparent = true;
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.context = context;
  params.delegate = widget_delegate;
  widget->Init(params);
#if defined(USE_AURA)
  SetShadowType(widget->GetNativeView(), views::corewm::SHADOW_TYPE_NONE);
#endif
  return widget;
}

void PaintCircle(const Circle& circle, gfx::Canvas* canvas) {
  SkPaint paint;
  paint.setAntiAlias(true);
  paint.setStyle(SkPaint::kFill_Style);
  paint.setColor(circle.color);
  canvas->DrawCircle(circle.center, circle.radius, paint);
}

// The points may not match exactly, since the selection range computation may
// introduce some floating point errors. So check for a minimum size to decide
// whether or not there is any selection.
bool IsEmptySelection(const gfx::Point& p1, const gfx::Point& p2) {
  int delta_x = p2.x() - p1.x();
  int delta_y = p2.y() - p1.y();
  return (abs(delta_x) < kMinSelectionSize && abs(delta_y) < kMinSelectionSize);
}

gfx::Rect GetHandleBoundsFromCursor(const gfx::Rect& cursor) {
  return gfx::Rect(cursor.x() - kSelectionHandleRadius, cursor.y(),
          2 * kSelectionHandleRadius,
          2 * kSelectionHandleRadius + cursor.height());
}

}  // namespace

namespace views {

// A View that displays the text selection handle.
class TouchSelectionControllerImpl::EditingHandleView
    : public views::WidgetDelegateView {
 public:
  explicit EditingHandleView(TouchSelectionControllerImpl* controller,
                             gfx::NativeView context)
      : controller_(controller),
        cursor_height_(0) {
    widget_.reset(CreateTouchSelectionPopupWidget(context, this));
    widget_->SetContentsView(this);
    widget_->SetAlwaysOnTop(true);

    // We are owned by the TouchSelectionController.
    set_owned_by_client();
  }

  virtual ~EditingHandleView() {
  }

  int cursor_height() const { return cursor_height_; }

  // Overridden from views::WidgetDelegateView:
  virtual bool WidgetHasHitTestMask() const OVERRIDE {
    return true;
  }

  virtual void GetWidgetHitTestMask(gfx::Path* mask) const OVERRIDE {
    mask->addCircle(SkIntToScalar(kSelectionHandleRadius),
                    SkIntToScalar(kSelectionHandleRadius + cursor_height_),
                    SkIntToScalar(kSelectionHandleRadius));
  }

  virtual void DeleteDelegate() OVERRIDE {
    // We are owned and deleted by TouchSelectionController.
  }

  // Overridden from views::View:
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE {
    Circle circle = {kSelectionHandleRadius, gfx::Point(kSelectionHandleRadius,
                     kSelectionHandleRadius + cursor_height_),
                     kSelectionHandleColor};
    PaintCircle(circle, canvas);
    canvas->DrawLine(gfx::Point(kSelectionHandleRadius, 0),
        gfx::Point(kSelectionHandleRadius, cursor_height_),
        kSelectionHandleColor);
  }

  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE {
    event->SetHandled();
    switch (event->type()) {
      case ui::ET_GESTURE_SCROLL_BEGIN:
        controller_->SetDraggingHandle(this);
        break;
      case ui::ET_GESTURE_SCROLL_UPDATE:
        controller_->SelectionHandleDragged(event->location());
        break;
      case ui::ET_GESTURE_SCROLL_END:
        controller_->SetDraggingHandle(NULL);
        break;
      default:
        break;
    }
  }

  virtual void SetVisible(bool visible) OVERRIDE {
    // We simply show/hide the container widget.
    if (visible != widget_->IsVisible()) {
      if (visible)
        widget_->Show();
      else
        widget_->Hide();
    }
    View::SetVisible(visible);
  }

  virtual gfx::Size GetPreferredSize() OVERRIDE {
    return gfx::Size(2 * kSelectionHandleRadius,
                     2 * kSelectionHandleRadius + cursor_height_);
  }

  bool IsWidgetVisible() const {
    return widget_->IsVisible();
  }

  void SetSelectionRectInScreen(const gfx::Rect& rect) {
    cursor_height_ = rect.height();
    gfx::Rect widget_bounds = GetHandleBoundsFromCursor(rect);
    widget_->SetBounds(widget_bounds);
  }

  gfx::Point GetScreenPosition() {
    return widget_->GetClientAreaBoundsInScreen().origin();
  }

 private:
  scoped_ptr<Widget> widget_;
  TouchSelectionControllerImpl* controller_;
  int cursor_height_;

  DISALLOW_COPY_AND_ASSIGN(EditingHandleView);
};

TouchSelectionControllerImpl::TouchSelectionControllerImpl(
    ui::TouchEditable* client_view)
    : client_view_(client_view),
      client_widget_(NULL),
      selection_handle_1_(new EditingHandleView(this,
                          client_view->GetNativeView())),
      selection_handle_2_(new EditingHandleView(this,
                          client_view->GetNativeView())),
      cursor_handle_(new EditingHandleView(this,
                     client_view->GetNativeView())),
      context_menu_(NULL),
      dragging_handle_(NULL) {
  client_widget_ = Widget::GetTopLevelWidgetForNativeView(
      client_view_->GetNativeView());
  if (client_widget_)
    client_widget_->AddObserver(this);
}

TouchSelectionControllerImpl::~TouchSelectionControllerImpl() {
  HideContextMenu();
  if (client_widget_)
    client_widget_->RemoveObserver(this);
}

void TouchSelectionControllerImpl::SelectionChanged() {
  gfx::Rect r1, r2;
  client_view_->GetSelectionEndPoints(&r1, &r2);
  gfx::Point screen_pos_1(r1.origin());
  client_view_->ConvertPointToScreen(&screen_pos_1);
  gfx::Point screen_pos_2(r2.origin());
  client_view_->ConvertPointToScreen(&screen_pos_2);
  gfx::Rect screen_rect_1(screen_pos_1, r1.size());
  gfx::Rect screen_rect_2(screen_pos_2, r2.size());

  if (client_view_->DrawsHandles()) {
    UpdateContextMenu(r1.origin(), r2.origin());
    return;
  }
  if (dragging_handle_) {
    // We need to reposition only the selection handle that is being dragged.
    // The other handle stays the same. Also, the selection handle being dragged
    // will always be at the end of selection, while the other handle will be at
    // the start.
    dragging_handle_->SetSelectionRectInScreen(screen_rect_2);

    if (dragging_handle_ != cursor_handle_.get()) {
      // The non-dragging-handle might have recently become visible.
      EditingHandleView* non_dragging_handle =
          dragging_handle_ == selection_handle_1_.get()?
              selection_handle_2_.get() : selection_handle_1_.get();
      if (client_view_->GetBounds().Contains(r1.origin())) {
        non_dragging_handle->SetSelectionRectInScreen(screen_rect_1);
        non_dragging_handle->SetVisible(true);
      } else {
        non_dragging_handle->SetVisible(false);
      }
    }
  } else {
    UpdateContextMenu(r1.origin(), r2.origin());

    // Check if there is any selection at all.
    if (IsEmptySelection(screen_pos_2, screen_pos_1)) {
      selection_handle_1_->SetVisible(false);
      selection_handle_2_->SetVisible(false);
      cursor_handle_->SetSelectionRectInScreen(screen_rect_1);
      cursor_handle_->SetVisible(true);
      return;
    }

    cursor_handle_->SetVisible(false);
    if (client_view_->GetBounds().Contains(r1.origin())) {
      selection_handle_1_->SetSelectionRectInScreen(screen_rect_1);
      selection_handle_1_->SetVisible(true);
    } else {
      selection_handle_1_->SetVisible(false);
    }

    if (client_view_->GetBounds().Contains(r2.origin())) {
      selection_handle_2_->SetSelectionRectInScreen(screen_rect_2);
      selection_handle_2_->SetVisible(true);
    } else {
      selection_handle_2_->SetVisible(false);
    }
  }
}

bool TouchSelectionControllerImpl::IsHandleDragInProgress() {
  return !!dragging_handle_;
}

void TouchSelectionControllerImpl::SetDraggingHandle(
    EditingHandleView* handle) {
  dragging_handle_ = handle;
  if (dragging_handle_)
    HideContextMenu();
  else
    StartContextMenuTimer();
}

void TouchSelectionControllerImpl::SelectionHandleDragged(
    const gfx::Point& drag_pos) {
  // We do not want to show the context menu while dragging.
  HideContextMenu();

  DCHECK(dragging_handle_);

  gfx::Point offset_drag_pos(drag_pos.x(),
      drag_pos.y() - dragging_handle_->cursor_height() / 2 -
      2 * kSelectionHandleRadius);
  ConvertPointToClientView(dragging_handle_, &offset_drag_pos);
  if (dragging_handle_ == cursor_handle_.get()) {
    client_view_->MoveCaretTo(offset_drag_pos);
    return;
  }

  // Find the stationary selection handle.
  EditingHandleView* fixed_handle = selection_handle_1_.get();
  if (fixed_handle == dragging_handle_)
    fixed_handle = selection_handle_2_.get();

  // Find selection end points in client_view's coordinate system.
  gfx::Point p2(kSelectionHandleRadius, fixed_handle->cursor_height() / 2);
  ConvertPointToClientView(fixed_handle, &p2);

  // Instruct client_view to select the region between p1 and p2. The position
  // of |fixed_handle| is the start and that of |dragging_handle| is the end
  // of selection.
  client_view_->SelectRect(p2, offset_drag_pos);
}

void TouchSelectionControllerImpl::ConvertPointToClientView(
    EditingHandleView* source, gfx::Point* point) {
  View::ConvertPointToScreen(source, point);
  client_view_->ConvertPointFromScreen(point);
}

bool TouchSelectionControllerImpl::IsCommandIdEnabled(int command_id) const {
  return client_view_->IsCommandIdEnabled(command_id);
}

void TouchSelectionControllerImpl::ExecuteCommand(int command_id,
                                                  int event_flags) {
  HideContextMenu();
  client_view_->ExecuteCommand(command_id, event_flags);
}

void TouchSelectionControllerImpl::OpenContextMenu() {
  gfx::Point anchor = context_menu_->anchor_rect().origin();
  anchor.Offset(0, -kSelectionHandleRadius);
  HideContextMenu();
  client_view_->OpenContextMenu(anchor);
}

void TouchSelectionControllerImpl::OnMenuClosed(TouchEditingMenuView* menu) {
  if (menu == context_menu_)
    context_menu_ = NULL;
}

void TouchSelectionControllerImpl::OnWidgetClosing(Widget* widget) {
  DCHECK_EQ(client_widget_, widget);
  client_widget_ = NULL;
}

void TouchSelectionControllerImpl::OnWidgetBoundsChanged(
    Widget* widget,
    const gfx::Rect& new_bounds) {
  DCHECK_EQ(client_widget_, widget);
  HideContextMenu();
  SelectionChanged();
}

void TouchSelectionControllerImpl::ContextMenuTimerFired() {
  // Get selection end points in client_view's space.
  gfx::Rect r1, r2;
  client_view_->GetSelectionEndPoints(&r1, &r2);

  gfx::Rect handle_1_bounds = GetHandleBoundsFromCursor(r1);
  gfx::Rect handle_2_bounds = GetHandleBoundsFromCursor(r2);

  // if selection is completely inside the view, we display the context menu
  // in the middle of the end points on the top. Else, we show it above the
  // visible handle. If no handle is visible, we do not show the menu.
  gfx::Rect menu_anchor;
  gfx::Rect client_bounds = client_view_->GetBounds();
  if (client_bounds.Contains(r1.origin()) &&
      client_bounds.Contains(r2.origin())) {
    menu_anchor = gfx::UnionRects(handle_1_bounds, handle_2_bounds);
  } else if (client_bounds.Contains(r1.origin())) {
    menu_anchor = handle_1_bounds;
  } else if (client_bounds.Contains(r2.origin())) {
    menu_anchor = handle_2_bounds;
  } else {
    return;
  }

  gfx::Point menu_origin = menu_anchor.origin();
  client_view_->ConvertPointToScreen(&menu_origin);
  menu_anchor.set_origin(menu_origin);

  DCHECK(!context_menu_);
  context_menu_ = new TouchEditingMenuView(this, menu_anchor,
      client_view_->GetNativeView());
}

void TouchSelectionControllerImpl::StartContextMenuTimer() {
  if (context_menu_timer_.IsRunning())
    return;
  context_menu_timer_.Start(
      FROM_HERE,
      base::TimeDelta::FromMilliseconds(kContextMenuTimoutMs),
      this,
      &TouchSelectionControllerImpl::ContextMenuTimerFired);
}

void TouchSelectionControllerImpl::UpdateContextMenu(const gfx::Point& p1,
                                                     const gfx::Point& p2) {
  // Hide context menu to be shown when the timer fires.
  HideContextMenu();
  StartContextMenuTimer();
}

void TouchSelectionControllerImpl::HideContextMenu() {
  if (context_menu_)
    context_menu_->Close();
  context_menu_ = NULL;
  context_menu_timer_.Stop();
}

gfx::Point TouchSelectionControllerImpl::GetSelectionHandle1Position() {
  return selection_handle_1_->GetScreenPosition();
}

gfx::Point TouchSelectionControllerImpl::GetSelectionHandle2Position() {
  return selection_handle_2_->GetScreenPosition();
}

gfx::Point TouchSelectionControllerImpl::GetCursorHandlePosition() {
  return cursor_handle_->GetScreenPosition();
}

bool TouchSelectionControllerImpl::IsSelectionHandle1Visible() {
  return selection_handle_1_->visible();
}

bool TouchSelectionControllerImpl::IsSelectionHandle2Visible() {
  return selection_handle_2_->visible();
}

bool TouchSelectionControllerImpl::IsCursorHandleVisible() {
  return cursor_handle_->visible();
}

ViewsTouchSelectionControllerFactory::ViewsTouchSelectionControllerFactory() {
}

ui::TouchSelectionController* ViewsTouchSelectionControllerFactory::create(
    ui::TouchEditable* client_view) {
  if (switches::IsTouchEditingEnabled())
    return new views::TouchSelectionControllerImpl(client_view);
  return NULL;
}

}  // namespace views
