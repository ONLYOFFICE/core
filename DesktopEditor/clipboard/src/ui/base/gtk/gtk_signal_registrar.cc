// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gtk/gtk_signal_registrar.h"

#include <glib-object.h>

#include "base/logging.h"
#include "ui/base/gtk/g_object_destructor_filo.h"

namespace ui {

GtkSignalRegistrar::GtkSignalRegistrar() {
}

GtkSignalRegistrar::~GtkSignalRegistrar() {
  for (HandlerMap::iterator list_iter = handler_lists_.begin();
       list_iter != handler_lists_.end(); ++list_iter) {
    GObject* object = list_iter->first;
    GObjectDestructorFILO::GetInstance()->Disconnect(
        object, WeakNotifyThunk, this);

    HandlerList& handlers = list_iter->second;
    for (HandlerList::iterator ids_iter = handlers.begin();
         ids_iter != handlers.end(); ++ids_iter) {
      g_signal_handler_disconnect(object, *ids_iter);
    }
  }
}

glong GtkSignalRegistrar::Connect(gpointer instance,
                                  const gchar* detailed_signal,
                                  GCallback signal_handler,
                                  gpointer data) {
  return ConnectInternal(instance, detailed_signal, signal_handler, data,
                         false);
}

glong GtkSignalRegistrar::ConnectAfter(gpointer instance,
                                       const gchar* detailed_signal,
                                       GCallback signal_handler,
                                       gpointer data) {
  return ConnectInternal(instance, detailed_signal, signal_handler, data, true);
}

glong GtkSignalRegistrar::ConnectInternal(gpointer instance,
                                          const gchar* detailed_signal,
                                          GCallback signal_handler,
                                          gpointer data,
                                          bool after) {
  GObject* object = G_OBJECT(instance);

  HandlerMap::iterator iter = handler_lists_.find(object);
  if (iter == handler_lists_.end()) {
    GObjectDestructorFILO::GetInstance()->Connect(
        object, WeakNotifyThunk, this);
    handler_lists_[object] = HandlerList();
    iter = handler_lists_.find(object);
  }

  glong handler_id = after ?
      g_signal_connect_after(instance, detailed_signal, signal_handler, data) :
      g_signal_connect(instance, detailed_signal, signal_handler, data);
  iter->second.push_back(handler_id);

  return handler_id;
}

void GtkSignalRegistrar::WeakNotify(GObject* where_the_object_was) {
  HandlerMap::iterator iter = handler_lists_.find(where_the_object_was);
  if (iter == handler_lists_.end()) {
    NOTREACHED();
    return;
  }
  // The signal handlers will be disconnected automatically. Just erase the
  // handler id list.
  handler_lists_.erase(iter);
}

void GtkSignalRegistrar::DisconnectAll(gpointer instance) {
  GObject* object = G_OBJECT(instance);
  HandlerMap::iterator iter = handler_lists_.find(object);
  if (iter == handler_lists_.end())
    return;

  GObjectDestructorFILO::GetInstance()->Disconnect(
      object, WeakNotifyThunk, this);
  HandlerList& handlers = iter->second;
  for (HandlerList::iterator ids_iter = handlers.begin();
       ids_iter != handlers.end(); ++ids_iter) {
    g_signal_handler_disconnect(object, *ids_iter);
  }

  handler_lists_.erase(iter);
}

}  // namespace ui
