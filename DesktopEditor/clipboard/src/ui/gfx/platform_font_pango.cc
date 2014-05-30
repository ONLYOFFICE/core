// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/platform_font_pango.h"

#include <fontconfig/fontconfig.h>
#include <pango/pango.h>

#include <algorithm>
#include <string>

#include "base/logging.h"
#include "base/strings/string_piece.h"
#include "base/strings/string_split.h"
#include "base/utf_string_conversions.h"
#include "grit/app_locale_settings.h"
#include "third_party/skia/include/core/SkPaint.h"
#include "third_party/skia/include/core/SkTypeface.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font.h"
#include "ui/gfx/pango_util.h"

#if defined(TOOLKIT_GTK)
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#endif

namespace {

// The font family name which is used when a user's application font for
// GNOME/KDE is a non-scalable one. The name should be listed in the
// IsFallbackFontAllowed function in skia/ext/SkFontHost_fontconfig_direct.cpp.
const char* kFallbackFontFamilyName = "sans";

// Returns the available font family that best (in FontConfig's eyes) matches
// the supplied list of family names.
std::string FindBestMatchFontFamilyName(
    const std::vector<std::string>& family_names) {
  FcPattern* pattern = FcPatternCreate();
  for (std::vector<std::string>::const_iterator it = family_names.begin();
       it != family_names.end(); ++it) {
    FcValue fcvalue;
    fcvalue.type = FcTypeString;
    fcvalue.u.s = reinterpret_cast<const FcChar8*>(it->c_str());
    FcPatternAdd(pattern, FC_FAMILY, fcvalue, FcTrue /* append */);
  }

  FcConfigSubstitute(0, pattern, FcMatchPattern);
  FcDefaultSubstitute(pattern);
  FcResult result;
  FcPattern* match = FcFontMatch(0, pattern, &result);
  DCHECK(match) << "Could not find font";
  FcChar8* match_family = NULL;
  FcPatternGetString(match, FC_FAMILY, 0, &match_family);
  std::string font_family(reinterpret_cast<char*>(match_family));
  FcPatternDestroy(pattern);
  FcPatternDestroy(match);
  return font_family;
}

// Returns a Pango font description (suitable for parsing by
// pango_font_description_from_string()) for the default UI font.
std::string GetDefaultFont() {
#if !defined(TOOLKIT_GTK)
#if defined(OS_CHROMEOS)
  return l10n_util::GetStringUTF8(IDS_UI_FONT_FAMILY_CROS);
#else
  return "sans 10";
#endif    // defined(OS_CHROMEOS)
#else
  GtkSettings* settings = gtk_settings_get_default();

  gchar* font_name = NULL;
  g_object_get(settings, "gtk-font-name", &font_name, NULL);

  // Temporary CHECK for helping track down
  // http://code.google.com/p/chromium/issues/detail?id=12530
  CHECK(font_name) << " Unable to get gtk-font-name for default font.";

  std::string default_font = std::string(font_name);
  g_free(font_name);
  return default_font;
#endif  // !defined(TOOLKIT_GTK)
}

}  // namespace

namespace gfx {

Font* PlatformFontPango::default_font_ = NULL;

////////////////////////////////////////////////////////////////////////////////
// PlatformFontPango, public:

PlatformFontPango::PlatformFontPango() {
  if (default_font_ == NULL) {
    std::string font_name = GetDefaultFont();

    ScopedPangoFontDescription desc(
        pango_font_description_from_string(font_name.c_str()));
    default_font_ = new Font(desc.get());

    DCHECK(default_font_);
  }

  InitFromPlatformFont(
      static_cast<PlatformFontPango*>(default_font_->platform_font()));
}

PlatformFontPango::PlatformFontPango(NativeFont native_font) {
  std::vector<std::string> family_names;
  base::SplitString(pango_font_description_get_family(native_font), ',',
                    &family_names);
  std::string font_family = FindBestMatchFontFamilyName(family_names);
  InitWithNameAndSize(font_family, gfx::GetPangoFontSizeInPixels(native_font));

  int style = 0;
  if (pango_font_description_get_weight(native_font) == PANGO_WEIGHT_BOLD) {
    // TODO(davemoore) What should we do about other weights? We currently
    // only support BOLD.
    style |= gfx::Font::BOLD;
  }
  if (pango_font_description_get_style(native_font) == PANGO_STYLE_ITALIC) {
    // TODO(davemoore) What about PANGO_STYLE_OBLIQUE?
    style |= gfx::Font::ITALIC;
  }
  if (style != 0)
    style_ = style;
}

PlatformFontPango::PlatformFontPango(const std::string& font_name,
                                     int font_size) {
  InitWithNameAndSize(font_name, font_size);
}

double PlatformFontPango::underline_position() const {
  const_cast<PlatformFontPango*>(this)->InitPangoMetrics();
  return underline_position_pixels_;
}

double PlatformFontPango::underline_thickness() const {
  const_cast<PlatformFontPango*>(this)->InitPangoMetrics();
  return underline_thickness_pixels_;
}

////////////////////////////////////////////////////////////////////////////////
// PlatformFontPango, PlatformFont implementation:

// static
void PlatformFontPango::ReloadDefaultFont() {
  delete default_font_;
  default_font_ = NULL;
}

Font PlatformFontPango::DeriveFont(int size_delta, int style) const {
  // If the delta is negative, if must not push the size below 1
  if (size_delta < 0)
    DCHECK_LT(-size_delta, font_size_pixels_);

  if (style == style_) {
    // Fast path, we just use the same typeface at a different size
    return Font(new PlatformFontPango(typeface_,
                                      font_family_,
                                      font_size_pixels_ + size_delta,
                                      style_));
  }

  // If the style has changed we may need to load a new face
  int skstyle = SkTypeface::kNormal;
  if (gfx::Font::BOLD & style)
    skstyle |= SkTypeface::kBold;
  if (gfx::Font::ITALIC & style)
    skstyle |= SkTypeface::kItalic;

  skia::RefPtr<SkTypeface> typeface = skia::AdoptRef(
      SkTypeface::CreateFromName(
          font_family_.c_str(),
          static_cast<SkTypeface::Style>(skstyle)));

  return Font(new PlatformFontPango(typeface,
                                    font_family_,
                                    font_size_pixels_ + size_delta,
                                    style));
}

int PlatformFontPango::GetHeight() const {
  return height_pixels_;
}

int PlatformFontPango::GetBaseline() const {
  return ascent_pixels_;
}

int PlatformFontPango::GetAverageCharacterWidth() const {
  const_cast<PlatformFontPango*>(this)->InitPangoMetrics();
  return SkScalarRound(average_width_pixels_);
}

int PlatformFontPango::GetStringWidth(const string16& text) const {
  return Canvas::GetStringWidth(text,
                                Font(const_cast<PlatformFontPango*>(this)));
}

int PlatformFontPango::GetExpectedTextWidth(int length) const {
  double char_width = const_cast<PlatformFontPango*>(this)->GetAverageWidth();
  return round(static_cast<float>(length) * char_width);
}

int PlatformFontPango::GetStyle() const {
  return style_;
}

std::string PlatformFontPango::GetFontName() const {
  return font_family_;
}

int PlatformFontPango::GetFontSize() const {
  return font_size_pixels_;
}

NativeFont PlatformFontPango::GetNativeFont() const {
  PangoFontDescription* pfd = pango_font_description_new();
  pango_font_description_set_family(pfd, GetFontName().c_str());
  // Set the absolute size to avoid overflowing UI elements.
  // pango_font_description_set_absolute_size() takes a size in Pango units.
  // There are PANGO_SCALE Pango units in one device unit.  Screen output
  // devices use pixels as their device units.
  pango_font_description_set_absolute_size(
      pfd, font_size_pixels_ * PANGO_SCALE);

  switch (GetStyle()) {
    case gfx::Font::NORMAL:
      // Nothing to do, should already be PANGO_STYLE_NORMAL.
      break;
    case gfx::Font::BOLD:
      pango_font_description_set_weight(pfd, PANGO_WEIGHT_BOLD);
      break;
    case gfx::Font::ITALIC:
      pango_font_description_set_style(pfd, PANGO_STYLE_ITALIC);
      break;
    case gfx::Font::UNDERLINE:
      // TODO(deanm): How to do underline?  Where do we use it?  Probably have
      // to paint it ourselves, see pango_font_metrics_get_underline_position.
      break;
  }

  return pfd;
}

////////////////////////////////////////////////////////////////////////////////
// PlatformFontPango, private:

PlatformFontPango::PlatformFontPango(const skia::RefPtr<SkTypeface>& typeface,
                                     const std::string& name,
                                     int size,
                                     int style) {
  InitWithTypefaceNameSizeAndStyle(typeface, name, size, style);
}

PlatformFontPango::~PlatformFontPango() {}

void PlatformFontPango::InitWithNameAndSize(const std::string& font_name,
                                            int font_size) {
  DCHECK_GT(font_size, 0);
  std::string fallback;

  skia::RefPtr<SkTypeface> typeface = skia::AdoptRef(
          SkTypeface::CreateFromName(font_name.c_str(), SkTypeface::kNormal));
  if (!typeface) {
    // A non-scalable font such as .pcf is specified. Falls back to a default
    // scalable font.
    typeface = skia::AdoptRef(
        SkTypeface::CreateFromName(
            kFallbackFontFamilyName, SkTypeface::kNormal));
    CHECK(typeface) << "Could not find any font: "
                    << font_name
                    << ", " << kFallbackFontFamilyName;
    fallback = kFallbackFontFamilyName;
  }

  InitWithTypefaceNameSizeAndStyle(typeface,
                                   fallback.empty() ? font_name : fallback,
                                   font_size,
                                   gfx::Font::NORMAL);
}

void PlatformFontPango::InitWithTypefaceNameSizeAndStyle(
    const skia::RefPtr<SkTypeface>& typeface,
    const std::string& font_family,
    int font_size,
    int style) {
  typeface_ = typeface;
  font_family_ = font_family;
  font_size_pixels_ = font_size;
  style_ = style;
  pango_metrics_inited_ = false;
  average_width_pixels_ = 0.0f;
  underline_position_pixels_ = 0.0f;
  underline_thickness_pixels_ = 0.0f;

  SkPaint paint;
  SkPaint::FontMetrics metrics;
  PaintSetup(&paint);
  paint.getFontMetrics(&metrics);

  ascent_pixels_ = SkScalarCeil(-metrics.fAscent);
  height_pixels_ = ascent_pixels_ + SkScalarCeil(metrics.fDescent);
}

void PlatformFontPango::InitFromPlatformFont(const PlatformFontPango* other) {
  typeface_ = other->typeface_;
  font_family_ = other->font_family_;
  font_size_pixels_ = other->font_size_pixels_;
  style_ = other->style_;
  height_pixels_ = other->height_pixels_;
  ascent_pixels_ = other->ascent_pixels_;
  pango_metrics_inited_ = other->pango_metrics_inited_;
  average_width_pixels_ = other->average_width_pixels_;
  underline_position_pixels_ = other->underline_position_pixels_;
  underline_thickness_pixels_ = other->underline_thickness_pixels_;
}

void PlatformFontPango::PaintSetup(SkPaint* paint) const {
  paint->setAntiAlias(false);
  paint->setSubpixelText(false);
  paint->setTextSize(font_size_pixels_);
  paint->setTypeface(typeface_.get());
  paint->setFakeBoldText((gfx::Font::BOLD & style_) && !typeface_->isBold());
  paint->setTextSkewX((gfx::Font::ITALIC & style_) && !typeface_->isItalic() ?
                      -SK_Scalar1/4 : 0);
}

void PlatformFontPango::InitPangoMetrics() {
  if (!pango_metrics_inited_) {
    pango_metrics_inited_ = true;
    ScopedPangoFontDescription pango_desc(GetNativeFont());
    PangoFontMetrics* pango_metrics = GetPangoFontMetrics(pango_desc.get());

    underline_position_pixels_ =
        pango_font_metrics_get_underline_position(pango_metrics) /
        PANGO_SCALE;

    // TODO(davemoore): Come up with a better solution.
    // This is a hack, but without doing this the underlines
    // we get end up fuzzy. So we align to the midpoint of a pixel.
    underline_position_pixels_ /= 2;

    underline_thickness_pixels_ =
        pango_font_metrics_get_underline_thickness(pango_metrics) /
        PANGO_SCALE;

    // First get the Pango-based width (converting from Pango units to pixels).
    const double pango_width_pixels =
        pango_font_metrics_get_approximate_char_width(pango_metrics) /
        PANGO_SCALE;

    // Yes, this is how Microsoft recommends calculating the dialog unit
    // conversions.
    const int text_width_pixels = GetStringWidth(
        ASCIIToUTF16("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
    const double dialog_units_pixels = (text_width_pixels / 26 + 1) / 2;
    average_width_pixels_ = std::min(pango_width_pixels, dialog_units_pixels);
  }
}


double PlatformFontPango::GetAverageWidth() const {
  const_cast<PlatformFontPango*>(this)->InitPangoMetrics();
  return average_width_pixels_;
}

////////////////////////////////////////////////////////////////////////////////
// PlatformFont, public:

// static
PlatformFont* PlatformFont::CreateDefault() {
  return new PlatformFontPango;
}

// static
PlatformFont* PlatformFont::CreateFromNativeFont(NativeFont native_font) {
  return new PlatformFontPango(native_font);
}

// static
PlatformFont* PlatformFont::CreateFromNameAndSize(const std::string& font_name,
                                                  int font_size) {
  return new PlatformFontPango(font_name, font_size);
}

}  // namespace gfx
