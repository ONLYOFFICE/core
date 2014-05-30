// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/keyboard/keyboard_util.h"

#include <string>

#include "base/command_line.h"
#include "base/logging.h"
#include "base/string_util.h"
#include "base/strings/string_number_conversions.h"
#include "base/values.h"
#include "ui/base/events/event.h"
#include "ui/base/events/event_constants.h"
#include "ui/base/events/key_identifier_conversion.h"
#include "ui/keyboard/keyboard_switches.h"

namespace {

// KeyEvent dictionary keys
const char kType[] = "type";
const char kKeyIdentifier[] = "keyIdentifier";
const char kAlt[] = "altKey";
const char kCtrl[] = "ctrlKey";
const char kMeta[] = "metaKey";
const char kShift[] = "shiftKey";
const char kKeyDown[] = "keydown";
const char kKeyUp[] = "keyup";

// Errors.
const char kInvalidArgumentsListError[] =
    "Argument list does not contain a dictionary.";
const char kInvalidKeyEventMissingKeyIdentifierError[] =
    "KeyEvent object is missing the keyIdentifier field";
const char kInvalidKeyEventMissingTypeError[] =
    "KeyEvent object is missing the type field";
const char kUnknownKeyEventTypeError[] =
    "Unknown event type in KeyEvent.";
const char kUnknownOrUnsupportedKeyIdentiferError[] =
    "Unknown or unsupported key identifier.";
const char kUnsupportedModifierError[] =
    "Unsupported modifier (meta).";

ui::EventType GetTypeFromString(const std::string& type) {
  if (type == kKeyDown) {
    return ui::ET_KEY_PRESSED;
  } else if (type == kKeyUp) {
    return ui::ET_KEY_RELEASED;
  }
  return ui::ET_UNKNOWN;
}

// Converts a hex string "U+NNNN" to uint16. Returns 0 on error.
uint16 UnicodeIdentifierStringToInt(const std::string& key_identifier) {
  int character = 0;
  if ((key_identifier.length() == 6) &&
      (key_identifier.substr(0, 2) == "U+") &&
      (key_identifier.substr(2).find_first_not_of("0123456789abcdefABCDEF") ==
       std::string::npos)) {
    const bool result =
        base::HexStringToInt(key_identifier.substr(2), &character);
    DCHECK(result) << key_identifier;
  }
  return character;
}

}  // namespace

namespace keyboard {

bool IsKeyboardEnabled() {
  return CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableVirtualKeyboard);
}

ui::KeyEvent* KeyEventFromArgs(const base::ListValue* args,
                               std::string* error) {
  const DictionaryValue* key_event;
  if (!args->GetDictionary(0, &key_event)) {
    *error = kInvalidArgumentsListError;
    return NULL;
  }

  std::string type_name;
  if (!key_event->GetString(kType, &type_name)) {
    *error = kInvalidKeyEventMissingTypeError;
    return NULL;
  }

  ui::EventType type = GetTypeFromString(type_name);
  if (type == ui::ET_UNKNOWN) {
    *error = kUnknownKeyEventTypeError;
    return NULL;
  }

  std::string identifier;
  if (!key_event->GetString(kKeyIdentifier, &identifier)) {
    *error = kInvalidKeyEventMissingKeyIdentifierError;
    return NULL;
  }
  TrimWhitespaceASCII(identifier, TRIM_ALL, &identifier);

  const ui::KeyEvent& prototype_event =
      ui::KeyEventFromKeyIdentifier(identifier);
  uint16 character = 0;
  if (prototype_event.key_code() == ui::VKEY_UNKNOWN) {
    character = UnicodeIdentifierStringToInt(identifier);
    if (!character) {
      *error = kUnknownOrUnsupportedKeyIdentiferError;
      return NULL;
    }
  }

  int flags = 0;
  if (prototype_event.key_code() != ui::VKEY_UNKNOWN)
    flags = prototype_event.flags();

  bool flag = false;
  if (key_event->GetBoolean(kAlt, &flag) && flag)
    flags |= ui::EF_ALT_DOWN;
  if (key_event->GetBoolean(kCtrl, &flag) && flag)
    flags |= ui::EF_CONTROL_DOWN;
  if (key_event->GetBoolean(kShift, &flag) && flag)
    flags |= ui::EF_SHIFT_DOWN;
  if (key_event->GetBoolean(kMeta, &flag) && flag) {
    // ui::KeyEvent does not have a Meta flag, so return an error for now.
    *error = kUnsupportedModifierError;
    return NULL;
  }

  ui::KeyEvent* event = new ui::KeyEvent(
      type, prototype_event.key_code(), flags, prototype_event.is_char());
  if (character) {
    event->set_character(character);
    event->set_unmodified_character(character);
  }

  return event;
}

}  // namespace keyboard
