// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gtk/gtk_expanded_container.h"

#include <gtk/gtk.h>

#include <algorithm>

#include "ui/base/gtk/gtk_compat.h"

namespace {

enum {
  CHILD_SIZE_REQUEST,
  LAST_SIGNAL
};

guint expanded_container_signals[LAST_SIGNAL] = { 0 };

struct SizeAllocateData {
  GtkWidget* container;
  GtkAllocation* allocation;
  int border_width;
};

void GetChildPosition(GtkWidget* container, GtkWidget* child, int* x, int* y) {
  GValue v = { 0 };
  g_value_init(&v, G_TYPE_INT);
  gtk_container_child_get_property(GTK_CONTAINER(container), child, "x", &v);
  *x = g_value_get_int(&v);
  gtk_container_child_get_property(GTK_CONTAINER(container), child, "y", &v);
  *y = g_value_get_int(&v);
  g_value_unset(&v);
}

void ChildSizeAllocate(GtkWidget* child, gpointer userdata) {
  if (!gtk_widget_get_visible(child))
    return;

  SizeAllocateData* data = reinterpret_cast<SizeAllocateData*>(userdata);

  GtkRequisition child_requisition;
  child_requisition.width = data->allocation->width - data->border_width * 2;
  child_requisition.height = data->allocation->height - data->border_width * 2;

  // We need to give whoever is pulling our strings a chance to adjust the
  // size of our children.
  g_signal_emit(data->container,
                expanded_container_signals[CHILD_SIZE_REQUEST], 0,
                child, &child_requisition);

  GtkAllocation child_allocation;
  child_allocation.width = child_requisition.width;
  child_allocation.height = child_requisition.height;
  if (child_allocation.width < 0 || child_allocation.height < 0) {
    gtk_widget_get_child_requisition(child, &child_requisition);
    if (child_allocation.width < 0)
      child_allocation.width = child_requisition.width;
    if (child_allocation.height < 0)
      child_allocation.height = child_requisition.height;
  }

  int x, y;
  GetChildPosition(data->container, child, &x, &y);

  child_allocation.x = x + data->border_width;
  child_allocation.y = y + data->border_width;

  if (!gtk_widget_get_has_window(data->container)) {
    child_allocation.x += data->allocation->x;
    child_allocation.y += data->allocation->y;
  }
  gtk_widget_size_allocate(child, &child_allocation);
}

void Marshal_VOID__OBJECT_BOXED(GClosure* closure,
                                GValue* return_value G_GNUC_UNUSED,
                                guint n_param_values,
                                const GValue* param_values,
                                gpointer invocation_hint G_GNUC_UNUSED,
                                gpointer marshal_data) {
  typedef void (*GMarshalFunc_VOID__OBJECT_BOXED) (gpointer data1,
                                                   gpointer arg_1,
                                                   gpointer arg_2,
                                                   gpointer data2);
  register GMarshalFunc_VOID__OBJECT_BOXED callback;
  register GCClosure *cc = reinterpret_cast<GCClosure*>(closure);
  register gpointer data1, data2;

  g_return_if_fail(n_param_values == 3);

  if (G_CCLOSURE_SWAP_DATA(closure)) {
    data1 = closure->data;
    data2 = g_value_peek_pointer(param_values + 0);
  } else {
    data1 = g_value_peek_pointer(param_values + 0);
    data2 = closure->data;
  }

  callback = reinterpret_cast<GMarshalFunc_VOID__OBJECT_BOXED>(
      marshal_data ? marshal_data : cc->callback);

  callback(data1,
           g_value_get_object(param_values + 1),
           g_value_get_boxed(param_values + 2),
           data2);
}

}  // namespace

G_BEGIN_DECLS

static void gtk_expanded_container_size_allocate(GtkWidget* widget,
                                                 GtkAllocation* allocation);

G_DEFINE_TYPE(GtkExpandedContainer, gtk_expanded_container, GTK_TYPE_FIXED)

static void gtk_expanded_container_class_init(
    GtkExpandedContainerClass *klass) {
  GtkObjectClass* object_class =
      reinterpret_cast<GtkObjectClass*>(klass);

  GtkWidgetClass* widget_class =
      reinterpret_cast<GtkWidgetClass*>(klass);
  widget_class->size_allocate = gtk_expanded_container_size_allocate;

  expanded_container_signals[CHILD_SIZE_REQUEST] =
      g_signal_new("child-size-request",
                   G_OBJECT_CLASS_TYPE(object_class),
                   static_cast<GSignalFlags>(G_SIGNAL_RUN_FIRST),
                   0,
                   NULL, NULL,
                   Marshal_VOID__OBJECT_BOXED,
                   G_TYPE_NONE, 2,
                   GTK_TYPE_WIDGET,
                   GTK_TYPE_REQUISITION | G_SIGNAL_TYPE_STATIC_SCOPE);
}

static void gtk_expanded_container_init(GtkExpandedContainer* container) {
}

static void gtk_expanded_container_size_allocate(GtkWidget* widget,
                                                 GtkAllocation* allocation) {
  gtk_widget_set_allocation(widget, allocation);

  if (gtk_widget_get_has_window(widget) && gtk_widget_get_realized(widget)) {
    gdk_window_move_resize(gtk_widget_get_window(widget),
                           allocation->x,
                           allocation->y,
                           allocation->width,
                           allocation->height);
  }

  SizeAllocateData data;
  data.container = widget;
  data.allocation = allocation;
  data.border_width = gtk_container_get_border_width(GTK_CONTAINER(widget));

  gtk_container_foreach(GTK_CONTAINER(widget), ChildSizeAllocate, &data);
}

GtkWidget* gtk_expanded_container_new() {
  return GTK_WIDGET(g_object_new(GTK_TYPE_EXPANDED_CONTAINER, NULL));
}

void gtk_expanded_container_put(GtkExpandedContainer* container,
                                GtkWidget* widget, gint x, gint y) {
  g_return_if_fail(GTK_IS_EXPANDED_CONTAINER(container));
  g_return_if_fail(GTK_IS_WIDGET(widget));
  gtk_fixed_put(GTK_FIXED(container), widget, x, y);
}

void gtk_expanded_container_move(GtkExpandedContainer* container,
                                 GtkWidget* widget, gint x, gint y) {
  g_return_if_fail(GTK_IS_EXPANDED_CONTAINER(container));
  g_return_if_fail(GTK_IS_WIDGET(widget));
  gtk_fixed_move(GTK_FIXED(container), widget, x, y);
}

void gtk_expanded_container_set_has_window(GtkExpandedContainer* container,
                                           gboolean has_window) {
  g_return_if_fail(GTK_IS_EXPANDED_CONTAINER(container));
  g_return_if_fail(!gtk_widget_get_realized(GTK_WIDGET(container)));
  gtk_widget_set_has_window(GTK_WIDGET(container), has_window);
}

gboolean gtk_expanded_container_get_has_window(
    GtkExpandedContainer* container) {
  g_return_val_if_fail(GTK_IS_EXPANDED_CONTAINER(container), FALSE);
  return gtk_widget_get_has_window(GTK_WIDGET(container));
}

G_END_DECLS
