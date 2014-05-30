// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_GTK_UTIL_H_
#define UI_GFX_GTK_UTIL_H_

#include <vector>

#include "base/time.h"
#include "ui/base/ui_export.h"

typedef struct _GdkPixbuf GdkPixbuf;
typedef struct _GdkRegion GdkRegion;
typedef struct _GdkCursor GdkCursor;

class CommandLine;
class SkBitmap;

namespace gfx {

class Rect;

// Call gtk_init() / gdk_init() using the argc and argv from command_line.
// These init functions want an argc and argv that they can mutate; we provide
// those, but leave the original CommandLine unaltered.
UI_EXPORT void GtkInitFromCommandLine(const CommandLine& command_line);
UI_EXPORT void GdkInitFromCommandLine(const CommandLine& command_line);

// Convert and copy a SkBitmap to a GdkPixbuf. NOTE: this uses BGRAToRGBA, so
// it is an expensive operation.  The returned GdkPixbuf will have a refcount of
// 1, and the caller is responsible for unrefing it when done.
UI_EXPORT GdkPixbuf* GdkPixbufFromSkBitmap(const SkBitmap& bitmap);

// Modify the given region by subtracting the given rectangles.
UI_EXPORT void SubtractRectanglesFromRegion(GdkRegion* region,
                                            const std::vector<Rect>& cutouts);

// Returns a static instance of a GdkCursor* object, sharable across the
// process. Caller must gdk_cursor_ref() it if they want to assume ownership.
UI_EXPORT GdkCursor* GetCursor(int type);

// Initialize some GTK settings so that our dialogs are consistent.
UI_EXPORT void InitRCStyles();

// Queries GtkSettings for the cursor blink cycle time.  Returns a 0 duration if
// blinking is disabled.
UI_EXPORT base::TimeDelta GetCursorBlinkCycle();

}  // namespace gfx

#endif  // UI_GFX_GTK_UTIL_H_
