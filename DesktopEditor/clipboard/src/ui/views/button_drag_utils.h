// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_BUTTON_DRAG_UTILS_H_
#define UI_VIEWS_BUTTON_DRAG_UTILS_H_

#include "base/string16.h"
#include "ui/views/views_export.h"

class GURL;

namespace gfx {
class ImageSkia;
}

namespace ui {
class OSExchangeData;
}

namespace views {
class Widget;
}

namespace button_drag_utils {

// Sets url and title on data as well as setting a suitable image for dragging.
// The image looks like that of the bookmark buttons.
VIEWS_EXPORT void SetURLAndDragImage(const GURL& url,
                                     const string16& title,
                                     const gfx::ImageSkia& icon,
                                     ui::OSExchangeData* data,
                                     views::Widget* widget);

}  // namespace drag_utils

#endif  // UI_VIEWS_BUTTON_DRAG_UTILS_H_
