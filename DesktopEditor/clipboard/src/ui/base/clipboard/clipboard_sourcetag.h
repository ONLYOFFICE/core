// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_CLIPBOARD_CLIPBOARD_SOURCETAG_H_
#define UI_BASE_CLIPBOARD_CLIPBOARD_SOURCETAG_H_

namespace ui {

// Clipboard::WriteObject() caller can use the SourceTag that will be stored in
// the clipboard. NULL value means "no tag".
typedef void* SourceTag;

}  // namespace ui

#endif  // UI_BASE_CLIPBOARD_CLIPBOARD_SOURCETAG_H_
