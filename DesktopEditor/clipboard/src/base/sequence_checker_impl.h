// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SEQUENCE_CHECKER_IMPL_H_
#define BASE_SEQUENCE_CHECKER_IMPL_H_

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread_checker_impl.h"

namespace base {

class SequencedTaskRunner;

// SequenceCheckerImpl is used to help verify that some methods of a
// class are called in sequence -- that is, called from the same
// SequencedTaskRunner. It is a generalization of ThreadChecker; in
// particular, it behaves exactly like ThreadChecker if its passed a
// NULL SequencedTaskRunner.
class BASE_EXPORT SequenceCheckerImpl {
 public:
  // |sequenced_task_runner| can be NULL.  In that case, this object
  // behaves exactly like a ThreadChecker bound to the current thread,
  // i.e.  CalledOnValidSequence() behaves like CalledOnValidThread().
  explicit SequenceCheckerImpl(
      const scoped_refptr<SequencedTaskRunner>& sequenced_task_runner);
  ~SequenceCheckerImpl();

  // Returns whether the we are being called on the underyling
  // SequencedTaskRunner.  If we're not bound to a
  // |sequenced_task_runner|, returns whether we are being called on
  // the underlying ThreadChecker's thread.
  bool CalledOnValidSequence() const;

  // Changes the underyling SequencedTaskRunner.
  // |sequenced_task_runner| can be NULL.  In that case, this object
  // behaves exactly like a ThreadChecker that has been detached from
  // its thread, i.e. we will be bound to the thread on which we next
  // call CalledOnValidSequence().
  void ChangeSequence(
      const scoped_refptr<SequencedTaskRunner>& sequenced_task_runner);

 private:
  // Guards all variables below.
  mutable Lock lock_;
  scoped_refptr<SequencedTaskRunner> sequenced_task_runner_;
  // Used if |sequenced_task_runner_| is NULL.
  ThreadCheckerImpl thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(SequenceCheckerImpl);
};

}  // namespace base

#endif  // BASE_SEQUENCE_CHECKER_IMPL_H_
