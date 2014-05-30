// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/x/selection_owner.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "base/logging.h"
#include "ui/base/x/selection_utils.h"

namespace ui {

namespace {

const char kMultiple[] = "MULTIPLE";
const char kTargets[] = "TARGETS";

const char* kAtomsToCache[] = {
  kMultiple,
  kTargets,
  NULL
};

}  // namespace

SelectionOwner::SelectionOwner(Display* x_display,
                               Window x_window,
                               Atom selection_name)
    : x_display_(x_display),
      x_window_(x_window),
      selection_name_(selection_name),
      atom_cache_(x_display_, kAtomsToCache) {
}

SelectionOwner::~SelectionOwner() {
}

void SelectionOwner::TakeOwnershipOfSelection(
    scoped_ptr<SelectionFormatMap> data) {
  XSetSelectionOwner(x_display_, selection_name_, x_window_, CurrentTime);

  if (XGetSelectionOwner(x_display_, selection_name_) == x_window_) {
    // The X server agrees that we are the selection owner. Commit our data.
    selection_data_ = data.Pass();
  }
}

void SelectionOwner::Clear() {
  if (XGetSelectionOwner(x_display_, selection_name_) == x_window_)
    XSetSelectionOwner(x_display_, selection_name_, None, CurrentTime);

  selection_data_.reset();
}

void SelectionOwner::OnSelectionRequest(const XSelectionRequestEvent& event) {
  // Incrementally build our selection. By default this is a refusal, and we'll
  // override the parts indicating success in the different cases.
  XEvent reply;
  reply.xselection.type = SelectionNotify;
  reply.xselection.requestor = event.requestor;
  reply.xselection.selection = event.selection;
  reply.xselection.target = event.target;
  reply.xselection.property = None;  // Indicates failure
  reply.xselection.time = event.time;

  // Get the proper selection.
  if (selection_data_.get()) {
    Atom targets_atom = atom_cache_.GetAtom(kTargets);
    if (event.target == targets_atom) {
      // We have been asked for TARGETS. Send an atom array back with the data
      // types we support.
      std::vector<Atom> targets;
      targets.push_back(targets_atom);
      for (SelectionFormatMap::const_iterator it = selection_data_->begin();
           it != selection_data_->end(); ++it) {
        targets.push_back(it->first);
      }

      XChangeProperty(x_display_, event.requestor, event.property, XA_ATOM, 32,
                      PropModeReplace,
                      reinterpret_cast<unsigned char*>(&targets.front()),
                      targets.size());
      reply.xselection.property = event.property;
    } else if (event.target == atom_cache_.GetAtom(kMultiple)) {
      // TODO(erg): Theoretically, the spec claims I'm supposed to handle the
      // MULTIPLE case, but I haven't seen it in the wild yet.
      NOTIMPLEMENTED();
    } else {
      // Try to find the data type in map.
      SelectionFormatMap::const_iterator it =
          selection_data_->find(event.target);
      if (it != selection_data_->end()) {
        XChangeProperty(x_display_, event.requestor, event.property,
                        event.target, 8,
                        PropModeReplace,
                        reinterpret_cast<unsigned char*>(it->second.first),
                        it->second.second);
        reply.xselection.property = event.property;
      }
      // I would put error logging here, but GTK ignores TARGETS and spams us
      // looking for its own internal types.
    }
  } else {
    DLOG(ERROR) << "XWindow " << x_window_ << " received a SelectionRequest "
                << "message when we don't have data to offer.";
  }

  // Send off the reply.
  XSendEvent(x_display_, event.requestor, False, 0, &reply);
}

void SelectionOwner::OnSelectionClear(const XSelectionClearEvent& event) {
  DLOG(ERROR) << "SelectionClear";

  // TODO(erg): If we receive a SelectionClear event while we're handling data,
  // we need to delay clearing.
}

}  // namespace ui

