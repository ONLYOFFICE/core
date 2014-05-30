// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_IME_CHARACTER_COMPOSER_H_
#define UI_BASE_IME_CHARACTER_COMPOSER_H_

#include <vector>

#include "base/string_util.h"
#include "ui/base/ui_export.h"

namespace ui {

// A class to recognize compose and dead key sequence.
// Outputs composed character.
//
// TODO(hashimoto): support unicode character composition starting with
// Ctrl-Shift-U. http://crosbug.com/15925
class UI_EXPORT CharacterComposer {
 public:
  CharacterComposer();
  ~CharacterComposer();

  void Reset();

  // Filters keypress.
  // Returns true if the keypress is recognized as a part of composition
  // sequence.
  // |keyval| must be a GDK_KEY_* constants.
  // |flags| must be a combination of ui::EF_* flags.
  //
  // composed_character() returns non empty string when there is a character
  // composed after this method returns true.
  // preedit_string() returns non empty string when there is a preedit string
  // after this method returns true.
  // Return values of composed_character() and preedit_string() are empty after
  // this method returns false.
  bool FilterKeyPress(unsigned int keyval, unsigned int flags);

  // Returns a string consisting of composed character.
  // Empty string is returned when there is no composition result.
  const string16& composed_character() const { return composed_character_; }

  // Returns the preedit string.
  const string16& preedit_string() const { return preedit_string_; }

 private:
  // An enum to describe composition mode.
  enum CompositionMode {
    // This is the initial state.
    // Composite a character with dead-keys and compose-key.
    KEY_SEQUENCE_MODE,
    // Composite a character with a hexadecimal unicode sequence.
    HEX_MODE,
  };

  // Filters keypress in key sequence mode.
  bool FilterKeyPressSequenceMode(unsigned int keyval, unsigned int flags);

  // Filters keypress in hexadecimal mode.
  bool FilterKeyPressHexMode(unsigned int keyval, unsigned int flags);

  // Commit a character composed from hexadecimal uncode sequence
  void CommitHex();

  // Updates preedit string in hexadecimal mode.
  void UpdatePreeditStringHexMode();

  // Remembers keypresses previously filtered.
  std::vector<unsigned int> compose_buffer_;

  // A string representing the composed character.
  string16 composed_character_;

  // Preedit string.
  string16 preedit_string_;

  // Composition mode which this instance is in.
  CompositionMode composition_mode_;

  DISALLOW_COPY_AND_ASSIGN(CharacterComposer);
};

}  // namespace ui

#endif  // UI_BASE_IME_CHARACTER_COMPOSER_H_
