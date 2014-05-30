// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_NATIVE_THEME_NATIVE_THEME_GTK_H_
#define UI_NATIVE_THEME_NATIVE_THEME_GTK_H_

#include "ui/native_theme/native_theme_base.h"

namespace ui {

// GTK implementation of native theme support.
class NativeThemeGtk : public NativeThemeBase {
 public:
  static NativeThemeGtk* instance();

  virtual SkColor GetSystemColor(ColorId color_id) const OVERRIDE;

 private:
  NativeThemeGtk();
  virtual ~NativeThemeGtk();

  DISALLOW_COPY_AND_ASSIGN(NativeThemeGtk);
};

}  // namespace ui

#endif  // UI_NATIVE_THEME_NATIVE_THEME_GTK_H_
