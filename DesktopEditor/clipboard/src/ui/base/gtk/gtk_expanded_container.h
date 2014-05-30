// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_GTK_EXPANDED_CONTAINER_H_
#define UI_BASE_GTK_GTK_EXPANDED_CONTAINER_H_

#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "ui/base/ui_export.h"

// A specialized container derived from GtkFixed, which expands the size of its
// children to fill the container, in one or both directions. The usage of this
// container is similar to GtkFixed.
//
// The "child-size-request" signal is optional, if you want to expand child
// widgets to customized size other than the container's size. It should have
// the following signature:
//
//   void (*child_size_request)(GtkExpandedContainer* container,
//                              GtkWidget* child,
//                              GtkRequisition* requisition);
//
// This signal is emitted for each child with the requisition set to the size of
// the container. Your handler may adjust the value of the requisition. If the
// width or height is set to -1, then that direction will not be expanded, and
// the original size request of the child will be used.

G_BEGIN_DECLS

#define GTK_TYPE_EXPANDED_CONTAINER                                 \
    (gtk_expanded_container_get_type())
#define GTK_EXPANDED_CONTAINER(obj)                                 \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), GTK_TYPE_EXPANDED_CONTAINER, \
                                GtkExpandedContainer))
#define GTK_EXPANDED_CONTAINER_CLASS(klass)                         \
    (G_TYPE_CHECK_CLASS_CAST((klass), GTK_TYPE_EXPANDED_CONTAINER,  \
                             GtkExpandedContainerClass))
#define GTK_IS_EXPANDED_CONTAINER(obj)                              \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj), GTK_TYPE_EXPANDED_CONTAINER))
#define GTK_IS_EXPANDED_CONTAINER_CLASS(klass)                      \
    (G_TYPE_CHECK_CLASS_TYPE((klass), GTK_TYPE_EXPANDED_CONTAINER))
#define GTK_EXPANDED_CONTAINER_GET_CLASS(obj)                       \
    (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_EXPANDED_CONTAINER,  \
                               GtkExpandedContainerClass))

typedef struct _GtkExpandedContainer GtkExpandedContainer;
typedef struct _GtkExpandedContainerClass GtkExpandedContainerClass;

struct _GtkExpandedContainer {
  // Parent class.
  GtkFixed fixed;
};

struct _GtkExpandedContainerClass {
  GtkFixedClass parent_class;
};

UI_EXPORT GType gtk_expanded_container_get_type() G_GNUC_CONST;
UI_EXPORT GtkWidget* gtk_expanded_container_new();
UI_EXPORT void gtk_expanded_container_put(GtkExpandedContainer* container,
                                          GtkWidget* widget, gint x, gint y);
UI_EXPORT void gtk_expanded_container_move(GtkExpandedContainer* container,
                                           GtkWidget* widget, gint x, gint y);
UI_EXPORT void gtk_expanded_container_set_has_window(
    GtkExpandedContainer* container,
    gboolean has_window);
UI_EXPORT gboolean gtk_expanded_container_get_has_window(
    GtkExpandedContainer* container);

G_END_DECLS

#endif  // UI_BASE_GTK_GTK_EXPANDED_CONTAINER_H_
