// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/oak/oak_tree_model.h"

#include "base/utf_string_conversions.h"
#include "ui/aura/root_window.h"
#include "ui/aura/window.h"
#include "ui/base/models/tree_node_model.h"

namespace oak {
namespace internal {

base::string16 GetNodeTitleForWindow(aura::Window* window) {
  std::string window_name = window->name();
  if (window_name.empty())
    window_name.append("Unnamed window");
  return ASCIIToUTF16(window_name);
}

void AddChildWindows(aura::Window* parent_window, WindowNode* parent_node) {
  aura::Window::Windows::const_iterator it = parent_window->children().begin();
  for (; it != parent_window->children().end(); ++it) {
    WindowNode* child_node = new WindowNode(GetNodeTitleForWindow(*it), *it);
    parent_node->Add(child_node, parent_node->child_count());
    AddChildWindows(*it, child_node);
  }
}

TreeOfWindows* GenerateModel(aura::Window* root) {
  WindowNode* root_node = new WindowNode(GetNodeTitleForWindow(root), root);
  TreeOfWindows* tree = new TreeOfWindows(root_node);
  AddChildWindows(root, root_node);
  return tree;
}

}  // namespace internal
}  // namespace oak
