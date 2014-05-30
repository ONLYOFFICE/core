// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_GTK_NATIVE_VIEW_ID_MANAGER_H_
#define UI_GFX_GTK_NATIVE_VIEW_ID_MANAGER_H_

#include <map>

#include "base/synchronization/lock.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/native_widget_types.h"

template <typename T> struct DefaultSingletonTraits;

typedef unsigned long XID;
struct _GtkPreserveWindow;

// NativeViewIds are the opaque values which the renderer holds as a reference
// to a window.
//
// We could make NativeViewIds be the X id of the window. However, at the
// time when we need to tell the renderer about its NativeViewId, an XID isn't
// availible and it goes very much against the grain of the code to make it so.
// Also, we worry that GTK might choose to change the underlying X window id
// when, say, the widget is hidden or repacked. Finally, if we used XIDs then a
// compromised renderer could start asking questions about any X windows on the
// system.
//
// Thus, we have this object. It produces random NativeViewIds from GtkWidget
// pointers and observes the various signals from the widget for when an X
// window is created, destroyed etc. Thus it provides a thread safe mapping
// from NativeViewIds to the current XID for that widget.
class UI_EXPORT GtkNativeViewManager {
 public:
  // Returns the singleton instance.
  static GtkNativeViewManager* GetInstance();

  // Must be called from the UI thread:
  //
  // Return a NativeViewId for the given widget and attach to the various
  // signals emitted by that widget. The NativeViewId is pseudo-randomly
  // allocated so that a compromised renderer trying to guess values will fail
  // with high probability. The NativeViewId will not be reused for the
  // lifetime of the GtkWidget.
  gfx::NativeViewId GetIdForWidget(gfx::NativeView widget);

  // May be called from any thread:
  //
  // xid: (output) the resulting X window ID, or 0
  // id: a value previously returned from GetIdForWidget
  // returns: true if |id| is a valid id, false otherwise.
  //
  // If the widget referenced by |id| does not current have an X window id,
  // |*xid| is set to 0.
  bool GetXIDForId(XID* xid, gfx::NativeViewId id);

  // May be called from the UI thread:
  //
  // Same as GetXIDForId except it returns the NativeView (GtkWidget*).
  bool GetNativeViewForId(gfx::NativeView* xid, gfx::NativeViewId id);

  // Must be called from the UI thread because we may need the associated
  // widget to create a window.
  //
  // Keeping the XID permanent requires a bit of overhead, so it must
  // be explicitly requested.
  //
  // xid: (output) the resulting X window
  // id: a value previously returned from GetIdForWidget
  // returns: true if |id| is a valid id, false otherwise.
  bool GetPermanentXIDForId(XID* xid, gfx::NativeViewId id);

  // Can be called from any thread.
  // Will return false if the given XID isn't permanent or has already been
  // released.
  bool AddRefPermanentXID(XID xid);

  // Must be called from the UI thread because we may need to access a
  // GtkWidget or destroy a GdkWindow.
  //
  // If the widget associated with the XID is still alive, allow the widget
  // to destroy the associated XID when it wants. Otherwise, destroy the
  // GdkWindow associated with the XID.
  void ReleasePermanentXID(XID xid);

  // These are actually private functions, but need to be called from statics.
  void OnRealize(gfx::NativeView widget);
  void OnUnrealize(gfx::NativeView widget);
  void OnDestroy(gfx::NativeView widget);

 private:
  // This object is a singleton:
  GtkNativeViewManager();
  ~GtkNativeViewManager();
  friend struct DefaultSingletonTraits<GtkNativeViewManager>;

  struct NativeViewInfo {
    NativeViewInfo() : widget(NULL), x_window_id(0) {
    }
    gfx::NativeView widget;
    XID x_window_id;
  };

  gfx::NativeViewId GetWidgetId(gfx::NativeView id);

  // protects native_view_to_id_ and id_to_info_
  base::Lock lock_;

  // If asked for an id for the same widget twice, we want to return the same
  // id. So this records the current mapping.
  std::map<gfx::NativeView, gfx::NativeViewId> native_view_to_id_;
  std::map<gfx::NativeViewId, NativeViewInfo> id_to_info_;

  struct PermanentXIDInfo {
    PermanentXIDInfo() : widget(NULL), ref_count(0) {
    }
    _GtkPreserveWindow* widget;
    int ref_count;
  };

  // Used to maintain the reference count for permanent XIDs
  // (referenced by GetPermanentXIDForId and dereferenced by
  // ReleasePermanentXID). Only those XIDs with a positive reference count
  // will be in the table.
  //
  // In general, several GTK widgets may share the same X window. We assume
  // that is not true of the widgets stored in this registry.
  //
  // An XID will map to NULL, if there is an outstanding reference but the
  // widget was destroyed. In this case, the destruction of the X window
  // is deferred to the dropping of all references.
  std::map<XID, PermanentXIDInfo> perm_xid_to_info_;

  DISALLOW_COPY_AND_ASSIGN(GtkNativeViewManager);
};

#endif  // UI_GFX_GTK_NATIVE_VIEW_ID_MANAGER_H_
