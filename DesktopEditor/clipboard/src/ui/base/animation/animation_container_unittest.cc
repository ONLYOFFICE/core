// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/memory/scoped_ptr.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/animation/animation_container.h"
#include "ui/base/animation/animation_container_observer.h"
#include "ui/base/animation/linear_animation.h"
#include "ui/base/animation/test_animation_delegate.h"

using testing::AtLeast;

namespace ui {

namespace {

class MockObserver : public AnimationContainerObserver {
 public:
  MockObserver() {}

  MOCK_METHOD1(AnimationContainerProgressed, void(AnimationContainer*));
  MOCK_METHOD1(AnimationContainerEmpty, void(AnimationContainer*));

 private:
  DISALLOW_COPY_AND_ASSIGN(MockObserver);
};

class TestAnimation : public LinearAnimation {
 public:
  explicit TestAnimation(AnimationDelegate* delegate)
      : LinearAnimation(20, 20, delegate) {
  }

  virtual void AnimateToState(double state) OVERRIDE {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TestAnimation);
};

}  // namespace

class AnimationContainerTest: public testing::Test {
 private:
  base::MessageLoopForUI message_loop_;
};

// Makes sure the animation ups the ref count of the container and releases it
// appropriately.
TEST_F(AnimationContainerTest, Ownership) {
  TestAnimationDelegate delegate;
  scoped_refptr<AnimationContainer> container(new AnimationContainer());
  scoped_ptr<Animation> animation(new TestAnimation(&delegate));
  animation->SetContainer(container.get());
  // Setting the container should up the ref count.
  EXPECT_FALSE(container->HasOneRef());

  animation.reset();

  // Releasing the animation should decrement the ref count.
  EXPECT_TRUE(container->HasOneRef());
}

// Makes sure multiple animations are managed correctly.
TEST_F(AnimationContainerTest, Multi) {
  TestAnimationDelegate delegate1;
  TestAnimationDelegate delegate2;

  scoped_refptr<AnimationContainer> container(new AnimationContainer());
  TestAnimation animation1(&delegate1);
  TestAnimation animation2(&delegate2);
  animation1.SetContainer(container.get());
  animation2.SetContainer(container.get());

  // Start both animations.
  animation1.Start();
  EXPECT_TRUE(container->is_running());
  animation2.Start();
  EXPECT_TRUE(container->is_running());

  // Run the message loop the delegate quits the message loop when notified.
  base::MessageLoop::current()->Run();

  // Both timers should have finished.
  EXPECT_TRUE(delegate1.finished());
  EXPECT_TRUE(delegate2.finished());

  // And the container should no longer be runnings.
  EXPECT_FALSE(container->is_running());
}

// Makes sure observer is notified appropriately.
TEST_F(AnimationContainerTest, Observer) {
  MockObserver observer;
  TestAnimationDelegate delegate1;

  scoped_refptr<AnimationContainer> container(new AnimationContainer());
  container->set_observer(&observer);
  TestAnimation animation1(&delegate1);
  animation1.SetContainer(container.get());

  // We expect to get these two calls: the animation progressed, and then when
  // the animation completed the container went empty.
  EXPECT_CALL(observer, AnimationContainerProgressed(container.get())).Times(
      AtLeast(1));
  EXPECT_CALL(observer, AnimationContainerEmpty(container.get())).Times(1);

  // Start the animation.
  animation1.Start();
  EXPECT_TRUE(container->is_running());

  // Run the message loop. The delegate quits the message loop when notified.
  base::MessageLoop::current()->Run();

  // The timer should have finished.
  EXPECT_TRUE(delegate1.finished());

  // And the container should no longer be running.
  EXPECT_FALSE(container->is_running());

  container->set_observer(NULL);
}

}  // namespace ui
