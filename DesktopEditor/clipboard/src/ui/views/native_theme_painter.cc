// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/native_theme_painter.h"

#include "base/logging.h"
#include "ui/base/animation/animation.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/rect.h"
#include "ui/views/native_theme_delegate.h"
#include "ui/views/view.h"

namespace views {

NativeThemePainter::NativeThemePainter(NativeThemeDelegate* delegate,
                                       views::View* view)
    : delegate_(delegate),
      view_(view) {
  DCHECK(delegate_);
}

gfx::Size NativeThemePainter::GetPreferredSize() {
  const ui::NativeTheme* theme = view_->GetNativeTheme();
  ui::NativeTheme::ExtraParams extra;
  ui::NativeTheme::State state = delegate_->GetThemeState(&extra);
  return theme->GetPartSize(delegate_->GetThemePart(), state, extra);
}

void NativeThemePainter::Paint(gfx::Canvas* canvas, const gfx::Size& size) {
  const ui::NativeTheme* native_theme = view_->GetNativeTheme();
  ui::NativeTheme::Part part = delegate_->GetThemePart();
  gfx::Rect rect(size);

  if (delegate_->GetThemeAnimation() != NULL &&
      delegate_->GetThemeAnimation()->is_animating()) {
    // Paint background state.
    ui::NativeTheme::ExtraParams prev_extra;
    ui::NativeTheme::State prev_state =
        delegate_->GetBackgroundThemeState(&prev_extra);
    native_theme->Paint(canvas->sk_canvas(), part, prev_state, rect,
                        prev_extra);

    // Composite foreground state above it.
    ui::NativeTheme::ExtraParams extra;
    ui::NativeTheme::State state = delegate_->GetForegroundThemeState(&extra);
    int alpha = delegate_->GetThemeAnimation()->CurrentValueBetween(0, 255);
    canvas->SaveLayerAlpha(static_cast<uint8>(alpha));
    native_theme->Paint(canvas->sk_canvas(), part, state, rect, extra);
    canvas->Restore();
  } else {
    ui::NativeTheme::ExtraParams extra;
    ui::NativeTheme::State state = delegate_->GetThemeState(&extra);
    native_theme->Paint(canvas->sk_canvas(), part, state, rect, extra);
  }
}

}  // namespace views
