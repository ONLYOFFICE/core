// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <X11/Xlib.h>
#undef Bool
#undef FocusIn
#undef FocusOut
#undef None

#include <cstring>

#include "base/i18n/char_iterator.h"
#include "base/memory/scoped_ptr.h"
#include "base/utf_string_conversions.h"
#include "chromeos/dbus/dbus_thread_manager.h"
#include "chromeos/dbus/ibus/ibus_text.h"
#include "chromeos/dbus/ibus/mock_ibus_client.h"
#include "chromeos/dbus/ibus/mock_ibus_input_context_client.h"
#include "chromeos/dbus/mock_dbus_thread_manager_without_gmock.h"
#include "chromeos/ime/mock_ibus_daemon_controller.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/ime/input_method_delegate.h"
#include "ui/base/ime/input_method_ibus.h"
#include "ui/base/ime/text_input_client.h"
#include "ui/gfx/rect.h"

namespace ui {
namespace {
const int kCreateInputContextMaxTrialCount = 10;
const uint32 kTestIBusKeyVal1 = 97;
const uint32 kTestIBusKeyVal2 = 30;
const uint32 kTestIBusKeyVal3 = 0;
const uint32 kTestIBusKeyCode1 = 98;
const uint32 kTestIBusKeyCode2 = 48;
const uint32 kTestIBusKeyCode3 = 1;
const uint32 kTestIBusState1 = 99;
const uint32 kTestIBusState2 = 46;
const uint32 kTestIBusState3 = 8;

uint32 GetOffsetInUTF16(const std::string& utf8_string, uint32 utf8_offset) {
  string16 utf16_string = UTF8ToUTF16(utf8_string);
  DCHECK_LT(utf8_offset, utf16_string.size());
  base::i18n::UTF16CharIterator char_iterator(&utf16_string);
  for (size_t i = 0; i < utf8_offset; ++i)
    char_iterator.Advance();
  return char_iterator.array_pos();
}

bool IsEqualXKeyEvent(const XEvent& e1, const XEvent& e2) {
  if ((e1.type == KeyPress && e2.type == KeyPress) ||
      (e1.type == KeyRelease && e2.type == KeyRelease)) {
    return !std::memcmp(&e1.xkey, &e2.xkey, sizeof(XKeyEvent));
  }
  return false;
}

enum KeyEventHandlerBehavior {
  KEYEVENT_CONSUME,
  KEYEVENT_NOT_CONSUME,
  KEYEVENT_ERROR,
};

}  // namespace


using chromeos::IBusInputContextClient;

class TestableInputMethodIBus : public InputMethodIBus {
 public:
  explicit TestableInputMethodIBus(internal::InputMethodDelegate* delegate)
      : InputMethodIBus(delegate),
        process_key_event_post_ime_call_count_(0) {
  }

  struct ProcessKeyEventPostIMEArgs {
    ProcessKeyEventPostIMEArgs() : ibus_keyval(0), handled(false) {
      std::memset(&event, 0, sizeof(XEvent));
    }
    XEvent event;
    uint32 ibus_keyval;
    bool handled;
  };

  struct IBusKeyEventFromNativeKeyEventResult {
    IBusKeyEventFromNativeKeyEventResult() : keyval(0), keycode(0), state(0) {}
    uint32 keyval;
    uint32 keycode;
    uint32 state;
  };

  // InputMethodIBus override.
  virtual void ProcessKeyEventPostIME(const base::NativeEvent& native_key_event,
                                      uint32 ibus_keyval,
                                      bool handled) OVERRIDE {
    process_key_event_post_ime_args_.event = *native_key_event;
    process_key_event_post_ime_args_.ibus_keyval = ibus_keyval;
    process_key_event_post_ime_args_.handled = handled;
    ++process_key_event_post_ime_call_count_;
  }

  // We can't call X11 related function without display in unit test, so
  // override with mock function.
  virtual void IBusKeyEventFromNativeKeyEvent(
      const base::NativeEvent& native_event,
      uint32* ibus_keyval,
      uint32* ibus_keycode,
      uint32* ibus_state) OVERRIDE {
    EXPECT_TRUE(native_event);
    EXPECT_TRUE(ibus_keyval);
    EXPECT_TRUE(ibus_keycode);
    EXPECT_TRUE(ibus_state);
    *ibus_keyval = ibus_key_event_from_native_key_event_result_.keyval;
    *ibus_keycode = ibus_key_event_from_native_key_event_result_.keycode;
    *ibus_state = ibus_key_event_from_native_key_event_result_.state;
  }

  void ResetCallCount() {
    process_key_event_post_ime_call_count_ = 0;
  }

  const ProcessKeyEventPostIMEArgs& process_key_event_post_ime_args() const {
    return process_key_event_post_ime_args_;
  }

  int process_key_event_post_ime_call_count() const {
    return process_key_event_post_ime_call_count_;
  }

  IBusKeyEventFromNativeKeyEventResult*
      mutable_ibus_key_event_from_native_key_event_result() {
    return &ibus_key_event_from_native_key_event_result_;
  }

  // Change access rights for testing.
  using InputMethodIBus::ExtractCompositionText;
  using InputMethodIBus::ResetContext;

 private:
  ProcessKeyEventPostIMEArgs process_key_event_post_ime_args_;
  int process_key_event_post_ime_call_count_;

  IBusKeyEventFromNativeKeyEventResult
      ibus_key_event_from_native_key_event_result_;
};

class CreateInputContextSuccessHandler {
 public:
  explicit CreateInputContextSuccessHandler(const dbus::ObjectPath& object_path)
      : object_path_(object_path) {
  }

  void Run(const std::string& client_name,
           const chromeos::IBusClient::CreateInputContextCallback& callback,
           const chromeos::IBusClient::ErrorCallback& error_callback) {
    EXPECT_EQ("chrome", client_name);
    callback.Run(object_path_);
  }

 private:
  dbus::ObjectPath object_path_;

  DISALLOW_COPY_AND_ASSIGN(CreateInputContextSuccessHandler);
};

class CreateInputContextFailHandler {
 public:
  CreateInputContextFailHandler() {}
  void Run(const std::string& client_name,
           const chromeos::IBusClient::CreateInputContextCallback& callback,
           const chromeos::IBusClient::ErrorCallback& error_callback) {
    error_callback.Run();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CreateInputContextFailHandler);
};

class CreateInputContextNoResponseHandler {
 public:
  CreateInputContextNoResponseHandler() {}
  void Run(const std::string& client_name,
           const chromeos::IBusClient::CreateInputContextCallback& callback,
           const chromeos::IBusClient::ErrorCallback& error_callback) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CreateInputContextNoResponseHandler);
};

class CreateInputContextDelayHandler {
 public:
  explicit CreateInputContextDelayHandler(const dbus::ObjectPath& object_path)
    : object_path_(object_path) {
  }

  void Run(const std::string& client_name,
           const chromeos::IBusClient::CreateInputContextCallback& callback,
           const chromeos::IBusClient::ErrorCallback& error_callback) {
    callback_ = callback;
    error_callback_ = error_callback;
  }

  void RunCallback(bool success) {
    if (success)
      callback_.Run(object_path_);
    else
      error_callback_.Run();
  }

 private:
  dbus::ObjectPath object_path_;
  chromeos::IBusClient::CreateInputContextCallback callback_;
  chromeos::IBusClient::ErrorCallback error_callback_;

  DISALLOW_COPY_AND_ASSIGN(CreateInputContextDelayHandler);
};

class SynchronousKeyEventHandler {
 public:
  SynchronousKeyEventHandler(uint32 expected_keyval,
                             uint32 expected_keycode,
                             uint32 expected_state,
                             KeyEventHandlerBehavior behavior)
      : expected_keyval_(expected_keyval),
        expected_keycode_(expected_keycode),
        expected_state_(expected_state),
        behavior_(behavior) {}

  virtual ~SynchronousKeyEventHandler() {}

  void Run(uint32 keyval,
           uint32 keycode,
           uint32 state,
           const IBusInputContextClient::ProcessKeyEventCallback& callback,
           const IBusInputContextClient::ErrorCallback& error_callback) {
    EXPECT_EQ(expected_keyval_, keyval);
    EXPECT_EQ(expected_keycode_, keycode);
    EXPECT_EQ(expected_state_, state);
    if (behavior_ == KEYEVENT_ERROR)
      error_callback.Run();
    else
      callback.Run(behavior_ == KEYEVENT_CONSUME);
  }

 private:
  const uint32 expected_keyval_;
  const uint32 expected_keycode_;
  const uint32 expected_state_;
  const KeyEventHandlerBehavior behavior_;

  DISALLOW_COPY_AND_ASSIGN(SynchronousKeyEventHandler);
};

class AsynchronousKeyEventHandler {
 public:
  AsynchronousKeyEventHandler(uint32 expected_keyval,
                              uint32 expected_keycode,
                              uint32 expected_state)
      : expected_keyval_(expected_keyval),
        expected_keycode_(expected_keycode),
        expected_state_(expected_state) {}

  virtual ~AsynchronousKeyEventHandler() {}

  void Run(uint32 keyval,
           uint32 keycode,
           uint32 state,
           const IBusInputContextClient::ProcessKeyEventCallback& callback,
           const IBusInputContextClient::ErrorCallback& error_callback) {
    EXPECT_EQ(expected_keyval_, keyval);
    EXPECT_EQ(expected_keycode_, keycode);
    EXPECT_EQ(expected_state_, state);
    callback_ = callback;
    error_callback_ = error_callback;
  }

  void RunCallback(KeyEventHandlerBehavior behavior) {
    if (behavior == KEYEVENT_ERROR)
      error_callback_.Run();
    else
      callback_.Run(behavior == KEYEVENT_CONSUME);
  }

 private:
  const uint32 expected_keyval_;
  const uint32 expected_keycode_;
  const uint32 expected_state_;
  IBusInputContextClient::ProcessKeyEventCallback callback_;
  IBusInputContextClient::ErrorCallback error_callback_;

  DISALLOW_COPY_AND_ASSIGN(AsynchronousKeyEventHandler);
};

class SetSurroundingTextVerifier {
 public:
  SetSurroundingTextVerifier(const std::string& expected_surrounding_text,
                             uint32 expected_cursor_position,
                             uint32 expected_anchor_position)
      : expected_surrounding_text_(expected_surrounding_text),
        expected_cursor_position_(expected_cursor_position),
        expected_anchor_position_(expected_anchor_position) {}

  void Verify(const std::string& text,
              uint32 cursor_pos,
              uint32 anchor_pos) {
    EXPECT_EQ(expected_surrounding_text_, text);
    EXPECT_EQ(expected_cursor_position_, cursor_pos);
    EXPECT_EQ(expected_anchor_position_, anchor_pos);
  }

 private:
  const std::string expected_surrounding_text_;
  const uint32 expected_cursor_position_;
  const uint32 expected_anchor_position_;

  DISALLOW_COPY_AND_ASSIGN(SetSurroundingTextVerifier);
};

class InputMethodIBusTest : public internal::InputMethodDelegate,
                            public testing::Test,
                            public TextInputClient {
 public:
  InputMethodIBusTest() {
    ResetFlags();
  }

  virtual ~InputMethodIBusTest() {
  }

  // testing::Test overrides:
  virtual void SetUp() OVERRIDE {
    // |thread_manager_| will be released by DBusThreadManager::Shutdown
    // function in TearDown function.
    // Current MockIBusInputContext is strongly depend on gmock, but gmock is
    // banned in ui/*. So just use stab implementation for testing.
    mock_dbus_thread_manager_ =
        new chromeos::MockDBusThreadManagerWithoutGMock();
    chromeos::DBusThreadManager::InitializeForTesting(
        mock_dbus_thread_manager_);
    mock_ibus_daemon_controller_ = new chromeos::MockIBusDaemonController();
    chromeos::IBusDaemonController::InitializeForTesting(
        mock_ibus_daemon_controller_);

    mock_ibus_client_ = mock_dbus_thread_manager_->mock_ibus_client();
    mock_ibus_input_context_client_ =
        mock_dbus_thread_manager_->mock_ibus_input_context_client();

    ime_.reset(new TestableInputMethodIBus(this));
    ime_->SetFocusedTextInputClient(this);
  }

  virtual void TearDown() OVERRIDE {
    if (ime_.get())
      ime_->SetFocusedTextInputClient(NULL);
    ime_.reset();
    chromeos::DBusThreadManager::Shutdown();
    chromeos::IBusDaemonController::Shutdown();
  }

  // ui::internal::InputMethodDelegate overrides:
  virtual bool DispatchKeyEventPostIME(
      const base::NativeEvent& native_key_event) OVERRIDE {
    dispatched_native_event_ = native_key_event;
    return false;
  }
  virtual bool DispatchFabricatedKeyEventPostIME(ui::EventType type,
                                                 ui::KeyboardCode key_code,
                                                 int flags) OVERRIDE {
    dispatched_fabricated_event_type_ = type;
    dispatched_fabricated_event_key_code_ = key_code;
    dispatched_fabricated_event_flags_ = flags;
    return false;
  }

  // ui::TextInputClient overrides:
  virtual void SetCompositionText(
      const CompositionText& composition) OVERRIDE {
    composition_text_ = composition;
  }
  virtual void ConfirmCompositionText() OVERRIDE {
    confirmed_text_ = composition_text_;
    composition_text_.Clear();
  }
  virtual void ClearCompositionText() OVERRIDE {
    composition_text_.Clear();
  }
  virtual void InsertText(const string16& text) OVERRIDE {
    inserted_text_ = text;
  }
  virtual void InsertChar(char16 ch, int flags) OVERRIDE {
    inserted_char_ = ch;
    inserted_char_flags_ = flags;
  }
  virtual TextInputType GetTextInputType() const OVERRIDE {
    return input_type_;
  }
  virtual bool CanComposeInline() const OVERRIDE {
    return can_compose_inline_;
  }
  virtual gfx::Rect GetCaretBounds() OVERRIDE {
    return caret_bounds_;
  }
  virtual bool GetCompositionCharacterBounds(uint32 index,
                                             gfx::Rect* rect) OVERRIDE {
    return false;
  }
  virtual bool HasCompositionText() OVERRIDE {
    CompositionText empty;
    return composition_text_ != empty;
  }
  virtual bool GetTextRange(Range* range) OVERRIDE {
    *range = text_range_;
    return true;
  }
  virtual bool GetCompositionTextRange(Range* range) OVERRIDE { return false; }
  virtual bool GetSelectionRange(Range* range) OVERRIDE {
    *range = selection_range_;
    return true;
  }

  virtual bool SetSelectionRange(const Range& range) OVERRIDE { return false; }
  virtual bool DeleteRange(const Range& range) OVERRIDE { return false; }
  virtual bool GetTextFromRange(const Range& range, string16* text) OVERRIDE {
    *text = surrounding_text_.substr(range.GetMin(), range.length());
    return true;
  }
  virtual void OnInputMethodChanged() OVERRIDE {
    ++on_input_method_changed_call_count_;
  }
  virtual bool ChangeTextDirectionAndLayoutAlignment(
      base::i18n::TextDirection direction) OVERRIDE { return false; }
  virtual void ExtendSelectionAndDelete(size_t before,
                                        size_t after) OVERRIDE { }
  virtual void EnsureCaretInRect(const gfx::Rect& rect) OVERRIDE { }

  bool HasNativeEvent() const {
    base::NativeEvent empty;
    std::memset(&empty, 0, sizeof(empty));
    return !!std::memcmp(&dispatched_native_event_,
                         &empty,
                         sizeof(dispatched_native_event_));
  }

  void ResetFlags() {
    std::memset(&dispatched_native_event_, 0, sizeof(dispatched_native_event_));
    DCHECK(!HasNativeEvent());
    dispatched_fabricated_event_type_ = ET_UNKNOWN;
    dispatched_fabricated_event_key_code_ = VKEY_UNKNOWN;
    dispatched_fabricated_event_flags_ = 0;

    composition_text_.Clear();
    confirmed_text_.Clear();
    inserted_text_.clear();
    inserted_char_ = 0;
    inserted_char_flags_ = 0;
    on_input_method_changed_call_count_ = 0;

    input_type_ = TEXT_INPUT_TYPE_NONE;
    can_compose_inline_ = true;
    caret_bounds_ = gfx::Rect();
  }

  void SetCreateContextSuccessHandler() {
    CreateInputContextSuccessHandler create_input_context_handler(
        dbus::ObjectPath("InputContext_1"));
    mock_ibus_client_->set_create_input_context_handler(base::Bind(
        &CreateInputContextSuccessHandler::Run,
        base::Unretained(&create_input_context_handler)));
  }

  scoped_ptr<TestableInputMethodIBus> ime_;

  // Variables for remembering the parameters that are passed to
  // ui::internal::InputMethodDelegate functions.
  base::NativeEvent dispatched_native_event_;
  ui::EventType dispatched_fabricated_event_type_;
  ui::KeyboardCode dispatched_fabricated_event_key_code_;
  int dispatched_fabricated_event_flags_;

  // Variables for remembering the parameters that are passed to
  // ui::TextInputClient functions.
  CompositionText composition_text_;
  CompositionText confirmed_text_;
  string16 inserted_text_;
  char16 inserted_char_;
  unsigned int on_input_method_changed_call_count_;
  int inserted_char_flags_;

  // Variables that will be returned from the ui::TextInputClient functions.
  TextInputType input_type_;
  bool can_compose_inline_;
  gfx::Rect caret_bounds_;
  ui::Range text_range_;
  ui::Range selection_range_;
  string16 surrounding_text_;

  // Variables for mock dbus connections.
  chromeos::MockDBusThreadManagerWithoutGMock* mock_dbus_thread_manager_;
  chromeos::MockIBusClient* mock_ibus_client_;
  chromeos::MockIBusInputContextClient* mock_ibus_input_context_client_;
  chromeos::MockIBusDaemonController* mock_ibus_daemon_controller_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodIBusTest);
};

// Tests public APIs in ui::InputMethod first.

TEST_F(InputMethodIBusTest, GetInputLocale) {
  // ui::InputMethodIBus does not support the API.
  ime_->Init(true);
  EXPECT_EQ("", ime_->GetInputLocale());
}

TEST_F(InputMethodIBusTest, GetInputTextDirection) {
  // ui::InputMethodIBus does not support the API.
  ime_->Init(true);
  EXPECT_EQ(base::i18n::UNKNOWN_DIRECTION, ime_->GetInputTextDirection());
}

TEST_F(InputMethodIBusTest, IsActive) {
  ime_->Init(true);
  // ui::InputMethodIBus always returns true.
  EXPECT_TRUE(ime_->IsActive());
}

TEST_F(InputMethodIBusTest, GetInputTextType) {
  ime_->Init(true);
  EXPECT_EQ(TEXT_INPUT_TYPE_NONE, ime_->GetTextInputType());
  input_type_ = TEXT_INPUT_TYPE_PASSWORD;
  ime_->OnTextInputTypeChanged(this);
  EXPECT_EQ(TEXT_INPUT_TYPE_PASSWORD, ime_->GetTextInputType());
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);
  EXPECT_EQ(TEXT_INPUT_TYPE_TEXT, ime_->GetTextInputType());
}

TEST_F(InputMethodIBusTest, CanComposeInline) {
  ime_->Init(true);
  EXPECT_TRUE(ime_->CanComposeInline());
  can_compose_inline_ = false;
  ime_->OnTextInputTypeChanged(this);
  EXPECT_FALSE(ime_->CanComposeInline());
}

TEST_F(InputMethodIBusTest, GetTextInputClient) {
  ime_->Init(true);
  EXPECT_EQ(this, ime_->GetTextInputClient());
  ime_->SetFocusedTextInputClient(NULL);
  EXPECT_EQ(NULL, ime_->GetTextInputClient());
}

TEST_F(InputMethodIBusTest, GetInputTextType_WithoutFocusedClient) {
  ime_->Init(true);
  EXPECT_EQ(TEXT_INPUT_TYPE_NONE, ime_->GetTextInputType());
  ime_->SetFocusedTextInputClient(NULL);
  input_type_ = TEXT_INPUT_TYPE_PASSWORD;
  ime_->OnTextInputTypeChanged(this);
  // The OnTextInputTypeChanged() call above should be ignored since |this| is
  // not the current focused client.
  EXPECT_EQ(TEXT_INPUT_TYPE_NONE, ime_->GetTextInputType());

  ime_->SetFocusedTextInputClient(this);
  ime_->OnTextInputTypeChanged(this);
  EXPECT_EQ(TEXT_INPUT_TYPE_PASSWORD, ime_->GetTextInputType());
}

TEST_F(InputMethodIBusTest, GetInputTextType_WithoutFocusedWindow) {
  ime_->Init(true);
  EXPECT_EQ(TEXT_INPUT_TYPE_NONE, ime_->GetTextInputType());
  ime_->OnBlur();
  input_type_ = TEXT_INPUT_TYPE_PASSWORD;
  ime_->OnTextInputTypeChanged(this);
  // The OnTextInputTypeChanged() call above should be ignored since the top-
  // level window which the ime_ is attached to is not currently focused.
  EXPECT_EQ(TEXT_INPUT_TYPE_NONE, ime_->GetTextInputType());

  ime_->OnFocus();
  ime_->OnTextInputTypeChanged(this);
  EXPECT_EQ(TEXT_INPUT_TYPE_PASSWORD, ime_->GetTextInputType());
}

TEST_F(InputMethodIBusTest, GetInputTextType_WithoutFocusedWindow2) {
  ime_->Init(false);  // the top-level is initially unfocused.
  EXPECT_EQ(TEXT_INPUT_TYPE_NONE, ime_->GetTextInputType());
  input_type_ = TEXT_INPUT_TYPE_PASSWORD;
  ime_->OnTextInputTypeChanged(this);
  EXPECT_EQ(TEXT_INPUT_TYPE_NONE, ime_->GetTextInputType());

  ime_->OnFocus();
  ime_->OnTextInputTypeChanged(this);
  EXPECT_EQ(TEXT_INPUT_TYPE_PASSWORD, ime_->GetTextInputType());
}

// Then, tests internal behavior of ui::InputMethodIBus, especially if the input
// method implementation calls ui::internal::IBusClient APIs as expected.

// Start ibus-daemon first, then create ui::InputMethodIBus. Check if a new
// input context is created.
TEST_F(InputMethodIBusTest, InitiallyConnected) {
  SetCreateContextSuccessHandler();
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  ime_->Init(true);
  // An input context should be created immediately since is_connected_ is true.
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(1, mock_ibus_input_context_client_->set_capabilities_call_count());
  // However, since the current text input type is 'NONE' (the default), FocusIn
  // shouldn't be called.
  EXPECT_EQ(0, mock_ibus_input_context_client_->focus_in_call_count());
  EXPECT_TRUE(mock_ibus_input_context_client_->IsObjectProxyReady());
}

// Create ui::InputMethodIBus, then start ibus-daemon.
TEST_F(InputMethodIBusTest, InitiallyDisconnected) {
  SetCreateContextSuccessHandler();
  ime_->Init(true);
  // A context shouldn't be created since the daemon is not running.
  EXPECT_EQ(0, mock_ibus_client_->create_input_context_call_count());
  // Start the daemon.
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  mock_ibus_daemon_controller_->EmulateConnect();
  // A context should be created upon the signal delivery.
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(1, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->focus_in_call_count());
  EXPECT_TRUE(mock_ibus_input_context_client_->IsObjectProxyReady());
}

// Confirm that ui::InputMethodIBus does not crash on "disconnected" signal
// delivery.
TEST_F(InputMethodIBusTest, Disconnect) {
  SetCreateContextSuccessHandler();
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  ime_->Init(true);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  // Currently we can't shutdown IBusBus connection except in
  // DBusThreadManager's shutting down. So set ibus_bus_ as NULL to emulate
  // dynamical shutting down.
  mock_dbus_thread_manager_->set_ibus_bus(NULL);
  mock_ibus_daemon_controller_->EmulateDisconnect();
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());
}

// Confirm that ui::InputMethodIBus re-creates an input context when ibus-daemon
// restarts.
TEST_F(InputMethodIBusTest, DisconnectThenReconnect) {
  SetCreateContextSuccessHandler();
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  ime_->Init(true);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(1, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_EQ(0,
            mock_ibus_input_context_client_->reset_object_proxy_call_caount());
  mock_dbus_thread_manager_->set_ibus_bus(NULL);
  mock_ibus_daemon_controller_->EmulateDisconnect();
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  mock_ibus_daemon_controller_->EmulateConnect();
  // Check if the old context is deleted.
  EXPECT_EQ(1,
            mock_ibus_input_context_client_->reset_object_proxy_call_caount());
  // Check if a new context is created.
  EXPECT_EQ(2, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(2, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_TRUE(mock_ibus_input_context_client_->IsObjectProxyReady());
}

// Confirm that ui::InputMethodIBus does not crash even if NULL context is
// passed.
// TODO(yusukes): Currently, ui::InputMethodIBus does not try to create ic once
// it fails (unless ibus sends the "connected" signal to Chrome again). It might
// be better to add some retry logic. Will revisit later.
TEST_F(InputMethodIBusTest, CreateContextFail) {
  CreateInputContextFailHandler create_input_context_handler;
  mock_ibus_client_->set_create_input_context_handler(base::Bind(
      &CreateInputContextFailHandler::Run,
      base::Unretained(&create_input_context_handler)));

  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  ime_->Init(true);
  // InputMethodIBus tries several times if the CreateInputContext method call
  // is failed.
  EXPECT_EQ(kCreateInputContextMaxTrialCount,
            mock_ibus_client_->create_input_context_call_count());
  // |set_capabilities_call_count()| should be zero since a context is not
  // created yet.
  EXPECT_EQ(0, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());
}

// Confirm that ui::InputMethodIBus does not crash even if ibus-daemon does not
// respond.
TEST_F(InputMethodIBusTest, CreateContextNoResp) {
  CreateInputContextNoResponseHandler create_input_context_handler;
  mock_ibus_client_->set_create_input_context_handler(base::Bind(
      &CreateInputContextNoResponseHandler::Run,
      base::Unretained(&create_input_context_handler)));

  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  ime_->Init(true);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());
}

// Confirm that ui::InputMethodIBus does not crash even if ibus-daemon responds
// after ui::InputMethodIBus is deleted.
TEST_F(InputMethodIBusTest, CreateContextFailDelayed) {
  CreateInputContextDelayHandler create_input_context_handler(
      dbus::ObjectPath("Sample object path"));
  mock_ibus_client_->set_create_input_context_handler(base::Bind(
      &CreateInputContextDelayHandler::Run,
      base::Unretained(&create_input_context_handler)));

  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  ime_->Init(true);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());

  ime_->SetFocusedTextInputClient(NULL);
  ime_.reset();
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());

  create_input_context_handler.RunCallback(false);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());
}

// Confirm that ui::InputMethodIBus does not crash even if ibus-daemon responds
// after ui::InputMethodIBus is deleted.
TEST_F(InputMethodIBusTest, CreateContextSuccessDelayed) {
  CreateInputContextDelayHandler create_input_context_handler(
      dbus::ObjectPath("Sample object path"));
  mock_ibus_client_->set_create_input_context_handler(base::Bind(
      &CreateInputContextDelayHandler::Run,
      base::Unretained(&create_input_context_handler)));

  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  ime_->Init(true);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());

  ime_->SetFocusedTextInputClient(NULL);
  ime_.reset();
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());

  create_input_context_handler.RunCallback(true);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());
}

// Confirm that ui::InputMethodIBus does not crash even if ibus-daemon responds
// after disconnected from ibus-daemon.
TEST_F(InputMethodIBusTest, CreateContextSuccessDelayedAfterDisconnection) {
  CreateInputContextDelayHandler create_input_context_handler(
      dbus::ObjectPath("Sample object path"));
  mock_ibus_client_->set_create_input_context_handler(base::Bind(
      &CreateInputContextDelayHandler::Run,
      base::Unretained(&create_input_context_handler)));

  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  ime_->Init(true);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());

  mock_ibus_daemon_controller_->EmulateDisconnect();
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());

  create_input_context_handler.RunCallback(true);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());
}

// Confirm that ui::InputMethodIBus does not crash even if ibus-daemon responds
// after disconnected from ibus-daemon.
TEST_F(InputMethodIBusTest, CreateContextFailDelayedAfterDisconnection) {
  CreateInputContextDelayHandler create_input_context_handler(
      dbus::ObjectPath("Sample object path"));
  mock_ibus_client_->set_create_input_context_handler(base::Bind(
      &CreateInputContextDelayHandler::Run,
      base::Unretained(&create_input_context_handler)));

  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  ime_->Init(true);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());

  mock_ibus_daemon_controller_->EmulateDisconnect();
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());

  create_input_context_handler.RunCallback(false);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->set_capabilities_call_count());
  EXPECT_FALSE(mock_ibus_input_context_client_->IsObjectProxyReady());
}

// Confirm that IBusClient::FocusIn is called on "connected" if input_type_ is
// TEXT.
TEST_F(InputMethodIBusTest, FocusIn_Text) {
  SetCreateContextSuccessHandler();
  ime_->Init(true);
  // A context shouldn't be created since the daemon is not running.
  EXPECT_EQ(0, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(0U, on_input_method_changed_call_count_);
  // Click a text input form.
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);
  // Start the daemon.
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  mock_ibus_daemon_controller_->EmulateConnect();
  // A context should be created upon the signal delivery.
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  // Since a form has focus, IBusClient::FocusIn() should be called.
  EXPECT_EQ(1, mock_ibus_input_context_client_->focus_in_call_count());
  EXPECT_EQ(1,
            mock_ibus_input_context_client_->set_cursor_location_call_count());
  // ui::TextInputClient::OnInputMethodChanged() should be called when
  // ui::InputMethodIBus connects/disconnects to/from ibus-daemon and the
  // current text input type is not NONE.
  EXPECT_EQ(1U, on_input_method_changed_call_count_);
}

// Confirm that IBusClient::FocusIn is NOT called on "connected" if input_type_
// is PASSWORD.
TEST_F(InputMethodIBusTest, FocusIn_Password) {
  SetCreateContextSuccessHandler();
  ime_->Init(true);
  EXPECT_EQ(0, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(0U, on_input_method_changed_call_count_);
  input_type_ = TEXT_INPUT_TYPE_PASSWORD;
  ime_->OnTextInputTypeChanged(this);
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  mock_ibus_daemon_controller_->EmulateConnect();
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  // Since a form has focus, IBusClient::FocusIn() should NOT be called.
  EXPECT_EQ(0, mock_ibus_input_context_client_->focus_in_call_count());
  EXPECT_EQ(1U, on_input_method_changed_call_count_);
}

// Confirm that IBusClient::FocusOut is called as expected.
TEST_F(InputMethodIBusTest, FocusOut_None) {
  SetCreateContextSuccessHandler();
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  ime_->Init(true);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(1, mock_ibus_input_context_client_->focus_in_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->focus_out_call_count());
  input_type_ = TEXT_INPUT_TYPE_NONE;
  ime_->OnTextInputTypeChanged(this);
  EXPECT_EQ(1, mock_ibus_input_context_client_->focus_in_call_count());
  EXPECT_EQ(1, mock_ibus_input_context_client_->focus_out_call_count());
}

// Confirm that IBusClient::FocusOut is called as expected.
TEST_F(InputMethodIBusTest, FocusOut_Password) {
  SetCreateContextSuccessHandler();
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  ime_->Init(true);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(1, mock_ibus_input_context_client_->focus_in_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->focus_out_call_count());
  input_type_ = TEXT_INPUT_TYPE_PASSWORD;
  ime_->OnTextInputTypeChanged(this);
  EXPECT_EQ(1, mock_ibus_input_context_client_->focus_in_call_count());
  EXPECT_EQ(1, mock_ibus_input_context_client_->focus_out_call_count());
}

// Confirm that IBusClient::FocusOut is NOT called.
TEST_F(InputMethodIBusTest, FocusOut_Url) {
  SetCreateContextSuccessHandler();
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  ime_->Init(true);
  EXPECT_EQ(1, mock_ibus_client_->create_input_context_call_count());
  EXPECT_EQ(1, mock_ibus_input_context_client_->focus_in_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->focus_out_call_count());
  input_type_ = TEXT_INPUT_TYPE_URL;
  ime_->OnTextInputTypeChanged(this);
  EXPECT_EQ(1, mock_ibus_input_context_client_->focus_in_call_count());
  EXPECT_EQ(0, mock_ibus_input_context_client_->focus_out_call_count());
}

// Test if the new |caret_bounds_| is correctly sent to ibus-daemon.
TEST_F(InputMethodIBusTest, OnCaretBoundsChanged) {
  SetCreateContextSuccessHandler();
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->Init(true);
  EXPECT_EQ(0,
            mock_ibus_input_context_client_->set_cursor_location_call_count());
  caret_bounds_ = gfx::Rect(1, 2, 3, 4);
  ime_->OnCaretBoundsChanged(this);
  EXPECT_EQ(1,
            mock_ibus_input_context_client_->set_cursor_location_call_count());
  caret_bounds_ = gfx::Rect(0, 2, 3, 4);
  ime_->OnCaretBoundsChanged(this);
  EXPECT_EQ(2,
            mock_ibus_input_context_client_->set_cursor_location_call_count());
  caret_bounds_ = gfx::Rect(0, 2, 3, 4);  // unchanged
  ime_->OnCaretBoundsChanged(this);
  // Current InputMethodIBus implementation performs the IPC regardless of the
  // bounds are changed or not.
  EXPECT_EQ(3,
            mock_ibus_input_context_client_->set_cursor_location_call_count());
}

TEST_F(InputMethodIBusTest, ExtractCompositionTextTest_NoAttribute) {
  const char kSampleText[] = "Sample Text";
  const uint32 kCursorPos = 2UL;

  const string16 utf16_string = UTF8ToUTF16(kSampleText);
  chromeos::IBusText ibus_text;
  ibus_text.set_text(kSampleText);

  CompositionText composition_text;
  ime_->ExtractCompositionText(ibus_text, kCursorPos, &composition_text);
  EXPECT_EQ(UTF8ToUTF16(kSampleText), composition_text.text);
  // If there is no selection, |selection| represents cursor position.
  EXPECT_EQ(kCursorPos, composition_text.selection.start());
  EXPECT_EQ(kCursorPos, composition_text.selection.end());
  // If there is no underline, |underlines| contains one underline and it is
  // whole text underline.
  ASSERT_EQ(1UL, composition_text.underlines.size());
  EXPECT_EQ(0UL, composition_text.underlines[0].start_offset);
  EXPECT_EQ(utf16_string.size(), composition_text.underlines[0].end_offset);
  EXPECT_FALSE(composition_text.underlines[0].thick);
}

TEST_F(InputMethodIBusTest, ExtractCompositionTextTest_SingleUnderline) {
  const char kSampleText[] = "\xE3\x81\x82\xE3\x81\x84\xE3\x81\x86"
                             "\xE3\x81\x88\xE3\x81\x8A";
  const uint32 kCursorPos = 2UL;

  // Set up ibus text with one underline attribute.
  chromeos::IBusText ibus_text;
  ibus_text.set_text(kSampleText);
  chromeos::IBusText::UnderlineAttribute underline;
  underline.type = chromeos::IBusText::IBUS_TEXT_UNDERLINE_SINGLE;
  underline.start_index = 1UL;
  underline.end_index = 4UL;
  ibus_text.mutable_underline_attributes()->push_back(underline);

  CompositionText composition_text;
  ime_->ExtractCompositionText(ibus_text, kCursorPos, &composition_text);
  EXPECT_EQ(UTF8ToUTF16(kSampleText), composition_text.text);
  // If there is no selection, |selection| represents cursor position.
  EXPECT_EQ(kCursorPos, composition_text.selection.start());
  EXPECT_EQ(kCursorPos, composition_text.selection.end());
  ASSERT_EQ(1UL, composition_text.underlines.size());
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, underline.start_index),
            composition_text.underlines[0].start_offset);
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, underline.end_index),
            composition_text.underlines[0].end_offset);
  // Single underline represents as black thin line.
  EXPECT_EQ(SK_ColorBLACK, composition_text.underlines[0].color);
  EXPECT_FALSE(composition_text.underlines[0].thick);
}

TEST_F(InputMethodIBusTest, ExtractCompositionTextTest_DoubleUnderline) {
  const char kSampleText[] = "\xE3\x81\x82\xE3\x81\x84\xE3\x81\x86"
                             "\xE3\x81\x88\xE3\x81\x8A";
  const uint32 kCursorPos = 2UL;

  // Set up ibus text with one underline attribute.
  chromeos::IBusText ibus_text;
  ibus_text.set_text(kSampleText);
  chromeos::IBusText::UnderlineAttribute underline;
  underline.type = chromeos::IBusText::IBUS_TEXT_UNDERLINE_DOUBLE;
  underline.start_index = 1UL;
  underline.end_index = 4UL;
  ibus_text.mutable_underline_attributes()->push_back(underline);

  CompositionText composition_text;
  ime_->ExtractCompositionText(ibus_text, kCursorPos, &composition_text);
  EXPECT_EQ(UTF8ToUTF16(kSampleText), composition_text.text);
  // If there is no selection, |selection| represents cursor position.
  EXPECT_EQ(kCursorPos, composition_text.selection.start());
  EXPECT_EQ(kCursorPos, composition_text.selection.end());
  ASSERT_EQ(1UL, composition_text.underlines.size());
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, underline.start_index),
            composition_text.underlines[0].start_offset);
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, underline.end_index),
            composition_text.underlines[0].end_offset);
  // Double underline represents as black thick line.
  EXPECT_EQ(SK_ColorBLACK, composition_text.underlines[0].color);
  EXPECT_TRUE(composition_text.underlines[0].thick);
}

TEST_F(InputMethodIBusTest, ExtractCompositionTextTest_ErrorUnderline) {
  const char kSampleText[] = "\xE3\x81\x82\xE3\x81\x84\xE3\x81\x86"
                             "\xE3\x81\x88\xE3\x81\x8A";
  const uint32 kCursorPos = 2UL;

  // Set up ibus text with one underline attribute.
  chromeos::IBusText ibus_text;
  ibus_text.set_text(kSampleText);
  chromeos::IBusText::UnderlineAttribute underline;
  underline.type = chromeos::IBusText::IBUS_TEXT_UNDERLINE_ERROR;
  underline.start_index = 1UL;
  underline.end_index = 4UL;
  ibus_text.mutable_underline_attributes()->push_back(underline);

  CompositionText composition_text;
  ime_->ExtractCompositionText(ibus_text, kCursorPos, &composition_text);
  EXPECT_EQ(UTF8ToUTF16(kSampleText), composition_text.text);
  EXPECT_EQ(kCursorPos, composition_text.selection.start());
  EXPECT_EQ(kCursorPos, composition_text.selection.end());
  ASSERT_EQ(1UL, composition_text.underlines.size());
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, underline.start_index),
            composition_text.underlines[0].start_offset);
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, underline.end_index),
            composition_text.underlines[0].end_offset);
  // Error underline represents as red thin line.
  EXPECT_EQ(SK_ColorRED, composition_text.underlines[0].color);
  EXPECT_FALSE(composition_text.underlines[0].thick);
}

TEST_F(InputMethodIBusTest, ExtractCompositionTextTest_Selection) {
  const char kSampleText[] = "\xE3\x81\x82\xE3\x81\x84\xE3\x81\x86"
                             "\xE3\x81\x88\xE3\x81\x8A";
  const uint32 kCursorPos = 2UL;

  // Set up ibus text with one underline attribute.
  chromeos::IBusText ibus_text;
  ibus_text.set_text(kSampleText);
  chromeos::IBusText::SelectionAttribute selection;
  selection.start_index = 1UL;
  selection.end_index = 4UL;
  ibus_text.mutable_selection_attributes()->push_back(selection);

  CompositionText composition_text;
  ime_->ExtractCompositionText(ibus_text, kCursorPos, &composition_text);
  EXPECT_EQ(UTF8ToUTF16(kSampleText), composition_text.text);
  EXPECT_EQ(kCursorPos, composition_text.selection.start());
  EXPECT_EQ(kCursorPos, composition_text.selection.end());
  ASSERT_EQ(1UL, composition_text.underlines.size());
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, selection.start_index),
            composition_text.underlines[0].start_offset);
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, selection.end_index),
            composition_text.underlines[0].end_offset);
  EXPECT_EQ(SK_ColorBLACK, composition_text.underlines[0].color);
  EXPECT_TRUE(composition_text.underlines[0].thick);
}

TEST_F(InputMethodIBusTest,
       ExtractCompositionTextTest_SelectionStartWithCursor) {
  const char kSampleText[] = "\xE3\x81\x82\xE3\x81\x84\xE3\x81\x86"
                             "\xE3\x81\x88\xE3\x81\x8A";
  const uint32 kCursorPos = 1UL;

  // Set up ibus text with one underline attribute.
  chromeos::IBusText ibus_text;
  ibus_text.set_text(kSampleText);
  chromeos::IBusText::SelectionAttribute selection;
  selection.start_index = kCursorPos;
  selection.end_index = 4UL;
  ibus_text.mutable_selection_attributes()->push_back(selection);

  CompositionText composition_text;
  ime_->ExtractCompositionText(ibus_text, kCursorPos, &composition_text);
  EXPECT_EQ(UTF8ToUTF16(kSampleText), composition_text.text);
  // If the cursor position is same as selection bounds, selection start
  // position become opposit side of selection from cursor.
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, selection.end_index),
            composition_text.selection.start());
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, kCursorPos),
            composition_text.selection.end());
  ASSERT_EQ(1UL, composition_text.underlines.size());
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, selection.start_index),
            composition_text.underlines[0].start_offset);
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, selection.end_index),
            composition_text.underlines[0].end_offset);
  EXPECT_EQ(SK_ColorBLACK, composition_text.underlines[0].color);
  EXPECT_TRUE(composition_text.underlines[0].thick);
}

TEST_F(InputMethodIBusTest, ExtractCompositionTextTest_SelectionEndWithCursor) {
  const char kSampleText[] = "\xE3\x81\x82\xE3\x81\x84\xE3\x81\x86"
                             "\xE3\x81\x88\xE3\x81\x8A";
  const uint32 kCursorPos = 4UL;

  // Set up ibus text with one underline attribute.
  chromeos::IBusText ibus_text;
  ibus_text.set_text(kSampleText);
  chromeos::IBusText::SelectionAttribute selection;
  selection.start_index = 1UL;
  selection.end_index = kCursorPos;
  ibus_text.mutable_selection_attributes()->push_back(selection);

  CompositionText composition_text;
  ime_->ExtractCompositionText(ibus_text, kCursorPos, &composition_text);
  EXPECT_EQ(UTF8ToUTF16(kSampleText), composition_text.text);
  // If the cursor position is same as selection bounds, selection start
  // position become opposit side of selection from cursor.
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, selection.start_index),
            composition_text.selection.start());
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, kCursorPos),
            composition_text.selection.end());
  ASSERT_EQ(1UL, composition_text.underlines.size());
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, selection.start_index),
            composition_text.underlines[0].start_offset);
  EXPECT_EQ(GetOffsetInUTF16(kSampleText, selection.end_index),
            composition_text.underlines[0].end_offset);
  EXPECT_EQ(SK_ColorBLACK, composition_text.underlines[0].color);
  EXPECT_TRUE(composition_text.underlines[0].thick);
}

TEST_F(InputMethodIBusTest, SurroundingText_NoSelectionTest) {
  SetCreateContextSuccessHandler();
  ime_->Init(true);
  // Click a text input form.
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);
  // Start the daemon.
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  mock_ibus_daemon_controller_->EmulateConnect();

  // Set the TextInputClient behaviors.
  surrounding_text_ = UTF8ToUTF16("abcdef");
  text_range_ = ui::Range(0, 6);
  selection_range_ = ui::Range(3, 3);

  // Set the verifier for SetSurroundingText mock call.
  SetSurroundingTextVerifier verifier(UTF16ToUTF8(surrounding_text_), 3, 3);


  mock_ibus_input_context_client_->set_set_surrounding_text_handler(
      base::Bind(&SetSurroundingTextVerifier::Verify,
                 base::Unretained(&verifier)));
  ime_->OnCaretBoundsChanged(this);

  // Check the call count.
  EXPECT_EQ(1,
            mock_ibus_input_context_client_->set_surrounding_text_call_count());
}

TEST_F(InputMethodIBusTest, SurroundingText_SelectionTest) {
  SetCreateContextSuccessHandler();
  ime_->Init(true);
  // Click a text input form.
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);
  // Start the daemon.
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  mock_ibus_daemon_controller_->EmulateConnect();

  // Set the TextInputClient behaviors.
  surrounding_text_ = UTF8ToUTF16("abcdef");
  text_range_ = ui::Range(0, 6);
  selection_range_ = ui::Range(2, 5);

  // Set the verifier for SetSurroundingText mock call.
  SetSurroundingTextVerifier verifier(UTF16ToUTF8(surrounding_text_), 2, 5);

  mock_ibus_input_context_client_->set_set_surrounding_text_handler(
      base::Bind(&SetSurroundingTextVerifier::Verify,
                 base::Unretained(&verifier)));
  ime_->OnCaretBoundsChanged(this);

  // Check the call count.
  EXPECT_EQ(1,
            mock_ibus_input_context_client_->set_surrounding_text_call_count());
}

TEST_F(InputMethodIBusTest, SurroundingText_PartialText) {
  SetCreateContextSuccessHandler();
  ime_->Init(true);
  // Click a text input form.
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);
  // Start the daemon.
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  mock_ibus_daemon_controller_->EmulateConnect();

  // Set the TextInputClient behaviors.
  surrounding_text_ = UTF8ToUTF16("abcdefghij");
  text_range_ = ui::Range(5, 10);
  selection_range_ = ui::Range(7, 9);

  // Set the verifier for SetSurroundingText mock call.
  // Here (2, 4) is selection range in expected surrounding text coordinates.
  SetSurroundingTextVerifier verifier("fghij", 2, 4);

  mock_ibus_input_context_client_->set_set_surrounding_text_handler(
      base::Bind(&SetSurroundingTextVerifier::Verify,
                 base::Unretained(&verifier)));
  ime_->OnCaretBoundsChanged(this);

  // Check the call count.
  EXPECT_EQ(1,
            mock_ibus_input_context_client_->set_surrounding_text_call_count());
}

TEST_F(InputMethodIBusTest, SurroundingText_BecomeEmptyText) {
  SetCreateContextSuccessHandler();
  ime_->Init(true);
  // Click a text input form.
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);
  // Start the daemon.
  chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
  mock_ibus_daemon_controller_->EmulateConnect();

  // Set the TextInputClient behaviors.
  // If the surrounding text becomes empty, text_range become (0, 0) and
  // selection range become invalid.
  surrounding_text_ = UTF8ToUTF16("");
  text_range_ = ui::Range(0, 0);
  selection_range_ = ui::Range::InvalidRange();

  // Set the verifier for SetSurroundingText mock call.
  SetSurroundingTextVerifier verifier("", 0, 0);

  mock_ibus_input_context_client_->set_set_surrounding_text_handler(
      base::Bind(&SetSurroundingTextVerifier::Verify,
                 base::Unretained(&verifier)));
  ime_->OnCaretBoundsChanged(this);

  // Check the call count.
  EXPECT_EQ(0,
            mock_ibus_input_context_client_->set_surrounding_text_call_count());

  // Should not be called twice with same condition.
  ime_->OnCaretBoundsChanged(this);
  EXPECT_EQ(0,
            mock_ibus_input_context_client_->set_surrounding_text_call_count());
}

class InputMethodIBusKeyEventTest : public InputMethodIBusTest {
 public:
  InputMethodIBusKeyEventTest() {}
  virtual ~InputMethodIBusKeyEventTest() {}

  virtual void SetUp() OVERRIDE {
    InputMethodIBusTest::SetUp();
    SetCreateContextSuccessHandler();
    chromeos::DBusThreadManager::Get()->InitIBusBus("dummy address",
                                                  base::Bind(&base::DoNothing));
    ime_->Init(true);
    mock_ibus_daemon_controller_->EmulateConnect();
    mock_ibus_input_context_client_->SetIsXKBLayout(false);
  }

  DISALLOW_COPY_AND_ASSIGN(InputMethodIBusKeyEventTest);
};

TEST_F(InputMethodIBusKeyEventTest, KeyEventConsumeTest) {
  XEvent event = {};
  event.xkey.type = KeyPress;

  // Set up IBusKeyEventFromNativeKeyEvent result.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState1;

  // Set up ProcessKeyEvent handler.
  SynchronousKeyEventHandler success_consume_handler(kTestIBusKeyVal1,
                                                     kTestIBusKeyCode1,
                                                     kTestIBusState1,
                                                     KEYEVENT_CONSUME);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&SynchronousKeyEventHandler::Run,
                 base::Unretained(&success_consume_handler)));

  // Do key event.
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);
  ime_->DispatchKeyEvent(&event);

  // Check result
  EXPECT_EQ(1,
            mock_ibus_input_context_client_->process_key_event_call_count());
  EXPECT_EQ(1, ime_->process_key_event_post_ime_call_count());
  EXPECT_TRUE(IsEqualXKeyEvent(event,
                               ime_->process_key_event_post_ime_args().event));
  EXPECT_EQ(kTestIBusKeyVal1,
            ime_->process_key_event_post_ime_args().ibus_keyval);
  EXPECT_TRUE(ime_->process_key_event_post_ime_args().handled);
}

TEST_F(InputMethodIBusKeyEventTest, KeyEventNotConsumeTest) {
  XEvent event = {};
  event.xkey.type = KeyPress;

  // Set up IBusKeyEventFromNativeKeyEvent result.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState1;

  // Set up ProcessKeyEvent handler.
  SynchronousKeyEventHandler success_nonconsume_handler(kTestIBusKeyVal1,
                                                        kTestIBusKeyCode1,
                                                        kTestIBusState1,
                                                        KEYEVENT_NOT_CONSUME);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&SynchronousKeyEventHandler::Run,
                 base::Unretained(&success_nonconsume_handler)));

  // Do key event.
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);
  ime_->DispatchKeyEvent(&event);

  EXPECT_EQ(1,
            mock_ibus_input_context_client_->process_key_event_call_count());
  EXPECT_EQ(1, ime_->process_key_event_post_ime_call_count());
  EXPECT_TRUE(IsEqualXKeyEvent(event,
                               ime_->process_key_event_post_ime_args().event));
  EXPECT_EQ(kTestIBusKeyVal1,
            ime_->process_key_event_post_ime_args().ibus_keyval);
  EXPECT_FALSE(ime_->process_key_event_post_ime_args().handled);
}

TEST_F(InputMethodIBusKeyEventTest, KeyEventFailTest) {
  XEvent event = {};
  event.xkey.type = KeyPress;

  // Set up IBusKeyEventFromNativeKeyEvent result.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState1;

  // Set up ProcessKeyEvent handler.
  SynchronousKeyEventHandler fail_handler(kTestIBusKeyVal1,
                                          kTestIBusKeyCode1,
                                          kTestIBusState1,
                                          KEYEVENT_ERROR);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&SynchronousKeyEventHandler::Run,
                 base::Unretained(&fail_handler)));

  // Do key event.
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);
  ime_->DispatchKeyEvent(&event);

  // Check result
  EXPECT_EQ(1,
            mock_ibus_input_context_client_->process_key_event_call_count());
  EXPECT_EQ(1, ime_->process_key_event_post_ime_call_count());
  EXPECT_EQ(kTestIBusKeyVal1,
            ime_->process_key_event_post_ime_args().ibus_keyval);
  // If some error is happend, key should not be consumed.
  EXPECT_FALSE(ime_->process_key_event_post_ime_args().handled);
}

TEST_F(InputMethodIBusKeyEventTest, KeyEventDelayResponseSuccessTest) {
  XEvent event = {};
  event.xkey.type = KeyPress;

  // Set up IBusKeyEventFromNativeKeyEvent result.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState1;

  // Set up ProcessKeyEvent handler.
  AsynchronousKeyEventHandler success_consume_handler(kTestIBusKeyVal1,
                                                      kTestIBusKeyCode1,
                                                      kTestIBusState1);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&AsynchronousKeyEventHandler::Run,
                 base::Unretained(&success_consume_handler)));

  // Do key event.
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);
  ime_->DispatchKeyEvent(&event);

  // Check before state.
  EXPECT_EQ(1,
            mock_ibus_input_context_client_->process_key_event_call_count());
  EXPECT_EQ(0, ime_->process_key_event_post_ime_call_count());

  // Do callback.
  success_consume_handler.RunCallback(KEYEVENT_CONSUME);

  // Check the results
  EXPECT_EQ(1, ime_->process_key_event_post_ime_call_count());
  EXPECT_TRUE(IsEqualXKeyEvent(event,
                               ime_->process_key_event_post_ime_args().event));
  EXPECT_EQ(kTestIBusKeyVal1,
            ime_->process_key_event_post_ime_args().ibus_keyval);
  EXPECT_TRUE(ime_->process_key_event_post_ime_args().handled);
}

TEST_F(InputMethodIBusKeyEventTest, KeyEventDelayResponseFailTest) {
  XEvent event = {};
  event.xkey.type = KeyPress;

  // Set up IBusKeyEventFromNativeKeyEvent result.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState1;

  // Set up ProcessKeyEvent handler.
  AsynchronousKeyEventHandler fail_handler(kTestIBusKeyVal1, kTestIBusKeyCode1,
                                           kTestIBusState1);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&AsynchronousKeyEventHandler::Run,
                 base::Unretained(&fail_handler)));

  // Do key event.
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);
  ime_->DispatchKeyEvent(&event);

  // Check before state.
  EXPECT_EQ(1,
            mock_ibus_input_context_client_->process_key_event_call_count());
  EXPECT_EQ(0, ime_->process_key_event_post_ime_call_count());

  // Do callback.
  fail_handler.RunCallback(KEYEVENT_ERROR);

  // Check the results
  EXPECT_EQ(1, ime_->process_key_event_post_ime_call_count());
  EXPECT_TRUE(IsEqualXKeyEvent(event,
                               ime_->process_key_event_post_ime_args().event));
  EXPECT_EQ(kTestIBusKeyVal1,
            ime_->process_key_event_post_ime_args().ibus_keyval);
  EXPECT_FALSE(ime_->process_key_event_post_ime_args().handled);
}

TEST_F(InputMethodIBusKeyEventTest, MultiKeyEventDelayResponseSuccessTest) {
  // Preparation
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);

  XEvent event = {};
  event.xkey.type = KeyPress;

  // Set up IBusKeyEventFromNativeKeyEvent result for first key event.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState1;

  // Set up ProcessKeyEvent handler.
  AsynchronousKeyEventHandler handler1(kTestIBusKeyVal1, kTestIBusKeyCode1,
                                       kTestIBusState1);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&AsynchronousKeyEventHandler::Run,
                 base::Unretained(&handler1)));

  // Do key event.
  ime_->DispatchKeyEvent(&event);

  // Set up IBusKeyEventFromNativeKeyEvent result for second key event.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal2;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode2;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState2;

  // Set up yet another ProcessKeyEvent handler.
  AsynchronousKeyEventHandler handler2(kTestIBusKeyVal2, kTestIBusKeyCode2,
                                       kTestIBusState2);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&AsynchronousKeyEventHandler::Run,
                 base::Unretained(&handler2)));

  // Do key event again.
  ime_->DispatchKeyEvent(&event);

  // Check before state.
  EXPECT_EQ(2,
            mock_ibus_input_context_client_->process_key_event_call_count());
  EXPECT_EQ(0, ime_->process_key_event_post_ime_call_count());

  // Do callback for first key event.
  handler1.RunCallback(KEYEVENT_CONSUME);

  // Check the results for first key event.
  EXPECT_EQ(1, ime_->process_key_event_post_ime_call_count());
  EXPECT_TRUE(IsEqualXKeyEvent(event,
                               ime_->process_key_event_post_ime_args().event));
  EXPECT_EQ(kTestIBusKeyVal1,
            ime_->process_key_event_post_ime_args().ibus_keyval);
  EXPECT_TRUE(ime_->process_key_event_post_ime_args().handled);

  // Do callback for second key event.
  handler2.RunCallback(KEYEVENT_NOT_CONSUME);

  // Check the results for second key event.
  EXPECT_EQ(2, ime_->process_key_event_post_ime_call_count());
  EXPECT_TRUE(IsEqualXKeyEvent(event,
                               ime_->process_key_event_post_ime_args().event));
  EXPECT_EQ(kTestIBusKeyVal2,
            ime_->process_key_event_post_ime_args().ibus_keyval);
  EXPECT_FALSE(ime_->process_key_event_post_ime_args().handled);
}

TEST_F(InputMethodIBusKeyEventTest, MultiKeyEventDelayResponseFailTest) {
  // Preparation
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);

  XEvent event = {};
  event.xkey.type = KeyPress;

  // Set up IBusKeyEventFromNativeKeyEvent result for first key event.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState1;

  // Set up ProcessKeyEvent handler.
  AsynchronousKeyEventHandler handler1(kTestIBusKeyVal1, kTestIBusKeyCode1,
                                       kTestIBusState1);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&AsynchronousKeyEventHandler::Run,
                 base::Unretained(&handler1)));

  // Do key event.
  ime_->DispatchKeyEvent(&event);

  // Set up IBusKeyEventFromNativeKeyEvent result for second key event.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal2;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode2;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState2;

  // Set up yet another ProcessKeyEvent handler.
  AsynchronousKeyEventHandler handler2(kTestIBusKeyVal2, kTestIBusKeyCode2,
                                       kTestIBusState2);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&AsynchronousKeyEventHandler::Run,
                 base::Unretained(&handler2)));

  // Do key event again.
  ime_->DispatchKeyEvent(&event);

  // Check before state.
  EXPECT_EQ(2,
            mock_ibus_input_context_client_->process_key_event_call_count());
  EXPECT_EQ(0, ime_->process_key_event_post_ime_call_count());

  // Do callback for first key event.
  handler1.RunCallback(KEYEVENT_ERROR);

  // Check the results for first key event.
  EXPECT_EQ(1, ime_->process_key_event_post_ime_call_count());
  EXPECT_TRUE(IsEqualXKeyEvent(event,
                               ime_->process_key_event_post_ime_args().event));
  EXPECT_EQ(kTestIBusKeyVal1,
            ime_->process_key_event_post_ime_args().ibus_keyval);
  EXPECT_FALSE(ime_->process_key_event_post_ime_args().handled);

  // Do callback for second key event.
  handler2.RunCallback(KEYEVENT_ERROR);

  // Check the results for second key event.
  EXPECT_EQ(2, ime_->process_key_event_post_ime_call_count());
  EXPECT_TRUE(IsEqualXKeyEvent(event,
                               ime_->process_key_event_post_ime_args().event));
  EXPECT_EQ(kTestIBusKeyVal2,
            ime_->process_key_event_post_ime_args().ibus_keyval);
  EXPECT_FALSE(ime_->process_key_event_post_ime_args().handled);
}

TEST_F(InputMethodIBusKeyEventTest,
       MultiKeyEventDelayResponseSuccessFailMixTest) {
  // Preparation
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);

  XEvent event = {};
  event.xkey.type = KeyPress;

  // Set up IBusKeyEventFromNativeKeyEvent result for first key event.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState1;

  // Set up ProcessKeyEvent handler.
  AsynchronousKeyEventHandler handler1(kTestIBusKeyVal1, kTestIBusKeyCode1,
                                       kTestIBusState1);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&AsynchronousKeyEventHandler::Run,
                 base::Unretained(&handler1)));

  // Do key event.
  ime_->DispatchKeyEvent(&event);

  // Set up IBusKeyEventFromNativeKeyEvent result for second key event.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal2;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode2;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState2;

  // Set up yet another ProcessKeyEvent handler.
  AsynchronousKeyEventHandler handler2(kTestIBusKeyVal2, kTestIBusKeyCode2,
                                       kTestIBusState2);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&AsynchronousKeyEventHandler::Run,
                 base::Unretained(&handler2)));

  // Do key event again.
  ime_->DispatchKeyEvent(&event);

  // Set up IBusKeyEventFromNativeKeyEvent result for second key event.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal3;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode3;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState3;

  // Set up yet another ProcessKeyEvent handler.
  AsynchronousKeyEventHandler handler3(kTestIBusKeyVal3, kTestIBusKeyCode3,
                                       kTestIBusState3);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&AsynchronousKeyEventHandler::Run,
                 base::Unretained(&handler3)));

  // Do key event again.
  ime_->DispatchKeyEvent(&event);

  // Check before state.
  EXPECT_EQ(3,
            mock_ibus_input_context_client_->process_key_event_call_count());
  EXPECT_EQ(0, ime_->process_key_event_post_ime_call_count());

  // Do callback for first key event.
  handler1.RunCallback(KEYEVENT_CONSUME);

  // Check the results for first key event.
  EXPECT_EQ(1, ime_->process_key_event_post_ime_call_count());
  EXPECT_TRUE(IsEqualXKeyEvent(event,
                               ime_->process_key_event_post_ime_args().event));
  EXPECT_EQ(kTestIBusKeyVal1,
            ime_->process_key_event_post_ime_args().ibus_keyval);
  EXPECT_TRUE(ime_->process_key_event_post_ime_args().handled);

  // Do callback for second key event.
  handler2.RunCallback(KEYEVENT_ERROR);

  // Check the results for second key event.
  EXPECT_EQ(2, ime_->process_key_event_post_ime_call_count());
  EXPECT_TRUE(IsEqualXKeyEvent(event,
                               ime_->process_key_event_post_ime_args().event));
  EXPECT_EQ(kTestIBusKeyVal2,
            ime_->process_key_event_post_ime_args().ibus_keyval);
  EXPECT_FALSE(ime_->process_key_event_post_ime_args().handled);

  // Do callback for first key event.
  handler3.RunCallback(KEYEVENT_CONSUME);

  // Check the results for first key event.
  EXPECT_EQ(3, ime_->process_key_event_post_ime_call_count());
  EXPECT_TRUE(IsEqualXKeyEvent(event,
                               ime_->process_key_event_post_ime_args().event));
  EXPECT_EQ(kTestIBusKeyVal3,
            ime_->process_key_event_post_ime_args().ibus_keyval);
  EXPECT_TRUE(ime_->process_key_event_post_ime_args().handled);
}

TEST_F(InputMethodIBusKeyEventTest, KeyEventDelayResponseResetTest) {
  XEvent event = {};
  event.xkey.type = KeyPress;

  // Set up IBusKeyEventFromNativeKeyEvent result.
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keyval
      = kTestIBusKeyVal1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->keycode
      = kTestIBusKeyCode1;
  ime_->mutable_ibus_key_event_from_native_key_event_result()->state
      = kTestIBusState1;

  // Set up ProcessKeyEvent handler.
  AsynchronousKeyEventHandler success_consume_handler(kTestIBusKeyVal1,
                                                      kTestIBusKeyCode1,
                                                      kTestIBusState1);
  mock_ibus_input_context_client_->set_process_key_event_handler(
      base::Bind(&AsynchronousKeyEventHandler::Run,
                 base::Unretained(&success_consume_handler)));

  // Do key event.
  input_type_ = TEXT_INPUT_TYPE_TEXT;
  ime_->OnTextInputTypeChanged(this);
  ime_->DispatchKeyEvent(&event);

  // Check before state.
  EXPECT_EQ(1,
            mock_ibus_input_context_client_->process_key_event_call_count());
  EXPECT_EQ(0, ime_->process_key_event_post_ime_call_count());

  ime_->ResetContext();

  // Do callback.
  success_consume_handler.RunCallback(KEYEVENT_CONSUME);

  EXPECT_EQ(0, ime_->process_key_event_post_ime_call_count());
}

// TODO(nona): Introduce ProcessKeyEventPostIME tests(crbug.com/156593).

}  // namespace ui
