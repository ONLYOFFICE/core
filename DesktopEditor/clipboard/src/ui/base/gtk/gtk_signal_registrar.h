// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_GTK_SIGNAL_REGISTRAR_H_
#define UI_BASE_GTK_GTK_SIGNAL_REGISTRAR_H_

#include <glib.h>
#include <map>
#include <vector>

#include "base/basictypes.h"
#include "ui/base/ui_export.h"

typedef void (*GCallback) (void);
typedef struct _GObject GObject;
typedef struct _GtkWidget GtkWidget;

namespace ui {

// A class that ensures that callbacks don't run on stale owner objects. Similar
// in spirit to NotificationRegistrar. Use as follows:
//
//   class ChromeObject {
//    public:
//     ChromeObject() {
//       ...
//
//       signals_.Connect(widget, "event", CallbackThunk, this);
//     }
//
//     ...
//
//    private:
//     GtkSignalRegistrar signals_;
//   };
//
// When |signals_| goes down, it will disconnect the handlers connected via
// Connect.
class UI_EXPORT GtkSignalRegistrar {
 public:
  GtkSignalRegistrar();
  ~GtkSignalRegistrar();

  // Connect before the default handler. Returns the handler id.
  glong Connect(gpointer instance, const gchar* detailed_signal,
                GCallback signal_handler, gpointer data);
  // Connect after the default handler. Returns the handler id.
  glong ConnectAfter(gpointer instance, const gchar* detailed_signal,
                     GCallback signal_handler, gpointer data);

  // Disconnects all signal handlers connected to |instance|.
  void DisconnectAll(gpointer instance);

 private:
  typedef std::vector<glong> HandlerList;
  typedef std::map<GObject*, HandlerList> HandlerMap;

  static void WeakNotifyThunk(gpointer data, GObject* where_the_object_was) {
    reinterpret_cast<GtkSignalRegistrar*>(data)->WeakNotify(
        where_the_object_was);
  }
  void WeakNotify(GObject* where_the_object_was);

  glong ConnectInternal(gpointer instance, const gchar* detailed_signal,
                        GCallback signal_handler, gpointer data, bool after);

  HandlerMap handler_lists_;

  DISALLOW_COPY_AND_ASSIGN(GtkSignalRegistrar);
};

}  // namespace ui

#endif  // UI_BASE_GTK_GTK_SIGNAL_REGISTRAR_H_
