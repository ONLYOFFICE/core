// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_NATIVE_THEME_NATIVE_THEME_AURA_H_
#define UI_NATIVE_THEME_NATIVE_THEME_AURA_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/native_theme/native_theme_base.h"

namespace ui {

// Aura implementation of native theme support.
class NATIVE_THEME_EXPORT NativeThemeAura : public NativeThemeBase {
 public:
  static NativeThemeAura* instance();

 private:
  NativeThemeAura();
  virtual ~NativeThemeAura();

  // Overridden from NativeThemeBase:
  virtual SkColor GetSystemColor(ColorId color_id) const OVERRIDE;
  virtual void PaintMenuPopupBackground(
      SkCanvas* canvas,
      const gfx::Size& size,
      const MenuBackgroundExtraParams& menu_background) const OVERRIDE;
  virtual void PaintScrollbarTrack(
      SkCanvas* canvas,
      Part part,
      State state,
      const ScrollbarTrackExtraParams& extra_params,
      const gfx::Rect& rect) const OVERRIDE;
  virtual void PaintScrollbarThumb(SkCanvas* canvas,
                                   Part part,
                                   State state,
                                   const gfx::Rect& rect) const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(NativeThemeAura);
};

}  // namespace ui

#endif  // UI_NATIVE_THEME_NATIVE_THEME_AURA_H_
