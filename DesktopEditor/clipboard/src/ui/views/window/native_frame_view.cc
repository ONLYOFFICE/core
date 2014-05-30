// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/window/native_frame_view.h"

#include "ui/views/widget/native_widget.h"
#include "ui/views/widget/widget.h"

#if defined(OS_WIN)
#include "ui/views/win/hwnd_util.h"
#endif

namespace views {

////////////////////////////////////////////////////////////////////////////////
// NativeFrameView, public:

NativeFrameView::NativeFrameView(Widget* frame)
    : NonClientFrameView(),
      frame_(frame) {
}

NativeFrameView::~NativeFrameView() {
}

////////////////////////////////////////////////////////////////////////////////
// NativeFrameView, NonClientFrameView overrides:

gfx::Rect NativeFrameView::GetBoundsForClientView() const {
  return gfx::Rect(0, 0, width(), height());
}

gfx::Rect NativeFrameView::GetWindowBoundsForClientBounds(
    const gfx::Rect& client_bounds) const {
#if defined(OS_WIN)
  return views::GetWindowBoundsForClientBounds(
      static_cast<View*>(const_cast<NativeFrameView*>(this)), client_bounds);
#else
  // TODO(sad):
  return client_bounds;
#endif
}

int NativeFrameView::NonClientHitTest(const gfx::Point& point) {
  return frame_->client_view()->NonClientHitTest(point);
}

void NativeFrameView::GetWindowMask(const gfx::Size& size,
                                    gfx::Path* window_mask) {
  // Nothing to do, we use the default window mask.
}

void NativeFrameView::ResetWindowControls() {
  // Nothing to do.
}

void NativeFrameView::UpdateWindowIcon() {
  // Nothing to do.
}

void NativeFrameView::UpdateWindowTitle() {
  // Nothing to do.
}

gfx::Size NativeFrameView::GetPreferredSize() {
  return frame_->client_view()->GetPreferredSize();
}

}  // namespace views
