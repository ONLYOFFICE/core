// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_SCOPED_GOBJECT_H_
#define UI_BASE_GTK_SCOPED_GOBJECT_H_

#include <glib-object.h>

#include "base/memory/scoped_ptr.h"

namespace ui {

// It's not legal C++ to have a templatized typedefs, so we wrap it in a
// struct.  When using this, you need to include ::Type.  E.g.,
// ScopedGObject<GdkPixbufLoader>::Type loader(gdk_pixbuf_loader_new());
template<class T>
struct ScopedGObject {
  // A helper class that will g_object_unref |p| when it goes out of scope.
  // This never adds a ref, it only unrefs.
  template<class U>
  struct GObjectUnrefer {
    void operator()(U* ptr) const {
      if (ptr)
        g_object_unref(ptr);
    }
  };

  typedef scoped_ptr_malloc<T, GObjectUnrefer<T> > Type;
};

}  // namespace ui

#endif  // UI_BASE_GTK_SCOPED_GOBJECT_H_
