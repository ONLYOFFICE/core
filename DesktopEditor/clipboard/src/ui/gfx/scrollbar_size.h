// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_SCROLLBAR_SIZE_H_
#define UI_GFX_SCROLLBAR_SIZE_H_

#include "ui/base/ui_export.h"

namespace gfx {

// This should return the thickness, in pixels, of a scrollbar in web content.
// This needs to match the values in WebCore's
// ScrollbarThemeChromiumXXX.cpp::scrollbarThickness().
UI_EXPORT int scrollbar_size();

}  // namespace gfx

#endif  // UI_GFX_SCROLLBAR_SIZE_H_
