// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/range/range.h"

TEST(RangeTest, FromNSRange) {
  NSRange nsr = NSMakeRange(10, 3);
  ui::Range r(nsr);
  EXPECT_EQ(nsr.location, r.start());
  EXPECT_EQ(13U, r.end());
  EXPECT_EQ(nsr.length, r.length());
  EXPECT_FALSE(r.is_reversed());
  EXPECT_TRUE(r.IsValid());
}

TEST(RangeTest, ToNSRange) {
  ui::Range r(10, 12);
  NSRange nsr = r.ToNSRange();
  EXPECT_EQ(10U, nsr.location);
  EXPECT_EQ(2U, nsr.length);
}

TEST(RangeTest, ReversedToNSRange) {
  ui::Range r(20, 10);
  NSRange nsr = r.ToNSRange();
  EXPECT_EQ(10U, nsr.location);
  EXPECT_EQ(10U, nsr.length);
}

TEST(RangeTest, FromNSRangeInvalid) {
  NSRange nsr = NSMakeRange(NSNotFound, 0);
  ui::Range r(nsr);
  EXPECT_FALSE(r.IsValid());
}

TEST(RangeTest, ToNSRangeInvalid) {
  ui::Range r(ui::Range::InvalidRange());
  NSRange nsr = r.ToNSRange();
  EXPECT_EQ(NSNotFound, nsr.location);
  EXPECT_EQ(0U, nsr.length);
}
