// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_VIEWS_H_
#define UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_VIEWS_H_

#include "base/memory/weak_ptr.h"
#include "base/string16.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/base/touch/touch_editing_controller.h"
#include "ui/gfx/font.h"
#include "ui/views/border.h"
#include "ui/views/context_menu_controller.h"
#include "ui/views/controls/textfield/native_textfield_wrapper.h"
#include "ui/views/controls/textfield/textfield_views_model.h"
#include "ui/views/drag_controller.h"
#include "ui/views/view.h"

namespace base {
class Time;
}

namespace gfx {
class Canvas;
}

namespace views {

class FocusableBorder;
class MenuModelAdapter;
class MenuRunner;

// A views/skia only implementation of NativeTextfieldWrapper.
// No platform specific code is used.
// Following features are not yet supported.
// * BIDI/Complex script.
// * Support surrogate pair, or maybe we should just use UTF32 internally.
// * X selection (only if we want to support).
// Once completed, this will replace Textfield, NativeTextfieldWin and
// NativeTextfieldGtk.
class VIEWS_EXPORT NativeTextfieldViews : public View,
                                          public ui::TouchEditable,
                                          public ContextMenuController,
                                          public DragController,
                                          public NativeTextfieldWrapper,
                                          public ui::TextInputClient,
                                          public TextfieldViewsModel::Delegate {
 public:
  // Interval over which the cursor/caret blinks.  This represents the full
  // cycle; the caret is shown for half of this time and hidden for the other
  // half.
  static const int kCursorBlinkCycleMs;

  explicit NativeTextfieldViews(Textfield* parent);
  virtual ~NativeTextfieldViews();

  // View overrides:
  virtual gfx::NativeCursor GetCursor(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual bool OnKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool GetDropFormats(
      int* formats,
      std::set<ui::OSExchangeData::CustomFormat>* custom_formats) OVERRIDE;
  virtual bool CanDrop(const ui::OSExchangeData& data) OVERRIDE;
  virtual int OnDragUpdated(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragExited() OVERRIDE;
  virtual int OnPerformDrop(const ui::DropTargetEvent& event) OVERRIDE;
  virtual void OnDragDone() OVERRIDE;
  virtual bool OnKeyReleased(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual void OnNativeThemeChanged(const ui::NativeTheme* theme) OVERRIDE;

  // ui::TouchEditable overrides:
  virtual void SelectRect(const gfx::Point& start,
                          const gfx::Point& end) OVERRIDE;
  virtual void MoveCaretTo(const gfx::Point& point) OVERRIDE;
  virtual void GetSelectionEndPoints(gfx::Rect* p1, gfx::Rect* p2) OVERRIDE;
  virtual gfx::Rect GetBounds() OVERRIDE;
  virtual gfx::NativeView GetNativeView() OVERRIDE;
  virtual void ConvertPointToScreen(gfx::Point* point) OVERRIDE;
  virtual void ConvertPointFromScreen(gfx::Point* point) OVERRIDE;
  virtual bool DrawsHandles() OVERRIDE;
  virtual void OpenContextMenu(const gfx::Point anchor) OVERRIDE;

  // ContextMenuController overrides:
  virtual void ShowContextMenuForView(View* source,
                                      const gfx::Point& point) OVERRIDE;

  // Overridden from DragController:
  virtual void WriteDragDataForView(View* sender,
                                    const gfx::Point& press_pt,
                                    ui::OSExchangeData* data) OVERRIDE;
  virtual int GetDragOperationsForView(View* sender,
                                       const gfx::Point& p) OVERRIDE;
  virtual bool CanStartDragForView(View* sender,
                                   const gfx::Point& press_pt,
                                   const gfx::Point& p) OVERRIDE;

  // NativeTextfieldWrapper overrides:
  virtual string16 GetText() const OVERRIDE;
  virtual void UpdateText() OVERRIDE;
  virtual void AppendText(const string16& text) OVERRIDE;
  virtual void ReplaceSelection(const string16& text) OVERRIDE;
  virtual base::i18n::TextDirection GetTextDirection() const OVERRIDE;
  virtual string16 GetSelectedText() const OVERRIDE;
  virtual void SelectAll(bool reversed) OVERRIDE;
  virtual void ClearSelection() OVERRIDE;
  virtual void UpdateBorder() OVERRIDE;
  virtual void UpdateBorderColor() OVERRIDE;
  virtual void UpdateTextColor() OVERRIDE;
  virtual void UpdateBackgroundColor() OVERRIDE;
  virtual void UpdateReadOnly() OVERRIDE;
  virtual void UpdateFont() OVERRIDE;
  virtual void UpdateIsObscured() OVERRIDE;
  virtual void UpdateEnabled() OVERRIDE;
  virtual gfx::Insets CalculateInsets() OVERRIDE;
  virtual void UpdateHorizontalMargins() OVERRIDE;
  virtual void UpdateVerticalMargins() OVERRIDE;
  virtual bool SetFocus() OVERRIDE;
  virtual View* GetView() OVERRIDE;
  virtual gfx::NativeView GetTestingHandle() const OVERRIDE;
  virtual bool IsIMEComposing() const OVERRIDE;
  virtual ui::Range GetSelectedRange() const OVERRIDE;
  virtual void SelectRange(const ui::Range& range) OVERRIDE;
  virtual gfx::SelectionModel GetSelectionModel() const OVERRIDE;
  virtual void SelectSelectionModel(const gfx::SelectionModel& sel) OVERRIDE;
  virtual size_t GetCursorPosition() const OVERRIDE;
  virtual bool GetCursorEnabled() const OVERRIDE;
  virtual void SetCursorEnabled(bool enabled) OVERRIDE;
  virtual bool HandleKeyPressed(const ui::KeyEvent& e) OVERRIDE;
  virtual bool HandleKeyReleased(const ui::KeyEvent& e) OVERRIDE;
  virtual void HandleFocus() OVERRIDE;
  virtual void HandleBlur() OVERRIDE;
  virtual ui::TextInputClient* GetTextInputClient() OVERRIDE;
  virtual void SetColor(SkColor value) OVERRIDE;
  virtual void ApplyColor(SkColor value, const ui::Range& range) OVERRIDE;
  virtual void SetStyle(gfx::TextStyle style, bool value) OVERRIDE;
  virtual void ApplyStyle(gfx::TextStyle style,
                          bool value,
                          const ui::Range& range) OVERRIDE;
  virtual void ClearEditHistory() OVERRIDE;
  virtual int GetFontHeight() OVERRIDE;
  virtual int GetTextfieldBaseline() const OVERRIDE;
  virtual int GetWidthNeededForText() const OVERRIDE;
  virtual void ExecuteTextCommand(int command_id) OVERRIDE;
  virtual bool HasTextBeingDragged() OVERRIDE;

  // ui::SimpleMenuModel::Delegate overrides
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual bool IsItemForCommandIdDynamic(int command_id) const OVERRIDE;
  virtual string16 GetLabelForCommandId(int command_id) const OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  // class name of internal
  static const char kViewClassName[];

 protected:
  // View override.
  virtual void OnBoundsChanged(const gfx::Rect& previous_bounds) OVERRIDE;

 private:
  friend class NativeTextfieldViewsTest;
  friend class TouchSelectionControllerImplTest;

  // Overridden from ui::TextInputClient:
  virtual void SetCompositionText(
      const ui::CompositionText& composition) OVERRIDE;
  virtual void ConfirmCompositionText() OVERRIDE;
  virtual void ClearCompositionText() OVERRIDE;
  virtual void InsertText(const string16& text) OVERRIDE;
  virtual void InsertChar(char16 ch, int flags) OVERRIDE;
  virtual ui::TextInputType GetTextInputType() const OVERRIDE;
  virtual bool CanComposeInline() const OVERRIDE;
  virtual gfx::Rect GetCaretBounds() OVERRIDE;
  virtual bool GetCompositionCharacterBounds(uint32 index,
                                             gfx::Rect* rect) OVERRIDE;
  virtual bool HasCompositionText() OVERRIDE;
  virtual bool GetTextRange(ui::Range* range) OVERRIDE;
  virtual bool GetCompositionTextRange(ui::Range* range) OVERRIDE;
  virtual bool GetSelectionRange(ui::Range* range) OVERRIDE;
  virtual bool SetSelectionRange(const ui::Range& range) OVERRIDE;
  virtual bool DeleteRange(const ui::Range& range) OVERRIDE;
  virtual bool GetTextFromRange(const ui::Range& range,
                                string16* text) OVERRIDE;
  virtual void OnInputMethodChanged() OVERRIDE;
  virtual bool ChangeTextDirectionAndLayoutAlignment(
      base::i18n::TextDirection direction) OVERRIDE;
  virtual void ExtendSelectionAndDelete(size_t before, size_t after) OVERRIDE;
  virtual void EnsureCaretInRect(const gfx::Rect& rect) OVERRIDE;

  // Overridden from TextfieldViewsModel::Delegate:
  virtual void OnCompositionTextConfirmedOrCleared() OVERRIDE;

  // Returns the TextfieldViewsModel's text/cursor/selection rendering model.
  gfx::RenderText* GetRenderText() const;

  // Converts |text| according to textfield style, e.g. lower case if
  // |textfield_| has STYLE_LOWERCASE style.
  string16 GetTextForDisplay(const string16& text);

  // Updates any colors that have not been explicitly set from the theme.
  void UpdateColorsFromTheme(const ui::NativeTheme* theme);

  // A callback function to periodically update the cursor state.
  void UpdateCursor();

  // Repaint the cursor.
  void RepaintCursor();

  // Update the cursor_bounds and text_offset.
  void UpdateCursorBoundsAndTextOffset(size_t cursor_pos, bool insert_mode);

  void PaintTextAndCursor(gfx::Canvas* canvas);

  // Handle the keyevent.
  bool HandleKeyEvent(const ui::KeyEvent& key_event);

  // Helper function to call MoveCursorTo on the TextfieldViewsModel.
  bool MoveCursorTo(const gfx::Point& point, bool select);

  // Utility function to inform the parent textfield (and its controller if any)
  // that the text in the textfield has changed.
  void PropagateTextChange();

  // Does necessary updates when the text and/or the position of the cursor
  // changed.
  void UpdateAfterChange(bool text_changed, bool cursor_changed);

  // Utility function to prepare the context menu.
  void UpdateContextMenu();

  // Convenience method to call InputMethod::OnTextInputTypeChanged();
  void OnTextInputTypeChanged();

  // Convenience method to call InputMethod::OnCaretBoundsChanged();
  void OnCaretBoundsChanged();

  // Convenience method to call TextfieldController::OnBeforeUserAction();
  void OnBeforeUserAction();

  // Convenience method to call TextfieldController::OnAfterUserAction();
  void OnAfterUserAction();

  // Calls |model_->Cut()| and notifies TextfieldController on success.
  bool Cut();

  // Calls |model_->Copy()| and notifies TextfieldController on success.
  bool Copy();

  // Calls |model_->Paste()| and calls TextfieldController::ContentsChanged()
  // explicitly if paste succeeded.
  bool Paste();

  // Tracks the mouse clicks for single/double/triple clicks.
  void TrackMouseClicks(const ui::MouseEvent& event);

  // Handles mouse press events.
  void HandleMousePressEvent(const ui::MouseEvent& event);

  // Returns true if the current text input type allows access by the IME.
  bool ImeEditingAllowed() const;

  // Returns true if distance between |event| and |last_click_location_|
  // exceeds the drag threshold.
  bool ExceededDragThresholdFromLastClickLocation(const ui::MouseEvent& event);

  // Checks if a char is ok to be inserted into the textfield. The |ch| is a
  // modified character, i.e., modifiers took effect when generating this char.
  static bool ShouldInsertChar(char16 ch, int flags);

  void CreateTouchSelectionControllerAndNotifyIt();

  // Platform specific gesture event handling.
  void PlatformGestureEventHandling(const ui::GestureEvent* event);

  // The parent textfield, the owner of this object.
  Textfield* textfield_;

  // The text model.
  scoped_ptr<TextfieldViewsModel> model_;

  // The reference to the border class. The object is owned by View::border_.
  FocusableBorder* text_border_;

  // The textfield's text and drop cursor visibility.
  bool is_cursor_visible_;

  // The drop cursor is a visual cue for where dragged text will be dropped.
  bool is_drop_cursor_visible_;
  // Position of the drop cursor, if it is visible.
  gfx::SelectionModel drop_cursor_position_;

  // True if InputMethod::CancelComposition() should not be called.
  bool skip_input_method_cancel_composition_;

  // Is the user potentially dragging and dropping from this view?
  bool initiating_drag_;

  // A runnable method factory for callback to update the cursor.
  base::WeakPtrFactory<NativeTextfieldViews> cursor_timer_;

  // State variables used to track double and triple clicks.
  size_t aggregated_clicks_;
  base::TimeDelta last_click_time_;
  gfx::Point last_click_location_;

  // Context menu and its content list for the textfield.
  scoped_ptr<ui::SimpleMenuModel> context_menu_contents_;
  scoped_ptr<views::MenuModelAdapter> context_menu_delegate_;
  scoped_ptr<views::MenuRunner> context_menu_runner_;

  scoped_ptr<ui::TouchSelectionController> touch_selection_controller_;

  DISALLOW_COPY_AND_ASSIGN(NativeTextfieldViews);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_VIEWS_H_
