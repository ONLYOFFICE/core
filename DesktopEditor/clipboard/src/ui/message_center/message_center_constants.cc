// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/message_center_constants.h"

namespace message_center {

// Exported values /////////////////////////////////////////////////////////////

// Square image sizes in pixels.
const int kNotificationButtonIconSize = 16;
const int kNotificationIconSize = 80;
const int kNotificationPreferredImageSize = kNotificationWidth;
const int kSettingsIconSize = 16;

// Limits.
const size_t kMaxVisibleMessageCenterNotifications = 100;
const size_t kMaxVisiblePopupNotifications = 3;

// Within a notification ///////////////////////////////////////////////////////

// Pixel dimensions.
const int kControlButtonSize = 29;
const int kNotificationWidth = 360;
const int kIconToTextPadding = 16;
const int kTextTopPadding = 12;

// Text sizes.
const int kTitleFontSize = 14;
const int kTitleLineHeight = 20;
const int kMessageFontSize = 12;
const int kMessageLineHeight = 18;

// Colors.
const SkColor kNotificationBackgroundColor = SkColorSetRGB(255, 255, 255);
const SkColor kLegacyIconBackgroundColor = SkColorSetRGB(0xf5, 0xf5, 0xf5);
const SkColor kRegularTextColor = SkColorSetRGB(34, 34, 34);
const SkColor kFocusBorderColor = SkColorSetRGB(64, 128, 250);

// Limits.
const int kNotificationMaximumImageHeight = kNotificationWidth * 1.5;
const size_t kNotificationMaximumItems = 8;

// Timing.
const int kAutocloseDefaultDelaySeconds = 8;
const int kAutocloseHighPriorityDelaySeconds = 25;

// Around notifications ////////////////////////////////////////////////////////

// Pixel dimensions.
const int kMarginBetweenItems = 10;

// Colors.
const SkColor kBackgroundLightColor = SkColorSetRGB(0xf1, 0xf1, 0xf1);
const SkColor kBackgroundDarkColor = SkColorSetRGB(0xe7, 0xe7, 0xe7);

}  // namespace message_center
