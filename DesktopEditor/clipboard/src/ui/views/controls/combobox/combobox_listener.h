// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_COMBOBOX_COMBOBOX_LISTENER_H_
#define UI_VIEWS_CONTROLS_COMBOBOX_COMBOBOX_LISTENER_H_

namespace views {

class Combobox;

// An interface implemented by an object to let it know that the selected index
// has changed.
class ComboboxListener {
 public:
  virtual void OnSelectedIndexChanged(Combobox* combobox) = 0;

 protected:
  virtual ~ComboboxListener() {}
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_COMBOBOX_COMBOBOX_LISTENER_H_
