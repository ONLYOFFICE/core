// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_WIN_H_
#define UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_WIN_H_

#include <atlbase.h>
#include <atlapp.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlmisc.h>
#include <oleacc.h>
#include <tom.h>  // For ITextDocument, a COM interface to CRichEditCtrl
#include <vsstyle.h>

#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "base/win/scoped_comptr.h"
#include "ui/base/models/simple_menu_model.h"
#include "ui/base/ime/win/tsf_event_router.h"
#include "ui/gfx/insets.h"
#include "ui/base/win/extra_sdk_defines.h"
#include "ui/views/controls/textfield/native_textfield_wrapper.h"

namespace views {

class MenuRunner;
class NativeViewHost;
class Textfield;

static const int kDefaultEditStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN |
    WS_CLIPSIBLINGS;

// TODO(beng): make a subclass of NativeControlWin instead.
class NativeTextfieldWin
    : public CWindowImpl<NativeTextfieldWin, CRichEditCtrl,
                         CWinTraits<kDefaultEditStyle> >,
      public CRichEditCommands<NativeTextfieldWin>,
      public NativeTextfieldWrapper,
      public ui::SimpleMenuModel::Delegate,
      public ui::TSFEventRouterObserver {
 public:
  DECLARE_WND_SUPERCLASS(L"ViewsTextfieldEdit", MSFTEDIT_CLASS);

  explicit NativeTextfieldWin(Textfield* parent);
  ~NativeTextfieldWin();

  // Returns true if the current point is close enough to the origin point in
  // space and time that it would be considered a double click.
  VIEWS_EXPORT static bool IsDoubleClick(const POINT& origin,
                                         const POINT& current,
                                         DWORD elapsed_time);

  // See the code in textfield.cc that calls this for why this is here.
  void AttachHack();

  // NativeTextfieldWrapper:
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
  virtual bool HandleKeyPressed(const ui::KeyEvent& event) OVERRIDE;
  virtual bool HandleKeyReleased(const ui::KeyEvent& event) OVERRIDE;
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

  // ui::SimpleMenuModel::Delegate:
  virtual bool IsCommandIdChecked(int command_id) const OVERRIDE;
  virtual bool IsCommandIdEnabled(int command_id) const OVERRIDE;
  virtual bool GetAcceleratorForCommandId(
      int command_id,
      ui::Accelerator* accelerator) OVERRIDE;
  virtual void ExecuteCommand(int command_id, int event_flags) OVERRIDE;

  // ui::TSFEventRouterObserver:
  virtual void OnTextUpdated(const ui::Range& composition_range) OVERRIDE;
  virtual void OnTSFStartComposition() OVERRIDE;
  virtual void OnTSFEndComposition() OVERRIDE;

  // Update accessibility information.
  void InitializeAccessibilityInfo();
  void UpdateAccessibleState(uint32 state_flag, bool set_value);
  void UpdateAccessibleValue(const string16& value);

  // CWindowImpl
  BEGIN_MSG_MAP(Edit)
    MSG_WM_CHAR(OnChar)
    MSG_WM_CONTEXTMENU(OnContextMenu)
    MSG_WM_COPY(OnCopy)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_CUT(OnCut)
    MESSAGE_HANDLER_EX(WM_GETOBJECT, OnGetObject)
    MESSAGE_HANDLER_EX(WM_IME_CHAR, OnImeChar)
    MESSAGE_HANDLER_EX(WM_IME_STARTCOMPOSITION, OnImeStartComposition)
    MESSAGE_HANDLER_EX(WM_IME_COMPOSITION, OnImeComposition)
    MESSAGE_HANDLER_EX(WM_IME_ENDCOMPOSITION, OnImeEndComposition)
    MESSAGE_HANDLER_EX(WM_POINTERDOWN, OnPointerDown)
    MSG_WM_KEYDOWN(OnKeyDown)
    MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
    MSG_WM_LBUTTONDOWN(OnLButtonDown)
    MSG_WM_LBUTTONUP(OnLButtonUp)
    MSG_WM_MBUTTONDOWN(OnNonLButtonDown)
    MSG_WM_MOUSEMOVE(OnMouseMove)
    MSG_WM_MOUSELEAVE(OnMouseLeave)
    MESSAGE_HANDLER_EX(WM_MOUSEWHEEL, OnMouseWheel)
    MSG_WM_NCCALCSIZE(OnNCCalcSize)
    MSG_WM_NCPAINT(OnNCPaint)
    MSG_WM_RBUTTONDOWN(OnNonLButtonDown)
    MSG_WM_PASTE(OnPaste)
    MSG_WM_SETFOCUS(OnSetFocus)
    MSG_WM_KILLFOCUS(OnKillFocus)
    MSG_WM_SYSCHAR(OnSysChar)  // WM_SYSxxx == WM_xxx with ALT down
    MSG_WM_SYSKEYDOWN(OnKeyDown)
  END_MSG_MAP()

 private:
  // This object freezes repainting of the edit until the object is destroyed.
  // Some methods of the CRichEditCtrl draw synchronously to the screen.  If we
  // don't freeze, the user will see a rapid series of calls to these as
  // flickers.
  //
  // Freezing the control while it is already frozen is permitted; the control
  // will unfreeze once both freezes are released (the freezes stack).
  class ScopedFreeze {
   public:
    ScopedFreeze(NativeTextfieldWin* edit, ITextDocument* text_object_model);
    ~ScopedFreeze();

   private:
    NativeTextfieldWin* const edit_;
    ITextDocument* const text_object_model_;

    DISALLOW_COPY_AND_ASSIGN(ScopedFreeze);
  };

  // This object suspends placing any operations on the edit's undo stack until
  // the object is destroyed.  If we don't do this, some of the operations we
  // perform behind the user's back will be undoable by the user, which feels
  // bizarre and confusing.
  class ScopedSuspendUndo {
   public:
    explicit ScopedSuspendUndo(ITextDocument* text_object_model);
    ~ScopedSuspendUndo();

   private:
    ITextDocument* const text_object_model_;

    DISALLOW_COPY_AND_ASSIGN(ScopedSuspendUndo);
  };

  // Message handlers.
  void OnChar(TCHAR key, UINT repeat_count, UINT flags);
  void OnContextMenu(HWND window, const POINT& point);
  void OnCopy();
  LRESULT OnCreate(const CREATESTRUCTW* create_struct);
  void OnCut();
  LRESULT OnGetObject(UINT message, WPARAM wparam, LPARAM lparam);
  LRESULT OnImeChar(UINT message, WPARAM wparam, LPARAM lparam);
  LRESULT OnImeStartComposition(UINT message, WPARAM wparam, LPARAM lparam);
  LRESULT OnImeComposition(UINT message, WPARAM wparam, LPARAM lparam);
  LRESULT OnImeEndComposition(UINT message, WPARAM wparam, LPARAM lparam);
  LRESULT OnPointerDown(UINT message, WPARAM wparam, LPARAM lparam);
  void OnKeyDown(TCHAR key, UINT repeat_count, UINT flags);
  void OnLButtonDblClk(UINT keys, const CPoint& point);
  void OnLButtonDown(UINT keys, const CPoint& point);
  void OnLButtonUp(UINT keys, const CPoint& point);
  void OnMouseLeave();
  LRESULT OnMouseWheel(UINT message, WPARAM w_param, LPARAM l_param);
  void OnMouseMove(UINT keys, const CPoint& point);
  int OnNCCalcSize(BOOL w_param, LPARAM l_param);
  void OnNCPaint(HRGN region);
  void OnNonLButtonDown(UINT keys, const CPoint& point);
  void OnPaste();
  void OnSetFocus(HWND hwnd);
  void OnKillFocus(HWND hwnd);
  void OnSysChar(TCHAR ch, UINT repeat_count, UINT flags);

  // CWindowImpl overrides:
  virtual void OnFinalMessage(HWND hwnd) OVERRIDE;

  // Helper function for OnChar() and OnKeyDown() that handles keystrokes that
  // could change the text in the edit.
  // Note: This function assumes GetCurrentMessage() returns a MSG with
  // msg > WM_KEYFIRST and < WM_KEYLAST.
  void HandleKeystroke();

  // Every piece of code that can change the edit should call these functions
  // before and after the change.  These functions determine if anything
  // meaningful changed, and do any necessary updating and notification.
  void OnBeforePossibleChange();

  // When a user types a BIDI mirroring character (e.g. left parenthesis
  // U+0028, which should be rendered as '(' in LTR context unless  surrounded
  // by RTL characters in both sides, and it should be rendered as ')' in RTL
  // context unless surrounded by LTR characters in both sides.), the textfield
  // does not properly mirror the character when necessary. However, if we
  // explicitly set the text in the edit to the entire current string, then
  // the BIDI mirroring characters will be mirrored properly. When
  // |should_redraw_text| is true, we explicitly set the text in the edit to
  // the entire current string any time the text changes.
  void OnAfterPossibleChange(bool should_redraw_text);

  // Given an X coordinate in client coordinates, returns that coordinate
  // clipped to be within the horizontal bounds of the visible text.
  //
  // This is used in our mouse handlers to work around quirky behaviors of the
  // underlying CRichEditCtrl like not supporting triple-click when the user
  // doesn't click on the text itself.
  //
  // |is_triple_click| should be true iff this is the third click of a triple
  // click.  Sadly, we need to clip slightly differently in this case.
  LONG ClipXCoordToVisibleText(LONG x, bool is_triple_click) const;

  // Sets whether the mouse is in the edit. As necessary this redraws the
  // edit.
  void SetContainsMouse(bool contains_mouse);

  // Handles composition related works on both IMM32 and TSF implementation.
  void OnImeStartCompositionInternal();
  void OnImeEndCompositionInternal();

  // Getter for the text_object_model_, used by the ScopedFreeze class.  Note
  // that the pointer returned here is only valid as long as the Edit is still
  // alive.
  ITextDocument* GetTextObjectModel() const;

  // Generates the contents of the context menu.
  void BuildContextMenu();

  // Returns true if normal processing of the current mouse press event should
  // occur.
  bool ShouldProcessMouseEvent();

  static HMODULE loaded_libarary_module_;

  // The Textfield this object is bound to.
  Textfield* textfield_;

  // We need to know if the user triple-clicks, so track double click points
  // and times so we can see if subsequent clicks are actually triple clicks.
  bool tracking_double_click_;
  CPoint double_click_point_;
  DWORD double_click_time_;

  // Used to discard unnecessary WM_MOUSEMOVE events after the first such
  // unnecessary event.  See detailed comments in OnMouseMove().
  bool can_discard_mousemove_;

  // The text of this control before a possible change.
  string16 text_before_change_;

  // If true, the mouse is over the edit.
  bool contains_mouse_;

  // The contents of the context menu for the edit.
  scoped_ptr<ui::SimpleMenuModel> context_menu_contents_;
  scoped_ptr<MenuRunner> context_menu_runner_;

  // Border insets.
  gfx::Insets content_insets_;

  // This interface is useful for accessing the CRichEditCtrl at a low level.
  mutable base::win::ScopedComPtr<ITextDocument> text_object_model_;

  // The position and the length of the ongoing composition string.
  // These values are used for removing a composition string from a search
  // text to emulate Firefox.
  bool ime_discard_composition_;
  int ime_composition_start_;
  int ime_composition_length_;

  // TODO(beng): remove this when we are a subclass of NativeControlWin.
  NativeViewHost* container_view_;

  COLORREF bg_color_;

  //  The accessibility state of this object.
  int accessibility_state_;

  scoped_ptr<ui::TSFEventRouter> tsf_event_router_;

  DISALLOW_COPY_AND_ASSIGN(NativeTextfieldWin);
};

}  // namespace views

#endif  // UI_VIEWS_CONTROLS_TEXTFIELD_NATIVE_TEXTFIELD_WIN_H_
