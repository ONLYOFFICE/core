// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_SIZE_CONVERSIONS_H_
#define UI_GFX_SIZE_CONVERSIONS_H_

#include "ui/gfx/size.h"
#include "ui/gfx/size_f.h"

namespace gfx {

// Returns a Size with each component from the input SizeF floored.
UI_EXPORT Size ToFlooredSize(const SizeF& size);

// Returns a Size with each component from the input SizeF ceiled.
UI_EXPORT Size ToCeiledSize(const SizeF& size);

// Returns a Size with each component from the input SizeF rounded.
UI_EXPORT Size ToRoundedSize(const SizeF& size);

}  // namespace gfx

#endif  // UI_GFX_SIZE_CONVERSIONS_H_
