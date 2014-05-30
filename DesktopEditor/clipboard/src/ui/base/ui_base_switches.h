// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Defines all the command-line switches used by ui/base.

#ifndef UI_BASE_UI_BASE_SWITCHES_H_
#define UI_BASE_UI_BASE_SWITCHES_H_

#include "base/compiler_specific.h"
#include "ui/base/ui_export.h"

namespace switches {

UI_EXPORT extern const char kDisableDwmComposition[];
UI_EXPORT extern const char kDisableTouchAdjustment[];
UI_EXPORT extern const char kDisableTouchDragDrop[];
UI_EXPORT extern const char kDisableTouchEditing[];
UI_EXPORT extern const char kDisableViewsTextfield[];
UI_EXPORT extern const char kEnableBezelTouch[];
UI_EXPORT extern const char kEnableNewDialogStyle[];
UI_EXPORT extern const char kEnableTouchDragDrop[];
UI_EXPORT extern const char kEnableTouchEditing[];
UI_EXPORT extern const char kEnableViewsTextfield[];
UI_EXPORT extern const char kForceDeviceScaleFactor[];
UI_EXPORT extern const char kHighlightMissingScaledResources[];
UI_EXPORT extern const char kLang[];
UI_EXPORT extern const char kLocalePak[];
UI_EXPORT extern const char kNoMessageBox[];
UI_EXPORT extern const char kTouchEvents[];
UI_EXPORT extern const char kTouchEventsAuto[];
UI_EXPORT extern const char kTouchEventsDisabled[];
UI_EXPORT extern const char kTouchEventsEnabled[];
UI_EXPORT extern const char kTouchOptimizedUI[];
UI_EXPORT extern const char kTouchOptimizedUIAuto[];
UI_EXPORT extern const char kTouchOptimizedUIDisabled[];
UI_EXPORT extern const char kTouchOptimizedUIEnabled[];

#if defined(USE_XI2_MT)
UI_EXPORT extern const char kTouchCalibration[];
#endif

#if defined(OS_MACOSX)
// This isn't really the right place for this switch, but is the most
// convenient place where it can be shared between
// src/webkit/plugins/npapi/ and src/content/plugin/ .
UI_EXPORT extern const char kDisableCoreAnimationPlugins[];
#endif

#if defined(TOOLKIT_VIEWS) && defined(OS_LINUX)
UI_EXPORT extern const char kTouchDevices[];
#endif

}  // namespace switches

#endif  // UI_BASE_UI_BASE_SWITCHES_H_
