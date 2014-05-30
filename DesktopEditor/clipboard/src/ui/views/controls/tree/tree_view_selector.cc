// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/tree/tree_view_selector.h"

#include "base/i18n/case_conversion.h"
#include "ui/base/ime/text_input_type.h"
#include "ui/base/range/range.h"
#include "ui/views/controls/tree/tree_view.h"

namespace views {

namespace {

const int64 kTimeBeforeClearingMS = 1000;

void ConvertRectToScreen(const views::View* src, gfx::Rect* r) {
  DCHECK(src);

  gfx::Point new_origin = r->origin();
  views::View::ConvertPointToScreen(src, &new_origin);
  r->set_origin(new_origin);
}

}  // namespace

TreeViewSelector::TreeViewSelector(TreeView* tree)
    : tree_(tree) {
}

TreeViewSelector::~TreeViewSelector() {
}

void TreeViewSelector::OnTreeViewBlur() {
  ClearText();
}

void TreeViewSelector::SetCompositionText(
    const ui::CompositionText& composition) {
}

void TreeViewSelector::ConfirmCompositionText() {
}

void TreeViewSelector::ClearCompositionText() {
}

void TreeViewSelector::InsertText(const string16& text) {
  OnTextInput(text);
}

void TreeViewSelector::InsertChar(char16 ch, int flags) {
  OnTextInput(string16(1, ch));
}

ui::TextInputType TreeViewSelector::GetTextInputType() const {
  return ui::TEXT_INPUT_TYPE_TEXT;
}

bool TreeViewSelector::CanComposeInline() const {
  return false;
}

gfx::Rect TreeViewSelector::GetCaretBounds() {
  gfx::Rect rect(tree_->GetVisibleBounds().origin(), gfx::Size());
  // TextInputClient::GetCaretBounds is expected to return a value in screen
  // coordinates.
  ConvertRectToScreen(tree_, &rect);
  return rect;
}

bool TreeViewSelector::GetCompositionCharacterBounds(uint32 index,
                                                     gfx::Rect* rect) {
  // TextInputClient::GetCompositionCharacterBounds is expected to fill |rect|
  // in screen coordinates and GetCaretBounds returns screen coordinates.
  *rect = GetCaretBounds();
  return false;
}

bool TreeViewSelector::HasCompositionText() {
  return false;
}

bool TreeViewSelector::GetTextRange(ui::Range* range) {
  *range = ui::Range();
  return false;
}

bool TreeViewSelector::GetCompositionTextRange(ui::Range* range) {
  *range = ui::Range();
  return false;
}

bool TreeViewSelector::GetSelectionRange(ui::Range* range) {
  *range = ui::Range();
  return false;
}

bool TreeViewSelector::SetSelectionRange(const ui::Range& range) {
  return false;
}

bool TreeViewSelector::DeleteRange(const ui::Range& range) {
  return false;
}

bool TreeViewSelector::GetTextFromRange(const ui::Range& range,
                                        string16* text) {
  return false;
}

void TreeViewSelector::OnInputMethodChanged() {
  ClearText();
}

bool TreeViewSelector::ChangeTextDirectionAndLayoutAlignment(
    base::i18n::TextDirection direction) {
  return true;
}

void TreeViewSelector::ExtendSelectionAndDelete(size_t before, size_t after) {
}

void TreeViewSelector::EnsureCaretInRect(const gfx::Rect& rect) {
}

void TreeViewSelector::OnTextInput(const string16& text) {
  const int row_count = tree_->GetRowCount();
  if (row_count == 0)
    return;

  // Search for |text| if it has been a while since the user typed, otherwise
  // append |text| to |current_text_| and search for that. If it has been a
  // while search after the current row, otherwise search starting from the
  // current row.
  int row = (tree_->GetSelectedNode() != NULL) ?
      tree_->GetRowForNode(tree_->GetSelectedNode()) : 0;
  const base::TimeTicks now(base::TimeTicks::Now());
  if ((now - time_of_last_key_).InMilliseconds() < kTimeBeforeClearingMS) {
    current_text_ += text;
  } else {
    current_text_ = text;
    if (tree_->GetSelectedNode())
      row = (row + 1) % row_count;
  }
  time_of_last_key_ = now;

  const int start_row = row;
  const string16 lower_text(base::i18n::ToLower(current_text_));
  do {
    if (TextAtRowMatchesText(row, current_text_)) {
      tree_->SetSelectedNode(tree_->GetNodeForRow(row));
      return;
    }
    row = (row + 1) % row_count;
  } while (row != start_row);
}

bool TreeViewSelector::TextAtRowMatchesText(int row,
                                            const string16& lower_text) {
  ui::TreeModelNode* node = tree_->GetNodeForRow(row);
  DCHECK(node);
  const string16 model_text(base::i18n::ToLower(node->GetTitle()));
  return (model_text.size() >= lower_text.size()) &&
      (model_text.compare(0, lower_text.size(), lower_text) == 0);
}

void TreeViewSelector::ClearText() {
  current_text_.clear();
  time_of_last_key_ = base::TimeTicks();
}

}  // namespace views
