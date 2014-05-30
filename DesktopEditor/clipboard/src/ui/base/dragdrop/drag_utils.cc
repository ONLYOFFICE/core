// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/dragdrop/drag_utils.h"

#include "base/file_util.h"
#include "base/logging.h"
#include "base/utf_string_conversions.h"
#include "googleurl/src/gurl.h"
#include "ui/base/dragdrop/os_exchange_data.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font.h"
#include "ui/gfx/image/canvas_image_source.h"
#include "ui/gfx/point.h"
#include "ui/gfx/size.h"

namespace drag_utils {

namespace {

// Maximum width of the link drag image in pixels.
static const int kLinkDragImageVPadding = 3;

// File dragging pixel measurements
static const int kFileDragImageMaxWidth = 200;
static const SkColor kFileDragImageTextColor = SK_ColorBLACK;

class FileDragImageSource : public gfx::CanvasImageSource {
 public:
  FileDragImageSource(const base::FilePath& file_name,
                      const gfx::ImageSkia& icon)
      : CanvasImageSource(CalculateSize(icon), false),
        file_name_(file_name),
        icon_(icon) {
  }

  virtual ~FileDragImageSource() {
  }

  // Overridden from gfx::CanvasImageSource.
  virtual void Draw(gfx::Canvas* canvas) OVERRIDE {
    // Set up our text portion
    ResourceBundle& rb = ResourceBundle::GetSharedInstance();
    gfx::Font font = rb.GetFont(ResourceBundle::BaseFont);

    // Paint the icon.
    canvas->DrawImageInt(icon_, (size().width() - icon_.width()) / 2, 0);

    string16 name = file_name_.BaseName().LossyDisplayName();
    const int flags = gfx::Canvas::TEXT_ALIGN_CENTER;
#if defined(OS_WIN)
    // Paint the file name. We inset it one pixel to allow room for the halo.
    canvas->DrawStringWithHalo(name, font, kFileDragImageTextColor,
                               SK_ColorWHITE, 1,
                               icon_.height() + kLinkDragImageVPadding + 1,
                               size().width() - 2, font.GetHeight(), flags);
#else
    // NO_SUBPIXEL_RENDERING is required when drawing to a non-opaque canvas.
    canvas->DrawStringInt(name, font, kFileDragImageTextColor,
                          0, icon_.height() + kLinkDragImageVPadding,
                          size().width(), font.GetHeight(),
                          flags | gfx::Canvas::NO_SUBPIXEL_RENDERING);
#endif
  }

 private:
  gfx::Size CalculateSize(const gfx::ImageSkia& icon) const {
    ResourceBundle& rb = ResourceBundle::GetSharedInstance();
    gfx::Font font = rb.GetFont(ResourceBundle::BaseFont);
    const int width = kFileDragImageMaxWidth;
    // Add +2 here to allow room for the halo.
    const int height = font.GetHeight() + icon.height() +
                       kLinkDragImageVPadding + 2;
    return gfx::Size(width, height);
  }

  const base::FilePath file_name_;
  const gfx::ImageSkia icon_;

  DISALLOW_COPY_AND_ASSIGN(FileDragImageSource);
};

}  // namespace

void CreateDragImageForFile(const base::FilePath& file_name,
                            const gfx::ImageSkia* icon,
                            ui::OSExchangeData* data_object) {
  DCHECK(icon);
  DCHECK(data_object);
  gfx::CanvasImageSource* source = new FileDragImageSource(file_name, *icon);
  gfx::Size size = source->size();
  // ImageSkia takes ownership of |source|.
  gfx::ImageSkia image = gfx::ImageSkia(source, size);

  gfx::Vector2d cursor_offset(size.width() / 2, kLinkDragImageVPadding);
  SetDragImageOnDataObject(image, size, cursor_offset, data_object);
}

void SetDragImageOnDataObject(const gfx::Canvas& canvas,
                              const gfx::Size& size,
                              const gfx::Vector2d& cursor_offset,
                              ui::OSExchangeData* data_object) {
  gfx::ImageSkia image = gfx::ImageSkia(canvas.ExtractImageRep());
  SetDragImageOnDataObject(image, size, cursor_offset, data_object);
}

}  // namespace drag_utils
