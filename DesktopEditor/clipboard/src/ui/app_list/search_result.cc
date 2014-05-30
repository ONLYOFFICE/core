// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/search_result.h"

#include "ui/app_list/search_result_observer.h"

namespace app_list {

SearchResult::ActionIconSet::ActionIconSet(const gfx::ImageSkia& base_image,
                                           const gfx::ImageSkia& hover_image,
                                           const gfx::ImageSkia& pressed_image,
                                           const string16& tooltip_text)
    : base_image(base_image),
      hover_image(hover_image),
      pressed_image(pressed_image),
      tooltip_text(tooltip_text) {
}

SearchResult::ActionIconSet::~ActionIconSet() {
}

SearchResult::SearchResult() {
}

SearchResult::~SearchResult() {
}

void SearchResult::SetIcon(const gfx::ImageSkia& icon) {
  icon_ = icon;
  FOR_EACH_OBSERVER(SearchResultObserver,
                    observers_,
                    OnIconChanged());
}

void SearchResult::SetActionIcons(const std::vector<ActionIconSet>& sets) {
  action_icons_ = sets;
  FOR_EACH_OBSERVER(SearchResultObserver,
                    observers_,
                    OnActionIconsChanged());
}

void SearchResult::AddObserver(SearchResultObserver* observer) {
  observers_.AddObserver(observer);
}

void SearchResult::RemoveObserver(SearchResultObserver* observer) {
  observers_.RemoveObserver(observer);
}

}  // namespace app_list
