// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gtk/gtk_expanded_container.h"

#include "testing/gtest/include/gtest/gtest.h"

#define EXPECT_ALLOCATION_EQ(widget, x_, y_, width_, height_) \
do { \
  GtkAllocation allocation; \
  gtk_widget_get_allocation(widget, &allocation); \
  EXPECT_EQ(x_, allocation.x); \
  EXPECT_EQ(y_, allocation.y); \
  EXPECT_EQ(width_, allocation.width); \
  EXPECT_EQ(height_, allocation.height); \
} while(0);

#define EXPECT_ALLOCATION_PARAM_EQ(widget, param, value) \
do { \
  GtkAllocation allocation; \
  gtk_widget_get_allocation(widget, &allocation); \
  EXPECT_EQ(value,allocation.param); \
} while(0);

class GtkExpandedContainerTest : public testing::Test {
 protected:
  GtkExpandedContainerTest()
      : window_(gtk_window_new(GTK_WINDOW_TOPLEVEL)),
        expanded_(gtk_expanded_container_new()) {
    gtk_window_set_default_size(GTK_WINDOW(window_), 200, 200);
    gtk_container_add(GTK_CONTAINER(window_), expanded_);
  }
  virtual ~GtkExpandedContainerTest() {
    gtk_widget_destroy(window_);
  }

  bool FindChild(GtkWidget* widget) {
    GList* children = gtk_container_get_children(GTK_CONTAINER(expanded_));
    for (GList* child = children; child; child = child->next) {
      if (GTK_WIDGET(child->data) == widget) {
        g_list_free(children);
        return true;
      }
    }
    g_list_free(children);
    return false;
  }

  int GetChildX(GtkWidget* widget) {
    GValue x = { 0 };
    g_value_init(&x, G_TYPE_INT);
    gtk_container_child_get_property(GTK_CONTAINER(expanded_), widget, "x", &x);
    return g_value_get_int(&x);
  }

  int GetChildY(GtkWidget* widget) {
    GValue y = { 0 };
    g_value_init(&y, G_TYPE_INT);
    gtk_container_child_get_property(GTK_CONTAINER(expanded_), widget, "y", &y);
    return g_value_get_int(&y);
  }

 protected:
  GtkWidget* window_;
  GtkWidget* expanded_;
};

TEST_F(GtkExpandedContainerTest, AddRemove) {
  GtkWidget* child1 = gtk_fixed_new();
  GtkWidget* child2 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(expanded_), child1);
  ASSERT_TRUE(FindChild(child1));

  gtk_container_add(GTK_CONTAINER(expanded_), child2);
  ASSERT_TRUE(FindChild(child2));
  ASSERT_TRUE(FindChild(child1));

  gtk_container_remove(GTK_CONTAINER(expanded_), child1);
  ASSERT_FALSE(FindChild(child1));
  ASSERT_TRUE(FindChild(child2));

  gtk_container_remove(GTK_CONTAINER(expanded_), child2);
  ASSERT_FALSE(FindChild(child2));
}

TEST_F(GtkExpandedContainerTest, Expand) {
  GtkWidget* child1 = gtk_fixed_new();
  GtkWidget* child2 = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(expanded_), child1);
  gtk_expanded_container_put(GTK_EXPANDED_CONTAINER(expanded_),
                             child2, 10, 20);
  gtk_widget_show_all(window_);

  GtkAllocation allocation = { 0, 0, 50, 100 };
  gtk_widget_size_allocate(expanded_, &allocation);

  EXPECT_ALLOCATION_EQ(child1, 0, 0, 50, 100);

  EXPECT_ALLOCATION_EQ(child2, 10, 20, 50, 100);

  allocation.x = 10;
  allocation.y = 20;
  gtk_widget_size_allocate(expanded_, &allocation);

  EXPECT_ALLOCATION_PARAM_EQ(child1, x, 10);
  EXPECT_ALLOCATION_PARAM_EQ(child1, y, 20);
  EXPECT_ALLOCATION_PARAM_EQ(child2, x, 20);
  EXPECT_ALLOCATION_PARAM_EQ(child2, y, 40);
}

// Test if the size allocation for children still works when using own
// GdkWindow. In this case, the children's origin starts from (0, 0) rather
// than the container's origin.
TEST_F(GtkExpandedContainerTest, HasWindow) {
  GtkWidget* child = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(expanded_), child);
  gtk_expanded_container_set_has_window(GTK_EXPANDED_CONTAINER(expanded_),
                                        TRUE);
  gtk_widget_show_all(window_);

  GtkAllocation allocation = { 10, 10, 50, 100 };
  gtk_widget_size_allocate(expanded_, &allocation);

  EXPECT_ALLOCATION_EQ(child, 0, 0, 50, 100);
}

static void OnChildSizeRequest(GtkExpandedContainer* container,
                               GtkWidget* child,
                               GtkRequisition* requisition,
                               gpointer userdata) {
  ASSERT_EQ(child, GTK_WIDGET(userdata));
  requisition->width = 250;
  requisition->height = -1;
}

TEST_F(GtkExpandedContainerTest, ChildSizeRequest) {
  GtkWidget* child = gtk_fixed_new();
  gtk_widget_set_size_request(child, 10, 25);
  g_signal_connect(expanded_, "child-size-request",
                   G_CALLBACK(OnChildSizeRequest), child);
  gtk_container_add(GTK_CONTAINER(expanded_), child);
  gtk_widget_show_all(window_);

  GtkAllocation allocation = { 0, 0, 300, 100 };
  gtk_widget_size_allocate(expanded_, &allocation);

  EXPECT_ALLOCATION_EQ(child, 0, 0, 250, 25);
}

TEST_F(GtkExpandedContainerTest, ChildPosition) {
  GtkWidget* child = gtk_fixed_new();
  gtk_expanded_container_put(GTK_EXPANDED_CONTAINER(expanded_),
                             child, 10, 20);
  gtk_widget_show_all(window_);

  EXPECT_EQ(10, GetChildX(child));
  EXPECT_EQ(20, GetChildY(child));

  gtk_expanded_container_move(GTK_EXPANDED_CONTAINER(expanded_),
                              child, 40, 50);
  EXPECT_EQ(40, GetChildX(child));
  EXPECT_EQ(50, GetChildY(child));
}
