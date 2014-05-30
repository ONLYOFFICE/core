// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_TEXT_UTILS_H_
#define UI_GFX_TEXT_UTILS_H_

#include "base/string16.h"
#include "ui/base/ui_export.h"

namespace gfx {

// Strip the accelerator char (typically '&') from a menu string.  A double
// accelerator char ('&&') will be converted to a single char.  The out params
// |accelerated_char_pos| and |accelerated_char_span| will be set to the index
// and span of the last accelerated character, respectively, or -1 and 0 if
// there was none.
UI_EXPORT string16 RemoveAcceleratorChar(const string16& s,
                                         char16 accelerator_char,
                                         int* accelerated_char_pos,
                                         int* accelerated_char_span);

}  // namespace gfx

#endif  // UI_GFX_TEXT_UTILS_H_
