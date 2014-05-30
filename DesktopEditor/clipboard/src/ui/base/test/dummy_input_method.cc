// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/test/dummy_input_method.h"

namespace ui {
namespace test {

DummyInputMethod::DummyInputMethod() {
}

DummyInputMethod::~DummyInputMethod() {
}

void DummyInputMethod::SetDelegate(internal::InputMethodDelegate* delegate) {
}

void DummyInputMethod::Init(bool focused) {
}

void DummyInputMethod::OnFocus() {
}

void DummyInputMethod::OnBlur() {
}

void DummyInputMethod::SetFocusedTextInputClient(TextInputClient* client) {
}

TextInputClient* DummyInputMethod::GetTextInputClient() const {
  return NULL;
}

bool DummyInputMethod::DispatchKeyEvent(const base::NativeEvent& event) {
  return false;
}

bool DummyInputMethod::DispatchFabricatedKeyEvent(const ui::KeyEvent& event) {
  return false;
}

void DummyInputMethod::OnTextInputTypeChanged(const TextInputClient* client) {
}

void DummyInputMethod::OnCaretBoundsChanged(const TextInputClient* client) {
}

void DummyInputMethod::CancelComposition(const TextInputClient* client) {
}

std::string DummyInputMethod::GetInputLocale() {
  return std::string();
}

base::i18n::TextDirection DummyInputMethod::GetInputTextDirection() {
  return base::i18n::UNKNOWN_DIRECTION;
}

bool DummyInputMethod::IsActive() {
  return true;
}

TextInputType DummyInputMethod::GetTextInputType() const {
  return TEXT_INPUT_TYPE_NONE;
}

bool DummyInputMethod::CanComposeInline() const {
  return true;
}

void DummyInputMethod::AddObserver(InputMethodObserver* observer) {
}

void DummyInputMethod::RemoveObserver(InputMethodObserver* observer) {
}

}  // namespace test
}  // namespace ui

