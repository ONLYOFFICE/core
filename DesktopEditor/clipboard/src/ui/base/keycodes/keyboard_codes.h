// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_KEYCODES_KEYBOARD_CODES_H_
#define UI_BASE_KEYCODES_KEYBOARD_CODES_H_

#include "build/build_config.h"

#if defined(OS_WIN)
#include "ui/base/keycodes/keyboard_codes_win.h"
#elif defined(OS_POSIX)
#include "ui/base/keycodes/keyboard_codes_posix.h"
#endif

#endif  // UI_BASE_KEYCODES_KEYBOARD_CODES_H_
