// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/aura/test/test_windows.h"

#include "base/string_number_conversions.h"
#include "ui/aura/client/aura_constants.h"
#include "ui/aura/window.h"
#include "ui/compositor/layer.h"
#include "ui/gfx/rect.h"

namespace aura {
namespace test {

Window* CreateTestWindowWithId(int id, Window* parent) {
  return CreateTestWindowWithDelegate(NULL, id, gfx::Rect(), parent);
}

Window* CreateTestWindowWithBounds(const gfx::Rect& bounds, Window* parent) {
  return CreateTestWindowWithDelegate(NULL, 0, bounds, parent);
}

Window* CreateTestWindow(SkColor color,
                         int id,
                         const gfx::Rect& bounds,
                         Window* parent) {
  return CreateTestWindowWithDelegate(new ColorTestWindowDelegate(color),
                                      id, bounds, parent);
}

Window* CreateTestWindowWithDelegate(WindowDelegate* delegate,
                                     int id,
                                     const gfx::Rect& bounds,
                                     Window* parent) {
  return CreateTestWindowWithDelegateAndType(
      delegate,
      aura::client::WINDOW_TYPE_NORMAL,
      id,
      bounds,
      parent);
}

Window* CreateTestWindowWithDelegateAndType(WindowDelegate* delegate,
                                            client::WindowType type,
                                            int id,
                                            const gfx::Rect& bounds,
                                            Window* parent) {
  Window* window = new Window(delegate);
  window->set_id(id);
  window->SetType(type);
  window->Init(ui::LAYER_TEXTURED);
  window->SetBounds(bounds);
  window->Show();
  if (parent)
    parent->AddChild(window);
  window->SetProperty(aura::client::kCanMaximizeKey, true);
  return window;
}

template <typename T>
bool ObjectIsAbove(T* upper, T* lower) {
  DCHECK_EQ(upper->parent(), lower->parent());
  DCHECK_NE(upper, lower);
  const std::vector<T*>& children = upper->parent()->children();
  const size_t upper_i =
      std::find(children.begin(), children.end(), upper) - children.begin();
  const size_t lower_i =
      std::find(children.begin(), children.end(), lower) - children.begin();
  return upper_i > lower_i;
}

bool WindowIsAbove(Window* upper, Window* lower) {
  return ObjectIsAbove<Window>(upper, lower);
}

bool LayerIsAbove(Window* upper, Window* lower) {
  return ObjectIsAbove<ui::Layer>(upper->layer(), lower->layer());
}

std::string ChildWindowIDsAsString(aura::Window* parent) {
  std::string result;
  for (Window::Windows::const_iterator i = parent->children().begin();
       i != parent->children().end(); ++i) {
    if (!result.empty())
      result += " ";
    result += base::IntToString((*i)->id());
  }
  return result;
}

}  // namespace test
}  // namespace aura
