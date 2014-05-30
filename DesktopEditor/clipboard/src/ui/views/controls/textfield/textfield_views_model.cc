// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/views/controls/textfield/textfield_views_model.h"

#include <algorithm>

#include "base/i18n/break_iterator.h"
#include "base/logging.h"
#include "base/stl_util.h"
#include "base/utf_string_conversions.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/clipboard/scoped_clipboard_writer.h"
#include "ui/base/range/range.h"
#include "ui/base/text/utf16_indexing.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/font.h"
#include "ui/gfx/render_text.h"
#include "ui/gfx/text_constants.h"
#include "ui/views/controls/textfield/textfield.h"

namespace views {

namespace internal {

// An edit object holds enough information/state to undo/redo the
// change. Two edits are merged when possible, for example, when
// you type new characters in sequence.  |Commit()| can be used to
// mark an edit as an independent edit and it shouldn't be merged.
// (For example, when you did undo/redo, or a text is appended via
// API)
class Edit {
 public:
  enum Type {
    INSERT_EDIT,
    DELETE_EDIT,
    REPLACE_EDIT
  };

  virtual ~Edit() {
  }

  // Revert the change made by this edit in |model|.
  void Undo(TextfieldViewsModel* model) {
    model->ModifyText(new_text_start_, new_text_end(),
                      old_text_, old_text_start_,
                      old_cursor_pos_);
  }

  // Apply the change of this edit to the |model|.
  void Redo(TextfieldViewsModel* model) {
    model->ModifyText(old_text_start_, old_text_end(),
                      new_text_, new_text_start_,
                      new_cursor_pos_);
  }

  // Try to merge the |edit| into this edit. Returns true if merge was
  // successful, or false otherwise. Merged edit will be deleted after
  // redo and should not be reused.
  bool Merge(const Edit* edit) {
    // Don't merge if previous edit is DELETE. This happens when a
    // user deletes characters then hits return. In this case, the
    // delete should be treated as separate edit that can be undone
    // and should not be merged with the replace edit.
    if (type_ != DELETE_EDIT && edit->merge_with_previous()) {
      MergeReplace(edit);
      return true;
    }
    return mergeable() && edit->mergeable() && DoMerge(edit);
  }

  // Commits the edit and marks as un-mergeable.
  void Commit() { merge_type_ = DO_NOT_MERGE; }

 private:
  friend class InsertEdit;
  friend class ReplaceEdit;
  friend class DeleteEdit;

  Edit(Type type,
       MergeType merge_type,
       size_t old_cursor_pos,
       const string16& old_text,
       size_t old_text_start,
       bool delete_backward,
       size_t new_cursor_pos,
       const string16& new_text,
       size_t new_text_start)
      : type_(type),
        merge_type_(merge_type),
        old_cursor_pos_(old_cursor_pos),
        old_text_(old_text),
        old_text_start_(old_text_start),
        delete_backward_(delete_backward),
        new_cursor_pos_(new_cursor_pos),
        new_text_(new_text),
        new_text_start_(new_text_start) {
  }

  // A template method pattern that provides specific merge
  // implementation for each type of edit.
  virtual bool DoMerge(const Edit* edit) = 0;

  Type type() const { return type_; }

  // Can this edit be merged?
  bool mergeable() const { return merge_type_ == MERGEABLE; }

  // Should this edit be forcibly merged with the previous edit?
  bool merge_with_previous() const {
    return merge_type_ == MERGE_WITH_PREVIOUS;
  }

  // Returns the end index of the |old_text_|.
  size_t old_text_end() const { return old_text_start_ + old_text_.length(); }

  // Returns the end index of the |new_text_|.
  size_t new_text_end() const { return new_text_start_ + new_text_.length(); }

  // Merge the replace edit into the current edit. This is a special case to
  // handle an omnibox setting autocomplete string after new character is
  // typed in.
  void MergeReplace(const Edit* edit) {
    CHECK_EQ(REPLACE_EDIT, edit->type_);
    CHECK_EQ(0U, edit->old_text_start_);
    CHECK_EQ(0U, edit->new_text_start_);
    string16 old_text = edit->old_text_;
    old_text.erase(new_text_start_, new_text_.length());
    old_text.insert(old_text_start_, old_text_);
    // SetText() replaces entire text. Set |old_text_| to the entire
    // replaced text with |this| edit undone.
    old_text_ = old_text;
    old_text_start_ = edit->old_text_start_;
    delete_backward_ = false;

    new_text_ = edit->new_text_;
    new_text_start_ = edit->new_text_start_;
    merge_type_ = DO_NOT_MERGE;
  }

  Type type_;

  // True if the edit can be marged.
  MergeType merge_type_;
  // Old cursor position.
  size_t old_cursor_pos_;
  // Deleted text by this edit.
  string16 old_text_;
  // The index of |old_text_|.
  size_t old_text_start_;
  // True if the deletion is made backward.
  bool delete_backward_;
  // New cursor position.
  size_t new_cursor_pos_;
  // Added text.
  string16 new_text_;
  // The index of |new_text_|
  size_t new_text_start_;

  DISALLOW_COPY_AND_ASSIGN(Edit);
};

class InsertEdit : public Edit {
 public:
  InsertEdit(bool mergeable, const string16& new_text, size_t at)
      : Edit(INSERT_EDIT,
             mergeable ? MERGEABLE : DO_NOT_MERGE,
             at  /* old cursor */,
             string16(),
             at,
             false  /* N/A */,
             at + new_text.length()  /* new cursor */,
             new_text,
             at) {
  }

  // Edit implementation.
  virtual bool DoMerge(const Edit* edit) OVERRIDE {
    if (edit->type() != INSERT_EDIT || new_text_end() != edit->new_text_start_)
      return false;
    // If continuous edit, merge it.
    // TODO(oshima): gtk splits edits between whitespace. Find out what
    // we want to here and implement if necessary.
    new_text_ += edit->new_text_;
    new_cursor_pos_ = edit->new_cursor_pos_;
    return true;
  }
};

class ReplaceEdit : public Edit {
 public:
  ReplaceEdit(MergeType merge_type,
              const string16& old_text,
              size_t old_cursor_pos,
              size_t old_text_start,
              bool backward,
              size_t new_cursor_pos,
              const string16& new_text,
              size_t new_text_start)
      : Edit(REPLACE_EDIT, merge_type,
             old_cursor_pos,
             old_text,
             old_text_start,
             backward,
             new_cursor_pos,
             new_text,
             new_text_start) {
  }

  // Edit implementation.
  virtual bool DoMerge(const Edit* edit) OVERRIDE {
    if (edit->type() == DELETE_EDIT ||
        new_text_end() != edit->old_text_start_ ||
        edit->old_text_start_ != edit->new_text_start_)
      return false;
    old_text_ += edit->old_text_;
    new_text_ += edit->new_text_;
    new_cursor_pos_ = edit->new_cursor_pos_;
    return true;
  }
};

class DeleteEdit : public Edit {
 public:
  DeleteEdit(bool mergeable,
             const string16& text,
             size_t text_start,
             bool backward)
      : Edit(DELETE_EDIT,
             mergeable ? MERGEABLE : DO_NOT_MERGE,
             (backward ? text_start + text.length() : text_start),
             text,
             text_start,
             backward,
             text_start,
             string16(),
             text_start) {
  }

  // Edit implementation.
  virtual bool DoMerge(const Edit* edit) OVERRIDE {
    if (edit->type() != DELETE_EDIT)
      return false;

    if (delete_backward_) {
      // backspace can be merged only with backspace at the
      // same position.
      if (!edit->delete_backward_ || old_text_start_ != edit->old_text_end())
        return false;
      old_text_start_ = edit->old_text_start_;
      old_text_ = edit->old_text_ + old_text_;
      new_cursor_pos_ = edit->new_cursor_pos_;
    } else {
      // delete can be merged only with delete at the same
      // position.
      if (edit->delete_backward_ || old_text_start_ != edit->old_text_start_)
        return false;
      old_text_ += edit->old_text_;
    }
    return true;
  }
};

}  // namespace internal

namespace {

// Returns the first segment that is visually emphasized. Usually it's used for
// representing the target clause (on Windows). Returns an invalid range if
// there is no such a range.
ui::Range GetFirstEmphasizedRange(const ui::CompositionText& composition) {
  for (size_t i = 0; i < composition.underlines.size(); ++i) {
    const ui::CompositionUnderline& underline = composition.underlines[i];
    if (underline.thick)
      return ui::Range(underline.start_offset, underline.end_offset);
  }
  return ui::Range::InvalidRange();
}

}  // namespace

using internal::Edit;
using internal::DeleteEdit;
using internal::InsertEdit;
using internal::ReplaceEdit;
using internal::MergeType;
using internal::DO_NOT_MERGE;
using internal::MERGE_WITH_PREVIOUS;
using internal::MERGEABLE;

/////////////////////////////////////////////////////////////////
// TextfieldViewsModel: public

TextfieldViewsModel::Delegate::~Delegate() {
}

TextfieldViewsModel::TextfieldViewsModel(Delegate* delegate)
    : delegate_(delegate),
      render_text_(gfx::RenderText::CreateInstance()),
      current_edit_(edit_history_.end()) {
}

TextfieldViewsModel::~TextfieldViewsModel() {
  ClearEditHistory();
  ClearComposition();
}

const string16& TextfieldViewsModel::GetText() const {
  return render_text_->text();
}

bool TextfieldViewsModel::SetText(const string16& text) {
  bool changed = false;
  if (HasCompositionText()) {
    ConfirmCompositionText();
    changed = true;
  }
  if (GetText() != text) {
    if (changed)  // No need to remember composition.
      Undo();
    size_t old_cursor = GetCursorPosition();
    // SetText moves the cursor to the end.
    size_t new_cursor = text.length();
    SelectAll(false);
    // If there is a composition text, don't merge with previous edit.
    // Otherwise, force merge the edits.
    ExecuteAndRecordReplace(
        changed ? DO_NOT_MERGE : MERGE_WITH_PREVIOUS,
        old_cursor,
        new_cursor,
        text,
        0U);
    render_text_->SetCursorPosition(new_cursor);
  }
  ClearSelection();
  return changed;
}

void TextfieldViewsModel::Append(const string16& text) {
  if (HasCompositionText())
    ConfirmCompositionText();
  size_t save = GetCursorPosition();
  MoveCursor(gfx::LINE_BREAK,
             render_text_->GetVisualDirectionOfLogicalEnd(),
             false);
  InsertText(text);
  render_text_->SetCursorPosition(save);
  ClearSelection();
}

bool TextfieldViewsModel::Delete() {
  if (HasCompositionText()) {
    // No undo/redo for composition text.
    CancelCompositionText();
    return true;
  }
  if (HasSelection()) {
    DeleteSelection();
    return true;
  }
  if (GetText().length() > GetCursorPosition()) {
    size_t cursor_position = GetCursorPosition();
    size_t next_grapheme_index = render_text_->IndexOfAdjacentGrapheme(
        cursor_position, gfx::CURSOR_FORWARD);
    ExecuteAndRecordDelete(ui::Range(cursor_position, next_grapheme_index),
                           true);
    return true;
  }
  return false;
}

bool TextfieldViewsModel::Backspace() {
  if (HasCompositionText()) {
    // No undo/redo for composition text.
    CancelCompositionText();
    return true;
  }
  if (HasSelection()) {
    DeleteSelection();
    return true;
  }
  size_t cursor_position = GetCursorPosition();
  if (cursor_position > 0) {
    // Delete one code point, which may be two UTF-16 words.
    size_t previous_char =
        ui::UTF16OffsetToIndex(GetText(), cursor_position, -1);
    ExecuteAndRecordDelete(ui::Range(cursor_position, previous_char), true);
    return true;
  }
  return false;
}

size_t TextfieldViewsModel::GetCursorPosition() const {
  return render_text_->cursor_position();
}

void TextfieldViewsModel::MoveCursor(gfx::BreakType break_type,
                                     gfx::VisualCursorDirection direction,
                                     bool select) {
  if (HasCompositionText())
    ConfirmCompositionText();
  render_text_->MoveCursor(break_type, direction, select);
}

bool TextfieldViewsModel::MoveCursorTo(const gfx::SelectionModel& model) {
  if (HasCompositionText()) {
    ConfirmCompositionText();
    // ConfirmCompositionText() updates cursor position. Need to reflect it in
    // the SelectionModel parameter of MoveCursorTo().
    ui::Range range(render_text_->selection().start(), model.caret_pos());
    if (!range.is_empty())
      return render_text_->SelectRange(range);
    return render_text_->MoveCursorTo(
        gfx::SelectionModel(model.caret_pos(), model.caret_affinity()));
  }
  return render_text_->MoveCursorTo(model);
}

bool TextfieldViewsModel::MoveCursorTo(const gfx::Point& point, bool select) {
  if (HasCompositionText())
    ConfirmCompositionText();
  return render_text_->MoveCursorTo(point, select);
}

string16 TextfieldViewsModel::GetSelectedText() const {
  return GetText().substr(render_text_->selection().GetMin(),
                          render_text_->selection().length());
}

void TextfieldViewsModel::SelectRange(const ui::Range& range) {
  if (HasCompositionText())
    ConfirmCompositionText();
  render_text_->SelectRange(range);
}

void TextfieldViewsModel::SelectSelectionModel(const gfx::SelectionModel& sel) {
  if (HasCompositionText())
    ConfirmCompositionText();
  render_text_->MoveCursorTo(sel);
}

void TextfieldViewsModel::SelectAll(bool reversed) {
  if (HasCompositionText())
    ConfirmCompositionText();
  render_text_->SelectAll(reversed);
}

void TextfieldViewsModel::SelectWord() {
  if (HasCompositionText())
    ConfirmCompositionText();
  render_text_->SelectWord();
}

void TextfieldViewsModel::ClearSelection() {
  if (HasCompositionText())
    ConfirmCompositionText();
  render_text_->ClearSelection();
}

bool TextfieldViewsModel::CanUndo() {
  return edit_history_.size() && current_edit_ != edit_history_.end();
}

bool TextfieldViewsModel::CanRedo() {
  if (!edit_history_.size())
    return false;
  // There is no redo iff the current edit is the last element
  // in the history.
  EditHistory::iterator iter = current_edit_;
  return iter == edit_history_.end() || // at the top.
      ++iter != edit_history_.end();
}

bool TextfieldViewsModel::Undo() {
  if (!CanUndo())
    return false;
  DCHECK(!HasCompositionText());
  if (HasCompositionText())  // safe guard for release build.
    CancelCompositionText();

  string16 old = GetText();
  size_t old_cursor = GetCursorPosition();
  (*current_edit_)->Commit();
  (*current_edit_)->Undo(this);

  if (current_edit_ == edit_history_.begin())
    current_edit_ = edit_history_.end();
  else
    current_edit_--;
  return old != GetText() || old_cursor != GetCursorPosition();
}

bool TextfieldViewsModel::Redo() {
  if (!CanRedo())
    return false;
  DCHECK(!HasCompositionText());
  if (HasCompositionText()) // safe guard for release build.
    CancelCompositionText();

  if (current_edit_ == edit_history_.end())
    current_edit_ = edit_history_.begin();
  else
    current_edit_ ++;
  string16 old = GetText();
  size_t old_cursor = GetCursorPosition();
  (*current_edit_)->Redo(this);
  return old != GetText() || old_cursor != GetCursorPosition();
}

bool TextfieldViewsModel::Cut() {
  if (!HasCompositionText() && HasSelection() && !render_text_->obscured()) {
    ui::ScopedClipboardWriter(
        ui::Clipboard::GetForCurrentThread(),
        ui::Clipboard::BUFFER_STANDARD).WriteText(GetSelectedText());
    // A trick to let undo/redo handle cursor correctly.
    // Undoing CUT moves the cursor to the end of the change rather
    // than beginning, unlike Delete/Backspace.
    // TODO(oshima): Change Delete/Backspace to use DeleteSelection,
    // update DeleteEdit and remove this trick.
    const ui::Range& selection = render_text_->selection();
    render_text_->SelectRange(ui::Range(selection.end(), selection.start()));
    DeleteSelection();
    return true;
  }
  return false;
}

bool TextfieldViewsModel::Copy() {
  if (!HasCompositionText() && HasSelection() && !render_text_->obscured()) {
    ui::ScopedClipboardWriter(
        ui::Clipboard::GetForCurrentThread(),
        ui::Clipboard::BUFFER_STANDARD).WriteText(GetSelectedText());
    return true;
  }
  return false;
}

bool TextfieldViewsModel::Paste() {
  string16 result;
  ui::Clipboard::GetForCurrentThread()
      ->ReadText(ui::Clipboard::BUFFER_STANDARD, &result);
  if (!result.empty()) {
    InsertTextInternal(result, false);
    return true;
  }
  return false;
}

bool TextfieldViewsModel::HasSelection() const {
  return !render_text_->selection().is_empty();
}

void TextfieldViewsModel::DeleteSelection() {
  DCHECK(!HasCompositionText());
  DCHECK(HasSelection());
  ExecuteAndRecordDelete(render_text_->selection(), false);
}

void TextfieldViewsModel::DeleteSelectionAndInsertTextAt(
    const string16& text, size_t position) {
  if (HasCompositionText())
    CancelCompositionText();
  ExecuteAndRecordReplace(DO_NOT_MERGE,
                          GetCursorPosition(),
                          position + text.length(),
                          text,
                          position);
}

string16 TextfieldViewsModel::GetTextFromRange(const ui::Range& range) const {
  if (range.IsValid() && range.GetMin() < GetText().length())
    return GetText().substr(range.GetMin(), range.length());
  return string16();
}

void TextfieldViewsModel::GetTextRange(ui::Range* range) const {
  *range = ui::Range(0, GetText().length());
}

void TextfieldViewsModel::SetCompositionText(
    const ui::CompositionText& composition) {
  if (HasCompositionText())
    CancelCompositionText();
  else if (HasSelection())
    DeleteSelection();

  if (composition.text.empty())
    return;

  size_t cursor = GetCursorPosition();
  string16 new_text = GetText();
  render_text_->SetText(new_text.insert(cursor, composition.text));
  ui::Range range(cursor, cursor + composition.text.length());
  render_text_->SetCompositionRange(range);
  ui::Range emphasized_range = GetFirstEmphasizedRange(composition);
  if (emphasized_range.IsValid()) {
    // This is a workaround due to the lack of support in RenderText to draw
    // a thick underline. In a composition returned from an IME, the segment
    // emphasized by a thick underline usually represents the target clause.
    // Because the target clause is more important than the actual selection
    // range (or caret position) in the composition here we use a selection-like
    // marker instead to show this range.
    // TODO(yukawa, msw): Support thick underline in RenderText and remove
    // this workaround.
    render_text_->SelectRange(ui::Range(
        cursor + emphasized_range.GetMin(),
        cursor + emphasized_range.GetMax()));
  } else if (!composition.selection.is_empty()) {
    render_text_->SelectRange(ui::Range(
        cursor + composition.selection.GetMin(),
        cursor + composition.selection.GetMax()));
  } else {
    render_text_->SetCursorPosition(cursor + composition.selection.end());
  }
}

void TextfieldViewsModel::ConfirmCompositionText() {
  DCHECK(HasCompositionText());
  ui::Range range = render_text_->GetCompositionRange();
  string16 text = GetText().substr(range.start(), range.length());
  // TODO(oshima): current behavior on ChromeOS is a bit weird and not
  // sure exactly how this should work. Find out and fix if necessary.
  AddOrMergeEditHistory(new InsertEdit(false, text, range.start()));
  render_text_->SetCursorPosition(range.end());
  ClearComposition();
  if (delegate_)
    delegate_->OnCompositionTextConfirmedOrCleared();
}

void TextfieldViewsModel::CancelCompositionText() {
  DCHECK(HasCompositionText());
  ui::Range range = render_text_->GetCompositionRange();
  ClearComposition();
  string16 new_text = GetText();
  render_text_->SetText(new_text.erase(range.start(), range.length()));
  render_text_->SetCursorPosition(range.start());
  if (delegate_)
    delegate_->OnCompositionTextConfirmedOrCleared();
}

void TextfieldViewsModel::ClearComposition() {
  render_text_->SetCompositionRange(ui::Range::InvalidRange());
}

void TextfieldViewsModel::GetCompositionTextRange(ui::Range* range) const {
  *range = ui::Range(render_text_->GetCompositionRange());
}

bool TextfieldViewsModel::HasCompositionText() const {
  return !render_text_->GetCompositionRange().is_empty();
}

/////////////////////////////////////////////////////////////////
// TextfieldViewsModel: private

void TextfieldViewsModel::InsertTextInternal(const string16& text,
                                             bool mergeable) {
  if (HasCompositionText()) {
    CancelCompositionText();
    ExecuteAndRecordInsert(text, mergeable);
  } else if (HasSelection()) {
    ExecuteAndRecordReplaceSelection(mergeable ? MERGEABLE : DO_NOT_MERGE,
                                     text);
  } else {
    ExecuteAndRecordInsert(text, mergeable);
  }
}

void TextfieldViewsModel::ReplaceTextInternal(const string16& text,
                                              bool mergeable) {
  if (HasCompositionText()) {
    CancelCompositionText();
  } else if (!HasSelection()) {
    size_t cursor = GetCursorPosition();
    const gfx::SelectionModel& model = render_text_->selection_model();
    // When there is no selection, the default is to replace the next grapheme
    // with |text|. So, need to find the index of next grapheme first.
    size_t next =
        render_text_->IndexOfAdjacentGrapheme(cursor, gfx::CURSOR_FORWARD);
    if (next == model.caret_pos())
      render_text_->MoveCursorTo(model);
    else
      render_text_->SelectRange(ui::Range(next, model.caret_pos()));
  }
  // Edit history is recorded in InsertText.
  InsertTextInternal(text, mergeable);
}

void TextfieldViewsModel::ClearEditHistory() {
  STLDeleteElements(&edit_history_);
  current_edit_ = edit_history_.end();
}

void TextfieldViewsModel::ClearRedoHistory() {
  if (edit_history_.begin() == edit_history_.end())
    return;
  if (current_edit_ == edit_history_.end()) {
    ClearEditHistory();
    return;
  }
  EditHistory::iterator delete_start = current_edit_;
  delete_start++;
  STLDeleteContainerPointers(delete_start, edit_history_.end());
  edit_history_.erase(delete_start, edit_history_.end());
}

void TextfieldViewsModel::ExecuteAndRecordDelete(ui::Range range,
                                                 bool mergeable) {
  size_t old_text_start = range.GetMin();
  const string16 text = GetText().substr(old_text_start, range.length());
  bool backward = range.is_reversed();
  Edit* edit = new DeleteEdit(mergeable, text, old_text_start, backward);
  bool delete_edit = AddOrMergeEditHistory(edit);
  edit->Redo(this);
  if (delete_edit)
    delete edit;
}

void TextfieldViewsModel::ExecuteAndRecordReplaceSelection(
    MergeType merge_type, const string16& new_text) {
  size_t new_text_start = render_text_->selection().GetMin();
  size_t new_cursor_pos = new_text_start + new_text.length();
  ExecuteAndRecordReplace(merge_type,
                          GetCursorPosition(),
                          new_cursor_pos,
                          new_text,
                          new_text_start);
}

void TextfieldViewsModel::ExecuteAndRecordReplace(MergeType merge_type,
                                                  size_t old_cursor_pos,
                                                  size_t new_cursor_pos,
                                                  const string16& new_text,
                                                  size_t new_text_start) {
  size_t old_text_start = render_text_->selection().GetMin();
  bool backward = render_text_->selection().is_reversed();
  Edit* edit = new ReplaceEdit(merge_type,
                               GetSelectedText(),
                               old_cursor_pos,
                               old_text_start,
                               backward,
                               new_cursor_pos,
                               new_text,
                               new_text_start);
  bool delete_edit = AddOrMergeEditHistory(edit);
  edit->Redo(this);
  if (delete_edit)
    delete edit;
}

void TextfieldViewsModel::ExecuteAndRecordInsert(const string16& text,
                                                 bool mergeable) {
  Edit* edit = new InsertEdit(mergeable, text, GetCursorPosition());
  bool delete_edit = AddOrMergeEditHistory(edit);
  edit->Redo(this);
  if (delete_edit)
    delete edit;
}

bool TextfieldViewsModel::AddOrMergeEditHistory(Edit* edit) {
  ClearRedoHistory();

  if (current_edit_ != edit_history_.end() && (*current_edit_)->Merge(edit)) {
    // If a current edit exists and has been merged with a new edit,
    // don't add to the history, and return true to delete |edit| after
    // redo.
    return true;
  }
  edit_history_.push_back(edit);
  if (current_edit_ == edit_history_.end()) {
    // If there is no redoable edit, this is the 1st edit because
    // RedoHistory has been already deleted.
    DCHECK_EQ(1u, edit_history_.size());
    current_edit_ = edit_history_.begin();
  } else {
    current_edit_++;
  }
  return false;
}

void TextfieldViewsModel::ModifyText(size_t delete_from,
                                     size_t delete_to,
                                     const string16& new_text,
                                     size_t new_text_insert_at,
                                     size_t new_cursor_pos) {
  DCHECK_LE(delete_from, delete_to);
  string16 text = GetText();
  ClearComposition();
  if (delete_from != delete_to)
    render_text_->SetText(text.erase(delete_from, delete_to - delete_from));
  if (!new_text.empty())
    render_text_->SetText(text.insert(new_text_insert_at, new_text));
  render_text_->SetCursorPosition(new_cursor_pos);
  // TODO(oshima): mac selects the text that is just undone (but gtk doesn't).
  // This looks fine feature and we may want to do the same.
}

}  // namespace views
