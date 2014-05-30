// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_WINDOW_H_
#define UI_AURA_WINDOW_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/string16.h"
#include "ui/aura/aura_export.h"
#include "ui/aura/client/window_types.h"
#include "ui/aura/window_observer.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/events/event_target.h"
#include "ui/base/gestures/gesture_types.h"
#include "ui/compositor/layer_animator.h"
#include "ui/compositor/layer_delegate.h"
#include "ui/compositor/layer_owner.h"
#include "ui/compositor/layer_type.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/rect.h"

namespace gfx {
class Display;
class Transform;
}

namespace ui {
class EventHandler;
class Layer;
class Texture;
}

namespace aura {

class LayoutManager;
class RootWindow;
class WindowDelegate;
class WindowObserver;

// Defined in window_property.h (which we do not include)
template<typename T>
struct WindowProperty;

namespace test {
class WindowTestApi;
}

// Aura window implementation. Interesting events are sent to the
// WindowDelegate.
// TODO(beng): resolve ownership.
class AURA_EXPORT Window : public ui::LayerDelegate,
                           public ui::LayerOwner,
                           public ui::EventTarget,
                           public ui::GestureConsumer {
 public:
  typedef std::vector<Window*> Windows;

  explicit Window(WindowDelegate* delegate);
  virtual ~Window();

  // Initializes the window. This creates the window's layer.
  void Init(ui::LayerType layer_type);

  // Creates a new layer for the window. Erases the layer-owned bounds, so the
  // caller may wish to set new bounds and other state on the window/layer.
  // Returns the old layer, which can be used for animations. Caller owns the
  // memory for the returned layer and must delete it when animation completes.
  // Returns NULL and does not recreate layer if window does not own its layer.
  ui::Layer* RecreateLayer() WARN_UNUSED_RESULT;

  void set_owned_by_parent(bool owned_by_parent) {
    owned_by_parent_ = owned_by_parent;
  }

  // A type is used to identify a class of Windows and customize behavior such
  // as event handling and parenting.  This field should only be consumed by the
  // shell -- Aura itself shouldn't contain type-specific logic.
  client::WindowType type() const { return type_; }
  void SetType(client::WindowType type);

  int id() const { return id_; }
  void set_id(int id) { id_ = id; }

  const std::string& name() const { return name_; }
  void SetName(const std::string& name);

  const string16 title() const { return title_; }
  void set_title(const string16& title) { title_ = title; }

  bool transparent() const { return transparent_; }
  void SetTransparent(bool transparent);

  WindowDelegate* delegate() { return delegate_; }

  const gfx::Rect& bounds() const;

  Window* parent() { return parent_; }
  const Window* parent() const { return parent_; }

  // Returns the RootWindow that contains this Window or NULL if the Window is
  // not contained by a RootWindow.
  virtual RootWindow* GetRootWindow();
  virtual const RootWindow* GetRootWindow() const;

  // The Window does not own this object.
  void set_user_data(void* user_data) { user_data_ = user_data; }
  void* user_data() const { return user_data_; }

  // Changes the visibility of the window.
  void Show();
  void Hide();
  // Returns true if this window and all its ancestors are visible.
  bool IsVisible() const;
  // Returns the visibility requested by this window. IsVisible() takes into
  // account the visibility of the layer and ancestors, where as this tracks
  // whether Show() without a Hide() has been invoked.
  bool TargetVisibility() const { return visible_; }

  // Returns the window's bounds in root window's coordinates.
  gfx::Rect GetBoundsInRootWindow() const;

  // Returns the window's bounds in screen coordinates.
  // How the root window's coordinates is mapped to screen's coordinates
  // is platform dependent and defined in the implementation of the
  // |aura::client::ScreenPositionClient| interface.
  gfx::Rect GetBoundsInScreen() const;

  virtual void SetTransform(const gfx::Transform& transform);

  // Assigns a LayoutManager to size and place child windows.
  // The Window takes ownership of the LayoutManager.
  void SetLayoutManager(LayoutManager* layout_manager);
  LayoutManager* layout_manager() { return layout_manager_.get(); }

  // Changes the bounds of the window. If present, the window's parent's
  // LayoutManager may adjust the bounds.
  void SetBounds(const gfx::Rect& new_bounds);

  // Changes the bounds of the window in the screen coordintates.
  // If present, the window's parent's LayoutManager may adjust the bounds.
  void SetBoundsInScreen(const gfx::Rect& new_bounds_in_screen_coords,
                         const gfx::Display& dst_display);

  // Returns the target bounds of the window. If the window's layer is
  // not animating, it simply returns the current bounds.
  gfx::Rect GetTargetBounds() const;

  // Marks the a portion of window as needing to be painted.
  void SchedulePaintInRect(const gfx::Rect& rect);

  // Assigns a new external texture to the window's layer.
  void SetExternalTexture(ui::Texture* texture);

  // Places this window per |root_window|'s stacking client. The final location
  // may be a RootWindow other than the one passed in. |root_window| may not be
  // NULL. |bounds_in_screen| may be empty; it is more optional context that
  // may, but isn't necessarily used.
  void SetDefaultParentByRootWindow(RootWindow* root_window,
                                    const gfx::Rect& bounds_in_screen);

  // Stacks the specified child of this Window at the front of the z-order.
  void StackChildAtTop(Window* child);

  // Stacks |child| above |target|.  Does nothing if |child| is already above
  // |target|.  Does not stack on top of windows with NULL layer delegates,
  // see WindowTest.StackingMadrigal for details.
  void StackChildAbove(Window* child, Window* target);

  // Stacks |child| below |target|. Does nothing if |child| is already below
  // |target|.
  void StackChildBelow(Window* child, Window* target);

  // Tree operations.
  void AddChild(Window* child);
  void RemoveChild(Window* child);

  const Windows& children() const { return children_; }

  // Returns true if this Window contains |other| somewhere in its children.
  bool Contains(const Window* other) const;

  // Adds or removes |child| as a transient child of this window. Transient
  // children get the following behavior:
  // . The transient parent destroys any transient children when it is
  //   destroyed. This means a transient child is destroyed if either its parent
  //   or transient parent is destroyed.
  // . If a transient child and its transient parent share the same parent, then
  //   transient children are always ordered above the transient parent.
  // Transient windows are typically used for popups and menus.
  void AddTransientChild(Window* child);
  void RemoveTransientChild(Window* child);

  const Windows& transient_children() const { return transient_children_; }

  Window* transient_parent() { return transient_parent_; }
  const Window* transient_parent() const { return transient_parent_; }

  // Retrieves the first-level child with the specified id, or NULL if no first-
  // level child is found matching |id|.
  Window* GetChildById(int id);
  const Window* GetChildById(int id) const;

  // Converts |point| from |source|'s coordinates to |target|'s. If |source| is
  // NULL, the function returns without modifying |point|. |target| cannot be
  // NULL.
  static void ConvertPointToTarget(const Window* source,
                                   const Window* target,
                                   gfx::Point* point);

  // Moves the cursor to the specified location relative to the window.
  virtual void MoveCursorTo(const gfx::Point& point_in_window);

  // Returns the cursor for the specified point, in window coordinates.
  gfx::NativeCursor GetCursor(const gfx::Point& point) const;

  // Sets an 'event filter' for the window. An 'event filter' for a Window is
  // a pre-target event handler, where the window owns the handler. A window
  // can have only one such event filter. Setting a new filter removes and
  // destroys any previously installed filter.
  void SetEventFilter(ui::EventHandler* event_filter);

  // Add/remove observer.
  void AddObserver(WindowObserver* observer);
  void RemoveObserver(WindowObserver* observer);
  bool HasObserver(WindowObserver* observer);

  void set_ignore_events(bool ignore_events) { ignore_events_ = ignore_events; }

  // Sets the window to grab hits for mouse and touch to an area extending
  // -|mouse_insets| and -|touch_insets| pixels outside its bounds. This can be
  // used to create an invisible non-client area, for example if your windows
  // have no visible frames but still need to have resize edges.
  void SetHitTestBoundsOverrideOuter(const gfx::Insets& mouse_insets,
                                     const gfx::Insets& touch_insets) {
    hit_test_bounds_override_outer_mouse_ = mouse_insets;
    hit_test_bounds_override_outer_touch_ = touch_insets;
  }

  gfx::Insets hit_test_bounds_override_outer_mouse() const {
    return hit_test_bounds_override_outer_mouse_;
  }

  // Sets the window to grab hits for an area extending |insets| pixels inside
  // its bounds (even if that inner region overlaps a child window). This can be
  // used to create an invisible non-client area that overlaps the client area.
  void set_hit_test_bounds_override_inner(const gfx::Insets& insets) {
    hit_test_bounds_override_inner_ = insets;
  }
  gfx::Insets hit_test_bounds_override_inner() const {
    return hit_test_bounds_override_inner_;
  }

  // Returns true if the |point_in_root| in root window's coordinate falls
  // within this window's bounds. Returns false if the window is detached
  // from root window.
  bool ContainsPointInRoot(const gfx::Point& point_in_root) const;

  // Returns true if relative-to-this-Window's-origin |local_point| falls
  // within this Window's bounds.
  bool ContainsPoint(const gfx::Point& local_point) const;

  // Returns true if the mouse pointer at relative-to-this-Window's-origin
  // |local_point| can trigger an event for this Window.
  // TODO(beng): A Window can supply a hit-test mask to cause some portions of
  // itself to not trigger events, causing the events to fall through to the
  // Window behind.
  bool HitTest(const gfx::Point& local_point);

  // Returns the Window that most closely encloses |local_point| for the
  // purposes of event targeting.
  Window* GetEventHandlerForPoint(const gfx::Point& local_point);

  // Returns the topmost Window with a delegate containing |local_point|.
  Window* GetTopWindowContainingPoint(const gfx::Point& local_point);

  // Returns this window's toplevel window (the highest-up-the-tree anscestor
  // that has a delegate set).  The toplevel window may be |this|.
  Window* GetToplevelWindow();

  // Claims or relinquishes the claim to focus.
  void Focus();
  void Blur();

  // Returns true if the Window is currently the focused window.
  bool HasFocus() const;

  // Returns true if the Window can be focused.
  virtual bool CanFocus() const;

  // Returns true if the Window can receive events.
  virtual bool CanReceiveEvents() const;

  // Does a capture on the window. This does nothing if the window isn't showing
  // (VISIBILITY_SHOWN) or isn't contained in a valid window hierarchy.
  void SetCapture();

  // Releases a capture.
  void ReleaseCapture();

  // Returns true if this window has capture.
  bool HasCapture();

  // Suppresses painting window content by disgarding damaged rect and ignoring
  // new paint requests.
  void SuppressPaint();

  // Sets the |value| of the given window |property|. Setting to the default
  // value (e.g., NULL) removes the property. The caller is responsible for the
  // lifetime of any object set as a property on the Window.
  template<typename T>
  void SetProperty(const WindowProperty<T>* property, T value);

  // Returns the value of the given window |property|.  Returns the
  // property-specific default value if the property was not previously set.
  template<typename T>
  T GetProperty(const WindowProperty<T>* property) const;

  // Sets the |property| to its default value. Useful for avoiding a cast when
  // setting to NULL.
  template<typename T>
  void ClearProperty(const WindowProperty<T>* property);

  // NativeWidget::[GS]etNativeWindowProperty use strings as keys, and this is
  // difficult to change while retaining compatibility with other platforms.
  // TODO(benrg): Find a better solution.
  void SetNativeWindowProperty(const char* key, void* value);
  void* GetNativeWindowProperty(const char* key) const;

  // Type of a function to delete a property that this window owns.
  typedef void (*PropertyDeallocator)(int64 value);

  // Overridden from ui::LayerDelegate:
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;

#ifndef NDEBUG
  // These methods are useful when debugging.
  std::string GetDebugInfo() const;
  void PrintWindowHierarchy(int depth) const;
#endif

 private:
  friend class test::WindowTestApi;
  friend class LayoutManager;

  // Used when stacking windows.
  enum StackDirection {
    STACK_ABOVE,
    STACK_BELOW
  };

  // Called by the public {Set,Get,Clear}Property functions.
  int64 SetPropertyInternal(const void* key,
                            const char* name,
                            PropertyDeallocator deallocator,
                            int64 value,
                            int64 default_value);
  int64 GetPropertyInternal(const void* key, int64 default_value) const;

  // Changes the bounds of the window without condition.
  void SetBoundsInternal(const gfx::Rect& new_bounds);

  // Updates the visible state of the layer, but does not make visible-state
  // specific changes. Called from Show()/Hide().
  void SetVisible(bool visible);

  // Schedules a paint for the Window's entire bounds.
  void SchedulePaint();

  // Gets a Window (either this one or a subwindow) containing |local_point|.
  // If |return_tightest| is true, returns the tightest-containing (i.e.
  // furthest down the hierarchy) Window containing the point; otherwise,
  // returns the loosest.  If |for_event_handling| is true, then hit-test masks
  // are honored; otherwise, only bounds checks are performed.
  Window* GetWindowForPoint(const gfx::Point& local_point,
                            bool return_tightest,
                            bool for_event_handling);

  // Implementation of RemoveChild(). If |child| is being removed as the result
  // of an add, |new_parent| is the new parent |child| is going to be parented
  // to.
  void RemoveChildImpl(Window* child, Window* new_parent);

  // Called when this window's parent has changed.
  void OnParentChanged();

  // Determines the real location for stacking |child| and invokes
  // StackChildRelativeToImpl().
  void StackChildRelativeTo(Window* child,
                            Window* target,
                            StackDirection direction);

  // Implementation of StackChildRelativeTo().
  void StackChildRelativeToImpl(Window* child,
                                Window* target,
                                StackDirection direction);

  // Called when this window's stacking order among its siblings is changed.
  void OnStackingChanged();

  // Notifies observers registered with this Window (and its subtree) when the
  // Window has been added or is about to be removed from a RootWindow.
  void NotifyRemovingFromRootWindow();
  void NotifyAddedToRootWindow();

  // Methods implementing hierarchy change notifications. See WindowObserver for
  // more details.
  void NotifyWindowHierarchyChange(
      const WindowObserver::HierarchyChangeParams& params);
  // Notifies this window and its child hierarchy.
  void NotifyWindowHierarchyChangeDown(
      const WindowObserver::HierarchyChangeParams& params);
  // Notifies this window and its parent hierarchy.
  void NotifyWindowHierarchyChangeUp(
      const WindowObserver::HierarchyChangeParams& params);
  // Notifies this window's observers.
  void NotifyWindowHierarchyChangeAtReceiver(
      const WindowObserver::HierarchyChangeParams& params);

  // Methods implementing visibility change notifications. See WindowObserver
  // for more details.
  void NotifyWindowVisibilityChanged(aura::Window* target, bool visible);
  // Notifies this window's observers. Returns false if |this| was deleted
  // during the call (by an observer), otherwise true.
  bool NotifyWindowVisibilityChangedAtReceiver(aura::Window* target,
                                               bool visible);
  // Notifies this window and its child hierarchy. Returns false if
  // |this| was deleted during the call (by an observer), otherwise
  // true.
  bool NotifyWindowVisibilityChangedDown(aura::Window* target, bool visible);
  // Notifies this window and its parent hierarchy.
  void NotifyWindowVisibilityChangedUp(aura::Window* target, bool visible);

  // Invoked from the closure returned by PrepareForLayerBoundsChange() after
  // the bounds of the layer has changed. |old_bounds| is the previous bounds of
  // the layer, and |contained_mouse| is true if the mouse was previously within
  // the window's bounds.
  void OnLayerBoundsChanged(const gfx::Rect& old_bounds, bool contained_mouse);

  // Overridden from ui::LayerDelegate:
  virtual void OnPaintLayer(gfx::Canvas* canvas) OVERRIDE;
  virtual base::Closure PrepareForLayerBoundsChange() OVERRIDE;

  // Overridden from ui::EventTarget:
  virtual bool CanAcceptEvent(const ui::Event& event) OVERRIDE;
  virtual EventTarget* GetParentTarget() OVERRIDE;

  // Updates the layer name with a name based on the window's name and id.
  void UpdateLayerName(const std::string& name);

  // Returns true if the mouse is currently within our bounds.
  bool ContainsMouse();

  client::WindowType type_;

  // True if the Window is owned by its parent - i.e. it will be deleted by its
  // parent during its parents destruction. True is the default.
  bool owned_by_parent_;

  WindowDelegate* delegate_;

  // The Window's parent.
  Window* parent_;

  // Child windows. Topmost is last.
  Windows children_;

  // Transient windows.
  Windows transient_children_;

  Window* transient_parent_;

  // The visibility state of the window as set by Show()/Hide(). This may differ
  // from the visibility of the underlying layer, which may remain visible after
  // the window is hidden (e.g. to animate its disappearance).
  bool visible_;

  int id_;
  std::string name_;

  string16 title_;

  // Whether layer is initialized as non-opaque.
  bool transparent_;

  scoped_ptr<ui::EventHandler> event_filter_;
  scoped_ptr<LayoutManager> layout_manager_;

  void* user_data_;

  // Makes the window pass all events through to any windows behind it.
  bool ignore_events_;

  // See set_hit_test_outer_override().
  gfx::Insets hit_test_bounds_override_outer_mouse_;
  gfx::Insets hit_test_bounds_override_outer_touch_;
  gfx::Insets hit_test_bounds_override_inner_;

  ObserverList<WindowObserver> observers_;

  // Value struct to keep the name and deallocator for this property.
  // Key cannot be used for this purpose because it can be char* or
  // WindowProperty<>.
  struct Value {
    const char* name;
    int64 value;
    PropertyDeallocator deallocator;
  };

  std::map<const void*, Value> prop_map_;

  DISALLOW_COPY_AND_ASSIGN(Window);
};

}  // namespace aura

#endif  // UI_AURA_WINDOW_H_
