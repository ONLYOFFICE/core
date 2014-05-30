// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_OAK_OAK_PRETTY_PRINT_H_
#define UI_OAK_OAK_PRETTY_PRINT_H_

#include "base/string16.h"

namespace gfx {
class Insets;
class Rect;
}

namespace oak {
namespace internal {

// Functions that return a string consisting of a prefix and the supplied value
// converted to a pretty string representation.
base::string16 PropertyWithInteger(const std::string& prefix, int value);
base::string16 PropertyWithVoidStar(const std::string& prefix, void* ptr);
base::string16 PropertyWithBool(const std::string& prefix, bool value);
base::string16 PropertyWithBounds(const std::string& prefix,
                                  const gfx::Rect& bounds);
base::string16 PropertyWithInsets(const std::string& prefix,
                                  const gfx::Insets& insets);

}  // namespace internal
}  // namespace oak

#endif  // UI_OAK_OAK_PRETTY_PRINT_H_
