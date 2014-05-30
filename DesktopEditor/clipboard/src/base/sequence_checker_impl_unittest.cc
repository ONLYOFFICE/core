// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/sequence_checker_impl.h"

#include <cstddef>

#include "base/bind.h"
#include "base/compiler_specific.h"
#include "base/location.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop.h"
#include "base/sequenced_task_runner.h"
#include "base/threading/thread.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

namespace {

// Implementation of SequencedTaskRunner that lets us control what
// RunsTasksOnCurrentThread() returns.
class FakeTaskRunner : public SequencedTaskRunner {
 public:
  FakeTaskRunner() : runs_tasks_on_current_thread_(false) {}

  void SetRunsTasksOnCurrentThread(bool runs_tasks_on_current_thread) {
    runs_tasks_on_current_thread_ = runs_tasks_on_current_thread;
  }

  // SequencedTaskRunner implementation.
  virtual bool PostDelayedTask(const tracked_objects::Location& from_here,
                               const Closure& task,
                               TimeDelta delay) OVERRIDE {
    ADD_FAILURE();
    return false;
  }

  virtual bool PostNonNestableDelayedTask(
      const tracked_objects::Location& from_here,
      const Closure& task,
      TimeDelta delay) OVERRIDE {
    ADD_FAILURE();
    return false;
  }

  virtual bool RunsTasksOnCurrentThread() const OVERRIDE {
    return runs_tasks_on_current_thread_;
  }

 protected:
  virtual ~FakeTaskRunner() {}

 private:
  bool runs_tasks_on_current_thread_;
};

class SequenceCheckerImplTest : public ::testing::Test {
};

// Create a SequenceCheckerImpl with a SequencedTaskRunner and make
// sure that CalledOnValidSequence() returns what that SequencedTaskRunner
// returns for RunsTasksOnCurrentThread().
TEST_F(SequenceCheckerImplTest, CalledOnValidSequenceNonNull) {
  const scoped_refptr<FakeTaskRunner> fake_sequenced_task_runner(
      new FakeTaskRunner());

  const SequenceCheckerImpl sequence_checker_impl(fake_sequenced_task_runner);
  EXPECT_FALSE(sequence_checker_impl.CalledOnValidSequence());

  fake_sequenced_task_runner->SetRunsTasksOnCurrentThread(true);
  EXPECT_TRUE(sequence_checker_impl.CalledOnValidSequence());

  fake_sequenced_task_runner->SetRunsTasksOnCurrentThread(false);
  EXPECT_FALSE(sequence_checker_impl.CalledOnValidSequence());
}

void ExpectCalledOnValidSequence(
    const tracked_objects::Location& location,
    const SequenceCheckerImpl* sequence_checker_impl,
    bool expected_value) {
  EXPECT_EQ(expected_value, sequence_checker_impl->CalledOnValidSequence())
      << location.ToString();
}

// Create a SequenceCheckerImpl with no SequencedTaskRunner and make
// sure that CalledOnValidSequence() behaves like
// ThreadChecker::CalledOnValidThread().
TEST_F(SequenceCheckerImplTest, CalledOnValidSequenceNull) {
  const SequenceCheckerImpl sequence_checker_impl(NULL);
  EXPECT_TRUE(sequence_checker_impl.CalledOnValidSequence());

  {
    Thread thread("thread 1");
    ASSERT_TRUE(thread.Start());
    thread.message_loop()->PostTask(
        FROM_HERE, Bind(&ExpectCalledOnValidSequence,
                        FROM_HERE,
                        Unretained(&sequence_checker_impl),
                        false));
  }

  EXPECT_TRUE(sequence_checker_impl.CalledOnValidSequence());
}

// Create a SequenceCheckerImpl with a SequencedTaskRunner and switch
// it to another one. CalledOnValidSequence() should return what its
// underlying SequencedTaskRunner returns for
// RunsTasksOnCurrentThread().
TEST_F(SequenceCheckerImplTest, ChangeSequenceNonNull) {
  const scoped_refptr<FakeTaskRunner> fake_sequenced_task_runner1(
      new FakeTaskRunner());

  const scoped_refptr<FakeTaskRunner> fake_sequenced_task_runner2(
      new FakeTaskRunner());

  SequenceCheckerImpl sequence_checker_impl(fake_sequenced_task_runner1);
  EXPECT_FALSE(sequence_checker_impl.CalledOnValidSequence());

  fake_sequenced_task_runner2->SetRunsTasksOnCurrentThread(true);
  EXPECT_FALSE(sequence_checker_impl.CalledOnValidSequence());

  sequence_checker_impl.ChangeSequence(fake_sequenced_task_runner2);
  EXPECT_TRUE(sequence_checker_impl.CalledOnValidSequence());

  sequence_checker_impl.ChangeSequence(fake_sequenced_task_runner1);
  EXPECT_FALSE(sequence_checker_impl.CalledOnValidSequence());
}

// Create a SequenceCheckerImpl with a SequencedTaskRunner and switch
// it to a NULL one. CalledOnValidSequence() should then behave like
// ThreadChecker::CalledOnValidThread().
TEST_F(SequenceCheckerImplTest, ChangeSequenceNull) {
  const scoped_refptr<FakeTaskRunner> fake_sequenced_task_runner(
      new FakeTaskRunner());

  SequenceCheckerImpl sequence_checker_impl(fake_sequenced_task_runner);
  EXPECT_FALSE(sequence_checker_impl.CalledOnValidSequence());

  sequence_checker_impl.ChangeSequence(NULL);
  // Binds to current thread.
  EXPECT_TRUE(sequence_checker_impl.CalledOnValidSequence());
  {
    Thread thread("thread 1");
    ASSERT_TRUE(thread.Start());
    thread.message_loop()->PostTask(
        FROM_HERE, Bind(&ExpectCalledOnValidSequence,
                        FROM_HERE,
                        Unretained(&sequence_checker_impl),
                        false));
  }

  EXPECT_TRUE(sequence_checker_impl.CalledOnValidSequence());

  sequence_checker_impl.ChangeSequence(NULL);
  // Binds to worker thread.
  {
    Thread thread("thread 2");
    ASSERT_TRUE(thread.Start());
    thread.message_loop()->PostTask(
        FROM_HERE, Bind(&ExpectCalledOnValidSequence,
                        FROM_HERE,
                        Unretained(&sequence_checker_impl),
                        true));
  }
  EXPECT_FALSE(sequence_checker_impl.CalledOnValidSequence());
}

// Create a SequenceCheckerImpl with the current thread's task runner
// and switch it to other task runners. CalledOnValidSequence() should
// return true only when it's on the correct thread.
TEST_F(SequenceCheckerImplTest, MultipleThreads) {
  MessageLoop loop;

  SequenceCheckerImpl sequence_checker_impl(loop.message_loop_proxy());
  EXPECT_TRUE(sequence_checker_impl.CalledOnValidSequence());

  {
    Thread thread("thread 1");
    ASSERT_TRUE(thread.Start());
    thread.message_loop()->PostTask(
        FROM_HERE, Bind(&ExpectCalledOnValidSequence,
                        FROM_HERE,
                        Unretained(&sequence_checker_impl),
                        false));
    thread.message_loop()->PostTask(
        FROM_HERE, Bind(&SequenceCheckerImpl::ChangeSequence,
                        Unretained(&sequence_checker_impl),
                        thread.message_loop_proxy()));
    thread.message_loop()->PostTask(
        FROM_HERE, Bind(&ExpectCalledOnValidSequence,
                        FROM_HERE,
                        Unretained(&sequence_checker_impl),
                        true));
  }

  EXPECT_FALSE(sequence_checker_impl.CalledOnValidSequence());

  sequence_checker_impl.ChangeSequence(loop.message_loop_proxy());
  EXPECT_TRUE(sequence_checker_impl.CalledOnValidSequence());

  {
    Thread thread("thread 2");
    ASSERT_TRUE(thread.Start());
    thread.message_loop()->PostTask(
        FROM_HERE, Bind(&ExpectCalledOnValidSequence,
                        FROM_HERE,
                        Unretained(&sequence_checker_impl),
                        false));
  }

  EXPECT_TRUE(sequence_checker_impl.CalledOnValidSequence());
}

}  // namespace

}  // namespace base
