// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "ui/gfx/canvas.h"

#include "base/logging.h"
#include "base/strings/sys_string_conversions.h"
#include "third_party/skia/include/core/SkTypeface.h"
#include "ui/gfx/font.h"
#include "ui/gfx/rect.h"

// Note: This is a temporary Skia-based implementation of the ui/gfx text
// rendering routines for views/aura. It replaces the stale Cocoa-based
// implementation. A future |canvas_skia.cc| implementation will supersede
// this and the other platform-specific implmenentations. Most drawing options,
// such as alignment, multi-line, and line heights are not implemented here.

namespace {

SkTypeface::Style FontTypefaceStyle(const gfx::Font& font) {
  int style = 0;
  if (font.GetStyle() & gfx::Font::BOLD)
    style |= SkTypeface::kBold;
  if (font.GetStyle() & gfx::Font::ITALIC)
    style |= SkTypeface::kItalic;

  return static_cast<SkTypeface::Style>(style);
}

}  // namespace

namespace gfx {

// static
void Canvas::SizeStringInt(const string16& text,
                           const gfx::Font& font,
                           int* width,
                           int* height,
                           int line_height,
                           int flags) {
  DLOG_IF(WARNING, line_height != 0) << "Line heights not implemented.";
  DLOG_IF(WARNING, flags & Canvas::MULTI_LINE) << "Multi-line not implemented.";

  NSFont* native_font = font.GetNativeFont();
  NSString* ns_string = base::SysUTF16ToNSString(text);
  NSDictionary* attributes =
      [NSDictionary dictionaryWithObject:native_font
                                  forKey:NSFontAttributeName];
  NSSize string_size = [ns_string sizeWithAttributes:attributes];
  *width = string_size.width;
  *height = font.GetHeight();
}

void Canvas::DrawStringWithShadows(const string16& text,
                                   const gfx::Font& font,
                                   SkColor color,
                                   const gfx::Rect& text_bounds,
                                   int line_height,
                                   int flags,
                                   const ShadowValues& shadows) {
  DLOG_IF(WARNING, line_height != 0) << "Line heights not implemented.";
  DLOG_IF(WARNING, flags & Canvas::MULTI_LINE) << "Multi-line not implemented.";
  DLOG_IF(WARNING, !shadows.empty()) << "Text shadows not implemented.";

  skia::RefPtr<SkTypeface> typeface = skia::AdoptRef(
      SkTypeface::CreateFromName(
          font.GetFontName().c_str(), FontTypefaceStyle(font)));
  SkPaint paint;
  paint.setTypeface(typeface.get());
  paint.setColor(color);
  canvas_->drawText(text.c_str(),
                    text.size() * sizeof(string16::value_type),
                    text_bounds.x(),
                    text_bounds.bottom(),
                    paint);
}

void Canvas::DrawStringWithHalo(const string16& text,
                                const gfx::Font& font,
                                SkColor text_color,
                                SkColor halo_color,
                                int x, int y, int w, int h,
                                int flags) {
}

}  // namespace gfx
