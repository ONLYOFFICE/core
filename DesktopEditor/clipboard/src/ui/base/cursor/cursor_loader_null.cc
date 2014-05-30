// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/cursor/cursor_loader_null.h"

namespace ui {

CursorLoader* CursorLoader::Create() {
  return new CursorLoaderNull;
}

CursorLoaderNull::CursorLoaderNull() {}
CursorLoaderNull::~CursorLoaderNull() {}
void CursorLoaderNull::LoadImageCursor(int id,
                                        int resource_id,
                                        const gfx::Point& hot) {}
void CursorLoaderNull::LoadAnimatedCursor(int id,
                                           int resource_id,
                                           const gfx::Point& hot,
                                           int frame_delay_ms) {}
void CursorLoaderNull::UnloadAll() {}
void CursorLoaderNull::SetPlatformCursor(gfx::NativeCursor* cursor) {}
void CursorLoaderNull::SetCursorResourceModule(const string16& module_name) {}

}  // namespace ui
