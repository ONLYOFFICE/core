// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_COLOR_PROFILE_H_
#define UI_GFX_COLOR_PROFILE_H_

#include <vector>

#include "base/basictypes.h"

#include "ui/base/ui_export.h"

namespace gfx {

static const size_t kMinProfileLength = 128;
static const size_t kMaxProfileLength = 4 * 1024 * 1024;

class UI_EXPORT ColorProfile {
 public:
  // On Windows, this reads a file from disk so it shouldn't be run on the UI
  // or IO thread.
  ColorProfile();
  ~ColorProfile();

  const std::vector<char>& profile() const { return profile_; }

 private:
  std::vector<char> profile_;

  DISALLOW_COPY_AND_ASSIGN(ColorProfile);
};

// Loads the monitor color space if available.
UI_EXPORT void GetColorProfile(std::vector<char>* profile);

}  // namespace gfx

#endif  // UI_GFX_COLOR_PROFILE_H_

