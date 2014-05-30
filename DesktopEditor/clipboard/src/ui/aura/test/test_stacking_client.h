// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_TEST_TEST_STACKING_CLIENT_H_
#define UI_AURA_TEST_TEST_STACKING_CLIENT_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/aura/aura_export.h"
#include "ui/aura/client/stacking_client.h"

namespace aura {
class RootWindow;
namespace test {

class TestStackingClient : public client::StackingClient {
 public:
  explicit TestStackingClient(RootWindow* root_window);
  virtual ~TestStackingClient();

  // Overridden from client::StackingClient:
  virtual Window* GetDefaultParent(Window* context,
                                   Window* window,
                                   const gfx::Rect& bounds) OVERRIDE;

 private:
  RootWindow* root_window_;

  DISALLOW_COPY_AND_ASSIGN(TestStackingClient);
};

}  // namespace test
}  // namespace aura

#endif  // UI_AURA_TEST_TEST_STACKING_CLIENT_H_
