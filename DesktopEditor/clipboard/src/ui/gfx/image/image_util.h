// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_IMAGE_IMAGE_UTIL_H_
#define UI_GFX_IMAGE_IMAGE_UTIL_H_

#include <vector>

#include "base/basictypes.h"
#include "ui/base/ui_export.h"

namespace gfx {
class Image;
}

namespace gfx {

// Creates an image from the given JPEG-encoded input. If there was an error
// creating the image, returns an IsEmpty() Image.
UI_EXPORT Image ImageFrom1xJPEGEncodedData(const unsigned char* input,
                                           size_t input_size);

// Fills the |dst| vector with JPEG-encoded bytes of the 1x representation of
// the given image.
// Returns true if the image has a 1x representation and the 1x representation
// was encoded successfully.
// |quality| determines the compression level, 0 == lowest, 100 == highest.
// Returns true if the Image was encoded successfully.
UI_EXPORT bool JPEG1xEncodedDataFromImage(const Image& image,
                                          int quality,
                                          std::vector<unsigned char>* dst);

}  // namespace gfx

#endif  // UI_GFX_IMAGE_IMAGE_UTIL_H_
