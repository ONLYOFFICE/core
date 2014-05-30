// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/gtk_native_view_id_manager.h"

#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#include "base/logging.h"
#include "base/memory/singleton.h"
#include "base/rand_util.h"
#include "ui/base/gtk/gdk_x_compat.h"
#include "ui/base/gtk/gtk_compat.h"
#include "ui/gfx/gtk_preserve_window.h"

// -----------------------------------------------------------------------------
// Bounce functions for GTK to callback into a C++ object...

void OnRealize(gfx::NativeView widget, void* arg) {
  GtkNativeViewManager* manager = reinterpret_cast<GtkNativeViewManager*>(arg);
  manager->OnRealize(widget);
}

void OnUnrealize(gfx::NativeView widget, void *arg) {
  GtkNativeViewManager* manager = reinterpret_cast<GtkNativeViewManager*>(arg);
  manager->OnUnrealize(widget);
}

static void OnDestroy(GtkObject* obj, void* arg) {
  GtkNativeViewManager* manager = reinterpret_cast<GtkNativeViewManager*>(arg);
  manager->OnDestroy(reinterpret_cast<GtkWidget*>(obj));
}

// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Public functions...

GtkNativeViewManager::GtkNativeViewManager() {
}

GtkNativeViewManager::~GtkNativeViewManager() {
}

// static
GtkNativeViewManager* GtkNativeViewManager::GetInstance() {
  return Singleton<GtkNativeViewManager>::get();
}

gfx::NativeViewId GtkNativeViewManager::GetIdForWidget(gfx::NativeView widget) {
  // This is just for unit tests:
  if (!widget)
    return 0;

  base::AutoLock locked(lock_);

  std::map<gfx::NativeView, gfx::NativeViewId>::const_iterator i =
    native_view_to_id_.find(widget);

  if (i != native_view_to_id_.end())
    return i->second;

  gfx::NativeViewId new_id =
      static_cast<gfx::NativeViewId>(base::RandUint64());
  while (id_to_info_.find(new_id) != id_to_info_.end())
    new_id = static_cast<gfx::NativeViewId>(base::RandUint64());

  NativeViewInfo info;
  info.widget = widget;
  if (gtk_widget_get_realized(widget)) {
    GdkWindow *gdk_window = gtk_widget_get_window(widget);
    DCHECK(gdk_window);
    info.x_window_id = GDK_WINDOW_XID(gdk_window);
  }

  native_view_to_id_[widget] = new_id;
  id_to_info_[new_id] = info;

  g_signal_connect(widget, "realize", G_CALLBACK(::OnRealize), this);
  g_signal_connect(widget, "unrealize", G_CALLBACK(::OnUnrealize), this);
  g_signal_connect(widget, "destroy", G_CALLBACK(::OnDestroy), this);

  return new_id;
}

bool GtkNativeViewManager::GetXIDForId(XID* output, gfx::NativeViewId id) {
  base::AutoLock locked(lock_);

  std::map<gfx::NativeViewId, NativeViewInfo>::const_iterator i =
      id_to_info_.find(id);

  if (i == id_to_info_.end())
    return false;

  *output = i->second.x_window_id;
  return true;
}

bool GtkNativeViewManager::GetNativeViewForId(gfx::NativeView* output,
                                              gfx::NativeViewId id) {
  base::AutoLock locked(lock_);

  std::map<gfx::NativeViewId, NativeViewInfo>::const_iterator i =
      id_to_info_.find(id);

  if (i == id_to_info_.end())
    return false;

  *output = i->second.widget;
  return true;
}

bool GtkNativeViewManager::GetPermanentXIDForId(XID* output,
                                                gfx::NativeViewId id) {
  base::AutoLock locked(lock_);

  std::map<gfx::NativeViewId, NativeViewInfo>::iterator i =
      id_to_info_.find(id);

  if (i == id_to_info_.end())
    return false;

  // We only return permanent XIDs for widgets that allow us to guarantee that
  // the XID will not change.
  DCHECK(GTK_IS_PRESERVE_WINDOW(i->second.widget));
  GtkPreserveWindow* widget =
      reinterpret_cast<GtkPreserveWindow*>(i->second.widget);
  gtk_preserve_window_set_preserve(widget, TRUE);

  *output = GDK_WINDOW_XID(gtk_widget_get_window(i->second.widget));

  // Update the reference count on the permanent XID.
  PermanentXIDInfo info;
  info.widget = widget;
  info.ref_count = 1;
  std::pair<std::map<XID, PermanentXIDInfo>::iterator, bool> ret =
    perm_xid_to_info_.insert(std::make_pair(*output, info));

  if (!ret.second) {
    DCHECK(ret.first->second.widget == widget);
    ret.first->second.ref_count++;
  }

  return true;
}

bool GtkNativeViewManager::AddRefPermanentXID(XID xid) {
  base::AutoLock locked(lock_);

  std::map<XID, PermanentXIDInfo>::iterator i =
    perm_xid_to_info_.find(xid);

  if (i == perm_xid_to_info_.end())
    return false;

  i->second.ref_count++;

  return true;
}

void GtkNativeViewManager::ReleasePermanentXID(XID xid) {
  base::AutoLock locked(lock_);

  std::map<XID, PermanentXIDInfo>::iterator i =
    perm_xid_to_info_.find(xid);

  if (i == perm_xid_to_info_.end())
    return;

  if (i->second.ref_count > 1) {
    i->second.ref_count--;
  } else {
    if (i->second.widget) {
      gtk_preserve_window_set_preserve(i->second.widget, FALSE);
    } else {
      GdkWindow* window = reinterpret_cast<GdkWindow*>(
          gdk_x11_window_lookup_for_display(gdk_display_get_default(), xid));
      DCHECK(window);
      gdk_window_destroy(window);
    }
    perm_xid_to_info_.erase(i);
  }
}

// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Private functions...

gfx::NativeViewId GtkNativeViewManager::GetWidgetId(gfx::NativeView widget) {
  lock_.AssertAcquired();

  std::map<gfx::NativeView, gfx::NativeViewId>::const_iterator i =
    native_view_to_id_.find(widget);

  CHECK(i != native_view_to_id_.end());
  return i->second;
}

void GtkNativeViewManager::OnRealize(gfx::NativeView widget) {
  base::AutoLock locked(lock_);

  const gfx::NativeViewId id = GetWidgetId(widget);
  std::map<gfx::NativeViewId, NativeViewInfo>::iterator i =
    id_to_info_.find(id);

  CHECK(i != id_to_info_.end());

  GdkWindow* gdk_window = gtk_widget_get_window(widget);
  CHECK(gdk_window);
  i->second.x_window_id = GDK_WINDOW_XID(gdk_window);
}

void GtkNativeViewManager::OnUnrealize(gfx::NativeView widget) {
  base::AutoLock locked(lock_);

  const gfx::NativeViewId id = GetWidgetId(widget);
  std::map<gfx::NativeViewId, NativeViewInfo>::iterator i =
    id_to_info_.find(id);

  CHECK(i != id_to_info_.end());
}

void GtkNativeViewManager::OnDestroy(gfx::NativeView widget) {
  base::AutoLock locked(lock_);

  std::map<gfx::NativeView, gfx::NativeViewId>::iterator i =
    native_view_to_id_.find(widget);
  CHECK(i != native_view_to_id_.end());

  std::map<gfx::NativeViewId, NativeViewInfo>::iterator j =
    id_to_info_.find(i->second);
  CHECK(j != id_to_info_.end());

  // If the XID is supposed to outlive the widget, mark it
  // in the lookup table.
  if (GTK_IS_PRESERVE_WINDOW(widget) &&
      gtk_preserve_window_get_preserve(
          reinterpret_cast<GtkPreserveWindow*>(widget))) {
    std::map<XID, PermanentXIDInfo>::iterator k =
        perm_xid_to_info_.find(GDK_WINDOW_XID(gtk_widget_get_window(widget)));

    if (k != perm_xid_to_info_.end())
      k->second.widget = NULL;
  }

  native_view_to_id_.erase(i);
  id_to_info_.erase(j);
}

// -----------------------------------------------------------------------------
