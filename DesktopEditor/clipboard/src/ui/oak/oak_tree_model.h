// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_OAK_OAK_TREE_MODEL_H_
#define UI_OAK_OAK_TREE_MODEL_H_

#include "base/compiler_specific.h"
#include "ui/base/models/tree_node_model.h"

namespace aura {
class Window;
}

namespace oak {
namespace internal {

typedef ui::TreeNodeWithValue<aura::Window*> WindowNode;
typedef ui::TreeNodeModel<WindowNode> TreeOfWindows;

TreeOfWindows* GenerateModel(aura::Window* root);

}  // namespace internal
}  // namespace oak

#endif  // UI_OAK_OAK_TREE_MODEL_H_
