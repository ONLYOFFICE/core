// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_INPUT_METHOD_IBUS_H_
#define UI_BASE_IME_INPUT_METHOD_IBUS_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chromeos/dbus/ibus/ibus_input_context_client.h"
#include "chromeos/ime/ibus_daemon_controller.h"
#include "ui/base/ime/character_composer.h"
#include "ui/base/ime/composition_text.h"
#include "ui/base/ime/input_method_base.h"

namespace dbus {
class ObjectPath;
}
namespace chromeos {
namespace ibus {
class IBusText;
}  // namespace ibus
}  // namespace chromeos

namespace ui {

// A ui::InputMethod implementation based on IBus.
class UI_EXPORT InputMethodIBus
    : public InputMethodBase,
      public chromeos::IBusInputContextHandlerInterface,
      public chromeos::IBusDaemonController::Observer {
 public:
  explicit InputMethodIBus(internal::InputMethodDelegate* delegate);
  virtual ~InputMethodIBus();

  // Overridden from InputMethod:
  virtual void OnFocus() OVERRIDE;
  virtual void OnBlur() OVERRIDE;
  virtual void Init(bool focused) OVERRIDE;
  virtual bool DispatchKeyEvent(
      const base::NativeEvent& native_key_event) OVERRIDE;
  virtual bool DispatchFabricatedKeyEvent(const ui::KeyEvent& event) OVERRIDE;
  virtual void OnTextInputTypeChanged(const TextInputClient* client) OVERRIDE;
  virtual void OnCaretBoundsChanged(const TextInputClient* client) OVERRIDE;
  virtual void CancelComposition(const TextInputClient* client) OVERRIDE;
  virtual std::string GetInputLocale() OVERRIDE;
  virtual base::i18n::TextDirection GetInputTextDirection() OVERRIDE;
  virtual bool IsActive() OVERRIDE;

 protected:
  // chromeos::IBusDaemonController::Observer overrides.
  virtual void OnConnected() OVERRIDE;
  virtual void OnDisconnected() OVERRIDE;

  // Converts |text| into CompositionText.
  void ExtractCompositionText(const chromeos::IBusText& text,
                              uint32 cursor_position,
                              CompositionText* out_composition) const;

  // Process a key returned from the input method.
  virtual void ProcessKeyEventPostIME(const base::NativeEvent& native_key_event,
                                      uint32 ibus_keycode,
                                      bool handled);

  // Converts |native_event| to ibus representation.
  virtual void IBusKeyEventFromNativeKeyEvent(
      const base::NativeEvent& native_event,
      uint32* ibus_keyval,
      uint32* ibus_keycode,
      uint32* ibus_state);

  // Resets context and abandon all pending results and key events.
  void ResetContext();

 private:
  enum InputContextState {
    // The input context is not working.
    INPUT_CONTEXT_STOP,
    // The input context is waiting for CreateInputContext reply from
    // ibus-daemon.
    INPUT_CONTEXT_WAIT_CREATE_INPUT_CONTEXT_RESPONSE,
    // The input context is working and ready to communicate with ibus-daemon.
    INPUT_CONTEXT_RUNNING,
  };
  class PendingKeyEvent;

  // Overridden from InputMethodBase:
  virtual void OnWillChangeFocusedClient(TextInputClient* focused_before,
                                         TextInputClient* focused) OVERRIDE;
  virtual void OnDidChangeFocusedClient(TextInputClient* focused_before,
                                        TextInputClient* focused) OVERRIDE;

  // Creates context asynchronously.
  void CreateContext();

  // Sets necessary signal handlers.
  void SetUpSignalHandlers();

  // Destroys context.
  void DestroyContext();

  // Asks the client to confirm current composition text.
  void ConfirmCompositionText();

  // Checks the availability of focused text input client and update focus
  // state.
  void UpdateContextFocusState();

  // Processes a key event that was already filtered by the input method.
  // A VKEY_PROCESSKEY may be dispatched to the focused View.
  void ProcessFilteredKeyPressEvent(const base::NativeEvent& native_key_event);

  // Processes a key event that was not filtered by the input method.
  void ProcessUnfilteredKeyPressEvent(const base::NativeEvent& native_key_event,
                                      uint32 ibus_keycode);
  void ProcessUnfilteredFabricatedKeyPressEvent(EventType type,
                                                KeyboardCode key_code,
                                                int flags,
                                                uint32 ibus_keyval);

  // Processes an unfiltered key press event with character composer.
  // This method returns true if the key press is filtered by the composer.
  bool ProcessUnfilteredKeyPressEventWithCharacterComposer(uint32 ibus_keyval,
                                                           uint32 state);

  // Sends input method result caused by the given key event to the focused text
  // input client.
  void ProcessInputMethodResult(const base::NativeEvent& native_key_event,
                                bool filtered);

  // Checks if the pending input method result needs inserting into the focused
  // text input client as a single character.
  bool NeedInsertChar() const;

  // Checks if there is pending input method result.
  bool HasInputMethodResult() const;

  // Fabricates a key event with VKEY_PROCESSKEY key code and dispatches it to
  // the focused View.
  void SendFakeProcessKeyEvent(bool pressed) const;

  // Abandons all pending key events. It usually happends when we lose keyboard
  // focus, the text input type is changed or we are destroyed.
  void AbandonAllPendingKeyEvents();

  // Releases context focus and confirms the composition text. Then destroy
  // object proxy.
  void ResetInputContext();

  // Returns true if the connection to ibus-daemon is established.
  bool IsConnected();

  // Returns true if the input context is ready to use.
  bool IsContextReady();

  // chromeos::IBusInputContextHandlerInterface overrides:
  virtual void CommitText(const chromeos::IBusText& text) OVERRIDE;
  virtual void ForwardKeyEvent(uint32 keyval,
                               uint32 keycode,
                               uint32 status) OVERRIDE;
  virtual void ShowPreeditText() OVERRIDE;
  virtual void HidePreeditText() OVERRIDE;
  virtual void UpdatePreeditText(const chromeos::IBusText& text,
                                 uint32 cursor_pos,
                                 bool visible) OVERRIDE;

  void CreateInputContextDone(const dbus::ObjectPath& object_path);
  void CreateInputContextFail();
  void ProcessKeyEventDone(uint32 id, XEvent* xevent, uint32 keyval,
                           bool is_handled);

  // All pending key events. Note: we do not own these object, we just save
  // pointers to these object so that we can abandon them when necessary.
  // They will be deleted in ProcessKeyEventDone().
  std::set<uint32> pending_key_events_;

  // Represents input context's state.
  InputContextState input_context_state_;

  // The count of CreateInputContext message failure.
  int create_input_context_fail_count_;

  // Pending composition text generated by the current pending key event.
  // It'll be sent to the focused text input client as soon as we receive the
  // processing result of the pending key event.
  CompositionText composition_;

  // Pending result text generated by the current pending key event.
  // It'll be sent to the focused text input client as soon as we receive the
  // processing result of the pending key event.
  string16 result_text_;

  string16 previous_surrounding_text_;
  ui::Range previous_selection_range_;

  // Indicates if input context is focused or not.
  bool context_focused_;

  // Indicates if there is an ongoing composition text.
  bool composing_text_;

  // Indicates if the composition text is changed or deleted.
  bool composition_changed_;

  // If it's true then all input method result received before the next key
  // event will be discarded.
  bool suppress_next_result_;

  // The latest id of key event.
  uint32 current_keyevent_id_;

  // An object to compose a character from a sequence of key presses
  // including dead key etc.
  CharacterComposer character_composer_;

  // Used for making callbacks.
  base::WeakPtrFactory<InputMethodIBus> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(InputMethodIBus);
};

}  // namespace ui

#endif  // UI_BASE_IME_INPUT_METHOD_IBUS_H_
