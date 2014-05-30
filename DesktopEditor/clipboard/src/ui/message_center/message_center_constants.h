// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_MESSAGE_CENTER_MESSAGE_CENTER_CONSTANTS_H_
#define UI_MESSAGE_CENTER_MESSAGE_CENTER_CONSTANTS_H_

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/message_center/message_center_export.h"

namespace message_center {

// Exported values /////////////////////////////////////////////////////////////

// Square image sizes in pixels.
MESSAGE_CENTER_EXPORT extern const int kNotificationButtonIconSize;
MESSAGE_CENTER_EXPORT extern const int kNotificationIconSize;
MESSAGE_CENTER_EXPORT extern const int kNotificationPreferredImageSize;
MESSAGE_CENTER_EXPORT extern const int kSettingsIconSize;

// Limits.
MESSAGE_CENTER_EXPORT extern const size_t kMaxVisiblePopupNotifications;
MESSAGE_CENTER_EXPORT extern const size_t kMaxVisibleMessageCenterNotifications;

// Within a notification ///////////////////////////////////////////////////////

// Pixel dimensions (H = horizontal, V = vertical).
extern const int kControlButtonSize;  // Square size of close & expand buttons.
extern const int kNotificationWidth;  // H size of the whole card.
extern const int kIconToTextPadding;  // H space between icon & title/message.
extern const int kTextTopPadding;     // V space between text elements.

// Text sizes.
extern const int kTitleFontSize;      // For title only.
extern const int kTitleLineHeight;    // In pixels.
extern const int kMessageFontSize;    // For everything but title.
extern const int kMessageLineHeight;  // In pixels.

// Colors.
extern const SkColor kNotificationBackgroundColor; // Background of the card.
extern const SkColor kLegacyIconBackgroundColor;   // Used behind icons smaller.
                                                   // than the icon view.
extern const SkColor kRegularTextColor;            // Title, message, ...
extern const SkColor kFocusBorderColor;            // The focus border.

// Limits.
extern const int kNotificationMaximumImageHeight;  // For image notifications.
extern const size_t kNotificationMaximumItems;     // For list notifications.

// Timing.
extern const int kAutocloseDefaultDelaySeconds;
extern const int kAutocloseHighPriorityDelaySeconds;

// Around notifications ////////////////////////////////////////////////////////

// Pixel dimensions (H = horizontal, V = vertical).
MESSAGE_CENTER_EXPORT extern const int kMarginBetweenItems; // H & V space
                                                            // around & between
                                                            // notifications.

// Colors.
extern const SkColor kBackgroundLightColor;  // Behind notifications, gradient
extern const SkColor kBackgroundDarkColor;   // from light to dark.

}  // namespace message_center

#endif  // UI_MESSAGE_CENTER_MESSAGE_CENTER_CONSTANTS_H_
