// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_OAK_OAK_DETAILS_MODEL_H_
#define UI_OAK_OAK_DETAILS_MODEL_H_

#include "ui/base/models/table_model.h"

namespace aura {
class Window;
}

namespace oak {
namespace internal {

class OakDetailsModel : public ui::TableModel {
 public:
  virtual ~OakDetailsModel() {}

  virtual void SetValue(aura::Window* window) = 0;

 protected:
  OakDetailsModel() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(OakDetailsModel);
};

}  // namespace internal
}  // namespace oak

#endif  // UI_OAK_OAK_DETAILS_MODEL_H_
