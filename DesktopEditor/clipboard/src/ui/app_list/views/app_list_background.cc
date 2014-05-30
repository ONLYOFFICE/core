// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/views/app_list_background.h"

#include "third_party/skia/include/core/SkPaint.h"
#include "third_party/skia/include/core/SkPath.h"
#include "ui/app_list/app_list_constants.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/skia_util.h"
#include "ui/views/view.h"

namespace {

const SkColor kSearchBoxBackground = SK_ColorWHITE;

// Colors and sizes of top separator between searchbox and grid view.
const SkColor kTopSeparatorColor = SkColorSetRGB(0xE5, 0xE5, 0xE5);
const int kTopSeparatorSize = 1;

}  // namespace

namespace app_list {

AppListBackground::AppListBackground(int corner_radius,
                                     views::View* search_box_view)
    : corner_radius_(corner_radius),
      search_box_view_(search_box_view) {
}

AppListBackground::~AppListBackground() {
}

void AppListBackground::Paint(gfx::Canvas* canvas,
                              views::View* view) const {
  gfx::Rect bounds = view->GetContentsBounds();

  canvas->Save();
  SkPath path;
  // Contents corner radius is 1px smaller than border corner radius.
  SkScalar radius = SkIntToScalar(corner_radius_ - 1);
  path.addRoundRect(gfx::RectToSkRect(bounds), radius, radius);
  canvas->ClipPath(path);

  SkPaint paint;
  paint.setStyle(SkPaint::kFill_Style);

  const gfx::Rect search_box_view_bounds =
      search_box_view_->ConvertRectToWidget(search_box_view_->GetLocalBounds());
  gfx::Rect search_box_rect(bounds.x(),
                            bounds.y(),
                            bounds.width(),
                            search_box_view_bounds.bottom() - bounds.y());

  paint.setColor(kSearchBoxBackground);
  canvas->DrawRect(search_box_rect, paint);

  gfx::Rect seperator_rect(search_box_rect);
  seperator_rect.set_y(seperator_rect.bottom());
  seperator_rect.set_height(kTopSeparatorSize);
  canvas->FillRect(seperator_rect, kTopSeparatorColor);

  gfx::Rect contents_rect(bounds.x(),
                          seperator_rect.bottom(),
                          bounds.width(),
                          bounds.bottom() - seperator_rect.bottom());

  paint.setColor(kContentsBackgroundColor);
  canvas->DrawRect(contents_rect, paint);
  canvas->Restore();
}

}  // namespace app_list
