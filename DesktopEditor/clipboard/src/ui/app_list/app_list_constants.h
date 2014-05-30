// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APP_LIST_CONSTANTS_H_
#define UI_APP_LIST_APP_LIST_CONSTANTS_H_

#include "third_party/skia/include/core/SkColor.h"
#include "ui/app_list/app_list_export.h"
#include "ui/base/resource/resource_bundle.h"

namespace app_list {

APP_LIST_EXPORT extern const SkColor kContentsBackgroundColor;
APP_LIST_EXPORT extern const SkColor kSelectedColor;
APP_LIST_EXPORT extern const SkColor kHoverAndPushedColor;

APP_LIST_EXPORT extern const int kPageTransitionDurationInMs;
APP_LIST_EXPORT extern const int kOverscrollPageTransitionDurationMs;

APP_LIST_EXPORT extern const int kPreferredCols;
APP_LIST_EXPORT extern const int kPreferredRows;

APP_LIST_EXPORT extern const ui::ResourceBundle::FontStyle kItemTextFontStyle;

}  // namespace app_list

#endif  // UI_APP_LIST_APP_LIST_CONSTANTS_H_
