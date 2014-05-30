// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_SCOPED_REGION_H_
#define UI_BASE_GTK_SCOPED_REGION_H_

#include "base/basictypes.h"
#include "ui/base/ui_export.h"

typedef struct _GdkRegion GdkRegion;

namespace ui {

// Wraps a GdkRegion. This class provides the same methods as ScopedGDIObject in
// base/win/scoped_gdi_object.h.
class UI_EXPORT ScopedRegion {
 public:
  ScopedRegion();
  explicit ScopedRegion(GdkRegion* region);

  ~ScopedRegion();

  void Set(GdkRegion* region);

  GdkRegion* Get();

  GdkRegion* release();

 private:
  void Close();

  GdkRegion* region_;

  DISALLOW_COPY_AND_ASSIGN(ScopedRegion);
};

}  // namespace ui

#endif  // UI_BASE_GTK_SCOPED_REGION_H_
