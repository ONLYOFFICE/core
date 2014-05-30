// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define _USE_MATH_DEFINES // For VC++ to get M_PI. This has to be first.

#include "ui/views/view.h"

#include <algorithm>
#include <cmath>

#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/stringprintf.h"
#include "base/utf_string_conversions.h"
#include "third_party/skia/include/core/SkRect.h"
#include "ui/base/accessibility/accessibility_types.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/ui_base_switches_util.h"
#include "ui/compositor/compositor.h"
#include "ui/compositor/layer.h"
#include "ui/compositor/layer_animator.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/interpolated_transform.h"
#include "ui/gfx/path.h"
#include "ui/gfx/point3_f.h"
#include "ui/gfx/point_conversions.h"
#include "ui/gfx/rect_conversions.h"
#include "ui/gfx/skia_util.h"
#include "ui/gfx/transform.h"
#include "ui/native_theme/native_theme.h"
#include "ui/views/accessibility/native_view_accessibility.h"
#include "ui/views/background.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/drag_controller.h"
#include "ui/views/layout/layout_manager.h"
#include "ui/views/views_delegate.h"
#include "ui/views/widget/native_widget_private.h"
#include "ui/views/widget/root_view.h"
#include "ui/views/widget/tooltip_manager.h"
#include "ui/views/widget/widget.h"

#if defined(OS_WIN)
#include "base/win/scoped_gdi_object.h"
#endif

namespace {

// Whether to use accelerated compositing when necessary (e.g. when a view has a
// transformation).
#if defined(USE_AURA)
bool use_acceleration_when_possible = true;
#else
bool use_acceleration_when_possible = false;
#endif

#if defined(OS_WIN)
const bool kContextMenuOnMousePress = false;
#else
const bool kContextMenuOnMousePress = true;
#endif

// Saves the drawing state, and restores the state when going out of scope.
class ScopedCanvas {
 public:
  explicit ScopedCanvas(gfx::Canvas* canvas) : canvas_(canvas) {
    if (canvas_)
      canvas_->Save();
  }
  ~ScopedCanvas() {
    if (canvas_)
      canvas_->Restore();
  }
  void SetCanvas(gfx::Canvas* canvas) {
    if (canvas_)
      canvas_->Restore();
    canvas_ = canvas;
    canvas_->Save();
  }

 private:
  gfx::Canvas* canvas_;

  DISALLOW_COPY_AND_ASSIGN(ScopedCanvas);
};

// Returns the top view in |view|'s hierarchy.
const views::View* GetHierarchyRoot(const views::View* view) {
  const views::View* root = view;
  while (root && root->parent())
    root = root->parent();
  return root;
}

}  // namespace

namespace views {

namespace internal {

// This event handler receives events in the post-target phase and takes care of
// the following:
//   - Generates context menu, or initiates drag-and-drop, from gesture events.
class PostEventDispatchHandler : public ui::EventHandler {
 public:
  explicit PostEventDispatchHandler(View* owner)
      : owner_(owner),
        touch_dnd_enabled_(switches::IsTouchDragDropEnabled()) {
  }
  virtual ~PostEventDispatchHandler() {}

 private:
  // Overridden from ui::EventHandler:
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE {
    DCHECK_EQ(ui::EP_POSTTARGET, event->phase());
    if (event->handled())
      return;

    if (touch_dnd_enabled_) {
      if (event->type() == ui::ET_GESTURE_LONG_PRESS &&
          (!owner_->drag_controller() ||
           owner_->drag_controller()->CanStartDragForView(
              owner_, event->location(), event->location()))) {
        if (owner_->DoDrag(*event, event->location(),
            ui::DragDropTypes::DRAG_EVENT_SOURCE_TOUCH)) {
          event->StopPropagation();
          return;
        }
      }
    }

    if (owner_->context_menu_controller() &&
        (event->type() == ui::ET_GESTURE_LONG_PRESS ||
         event->type() == ui::ET_GESTURE_LONG_TAP ||
         event->type() == ui::ET_GESTURE_TWO_FINGER_TAP)) {
      gfx::Point location(event->location());
      View::ConvertPointToScreen(owner_, &location);
      owner_->ShowContextMenu(location, true);
      event->StopPropagation();
    }
  }

  View* owner_;
  bool touch_dnd_enabled_;

  DISALLOW_COPY_AND_ASSIGN(PostEventDispatchHandler);
};

}  // namespace internal

// static
ViewsDelegate* ViewsDelegate::views_delegate = NULL;

// static
const char View::kViewClassName[] = "views/View";

////////////////////////////////////////////////////////////////////////////////
// View, public:

// Creation and lifetime -------------------------------------------------------

View::View()
    : owned_by_client_(false),
      id_(0),
      group_(-1),
      parent_(NULL),
      visible_(true),
      enabled_(true),
      notify_enter_exit_on_child_(false),
      registered_for_visible_bounds_notification_(false),
      clip_insets_(0, 0, 0, 0),
      needs_layout_(true),
      focus_border_(FocusBorder::CreateDashedFocusBorder()),
      flip_canvas_on_paint_for_rtl_ui_(false),
      paint_to_layer_(false),
      accelerator_registration_delayed_(false),
      accelerator_focus_manager_(NULL),
      registered_accelerator_count_(0),
      next_focusable_view_(NULL),
      previous_focusable_view_(NULL),
      focusable_(false),
      accessibility_focusable_(false),
      context_menu_controller_(NULL),
      drag_controller_(NULL),
      post_dispatch_handler_(new internal::PostEventDispatchHandler(this)),
      native_view_accessibility_(NULL) {
  AddPostTargetHandler(post_dispatch_handler_.get());
}

View::~View() {
  if (parent_)
    parent_->RemoveChildView(this);

  for (Views::const_iterator i(children_.begin()); i != children_.end(); ++i) {
    (*i)->parent_ = NULL;
    if (!(*i)->owned_by_client_)
      delete *i;
  }

  // Release ownership of the native accessibility object, but it's
  // reference-counted on some platforms, so it may not be deleted right away.
  if (native_view_accessibility_)
    native_view_accessibility_->Destroy();
}

// Tree operations -------------------------------------------------------------

const Widget* View::GetWidget() const {
  // The root view holds a reference to this view hierarchy's Widget.
  return parent_ ? parent_->GetWidget() : NULL;
}

Widget* View::GetWidget() {
  return const_cast<Widget*>(const_cast<const View*>(this)->GetWidget());
}

void View::AddChildView(View* view) {
  if (view->parent_ == this)
    return;
  AddChildViewAt(view, child_count());
}

void View::AddChildViewAt(View* view, int index) {
  CHECK_NE(view, this) << "You cannot add a view as its own child";
  DCHECK_GE(index, 0);
  DCHECK_LE(index, child_count());

  // If |view| has a parent, remove it from its parent.
  View* parent = view->parent_;
  const ui::NativeTheme* old_theme = view->GetNativeTheme();
  if (parent) {
    if (parent == this) {
      ReorderChildView(view, index);
      return;
    }
    parent->RemoveChildView(view);
  }

  // Sets the prev/next focus views.
  InitFocusSiblings(view, index);

  // Let's insert the view.
  view->parent_ = this;
  children_.insert(children_.begin() + index, view);

  for (View* v = this; v; v = v->parent_)
    v->ViewHierarchyChangedImpl(false, true, this, view);

  view->PropagateAddNotifications(this, view);
  UpdateTooltip();
  views::Widget* widget = GetWidget();
  if (widget) {
    RegisterChildrenForVisibleBoundsNotification(view);
    const ui::NativeTheme* new_theme = widget->GetNativeTheme();
    if (new_theme != old_theme)
      PropagateNativeThemeChanged(new_theme);
  }

  if (layout_manager_.get())
    layout_manager_->ViewAdded(this, view);

  if (use_acceleration_when_possible)
    ReorderLayers();

  // Make sure the visibility of the child layers are correct.
  // If any of the parent View is hidden, then the layers of the subtree
  // rooted at |this| should be hidden. Otherwise, all the child layers should
  // inherit the visibility of the owner View.
  UpdateLayerVisibility();
}

void View::ReorderChildView(View* view, int index) {
  DCHECK_EQ(view->parent_, this);
  if (index < 0)
    index = child_count() - 1;
  else if (index >= child_count())
    return;
  if (children_[index] == view)
    return;

  const Views::iterator i(std::find(children_.begin(), children_.end(), view));
  DCHECK(i != children_.end());
  children_.erase(i);

  // Unlink the view first
  View* next_focusable = view->next_focusable_view_;
  View* prev_focusable = view->previous_focusable_view_;
  if (prev_focusable)
    prev_focusable->next_focusable_view_ = next_focusable;
  if (next_focusable)
    next_focusable->previous_focusable_view_ = prev_focusable;

  // Add it in the specified index now.
  InitFocusSiblings(view, index);
  children_.insert(children_.begin() + index, view);

  if (use_acceleration_when_possible)
    ReorderLayers();
}

void View::RemoveChildView(View* view) {
  DoRemoveChildView(view, true, true, false);
}

void View::RemoveAllChildViews(bool delete_children) {
  while (!children_.empty())
    DoRemoveChildView(children_.front(), false, false, delete_children);
  UpdateTooltip();
}

bool View::Contains(const View* view) const {
  for (const View* v = view; v; v = v->parent_) {
    if (v == this)
      return true;
  }
  return false;
}

int View::GetIndexOf(const View* view) const {
  Views::const_iterator i(std::find(children_.begin(), children_.end(), view));
  return i != children_.end() ? static_cast<int>(i - children_.begin()) : -1;
}

// Size and disposition --------------------------------------------------------

void View::SetBounds(int x, int y, int width, int height) {
  SetBoundsRect(gfx::Rect(x, y, std::max(0, width), std::max(0, height)));
}

void View::SetBoundsRect(const gfx::Rect& bounds) {
  if (bounds == bounds_) {
    if (needs_layout_) {
      needs_layout_ = false;
      Layout();
      SchedulePaint();
    }
    return;
  }

  if (visible_) {
    // Paint where the view is currently.
    SchedulePaintBoundsChanged(
        bounds_.size() == bounds.size() ? SCHEDULE_PAINT_SIZE_SAME :
        SCHEDULE_PAINT_SIZE_CHANGED);
  }

  gfx::Rect prev = bounds_;
  bounds_ = bounds;
  BoundsChanged(prev);
}

void View::SetSize(const gfx::Size& size) {
  SetBounds(x(), y(), size.width(), size.height());
}

void View::SetPosition(const gfx::Point& position) {
  SetBounds(position.x(), position.y(), width(), height());
}

void View::SetX(int x) {
  SetBounds(x, y(), width(), height());
}

void View::SetY(int y) {
  SetBounds(x(), y, width(), height());
}

gfx::Rect View::GetContentsBounds() const {
  gfx::Rect contents_bounds(GetLocalBounds());
  if (border_.get())
    contents_bounds.Inset(border_->GetInsets());
  return contents_bounds;
}

gfx::Rect View::GetLocalBounds() const {
  return gfx::Rect(size());
}

gfx::Rect View::GetLayerBoundsInPixel() const {
  return layer()->GetTargetBounds();
}

gfx::Insets View::GetInsets() const {
  return border_.get() ? border_->GetInsets() : gfx::Insets();
}

gfx::Rect View::GetVisibleBounds() const {
  if (!IsDrawn())
    return gfx::Rect();
  gfx::Rect vis_bounds(GetLocalBounds());
  gfx::Rect ancestor_bounds;
  const View* view = this;
  gfx::Transform transform;

  while (view != NULL && !vis_bounds.IsEmpty()) {
    transform.ConcatTransform(view->GetTransform());
    gfx::Transform translation;
    translation.Translate(static_cast<float>(view->GetMirroredX()),
                          static_cast<float>(view->y()));
    transform.ConcatTransform(translation);

    vis_bounds = view->ConvertRectToParent(vis_bounds);
    const View* ancestor = view->parent_;
    if (ancestor != NULL) {
      ancestor_bounds.SetRect(0, 0, ancestor->width(), ancestor->height());
      vis_bounds.Intersect(ancestor_bounds);
    } else if (!view->GetWidget()) {
      // If the view has no Widget, we're not visible. Return an empty rect.
      return gfx::Rect();
    }
    view = ancestor;
  }
  if (vis_bounds.IsEmpty())
    return vis_bounds;
  // Convert back to this views coordinate system.
  gfx::RectF views_vis_bounds(vis_bounds);
  transform.TransformRectReverse(&views_vis_bounds);
  // Partially visible pixels should be considered visible.
  return gfx::ToEnclosingRect(views_vis_bounds);
}

gfx::Rect View::GetBoundsInScreen() const {
  gfx::Point origin;
  View::ConvertPointToScreen(this, &origin);
  return gfx::Rect(origin, size());
}

gfx::Size View::GetPreferredSize() {
  if (layout_manager_.get())
    return layout_manager_->GetPreferredSize(this);
  return gfx::Size();
}

int View::GetBaseline() const {
  return -1;
}

void View::SizeToPreferredSize() {
  gfx::Size prefsize = GetPreferredSize();
  if ((prefsize.width() != width()) || (prefsize.height() != height()))
    SetBounds(x(), y(), prefsize.width(), prefsize.height());
}

gfx::Size View::GetMinimumSize() {
  return GetPreferredSize();
}

gfx::Size View::GetMaximumSize() {
  return gfx::Size();
}

int View::GetHeightForWidth(int w) {
  if (layout_manager_.get())
    return layout_manager_->GetPreferredHeightForWidth(this, w);
  return GetPreferredSize().height();
}

void View::SetVisible(bool visible) {
  if (visible != visible_) {
    // If the View is currently visible, schedule paint to refresh parent.
    // TODO(beng): not sure we should be doing this if we have a layer.
    if (visible_)
      SchedulePaint();

    visible_ = visible;

    // Notify the parent.
    if (parent_)
      parent_->ChildVisibilityChanged(this);

    // This notifies all sub-views recursively.
    PropagateVisibilityNotifications(this, visible_);
    UpdateLayerVisibility();

    // If we are newly visible, schedule paint.
    if (visible_)
      SchedulePaint();
  }
}

bool View::IsDrawn() const {
  return visible_ && parent_ ? parent_->IsDrawn() : false;
}

void View::SetEnabled(bool enabled) {
  if (enabled != enabled_) {
    enabled_ = enabled;
    OnEnabledChanged();
  }
}

void View::OnEnabledChanged() {
  SchedulePaint();
}

// Transformations -------------------------------------------------------------

gfx::Transform View::GetTransform() const {
  return layer() ? layer()->transform() : gfx::Transform();
}

void View::SetTransform(const gfx::Transform& transform) {
  if (transform.IsIdentity()) {
    if (layer()) {
      layer()->SetTransform(transform);
      if (!paint_to_layer_)
        DestroyLayer();
    } else {
      // Nothing.
    }
  } else {
    if (!layer())
      CreateLayer();
    layer()->SetTransform(transform);
    layer()->ScheduleDraw();
  }
}

void View::SetPaintToLayer(bool paint_to_layer) {
  paint_to_layer_ = paint_to_layer;
  if (paint_to_layer_ && !layer()) {
    CreateLayer();
  } else if (!paint_to_layer_ && layer()) {
    DestroyLayer();
  }
}

ui::Layer* View::RecreateLayer() {
  ui::Layer* layer = AcquireLayer();
  if (!layer)
    return NULL;

  CreateLayer();
  layer_->set_scale_content(layer->scale_content());
  return layer;
}

// RTL positioning -------------------------------------------------------------

gfx::Rect View::GetMirroredBounds() const {
  gfx::Rect bounds(bounds_);
  bounds.set_x(GetMirroredX());
  return bounds;
}

gfx::Point View::GetMirroredPosition() const {
  return gfx::Point(GetMirroredX(), y());
}

int View::GetMirroredX() const {
  return parent_ ? parent_->GetMirroredXForRect(bounds_) : x();
}

int View::GetMirroredXForRect(const gfx::Rect& bounds) const {
  return base::i18n::IsRTL() ?
      (width() - bounds.x() - bounds.width()) : bounds.x();
}

int View::GetMirroredXInView(int x) const {
  return base::i18n::IsRTL() ? width() - x : x;
}

int View::GetMirroredXWithWidthInView(int x, int w) const {
  return base::i18n::IsRTL() ? width() - x - w : x;
}

// Layout ----------------------------------------------------------------------

void View::Layout() {
  needs_layout_ = false;

  // If we have a layout manager, let it handle the layout for us.
  if (layout_manager_.get())
    layout_manager_->Layout(this);

  // Make sure to propagate the Layout() call to any children that haven't
  // received it yet through the layout manager and need to be laid out. This
  // is needed for the case when the child requires a layout but its bounds
  // weren't changed by the layout manager. If there is no layout manager, we
  // just propagate the Layout() call down the hierarchy, so whoever receives
  // the call can take appropriate action.
  for (int i = 0, count = child_count(); i < count; ++i) {
    View* child = child_at(i);
    if (child->needs_layout_ || !layout_manager_.get()) {
      child->needs_layout_ = false;
      child->Layout();
    }
  }
}

void View::InvalidateLayout() {
  // Always invalidate up. This is needed to handle the case of us already being
  // valid, but not our parent.
  needs_layout_ = true;
  if (parent_)
    parent_->InvalidateLayout();
}

LayoutManager* View::GetLayoutManager() const {
  return layout_manager_.get();
}

void View::SetLayoutManager(LayoutManager* layout_manager) {
  if (layout_manager_.get())
    layout_manager_->Uninstalled(this);

  layout_manager_.reset(layout_manager);
  if (layout_manager_.get())
    layout_manager_->Installed(this);
}

// Attributes ------------------------------------------------------------------

std::string View::GetClassName() const {
  return kViewClassName;
}

View* View::GetAncestorWithClassName(const std::string& name) {
  for (View* view = this; view; view = view->parent_) {
    if (view->GetClassName() == name)
      return view;
  }
  return NULL;
}

const View* View::GetViewByID(int id) const {
  if (id == id_)
    return const_cast<View*>(this);

  for (int i = 0, count = child_count(); i < count; ++i) {
    const View* view = child_at(i)->GetViewByID(id);
    if (view)
      return view;
  }
  return NULL;
}

View* View::GetViewByID(int id) {
  return const_cast<View*>(const_cast<const View*>(this)->GetViewByID(id));
}

void View::SetGroup(int gid) {
  // Don't change the group id once it's set.
  DCHECK(group_ == -1 || group_ == gid);
  group_ = gid;
}

int View::GetGroup() const {
  return group_;
}

bool View::IsGroupFocusTraversable() const {
  return true;
}

void View::GetViewsInGroup(int group, Views* views) {
  if (group_ == group)
    views->push_back(this);

  for (int i = 0, count = child_count(); i < count; ++i)
    child_at(i)->GetViewsInGroup(group, views);
}

View* View::GetSelectedViewForGroup(int group) {
  Views views;
  GetWidget()->GetRootView()->GetViewsInGroup(group, &views);
  return views.empty() ? NULL : views[0];
}

// Coordinate conversion -------------------------------------------------------

// static
void View::ConvertPointToTarget(const View* source,
                                const View* target,
                                gfx::Point* point) {
  if (source == target)
    return;

  // |source| can be NULL.
  const View* root = GetHierarchyRoot(target);
  if (source) {
    CHECK_EQ(GetHierarchyRoot(source), root);

    if (source != root)
      source->ConvertPointForAncestor(root, point);
  }

  if (target != root)
    target->ConvertPointFromAncestor(root, point);

  // API defines NULL |source| as returning the point in screen coordinates.
  if (!source) {
    *point -=
        root->GetWidget()->GetClientAreaBoundsInScreen().OffsetFromOrigin();
  }
}

// static
void View::ConvertPointToWidget(const View* src, gfx::Point* p) {
  DCHECK(src);
  DCHECK(p);

  src->ConvertPointForAncestor(NULL, p);
}

// static
void View::ConvertPointFromWidget(const View* dest, gfx::Point* p) {
  DCHECK(dest);
  DCHECK(p);

  dest->ConvertPointFromAncestor(NULL, p);
}

// static
void View::ConvertPointToScreen(const View* src, gfx::Point* p) {
  DCHECK(src);
  DCHECK(p);

  // If the view is not connected to a tree, there's nothing we can do.
  const Widget* widget = src->GetWidget();
  if (widget) {
    ConvertPointToWidget(src, p);
    *p += widget->GetClientAreaBoundsInScreen().OffsetFromOrigin();
  }
}

// static
void View::ConvertPointFromScreen(const View* dst, gfx::Point* p) {
  DCHECK(dst);
  DCHECK(p);

  const views::Widget* widget = dst->GetWidget();
  if (!widget)
    return;
  *p -= widget->GetClientAreaBoundsInScreen().OffsetFromOrigin();
  views::View::ConvertPointFromWidget(dst, p);
}

gfx::Rect View::ConvertRectToParent(const gfx::Rect& rect) const {
  gfx::RectF x_rect = rect;
  GetTransform().TransformRect(&x_rect);
  x_rect.Offset(GetMirroredPosition().OffsetFromOrigin());
  // Pixels we partially occupy in the parent should be included.
  return gfx::ToEnclosingRect(x_rect);
}

gfx::Rect View::ConvertRectToWidget(const gfx::Rect& rect) const {
  gfx::Rect x_rect = rect;
  for (const View* v = this; v; v = v->parent_)
    x_rect = v->ConvertRectToParent(x_rect);
  return x_rect;
}

// Painting --------------------------------------------------------------------

void View::SchedulePaint() {
  SchedulePaintInRect(GetLocalBounds());
}

void View::SchedulePaintInRect(const gfx::Rect& rect) {
  if (!visible_)
    return;

  if (layer()) {
    layer()->SchedulePaint(rect);
  } else if (parent_) {
    // Translate the requested paint rect to the parent's coordinate system
    // then pass this notification up to the parent.
    parent_->SchedulePaintInRect(ConvertRectToParent(rect));
  }
}

void View::Paint(gfx::Canvas* canvas) {
  TRACE_EVENT1("views", "View::Paint", "class", GetClassName());

  ScopedCanvas scoped_canvas(canvas);

  // Paint this View and its children, setting the clip rect to the bounds
  // of this View and translating the origin to the local bounds' top left
  // point.
  //
  // Note that the X (or left) position we pass to ClipRectInt takes into
  // consideration whether or not the view uses a right-to-left layout so that
  // we paint our view in its mirrored position if need be.
  gfx::Rect clip_rect = bounds();
  clip_rect.Inset(clip_insets_);
  if (parent_)
    clip_rect.set_x(parent_->GetMirroredXForRect(clip_rect));
  if (!canvas->ClipRect(clip_rect))
    return;

  // Non-empty clip, translate the graphics such that 0,0 corresponds to
  // where this view is located (related to its parent).
  canvas->Translate(GetMirroredPosition().OffsetFromOrigin());
  canvas->Transform(GetTransform());

  PaintCommon(canvas);
}

ui::ThemeProvider* View::GetThemeProvider() const {
  const Widget* widget = GetWidget();
  return widget ? widget->GetThemeProvider() : NULL;
}

const ui::NativeTheme* View::GetNativeTheme() const {
  const Widget* widget = GetWidget();
  return widget ? widget->GetNativeTheme() : ui::NativeTheme::instance();
}

// Accelerated Painting --------------------------------------------------------

// static
void View::set_use_acceleration_when_possible(bool use) {
  use_acceleration_when_possible = use;
}

// static
bool View::get_use_acceleration_when_possible() {
  return use_acceleration_when_possible;
}

// Input -----------------------------------------------------------------------

View* View::GetEventHandlerForPoint(const gfx::Point& point) {
  // Walk the child Views recursively looking for the View that most
  // tightly encloses the specified point.
  for (int i = child_count() - 1; i >= 0; --i) {
    View* child = child_at(i);
    if (!child->visible())
      continue;

    gfx::Point point_in_child_coords(point);
    ConvertPointToTarget(this, child, &point_in_child_coords);
    if (child->HitTestPoint(point_in_child_coords))
      return child->GetEventHandlerForPoint(point_in_child_coords);
  }
  return this;
}

View* View::GetTooltipHandlerForPoint(const gfx::Point& point) {
  if (!HitTestPoint(point))
    return NULL;

  // Walk the child Views recursively looking for the View that most
  // tightly encloses the specified point.
  for (int i = child_count() - 1; i >= 0; --i) {
    View* child = child_at(i);
    if (!child->visible())
      continue;

    gfx::Point point_in_child_coords(point);
    ConvertPointToTarget(this, child, &point_in_child_coords);
    View* handler = child->GetTooltipHandlerForPoint(point_in_child_coords);
    if (handler)
      return handler;
  }
  return this;
}

gfx::NativeCursor View::GetCursor(const ui::MouseEvent& event) {
#if defined(OS_WIN)
#if defined(USE_AURA)
  static ui::Cursor arrow;
  if (!arrow.platform())
    arrow.SetPlatformCursor(LoadCursor(NULL, IDC_ARROW));
  return arrow;
#else
  static HCURSOR arrow = LoadCursor(NULL, IDC_ARROW);
  return arrow;
#endif
#else
  return gfx::kNullCursor;
#endif
}

bool View::HitTestPoint(const gfx::Point& point) const {
  return HitTestRect(gfx::Rect(point, gfx::Size(1, 1)));
}

bool View::HitTestRect(const gfx::Rect& rect) const {
  if (GetLocalBounds().Intersects(rect)) {
    if (HasHitTestMask()) {
      gfx::Path mask;
      GetHitTestMask(&mask);
#if defined(USE_AURA)
      // TODO: should we use this every where?
      SkRegion clip_region;
      clip_region.setRect(0, 0, width(), height());
      SkRegion mask_region;
      return mask_region.setPath(mask, clip_region) &&
             mask_region.intersects(RectToSkIRect(rect));
#elif defined(OS_WIN)
      base::win::ScopedRegion rgn(mask.CreateNativeRegion());
      const RECT r(rect.ToRECT());
      return RectInRegion(rgn, &r) != 0;
#endif
    }
    // No mask, but inside our bounds.
    return true;
  }
  // Outside our bounds.
  return false;
}

bool View::OnMousePressed(const ui::MouseEvent& event) {
  return false;
}

bool View::OnMouseDragged(const ui::MouseEvent& event) {
  return false;
}

void View::OnMouseReleased(const ui::MouseEvent& event) {
}

void View::OnMouseCaptureLost() {
}

void View::OnMouseMoved(const ui::MouseEvent& event) {
}

void View::OnMouseEntered(const ui::MouseEvent& event) {
}

void View::OnMouseExited(const ui::MouseEvent& event) {
}

void View::SetMouseHandler(View* new_mouse_handler) {
  // |new_mouse_handler| may be NULL.
  if (parent_)
    parent_->SetMouseHandler(new_mouse_handler);
}

bool View::OnKeyPressed(const ui::KeyEvent& event) {
  return false;
}

bool View::OnKeyReleased(const ui::KeyEvent& event) {
  return false;
}

bool View::OnMouseWheel(const ui::MouseWheelEvent& event) {
  return false;
}

void View::OnKeyEvent(ui::KeyEvent* event) {
  bool consumed = (event->type() == ui::ET_KEY_PRESSED) ? OnKeyPressed(*event) :
                                                          OnKeyReleased(*event);
  if (consumed)
    event->StopPropagation();
}

void View::OnMouseEvent(ui::MouseEvent* event) {
  switch (event->type()) {
    case ui::ET_MOUSE_PRESSED:
      if (ProcessMousePressed(*event))
        event->SetHandled();
      return;

    case ui::ET_MOUSE_MOVED:
      if ((event->flags() & (ui::EF_LEFT_MOUSE_BUTTON |
                             ui::EF_RIGHT_MOUSE_BUTTON |
                             ui::EF_MIDDLE_MOUSE_BUTTON)) == 0) {
        OnMouseMoved(*event);
        return;
      }
      // FALL-THROUGH
    case ui::ET_MOUSE_DRAGGED:
      if (ProcessMouseDragged(*event))
        event->SetHandled();
      return;

    case ui::ET_MOUSE_RELEASED:
      ProcessMouseReleased(*event);
      return;

    case ui::ET_MOUSEWHEEL:
      if (OnMouseWheel(*static_cast<ui::MouseWheelEvent*>(event)))
        event->SetHandled();
      break;

    case ui::ET_MOUSE_ENTERED:
      OnMouseEntered(*event);
      break;

    case ui::ET_MOUSE_EXITED:
      OnMouseExited(*event);
      break;

    default:
      return;
  }
}

void View::OnScrollEvent(ui::ScrollEvent* event) {
}

void View::OnTouchEvent(ui::TouchEvent* event) {
}

void View::OnGestureEvent(ui::GestureEvent* event) {
}

ui::TextInputClient* View::GetTextInputClient() {
  return NULL;
}

InputMethod* View::GetInputMethod() {
  Widget* widget = GetWidget();
  return widget ? widget->GetInputMethod() : NULL;
}

bool View::CanAcceptEvent(const ui::Event& event) {
  return event.dispatch_to_hidden_targets() || IsDrawn();
}

ui::EventTarget* View::GetParentTarget() {
  return parent_;
}

// Accelerators ----------------------------------------------------------------

void View::AddAccelerator(const ui::Accelerator& accelerator) {
  if (!accelerators_.get())
    accelerators_.reset(new std::vector<ui::Accelerator>());

  if (std::find(accelerators_->begin(), accelerators_->end(), accelerator) ==
      accelerators_->end()) {
    accelerators_->push_back(accelerator);
  }
  RegisterPendingAccelerators();
}

void View::RemoveAccelerator(const ui::Accelerator& accelerator) {
  if (!accelerators_.get()) {
    NOTREACHED() << "Removing non-existing accelerator";
    return;
  }

  std::vector<ui::Accelerator>::iterator i(
      std::find(accelerators_->begin(), accelerators_->end(), accelerator));
  if (i == accelerators_->end()) {
    NOTREACHED() << "Removing non-existing accelerator";
    return;
  }

  size_t index = i - accelerators_->begin();
  accelerators_->erase(i);
  if (index >= registered_accelerator_count_) {
    // The accelerator is not registered to FocusManager.
    return;
  }
  --registered_accelerator_count_;

  // Providing we are attached to a Widget and registered with a focus manager,
  // we should de-register from that focus manager now.
  if (GetWidget() && accelerator_focus_manager_)
    accelerator_focus_manager_->UnregisterAccelerator(accelerator, this);
}

void View::ResetAccelerators() {
  if (accelerators_.get())
    UnregisterAccelerators(false);
}

bool View::AcceleratorPressed(const ui::Accelerator& accelerator) {
  return false;
}

bool View::CanHandleAccelerators() const {
  return enabled() && IsDrawn() && GetWidget() && GetWidget()->IsVisible();
}

// Focus -----------------------------------------------------------------------

bool View::HasFocus() const {
  const FocusManager* focus_manager = GetFocusManager();
  return focus_manager && (focus_manager->GetFocusedView() == this);
}

View* View::GetNextFocusableView() {
  return next_focusable_view_;
}

const View* View::GetNextFocusableView() const {
  return next_focusable_view_;
}

View* View::GetPreviousFocusableView() {
  return previous_focusable_view_;
}

void View::SetNextFocusableView(View* view) {
  if (view)
    view->previous_focusable_view_ = this;
  next_focusable_view_ = view;
}

bool View::IsFocusable() const {
  return focusable_ && enabled_ && IsDrawn();
}

bool View::IsAccessibilityFocusable() const {
  return (focusable_ || accessibility_focusable_) && enabled_ && IsDrawn();
}

FocusManager* View::GetFocusManager() {
  Widget* widget = GetWidget();
  return widget ? widget->GetFocusManager() : NULL;
}

const FocusManager* View::GetFocusManager() const {
  const Widget* widget = GetWidget();
  return widget ? widget->GetFocusManager() : NULL;
}

void View::RequestFocus() {
  FocusManager* focus_manager = GetFocusManager();
  if (focus_manager && IsFocusable())
    focus_manager->SetFocusedView(this);
}

bool View::SkipDefaultKeyEventProcessing(const ui::KeyEvent& event) {
  return false;
}

FocusTraversable* View::GetFocusTraversable() {
  return NULL;
}

FocusTraversable* View::GetPaneFocusTraversable() {
  return NULL;
}

// Tooltips --------------------------------------------------------------------

bool View::GetTooltipText(const gfx::Point& p, string16* tooltip) const {
  return false;
}

bool View::GetTooltipTextOrigin(const gfx::Point& p, gfx::Point* loc) const {
  return false;
}

// Context menus ---------------------------------------------------------------

void View::ShowContextMenu(const gfx::Point& p, bool is_mouse_gesture) {
  if (!context_menu_controller_)
    return;

  context_menu_controller_->ShowContextMenuForView(this, p);
}

// static
bool View::ShouldShowContextMenuOnMousePress() {
  return kContextMenuOnMousePress;
}

// Drag and drop ---------------------------------------------------------------

bool View::GetDropFormats(
      int* formats,
      std::set<OSExchangeData::CustomFormat>* custom_formats) {
  return false;
}

bool View::AreDropTypesRequired() {
  return false;
}

bool View::CanDrop(const OSExchangeData& data) {
  // TODO(sky): when I finish up migration, this should default to true.
  return false;
}

void View::OnDragEntered(const ui::DropTargetEvent& event) {
}

int View::OnDragUpdated(const ui::DropTargetEvent& event) {
  return ui::DragDropTypes::DRAG_NONE;
}

void View::OnDragExited() {
}

int View::OnPerformDrop(const ui::DropTargetEvent& event) {
  return ui::DragDropTypes::DRAG_NONE;
}

void View::OnDragDone() {
}

// static
bool View::ExceededDragThreshold(const gfx::Vector2d& delta) {
  return (abs(delta.x()) > GetHorizontalDragThreshold() ||
          abs(delta.y()) > GetVerticalDragThreshold());
}

// Accessibility----------------------------------------------------------------

gfx::NativeViewAccessible View::GetNativeViewAccessible() {
  if (!native_view_accessibility_)
    native_view_accessibility_ = NativeViewAccessibility::Create(this);
  if (native_view_accessibility_)
    return native_view_accessibility_->GetNativeObject();
  return NULL;
}

void View::NotifyAccessibilityEvent(
    ui::AccessibilityTypes::Event event_type,
    bool send_native_event) {
  if (ViewsDelegate::views_delegate)
    ViewsDelegate::views_delegate->NotifyAccessibilityEvent(this, event_type);

  if (send_native_event) {
    if (!native_view_accessibility_)
      native_view_accessibility_ = NativeViewAccessibility::Create(this);
    if (native_view_accessibility_)
      native_view_accessibility_->NotifyAccessibilityEvent(event_type);
  }
}

// Scrolling -------------------------------------------------------------------

void View::ScrollRectToVisible(const gfx::Rect& rect) {
  // We must take RTL UI mirroring into account when adjusting the position of
  // the region.
  if (parent_) {
    gfx::Rect scroll_rect(rect);
    scroll_rect.Offset(GetMirroredX(), y());
    parent_->ScrollRectToVisible(scroll_rect);
  }
}

int View::GetPageScrollIncrement(ScrollView* scroll_view,
                                 bool is_horizontal, bool is_positive) {
  return 0;
}

int View::GetLineScrollIncrement(ScrollView* scroll_view,
                                 bool is_horizontal, bool is_positive) {
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// View, protected:

// Size and disposition --------------------------------------------------------

void View::OnBoundsChanged(const gfx::Rect& previous_bounds) {
}

void View::PreferredSizeChanged() {
  InvalidateLayout();
  if (parent_)
    parent_->ChildPreferredSizeChanged(this);
}

bool View::NeedsNotificationWhenVisibleBoundsChange() const {
  return false;
}

void View::OnVisibleBoundsChanged() {
}

// Tree operations -------------------------------------------------------------

void View::ViewHierarchyChanged(bool is_add, View* parent, View* child) {
}

void View::VisibilityChanged(View* starting_from, bool is_visible) {
}

void View::NativeViewHierarchyChanged(bool attached,
                                      gfx::NativeView native_view,
                                      internal::RootView* root_view) {
  FocusManager* focus_manager = GetFocusManager();
  if (!accelerator_registration_delayed_ &&
      accelerator_focus_manager_ &&
      accelerator_focus_manager_ != focus_manager) {
    UnregisterAccelerators(true);
    accelerator_registration_delayed_ = true;
  }
  if (accelerator_registration_delayed_ && attached) {
    if (focus_manager) {
      RegisterPendingAccelerators();
      accelerator_registration_delayed_ = false;
    }
  }
}

// Painting --------------------------------------------------------------------

void View::PaintChildren(gfx::Canvas* canvas) {
  TRACE_EVENT1("views", "View::PaintChildren", "class", GetClassName());
  for (int i = 0, count = child_count(); i < count; ++i)
    if (!child_at(i)->layer())
      child_at(i)->Paint(canvas);
}

void View::OnPaint(gfx::Canvas* canvas) {
  TRACE_EVENT1("views", "View::OnPaint", "class", GetClassName());
  OnPaintBackground(canvas);
  OnPaintFocusBorder(canvas);
  OnPaintBorder(canvas);
}

void View::OnPaintBackground(gfx::Canvas* canvas) {
  if (background_.get()) {
    TRACE_EVENT2("views", "View::OnPaintBackground",
                 "width", canvas->sk_canvas()->getDevice()->width(),
                 "height", canvas->sk_canvas()->getDevice()->height());
    background_->Paint(canvas, this);
  }
}

void View::OnPaintBorder(gfx::Canvas* canvas) {
  if (border_.get()) {
    TRACE_EVENT2("views", "View::OnPaintBorder",
                 "width", canvas->sk_canvas()->getDevice()->width(),
                 "height", canvas->sk_canvas()->getDevice()->height());
    border_->Paint(*this, canvas);
  }
}

void View::OnPaintFocusBorder(gfx::Canvas* canvas) {
  if (focus_border_.get() &&
      HasFocus() && (focusable() || IsAccessibilityFocusable())) {
    TRACE_EVENT2("views", "views::OnPaintFocusBorder",
                 "width", canvas->sk_canvas()->getDevice()->width(),
                 "height", canvas->sk_canvas()->getDevice()->height());
    focus_border_->Paint(*this, canvas);
  }
}

// Accelerated Painting --------------------------------------------------------

void View::SetFillsBoundsOpaquely(bool fills_bounds_opaquely) {
  // This method should not have the side-effect of creating the layer.
  if (layer())
    layer()->SetFillsBoundsOpaquely(fills_bounds_opaquely);
}

bool View::SetExternalTexture(ui::Texture* texture) {
  DCHECK(texture);
  SetPaintToLayer(true);

  layer()->SetExternalTexture(texture);

  // Child views must not paint into the external texture. So make sure each
  // child view has its own layer to paint into.
  for (Views::iterator i = children_.begin(); i != children_.end(); ++i)
    (*i)->SetPaintToLayer(true);

  SchedulePaintInRect(GetLocalBounds());
  return true;
}

gfx::Vector2d View::CalculateOffsetToAncestorWithLayer(
    ui::Layer** layer_parent) {
  if (layer()) {
    if (layer_parent)
      *layer_parent = layer();
    return gfx::Vector2d();
  }
  if (!parent_)
    return gfx::Vector2d();

  return gfx::Vector2d(GetMirroredX(), y()) +
      parent_->CalculateOffsetToAncestorWithLayer(layer_parent);
}

void View::MoveLayerToParent(ui::Layer* parent_layer,
                             const gfx::Point& point) {
  gfx::Point local_point(point);
  if (parent_layer != layer())
    local_point.Offset(GetMirroredX(), y());
  if (layer() && parent_layer != layer()) {
    parent_layer->Add(layer());
    SetLayerBounds(gfx::Rect(local_point.x(), local_point.y(),
                             width(), height()));
  } else {
    for (int i = 0, count = child_count(); i < count; ++i)
      child_at(i)->MoveLayerToParent(parent_layer, local_point);
  }
}

void View::UpdateLayerVisibility() {
  if (!use_acceleration_when_possible)
    return;
  bool visible = visible_;
  for (const View* v = parent_; visible && v && !v->layer(); v = v->parent_)
    visible = v->visible();

  UpdateChildLayerVisibility(visible);
}

void View::UpdateChildLayerVisibility(bool ancestor_visible) {
  if (layer()) {
    layer()->SetVisible(ancestor_visible && visible_);
  } else {
    for (int i = 0, count = child_count(); i < count; ++i)
      child_at(i)->UpdateChildLayerVisibility(ancestor_visible && visible_);
  }
}

void View::UpdateChildLayerBounds(const gfx::Vector2d& offset) {
  if (layer()) {
    SetLayerBounds(GetLocalBounds() + offset);
  } else {
    for (int i = 0, count = child_count(); i < count; ++i) {
      View* child = child_at(i);
      child->UpdateChildLayerBounds(
          offset + gfx::Vector2d(child->GetMirroredX(), child->y()));
    }
  }
}

void View::OnPaintLayer(gfx::Canvas* canvas) {
  if (!layer() || !layer()->fills_bounds_opaquely())
    canvas->DrawColor(SK_ColorBLACK, SkXfermode::kClear_Mode);
  PaintCommon(canvas);
}

void View::OnDeviceScaleFactorChanged(float device_scale_factor) {
  // Repainting with new scale factor will paint the content at the right scale.
}

base::Closure View::PrepareForLayerBoundsChange() {
  return base::Closure();
}

void View::ReorderLayers() {
  View* v = this;
  while (v && !v->layer())
    v = v->parent();

  // Forward to widget in case we're in a NativeWidgetAura.
  if (!v) {
    if (GetWidget())
      GetWidget()->ReorderLayers();
  } else {
    for (Views::const_iterator i(v->children_.begin());
         i != v->children_.end();
         ++i)
      (*i)->ReorderChildLayers(v->layer());
  }
}

void View::ReorderChildLayers(ui::Layer* parent_layer) {
  if (layer()) {
    DCHECK_EQ(parent_layer, layer()->parent());
    parent_layer->StackAtTop(layer());
  } else {
    for (Views::const_iterator i(children_.begin()); i != children_.end(); ++i)
      (*i)->ReorderChildLayers(parent_layer);
  }
}

// Input -----------------------------------------------------------------------

bool View::HasHitTestMask() const {
  return false;
}

void View::GetHitTestMask(gfx::Path* mask) const {
  DCHECK(mask);
}

View::DragInfo* View::GetDragInfo() {
  return parent_ ? parent_->GetDragInfo() : NULL;
}

// Focus -----------------------------------------------------------------------

void View::OnFocus() {
  // TODO(beng): Investigate whether it's possible for us to move this to
  //             Focus().
  // By default, we clear the native focus. This ensures that no visible native
  // view as the focus and that we still receive keyboard inputs.
  FocusManager* focus_manager = GetFocusManager();
  if (focus_manager)
    focus_manager->ClearNativeFocus();

  // TODO(beng): Investigate whether it's possible for us to move this to
  //             Focus().
  // Notify assistive technologies of the focus change.
  NotifyAccessibilityEvent(ui::AccessibilityTypes::EVENT_FOCUS, true);
}

void View::OnBlur() {
}

void View::Focus() {
  SchedulePaint();
  OnFocus();
}

void View::Blur() {
  SchedulePaint();
  OnBlur();
}

// Tooltips --------------------------------------------------------------------

void View::TooltipTextChanged() {
  Widget* widget = GetWidget();
  // TooltipManager may be null if there is a problem creating it.
  if (widget && widget->native_widget_private()->GetTooltipManager()) {
    widget->native_widget_private()->GetTooltipManager()->
        TooltipTextChanged(this);
  }
}

// Context menus ---------------------------------------------------------------

gfx::Point View::GetKeyboardContextMenuLocation() {
  gfx::Rect vis_bounds = GetVisibleBounds();
  gfx::Point screen_point(vis_bounds.x() + vis_bounds.width() / 2,
                          vis_bounds.y() + vis_bounds.height() / 2);
  ConvertPointToScreen(this, &screen_point);
  return screen_point;
}

// Drag and drop ---------------------------------------------------------------

int View::GetDragOperations(const gfx::Point& press_pt) {
  return drag_controller_ ?
      drag_controller_->GetDragOperationsForView(this, press_pt) :
      ui::DragDropTypes::DRAG_NONE;
}

void View::WriteDragData(const gfx::Point& press_pt, OSExchangeData* data) {
  DCHECK(drag_controller_);
  drag_controller_->WriteDragDataForView(this, press_pt, data);
}

bool View::InDrag() {
  Widget* widget = GetWidget();
  return widget ? widget->dragged_view() == this : false;
}

// Debugging -------------------------------------------------------------------

#if !defined(NDEBUG)

std::string View::PrintViewGraph(bool first) {
  return DoPrintViewGraph(first, this);
}

std::string View::DoPrintViewGraph(bool first, View* view_with_children) {
  // 64-bit pointer = 16 bytes of hex + "0x" + '\0' = 19.
  const size_t kMaxPointerStringLength = 19;

  std::string result;

  if (first)
    result.append("digraph {\n");

  // Node characteristics.
  char p[kMaxPointerStringLength];

  size_t base_name_index = GetClassName().find_last_of('/');
  if (base_name_index == std::string::npos)
    base_name_index = 0;
  else
    base_name_index++;

  char bounds_buffer[512];

  // Information about current node.
  base::snprintf(p, arraysize(bounds_buffer), "%p", view_with_children);
  result.append("  N");
  result.append(p + 2);
  result.append(" [label=\"");

  result.append(GetClassName().substr(base_name_index).c_str());

  base::snprintf(bounds_buffer,
                 arraysize(bounds_buffer),
                 "\\n bounds: (%d, %d), (%dx%d)",
                 bounds().x(),
                 bounds().y(),
                 bounds().width(),
                 bounds().height());
  result.append(bounds_buffer);

  gfx::DecomposedTransform decomp;
  if (!GetTransform().IsIdentity() &&
      gfx::DecomposeTransform(&decomp, GetTransform())) {
    base::snprintf(bounds_buffer,
                   arraysize(bounds_buffer),
                   "\\n translation: (%f, %f)",
                   decomp.translate[0],
                   decomp.translate[1]);
    result.append(bounds_buffer);

    base::snprintf(bounds_buffer,
                   arraysize(bounds_buffer),
                   "\\n rotation: %3.2f",
                   std::acos(decomp.quaternion[3]) * 360.0 / M_PI);
    result.append(bounds_buffer);

    base::snprintf(bounds_buffer,
                   arraysize(bounds_buffer),
                   "\\n scale: (%2.4f, %2.4f)",
                   decomp.scale[0],
                   decomp.scale[1]);
    result.append(bounds_buffer);
  }

  result.append("\"");
  if (!parent_)
    result.append(", shape=box");
  if (layer()) {
    if (layer()->texture())
      result.append(", color=green");
    else
      result.append(", color=red");

    if (layer()->fills_bounds_opaquely())
      result.append(", style=filled");
  }
  result.append("]\n");

  // Link to parent.
  if (parent_) {
    char pp[kMaxPointerStringLength];

    base::snprintf(pp, kMaxPointerStringLength, "%p", parent_);
    result.append("  N");
    result.append(pp + 2);
    result.append(" -> N");
    result.append(p + 2);
    result.append("\n");
  }

  // Children.
  for (int i = 0, count = view_with_children->child_count(); i < count; ++i)
    result.append(view_with_children->child_at(i)->PrintViewGraph(false));

  if (first)
    result.append("}\n");

  return result;
}

#endif

////////////////////////////////////////////////////////////////////////////////
// View, private:

// DropInfo --------------------------------------------------------------------

void View::DragInfo::Reset() {
  possible_drag = false;
  start_pt = gfx::Point();
}

void View::DragInfo::PossibleDrag(const gfx::Point& p) {
  possible_drag = true;
  start_pt = p;
}

// Painting --------------------------------------------------------------------

void View::SchedulePaintBoundsChanged(SchedulePaintType type) {
  // If we have a layer and the View's size did not change, we do not need to
  // schedule any paints since the layer will be redrawn at its new location
  // during the next Draw() cycle in the compositor.
  if (!layer() || type == SCHEDULE_PAINT_SIZE_CHANGED) {
    // Otherwise, if the size changes or we don't have a layer then we need to
    // use SchedulePaint to invalidate the area occupied by the View.
    SchedulePaint();
  } else if (parent_ && type == SCHEDULE_PAINT_SIZE_SAME) {
    // The compositor doesn't Draw() until something on screen changes, so
    // if our position changes but nothing is being animated on screen, then
    // tell the compositor to redraw the scene. We know layer() exists due to
    // the above if clause.
    layer()->ScheduleDraw();
  }
}

void View::PaintCommon(gfx::Canvas* canvas) {
  if (!visible_)
    return;

  {
    // If the View we are about to paint requested the canvas to be flipped, we
    // should change the transform appropriately.
    // The canvas mirroring is undone once the View is done painting so that we
    // don't pass the canvas with the mirrored transform to Views that didn't
    // request the canvas to be flipped.
    ScopedCanvas scoped(canvas);
    if (FlipCanvasOnPaintForRTLUI()) {
      canvas->Translate(gfx::Vector2d(width(), 0));
      canvas->Scale(-1, 1);
    }

    OnPaint(canvas);
  }

  PaintChildren(canvas);
}

// Tree operations -------------------------------------------------------------

void View::DoRemoveChildView(View* view,
                             bool update_focus_cycle,
                             bool update_tool_tip,
                             bool delete_removed_view) {
  DCHECK(view);
  const Views::iterator i(std::find(children_.begin(), children_.end(), view));
  scoped_ptr<View> view_to_be_deleted;
  if (i != children_.end()) {
    if (update_focus_cycle) {
      // Let's remove the view from the focus traversal.
      View* next_focusable = view->next_focusable_view_;
      View* prev_focusable = view->previous_focusable_view_;
      if (prev_focusable)
        prev_focusable->next_focusable_view_ = next_focusable;
      if (next_focusable)
        next_focusable->previous_focusable_view_ = prev_focusable;
    }

    if (GetWidget())
      UnregisterChildrenForVisibleBoundsNotification(view);
    view->PropagateRemoveNotifications(this);
    view->parent_ = NULL;
    view->UpdateLayerVisibility();

    if (delete_removed_view && !view->owned_by_client_)
      view_to_be_deleted.reset(view);

    children_.erase(i);
  }

  if (update_tool_tip)
    UpdateTooltip();

  if (layout_manager_.get())
    layout_manager_->ViewRemoved(this, view);
}

void View::PropagateRemoveNotifications(View* parent) {
  for (int i = 0, count = child_count(); i < count; ++i)
    child_at(i)->PropagateRemoveNotifications(parent);

  for (View* v = this; v; v = v->parent_)
    v->ViewHierarchyChangedImpl(true, false, parent, this);
}

void View::PropagateAddNotifications(View* parent, View* child) {
  for (int i = 0, count = child_count(); i < count; ++i)
    child_at(i)->PropagateAddNotifications(parent, child);
  ViewHierarchyChangedImpl(true, true, parent, child);
}

void View::PropagateNativeViewHierarchyChanged(bool attached,
                                               gfx::NativeView native_view,
                                               internal::RootView* root_view) {
  for (int i = 0, count = child_count(); i < count; ++i)
    child_at(i)->PropagateNativeViewHierarchyChanged(attached,
                                                           native_view,
                                                           root_view);
  NativeViewHierarchyChanged(attached, native_view, root_view);
}

void View::ViewHierarchyChangedImpl(bool register_accelerators,
                                    bool is_add,
                                    View* parent,
                                    View* child) {
  if (register_accelerators) {
    if (is_add) {
      // If you get this registration, you are part of a subtree that has been
      // added to the view hierarchy.
      if (GetFocusManager()) {
        RegisterPendingAccelerators();
      } else {
        // Delay accelerator registration until visible as we do not have
        // focus manager until then.
        accelerator_registration_delayed_ = true;
      }
    } else {
      if (child == this)
        UnregisterAccelerators(true);
    }
  }

  if (is_add && layer() && !layer()->parent()) {
    UpdateParentLayer();
    Widget* widget = GetWidget();
    if (widget)
      widget->UpdateRootLayers();
  } else if (!is_add && child == this) {
    // Make sure the layers beloning to the subtree rooted at |child| get
    // removed from layers that do not belong in the same subtree.
    OrphanLayers();
    if (use_acceleration_when_possible) {
      Widget* widget = GetWidget();
      if (widget)
        widget->UpdateRootLayers();
    }
  }

  ViewHierarchyChanged(is_add, parent, child);
  parent->needs_layout_ = true;
}

void View::PropagateNativeThemeChanged(const ui::NativeTheme* theme) {
  for (int i = 0, count = child_count(); i < count; ++i)
    child_at(i)->PropagateNativeThemeChanged(theme);
  OnNativeThemeChanged(theme);
}

// Size and disposition --------------------------------------------------------

void View::PropagateVisibilityNotifications(View* start, bool is_visible) {
  for (int i = 0, count = child_count(); i < count; ++i)
    child_at(i)->PropagateVisibilityNotifications(start, is_visible);
  VisibilityChangedImpl(start, is_visible);
}

void View::VisibilityChangedImpl(View* starting_from, bool is_visible) {
  VisibilityChanged(starting_from, is_visible);
}

void View::BoundsChanged(const gfx::Rect& previous_bounds) {
  if (visible_) {
    // Paint the new bounds.
    SchedulePaintBoundsChanged(
        bounds_.size() == previous_bounds.size() ? SCHEDULE_PAINT_SIZE_SAME :
        SCHEDULE_PAINT_SIZE_CHANGED);
  }

  if (use_acceleration_when_possible) {
    if (layer()) {
      if (parent_) {
        SetLayerBounds(GetLocalBounds() +
                       gfx::Vector2d(GetMirroredX(), y()) +
                       parent_->CalculateOffsetToAncestorWithLayer(NULL));
      } else {
        SetLayerBounds(bounds_);
      }
      // TODO(beng): this seems redundant with the SchedulePaint at the top of
      //             this function. explore collapsing.
      if (previous_bounds.size() != bounds_.size() &&
          !layer()->layer_updated_externally()) {
        // If our bounds have changed then we need to update the complete
        // texture.
        layer()->SchedulePaint(GetLocalBounds());
      }
    } else {
      // If our bounds have changed, then any descendant layer bounds may
      // have changed. Update them accordingly.
      UpdateChildLayerBounds(CalculateOffsetToAncestorWithLayer(NULL));
    }
  }

  OnBoundsChanged(previous_bounds);

  if (previous_bounds.size() != size()) {
    needs_layout_ = false;
    Layout();
  }

  if (NeedsNotificationWhenVisibleBoundsChange())
    OnVisibleBoundsChanged();

  // Notify interested Views that visible bounds within the root view may have
  // changed.
  if (descendants_to_notify_.get()) {
    for (Views::iterator i(descendants_to_notify_->begin());
         i != descendants_to_notify_->end(); ++i) {
      (*i)->OnVisibleBoundsChanged();
    }
  }
}

// static
void View::RegisterChildrenForVisibleBoundsNotification(View* view) {
  if (view->NeedsNotificationWhenVisibleBoundsChange())
    view->RegisterForVisibleBoundsNotification();
  for (int i = 0; i < view->child_count(); ++i)
    RegisterChildrenForVisibleBoundsNotification(view->child_at(i));
}

// static
void View::UnregisterChildrenForVisibleBoundsNotification(View* view) {
  if (view->NeedsNotificationWhenVisibleBoundsChange())
    view->UnregisterForVisibleBoundsNotification();
  for (int i = 0; i < view->child_count(); ++i)
    UnregisterChildrenForVisibleBoundsNotification(view->child_at(i));
}

void View::RegisterForVisibleBoundsNotification() {
  if (registered_for_visible_bounds_notification_)
    return;

  registered_for_visible_bounds_notification_ = true;
  for (View* ancestor = parent_; ancestor; ancestor = ancestor->parent_)
    ancestor->AddDescendantToNotify(this);
}

void View::UnregisterForVisibleBoundsNotification() {
  if (!registered_for_visible_bounds_notification_)
    return;

  registered_for_visible_bounds_notification_ = false;
  for (View* ancestor = parent_; ancestor; ancestor = ancestor->parent_)
    ancestor->RemoveDescendantToNotify(this);
}

void View::AddDescendantToNotify(View* view) {
  DCHECK(view);
  if (!descendants_to_notify_.get())
    descendants_to_notify_.reset(new Views);
  descendants_to_notify_->push_back(view);
}

void View::RemoveDescendantToNotify(View* view) {
  DCHECK(view && descendants_to_notify_.get());
  Views::iterator i(std::find(
      descendants_to_notify_->begin(), descendants_to_notify_->end(), view));
  DCHECK(i != descendants_to_notify_->end());
  descendants_to_notify_->erase(i);
  if (descendants_to_notify_->empty())
    descendants_to_notify_.reset();
}

void View::SetLayerBounds(const gfx::Rect& bounds) {
  layer()->SetBounds(bounds);
}

// Transformations -------------------------------------------------------------

bool View::GetTransformRelativeTo(const View* ancestor,
                                  gfx::Transform* transform) const {
  const View* p = this;

  while (p && p != ancestor) {
    transform->ConcatTransform(p->GetTransform());
    gfx::Transform translation;
    translation.Translate(static_cast<float>(p->GetMirroredX()),
                          static_cast<float>(p->y()));
    transform->ConcatTransform(translation);

    p = p->parent_;
  }

  return p == ancestor;
}

// Coordinate conversion -------------------------------------------------------

bool View::ConvertPointForAncestor(const View* ancestor,
                                   gfx::Point* point) const {
  gfx::Transform trans;
  // TODO(sad): Have some way of caching the transformation results.
  bool result = GetTransformRelativeTo(ancestor, &trans);
  gfx::Point3F p(*point);
  trans.TransformPoint(p);
  *point = gfx::ToFlooredPoint(p.AsPointF());
  return result;
}

bool View::ConvertPointFromAncestor(const View* ancestor,
                                    gfx::Point* point) const {
  gfx::Transform trans;
  bool result = GetTransformRelativeTo(ancestor, &trans);
  gfx::Point3F p(*point);
  trans.TransformPointReverse(p);
  *point = gfx::ToFlooredPoint(p.AsPointF());
  return result;
}

// Accelerated painting --------------------------------------------------------

void View::CreateLayer() {
  // A new layer is being created for the view. So all the layers of the
  // sub-tree can inherit the visibility of the corresponding view.
  for (int i = 0, count = child_count(); i < count; ++i)
    child_at(i)->UpdateChildLayerVisibility(true);

  layer_ = new ui::Layer();
  layer_owner_.reset(layer_);
  layer_->set_delegate(this);
#if !defined(NDEBUG)
  layer_->set_name(GetClassName());
#endif

  UpdateParentLayers();
  UpdateLayerVisibility();

  // The new layer needs to be ordered in the layer tree according
  // to the view tree. Children of this layer were added in order
  // in UpdateParentLayers().
  if (parent())
    parent()->ReorderLayers();

  Widget* widget = GetWidget();
  if (widget)
    widget->UpdateRootLayers();
}

void View::UpdateParentLayers() {
  // Attach all top-level un-parented layers.
  if (layer() && !layer()->parent()) {
    UpdateParentLayer();
  } else {
    for (int i = 0, count = child_count(); i < count; ++i)
      child_at(i)->UpdateParentLayers();
  }
}

void View::UpdateParentLayer() {
  if (!layer())
    return;

  ui::Layer* parent_layer = NULL;
  gfx::Vector2d offset(GetMirroredX(), y());

  // TODO(sad): The NULL check here for parent_ essentially is to check if this
  // is the RootView. Instead of doing this, this function should be made
  // virtual and overridden from the RootView.
  if (parent_)
    offset += parent_->CalculateOffsetToAncestorWithLayer(&parent_layer);
  else if (!parent_ && GetWidget())
    offset += GetWidget()->CalculateOffsetToAncestorWithLayer(&parent_layer);

  ReparentLayer(offset, parent_layer);
}

void View::OrphanLayers() {
  if (layer()) {
    if (layer()->parent())
      layer()->parent()->Remove(layer());

    // The layer belonging to this View has already been orphaned. It is not
    // necessary to orphan the child layers.
    return;
  }
  for (int i = 0, count = child_count(); i < count; ++i)
    child_at(i)->OrphanLayers();
}

void View::ReparentLayer(const gfx::Vector2d& offset, ui::Layer* parent_layer) {
  layer_->SetBounds(GetLocalBounds() + offset);
  DCHECK_NE(layer(), parent_layer);
  if (parent_layer)
    parent_layer->Add(layer());
  layer_->SchedulePaint(GetLocalBounds());
  MoveLayerToParent(layer(), gfx::Point());
}

void View::DestroyLayer() {
  ui::Layer* new_parent = layer()->parent();
  std::vector<ui::Layer*> children = layer()->children();
  for (size_t i = 0; i < children.size(); ++i) {
    layer()->Remove(children[i]);
    if (new_parent)
      new_parent->Add(children[i]);
  }

  layer_ = NULL;
  layer_owner_.reset();

  if (new_parent)
    ReorderLayers();

  UpdateChildLayerBounds(CalculateOffsetToAncestorWithLayer(NULL));

  SchedulePaint();

  Widget* widget = GetWidget();
  if (widget)
    widget->UpdateRootLayers();
}

// Input -----------------------------------------------------------------------

bool View::ProcessMousePressed(const ui::MouseEvent& event) {
  int drag_operations =
      (enabled_ && event.IsOnlyLeftMouseButton() &&
       HitTestPoint(event.location())) ?
       GetDragOperations(event.location()) : 0;
  ContextMenuController* context_menu_controller = event.IsRightMouseButton() ?
      context_menu_controller_ : 0;
  View::DragInfo* drag_info = GetDragInfo();

  const bool enabled = enabled_;
  const bool result = OnMousePressed(event);

  if (!enabled)
    return result;

  if (event.IsOnlyRightMouseButton() && context_menu_controller &&
      kContextMenuOnMousePress) {
    // Assume that if there is a context menu controller we won't be deleted
    // from mouse pressed.
    gfx::Point location(event.location());
    if (HitTestPoint(location)) {
      ConvertPointToScreen(this, &location);
      ShowContextMenu(location, true);
      return true;
    }
  }

  // WARNING: we may have been deleted, don't use any View variables.
  if (drag_operations != ui::DragDropTypes::DRAG_NONE) {
    drag_info->PossibleDrag(event.location());
    return true;
  }
  return !!context_menu_controller || result;
}

bool View::ProcessMouseDragged(const ui::MouseEvent& event) {
  // Copy the field, that way if we're deleted after drag and drop no harm is
  // done.
  ContextMenuController* context_menu_controller = context_menu_controller_;
  const bool possible_drag = GetDragInfo()->possible_drag;
  if (possible_drag &&
      ExceededDragThreshold(GetDragInfo()->start_pt - event.location()) &&
      (!drag_controller_ ||
       drag_controller_->CanStartDragForView(
           this, GetDragInfo()->start_pt, event.location()))) {
    DoDrag(event, GetDragInfo()->start_pt,
           ui::DragDropTypes::DRAG_EVENT_SOURCE_MOUSE);
  } else {
    if (OnMouseDragged(event))
      return true;
    // Fall through to return value based on context menu controller.
  }
  // WARNING: we may have been deleted.
  return (context_menu_controller != NULL) || possible_drag;
}

void View::ProcessMouseReleased(const ui::MouseEvent& event) {
  if (!kContextMenuOnMousePress && context_menu_controller_ &&
      event.IsOnlyRightMouseButton()) {
    // Assume that if there is a context menu controller we won't be deleted
    // from mouse released.
    gfx::Point location(event.location());
    OnMouseReleased(event);
    if (HitTestPoint(location)) {
      ConvertPointToScreen(this, &location);
      ShowContextMenu(location, true);
    }
  } else {
    OnMouseReleased(event);
  }
  // WARNING: we may have been deleted.
}

// Accelerators ----------------------------------------------------------------

void View::RegisterPendingAccelerators() {
  if (!accelerators_.get() ||
      registered_accelerator_count_ == accelerators_->size()) {
    // No accelerators are waiting for registration.
    return;
  }

  if (!GetWidget()) {
    // The view is not yet attached to a widget, defer registration until then.
    return;
  }

  accelerator_focus_manager_ = GetFocusManager();
  if (!accelerator_focus_manager_) {
    // Some crash reports seem to show that we may get cases where we have no
    // focus manager (see bug #1291225).  This should never be the case, just
    // making sure we don't crash.
    NOTREACHED();
    return;
  }
  for (std::vector<ui::Accelerator>::const_iterator i(
           accelerators_->begin() + registered_accelerator_count_);
       i != accelerators_->end(); ++i) {
    accelerator_focus_manager_->RegisterAccelerator(
        *i, ui::AcceleratorManager::kNormalPriority, this);
  }
  registered_accelerator_count_ = accelerators_->size();
}

void View::UnregisterAccelerators(bool leave_data_intact) {
  if (!accelerators_.get())
    return;

  if (GetWidget()) {
    if (accelerator_focus_manager_) {
      // We may not have a FocusManager if the window containing us is being
      // closed, in which case the FocusManager is being deleted so there is
      // nothing to unregister.
      accelerator_focus_manager_->UnregisterAccelerators(this);
      accelerator_focus_manager_ = NULL;
    }
    if (!leave_data_intact) {
      accelerators_->clear();
      accelerators_.reset();
    }
    registered_accelerator_count_ = 0;
  }
}

// Focus -----------------------------------------------------------------------

void View::InitFocusSiblings(View* v, int index) {
  int count = child_count();

  if (count == 0) {
    v->next_focusable_view_ = NULL;
    v->previous_focusable_view_ = NULL;
  } else {
    if (index == count) {
      // We are inserting at the end, but the end of the child list may not be
      // the last focusable element. Let's try to find an element with no next
      // focusable element to link to.
      View* last_focusable_view = NULL;
      for (Views::iterator i(children_.begin()); i != children_.end(); ++i) {
          if (!(*i)->next_focusable_view_) {
            last_focusable_view = *i;
            break;
          }
      }
      if (last_focusable_view == NULL) {
        // Hum... there is a cycle in the focus list. Let's just insert ourself
        // after the last child.
        View* prev = children_[index - 1];
        v->previous_focusable_view_ = prev;
        v->next_focusable_view_ = prev->next_focusable_view_;
        prev->next_focusable_view_->previous_focusable_view_ = v;
        prev->next_focusable_view_ = v;
      } else {
        last_focusable_view->next_focusable_view_ = v;
        v->next_focusable_view_ = NULL;
        v->previous_focusable_view_ = last_focusable_view;
      }
    } else {
      View* prev = children_[index]->GetPreviousFocusableView();
      v->previous_focusable_view_ = prev;
      v->next_focusable_view_ = children_[index];
      if (prev)
        prev->next_focusable_view_ = v;
      children_[index]->previous_focusable_view_ = v;
    }
  }
}

// System events ---------------------------------------------------------------

void View::PropagateThemeChanged() {
  for (int i = child_count() - 1; i >= 0; --i)
    child_at(i)->PropagateThemeChanged();
  OnThemeChanged();
}

void View::PropagateLocaleChanged() {
  for (int i = child_count() - 1; i >= 0; --i)
    child_at(i)->PropagateLocaleChanged();
  OnLocaleChanged();
}

// Tooltips --------------------------------------------------------------------

void View::UpdateTooltip() {
  Widget* widget = GetWidget();
  // TODO(beng): The TooltipManager NULL check can be removed when we
  //             consolidate Init() methods and make views_unittests Init() all
  //             Widgets that it uses.
  if (widget && widget->native_widget_private()->GetTooltipManager())
    widget->native_widget_private()->GetTooltipManager()->UpdateTooltip();
}

// Drag and drop ---------------------------------------------------------------

bool View::DoDrag(const ui::LocatedEvent& event,
                  const gfx::Point& press_pt,
                  ui::DragDropTypes::DragEventSource source) {
#if !defined(OS_MACOSX)
  int drag_operations = GetDragOperations(press_pt);
  if (drag_operations == ui::DragDropTypes::DRAG_NONE)
    return false;

  OSExchangeData data;
  WriteDragData(press_pt, &data);

  // Message the RootView to do the drag and drop. That way if we're removed
  // the RootView can detect it and avoid calling us back.
  gfx::Point widget_location(event.location());
  ConvertPointToWidget(this, &widget_location);
  GetWidget()->RunShellDrag(this, data, widget_location, drag_operations,
      source);
  return true;
#else
  return false;
#endif  // !defined(OS_MACOSX)
}

}  // namespace views
