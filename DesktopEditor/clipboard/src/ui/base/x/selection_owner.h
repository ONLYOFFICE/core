// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_X_SELECTION_OWNER_H_
#define UI_BASE_X_SELECTION_OWNER_H_

#include <X11/Xlib.h>

// Get rid of a macro from Xlib.h that conflicts with Aura's RootWindow class.
#undef RootWindow

#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "ui/base/ui_export.h"
#include "ui/base/x/x11_atom_cache.h"

namespace ui {

class SelectionFormatMap;

// Owns a specific X11 selection on an X window.
//
// The selection owner object keeps track of which xwindow is the current
// owner, and when its |xwindow_|, offers different data types to other
// processes.
class UI_EXPORT SelectionOwner {
 public:
  SelectionOwner(Display* xdisplay,
                 ::Window xwindow,
                 ::Atom selection_name);
  ~SelectionOwner();

  // Returns the current selection data. Useful for fast paths.
  SelectionFormatMap* selection_format_map() { return selection_data_.get(); }

  // Attempts to take ownership of the selection. If we're successful, present
  // |data| to other windows.
  void TakeOwnershipOfSelection(scoped_ptr<SelectionFormatMap> data);

  // Releases the selection (if we own it) and clears any data we own.
  void Clear();

  // It is our owner's responsibility to plumb X11 events on |xwindow_| to us.
  void OnSelectionRequest(const XSelectionRequestEvent& event);
  void OnSelectionClear(const XSelectionClearEvent& event);
  // TODO(erg): Do we also need to follow PropertyNotify events? We currently
  // don't, but there were open todos in the previous implementation.

 private:
  // Our X11 state.
  Display* x_display_;
  ::Window x_window_;

  // The X11 selection that this instance communicates on.
  ::Atom selection_name_;

  // The data we are currently serving.
  scoped_ptr<SelectionFormatMap> selection_data_;

  X11AtomCache atom_cache_;

  DISALLOW_COPY_AND_ASSIGN(SelectionOwner);
};

}  // namespace ui

#endif  // UI_BASE_X_SELECTION_OWNER_H_
