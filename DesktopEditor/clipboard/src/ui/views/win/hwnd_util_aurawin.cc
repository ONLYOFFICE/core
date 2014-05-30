// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/win/hwnd_util.h"

#include "ui/aura/root_window.h"
#include "ui/views/widget/widget.h"

namespace views {

HWND HWNDForView(View* view) {
  return view->GetWidget() ? HWNDForWidget(view->GetWidget()) : NULL;
}

HWND HWNDForWidget(Widget* widget) {
  return HWNDForNativeWindow(widget->GetNativeWindow());
}

HWND HWNDForNativeView(gfx::NativeView view) {
  return view && view->GetRootWindow() ?
      view->GetRootWindow()->GetAcceleratedWidget() : NULL;
}

HWND HWNDForNativeWindow(gfx::NativeWindow window) {
  return window && window->GetRootWindow() ?
      window->GetRootWindow()->GetAcceleratedWidget() : NULL;
}

gfx::Rect GetWindowBoundsForClientBounds(View* view,
                                         const gfx::Rect& client_bounds) {
  DCHECK(view);
  aura::RootWindow* window =
      view->GetWidget()->GetNativeWindow()->GetRootWindow();
  if (window) {
    HWND hwnd = window->GetAcceleratedWidget();
    RECT rect = client_bounds.ToRECT();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    DWORD ex_style = ::GetWindowLong(hwnd, GWL_EXSTYLE);
    AdjustWindowRectEx(&rect, style, FALSE, ex_style);
    return gfx::Rect(rect);
  }
  return client_bounds;
}

}  // namespace views
