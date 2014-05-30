// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_OAK_OAK_AURA_WINDOW_DISPLAY_H_
#define UI_OAK_OAK_AURA_WINDOW_DISPLAY_H_

#include "base/compiler_specific.h"
#include "ui/oak/oak_details_model.h"

namespace oak {
namespace internal {

class OakAuraWindowDisplay : public OakDetailsModel {
 public:
  OakAuraWindowDisplay();
  virtual ~OakAuraWindowDisplay();

 private:
  // Overridden from OakDetailsModel:
  virtual void SetValue(aura::Window* window) OVERRIDE;

  // Overridden from ui::TableModel:
  virtual int RowCount() OVERRIDE;
  virtual base::string16 GetText(int row, int column_id) OVERRIDE;
  virtual void SetObserver(ui::TableModelObserver* observer) OVERRIDE;

  ui::TableModelObserver* observer_;
  aura::Window* window_;

  DISALLOW_COPY_AND_ASSIGN(OakAuraWindowDisplay);
};

}  // namespace internal
}  // namespace oak

#endif  // UI_OAK_OAK_AURA_WINDOW_DISPLAY_H_
