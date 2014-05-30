// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_LINUX_UI_LINUX_UI_EXPORT_H_
#define UI_LINUX_UI_LINUX_UI_EXPORT_H_

// Defines LINUX_UI_EXPORT so that functionality implemented by the
// native_theme library can be exported to consumers.

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(LINUX_UI_IMPLEMENTATION)
#define LINUX_UI_EXPORT __declspec(dllexport)
#else
#define LINUX_UI_EXPORT __declspec(dllimport)
#endif  // defined(LINUX_UI_IMPLEMENTATION)

#else  // !defined(WIN32)

#if defined(LINUX_UI_IMPLEMENTATION)
#define LINUX_UI_EXPORT __attribute__((visibility("default")))
#else
#define LINUX_UI_EXPORT
#endif

#endif  // defined(WIN32)

#else  // !defined(COMPONENT_BUILD)

#define LINUX_UI_EXPORT

#endif  // defined(COMPONENT_BUILD)

#endif  // UI_LINUX_UI_LINUX_UI_EXPORT_H_
