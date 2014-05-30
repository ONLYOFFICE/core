// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/color_profile.h"

#include "base/mac/mac_util.h"

namespace gfx {

void ReadColorProfile(std::vector<char>* profile) {
  CGColorSpaceRef monitor_color_space(base::mac::GetSystemColorSpace());
  base::mac::ScopedCFTypeRef<CFDataRef> icc_profile(
      CGColorSpaceCopyICCProfile(monitor_color_space));
  if (icc_profile) {
    size_t length = CFDataGetLength(icc_profile);
    if (length > gfx::kMaxProfileLength)
      return;
    if (length < gfx::kMinProfileLength)
      return;
    const unsigned char* sys_profile = CFDataGetBytePtr(icc_profile);
    profile->assign(sys_profile, sys_profile + length);
  }
}

}  // namespace gfx

