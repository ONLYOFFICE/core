// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/menu/menu_config.h"

#include "build/build_config.h"
#include "ui/base/layout.h"
#include "ui/native_theme/native_theme.h"

namespace views {

MenuConfig::MenuConfig(const ui::NativeTheme* theme)
    : text_color(SK_ColorBLACK),
      arrow_color(SK_ColorBLACK),
      menu_vertical_border_size(7),
      menu_horizontal_border_size(0),
      submenu_horizontal_inset(3),
      item_top_margin(7),
      item_bottom_margin(6),
      item_no_icon_top_margin(7),
      item_no_icon_bottom_margin(7),
      item_left_margin(10),
      label_to_arrow_padding(10),
      arrow_to_edge_padding(5),
      icon_to_label_padding(10),
      gutter_to_label(5),
      check_width(16),
      check_height(16),
      radio_width(16),
      radio_height(16),
      arrow_height(9),
      arrow_width(9),
      gutter_width(0),
      separator_height(15),
      separator_upper_height(3),
      separator_lower_height(4),
      separator_spacing_height(3),
      render_gutter(false),
      show_mnemonics(false),
      scroll_arrow_height(3),
      label_to_accelerator_padding(10),
      item_min_height(0),
      show_accelerators(true),
      always_use_icon_to_label_padding(false),
      align_arrow_and_shortcut(false),
      offset_context_menus(false),
      native_theme(theme),
      show_delay(400),
      corner_radius(0) {
  AdjustForMenuVariations();
  // Use 40px tall menu items when running in touch optimized mode.
  // For Windows use 40px tall menu items when running in touch optimized mode.
  if (ui::GetDisplayLayout() == ui::LAYOUT_TOUCH) {
    item_top_margin = item_no_icon_top_margin = 12;
    item_bottom_margin = item_no_icon_bottom_margin = 13;
  }
  Init(theme);
}

MenuConfig::~MenuConfig() {}

void MenuConfig::AdjustForMenuVariations() {
  switch (ui::NativeTheme::GetMenuVariation()) {
    case ui::NativeTheme::MENU_VARIATION_COMPACT_1:
    case ui::NativeTheme::MENU_VARIATION_CONTRAST:
      item_top_margin = 5;
      item_bottom_margin = 4;
      item_no_icon_top_margin = 5;
      item_no_icon_bottom_margin = 5;
      separator_height = 11;
      menu_vertical_border_size = 5;
      break;
    case ui::NativeTheme::MENU_VARIATION_COMPACT_2:
      item_top_margin = 4;
      item_bottom_margin = 3;
      item_no_icon_top_margin = 4;
      item_no_icon_bottom_margin = 4;
      separator_height = 11;
      menu_vertical_border_size = 3;
      break;
    default:
      break;
  }
}

}  // namespace views
