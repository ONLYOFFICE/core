// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_FACTORY_H_
#define UI_BASE_IME_INPUT_METHOD_FACTORY_H_

#include "ui/base/ui_export.h"
#include "ui/gfx/native_widget_types.h"

namespace ui {

class InputMethod;

namespace internal {
class InputMethodDelegate;
}  // namespace internal

// Creates and returns an input method implementation for the platform. Caller
// must delete the object. The object does not own |delegate|.
UI_EXPORT InputMethod* CreateInputMethod(
    internal::InputMethodDelegate* delegate,
    gfx::AcceleratedWidget widget);

// With calling this function, CreateInputMethod will return MockInputMethod.
UI_EXPORT void SetUpInputMethodFactoryForTesting();

}  // namespace ui;

#endif  // UI_BASE_IME_INPUT_METHOD_FACTORY_H_
