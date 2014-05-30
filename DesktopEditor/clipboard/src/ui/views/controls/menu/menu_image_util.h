// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_MENU_MENU_IMAGE_UTIL_H_
#define UI_VIEWS_CONTROLS_MENU_MENU_IMAGE_UTIL_H_

namespace gfx {
class ImageSkia;
}

namespace views {

// Returns the Menu Check box image (always checked).
// The returned image is global object and should not be freed.
const gfx::ImageSkia* GetMenuCheckImage();

// Return the RadioButton image for given state.
// It returns the "selected" image when |selected| is
// true, or the "unselected" image if false.
// The returned image is global object and should not be freed.
const gfx::ImageSkia* GetRadioButtonImage(bool selected);

// Returns the image for submenu arrow for current RTL setting.
const gfx::ImageSkia* GetSubmenuArrowImage();

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_MENU_MENU_IMAGE_UTIL_H_
