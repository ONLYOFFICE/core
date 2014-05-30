// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/gtk/scoped_region.h"

#include <gdk/gdk.h>

namespace ui {

ScopedRegion::ScopedRegion() : region_(NULL) {}

ScopedRegion::ScopedRegion(GdkRegion* region) : region_(region) {}

ScopedRegion::~ScopedRegion() {
  Close();
}

void ScopedRegion::Set(GdkRegion* region) {
  Close();
  region_ = region;
}

GdkRegion* ScopedRegion::Get() {
  return region_;
}

GdkRegion* ScopedRegion::release() {
  GdkRegion* region = region_;
  region_ = NULL;
  return region;
}

void ScopedRegion::Close() {
  if (region_) {
    gdk_region_destroy(region_);
    region_ = NULL;
  }
}

}  // namespace ui
