// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/oak/oak_pretty_print.h"

#include "base/string_util.h"
#include "base/utf_string_conversions.h"
#include "base/string_number_conversions.h"
#include "base/stringprintf.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/rect.h"

namespace oak {
namespace internal {

base::string16 PropertyWithInteger(const std::string& prefix, int value) {
  return ASCIIToUTF16(prefix) + base::IntToString16(value);
}

base::string16 PropertyWithVoidStar(const std::string& prefix, void* ptr) {
  unsigned int cast_ptr =
      static_cast<unsigned int>(reinterpret_cast<intptr_t>(ptr));
  return ASCIIToUTF16(
      prefix + "0x" + (ptr ? base::StringPrintf("%x", cast_ptr) : "0"));
}

base::string16 PropertyWithBool(const std::string& prefix, bool value) {
  return ASCIIToUTF16(prefix + (value ? "true" : "false"));
}

base::string16 PropertyWithBounds(const std::string& prefix,
                                  const gfx::Rect& bounds) {
  return ASCIIToUTF16(prefix + bounds.ToString());
}

base::string16 PropertyWithInsets(const std::string& prefix,
                                  const gfx::Insets& insets) {
  return ASCIIToUTF16(prefix + insets.ToString());
}

}  // namespace internal
}  // namespace oak
