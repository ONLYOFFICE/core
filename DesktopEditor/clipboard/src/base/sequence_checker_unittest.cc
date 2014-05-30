// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/sequence_checker.h"

#include <cstddef>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/test/null_task_runner.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

namespace {

// Call various methods of SequenceChecker to make sure nothing blows
// up in either debug or release mode.
TEST(SequenceCheckerTest, Basic) {
  SequenceChecker sequence_checker(new NullTaskRunner());
  sequence_checker.CalledOnValidSequence();
  sequence_checker.ChangeSequence(NULL);
  sequence_checker.CalledOnValidSequence();
}

}  // namespace

}  // namespace base
