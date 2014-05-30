// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_COMPOSITOR_SETUP_H_
#define UI_COMPOSITOR_COMPOSITOR_SETUP_H_

#include "ui/compositor/compositor_export.h"

namespace ui {

// Configures the compositor in such a way that it doesn't render anything.
// Does nothing on platforms that aren't using the compositor.
#if !defined(USE_AURA)
// To centralize the ifdef to this file we define the function as doing nothing
// on all platforms that don't use a compositor.
static inline void SetupTestCompositor() {}
static inline void DisableTestCompositor() {}
#else
COMPOSITOR_EXPORT void SetupTestCompositor();

// Disables the test compositor so that the normal compositor is used.
COMPOSITOR_EXPORT void DisableTestCompositor();
COMPOSITOR_EXPORT bool IsTestCompositorEnabled();
#endif

}  // namespace ui

#endif  // UI_COMPOSITOR_COMPOSITOR_SETUP_H_
