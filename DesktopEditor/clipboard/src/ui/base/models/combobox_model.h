// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_MODELS_COMBOBOX_MODEL_H_
#define UI_BASE_MODELS_COMBOBOX_MODEL_H_

#include "base/string16.h"
#include "ui/base/ui_export.h"

namespace ui {

// A data model for a combo box.
class UI_EXPORT ComboboxModel {
 public:
  // Returns the number of items in the combo box.
  virtual int GetItemCount() const = 0;

  // Returns the string at the specified index.
  virtual string16 GetItemAt(int index) = 0;

  // Should return true if the item at |index| is a non-selectable separator
  // item.
  virtual bool IsItemSeparatorAt(int index);

  // The index of the item that is selected by default (before user
  // interaction).
  virtual int GetDefaultIndex() const;

 protected:
  virtual ~ComboboxModel() {}
};

}  // namespace ui

#endif  // UI_BASE_MODELS_COMBOBOX_MODEL_H_
