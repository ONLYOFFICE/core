// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/sequence_checker_impl.h"

#include "base/sequenced_task_runner.h"

namespace base {

SequenceCheckerImpl::SequenceCheckerImpl(
    const scoped_refptr<SequencedTaskRunner>& sequenced_task_runner)
    : sequenced_task_runner_(sequenced_task_runner) {}

SequenceCheckerImpl::~SequenceCheckerImpl() {}

bool SequenceCheckerImpl::CalledOnValidSequence() const {
  AutoLock auto_lock(lock_);
  return sequenced_task_runner_.get() ?
      sequenced_task_runner_->RunsTasksOnCurrentThread() :
      thread_checker_.CalledOnValidThread();
}

void SequenceCheckerImpl::ChangeSequence(
    const scoped_refptr<SequencedTaskRunner>& sequenced_task_runner) {
  AutoLock auto_lock(lock_);
  sequenced_task_runner_ = sequenced_task_runner;
  thread_checker_.DetachFromThread();
}

}  // namespace base
