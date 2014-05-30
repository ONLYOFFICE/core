// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_DEFAULT_THEME_PROVIDER_H_
#define UI_VIEWS_WIDGET_DEFAULT_THEME_PROVIDER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/base/theme_provider.h"
#include "ui/views/views_export.h"

namespace ui {
class ResourceBundle;
}
using ui::ResourceBundle;

namespace views {

class VIEWS_EXPORT DefaultThemeProvider : public ui::ThemeProvider {
 public:
  DefaultThemeProvider();
  virtual ~DefaultThemeProvider();

  // Overridden from ui::ThemeProvider:
  virtual gfx::ImageSkia* GetImageSkiaNamed(int id) const OVERRIDE;
  virtual SkColor GetColor(int id) const OVERRIDE;
  virtual bool GetDisplayProperty(int id, int* result) const OVERRIDE;
  virtual bool ShouldUseNativeFrame() const OVERRIDE;
  virtual bool HasCustomImage(int id) const OVERRIDE;
  virtual base::RefCountedMemory* GetRawData(
      int id,
      ui::ScaleFactor scale_factor) const OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(DefaultThemeProvider);
};

}  // namespace views

#endif  // UI_VIEWS_WIDGET_DEFAULT_THEME_PROVIDER_H_
