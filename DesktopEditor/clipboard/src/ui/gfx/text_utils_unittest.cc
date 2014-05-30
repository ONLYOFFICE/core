// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/gfx/text_utils.h"

#include "base/utf_string_conversions.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace gfx {
namespace {

const char16 kAcceleratorChar = '&';

TEST(TextUtilsTest, RemoveAcceleratorChar) {
  struct TestData {
    const char* input;
    int accelerated_char_pos;
    int accelerated_char_span;
    const char* output;
  } cases[] = {
    { "", -1, 0, "" },
    { "&", -1, 0, "" },
    { "no accelerator", -1, 0, "no accelerator" },
    { "&one accelerator", 0, 1, "one accelerator" },
    { "one &accelerator", 4, 1, "one accelerator" },
    { "one_accelerator&", -1, 0, "one_accelerator" },
    { "&two &accelerators", 4, 1, "two accelerators" },
    { "two &accelerators&", 4, 1, "two accelerators" },
    { "two& &accelerators", 4, 1, "two accelerators" },
    { "&&escaping", -1, 0, "&escaping" },
    { "escap&&ing", -1, 0, "escap&ing" },
    { "escaping&&", -1, 0, "escaping&" },
    { "&mix&&ed", 0, 1, "mix&ed" },
    { "&&m&ix&&e&d&", 6, 1, "&mix&ed" },
    { "&&m&&ix&ed&&", 5, 1, "&m&ixed&" },
    { "&m&&ix&ed&&", 4, 1, "m&ixed&" },
    // U+1D49C MATHEMATICAL SCRIPT CAPITAL A, which occupies two |char16|'s.
    { "&\xF0\x9D\x92\x9C", 0, 2, "\xF0\x9D\x92\x9C" },
    { "Test&\xF0\x9D\x92\x9Cing", 4, 2, "Test\xF0\x9D\x92\x9Cing" },
    { "Test\xF0\x9D\x92\x9C&ing", 6, 1, "Test\xF0\x9D\x92\x9Cing" },
    { "Test&\xF0\x9D\x92\x9C&ing", 6, 1, "Test\xF0\x9D\x92\x9Cing" },
    { "Test&\xF0\x9D\x92\x9C&&ing", 4, 2, "Test\xF0\x9D\x92\x9C&ing" },
    { "Test&\xF0\x9D\x92\x9C&\xF0\x9D\x92\x9Cing", 6, 2,
      "Test\xF0\x9D\x92\x9C\xF0\x9D\x92\x9Cing" },
  };

  for (size_t i = 0; i < ARRAYSIZE_UNSAFE(cases); ++i) {
    int accelerated_char_pos;
    int accelerated_char_span;
    string16 result = RemoveAcceleratorChar(UTF8ToUTF16(cases[i].input),
                                            kAcceleratorChar,
                                            &accelerated_char_pos,
                                            &accelerated_char_span);
    EXPECT_EQ(result, UTF8ToUTF16(cases[i].output));
    EXPECT_EQ(accelerated_char_pos, cases[i].accelerated_char_pos);
    EXPECT_EQ(accelerated_char_span, cases[i].accelerated_char_span);
  }
}

}  // namespace
}  // namespace gfx
