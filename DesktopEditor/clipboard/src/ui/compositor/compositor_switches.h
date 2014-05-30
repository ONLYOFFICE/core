// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_COMPOSITOR_SWITCHES_H_
#define UI_COMPOSITOR_COMPOSITOR_SWITCHES_H_

#include "ui/compositor/compositor_export.h"

namespace switches {

COMPOSITOR_EXPORT extern const char kDisableTestCompositor[];
COMPOSITOR_EXPORT extern const char kUIDisableThreadedCompositing[];
COMPOSITOR_EXPORT extern const char kUIEnableSoftwareCompositing[];
COMPOSITOR_EXPORT extern const char kUIEnableThreadedCompositing[];
COMPOSITOR_EXPORT extern const char kUIMaxFramesPending[];
COMPOSITOR_EXPORT extern const char kUIShowPaintRects[];

}  // namespace switches

#endif  // UI_COMPOSITOR_COMPOSITOR_SWITCHES_H_
