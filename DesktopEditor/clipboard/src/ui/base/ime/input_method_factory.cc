// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/ime/input_method_factory.h"

#include "ui/base/ime/input_method_delegate.h"
#include "ui/base/ime/mock_input_method.h"

#if defined(OS_CHROMEOS) && defined(USE_X11)
#include "ui/base/ime/input_method_ibus.h"
#elif defined(OS_WIN)
#include "ui/base/ime/input_method_win.h"
#else
#include "ui/base/ime/fake_input_method.h"
#endif

namespace ui {

namespace {
static bool g_input_method_set_for_testing = false;
}

InputMethod* CreateInputMethod(internal::InputMethodDelegate* delegate,
                               gfx::AcceleratedWidget widget) {
  if (g_input_method_set_for_testing)
    return new MockInputMethod(delegate);
#if defined(OS_CHROMEOS) && defined(USE_X11)
  return new InputMethodIBus(delegate);
#elif defined(OS_WIN)
  return new InputMethodWin(delegate, widget);
#else
  return new FakeInputMethod(delegate);
#endif
}

void SetUpInputMethodFactoryForTesting() {
  g_input_method_set_for_testing = true;
}

}  // namespace ui
