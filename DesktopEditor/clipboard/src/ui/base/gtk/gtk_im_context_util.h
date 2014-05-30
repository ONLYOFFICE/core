// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_GTK_GTK_IM_CONTEXT_UTIL_H_
#define UI_BASE_GTK_GTK_IM_CONTEXT_UTIL_H_

#include <pango/pango-attributes.h>

#include "ui/base/ui_export.h"

namespace ui {

struct CompositionText;

// Extracts composition text information (text, underlines, selection range)
// from given Gtk preedit data (utf-8 text, pango attributes, cursor position).
UI_EXPORT void ExtractCompositionTextFromGtkPreedit(
    const gchar* utf8_text,
    PangoAttrList* attrs,
    int cursor_position,
    CompositionText* composition);

}  // namespace ui

#endif  // UI_BASE_GTK_GTK_IM_CONTEXT_UTIL_H_
