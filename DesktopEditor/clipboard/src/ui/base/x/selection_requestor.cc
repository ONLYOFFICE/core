// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/x/selection_requestor.h"

#include "base/message_pump_aurax11.h"
#include "base/run_loop.h"
#include "ui/base/x/selection_utils.h"

namespace ui {

namespace {

const char kChromeSelection[] = "CHROME_SELECTION";

const char* kAtomsToCache[] = {
  kChromeSelection,
  NULL
};

}  // namespace

SelectionRequestor::SelectionRequestor(Display* x_display,
                                       Window x_window,
                                       Atom selection_name)
    : x_display_(x_display),
      x_window_(x_window),
      in_nested_loop_(false),
      selection_name_(selection_name),
      current_target_(None),
      returned_property_(None),
      atom_cache_(x_display_, kAtomsToCache) {
}

SelectionRequestor::~SelectionRequestor() {}

bool SelectionRequestor::PerformBlockingConvertSelection(
    Atom target,
    unsigned char** out_data,
    size_t* out_data_bytes,
    size_t* out_data_items,
    Atom* out_type) {
  // The name of the property we're asking to be set on |x_window_|.
  Atom property_to_set = atom_cache_.GetAtom(kChromeSelection);

  XConvertSelection(x_display_,
                    selection_name_,
                    target,
                    property_to_set,
                    x_window_,
                    CurrentTime);

  // Now that we've thrown our message off to the X11 server, we block waiting
  // for a response.
  base::MessageLoopForUI* loop = base::MessageLoopForUI::current();
  base::MessageLoop::ScopedNestableTaskAllower allow_nested(loop);
  base::RunLoop run_loop(base::MessagePumpAuraX11::Current());

  current_target_ = target;
  in_nested_loop_ = true;
  quit_closure_ = run_loop.QuitClosure();
  run_loop.Run();
  in_nested_loop_ = false;
  current_target_ = None;

  if (returned_property_ != property_to_set)
    return false;

  // Retrieve the data from our window.
  unsigned long nitems = 0;
  unsigned long nbytes = 0;
  Atom prop_type = None;
  int prop_format = 0;
  unsigned char* property_data = NULL;
  if (XGetWindowProperty(x_display_,
                         x_window_,
                         returned_property_,
                         0, 0x1FFFFFFF /* MAXINT32 / 4 */, False,
                         AnyPropertyType, &prop_type, &prop_format,
                         &nitems, &nbytes, &property_data) != Success) {
    return false;
  }

  if (prop_type == None)
    return false;

  if (out_data)
    *out_data = property_data;

  if (out_data_bytes) {
    // So even though we should theoretically have nbytes (and we can't
    // pass NULL there), we need to manually calculate the byte length here
    // because nbytes always returns zero.
    switch (prop_format) {
      case 8:
        *out_data_bytes = nitems;
        break;
      case 16:
        *out_data_bytes = sizeof(short) * nitems;
        break;
      case 32:
        *out_data_bytes = sizeof(long) * nitems;
        break;
      default:
        NOTREACHED();
        break;
    }
  }

  if (out_data_items)
    *out_data_items = nitems;

  if (out_type)
    *out_type = prop_type;

  return true;
}

scoped_ptr<SelectionData> SelectionRequestor::RequestAndWaitForTypes(
    const std::vector< ::Atom>& types) {
  for (std::vector< ::Atom>::const_iterator it = types.begin();
       it != types.end(); ++it) {
    unsigned char* data = NULL;
    size_t data_bytes = 0;
    ::Atom type = None;
    if (PerformBlockingConvertSelection(*it,
                                        &data,
                                        &data_bytes,
                                        NULL,
                                        &type) &&
        type == *it) {
      scoped_ptr<SelectionData> data_out(new SelectionData(x_display_));
      data_out->Set(type, (char*)data, data_bytes, true);
      return data_out.Pass();
    }
  }

  return scoped_ptr<SelectionData>();
}

void SelectionRequestor::OnSelectionNotify(const XSelectionEvent& event) {
  if (!in_nested_loop_) {
    // This shouldn't happen; we're not waiting on the X server for data, but
    // any client can send any message...
    return;
  }

  if (selection_name_ == event.selection &&
      current_target_ == event.target) {
    returned_property_ = event.property;
  } else {
    // I am assuming that if some other client sent us a message after we've
    // asked for data, but it's malformed, we should just treat as if they sent
    // us an error message.
    returned_property_ = None;
  }

  quit_closure_.Run();
}

}  // namespace ui
