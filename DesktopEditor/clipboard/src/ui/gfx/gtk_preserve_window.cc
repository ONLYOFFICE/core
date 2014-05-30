// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/gtk_preserve_window.h"

#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "ui/base/gtk/gtk_compat.h"

G_BEGIN_DECLS

#define GTK_PRESERVE_WINDOW_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), \
                                            GTK_TYPE_PRESERVE_WINDOW, \
                                            GtkPreserveWindowPrivate))

typedef struct _GtkPreserveWindowPrivate GtkPreserveWindowPrivate;

struct _GtkPreserveWindowPrivate {
  // If true, don't create/destroy windows on realize/unrealize.
  gboolean preserve_window;

  // Whether or not we delegate the resize of the GdkWindow
  // to someone else.
  gboolean delegate_resize;

  // Accessible factory and userdata.
  AtkObject* (*accessible_factory)(void* userdata);
  void* accessible_factory_userdata;
};

G_DEFINE_TYPE(GtkPreserveWindow, gtk_preserve_window, GTK_TYPE_FIXED)

static void gtk_preserve_window_destroy(GtkObject* object);
static void gtk_preserve_window_realize(GtkWidget* widget);
static void gtk_preserve_window_unrealize(GtkWidget* widget);
static void gtk_preserve_window_size_allocate(GtkWidget* widget,
                                              GtkAllocation* allocation);
static AtkObject* gtk_preserve_window_get_accessible(GtkWidget* widget);

static void gtk_preserve_window_class_init(GtkPreserveWindowClass *klass) {
  GtkWidgetClass* widget_class = reinterpret_cast<GtkWidgetClass*>(klass);
  widget_class->realize = gtk_preserve_window_realize;
  widget_class->unrealize = gtk_preserve_window_unrealize;
  widget_class->size_allocate = gtk_preserve_window_size_allocate;
  widget_class->get_accessible = gtk_preserve_window_get_accessible;

  GtkObjectClass* object_class = reinterpret_cast<GtkObjectClass*>(klass);
  object_class->destroy = gtk_preserve_window_destroy;

  GObjectClass* gobject_class = G_OBJECT_CLASS(klass);
  g_type_class_add_private(gobject_class, sizeof(GtkPreserveWindowPrivate));
}

static void gtk_preserve_window_init(GtkPreserveWindow* widget) {
  GtkPreserveWindowPrivate* priv = GTK_PRESERVE_WINDOW_GET_PRIVATE(widget);
  priv->preserve_window = FALSE;
  priv->accessible_factory = NULL;
  priv->accessible_factory_userdata = NULL;

  // These widgets always have their own window.
  gtk_widget_set_has_window(GTK_WIDGET(widget), TRUE);
}

GtkWidget* gtk_preserve_window_new() {
  return GTK_WIDGET(g_object_new(GTK_TYPE_PRESERVE_WINDOW, NULL));
}

static void gtk_preserve_window_destroy(GtkObject* object) {
  GtkWidget* widget = reinterpret_cast<GtkWidget*>(object);
  GtkPreserveWindowPrivate* priv = GTK_PRESERVE_WINDOW_GET_PRIVATE(widget);

  GdkWindow* gdk_window = gtk_widget_get_window(widget);
  if (gdk_window) {
    gdk_window_set_user_data(gdk_window, NULL);
    // If the window is preserved, someone else must destroy it.
    if (!priv->preserve_window)
      gdk_window_destroy(gdk_window);
    gtk_widget_set_window(widget, NULL);
  }

  GTK_OBJECT_CLASS(gtk_preserve_window_parent_class)->destroy(object);
}

static void gtk_preserve_window_realize(GtkWidget* widget) {
  g_return_if_fail(GTK_IS_PRESERVE_WINDOW(widget));

  GdkWindow* gdk_window = gtk_widget_get_window(widget);
  if (gdk_window) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);

    gdk_window_reparent(gdk_window,
                        gtk_widget_get_parent_window(widget),
                        allocation.x,
                        allocation.y);
    GtkPreserveWindowPrivate* priv = GTK_PRESERVE_WINDOW_GET_PRIVATE(widget);
    if (!priv->delegate_resize) {
      gdk_window_resize(gdk_window,
                        allocation.width,
                        allocation.height);
    }
    gint event_mask = gtk_widget_get_events(widget);
    event_mask |= GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK;
    gdk_window_set_events(gdk_window, (GdkEventMask) event_mask);
    gdk_window_set_user_data(gdk_window, widget);

    gtk_widget_set_realized(widget, TRUE);

    gtk_widget_style_attach(widget);
    gtk_style_set_background(gtk_widget_get_style(widget),
                             gdk_window, GTK_STATE_NORMAL);
  } else {
    GTK_WIDGET_CLASS(gtk_preserve_window_parent_class)->realize(widget);
  }
}

static void gtk_preserve_window_unrealize(GtkWidget* widget) {
  g_return_if_fail(GTK_IS_PRESERVE_WINDOW(widget));

  GtkPreserveWindowPrivate* priv = GTK_PRESERVE_WINDOW_GET_PRIVATE(widget);
  if (priv->preserve_window) {
    GtkWidgetClass* widget_class =
        GTK_WIDGET_CLASS(gtk_preserve_window_parent_class);
    GtkContainerClass* container_class =
        GTK_CONTAINER_CLASS(gtk_preserve_window_parent_class);

    if (gtk_widget_get_mapped(widget)) {
      widget_class->unmap(widget);

      gtk_widget_set_mapped(widget, FALSE);
    }

    // This is the behavior from GtkWidget, inherited by GtkFixed.
    // It is unclear why we should not call the potentially overridden
    // unrealize method (via the callback), but doing so causes errors.
    container_class->forall(
        GTK_CONTAINER(widget), FALSE,
        reinterpret_cast<GtkCallback>(gtk_widget_unrealize), NULL);

    GdkWindow* gdk_window = gtk_widget_get_window(widget);

    // TODO(erg): Almost all style handling will need to be overhauled in GTK3.
    gtk_style_detach(gtk_widget_get_style(widget));
    gdk_window_reparent(gdk_window, gdk_get_default_root_window(), 0, 0);
    gtk_selection_remove_all(widget);
    gdk_window_set_user_data(gdk_window, NULL);

    gtk_widget_set_realized(widget, FALSE);
  } else {
    GTK_WIDGET_CLASS(gtk_preserve_window_parent_class)->unrealize(widget);
  }
}

gboolean gtk_preserve_window_get_preserve(GtkPreserveWindow* window) {
  g_return_val_if_fail(GTK_IS_PRESERVE_WINDOW(window), FALSE);
  GtkPreserveWindowPrivate* priv = GTK_PRESERVE_WINDOW_GET_PRIVATE(window);

  return priv->preserve_window;
}

void gtk_preserve_window_set_preserve(GtkPreserveWindow* window,
                                      gboolean value) {
  g_return_if_fail(GTK_IS_PRESERVE_WINDOW(window));
  GtkPreserveWindowPrivate* priv = GTK_PRESERVE_WINDOW_GET_PRIVATE(window);
  priv->preserve_window = value;

  GtkWidget* widget = GTK_WIDGET(window);
  GdkWindow* gdk_window = gtk_widget_get_window(widget);
  if (value && !gdk_window) {
    GdkWindowAttr attributes;
    gint attributes_mask;

    // We may not know the width and height, so we rely on the fact
    // that a size-allocation will resize it later.
    attributes.width = 1;
    attributes.height = 1;

    attributes.window_type = GDK_WINDOW_CHILD;
    attributes.wclass = GDK_INPUT_OUTPUT;
    attributes.override_redirect = TRUE;

    attributes.visual = gtk_widget_get_visual(widget);
    attributes.colormap = gtk_widget_get_colormap(widget);

    attributes.event_mask = gtk_widget_get_events(widget);
    attributes.event_mask |= GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK;

    attributes_mask = GDK_WA_VISUAL | GDK_WA_COLORMAP | GDK_WA_NOREDIR;
    gdk_window = gdk_window_new(
        gdk_get_default_root_window(), &attributes, attributes_mask);
    gtk_widget_set_window(widget, gdk_window);
  } else if (!value && gdk_window && !gtk_widget_get_realized(widget)) {
    gdk_window_destroy(gdk_window);
    gtk_widget_set_window(widget, NULL);
  }
}

void gtk_preserve_window_size_allocate(GtkWidget* widget,
                                       GtkAllocation* allocation) {
  g_return_if_fail(GTK_IS_PRESERVE_WINDOW(widget));

  gtk_widget_set_allocation(widget, allocation);

  if (gtk_widget_get_realized(widget)) {
    GtkPreserveWindowPrivate* priv = GTK_PRESERVE_WINDOW_GET_PRIVATE(widget);
    GdkWindow* gdk_window = gtk_widget_get_window(widget);
    if (priv->delegate_resize) {
      gdk_window_move(gdk_window, allocation->x, allocation->y);
    } else {
      gdk_window_move_resize(
          gdk_window, allocation->x, allocation->y,
          allocation->width, allocation->height);
    }
  }

  // Propagate resize to children
  guint16 border_width = gtk_container_get_border_width(GTK_CONTAINER(widget));
  GList *children = GTK_FIXED(widget)->children;
  while (children) {
    GtkFixedChild *child = reinterpret_cast<GtkFixedChild*>(children->data);
    if (gtk_widget_get_visible(child->widget)) {
      GtkRequisition child_requisition;
      gtk_widget_get_child_requisition(child->widget, &child_requisition);

      GtkAllocation child_allocation;
      child_allocation.x = child->x + border_width;
      child_allocation.y = child->y + border_width;
      child_allocation.width = child_requisition.width;
      child_allocation.height = child_requisition.height;

      gtk_widget_size_allocate(child->widget, &child_allocation);
    }
    children = children->next;
  }
}

void gtk_preserve_window_delegate_resize(GtkPreserveWindow* widget,
                                         gboolean delegate) {
  GtkPreserveWindowPrivate* priv = GTK_PRESERVE_WINDOW_GET_PRIVATE(widget);
  priv->delegate_resize = delegate;
}

void gtk_preserve_window_set_accessible_factory(
    GtkPreserveWindow* widget,
    AtkObject* (*factory)(void* userdata),
    gpointer userdata) {
  GtkPreserveWindowPrivate* priv = GTK_PRESERVE_WINDOW_GET_PRIVATE(widget);
  priv->accessible_factory = factory;
  priv->accessible_factory_userdata = userdata;
}

AtkObject* gtk_preserve_window_get_accessible(GtkWidget* widget) {
  GtkPreserveWindowPrivate* priv = GTK_PRESERVE_WINDOW_GET_PRIVATE(widget);
  if (priv->accessible_factory) {
    return priv->accessible_factory(priv->accessible_factory_userdata);
  } else {
    return GTK_WIDGET_CLASS(gtk_preserve_window_parent_class)
        ->get_accessible(widget);
  }
}

G_END_DECLS
