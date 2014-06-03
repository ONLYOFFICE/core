// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/compiler_specific.h"
#include "skia/ext/analysis_canvas.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/skia/include/core/SkShader.h"

namespace {

void solidColorFill(skia::AnalysisCanvas& canvas) {
  canvas.clear(SkColorSetARGB(255, 255, 255, 255));
}

void transparentFill(skia::AnalysisCanvas& canvas) {
  canvas.clear(SkColorSetARGB(0, 0, 0, 0));
}

} // namespace
namespace skia {

TEST(AnalysisCanvasTest, EmptyCanvas) {
  SkBitmap emptyBitmap;
  emptyBitmap.setConfig(SkBitmap::kNo_Config, 255, 255);
  skia::AnalysisDevice device(emptyBitmap);
  skia::AnalysisCanvas canvas(&device);
  
  SkColor color;
  EXPECT_FALSE(canvas.getColorIfSolid(&color));
}

TEST(AnalysisCanvasTest, ClearCanvas) {
  SkBitmap emptyBitmap;
  emptyBitmap.setConfig(SkBitmap::kNo_Config, 255, 255);
  skia::AnalysisDevice device(emptyBitmap);
  skia::AnalysisCanvas canvas(&device);

  // Transparent color
  SkColor color = SkColorSetARGB(0, 12, 34, 56);
  canvas.clear(color);

  SkColor outputColor;
  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_EQ(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);

  // Solid color
  color = SkColorSetARGB(255, 65, 43, 21);
  canvas.clear(color);

  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_NE(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);
  EXPECT_EQ(outputColor, color);

  // Translucent color
  color = SkColorSetARGB(128, 11, 22, 33);
  canvas.clear(color);

  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  // Test helper methods
  solidColorFill(canvas);
  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_NE(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);

  transparentFill(canvas);
  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_EQ(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);
}

TEST(AnalysisCanvasTest, ComplexActions) {
  SkBitmap emptyBitmap;
  emptyBitmap.setConfig(SkBitmap::kNo_Config, 255, 255);
  skia::AnalysisDevice device(emptyBitmap);
  skia::AnalysisCanvas canvas(&device);

  // Draw paint test.
  SkColor color = SkColorSetARGB(255, 11, 22, 33);
  SkPaint paint;
  paint.setColor(color);

  canvas.drawPaint(paint);

  SkColor outputColor;
  //TODO(vmpstr): This should return true. (crbug.com/180597)
  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  // Draw points test.
  SkPoint points[4] = {
    SkPoint::Make(0, 0),
    SkPoint::Make(255, 0),
    SkPoint::Make(255, 255),
    SkPoint::Make(0, 255)
  };

  solidColorFill(canvas);
  canvas.drawPoints(SkCanvas::kLines_PointMode, 4, points, paint);

  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  // Draw oval test.
  solidColorFill(canvas);
  canvas.drawOval(SkRect::MakeWH(255, 255), paint);

  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  // Draw bitmap test.
  solidColorFill(canvas);
  SkBitmap secondBitmap;
  secondBitmap.setConfig(SkBitmap::kNo_Config, 255, 255);
  canvas.drawBitmap(secondBitmap, 0, 0);

  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));
}

TEST(AnalysisCanvasTest, SimpleDrawRect) {
  SkBitmap emptyBitmap;
  emptyBitmap.setConfig(SkBitmap::kNo_Config, 255, 255);
  skia::AnalysisDevice device(emptyBitmap);
  skia::AnalysisCanvas canvas(&device);

  SkColor color = SkColorSetARGB(255, 11, 22, 33);
  SkPaint paint;
  paint.setColor(color);
  canvas.clipRect(SkRect::MakeWH(255, 255));
  canvas.drawRect(SkRect::MakeWH(255, 255), paint);

  SkColor outputColor;
  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_NE(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);
  EXPECT_EQ(color, outputColor);

  color = SkColorSetARGB(255, 22, 33, 44);
  paint.setColor(color);
  canvas.translate(-128, -128);
  canvas.drawRect(SkRect::MakeWH(382, 382), paint);

  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  color = SkColorSetARGB(255, 33, 44, 55);
  paint.setColor(color);
  canvas.drawRect(SkRect::MakeWH(383, 383), paint);

  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_NE(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);
  EXPECT_EQ(color, outputColor);

  color = SkColorSetARGB(0, 0, 0, 0);
  paint.setColor(color);
  canvas.drawRect(SkRect::MakeWH(383, 383), paint);

  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_NE(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);
  EXPECT_EQ(outputColor, SkColorSetARGB(255, 33, 44, 55));

  color = SkColorSetARGB(128, 128, 128, 128);
  paint.setColor(color);
  canvas.drawRect(SkRect::MakeWH(383, 383), paint);

  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  paint.setXfermodeMode(SkXfermode::kClear_Mode);
  canvas.drawRect(SkRect::MakeWH(382, 382), paint);

  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  canvas.drawRect(SkRect::MakeWH(383, 383), paint);

  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_EQ(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);

  canvas.translate(128, 128);
  color = SkColorSetARGB(255, 11, 22, 33);
  paint.setColor(color);
  paint.setXfermodeMode(SkXfermode::kSrcOver_Mode);
  canvas.drawRect(SkRect::MakeWH(255, 255), paint);

  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_NE(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);
  EXPECT_EQ(color, outputColor);

  canvas.rotate(50);
  canvas.drawRect(SkRect::MakeWH(255, 255), paint);

  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));
}

TEST(AnalysisCanvasTest, ClipPath) {
  SkBitmap emptyBitmap;
  emptyBitmap.setConfig(SkBitmap::kNo_Config, 255, 255);
  skia::AnalysisDevice device(emptyBitmap);
  skia::AnalysisCanvas canvas(&device);

  SkPath path;
  path.moveTo(0, 0);
  path.lineTo(255, 0);
  path.lineTo(255, 255);
  path.lineTo(0, 255);

  SkColor outputColor;
  solidColorFill(canvas);
  canvas.clipPath(path);
  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  canvas.save();
  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  canvas.clipPath(path);
  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  canvas.restore();
  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  solidColorFill(canvas);
  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));
}

TEST(AnalysisCanvasTest, SaveLayerRestore) {
  SkBitmap emptyBitmap;
  emptyBitmap.setConfig(SkBitmap::kNo_Config, 255, 255);
  skia::AnalysisDevice device(emptyBitmap);
  skia::AnalysisCanvas canvas(&device);

  SkColor outputColor;
  solidColorFill(canvas);
  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  
  SkRect bounds = SkRect::MakeWH(255, 255);
  SkPaint paint;
  paint.setColor(SkColorSetARGB(255, 255, 255, 255));
  paint.setXfermodeMode(SkXfermode::kSrcOver_Mode);

  // This should force non-transparency
  canvas.saveLayer(&bounds, &paint, SkCanvas::kMatrix_SaveFlag);
  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_NE(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);

  transparentFill(canvas);
  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  solidColorFill(canvas);
  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_NE(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);

  paint.setXfermodeMode(SkXfermode::kDst_Mode);

  // This should force non-solid color
  canvas.saveLayer(&bounds, &paint, SkCanvas::kMatrix_SaveFlag);
  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  transparentFill(canvas);
  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  solidColorFill(canvas);
  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));
  
  canvas.restore();
  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  transparentFill(canvas);
  EXPECT_FALSE(canvas.getColorIfSolid(&outputColor));

  solidColorFill(canvas);
  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_NE(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);

  canvas.restore();
  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_NE(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);

  transparentFill(canvas);
  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_EQ(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);

  solidColorFill(canvas);
  EXPECT_TRUE(canvas.getColorIfSolid(&outputColor));
  EXPECT_NE(static_cast<SkColor>(SK_ColorTRANSPARENT), outputColor);
}

TEST(AnalysisCanvasTest, HasText) {
  int width = 200;
  int height = 100;

  SkBitmap bitmap;
  bitmap.setConfig(SkBitmap::kNo_Config, width, height);

  const char* text = "A";
  size_t byteLength = 1;

  SkPoint point = SkPoint::Make(SkIntToScalar(25), SkIntToScalar(25));
  SkPath path;
  path.moveTo(point);
  path.lineTo(SkIntToScalar(75), SkIntToScalar(75));

  SkPaint paint;
  paint.setColor(SK_ColorGRAY);
  paint.setTextSize(SkIntToScalar(10));

  {
    skia::AnalysisDevice device(bitmap);
    skia::AnalysisCanvas canvas(&device);
    // Test after initialization.
    EXPECT_FALSE(canvas.hasText());
    // Test drawing anything other than text.
    canvas.drawRect(SkRect::MakeWH(width/2, height), paint);
    EXPECT_FALSE(canvas.hasText());
  }
  {
    // Test SkCanvas::drawText.
    skia::AnalysisDevice device(bitmap);
    skia::AnalysisCanvas canvas(&device);
    canvas.drawText(text, byteLength, point.fX, point.fY, paint);
    EXPECT_TRUE(canvas.hasText());
  }
  {
    // Test SkCanvas::drawPosText.
    skia::AnalysisDevice device(bitmap);
    skia::AnalysisCanvas canvas(&device);
    canvas.drawPosText(text, byteLength, &point, paint);
    EXPECT_TRUE(canvas.hasText());
  }
  {
    // Test SkCanvas::drawPosTextH.
    skia::AnalysisDevice device(bitmap);
    skia::AnalysisCanvas canvas(&device);
    canvas.drawPosTextH(text, byteLength, &point.fX, point.fY, paint);
    EXPECT_TRUE(canvas.hasText());
  }
  {
    // Test SkCanvas::drawTextOnPathHV.
    skia::AnalysisDevice device(bitmap);
    skia::AnalysisCanvas canvas(&device);
    canvas.drawTextOnPathHV(text, byteLength, path, point.fX, point.fY, paint);
    EXPECT_TRUE(canvas.hasText());
  }
  {
    // Test SkCanvas::drawTextOnPath.
    skia::AnalysisDevice device(bitmap);
    skia::AnalysisCanvas canvas(&device);
    canvas.drawTextOnPath(text, byteLength, path, NULL, paint);
    EXPECT_TRUE(canvas.hasText());
  }
  {
    // Text under opaque rect.
    skia::AnalysisDevice device(bitmap);
    skia::AnalysisCanvas canvas(&device);
    canvas.drawText(text, byteLength, point.fX, point.fY, paint);
    EXPECT_TRUE(canvas.hasText());
    canvas.drawRect(SkRect::MakeWH(width, height), paint);
    EXPECT_FALSE(canvas.hasText());
  }
  {
    // Text under translucent rect.
    skia::AnalysisDevice device(bitmap);
    skia::AnalysisCanvas canvas(&device);
    canvas.drawText(text, byteLength, point.fX, point.fY, paint);
    EXPECT_TRUE(canvas.hasText());
    SkPaint translucentPaint;
    translucentPaint.setColor(0x88FFFFFF);
    canvas.drawRect(SkRect::MakeWH(width, height), translucentPaint);
    EXPECT_TRUE(canvas.hasText());
  }
  {
    // Text under rect in clear mode.
    skia::AnalysisDevice device(bitmap);
    skia::AnalysisCanvas canvas(&device);
    canvas.drawText(text, byteLength, point.fX, point.fY, paint);
    EXPECT_TRUE(canvas.hasText());
    SkPaint clearModePaint;
    clearModePaint.setXfermodeMode(SkXfermode::kClear_Mode);
    canvas.drawRect(SkRect::MakeWH(width, height), clearModePaint);
    EXPECT_FALSE(canvas.hasText());
  }
  {
    // Clear.
    skia::AnalysisDevice device(bitmap);
    skia::AnalysisCanvas canvas(&device);
    canvas.drawText(text, byteLength, point.fX, point.fY, paint);
    EXPECT_TRUE(canvas.hasText());
    canvas.clear(SK_ColorGRAY);
    EXPECT_FALSE(canvas.hasText());
  }
  {
    // Text inside clip region.
    skia::AnalysisDevice device(bitmap);
    skia::AnalysisCanvas canvas(&device);
    canvas.clipRect(SkRect::MakeWH(100, 100));
    canvas.drawText(text, byteLength, point.fX, point.fY, paint);
    EXPECT_TRUE(canvas.hasText());
  }
  {
    // Text outside clip region.
    skia::AnalysisDevice device(bitmap);
    skia::AnalysisCanvas canvas(&device);
    canvas.clipRect(SkRect::MakeXYWH(100, 0, 100, 100));
    canvas.drawText(text, byteLength, point.fX, point.fY, paint);
    // Analysis device does not do any clipping.
    // So even when text is outside the clip region,
    // it is marked as having the text.
    // TODO(alokp): We may be able to do some trivial rejection.
    EXPECT_TRUE(canvas.hasText());
  }
}

}  // namespace skia
