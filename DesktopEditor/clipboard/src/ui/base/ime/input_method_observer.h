// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_OBSERVER_H_
#define UI_BASE_IME_INPUT_METHOD_OBSERVER_H_

#include "ui/base/ui_export.h"

namespace ui {

class InputMethod;
class TextInputClient;

class UI_EXPORT InputMethodObserver {
 public:
  virtual ~InputMethodObserver() {}

  // Called when either:
  //  - the TextInputClient is changed (e.g. by a change of focus)
  //  - the TextInputType of the TextInputClient changes
  virtual void OnTextInputStateChanged(const TextInputClient* client) = 0;

  // Called when the observed InputMethod is being destroyed.
  virtual void OnInputMethodDestroyed(const InputMethod* input_method) = 0;
};

}  // namespace ui

#endif  // UI_BASE_IME_INPUT_METHOD_OBSERVER_H_
