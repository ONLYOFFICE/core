// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/events/key_identifier_conversion.h"

#include "base/basictypes.h"
#include "base/logging.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/events/event.h"
#include "ui/base/keycodes/keyboard_codes.h"

namespace ui {

TEST(KeyEventFromKeyIdentifierTest, MatchOnIdentifier) {
  EXPECT_EQ(ui::VKEY_APPS, KeyEventFromKeyIdentifier("Apps").key_code());
  EXPECT_EQ(ui::VKEY_UNKNOWN,
      KeyEventFromKeyIdentifier("Nonsense").key_code());
}

TEST(KeyEventFromKeyIdentifierTest, MatchOnCharacter) {
  EXPECT_EQ(ui::VKEY_A, KeyEventFromKeyIdentifier("a").key_code());
  EXPECT_EQ(ui::VKEY_A, KeyEventFromKeyIdentifier("A").key_code());
  EXPECT_EQ(ui::VKEY_OEM_PERIOD, KeyEventFromKeyIdentifier(">").key_code());

  std::string non_printing_char(" ");
  non_printing_char[0] = static_cast<char>(1);
  EXPECT_EQ(ui::VKEY_UNKNOWN,
      KeyEventFromKeyIdentifier(non_printing_char).key_code());
}

TEST(KeyEventFromKeyIdentifierTest, MatchOnUnicodeCodepoint) {
  EXPECT_EQ(ui::VKEY_A, KeyEventFromKeyIdentifier("U+0041").key_code());
  EXPECT_EQ(ui::VKEY_A, KeyEventFromKeyIdentifier("U+0061").key_code());
  EXPECT_EQ(ui::VKEY_DELETE, KeyEventFromKeyIdentifier("U+007F").key_code());

  // this one exists in the map, but has no valid ui::VKEY
  EXPECT_EQ(ui::VKEY_UNKNOWN, KeyEventFromKeyIdentifier("U+030A").key_code());

  // this one is not in the map
  EXPECT_EQ(ui::VKEY_UNKNOWN, KeyEventFromKeyIdentifier("U+0001").key_code());
}

TEST(KeyEventFromKeyIdentifierTest, DoesNotMatchEmptyString) {
  EXPECT_EQ(ui::VKEY_UNKNOWN, KeyEventFromKeyIdentifier("").key_code());
}

TEST(KeyEventFromKeyIdentifierTest, ShiftModifiersAreSet) {
  EXPECT_EQ(0, KeyEventFromKeyIdentifier("1").flags());

  const char* keys_with_shift[] = {
    "~", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+",
    "{", "}", "|", ":", "<", ">", "?", "\""
  };
  int kNumKeysWithShift = arraysize(keys_with_shift);

  for (int i = 0; i < kNumKeysWithShift; ++i) {
    EXPECT_EQ(ui::EF_SHIFT_DOWN,
        KeyEventFromKeyIdentifier(keys_with_shift[i]).flags());
  }
}

}  // namespace ui
