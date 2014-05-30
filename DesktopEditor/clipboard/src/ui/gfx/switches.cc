// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/switches.h"

namespace gfx {
namespace switches {

// Let text glyphs have X-positions that aren't snapped to the pixel grid in
// the browser UI.
const char kEnableBrowserTextSubpixelPositioning[] =
    "enable-browser-text-subpixel-positioning";

// Enable text glyphs to have X-positions that aren't snapped to the pixel grid
// in webkit renderers.
const char kEnableWebkitTextSubpixelPositioning[] =
    "enable-webkit-text-subpixel-positioning";

}  // namespace switches
}  // namespace gfx
