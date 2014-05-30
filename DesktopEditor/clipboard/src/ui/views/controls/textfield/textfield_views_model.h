// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_VIEWS_MODEL_H_
#define UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_VIEWS_MODEL_H_

#include <list>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/base/ime/composition_text.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/render_text.h"
#include "ui/gfx/text_constants.h"
#include "ui/views/views_export.h"

namespace gfx {
class RenderText;
}  // namespace gfx

namespace ui {
class Range;
}  // namespace ui

namespace views {

namespace internal {
// Internal Edit class that keeps track of edits for undo/redo.
class Edit;

// C++ doesn't allow forward decl enum, so let's define here.
enum MergeType {
  // The edit should not be merged with next edit. It still may
  // be merged with an edit with MERGE_WITH_PREVIOUS.
  DO_NOT_MERGE,
  // The edit can be merged with next edit when possible.
  MERGEABLE,
  // Does the edit have to be merged with previous edit?
  // This forces the merge even if the previous edit is marked
  // as DO_NOT_MERGE.
  MERGE_WITH_PREVIOUS,
};

}  // namespace internal

// A model that represents a text content for TextfieldViews.
// It supports editing, selection and cursor manipulation.
class VIEWS_EXPORT TextfieldViewsModel {
 public:
  // Delegate interface implemented by the textfield view class to provided
  // additional functionalities required by the model.
  class VIEWS_EXPORT Delegate {
   public:
    // Called when the current composition text is confirmed or cleared.
    virtual void OnCompositionTextConfirmedOrCleared() = 0;

   protected:
    virtual ~Delegate();
  };

  explicit TextfieldViewsModel(Delegate* delegate);
  virtual ~TextfieldViewsModel();

  // Edit related methods.

  const string16& GetText() const;
  // Sets the text. Returns true if the text has been modified.  The
  // current composition text will be confirmed first.  Setting
  // the same text will not add edit history because it's not user
  // visible change nor user-initiated change. This allow a client
  // code to set the same text multiple times without worrying about
  // messing edit history.
  bool SetText(const string16& text);

  gfx::RenderText* render_text() { return render_text_.get(); }

  // Inserts given |text| at the current cursor position.
  // The current composition text will be cleared.
  void InsertText(const string16& text) {
    InsertTextInternal(text, false);
  }

  // Inserts a character at the current cursor position.
  void InsertChar(char16 c) {
    InsertTextInternal(string16(&c, 1), true);
  }

  // Replaces characters at the current position with characters in given text.
  // The current composition text will be cleared.
  void ReplaceText(const string16& text) {
    ReplaceTextInternal(text, false);
  }

  // Replaces the char at the current position with given character.
  void ReplaceChar(char16 c) {
    ReplaceTextInternal(string16(&c, 1), true);
  }

  // Appends the text.
  // The current composition text will be confirmed.
  void Append(const string16& text);

  // Deletes the first character after the current cursor position (as if, the
  // the user has pressed delete key in the textfield). Returns true if
  // the deletion is successful.
  // If there is composition text, it'll be deleted instead.
  bool Delete();

  // Deletes the first character before the current cursor position (as if, the
  // the user has pressed backspace key in the textfield). Returns true if
  // the removal is successful.
  // If there is composition text, it'll be deleted instead.
  bool Backspace();

  // Cursor related methods.

  // Returns the current cursor position.
  size_t GetCursorPosition() const;

  // Moves the cursor, see RenderText for additional details.
  // The current composition text will be confirmed.
  void MoveCursor(gfx::BreakType break_type,
                  gfx::VisualCursorDirection direction,
                  bool select);

  // Moves the selection to the specified selection in |selection|.
  // If there is composition text, it will be confirmed, which will update the
  // selection range, and it overrides the selection_start to which the
  // selection will move to.
  bool MoveCursorTo(const gfx::SelectionModel& selection);

  // Helper function to call MoveCursorTo on the TextfieldViewsModel.
  bool MoveCursorTo(const gfx::Point& point, bool select);

  // Selection related method

  // Returns the selected text.
  string16 GetSelectedText() const;

  // The current composition text will be confirmed. The selection starts with
  // the range's start position, and ends with the range's end position,
  // therefore the cursor position becomes the end position.
  void SelectRange(const ui::Range& range);

  // The current composition text will be confirmed.
  // render_text_'s selection model is set to |sel|.
  void SelectSelectionModel(const gfx::SelectionModel& sel);

  // Select the entire text range. If |reversed| is true, the range will end at
  // the logical beginning of the text; this generally shows the leading portion
  // of text that overflows its display area.
  // The current composition text will be confirmed.
  void SelectAll(bool reversed);

  // Selects the word at which the cursor is currently positioned.
  // The current composition text will be confirmed.
  void SelectWord();

  // Clears selection.
  // The current composition text will be confirmed.
  void ClearSelection();

  // Returns true if there is an undoable edit.
  bool CanUndo();

  // Returns true if there is an redoable edit.
  bool CanRedo();

  // Undo edit. Returns true if undo changed the text.
  bool Undo();

  // Redo edit. Returns true if redo changed the text.
  bool Redo();

  // Cuts the currently selected text and puts it to clipboard. Returns true
  // if text has changed after cutting.
  bool Cut();

  // Copies the currently selected text and puts it to clipboard. Returns true
  // if something was copied to the clipboard.
  bool Copy();

  // Pastes text from the clipboard at current cursor position. Returns true
  // if any text is pasted.
  bool Paste();

  // Tells if any text is selected, even if the selection is in composition
  // text.
  bool HasSelection() const;

  // Deletes the selected text. This method shouldn't be called with
  // composition text.
  void DeleteSelection();

  // Deletes the selected text (if any) and insert text at given
  // position.
  void DeleteSelectionAndInsertTextAt(
      const string16& text, size_t position);

  // Retrieves the text content in a given range.
  string16 GetTextFromRange(const ui::Range& range) const;

  // Retrieves the range containing all text in the model.
  void GetTextRange(ui::Range* range) const;

  // Sets composition text and attributes. If there is composition text already,
  // it'll be replaced by the new one. Otherwise, current selection will be
  // replaced. If there is no selection, the composition text will be inserted
  // at the insertion point.
  // Any changes to the model except text insertion will confirm the current
  // composition text.
  void SetCompositionText(const ui::CompositionText& composition);

  // Converts current composition text into final content.
  void ConfirmCompositionText();

  // Removes current composition text.
  void CancelCompositionText();

  // Retrieves the range of current composition text.
  void GetCompositionTextRange(ui::Range* range) const;

  // Returns true if there is composition text.
  bool HasCompositionText() const;

 private:
  friend class NativeTextfieldViews;
  friend class NativeTextfieldViewsTest;
  friend class TextfieldViewsModelTest;
  friend class UndoRedo_BasicTest;
  friend class UndoRedo_CutCopyPasteTest;
  friend class UndoRedo_ReplaceTest;
  friend class internal::Edit;

  FRIEND_TEST_ALL_PREFIXES(TextfieldViewsModelTest, UndoRedo_BasicTest);
  FRIEND_TEST_ALL_PREFIXES(TextfieldViewsModelTest, UndoRedo_CutCopyPasteTest);
  FRIEND_TEST_ALL_PREFIXES(TextfieldViewsModelTest, UndoRedo_ReplaceTest);

  // Insert the given |text|. |mergeable| indicates if this insert
  // operation can be merged to previous edit in the edit history.
  void InsertTextInternal(const string16& text, bool mergeable);

  // Replace the current text with the given |text|. |mergeable|
  // indicates if this replace operation can be merged to previous
  // edit in the edit history.
  void ReplaceTextInternal(const string16& text, bool mergeable);

  // Clears all edit history.
  void ClearEditHistory();

  // Clears redo history.
  void ClearRedoHistory();

  // Executes and records edit operations.
  void ExecuteAndRecordDelete(ui::Range range, bool mergeable);
  void ExecuteAndRecordReplaceSelection(internal::MergeType merge_type,
                                        const string16& text);
  void ExecuteAndRecordReplace(internal::MergeType merge_type,
                               size_t old_cursor_pos,
                               size_t new_cursor_pos,
                               const string16& text,
                               size_t new_text_start);
  void ExecuteAndRecordInsert(const string16& text, bool mergeable);

  // Adds or merge |edit| into edit history. Return true if the edit
  // has been merged and must be deleted after redo.
  bool AddOrMergeEditHistory(internal::Edit* edit);

  // Modify the text buffer in following way:
  // 1) Delete the string from |delete_from| to |delte_to|.
  // 2) Insert the |new_text| at the index |new_text_insert_at|.
  //    Note that the index is after deletion.
  // 3) Move the cursor to |new_cursor_pos|.
  void ModifyText(size_t delete_from,
                  size_t delete_to,
                  const string16& new_text,
                  size_t new_text_insert_at,
                  size_t new_cursor_pos);

  void ClearComposition();

  // Pointer to a TextfieldViewsModel::Delegate instance, should be provided by
  // the View object.
  Delegate* delegate_;

  // The stylized text, cursor, selection, and the visual layout model.
  scoped_ptr<gfx::RenderText> render_text_;

  typedef std::list<internal::Edit*> EditHistory;
  EditHistory edit_history_;

  // An iterator that points to the current edit that can be undone.
  // This iterator moves from the |end()|, meaining no edit to undo,
  // to the last element (one before |end()|), meaning no edit to redo.
  //  There is no edit to undo (== end()) when:
  // 1) in initial state. (nothing to undo)
  // 2) very 1st edit is undone.
  // 3) all edit history is removed.
  //  There is no edit to redo (== last element or no element) when:
  // 1) in initial state. (nothing to redo)
  // 2) new edit is added. (redo history is cleared)
  // 3) redone all undone edits.
  EditHistory::iterator current_edit_;

  DISALLOW_COPY_AND_ASSIGN(TextfieldViewsModel);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_TEXTFIELD_TEXTFIELD_VIEWS_MODEL_H_
