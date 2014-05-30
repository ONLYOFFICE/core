// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/path.h"

#include <gdk/gdk.h>

#include "base/command_line.h"
#include "base/memory/scoped_ptr.h"

namespace gfx {

GdkRegion* Path::CreateNativeRegion() const {
  int point_count = getPoints(NULL, 0);
  if (point_count <= 1) {
    // NOTE: ideally this would return gdk_empty_region, but that returns a
    // region with nothing in it.
    return NULL;
  }

  scoped_ptr<SkPoint[]> points(new SkPoint[point_count]);
  getPoints(points.get(), point_count);

  scoped_ptr<GdkPoint[]> gdk_points(new GdkPoint[point_count]);
  for (int i = 0; i < point_count; ++i) {
    gdk_points[i].x = SkScalarRound(points[i].fX);
    gdk_points[i].y = SkScalarRound(points[i].fY);
  }

  return gdk_region_polygon(gdk_points.get(), point_count, GDK_EVEN_ODD_RULE);
}

// static
NativeRegion Path::IntersectRegions(NativeRegion r1, NativeRegion r2) {
  GdkRegion* copy = gdk_region_copy(r1);
  gdk_region_intersect(copy, r2);
  return copy;
}

// static
NativeRegion Path::CombineRegions(NativeRegion r1, NativeRegion r2) {
  GdkRegion* copy = gdk_region_copy(r1);
  gdk_region_union(copy, r2);
  return copy;
}

// static
NativeRegion Path::SubtractRegion(NativeRegion r1, NativeRegion r2) {
  GdkRegion* copy = gdk_region_copy(r1);
  gdk_region_subtract(copy, r2);
  return copy;
}

}  // namespace gfx
