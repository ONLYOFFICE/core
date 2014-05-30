// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/platform_font_mac.h"

#include <Cocoa/Cocoa.h>

#include "base/basictypes.h"
#include "base/memory/scoped_nsobject.h"
#include "base/strings/sys_string_conversions.h"
#include "base/utf_string_conversions.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font.h"

namespace gfx {

////////////////////////////////////////////////////////////////////////////////
// PlatformFontMac, public:

PlatformFontMac::PlatformFontMac() {
  font_size_ = [NSFont systemFontSize];
  style_ = gfx::Font::NORMAL;
  NSFont* system_font = [NSFont systemFontOfSize:font_size_];
  font_name_ = base::SysNSStringToUTF8([system_font fontName]);
  CalculateMetrics();
}

PlatformFontMac::PlatformFontMac(NativeFont native_font) {
  int style = 0;
  NSFontSymbolicTraits traits = [[native_font fontDescriptor] symbolicTraits];
  if (traits & NSFontItalicTrait)
    style |= Font::ITALIC;
  if (traits & NSFontBoldTrait)
    style |= Font::BOLD;

  InitWithNameSizeAndStyle(base::SysNSStringToUTF8([native_font familyName]),
                           [native_font pointSize],
                           style);
}

PlatformFontMac::PlatformFontMac(const std::string& font_name,
                                 int font_size) {
  InitWithNameSizeAndStyle(font_name, font_size, gfx::Font::NORMAL);
}

////////////////////////////////////////////////////////////////////////////////
// PlatformFontMac, PlatformFont implementation:

Font PlatformFontMac::DeriveFont(int size_delta, int style) const {
  return Font(new PlatformFontMac(font_name_, font_size_ + size_delta, style));
}

int PlatformFontMac::GetHeight() const {
  return height_;
}

int PlatformFontMac::GetBaseline() const {
  return ascent_;
}

int PlatformFontMac::GetAverageCharacterWidth() const {
  return average_width_;
}

int PlatformFontMac::GetStringWidth(const string16& text) const {
  return Canvas::GetStringWidth(text,
                                Font(const_cast<PlatformFontMac*>(this)));
}

int PlatformFontMac::GetExpectedTextWidth(int length) const {
  return length * average_width_;
}

int PlatformFontMac::GetStyle() const {
  return style_;
}

std::string PlatformFontMac::GetFontName() const {
  return font_name_;
}

int PlatformFontMac::GetFontSize() const {
  return font_size_;
}

NativeFont PlatformFontMac::GetNativeFont() const {
  // We could cache this, but then we'd have to conditionally change the
  // dtor just for MacOS. Not sure if we want to/need to do that.
  NSFont* font = [NSFont fontWithName:base::SysUTF8ToNSString(font_name_)
                                 size:font_size_];

  if (style_ & Font::BOLD) {
    font = [[NSFontManager sharedFontManager] convertFont:font
                                              toHaveTrait:NSBoldFontMask];
  }
  if (style_ & Font::ITALIC) {
    font = [[NSFontManager sharedFontManager] convertFont:font
                                              toHaveTrait:NSItalicFontMask];
  }
  // Mac doesn't support underline as a font trait, just drop it. Underlines
  // can instead be added as an attribute on an NSAttributedString.

  return font;
}

////////////////////////////////////////////////////////////////////////////////
// PlatformFontMac, private:

PlatformFontMac::PlatformFontMac(const std::string& font_name,
                                 int font_size,
                                 int style) {
  InitWithNameSizeAndStyle(font_name, font_size, style);
}

void PlatformFontMac::InitWithNameSizeAndStyle(const std::string& font_name,
                                               int font_size,
                                               int style) {
  font_name_ = font_name;
  font_size_ = font_size;
  style_ = style;
  CalculateMetrics();
}

void PlatformFontMac::CalculateMetrics() {
  NSFont* font = GetNativeFont();
  scoped_nsobject<NSLayoutManager> layout_manager(
      [[NSLayoutManager alloc] init]);
  height_ = [layout_manager defaultLineHeightForFont:font];
  ascent_ = [font ascender];
  average_width_ =
      NSWidth([font boundingRectForGlyph:[font glyphWithName:@"x"]]);
}

////////////////////////////////////////////////////////////////////////////////
// PlatformFont, public:

// static
PlatformFont* PlatformFont::CreateDefault() {
  return new PlatformFontMac;
}

// static
PlatformFont* PlatformFont::CreateFromNativeFont(NativeFont native_font) {
  return new PlatformFontMac(native_font);
}

// static
PlatformFont* PlatformFont::CreateFromNameAndSize(const std::string& font_name,
                                                  int font_size) {
  return new PlatformFontMac(font_name, font_size);
}

}  // namespace gfx

