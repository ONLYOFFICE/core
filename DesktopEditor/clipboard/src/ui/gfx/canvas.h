// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_CANVAS_H_
#define UI_GFX_CANVAS_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "skia/ext/platform_canvas.h"
#include "skia/ext/refptr.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/shadow_value.h"

namespace gfx {

class Rect;
class Font;
class Point;
class Size;
class Transform;

// Canvas is a SkCanvas wrapper that provides a number of methods for
// common operations used throughout an application built using ui/gfx.
//
// All methods that take integer arguments (as is used throughout views)
// end with Int. If you need to use methods provided by SkCanvas, you'll
// need to do a conversion. In particular you'll need to use |SkIntToScalar()|,
// or if converting from a scalar to an integer |SkScalarRound()|.
//
// A handful of methods in this class are overloaded providing an additional
// argument of type SkXfermode::Mode. SkXfermode::Mode specifies how the
// source and destination colors are combined. Unless otherwise specified,
// the variant that does not take a SkXfermode::Mode uses a transfer mode
// of kSrcOver_Mode.
class UI_EXPORT Canvas {
 public:
  enum TruncateFadeMode {
    TruncateFadeTail,
    TruncateFadeHead,
    TruncateFadeHeadAndTail,
  };

  // Specifies the alignment for text rendered with the DrawStringInt method.
  enum {
    TEXT_ALIGN_LEFT = 1 << 0,
    TEXT_ALIGN_CENTER = 1 << 1,
    TEXT_ALIGN_RIGHT = 1 << 2,

    // Specifies the text consists of multiple lines.
    MULTI_LINE = 1 << 3,

    // By default DrawStringInt does not process the prefix ('&') character
    // specially. That is, the string "&foo" is rendered as "&foo". When
    // rendering text from a resource that uses the prefix character for
    // mnemonics, the prefix should be processed and can be rendered as an
    // underline (SHOW_PREFIX), or not rendered at all (HIDE_PREFIX).
    SHOW_PREFIX = 1 << 4,
    HIDE_PREFIX = 1 << 5,

    // Prevent ellipsizing
    NO_ELLIPSIS = 1 << 6,

    // Specifies if words can be split by new lines.
    // This only works with MULTI_LINE.
    CHARACTER_BREAK = 1 << 7,

    // Instructs DrawStringInt() to render the text using RTL directionality.
    // In most cases, passing this flag is not necessary because information
    // about the text directionality is going to be embedded within the string
    // in the form of special Unicode characters. However, we don't insert
    // directionality characters into strings if the locale is LTR because some
    // platforms (for example, an English Windows XP with no RTL fonts
    // installed) don't support these characters. Thus, this flag should be
    // used to render text using RTL directionality when the locale is LTR.
    FORCE_RTL_DIRECTIONALITY = 1 << 8,

    // Similar to FORCE_RTL_DIRECTIONALITY, but left-to-right.
    // See FORCE_RTL_DIRECTIONALITY for details.
    FORCE_LTR_DIRECTIONALITY = 1 << 9,

    // Instructs DrawStringInt() to not use subpixel rendering.  This is useful
    // when rendering text onto a fully- or partially-transparent background
    // that will later be blended with another image.
    NO_SUBPIXEL_RENDERING = 1 << 10,
  };

  // Creates an empty canvas with scale factor of 1x.
  Canvas();

  // Creates canvas with provided DIP |size| and |scale_factor|.
  // If this canvas is not opaque, it's explicitly cleared to transparent before
  // being returned.
  Canvas(const gfx::Size& size,
         ui::ScaleFactor scale_factor,
         bool is_opaque);

  // Constructs a canvas with the size and the scale factor of the
  // provided |image_rep|, and draws the |image_rep| into it.
  Canvas(const gfx::ImageSkiaRep& image_rep, bool is_opaque);

  virtual ~Canvas();

  // Creates a gfx::Canvas backed by an |sk_canvas| with |scale_factor|.
  // |sk_canvas| is assumed to be already scaled based on |scale_factor|
  // so no additional scaling is applied.
  static Canvas* CreateCanvasWithoutScaling(SkCanvas* sk_canvas,
                                            ui::ScaleFactor scale_factor);

  // Recreates the backing platform canvas with DIP |size| and |scale_factor|.
  // If the canvas is not opaque, it is explicitly cleared.
  // This method is public so that canvas_skia_paint can recreate the platform
  // canvas after having initialized the canvas.
  // TODO(pkotwicz): Push the scale factor into skia::PlatformCanvas such that
  // this method can be private.
  void RecreateBackingCanvas(const gfx::Size& size,
                             ui::ScaleFactor scale_factor,
                             bool is_opaque);

  // Compute the size required to draw some text with the provided font.
  // Attempts to fit the text with the provided width and height. Increases
  // height and then width as needed to make the text fit. This method
  // supports multiple lines. On Skia only a line_height can be specified and
  // specifying a 0 value for it will cause the default height to be used.
  static void SizeStringInt(const string16& text,
                            const gfx::Font& font,
                            int* width, int* height,
                            int line_height,
                            int flags);

  // Returns the number of horizontal pixels needed to display the specified
  // |text| with |font|.
  static int GetStringWidth(const string16& text, const gfx::Font& font);

  // Returns the default text alignment to be used when drawing text on a
  // gfx::Canvas based on the directionality of the system locale language.
  // This function is used by gfx::Canvas::DrawStringInt when the text alignment
  // is not specified.
  //
  // This function returns either gfx::Canvas::TEXT_ALIGN_LEFT or
  // gfx::Canvas::TEXT_ALIGN_RIGHT.
  static int DefaultCanvasTextAlignment();

  // Draws text with a 1-pixel halo around it of the given color.
  // On Windows, it allows ClearType to be drawn to an otherwise transparenct
  //   bitmap for drag images. Drag images have only 1-bit of transparency, so
  //   we don't do any fancy blurring.
  // On Linux, text with halo is created by stroking it with 2px |halo_color|
  //   then filling it with |text_color|.
  // On Mac, NOTIMPLEMENTED.
  //   TODO(dhollowa): Skia-native implementation is underway.  Cut over to
  //   that when ready.  http::/crbug.com/109946
  void DrawStringWithHalo(const string16& text,
                          const gfx::Font& font,
                          SkColor text_color,
                          SkColor halo_color,
                          int x, int y, int w, int h,
                          int flags);

  // Extracts an ImageSkiaRep from the contents of this canvas.
  gfx::ImageSkiaRep ExtractImageRep() const;

  // Draws a dashed rectangle of the specified color.
  void DrawDashedRect(const gfx::Rect& rect, SkColor color);

  // Saves a copy of the drawing state onto a stack, operating on this copy
  // until a balanced call to Restore() is made.
  void Save();

  // As with Save(), except draws to a layer that is blended with the canvas
  // at the specified alpha once Restore() is called.
  // |layer_bounds| are the bounds of the layer relative to the current
  // transform.
  void SaveLayerAlpha(uint8 alpha);
  void SaveLayerAlpha(uint8 alpha, const gfx::Rect& layer_bounds);

  // Restores the drawing state after a call to Save*(). It is an error to
  // call Restore() more times than Save*().
  void Restore();

  // Adds |rect| to the current clip. Returns true if the resulting clip is
  // non-empty.
  bool ClipRect(const gfx::Rect& rect);

  // Adds |path| to the current clip. Returns true if the resulting clip is
  // non-empty.
  bool ClipPath(const SkPath& path);

  // Returns the bounds of the current clip (in local coordinates) in the
  // |bounds| parameter, and returns true if it is non empty.
  bool GetClipBounds(gfx::Rect* bounds);

  void Translate(const gfx::Vector2d& offset);

  void Scale(int x_scale, int y_scale);

  // Fills the entire canvas' bitmap (restricted to current clip) with
  // specified |color| using a transfer mode of SkXfermode::kSrcOver_Mode.
  void DrawColor(SkColor color);

  // Fills the entire canvas' bitmap (restricted to current clip) with
  // specified |color| and |mode|.
  void DrawColor(SkColor color, SkXfermode::Mode mode);

  // Fills |rect| with |color| using a transfer mode of
  // SkXfermode::kSrcOver_Mode.
  void FillRect(const gfx::Rect& rect, SkColor color);

  // Fills |rect| with the specified |color| and |mode|.
  void FillRect(const gfx::Rect& rect, SkColor color, SkXfermode::Mode mode);

  // Draws a single pixel rect in the specified region with the specified
  // color, using a transfer mode of SkXfermode::kSrcOver_Mode.
  //
  // NOTE: if you need a single pixel line, use DrawLine.
  void DrawRect(const gfx::Rect& rect, SkColor color);

  // Draws a single pixel rect in the specified region with the specified
  // color and transfer mode.
  //
  // NOTE: if you need a single pixel line, use DrawLine.
  void DrawRect(const gfx::Rect& rect, SkColor color, SkXfermode::Mode mode);

  // Draws the given rectangle with the given |paint| parameters.
  void DrawRect(const gfx::Rect& rect, const SkPaint& paint);

  // Draw the given point with the given |paint| parameters.
  void DrawPoint(const gfx::Point& p, const SkPaint& paint);

  // Draws a single pixel line with the specified color.
  void DrawLine(const gfx::Point& p1, const gfx::Point& p2, SkColor color);

  // Draws a line with the given |paint| parameters.
  void DrawLine(const gfx::Point& p1,
                const gfx::Point& p2,
                const SkPaint& paint);

  // Draws a circle with the given |paint| parameters.
  void DrawCircle(const gfx::Point& center_point,
                  int radius,
                  const SkPaint& paint);

  // Draws the given rectangle with rounded corners of |radius| using the
  // given |paint| parameters.
  void DrawRoundRect(const gfx::Rect& rect, int radius, const SkPaint& paint);

  // Draws the given path using the given |paint| parameters.
  void DrawPath(const SkPath& path, const SkPaint& paint);

  // Draws an image with the origin at the specified location. The upper left
  // corner of the bitmap is rendered at the specified location.
  // Parameters are specified relative to current canvas scale not in pixels.
  // Thus, x is 2 pixels if canvas scale = 2 & |x| = 1.
  void DrawImageInt(const gfx::ImageSkia&, int x, int y);

  // Helper for DrawImageInt(..., paint) that constructs a temporary paint and
  // calls paint.setAlpha(alpha).
  void DrawImageInt(const gfx::ImageSkia&, int x, int y, uint8 alpha);

  // Draws an image with the origin at the specified location, using the
  // specified paint. The upper left corner of the bitmap is rendered at the
  // specified location.
  // Parameters are specified relative to current canvas scale not in pixels.
  // Thus, |x| is 2 pixels if canvas scale = 2 & |x| = 1.
  void DrawImageInt(const gfx::ImageSkia& image,
                    int x, int y,
                    const SkPaint& paint);

  // Draws a portion of an image in the specified location. The src parameters
  // correspond to the region of the bitmap to draw in the region defined
  // by the dest coordinates.
  //
  // If the width or height of the source differs from that of the destination,
  // the image will be scaled. When scaling down, a mipmap will be generated.
  // Set |filter| to use filtering for images, otherwise the nearest-neighbor
  // algorithm is used for resampling.
  //
  // An optional custom SkPaint can be provided.
  // Parameters are specified relative to current canvas scale not in pixels.
  // Thus, |x| is 2 pixels if canvas scale = 2 & |x| = 1.
  void DrawImageInt(const gfx::ImageSkia& image,
                    int src_x, int src_y, int src_w, int src_h,
                    int dest_x, int dest_y, int dest_w, int dest_h,
                    bool filter);
  void DrawImageInt(const gfx::ImageSkia& image,
                    int src_x, int src_y, int src_w, int src_h,
                    int dest_x, int dest_y, int dest_w, int dest_h,
                    bool filter,
                    const SkPaint& paint);

  // Draws an |image| with the top left corner at |x| and |y|, clipped to
  // |path|.
  // Parameters are specified relative to current canvas scale not in pixels.
  // Thus, x is 2 pixels if canvas scale = 2 & |x| = 1.
  void DrawImageInPath(const gfx::ImageSkia& image,
                       int x,
                       int y,
                       const SkPath& path,
                       const SkPaint& paint);

  // Draws text with the specified color, font and location. The text is
  // aligned to the left, vertically centered, clipped to the region. If the
  // text is too big, it is truncated and '...' is added to the end.
  void DrawStringInt(const string16& text,
                     const gfx::Font& font,
                     SkColor color,
                     int x, int y, int w, int h);
  void DrawStringInt(const string16& text,
                     const gfx::Font& font,
                     SkColor color,
                     const gfx::Rect& display_rect);

  // Draws text with the specified color, font and location. The last argument
  // specifies flags for how the text should be rendered. It can be one of
  // TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT or TEXT_ALIGN_LEFT.
  void DrawStringInt(const string16& text,
                     const gfx::Font& font,
                     SkColor color,
                     int x, int y, int w, int h,
                     int flags);

  // Similar to above DrawStringInt method but with text shadows support.
  // Currently it's only implemented for canvas skia. Specifying a 0 line_height
  // will cause the default height to be used.
  void DrawStringWithShadows(const string16& text,
                             const gfx::Font& font,
                             SkColor color,
                             const gfx::Rect& text_bounds,
                             int line_height,
                             int flags,
                             const ShadowValues& shadows);

  // Draws a dotted gray rectangle used for focus purposes.
  void DrawFocusRect(const gfx::Rect& rect);

  // Tiles the image in the specified region.
  // Parameters are specified relative to current canvas scale not in pixels.
  // Thus, |x| is 2 pixels if canvas scale = 2 & |x| = 1.
  void TileImageInt(const gfx::ImageSkia& image,
                    int x, int y, int w, int h);
  void TileImageInt(const gfx::ImageSkia& image,
                    int src_x, int src_y,
                    int dest_x, int dest_y, int w, int h);
  void TileImageInt(const gfx::ImageSkia& image,
                    int src_x, int src_y,
                    float tile_scale_x, float tile_scale_y,
                    int dest_x, int dest_y, int w, int h);

  // Returns a native drawing context for platform specific drawing routines to
  // use. Must be balanced by a call to EndPlatformPaint().
  NativeDrawingContext BeginPlatformPaint();

  // Signifies the end of platform drawing using the native drawing context
  // returned by BeginPlatformPaint().
  void EndPlatformPaint();

  // Apply transformation on the canvas.
  void Transform(const gfx::Transform& transform);

  // Draws the given string with the beginning and/or the end using a fade
  // gradient. When truncating the head
  // |desired_characters_to_truncate_from_head| specifies the maximum number of
  // characters that can be truncated.
  void DrawFadeTruncatingString(
      const string16& text,
      TruncateFadeMode truncate_mode,
      size_t desired_characters_to_truncate_from_head,
      const gfx::Font& font,
      SkColor color,
      const gfx::Rect& display_rect);

  skia::PlatformCanvas* platform_canvas() const { return owned_canvas_.get(); }
  SkCanvas* sk_canvas() const { return canvas_; }
  ui::ScaleFactor scale_factor() const { return scale_factor_; }

 private:
  Canvas(SkCanvas* canvas, ui::ScaleFactor scale_factor);

  // Test whether the provided rectangle intersects the current clip rect.
  bool IntersectsClipRectInt(int x, int y, int w, int h);
  bool IntersectsClipRect(const gfx::Rect& rect);

  // Returns the image rep which best matches the canvas |scale_factor_|.
  // Returns a null image rep if |image| contains no image reps.
  // Builds mip map for returned image rep if necessary.
  //
  // An optional additional user defined scale can be provided.
  const gfx::ImageSkiaRep& GetImageRepToPaint(
      const gfx::ImageSkia& image) const;
  const gfx::ImageSkiaRep& GetImageRepToPaint(
      const gfx::ImageSkia& image,
      float user_defined_scale_factor_x,
      float user_defined_scale_factor_y)  const;

  // The device scale factor at which drawing on this canvas occurs.
  // An additional scale can be applied via Canvas::Scale(). However,
  // Canvas::Scale() does not affect |scale_factor_|.
  ui::ScaleFactor scale_factor_;

  skia::RefPtr<skia::PlatformCanvas> owned_canvas_;
  SkCanvas* canvas_;

  DISALLOW_COPY_AND_ASSIGN(Canvas);
};

}  // namespace gfx

#endif  // UI_GFX_CANVAS_H_
