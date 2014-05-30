// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gtk/gtk_floating_container.h"

#include <gtk/gtk.h>

#include <algorithm>

#include "ui/base/gtk/gtk_compat.h"

namespace {

enum {
  SET_FLOATING_POSITION,
  LAST_SIGNAL
};

enum {
  CHILD_PROP_0,
  CHILD_PROP_X,
  CHILD_PROP_Y
};

// Returns the GtkFloatingContainerChild associated with |widget| (or NULL if
// |widget| not found).
GtkFloatingContainerChild* GetChild(GtkFloatingContainer* container,
                                    GtkWidget* widget) {
  for (GList* floating_children = container->floating_children;
       floating_children; floating_children = g_list_next(floating_children)) {
    GtkFloatingContainerChild* child =
        reinterpret_cast<GtkFloatingContainerChild*>(floating_children->data);

    if (child->widget == widget)
      return child;
  }

  return NULL;
}

const GParamFlags kStaticReadWriteProp = static_cast<GParamFlags>(
    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

}  // namespace

G_BEGIN_DECLS

static void gtk_floating_container_remove(GtkContainer* container,
                                          GtkWidget* widget);
static void gtk_floating_container_forall(GtkContainer* container,
                                          gboolean include_internals,
                                          GtkCallback callback,
                                          gpointer callback_data);
static void gtk_floating_container_size_request(GtkWidget* widget,
                                                GtkRequisition* requisition);
static void gtk_floating_container_size_allocate(GtkWidget* widget,
                                                 GtkAllocation* allocation);
static void gtk_floating_container_set_child_property(GtkContainer* container,
                                                      GtkWidget* child,
                                                      guint property_id,
                                                      const GValue* value,
                                                      GParamSpec* pspec);
static void gtk_floating_container_get_child_property(GtkContainer* container,
                                                      GtkWidget* child,
                                                      guint property_id,
                                                      GValue* value,
                                                      GParamSpec* pspec);

static guint floating_container_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE(GtkFloatingContainer, gtk_floating_container, GTK_TYPE_BIN)

static void gtk_floating_container_class_init(
    GtkFloatingContainerClass *klass) {
  GtkObjectClass* object_class =
      reinterpret_cast<GtkObjectClass*>(klass);

  GtkWidgetClass* widget_class =
      reinterpret_cast<GtkWidgetClass*>(klass);
  widget_class->size_request = gtk_floating_container_size_request;
  widget_class->size_allocate = gtk_floating_container_size_allocate;

  GtkContainerClass* container_class =
      reinterpret_cast<GtkContainerClass*>(klass);
  container_class->remove = gtk_floating_container_remove;
  container_class->forall = gtk_floating_container_forall;

  container_class->set_child_property =
      gtk_floating_container_set_child_property;
  container_class->get_child_property =
      gtk_floating_container_get_child_property;

  gtk_container_class_install_child_property(
      container_class,
      CHILD_PROP_X,
      g_param_spec_int("x",
                       "X position",
                       "X position of child widget",
                       G_MININT,
                       G_MAXINT,
                       0,
                       kStaticReadWriteProp));

  gtk_container_class_install_child_property(
      container_class,
      CHILD_PROP_Y,
      g_param_spec_int("y",
                       "Y position",
                       "Y position of child widget",
                       G_MININT,
                       G_MAXINT,
                       0,
                       kStaticReadWriteProp));

  floating_container_signals[SET_FLOATING_POSITION] =
      g_signal_new("set-floating-position",
                   G_OBJECT_CLASS_TYPE(object_class),
                   static_cast<GSignalFlags>(G_SIGNAL_RUN_FIRST |
                                             G_SIGNAL_ACTION),
                   0,
                   NULL, NULL,
                   g_cclosure_marshal_VOID__BOXED,
                   G_TYPE_NONE, 1,
                   GDK_TYPE_RECTANGLE | G_SIGNAL_TYPE_STATIC_SCOPE);
}

static void gtk_floating_container_init(GtkFloatingContainer* container) {
  gtk_widget_set_has_window(GTK_WIDGET(container), FALSE);
  container->floating_children = NULL;
}

static void gtk_floating_container_remove(GtkContainer* container,
                                          GtkWidget* widget) {
  g_return_if_fail(GTK_IS_WIDGET(widget));

  GtkBin* bin = GTK_BIN(container);
  if (gtk_bin_get_child(bin) == widget) {
    ((GTK_CONTAINER_CLASS(gtk_floating_container_parent_class))->remove)
        (container, widget);
  } else {
    // Handle the other case where it's in our |floating_children| list.
    GtkFloatingContainer* floating = GTK_FLOATING_CONTAINER(container);
    GList* children = floating->floating_children;
    gboolean removed_child = false;
    while (children) {
      GtkFloatingContainerChild* child =
          reinterpret_cast<GtkFloatingContainerChild*>(children->data);

      if (child->widget == widget) {
        removed_child = true;
        gboolean was_visible = gtk_widget_get_visible(GTK_WIDGET(widget));

        gtk_widget_unparent(widget);

        floating->floating_children =
            g_list_remove_link(floating->floating_children, children);
        g_list_free(children);
        g_free(child);

        if (was_visible && gtk_widget_get_visible(GTK_WIDGET(container)))
          gtk_widget_queue_resize(GTK_WIDGET(container));

        break;
      }
      children = children->next;
    }

    g_return_if_fail(removed_child);
  }
}

static void gtk_floating_container_forall(GtkContainer* container,
                                          gboolean include_internals,
                                          GtkCallback callback,
                                          gpointer callback_data) {
  g_return_if_fail(container != NULL);
  g_return_if_fail(callback != NULL);

  // Let GtkBin do its part of the forall.
  ((GTK_CONTAINER_CLASS(gtk_floating_container_parent_class))->forall)
      (container, include_internals, callback, callback_data);

  GtkFloatingContainer* floating = GTK_FLOATING_CONTAINER(container);
  GList* children = floating->floating_children;
  while (children) {
    GtkFloatingContainerChild* child =
        reinterpret_cast<GtkFloatingContainerChild*>(children->data);
    children = children->next;

    (*callback)(child->widget, callback_data);
  }
}

static void gtk_floating_container_size_request(GtkWidget* widget,
                                                GtkRequisition* requisition) {
  GtkBin* bin = GTK_BIN(widget);
  if (bin && gtk_bin_get_child(bin)) {
    gtk_widget_size_request(gtk_bin_get_child(bin), requisition);
  } else {
    requisition->width = 0;
    requisition->height = 0;
  }
}

static void gtk_floating_container_size_allocate(GtkWidget* widget,
                                                 GtkAllocation* allocation) {
  gtk_widget_set_allocation(widget, allocation);

  if (gtk_widget_get_has_window(widget) && gtk_widget_get_realized(widget)) {
    gdk_window_move_resize(gtk_widget_get_window(widget),
                           allocation->x,
                           allocation->y,
                           allocation->width,
                           allocation->height);
  }

  // Give the same allocation to our GtkBin component.
  GtkBin* bin = GTK_BIN(widget);
  if (gtk_bin_get_child(bin)) {
    gtk_widget_size_allocate(gtk_bin_get_child(bin), allocation);
  }

  // We need to give whoever is pulling our strings a chance to set the "x" and
  // "y" properties on all of our children.
  g_signal_emit(widget, floating_container_signals[SET_FLOATING_POSITION], 0,
                allocation);

  // Our allocation has been set. We've asked our controller to place the other
  // widgets. Pass out allocations to all our children based on where they want
  // to be.
  GtkFloatingContainer* container = GTK_FLOATING_CONTAINER(widget);
  GList* children = container->floating_children;
  GtkAllocation child_allocation;
  GtkRequisition child_requisition;
  while (children) {
    GtkFloatingContainerChild* child =
        reinterpret_cast<GtkFloatingContainerChild*>(children->data);
    children = children->next;

    if (gtk_widget_get_visible(GTK_WIDGET(child->widget))) {
      gtk_widget_size_request(child->widget, &child_requisition);
      child_allocation.x = allocation->x + child->x;
      child_allocation.y = allocation->y + child->y;
      child_allocation.width = std::max(1, std::min(child_requisition.width,
                                                    allocation->width));
      child_allocation.height = std::max(1, std::min(child_requisition.height,
                                                     allocation->height));
      gtk_widget_size_allocate(child->widget, &child_allocation);
    }
  }
}

static void gtk_floating_container_set_child_property(GtkContainer* container,
                                                      GtkWidget* child,
                                                      guint property_id,
                                                      const GValue* value,
                                                      GParamSpec* pspec) {
  GtkFloatingContainerChild* floating_child =
      GetChild(GTK_FLOATING_CONTAINER(container), child);
  g_return_if_fail(floating_child);

  switch (property_id) {
    case CHILD_PROP_X:
      floating_child->x = g_value_get_int(value);
      gtk_widget_child_notify(child, "x");
      break;
    case CHILD_PROP_Y:
      floating_child->y = g_value_get_int(value);
      gtk_widget_child_notify(child, "y");
      break;
    default:
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID(
          container, property_id, pspec);
      break;
  };
}

static void gtk_floating_container_get_child_property(GtkContainer* container,
                                                      GtkWidget* child,
                                                      guint property_id,
                                                      GValue* value,
                                                      GParamSpec* pspec) {
  GtkFloatingContainerChild* floating_child =
      GetChild(GTK_FLOATING_CONTAINER(container), child);
  g_return_if_fail(floating_child);

  switch (property_id) {
    case CHILD_PROP_X:
      g_value_set_int(value, floating_child->x);
      break;
    case CHILD_PROP_Y:
      g_value_set_int(value, floating_child->y);
      break;
    default:
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID(
          container, property_id, pspec);
      break;
  };
}

GtkWidget* gtk_floating_container_new() {
  return GTK_WIDGET(g_object_new(GTK_TYPE_FLOATING_CONTAINER, NULL));
}

void gtk_floating_container_add_floating(GtkFloatingContainer* container,
                                         GtkWidget* widget) {
  g_return_if_fail(GTK_IS_FLOATING_CONTAINER(container));
  g_return_if_fail(GTK_IS_WIDGET(widget));

  GtkFloatingContainerChild* child_info = g_new(GtkFloatingContainerChild, 1);
  child_info->widget = widget;
  child_info->x = 0;
  child_info->y = 0;

  gtk_widget_set_parent(widget, GTK_WIDGET(container));

  container->floating_children =
      g_list_append(container->floating_children, child_info);
}

G_END_DECLS
