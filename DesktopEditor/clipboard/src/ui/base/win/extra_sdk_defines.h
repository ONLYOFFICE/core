// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WIN_EXTRA_SDK_DEFINES_H_
#define UI_BASE_WIN_EXTRA_SDK_DEFINES_H_

#include <windows.h>
#include <winuser.h>

// TODO(tommi): These should be removed once we switch over to version 8 of the
// Windows SDK.
#if !defined(WM_POINTERDOWN)
#define WM_POINTERDOWN 0x0246
#endif  // WM_POINTERDOWN

#if !defined(WM_POINTERUP)
#define WM_POINTERUP 0x0247
#endif  // WM_POINTERUP

#ifndef POINTER_MESSAGE_FLAG_FIRSTBUTTON
#define POINTER_MESSAGE_FLAG_FIRSTBUTTON 0x00000010
#endif  // POINTER_MESSAGE_FLAG_FIRSTBUTTON

#ifndef IS_POINTER_FLAG_SET_WPARAM
#define IS_POINTER_FLAG_SET_WPARAM(wParam, flag) \
    (((DWORD)HIWORD(wParam) & (flag)) == (flag))
#endif  // IS_POINTER_FLAG_SET_WPARAM

#ifndef IS_POINTER_FIRSTBUTTON_WPARAM
#define IS_POINTER_FIRSTBUTTON_WPARAM(wParam) \
    IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_FIRSTBUTTON)
#endif  // IS_POINTER_FIRSTBUTTON_WPARAM

#if !defined(WM_POINTERUPDATE)
#define WM_POINTERUPDATE 0x0245
#endif  // WM_POINTERUPDATE

#endif  // UI_BASE_WIN_EXTRA_SDK_DEFINES_H_
