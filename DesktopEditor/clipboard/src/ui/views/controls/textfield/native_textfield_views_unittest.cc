// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <set>
#include <string>
#include <vector>

#include "base/auto_reset.h"
#include "base/bind.h"
#include "base/bind_helpers.h"
#include "base/callback.h"
#include "base/command_line.h"
#include "base/message_loop.h"
#include "base/pickle.h"
#include "base/string16.h"
#include "base/utf_string_conversions.h"
#include "googleurl/src/gurl.h"
#include "grit/ui_strings.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/base/clipboard/scoped_clipboard_writer.h"
#include "ui/base/dragdrop/drag_drop_types.h"
#include "ui/base/events/event.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/base/keycodes/keyboard_codes.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/ui_base_switches.h"
#include "ui/gfx/render_text.h"
#include "ui/views/controls/textfield/native_textfield_views.h"
#include "ui/views/controls/textfield/textfield.h"
#include "ui/views/controls/textfield/textfield_controller.h"
#include "ui/views/controls/textfield/textfield_views_model.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/ime/mock_input_method.h"
#include "ui/views/test/test_views_delegate.h"
#include "ui/views/test/views_test_base.h"
#include "ui/views/widget/native_widget_private.h"
#include "ui/views/widget/widget.h"

namespace {

// A wrapper of Textfield to intercept the result of OnKeyPressed() and
// OnKeyReleased() methods.
class TestTextfield : public views::Textfield {
 public:
  explicit TestTextfield(StyleFlags style)
      : Textfield(style),
        key_handled_(false),
        key_received_(false) {
  }

  virtual bool OnKeyPressed(const ui::KeyEvent& e) OVERRIDE {
    key_received_ = true;
    key_handled_ = views::Textfield::OnKeyPressed(e);
    return key_handled_;
  }

  virtual bool OnKeyReleased(const ui::KeyEvent& e) OVERRIDE {
    key_received_ = true;
    key_handled_ = views::Textfield::OnKeyReleased(e);
    return key_handled_;
  }

  bool key_handled() const { return key_handled_; }
  bool key_received() const { return key_received_; }

  void clear() { key_received_ = key_handled_ = false; }

 private:
  bool key_handled_;
  bool key_received_;

  DISALLOW_COPY_AND_ASSIGN(TestTextfield);
};

// A helper class for use with ui::TextInputClient::GetTextFromRange().
class GetTextHelper {
 public:
  GetTextHelper() {}

  void set_text(const string16& text) { text_ = text; }
  const string16& text() const { return text_; }

 private:
  string16 text_;

  DISALLOW_COPY_AND_ASSIGN(GetTextHelper);
};

// Convenience to make constructing a GestureEvent simpler.
class GestureEventForTest : public ui::GestureEvent {
 public:
  GestureEventForTest(ui::EventType type, int x, int y, int flags)
      : GestureEvent(type, x, y, flags, base::TimeDelta(),
                     ui::GestureEventDetails(type, 0.0f, 0.0f), 0) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(GestureEventForTest);
};

const char16 kHebrewLetterSamekh = 0x05E1;

}  // namespace

namespace views {

// Convert to Wide so that the printed string will be readable when
// check fails.
#define EXPECT_STR_EQ(ascii, utf16) \
  EXPECT_EQ(ASCIIToWide(ascii), UTF16ToWide(utf16))
#define EXPECT_STR_NE(ascii, utf16) \
  EXPECT_NE(ASCIIToWide(ascii), UTF16ToWide(utf16))

// TODO(oshima): Move tests that are independent of TextfieldViews to
// textfield_unittests.cc once we move the test utility functions
// from chrome/browser/automation/ to ui/base/test/.
class NativeTextfieldViewsTest : public ViewsTestBase,
                                 public TextfieldController {
 public:
  NativeTextfieldViewsTest()
      : widget_(NULL),
        textfield_(NULL),
        textfield_view_(NULL),
        model_(NULL),
        input_method_(NULL),
        on_before_user_action_(0),
        on_after_user_action_(0) {
  }

  // ::testing::Test:
  virtual void SetUp() {
    ViewsTestBase::SetUp();
  }

  virtual void TearDown() {
    if (widget_)
      widget_->Close();
    ViewsTestBase::TearDown();
  }

  // TextfieldController:
  virtual void ContentsChanged(Textfield* sender,
                               const string16& new_contents) OVERRIDE {
    // Paste calls TextfieldController::ContentsChanged() explicitly even if the
    // paste action did not change the content. So |new_contents| may match
    // |last_contents_|. For more info, see http://crbug.com/79002
    last_contents_ = new_contents;
  }

  virtual bool HandleKeyEvent(Textfield* sender,
                              const ui::KeyEvent& key_event) OVERRIDE {
    // TODO(oshima): figure out how to test the keystroke.
    return false;
  }

  virtual void OnBeforeUserAction(Textfield* sender) OVERRIDE {
    ++on_before_user_action_;
  }

  virtual void OnAfterUserAction(Textfield* sender) OVERRIDE {
    ++on_after_user_action_;
  }

  void InitTextfield(Textfield::StyleFlags style) {
    InitTextfields(style, 1);
  }

  void InitTextfields(Textfield::StyleFlags style, int count) {
    // Append kEnableViewsTextfield to use NativeTextfieldViews on Windows.
    CommandLine* command_line = CommandLine::ForCurrentProcess();
    command_line->AppendSwitch(switches::kEnableViewsTextfield);

    ASSERT_FALSE(textfield_);
    textfield_ = new TestTextfield(style);
    textfield_->SetController(this);
    widget_ = new Widget();
    Widget::InitParams params = CreateParams(Widget::InitParams::TYPE_POPUP);
    params.bounds = gfx::Rect(100, 100, 100, 100);
    widget_->Init(params);
    View* container = new View();
    widget_->SetContentsView(container);
    container->AddChildView(textfield_);

    textfield_view_ = static_cast<NativeTextfieldViews*>(
        textfield_->GetNativeWrapperForTesting());
    textfield_view_->SetBoundsRect(params.bounds);
    textfield_->set_id(1);

    for (int i = 1; i < count; i++) {
      Textfield* textfield = new Textfield(style);
      container->AddChildView(textfield);
      textfield->set_id(i + 1);
    }

    DCHECK(textfield_view_);
    model_ = textfield_view_->model_.get();
    model_->ClearEditHistory();

    input_method_ = new MockInputMethod();
    widget_->ReplaceInputMethod(input_method_);

    // Activate the widget and focus the textfield for input handling.
    widget_->Activate();
    textfield_->RequestFocus();
  }

  ui::MenuModel* GetContextMenuModel() {
    textfield_view_->UpdateContextMenu();
    return textfield_view_->context_menu_contents_.get();
  }

  ui::TouchSelectionController* GetTouchSelectionController() {
    return textfield_view_->touch_selection_controller_.get();
  }

 protected:
  void SendKeyEvent(ui::KeyboardCode key_code,
                    bool alt,
                    bool shift,
                    bool control,
                    bool caps_lock) {
    int flags = (alt ? ui::EF_ALT_DOWN : 0) |
                (shift ? ui::EF_SHIFT_DOWN : 0) |
                (control ? ui::EF_CONTROL_DOWN : 0) |
                (caps_lock ? ui::EF_CAPS_LOCK_DOWN : 0);
    ui::KeyEvent event(ui::ET_KEY_PRESSED, key_code, flags, false);
    input_method_->DispatchKeyEvent(event);
  }

  void SendKeyEvent(ui::KeyboardCode key_code, bool shift, bool control) {
    SendKeyEvent(key_code, false, shift, control, false);
  }

  void SendKeyEvent(ui::KeyboardCode key_code) {
    SendKeyEvent(key_code, false, false);
  }

  void SendKeyEvent(char16 ch) {
    if (ch < 0x80) {
      ui::KeyboardCode code =
          ch == ' ' ? ui::VKEY_SPACE :
          static_cast<ui::KeyboardCode>(ui::VKEY_A + ch - 'a');
      SendKeyEvent(code);
    } else {
      ui::KeyEvent event(ui::ET_KEY_PRESSED, ui::VKEY_UNKNOWN, 0, false);
      event.set_character(ch);
      input_method_->DispatchKeyEvent(event);
    }
  }

  string16 GetClipboardText() const {
    string16 text;
    ui::Clipboard::GetForCurrentThread()->
        ReadText(ui::Clipboard::BUFFER_STANDARD, &text);
    return text;
  }

  void SetClipboardText(const std::string& text) {
    ui::ScopedClipboardWriter clipboard_writer(
        ui::Clipboard::GetForCurrentThread(),
        ui::Clipboard::BUFFER_STANDARD);
    clipboard_writer.WriteText(ASCIIToUTF16(text));
  }

  View* GetFocusedView() {
    return widget_->GetFocusManager()->GetFocusedView();
  }

  int GetCursorPositionX(int cursor_pos) {
    gfx::RenderText* render_text = textfield_view_->GetRenderText();
    return render_text->GetCursorBounds(
        gfx::SelectionModel(cursor_pos, gfx::CURSOR_FORWARD), false).x();
  }

  // Get the current cursor bounds.
  gfx::Rect GetCursorBounds() {
    gfx::RenderText* render_text = textfield_view_->GetRenderText();
    gfx::Rect bounds = render_text->GetUpdatedCursorBounds();
    return bounds;
  }

  // Get the cursor bounds of |sel|.
  gfx::Rect GetCursorBounds(const gfx::SelectionModel& sel) {
    gfx::RenderText* render_text = textfield_view_->GetRenderText();
    gfx::Rect bounds = render_text->GetCursorBounds(sel, true);
    return bounds;
  }

  gfx::Rect GetDisplayRect() {
    return textfield_view_->GetRenderText()->display_rect();
  }

  // Mouse click on the point whose x-axis is |bound|'s x plus |x_offset| and
  // y-axis is in the middle of |bound|'s vertical range.
  void MouseClick(const gfx::Rect bound, int x_offset) {
    gfx::Point point(bound.x() +  x_offset, bound.y() + bound.height() / 2);
    ui::MouseEvent click(ui::ET_MOUSE_PRESSED, point, point,
                         ui::EF_LEFT_MOUSE_BUTTON);
    textfield_view_->OnMousePressed(click);
    ui::MouseEvent release(ui::ET_MOUSE_RELEASED, point, point,
                           ui::EF_LEFT_MOUSE_BUTTON);
    textfield_view_->OnMouseReleased(release);
  }

  // This is to avoid double/triple click.
  void NonClientMouseClick() {
    ui::MouseEvent click(ui::ET_MOUSE_PRESSED, gfx::Point(), gfx::Point(),
                         ui::EF_LEFT_MOUSE_BUTTON | ui::EF_IS_NON_CLIENT);
    textfield_view_->OnMousePressed(click);
    ui::MouseEvent release(ui::ET_MOUSE_RELEASED, gfx::Point(), gfx::Point(),
                           ui::EF_LEFT_MOUSE_BUTTON | ui::EF_IS_NON_CLIENT);
    textfield_view_->OnMouseReleased(release);
  }

  // Wrap for visibility in test classes.
  ui::TextInputType GetTextInputType() {
    return textfield_view_->GetTextInputType();
  }

  void VerifyTextfieldContextMenuContents(bool textfield_has_selection,
                                          ui::MenuModel* menu_model) {
    EXPECT_TRUE(menu_model->IsEnabledAt(4 /* Separator */));
    EXPECT_TRUE(menu_model->IsEnabledAt(5 /* SELECT ALL */));
    EXPECT_EQ(textfield_has_selection, menu_model->IsEnabledAt(0 /* CUT */));
    EXPECT_EQ(textfield_has_selection, menu_model->IsEnabledAt(1 /* COPY */));
    EXPECT_EQ(textfield_has_selection, menu_model->IsEnabledAt(3 /* DELETE */));
    string16 str(GetClipboardText());
    EXPECT_NE(str.empty(), menu_model->IsEnabledAt(2 /* PASTE */));
  }

  // We need widget to populate wrapper class.
  Widget* widget_;

  TestTextfield* textfield_;
  NativeTextfieldViews* textfield_view_;
  TextfieldViewsModel* model_;

  // The string from Controller::ContentsChanged callback.
  string16 last_contents_;

  // For testing input method related behaviors.
  MockInputMethod* input_method_;

  // Indicates how many times OnBeforeUserAction() is called.
  int on_before_user_action_;

  // Indicates how many times OnAfterUserAction() is called.
  int on_after_user_action_;

 private:
  DISALLOW_COPY_AND_ASSIGN(NativeTextfieldViewsTest);
};

TEST_F(NativeTextfieldViewsTest, ModelChangesTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);

  // TextfieldController::ContentsChanged() shouldn't be called when changing
  // text programmatically.
  last_contents_.clear();
  textfield_->SetText(ASCIIToUTF16("this is"));

  EXPECT_STR_EQ("this is", model_->GetText());
  EXPECT_STR_EQ("this is", textfield_->text());
  EXPECT_TRUE(last_contents_.empty());

  textfield_->AppendText(ASCIIToUTF16(" a test"));
  EXPECT_STR_EQ("this is a test", model_->GetText());
  EXPECT_STR_EQ("this is a test", textfield_->text());
  EXPECT_TRUE(last_contents_.empty());

  EXPECT_EQ(string16(), textfield_->GetSelectedText());
  textfield_->SelectAll(false);
  EXPECT_STR_EQ("this is a test", textfield_->GetSelectedText());
  EXPECT_TRUE(last_contents_.empty());
}

TEST_F(NativeTextfieldViewsTest, ModelChangesTestLowerCase) {
  // Check if |model_|'s text is properly lowercased for STYLE_LOWERCASE.
  InitTextfield(Textfield::STYLE_LOWERCASE);
  EXPECT_EQ(0U, textfield_->GetCursorPosition());

  last_contents_.clear();
  textfield_->SetText(ASCIIToUTF16("THIS IS"));
  EXPECT_EQ(7U, textfield_->GetCursorPosition());

  EXPECT_STR_EQ("this is", model_->GetText());
  EXPECT_STR_EQ("THIS IS", textfield_->text());
  EXPECT_TRUE(last_contents_.empty());

  textfield_->AppendText(ASCIIToUTF16(" A TEST"));
  EXPECT_EQ(7U, textfield_->GetCursorPosition());
  EXPECT_STR_EQ("this is a test", model_->GetText());
  EXPECT_STR_EQ("THIS IS A TEST", textfield_->text());

  EXPECT_TRUE(last_contents_.empty());
}

TEST_F(NativeTextfieldViewsTest, ModelChangesTestLowerCaseI18n) {
  // Check if lower case conversion works for non-ASCII characters.
  InitTextfield(Textfield::STYLE_LOWERCASE);
  EXPECT_EQ(0U, textfield_->GetCursorPosition());

  last_contents_.clear();
  // Zenkaku Japanese "ABCabc"
  textfield_->SetText(WideToUTF16(L"\xFF21\xFF22\xFF23\xFF41\xFF42\xFF43"));
  EXPECT_EQ(6U, textfield_->GetCursorPosition());
  // Zenkaku Japanese "abcabc"
  EXPECT_EQ(WideToUTF16(L"\xFF41\xFF42\xFF43\xFF41\xFF42\xFF43"),
            model_->GetText());
  // Zenkaku Japanese "ABCabc"
  EXPECT_EQ(WideToUTF16(L"\xFF21\xFF22\xFF23\xFF41\xFF42\xFF43"),
            textfield_->text());
  EXPECT_TRUE(last_contents_.empty());

  // Zenkaku Japanese "XYZxyz"
  textfield_->AppendText(WideToUTF16(L"\xFF38\xFF39\xFF3A\xFF58\xFF59\xFF5A"));
  EXPECT_EQ(6U, textfield_->GetCursorPosition());
  // Zenkaku Japanese "abcabcxyzxyz"
  EXPECT_EQ(WideToUTF16(L"\xFF41\xFF42\xFF43\xFF41\xFF42\xFF43"
                        L"\xFF58\xFF59\xFF5A\xFF58\xFF59\xFF5A"),
            model_->GetText());
  // Zenkaku Japanese "ABCabcXYZxyz"
  EXPECT_EQ(WideToUTF16(L"\xFF21\xFF22\xFF23\xFF41\xFF42\xFF43"
                        L"\xFF38\xFF39\xFF3A\xFF58\xFF59\xFF5A"),
            textfield_->text());
  EXPECT_TRUE(last_contents_.empty());
}

TEST_F(NativeTextfieldViewsTest, ModelChangesTestLowerCaseWithLocale) {
  // Check if lower case conversion honors locale properly.
  std::string locale = l10n_util::GetApplicationLocale("");
  base::i18n::SetICUDefaultLocale("tr");

  InitTextfield(Textfield::STYLE_LOWERCASE);
  EXPECT_EQ(0U, textfield_->GetCursorPosition());

  last_contents_.clear();
  // Turkish 'I' should be converted to dotless 'i' (U+0131).
  textfield_->SetText(WideToUTF16(L"I"));
  EXPECT_EQ(1U, textfield_->GetCursorPosition());
  EXPECT_EQ(WideToUTF16(L"\x0131"), model_->GetText());
  EXPECT_EQ(WideToUTF16(L"I"), textfield_->text());
  EXPECT_TRUE(last_contents_.empty());

  base::i18n::SetICUDefaultLocale(locale);

  // On default (en) locale, 'I' should be converted to 'i'.
  textfield_->SetText(WideToUTF16(L"I"));
  EXPECT_EQ(1U, textfield_->GetCursorPosition());
  EXPECT_EQ(WideToUTF16(L"i"), model_->GetText());
  EXPECT_EQ(WideToUTF16(L"I"), textfield_->text());
  EXPECT_TRUE(last_contents_.empty());
}

TEST_F(NativeTextfieldViewsTest, KeyTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  // Event flags:  key,    alt,   shift, ctrl,  caps-lock.
  SendKeyEvent(ui::VKEY_T, false, true,  false, false);
  SendKeyEvent(ui::VKEY_E, false, false, false, false);
  SendKeyEvent(ui::VKEY_X, false, true,  false, true);
  SendKeyEvent(ui::VKEY_T, false, false, false, true);
  SendKeyEvent(ui::VKEY_1, false, true,  false, false);
  SendKeyEvent(ui::VKEY_1, false, false, false, false);
  SendKeyEvent(ui::VKEY_1, false, true,  false, true);
  SendKeyEvent(ui::VKEY_1, false, false, false, true);
  EXPECT_STR_EQ("TexT!1!1", textfield_->text());
}

TEST_F(NativeTextfieldViewsTest, ControlAndSelectTest) {
  // Insert a test string in a textfield.
  InitTextfield(Textfield::STYLE_DEFAULT);
  textfield_->SetText(ASCIIToUTF16("one two three"));
  SendKeyEvent(ui::VKEY_HOME,  false /* shift */, false /* control */);
  SendKeyEvent(ui::VKEY_RIGHT, true, false);
  SendKeyEvent(ui::VKEY_RIGHT, true, false);
  SendKeyEvent(ui::VKEY_RIGHT, true, false);

  EXPECT_STR_EQ("one", textfield_->GetSelectedText());

  // Test word select.
  SendKeyEvent(ui::VKEY_RIGHT, true, true);
  EXPECT_STR_EQ("one two", textfield_->GetSelectedText());
  SendKeyEvent(ui::VKEY_RIGHT, true, true);
  EXPECT_STR_EQ("one two three", textfield_->GetSelectedText());
  SendKeyEvent(ui::VKEY_LEFT, true, true);
  EXPECT_STR_EQ("one two ", textfield_->GetSelectedText());
  SendKeyEvent(ui::VKEY_LEFT, true, true);
  EXPECT_STR_EQ("one ", textfield_->GetSelectedText());

  // Replace the selected text.
  SendKeyEvent(ui::VKEY_Z, true, false);
  SendKeyEvent(ui::VKEY_E, true, false);
  SendKeyEvent(ui::VKEY_R, true, false);
  SendKeyEvent(ui::VKEY_O, true, false);
  SendKeyEvent(ui::VKEY_SPACE, false, false);
  EXPECT_STR_EQ("ZERO two three", textfield_->text());

  SendKeyEvent(ui::VKEY_END, true, false);
  EXPECT_STR_EQ("two three", textfield_->GetSelectedText());
  SendKeyEvent(ui::VKEY_HOME, true, false);
  EXPECT_STR_EQ("ZERO ", textfield_->GetSelectedText());
}

TEST_F(NativeTextfieldViewsTest, InsertionDeletionTest) {
  // Insert a test string in a textfield.
  InitTextfield(Textfield::STYLE_DEFAULT);
  char test_str[] = "this is a test";
  for (size_t i = 0; i < sizeof(test_str); i++) {
    // This is ugly and should be replaced by a utility standard function.
    // See comment in NativeTextfieldViews::GetPrintableChar.
    char c = test_str[i];
    ui::KeyboardCode code =
        c == ' ' ? ui::VKEY_SPACE :
        static_cast<ui::KeyboardCode>(ui::VKEY_A + c - 'a');
    SendKeyEvent(code);
  }
  EXPECT_STR_EQ(test_str, textfield_->text());

  // Move the cursor around.
  for (int i = 0; i < 6; i++) {
    SendKeyEvent(ui::VKEY_LEFT);
  }
  SendKeyEvent(ui::VKEY_RIGHT);

  // Delete using backspace and check resulting string.
  SendKeyEvent(ui::VKEY_BACK);
  EXPECT_STR_EQ("this is  test", textfield_->text());

  // Delete using delete key and check resulting string.
  for (int i = 0; i < 5; i++) {
    SendKeyEvent(ui::VKEY_DELETE);
  }
  EXPECT_STR_EQ("this is ", textfield_->text());

  // Select all and replace with "k".
  textfield_->SelectAll(false);
  SendKeyEvent(ui::VKEY_K);
  EXPECT_STR_EQ("k", textfield_->text());

  // Delete the previous word from cursor.
  textfield_->SetText(ASCIIToUTF16("one two three four"));
  SendKeyEvent(ui::VKEY_END);
  SendKeyEvent(ui::VKEY_BACK, false, false, true, false);
  EXPECT_STR_EQ("one two three ", textfield_->text());

  // Delete upto the beginning of the buffer from cursor in chromeos, do nothing
  // in windows.
  SendKeyEvent(ui::VKEY_LEFT, false, false, true, false);
  SendKeyEvent(ui::VKEY_BACK, false, true, true, false);
#if defined(OS_WIN)
  EXPECT_STR_EQ("one two three ", textfield_->text());
#else
  EXPECT_STR_EQ("three ", textfield_->text());
#endif

  // Delete the next word from cursor.
  textfield_->SetText(ASCIIToUTF16("one two three four"));
  SendKeyEvent(ui::VKEY_HOME);
  SendKeyEvent(ui::VKEY_DELETE, false, false, true, false);
  EXPECT_STR_EQ(" two three four", textfield_->text());

  // Delete upto the end of the buffer from cursor in chromeos, do nothing
  // in windows.
  SendKeyEvent(ui::VKEY_RIGHT, false, false, true, false);
  SendKeyEvent(ui::VKEY_DELETE, false, true, true, false);
#if defined(OS_WIN)
  EXPECT_STR_EQ(" two three four", textfield_->text());
#else
  EXPECT_STR_EQ(" two", textfield_->text());
#endif
}

TEST_F(NativeTextfieldViewsTest, PasswordTest) {
  InitTextfield(Textfield::STYLE_OBSCURED);
  EXPECT_EQ(ui::TEXT_INPUT_TYPE_PASSWORD, GetTextInputType());
  EXPECT_TRUE(textfield_->enabled());
  EXPECT_TRUE(textfield_->focusable());

  last_contents_.clear();
  textfield_->SetText(ASCIIToUTF16("password"));
  // Ensure text() and the callback returns the actual text instead of "*".
  EXPECT_STR_EQ("password", textfield_->text());
  EXPECT_TRUE(last_contents_.empty());
  model_->SelectAll(false);
  SetClipboardText("foo");

  // Cut and copy should be disabled.
  EXPECT_FALSE(textfield_view_->IsCommandIdEnabled(IDS_APP_CUT));
  textfield_view_->ExecuteCommand(IDS_APP_CUT, 0);
  SendKeyEvent(ui::VKEY_X, false, true);
  EXPECT_FALSE(textfield_view_->IsCommandIdEnabled(IDS_APP_COPY));
  textfield_view_->ExecuteCommand(IDS_APP_COPY, 0);
  SendKeyEvent(ui::VKEY_C, false, true);
  SendKeyEvent(ui::VKEY_INSERT, false, true);
  EXPECT_STR_EQ("foo", string16(GetClipboardText()));
  EXPECT_STR_EQ("password", textfield_->text());

  // Paste should work normally.
  EXPECT_TRUE(textfield_view_->IsCommandIdEnabled(IDS_APP_PASTE));
  textfield_view_->ExecuteCommand(IDS_APP_PASTE, 0);
  SendKeyEvent(ui::VKEY_V, false, true);
  SendKeyEvent(ui::VKEY_INSERT, true, false);
  EXPECT_STR_EQ("foo", string16(GetClipboardText()));
  EXPECT_STR_EQ("foofoofoo", textfield_->text());
}

TEST_F(NativeTextfieldViewsTest, InputTypeSetsObscured) {
  InitTextfield(Textfield::STYLE_DEFAULT);

  // Defaults to TEXT
  EXPECT_EQ(ui::TEXT_INPUT_TYPE_TEXT, GetTextInputType());

  // Setting to TEXT_INPUT_TYPE_PASSWORD also sets obscured state of textfield.
  textfield_->SetTextInputType(ui::TEXT_INPUT_TYPE_PASSWORD);
  EXPECT_EQ(ui::TEXT_INPUT_TYPE_PASSWORD, GetTextInputType());
  EXPECT_TRUE(textfield_->IsObscured());
}

TEST_F(NativeTextfieldViewsTest, ObscuredSetsInputType) {
  InitTextfield(Textfield::STYLE_DEFAULT);

  // Defaults to TEXT
  EXPECT_EQ(ui::TEXT_INPUT_TYPE_TEXT, GetTextInputType());

  textfield_->SetObscured(true);
  EXPECT_EQ(ui::TEXT_INPUT_TYPE_PASSWORD, GetTextInputType());

  textfield_->SetObscured(false);
  EXPECT_EQ(ui::TEXT_INPUT_TYPE_TEXT, GetTextInputType());
}

TEST_F(NativeTextfieldViewsTest, TextInputType) {
  InitTextfield(Textfield::STYLE_DEFAULT);

  // Defaults to TEXT
  EXPECT_EQ(ui::TEXT_INPUT_TYPE_TEXT, GetTextInputType());

  // And can be set.
  textfield_->SetTextInputType(ui::TEXT_INPUT_TYPE_URL);
  EXPECT_EQ(ui::TEXT_INPUT_TYPE_URL, GetTextInputType());

  // Readonly textfields have type NONE
  textfield_->SetReadOnly(true);
  EXPECT_EQ(ui::TEXT_INPUT_TYPE_NONE, GetTextInputType());

  textfield_->SetReadOnly(false);
  EXPECT_EQ(ui::TEXT_INPUT_TYPE_URL, GetTextInputType());

  // As do disabled textfields
  textfield_->SetEnabled(false);
  EXPECT_EQ(ui::TEXT_INPUT_TYPE_NONE, GetTextInputType());
}

TEST_F(NativeTextfieldViewsTest, OnKeyPressReturnValueTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);

  // Character keys will be handled by input method.
  SendKeyEvent(ui::VKEY_A);
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_FALSE(textfield_->key_handled());
  textfield_->clear();

  // Home will be handled.
  SendKeyEvent(ui::VKEY_HOME);
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_TRUE(textfield_->key_handled());
  textfield_->clear();

  // F24, up/down key won't be handled.
  SendKeyEvent(ui::VKEY_F24);
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_FALSE(textfield_->key_handled());
  textfield_->clear();

  SendKeyEvent(ui::VKEY_UP);
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_FALSE(textfield_->key_handled());
  textfield_->clear();

  SendKeyEvent(ui::VKEY_DOWN);
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_FALSE(textfield_->key_handled());
  textfield_->clear();

  // Empty Textfield does not handle left/right.
  textfield_->SetText(string16());
  SendKeyEvent(ui::VKEY_LEFT);
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_FALSE(textfield_->key_handled());
  textfield_->clear();

  SendKeyEvent(ui::VKEY_RIGHT);
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_FALSE(textfield_->key_handled());
  textfield_->clear();

  // Add a char. Right key should not be handled when cursor is at the end.
  SendKeyEvent(ui::VKEY_B);
  SendKeyEvent(ui::VKEY_RIGHT);
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_FALSE(textfield_->key_handled());
  textfield_->clear();

  // First left key is handled to move cursor left to the beginning.
  SendKeyEvent(ui::VKEY_LEFT);
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_TRUE(textfield_->key_handled());
  textfield_->clear();

  // Now left key should not be handled.
  SendKeyEvent(ui::VKEY_LEFT);
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_FALSE(textfield_->key_handled());
  textfield_->clear();
}

TEST_F(NativeTextfieldViewsTest, CursorMovement) {
  InitTextfield(Textfield::STYLE_DEFAULT);

  // Test with trailing whitespace.
  textfield_->SetText(ASCIIToUTF16("one two hre "));

  // Send the cursor at the end.
  SendKeyEvent(ui::VKEY_END);

  // Ctrl+Left should move the cursor just before the last word.
  SendKeyEvent(ui::VKEY_LEFT, false, true);
  SendKeyEvent(ui::VKEY_T);
  EXPECT_STR_EQ("one two thre ", textfield_->text());
  EXPECT_STR_EQ("one two thre ", last_contents_);

  // Ctrl+Right should move the cursor to the end of the last word.
  SendKeyEvent(ui::VKEY_RIGHT, false, true);
  SendKeyEvent(ui::VKEY_E);
  EXPECT_STR_EQ("one two three ", textfield_->text());
  EXPECT_STR_EQ("one two three ", last_contents_);

  // Ctrl+Right again should move the cursor to the end.
  SendKeyEvent(ui::VKEY_RIGHT, false, true);
  SendKeyEvent(ui::VKEY_BACK);
  EXPECT_STR_EQ("one two three", textfield_->text());
  EXPECT_STR_EQ("one two three", last_contents_);

  // Test with leading whitespace.
  textfield_->SetText(ASCIIToUTF16(" ne two"));

  // Send the cursor at the beginning.
  SendKeyEvent(ui::VKEY_HOME);

  // Ctrl+Right, then Ctrl+Left should move the cursor to the beginning of the
  // first word.
  SendKeyEvent(ui::VKEY_RIGHT, false, true);
  SendKeyEvent(ui::VKEY_LEFT, false, true);
  SendKeyEvent(ui::VKEY_O);
  EXPECT_STR_EQ(" one two", textfield_->text());
  EXPECT_STR_EQ(" one two", last_contents_);

  // Ctrl+Left to move the cursor to the beginning of the first word.
  SendKeyEvent(ui::VKEY_LEFT, false, true);
  // Ctrl+Left again should move the cursor back to the very beginning.
  SendKeyEvent(ui::VKEY_LEFT, false, true);
  SendKeyEvent(ui::VKEY_DELETE);
  EXPECT_STR_EQ("one two", textfield_->text());
  EXPECT_STR_EQ("one two", last_contents_);
}

TEST_F(NativeTextfieldViewsTest, FocusTraversalTest) {
  InitTextfields(Textfield::STYLE_DEFAULT, 3);
  textfield_->RequestFocus();

  EXPECT_EQ(1, GetFocusedView()->id());
  widget_->GetFocusManager()->AdvanceFocus(false);
  EXPECT_EQ(2, GetFocusedView()->id());
  widget_->GetFocusManager()->AdvanceFocus(false);
  EXPECT_EQ(3, GetFocusedView()->id());
  // Cycle back to the first textfield.
  widget_->GetFocusManager()->AdvanceFocus(false);
  EXPECT_EQ(1, GetFocusedView()->id());

  widget_->GetFocusManager()->AdvanceFocus(true);
  EXPECT_EQ(3, GetFocusedView()->id());
  widget_->GetFocusManager()->AdvanceFocus(true);
  EXPECT_EQ(2, GetFocusedView()->id());
  widget_->GetFocusManager()->AdvanceFocus(true);
  EXPECT_EQ(1, GetFocusedView()->id());
  // Cycle back to the last textfield.
  widget_->GetFocusManager()->AdvanceFocus(true);
  EXPECT_EQ(3, GetFocusedView()->id());

  // Request focus should still work.
  textfield_->RequestFocus();
  EXPECT_EQ(1, GetFocusedView()->id());

  // Test if clicking on textfield view sets the focus to textfield_.
  widget_->GetFocusManager()->AdvanceFocus(true);
  EXPECT_EQ(3, GetFocusedView()->id());
  ui::MouseEvent click(ui::ET_MOUSE_PRESSED, gfx::Point(), gfx::Point(),
                       ui::EF_LEFT_MOUSE_BUTTON);
  textfield_view_->OnMousePressed(click);
  EXPECT_EQ(1, GetFocusedView()->id());
}

TEST_F(NativeTextfieldViewsTest, ContextMenuDisplayTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  textfield_->SetText(ASCIIToUTF16("hello world"));
  EXPECT_TRUE(GetContextMenuModel());
  VerifyTextfieldContextMenuContents(false, GetContextMenuModel());

  textfield_->SelectAll(false);
  VerifyTextfieldContextMenuContents(true, GetContextMenuModel());
}

TEST_F(NativeTextfieldViewsTest, DoubleAndTripleClickTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  textfield_->SetText(ASCIIToUTF16("hello world"));
  ui::MouseEvent click(ui::ET_MOUSE_PRESSED, gfx::Point(), gfx::Point(),
                       ui::EF_LEFT_MOUSE_BUTTON);
  ui::MouseEvent release(ui::ET_MOUSE_RELEASED, gfx::Point(), gfx::Point(),
                         ui::EF_LEFT_MOUSE_BUTTON);
  ui::MouseEvent double_click(
      ui::ET_MOUSE_PRESSED, gfx::Point(), gfx::Point(),
      ui::EF_LEFT_MOUSE_BUTTON | ui::EF_IS_DOUBLE_CLICK);

  // Test for double click.
  textfield_view_->OnMousePressed(click);
  textfield_view_->OnMouseReleased(release);
  EXPECT_TRUE(textfield_->GetSelectedText().empty());
  textfield_view_->OnMousePressed(double_click);
  textfield_view_->OnMouseReleased(release);
  EXPECT_STR_EQ("hello", textfield_->GetSelectedText());

  // Test for triple click.
  textfield_view_->OnMousePressed(click);
  textfield_view_->OnMouseReleased(release);
  EXPECT_STR_EQ("hello world", textfield_->GetSelectedText());

  // Another click should reset back to single click.
  textfield_view_->OnMousePressed(click);
  textfield_view_->OnMouseReleased(release);
  EXPECT_TRUE(textfield_->GetSelectedText().empty());
}

TEST_F(NativeTextfieldViewsTest, DragToSelect) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  textfield_->SetText(ASCIIToUTF16("hello world"));
  const int kStart = GetCursorPositionX(5);
  const int kEnd = 500;
  gfx::Point start_point(kStart, 0);
  gfx::Point end_point(kEnd, 0);
  ui::MouseEvent click_a(ui::ET_MOUSE_PRESSED, start_point, start_point,
                         ui::EF_LEFT_MOUSE_BUTTON);
  ui::MouseEvent click_b(ui::ET_MOUSE_PRESSED, end_point, end_point,
                         ui::EF_LEFT_MOUSE_BUTTON);
  ui::MouseEvent drag_left(ui::ET_MOUSE_DRAGGED, gfx::Point(), gfx::Point(),
                           ui::EF_LEFT_MOUSE_BUTTON);
  ui::MouseEvent drag_right(ui::ET_MOUSE_DRAGGED, end_point, end_point,
                            ui::EF_LEFT_MOUSE_BUTTON);
  ui::MouseEvent release(ui::ET_MOUSE_RELEASED, end_point, end_point,
                         ui::EF_LEFT_MOUSE_BUTTON);
  textfield_view_->OnMousePressed(click_a);
  EXPECT_TRUE(textfield_->GetSelectedText().empty());
  // Check that dragging left selects the beginning of the string.
  textfield_view_->OnMouseDragged(drag_left);
  string16 text_left = textfield_->GetSelectedText();
  EXPECT_STR_EQ("hello", text_left);
  // Check that dragging right selects the rest of the string.
  textfield_view_->OnMouseDragged(drag_right);
  string16 text_right = textfield_->GetSelectedText();
  EXPECT_STR_EQ(" world", text_right);
  // Check that releasing in the same location does not alter the selection.
  textfield_view_->OnMouseReleased(release);
  EXPECT_EQ(text_right, textfield_->GetSelectedText());
  // Check that dragging from beyond the text length works too.
  textfield_view_->OnMousePressed(click_b);
  textfield_view_->OnMouseDragged(drag_left);
  textfield_view_->OnMouseReleased(release);
  EXPECT_EQ(textfield_->text(), textfield_->GetSelectedText());
}

#if defined(OS_WIN)
TEST_F(NativeTextfieldViewsTest, DragAndDrop_AcceptDrop) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  textfield_->SetText(ASCIIToUTF16("hello world"));

  ui::OSExchangeData data;
  string16 string(ASCIIToUTF16("string "));
  data.SetString(string);
  int formats = 0;
  std::set<OSExchangeData::CustomFormat> custom_formats;

  // Ensure that disabled textfields do not accept drops.
  textfield_->SetEnabled(false);
  EXPECT_FALSE(textfield_view_->GetDropFormats(&formats, &custom_formats));
  EXPECT_EQ(0, formats);
  EXPECT_TRUE(custom_formats.empty());
  EXPECT_FALSE(textfield_view_->CanDrop(data));
  textfield_->SetEnabled(true);

  // Ensure that read-only textfields do not accept drops.
  textfield_->SetReadOnly(true);
  EXPECT_FALSE(textfield_view_->GetDropFormats(&formats, &custom_formats));
  EXPECT_EQ(0, formats);
  EXPECT_TRUE(custom_formats.empty());
  EXPECT_FALSE(textfield_view_->CanDrop(data));
  textfield_->SetReadOnly(false);

  // Ensure that enabled and editable textfields do accept drops.
  EXPECT_TRUE(textfield_view_->GetDropFormats(&formats, &custom_formats));
  EXPECT_EQ(ui::OSExchangeData::STRING, formats);
  EXPECT_TRUE(custom_formats.empty());
  EXPECT_TRUE(textfield_view_->CanDrop(data));
  gfx::Point drop_point(GetCursorPositionX(6), 0);
  ui::DropTargetEvent drop(data, drop_point, drop_point,
      ui::DragDropTypes::DRAG_COPY | ui::DragDropTypes::DRAG_MOVE);
  EXPECT_EQ(ui::DragDropTypes::DRAG_COPY | ui::DragDropTypes::DRAG_MOVE,
            textfield_view_->OnDragUpdated(drop));
  EXPECT_EQ(ui::DragDropTypes::DRAG_COPY, textfield_view_->OnPerformDrop(drop));
  EXPECT_STR_EQ("hello string world", textfield_->text());

  // Ensure that textfields do not accept non-OSExchangeData::STRING types.
  ui::OSExchangeData bad_data;
  bad_data.SetFilename(base::FilePath(FILE_PATH_LITERAL("x")));
#if defined(OS_WIN)
  ui::OSExchangeData::CustomFormat fmt = CF_BITMAP;
  bad_data.SetPickledData(fmt, Pickle());
  bad_data.SetFileContents(base::FilePath(L"x"), "x");
  bad_data.SetHtml(string16(ASCIIToUTF16("x")), GURL("x.org"));
  ui::OSExchangeData::DownloadFileInfo download(base::FilePath(), NULL);
  bad_data.SetDownloadFileInfo(download);
#endif
  EXPECT_FALSE(textfield_view_->CanDrop(bad_data));
}
#endif

// TODO(erg): Disabled while the other half of drag and drop is being written.
// http://crbug.com/130806
#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
#define MAYBE_DragAndDrop_InitiateDrag DISABLED_DragAndDrop_InitiateDrag
#else
#define MAYBE_DragAndDrop_InitiateDrag DragAndDrop_InitiateDrag
#endif
TEST_F(NativeTextfieldViewsTest, MAYBE_DragAndDrop_InitiateDrag) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  textfield_->SetText(ASCIIToUTF16("hello string world"));

  // Ensure the textfield will provide selected text for drag data.
  string16 string;
  ui::OSExchangeData data;
  const ui::Range kStringRange(6, 12);
  textfield_->SelectRange(kStringRange);
  const gfx::Point kStringPoint(GetCursorPositionX(9), 0);
  textfield_view_->WriteDragDataForView(NULL, kStringPoint, &data);
  EXPECT_TRUE(data.GetString(&string));
  EXPECT_EQ(textfield_->GetSelectedText(), string);

  // Ensure that disabled textfields do not support drag operations.
  textfield_->SetEnabled(false);
  EXPECT_EQ(ui::DragDropTypes::DRAG_NONE,
            textfield_view_->GetDragOperationsForView(NULL, kStringPoint));
  textfield_->SetEnabled(true);
  // Ensure that textfields without selections do not support drag operations.
  textfield_->ClearSelection();
  EXPECT_EQ(ui::DragDropTypes::DRAG_NONE,
            textfield_view_->GetDragOperationsForView(NULL, kStringPoint));
  textfield_->SelectRange(kStringRange);
  // Ensure that password textfields do not support drag operations.
  textfield_->SetObscured(true);
  EXPECT_EQ(ui::DragDropTypes::DRAG_NONE,
            textfield_view_->GetDragOperationsForView(NULL, kStringPoint));
  textfield_->SetObscured(false);
  // Ensure that textfields only initiate drag operations inside the selection.
  ui::MouseEvent press_event(ui::ET_MOUSE_PRESSED, kStringPoint, kStringPoint,
                             ui::EF_LEFT_MOUSE_BUTTON);
  textfield_view_->OnMousePressed(press_event);
  EXPECT_EQ(ui::DragDropTypes::DRAG_NONE,
            textfield_view_->GetDragOperationsForView(NULL, gfx::Point()));
  EXPECT_FALSE(textfield_view_->CanStartDragForView(NULL, gfx::Point(),
                                                    gfx::Point()));
  EXPECT_EQ(ui::DragDropTypes::DRAG_COPY,
            textfield_view_->GetDragOperationsForView(NULL, kStringPoint));
  EXPECT_TRUE(textfield_view_->CanStartDragForView(NULL, kStringPoint,
                                                   gfx::Point()));
  // Ensure that textfields support local moves.
  EXPECT_EQ(ui::DragDropTypes::DRAG_MOVE | ui::DragDropTypes::DRAG_COPY,
      textfield_view_->GetDragOperationsForView(textfield_view_, kStringPoint));
}

// TODO(erg): Disabled while the other half of drag and drop is being written.
// http://crbug.com/130806
#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
#define MAYBE_DragAndDrop_ToTheRight DISABLED_DragAndDrop_ToTheRight
#else
#define MAYBE_DragAndDrop_ToTheRight DragAndDrop_ToTheRight
#endif
TEST_F(NativeTextfieldViewsTest, MAYBE_DragAndDrop_ToTheRight) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  textfield_->SetText(ASCIIToUTF16("hello world"));

  string16 string;
  ui::OSExchangeData data;
  int formats = 0;
  int operations = 0;
  std::set<OSExchangeData::CustomFormat> custom_formats;

  // Start dragging "ello".
  textfield_->SelectRange(ui::Range(1, 5));
  gfx::Point point(GetCursorPositionX(3), 0);
  ui::MouseEvent click_a(ui::ET_MOUSE_PRESSED, point, point,
                         ui::EF_LEFT_MOUSE_BUTTON);
  textfield_view_->OnMousePressed(click_a);
  EXPECT_TRUE(textfield_view_->CanStartDragForView(textfield_view_,
                  click_a.location(), gfx::Point()));
  operations = textfield_view_->GetDragOperationsForView(textfield_view_,
                                                         click_a.location());
  EXPECT_EQ(ui::DragDropTypes::DRAG_MOVE | ui::DragDropTypes::DRAG_COPY,
            operations);
  textfield_view_->WriteDragDataForView(NULL, click_a.location(), &data);
  EXPECT_TRUE(data.GetString(&string));
  EXPECT_EQ(textfield_->GetSelectedText(), string);
  EXPECT_TRUE(textfield_view_->GetDropFormats(&formats, &custom_formats));
  EXPECT_EQ(ui::OSExchangeData::STRING, formats);
  EXPECT_TRUE(custom_formats.empty());

  // Drop "ello" after "w".
  const gfx::Point kDropPoint(GetCursorPositionX(7), 0);
  EXPECT_TRUE(textfield_view_->CanDrop(data));
  ui::DropTargetEvent drop_a(data, kDropPoint, kDropPoint, operations);
  EXPECT_EQ(ui::DragDropTypes::DRAG_MOVE,
            textfield_view_->OnDragUpdated(drop_a));
  EXPECT_EQ(ui::DragDropTypes::DRAG_MOVE,
            textfield_view_->OnPerformDrop(drop_a));
  EXPECT_STR_EQ("h welloorld", textfield_->text());
  textfield_view_->OnDragDone();

  // Undo/Redo the drag&drop change.
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("hello world", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("hello world", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("h welloorld", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("h welloorld", textfield_->text());
}

// TODO(erg): Disabled while the other half of drag and drop is being written.
// http://crbug.com/130806
#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
#define MAYBE_DragAndDrop_ToTheLeft DISABLED_DragAndDrop_ToTheLeft
#else
#define MAYBE_DragAndDrop_ToTheLeft DragAndDrop_ToTheLeft
#endif
TEST_F(NativeTextfieldViewsTest, MAYBE_DragAndDrop_ToTheLeft) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  textfield_->SetText(ASCIIToUTF16("hello world"));

  string16 string;
  ui::OSExchangeData data;
  int formats = 0;
  int operations = 0;
  std::set<OSExchangeData::CustomFormat> custom_formats;

  // Start dragging " worl".
  textfield_->SelectRange(ui::Range(5, 10));
  gfx::Point point(GetCursorPositionX(7), 0);
  ui::MouseEvent click_a(ui::ET_MOUSE_PRESSED, point, point,
                         ui::EF_LEFT_MOUSE_BUTTON);
  textfield_view_->OnMousePressed(click_a);
  EXPECT_TRUE(textfield_view_->CanStartDragForView(textfield_view_,
                  click_a.location(), gfx::Point()));
  operations = textfield_view_->GetDragOperationsForView(textfield_view_,
                                                         click_a.location());
  EXPECT_EQ(ui::DragDropTypes::DRAG_MOVE | ui::DragDropTypes::DRAG_COPY,
            operations);
  textfield_view_->WriteDragDataForView(NULL, click_a.location(), &data);
  EXPECT_TRUE(data.GetString(&string));
  EXPECT_EQ(textfield_->GetSelectedText(), string);
  EXPECT_TRUE(textfield_view_->GetDropFormats(&formats, &custom_formats));
  EXPECT_EQ(ui::OSExchangeData::STRING, formats);
  EXPECT_TRUE(custom_formats.empty());

  // Drop " worl" after "h".
  EXPECT_TRUE(textfield_view_->CanDrop(data));
  gfx::Point drop_point(GetCursorPositionX(1), 0);
  ui::DropTargetEvent drop_a(data, drop_point, drop_point, operations);
  EXPECT_EQ(ui::DragDropTypes::DRAG_MOVE,
            textfield_view_->OnDragUpdated(drop_a));
  EXPECT_EQ(ui::DragDropTypes::DRAG_MOVE,
            textfield_view_->OnPerformDrop(drop_a));
  EXPECT_STR_EQ("h worlellod", textfield_->text());
  textfield_view_->OnDragDone();

  // Undo/Redo the drag&drop change.
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("hello world", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("hello world", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("h worlellod", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("h worlellod", textfield_->text());
}

// TODO(erg): Disabled while the other half of drag and drop is being written.
// http://crbug.com/130806
#if defined(OS_LINUX) && !defined(OS_CHROMEOS)
#define MAYBE_DragAndDrop_Canceled DISABLED_DragAndDrop_Canceled
#else
#define MAYBE_DragAndDrop_Canceled DragAndDrop_Canceled
#endif
TEST_F(NativeTextfieldViewsTest, MAYBE_DragAndDrop_Canceled) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  textfield_->SetText(ASCIIToUTF16("hello world"));

  // Start dragging "worl".
  textfield_->SelectRange(ui::Range(6, 10));
  gfx::Point point(GetCursorPositionX(8), 0);
  ui::MouseEvent click(ui::ET_MOUSE_PRESSED, point, point,
                       ui::EF_LEFT_MOUSE_BUTTON);
  textfield_view_->OnMousePressed(click);
  ui::OSExchangeData data;
  textfield_view_->WriteDragDataForView(NULL, click.location(), &data);
  EXPECT_TRUE(textfield_view_->CanDrop(data));
  // Drag the text over somewhere valid, outside the current selection.
  gfx::Point drop_point(GetCursorPositionX(2), 0);
  ui::DropTargetEvent drop(data, drop_point, drop_point,
                           ui::DragDropTypes::DRAG_MOVE);
  EXPECT_EQ(ui::DragDropTypes::DRAG_MOVE, textfield_view_->OnDragUpdated(drop));
  // "Cancel" the drag, via move and release over the selection, and OnDragDone.
  gfx::Point drag_point(GetCursorPositionX(9), 0);
  ui::MouseEvent drag(ui::ET_MOUSE_DRAGGED, drag_point, drag_point,
                      ui::EF_LEFT_MOUSE_BUTTON);
  ui::MouseEvent release(ui::ET_MOUSE_RELEASED, drag_point, drag_point,
                         ui::EF_LEFT_MOUSE_BUTTON);
  textfield_view_->OnMouseDragged(drag);
  textfield_view_->OnMouseReleased(release);
  textfield_view_->OnDragDone();
  EXPECT_EQ(ASCIIToUTF16("hello world"), textfield_->text());
}

TEST_F(NativeTextfieldViewsTest, ReadOnlyTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  textfield_->SetText(ASCIIToUTF16("read only"));
  textfield_->SetReadOnly(true);
  EXPECT_TRUE(textfield_->enabled());
  EXPECT_TRUE(textfield_->focusable());

  SendKeyEvent(ui::VKEY_HOME);
  EXPECT_EQ(0U, textfield_->GetCursorPosition());
  SendKeyEvent(ui::VKEY_END);
  EXPECT_EQ(9U, textfield_->GetCursorPosition());

  SendKeyEvent(ui::VKEY_LEFT, false, false);
  EXPECT_EQ(8U, textfield_->GetCursorPosition());
  SendKeyEvent(ui::VKEY_LEFT, false, true);
  EXPECT_EQ(5U, textfield_->GetCursorPosition());
  SendKeyEvent(ui::VKEY_LEFT, true, true);
  EXPECT_EQ(0U, textfield_->GetCursorPosition());
  EXPECT_STR_EQ("read ", textfield_->GetSelectedText());
  textfield_->SelectAll(false);
  EXPECT_STR_EQ("read only", textfield_->GetSelectedText());

  // Cut should be disabled.
  SetClipboardText("Test");
  EXPECT_FALSE(textfield_view_->IsCommandIdEnabled(IDS_APP_CUT));
  textfield_view_->ExecuteCommand(IDS_APP_CUT, 0);
  SendKeyEvent(ui::VKEY_X, false, true);
  EXPECT_STR_EQ("Test", string16(GetClipboardText()));
  EXPECT_STR_EQ("read only", textfield_->text());

  // Paste should be disabled.
  EXPECT_FALSE(textfield_view_->IsCommandIdEnabled(IDS_APP_PASTE));
  textfield_view_->ExecuteCommand(IDS_APP_PASTE, 0);
  SendKeyEvent(ui::VKEY_V, false, true);
  SendKeyEvent(ui::VKEY_INSERT, true, false);
  EXPECT_STR_EQ("read only", textfield_->text());

  // Copy should work normally.
  SetClipboardText("Test");
  EXPECT_TRUE(textfield_view_->IsCommandIdEnabled(IDS_APP_COPY));
  textfield_view_->ExecuteCommand(IDS_APP_COPY, 0);
  EXPECT_STR_EQ("read only", string16(GetClipboardText()));
  SetClipboardText("Test");
  SendKeyEvent(ui::VKEY_C, false, true);
  EXPECT_STR_EQ("read only", string16(GetClipboardText()));
  SetClipboardText("Test");
  SendKeyEvent(ui::VKEY_INSERT, false, true);
  EXPECT_STR_EQ("read only", string16(GetClipboardText()));

  // SetText should work even in read only mode.
  textfield_->SetText(ASCIIToUTF16(" four five six "));
  EXPECT_STR_EQ(" four five six ", textfield_->text());

  textfield_->SelectAll(false);
  EXPECT_STR_EQ(" four five six ", textfield_->GetSelectedText());

  // Text field is unmodifiable and selection shouldn't change.
  SendKeyEvent(ui::VKEY_DELETE);
  EXPECT_STR_EQ(" four five six ", textfield_->GetSelectedText());
  SendKeyEvent(ui::VKEY_BACK);
  EXPECT_STR_EQ(" four five six ", textfield_->GetSelectedText());
  SendKeyEvent(ui::VKEY_T);
  EXPECT_STR_EQ(" four five six ", textfield_->GetSelectedText());
}

TEST_F(NativeTextfieldViewsTest, TextInputClientTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  ui::TextInputClient* client = textfield_->GetTextInputClient();
  EXPECT_TRUE(client);
  EXPECT_EQ(ui::TEXT_INPUT_TYPE_TEXT, client->GetTextInputType());

  textfield_->SetText(ASCIIToUTF16("0123456789"));
  ui::Range range;
  EXPECT_TRUE(client->GetTextRange(&range));
  EXPECT_EQ(0U, range.start());
  EXPECT_EQ(10U, range.end());

  EXPECT_TRUE(client->SetSelectionRange(ui::Range(1, 4)));
  EXPECT_TRUE(client->GetSelectionRange(&range));
  EXPECT_EQ(ui::Range(1, 4), range);

  // This code can't be compiled because of a bug in base::Callback.
#if 0
  GetTextHelper helper;
  base::Callback<void(string16)> callback =
      base::Bind(&GetTextHelper::set_text, base::Unretained(&helper));

  EXPECT_TRUE(client->GetTextFromRange(range, callback));
  EXPECT_STR_EQ("123", helper.text());
#endif

  EXPECT_TRUE(client->DeleteRange(range));
  EXPECT_STR_EQ("0456789", textfield_->text());

  ui::CompositionText composition;
  composition.text = UTF8ToUTF16("321");
  // Set composition through input method.
  input_method_->Clear();
  input_method_->SetCompositionTextForNextKey(composition);
  textfield_->clear();

  on_before_user_action_ = on_after_user_action_ = 0;
  SendKeyEvent(ui::VKEY_A);
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_FALSE(textfield_->key_handled());
  EXPECT_TRUE(client->HasCompositionText());
  EXPECT_TRUE(client->GetCompositionTextRange(&range));
  EXPECT_STR_EQ("0321456789", textfield_->text());
  EXPECT_EQ(ui::Range(1, 4), range);
  EXPECT_EQ(2, on_before_user_action_);
  EXPECT_EQ(2, on_after_user_action_);

  input_method_->SetResultTextForNextKey(UTF8ToUTF16("123"));
  on_before_user_action_ = on_after_user_action_ = 0;
  textfield_->clear();
  SendKeyEvent(ui::VKEY_A);
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_FALSE(textfield_->key_handled());
  EXPECT_FALSE(client->HasCompositionText());
  EXPECT_FALSE(input_method_->cancel_composition_called());
  EXPECT_STR_EQ("0123456789", textfield_->text());
  EXPECT_EQ(2, on_before_user_action_);
  EXPECT_EQ(2, on_after_user_action_);

  input_method_->Clear();
  input_method_->SetCompositionTextForNextKey(composition);
  textfield_->clear();
  SendKeyEvent(ui::VKEY_A);
  EXPECT_TRUE(client->HasCompositionText());
  EXPECT_STR_EQ("0123321456789", textfield_->text());

  on_before_user_action_ = on_after_user_action_ = 0;
  textfield_->clear();
  SendKeyEvent(ui::VKEY_RIGHT);
  EXPECT_FALSE(client->HasCompositionText());
  EXPECT_TRUE(input_method_->cancel_composition_called());
  EXPECT_TRUE(textfield_->key_received());
  EXPECT_TRUE(textfield_->key_handled());
  EXPECT_STR_EQ("0123321456789", textfield_->text());
  EXPECT_EQ(8U, textfield_->GetCursorPosition());
  EXPECT_EQ(1, on_before_user_action_);
  EXPECT_EQ(1, on_after_user_action_);

  // On{Before,After}UserAction should be called by whatever user action
  // triggers clearing or setting a selection if appropriate.
  on_before_user_action_ = on_after_user_action_ = 0;
  textfield_->clear();
  textfield_->ClearSelection();
  textfield_->SelectAll(false);
  EXPECT_EQ(0, on_before_user_action_);
  EXPECT_EQ(0, on_after_user_action_);

  input_method_->Clear();
  textfield_->SetReadOnly(true);
  EXPECT_TRUE(input_method_->text_input_type_changed());
  EXPECT_FALSE(textfield_->GetTextInputClient());

  textfield_->SetReadOnly(false);
  input_method_->Clear();
  textfield_->SetObscured(true);
  EXPECT_TRUE(input_method_->text_input_type_changed());
  EXPECT_TRUE(textfield_->GetTextInputClient());
}

TEST_F(NativeTextfieldViewsTest, UndoRedoTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  SendKeyEvent(ui::VKEY_A);
  EXPECT_STR_EQ("a", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("a", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("a", textfield_->text());

  // AppendText
  textfield_->AppendText(ASCIIToUTF16("b"));
  last_contents_.clear();  // AppendText doesn't call ContentsChanged.
  EXPECT_STR_EQ("ab", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("a", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("ab", textfield_->text());

  // SetText
  SendKeyEvent(ui::VKEY_C);
  // Undo'ing append moves the cursor to the end for now.
  // no-op SetText won't add new edit. See TextfieldViewsModel::SetText
  // description.
  EXPECT_STR_EQ("abc", textfield_->text());
  textfield_->SetText(ASCIIToUTF16("abc"));
  EXPECT_STR_EQ("abc", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("ab", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("abc", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("abc", textfield_->text());
  textfield_->SetText(ASCIIToUTF16("123"));
  textfield_->SetText(ASCIIToUTF16("123"));
  EXPECT_STR_EQ("123", textfield_->text());
  SendKeyEvent(ui::VKEY_END, false, false);
  SendKeyEvent(ui::VKEY_4, false, false);
  EXPECT_STR_EQ("1234", textfield_->text());
  last_contents_.clear();
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("123", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  // the insert edit "c" and set edit "123" are merged to single edit,
  // so text becomes "ab" after undo.
  EXPECT_STR_EQ("ab", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("a", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("ab", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("123", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("1234", textfield_->text());

  // Undoing to the same text shouldn't call ContentsChanged.
  SendKeyEvent(ui::VKEY_A, false, true);  // select all
  SendKeyEvent(ui::VKEY_A);
  EXPECT_STR_EQ("a", textfield_->text());
  SendKeyEvent(ui::VKEY_B);
  SendKeyEvent(ui::VKEY_C);
  EXPECT_STR_EQ("abc", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("1234", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("abc", textfield_->text());

  // Delete/Backspace
  SendKeyEvent(ui::VKEY_BACK);
  EXPECT_STR_EQ("ab", textfield_->text());
  SendKeyEvent(ui::VKEY_HOME);
  SendKeyEvent(ui::VKEY_DELETE);
  EXPECT_STR_EQ("b", textfield_->text());
  SendKeyEvent(ui::VKEY_A, false, true);
  SendKeyEvent(ui::VKEY_DELETE);
  EXPECT_STR_EQ("", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("b", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("ab", textfield_->text());
  SendKeyEvent(ui::VKEY_Z, false, true);
  EXPECT_STR_EQ("abc", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("ab", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("b", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("", textfield_->text());
  SendKeyEvent(ui::VKEY_Y, false, true);
  EXPECT_STR_EQ("", textfield_->text());
}

TEST_F(NativeTextfieldViewsTest, CutCopyPaste) {
  InitTextfield(Textfield::STYLE_DEFAULT);

  // Ensure IDS_APP_CUT cuts.
  textfield_->SetText(ASCIIToUTF16("123"));
  textfield_->SelectAll(false);
  EXPECT_TRUE(textfield_view_->IsCommandIdEnabled(IDS_APP_CUT));
  textfield_view_->ExecuteCommand(IDS_APP_CUT, 0);
  EXPECT_STR_EQ("123", string16(GetClipboardText()));
  EXPECT_STR_EQ("", textfield_->text());

  // Ensure [Ctrl]+[x] cuts and [Ctrl]+[Alt][x] does nothing.
  textfield_->SetText(ASCIIToUTF16("456"));
  textfield_->SelectAll(false);
  SendKeyEvent(ui::VKEY_X, true, false, true, false);
  EXPECT_STR_EQ("123", string16(GetClipboardText()));
  EXPECT_STR_EQ("456", textfield_->text());
  SendKeyEvent(ui::VKEY_X, false, true);
  EXPECT_STR_EQ("456", string16(GetClipboardText()));
  EXPECT_STR_EQ("", textfield_->text());

  // Ensure IDS_APP_COPY copies.
  textfield_->SetText(ASCIIToUTF16("789"));
  textfield_->SelectAll(false);
  EXPECT_TRUE(textfield_view_->IsCommandIdEnabled(IDS_APP_COPY));
  textfield_view_->ExecuteCommand(IDS_APP_COPY, 0);
  EXPECT_STR_EQ("789", string16(GetClipboardText()));

  // Ensure [Ctrl]+[c] copies and [Ctrl]+[Alt][c] does nothing.
  textfield_->SetText(ASCIIToUTF16("012"));
  textfield_->SelectAll(false);
  SendKeyEvent(ui::VKEY_C, true, false, true, false);
  EXPECT_STR_EQ("789", string16(GetClipboardText()));
  SendKeyEvent(ui::VKEY_C, false, true);
  EXPECT_STR_EQ("012", string16(GetClipboardText()));

  // Ensure [Ctrl]+[Insert] copies.
  textfield_->SetText(ASCIIToUTF16("345"));
  textfield_->SelectAll(false);
  SendKeyEvent(ui::VKEY_INSERT, false, true);
  EXPECT_STR_EQ("345", string16(GetClipboardText()));
  EXPECT_STR_EQ("345", textfield_->text());

  // Ensure IDS_APP_PASTE, [Ctrl]+[V], and [Shift]+[Insert] pastes;
  // also ensure that [Ctrl]+[Alt]+[V] does nothing.
  SetClipboardText("abc");
  textfield_->SetText(string16());
  EXPECT_TRUE(textfield_view_->IsCommandIdEnabled(IDS_APP_PASTE));
  textfield_view_->ExecuteCommand(IDS_APP_PASTE, 0);
  EXPECT_STR_EQ("abc", textfield_->text());
  SendKeyEvent(ui::VKEY_V, false, true);
  EXPECT_STR_EQ("abcabc", textfield_->text());
  SendKeyEvent(ui::VKEY_INSERT, true, false);
  EXPECT_STR_EQ("abcabcabc", textfield_->text());
  SendKeyEvent(ui::VKEY_V, true, false, true, false);
  EXPECT_STR_EQ("abcabcabc", textfield_->text());

  // Ensure [Ctrl]+[Shift]+[Insert] is a no-op.
  textfield_->SelectAll(false);
  SendKeyEvent(ui::VKEY_INSERT, true, true);
  EXPECT_STR_EQ("abc", string16(GetClipboardText()));
  EXPECT_STR_EQ("abcabcabc", textfield_->text());
}

TEST_F(NativeTextfieldViewsTest, OvertypeMode) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  // Overtype mode should be disabled (no-op [Insert]).
  textfield_->SetText(ASCIIToUTF16("2"));
  SendKeyEvent(ui::VKEY_HOME);
  SendKeyEvent(ui::VKEY_INSERT);
  SendKeyEvent(ui::VKEY_1, false, false);
  EXPECT_STR_EQ("12", textfield_->text());
}

TEST_F(NativeTextfieldViewsTest, TextCursorDisplayTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  // LTR-RTL string in LTR context.
  SendKeyEvent('a');
  EXPECT_STR_EQ("a", textfield_->text());
  int x = GetCursorBounds().x();
  int prev_x = x;

  SendKeyEvent('b');
  EXPECT_STR_EQ("ab", textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_LT(prev_x, x);
  prev_x = x;

  SendKeyEvent(0x05E1);
  EXPECT_EQ(WideToUTF16(L"ab\x05E1"), textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_EQ(prev_x, x);

  SendKeyEvent(0x05E2);
  EXPECT_EQ(WideToUTF16(L"ab\x05E1\x5E2"), textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_EQ(prev_x, x);

  // Clear text.
  SendKeyEvent(ui::VKEY_A, false, true);
  SendKeyEvent('\n');

  // RTL-LTR string in LTR context.
  SendKeyEvent(0x05E1);
  EXPECT_EQ(WideToUTF16(L"\x05E1"), textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_EQ(GetDisplayRect().x(), x);
  prev_x = x;

  SendKeyEvent(0x05E2);
  EXPECT_EQ(WideToUTF16(L"\x05E1\x05E2"), textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_EQ(prev_x, x);

  SendKeyEvent('a');
  EXPECT_EQ(WideToUTF16(L"\x05E1\x5E2"L"a"), textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_LT(prev_x, x);
  prev_x = x;

  SendKeyEvent('b');
  EXPECT_EQ(WideToUTF16(L"\x05E1\x5E2"L"ab"), textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_LT(prev_x, x);
}

TEST_F(NativeTextfieldViewsTest, TextCursorDisplayInRTLTest) {
  std::string locale = l10n_util::GetApplicationLocale("");
  base::i18n::SetICUDefaultLocale("he");

  InitTextfield(Textfield::STYLE_DEFAULT);
  // LTR-RTL string in RTL context.
  SendKeyEvent('a');
  EXPECT_STR_EQ("a", textfield_->text());
  int x = GetCursorBounds().x();
  EXPECT_EQ(GetDisplayRect().right() - 1, x);
  int prev_x = x;

  SendKeyEvent('b');
  EXPECT_STR_EQ("ab", textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_EQ(prev_x, x);

  SendKeyEvent(0x05E1);
  EXPECT_EQ(WideToUTF16(L"ab\x05E1"), textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_GT(prev_x, x);
  prev_x = x;

  SendKeyEvent(0x05E2);
  EXPECT_EQ(WideToUTF16(L"ab\x05E1\x5E2"), textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_GT(prev_x, x);

  SendKeyEvent(ui::VKEY_A, false, true);
  SendKeyEvent('\n');

  // RTL-LTR string in RTL context.
  SendKeyEvent(0x05E1);
  EXPECT_EQ(WideToUTF16(L"\x05E1"), textfield_->text());
  x = GetCursorBounds().x();
  prev_x = x;

  SendKeyEvent(0x05E2);
  EXPECT_EQ(WideToUTF16(L"\x05E1\x05E2"), textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_GT(prev_x, x);
  prev_x = x;

  SendKeyEvent('a');
  EXPECT_EQ(WideToUTF16(L"\x05E1\x5E2"L"a"), textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_EQ(prev_x, x);
  prev_x = x;

  SendKeyEvent('b');
  EXPECT_EQ(WideToUTF16(L"\x05E1\x5E2"L"ab"), textfield_->text());
  x = GetCursorBounds().x();
  EXPECT_EQ(prev_x, x);

  // Reset locale.
  base::i18n::SetICUDefaultLocale(locale);
}

TEST_F(NativeTextfieldViewsTest, HitInsideTextAreaTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  textfield_->SetText(WideToUTF16(L"ab\x05E1\x5E2"));
  std::vector<gfx::Rect> cursor_bounds;

  // Save each cursor bound.
  gfx::SelectionModel sel(0, gfx::CURSOR_FORWARD);
  cursor_bounds.push_back(GetCursorBounds(sel));

  sel = gfx::SelectionModel(1, gfx::CURSOR_BACKWARD);
  gfx::Rect bound = GetCursorBounds(sel);
  sel = gfx::SelectionModel(1, gfx::CURSOR_FORWARD);
  EXPECT_EQ(bound.x(), GetCursorBounds(sel).x());
  cursor_bounds.push_back(bound);

  // Check that a cursor at the end of the Latin portion of the text is at the
  // same position as a cursor placed at the end of the RTL Hebrew portion.
  sel = gfx::SelectionModel(2, gfx::CURSOR_BACKWARD);
  bound = GetCursorBounds(sel);
  sel = gfx::SelectionModel(4, gfx::CURSOR_BACKWARD);
  EXPECT_EQ(bound.x(), GetCursorBounds(sel).x());
  cursor_bounds.push_back(bound);

  sel = gfx::SelectionModel(3, gfx::CURSOR_BACKWARD);
  bound = GetCursorBounds(sel);
  sel = gfx::SelectionModel(3, gfx::CURSOR_FORWARD);
  EXPECT_EQ(bound.x(), GetCursorBounds(sel).x());
  cursor_bounds.push_back(bound);

  sel = gfx::SelectionModel(2, gfx::CURSOR_FORWARD);
  bound = GetCursorBounds(sel);
  sel = gfx::SelectionModel(4, gfx::CURSOR_FORWARD);
  EXPECT_EQ(bound.x(), GetCursorBounds(sel).x());
  cursor_bounds.push_back(bound);

  // Expected cursor position when clicking left and right of each character.
  size_t cursor_pos_expected[] = {0, 1, 1, 2, 4, 3, 3, 2};

  int index = 0;
  for (int i = 0; i < static_cast<int>(cursor_bounds.size() - 1); ++i) {
    int half_width = (cursor_bounds[i + 1].x() - cursor_bounds[i].x()) / 2;
    MouseClick(cursor_bounds[i], half_width / 2);
    EXPECT_EQ(cursor_pos_expected[index++], textfield_->GetCursorPosition());

    // To avoid trigger double click. Not using sleep() since it takes longer
    // for the test to run if using sleep().
    NonClientMouseClick();

    MouseClick(cursor_bounds[i + 1], - (half_width / 2));
    EXPECT_EQ(cursor_pos_expected[index++], textfield_->GetCursorPosition());

    NonClientMouseClick();
  }
}

TEST_F(NativeTextfieldViewsTest, HitOutsideTextAreaTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);

  // LTR-RTL string in LTR context.
  textfield_->SetText(WideToUTF16(L"ab\x05E1\x5E2"));

  SendKeyEvent(ui::VKEY_HOME);
  gfx::Rect bound = GetCursorBounds();
  MouseClick(bound, -10);
  EXPECT_EQ(bound, GetCursorBounds());

  SendKeyEvent(ui::VKEY_END);
  bound = GetCursorBounds();
  MouseClick(bound, 10);
  EXPECT_EQ(bound, GetCursorBounds());

  NonClientMouseClick();

  // RTL-LTR string in LTR context.
  textfield_->SetText(WideToUTF16(L"\x05E1\x5E2"L"ab"));

  SendKeyEvent(ui::VKEY_HOME);
  bound = GetCursorBounds();
  MouseClick(bound, 10);
  EXPECT_EQ(bound, GetCursorBounds());

  SendKeyEvent(ui::VKEY_END);
  bound = GetCursorBounds();
  MouseClick(bound, -10);
  EXPECT_EQ(bound, GetCursorBounds());
}

TEST_F(NativeTextfieldViewsTest, HitOutsideTextAreaInRTLTest) {
  std::string locale = l10n_util::GetApplicationLocale("");
  base::i18n::SetICUDefaultLocale("he");

  InitTextfield(Textfield::STYLE_DEFAULT);

  // RTL-LTR string in RTL context.
  textfield_->SetText(WideToUTF16(L"\x05E1\x5E2"L"ab"));
  SendKeyEvent(ui::VKEY_HOME);
  gfx::Rect bound = GetCursorBounds();
  MouseClick(bound, 10);
  EXPECT_EQ(bound, GetCursorBounds());

  SendKeyEvent(ui::VKEY_END);
  bound = GetCursorBounds();
  MouseClick(bound, -10);
  EXPECT_EQ(bound, GetCursorBounds());

  NonClientMouseClick();

  // LTR-RTL string in RTL context.
  textfield_->SetText(WideToUTF16(L"ab\x05E1\x5E2"));
  SendKeyEvent(ui::VKEY_HOME);
  bound = GetCursorBounds();
  MouseClick(bound, -10);
  EXPECT_EQ(bound, GetCursorBounds());

  SendKeyEvent(ui::VKEY_END);
  bound = GetCursorBounds();
  MouseClick(bound, 10);
  EXPECT_EQ(bound, GetCursorBounds());

  // Reset locale.
  base::i18n::SetICUDefaultLocale(locale);
}

// This verifies that |bound| is contained by |display|. |bound|'s right edge
// must be less than |diaplay|'s right edge.
void OverflowCursorBoundTestVerifier(const gfx::Rect& display,
                                     const gfx::Rect& bound) {
  EXPECT_LE(display.x(), bound.x());
  EXPECT_GT(display.right(), bound.right());
  EXPECT_LE(display.y(), bound.y());
  EXPECT_GE(display.bottom(), bound.bottom());
}

TEST_F(NativeTextfieldViewsTest, OverflowTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);

  string16 str;
  for (int i = 0; i < 500; ++i)
    SendKeyEvent('a');
  SendKeyEvent(kHebrewLetterSamekh);
  gfx::Rect bound = GetCursorBounds();
  gfx::Rect display = GetDisplayRect();
  OverflowCursorBoundTestVerifier(display, bound);

  // Test mouse pointing.
  MouseClick(bound, -1);
  EXPECT_EQ(500U, textfield_->GetCursorPosition());

  // Clear text.
  SendKeyEvent(ui::VKEY_A, false, true);
  SendKeyEvent('\n');

  for (int i = 0; i < 500; ++i)
    SendKeyEvent(kHebrewLetterSamekh);
  SendKeyEvent('a');
  bound = GetCursorBounds();
  display = GetDisplayRect();
  OverflowCursorBoundTestVerifier(display, bound);

  MouseClick(bound, -1);
  EXPECT_EQ(501U, textfield_->GetCursorPosition());
}

TEST_F(NativeTextfieldViewsTest, OverflowInRTLTest) {
  std::string locale = l10n_util::GetApplicationLocale("");
  base::i18n::SetICUDefaultLocale("he");

  InitTextfield(Textfield::STYLE_DEFAULT);

  string16 str;
  for (int i = 0; i < 500; ++i)
    SendKeyEvent('a');
  SendKeyEvent(kHebrewLetterSamekh);
  gfx::Rect bound = GetCursorBounds();
  gfx::Rect display = GetDisplayRect();
  OverflowCursorBoundTestVerifier(display, bound);

  MouseClick(bound, 1);
  EXPECT_EQ(501U, textfield_->GetCursorPosition());

  // Clear text.
  SendKeyEvent(ui::VKEY_A, false, true);
  SendKeyEvent('\n');

  for (int i = 0; i < 500; ++i)
    SendKeyEvent(kHebrewLetterSamekh);
  SendKeyEvent('a');
  bound = GetCursorBounds();
  display = GetDisplayRect();
  OverflowCursorBoundTestVerifier(display, bound);


#if !defined(OS_WIN)
  // TODO(jennyz): NonClientMouseClick() does not work for os_win builds;
  // see crbug.com/104150. The mouse click in the next test will be confused
  // as a double click, which breaks the test. Disable the test until the
  // issue is fixed.
  NonClientMouseClick();

  MouseClick(bound, 1);
  EXPECT_EQ(500U, textfield_->GetCursorPosition());
#endif  // !defined(OS_WIN)

  // Reset locale.
  base::i18n::SetICUDefaultLocale(locale);
}

TEST_F(NativeTextfieldViewsTest, GetCompositionCharacterBoundsTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);

  string16 str;
  const uint32 char_count = 10UL;
  ui::CompositionText composition;
  composition.text = UTF8ToUTF16("0123456789");
  ui::TextInputClient* client = textfield_->GetTextInputClient();

  // Return false if there is no composition text.
  gfx::Rect rect;
  EXPECT_FALSE(client->GetCompositionCharacterBounds(0, &rect));

  // Get each character boundary by cursor.
  gfx::Rect char_rect_in_screen_coord[char_count];
  gfx::Rect prev_cursor = GetCursorBounds();
  for (uint32 i = 0; i < char_count; ++i) {
    composition.selection = ui::Range(0, i+1);
    client->SetCompositionText(composition);
    EXPECT_TRUE(client->HasCompositionText()) << " i=" << i;
    gfx::Rect cursor_bounds = GetCursorBounds();
    gfx::Point top_left(prev_cursor.x(), prev_cursor.y());
    gfx::Point bottom_right(cursor_bounds.x(), prev_cursor.bottom());
    views::View::ConvertPointToScreen(textfield_view_, &top_left);
    views::View::ConvertPointToScreen(textfield_view_, &bottom_right);
    char_rect_in_screen_coord[i].set_origin(top_left);
    char_rect_in_screen_coord[i].set_width(bottom_right.x() - top_left.x());
    char_rect_in_screen_coord[i].set_height(bottom_right.y() - top_left.y());
    prev_cursor = cursor_bounds;
  }

  for (uint32 i = 0; i < char_count; ++i) {
    gfx::Rect actual_rect;
    EXPECT_TRUE(client->GetCompositionCharacterBounds(i, &actual_rect))
        << " i=" << i;
    EXPECT_EQ(char_rect_in_screen_coord[i], actual_rect) << " i=" << i;
  }

  // Return false if the index is out of range.
  EXPECT_FALSE(client->GetCompositionCharacterBounds(char_count, &rect));
  EXPECT_FALSE(client->GetCompositionCharacterBounds(char_count + 1, &rect));
  EXPECT_FALSE(client->GetCompositionCharacterBounds(char_count + 100, &rect));
}

// Touch selection and draggin currently only works for chromeos.
#if defined(OS_CHROMEOS)
TEST_F(NativeTextfieldViewsTest, TouchSelectionAndDraggingTest) {
  InitTextfield(Textfield::STYLE_DEFAULT);
  textfield_->SetText(ASCIIToUTF16("hello world"));
  EXPECT_FALSE(GetTouchSelectionController());
  const int eventX = GetCursorPositionX(2);
  const int eventY = 0;
  CommandLine::ForCurrentProcess()->AppendSwitch(switches::kEnableTouchEditing);

  // Tapping on the textfield should turn on the TouchSelectionController.
  ui::GestureEvent tap(ui::ET_GESTURE_TAP, eventX, eventY, 0, base::TimeDelta(),
                       ui::GestureEventDetails(ui::ET_GESTURE_TAP, 1.0f, 0.0f),
                       0);
  textfield_view_->OnGestureEvent(&tap);
  EXPECT_TRUE(GetTouchSelectionController());

  // Un-focusing the textfield should reset the TouchSelectionController
  textfield_view_->GetFocusManager()->ClearFocus();
  EXPECT_FALSE(GetTouchSelectionController());

  // With touch editing enabled, long press should not show context menu.
  // Instead, select word and invoke TouchSelectionController.
  GestureEventForTest tap_down(ui::ET_GESTURE_TAP_DOWN, eventX, eventY, 0);
  textfield_view_->OnGestureEvent(&tap_down);
  GestureEventForTest long_press(ui::ET_GESTURE_LONG_PRESS, eventX, eventY, 0);
  textfield_view_->OnGestureEvent(&long_press);
  EXPECT_STR_EQ("hello", textfield_->GetSelectedText());
  EXPECT_TRUE(GetTouchSelectionController());

  // Long pressing again in the selecting region should not do anything since
  // touch drag drop is not yet enabled.
  textfield_view_->OnGestureEvent(&tap_down);
  textfield_view_->OnGestureEvent(&long_press);
  EXPECT_STR_EQ("hello", textfield_->GetSelectedText());
  EXPECT_TRUE(GetTouchSelectionController());
  EXPECT_TRUE(long_press.handled());

  // After enabling touch drag drop, long pressing in the selected region should
  // start a drag and remove TouchSelectionController.
  CommandLine::ForCurrentProcess()->AppendSwitch(
      switches::kEnableTouchDragDrop);
  textfield_view_->OnGestureEvent(&tap_down);
  // Create a new long press event since the previous one is not marked handled.
  GestureEventForTest long_press2(ui::ET_GESTURE_LONG_PRESS, eventX, eventY, 0);
  textfield_view_->OnGestureEvent(&long_press2);
  EXPECT_STR_EQ("hello", textfield_->GetSelectedText());
  EXPECT_FALSE(GetTouchSelectionController());
}
#endif

}  // namespace views
