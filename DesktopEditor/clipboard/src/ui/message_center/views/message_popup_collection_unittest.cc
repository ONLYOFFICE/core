// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/message_center/views/message_popup_collection.h"

#include <list>

#include "base/message_loop.h"
#include "base/strings/string_number_conversions.h"
#include "base/utf_string_conversions.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/gfx/rect.h"
#include "ui/message_center/fake_message_center.h"
#include "ui/message_center/views/toast_contents_view.h"
#include "ui/views/test/views_test_base.h"
#include "ui/views/widget/widget.h"

namespace message_center {
namespace test {

class MessagePopupCollectionTest : public views::ViewsTestBase {
 public:
  virtual void SetUp() OVERRIDE {
    views::ViewsTestBase::SetUp();
    MessageCenter::Initialize();
    collection_.reset(
        new MessagePopupCollection(GetContext(), MessageCenter::Get()));
    // This size fits test machines resolution and also can keep a few toasts
    // w/o ill effects of hitting the screen overflow. This allows us to assume
    // and verify normal layout of the toast stack.
    collection_->SetWorkAreaForTest(gfx::Rect(0, 0, 600, 400));
    id_ = 0;
  }

  virtual void TearDown() OVERRIDE {
    collection_->CloseAllWidgets();
    collection_.reset();
    MessageCenter::Shutdown();
    views::ViewsTestBase::TearDown();
  }

 protected:
  MessagePopupCollection* collection() { return collection_.get(); }

  size_t GetToastCounts() {
    return collection_->toasts_.size();
  }

  bool IsToastShown(const std::string& id) {
    views::Widget* widget = collection_->GetWidgetForTest(id);
    return widget && widget->IsVisible();
  }

  views::Widget* GetWidget(const std::string& id) {
    return collection_->GetWidgetForTest(id);
  }

  ToastContentsView* GetToast(const std::string& id) {
    for (MessagePopupCollection::Toasts::iterator iter =
             collection_->toasts_.begin();
         iter != collection_->toasts_.end(); ++iter) {
      if ((*iter)->id() == id)
        return *iter;
    }
    return NULL;
  }

  std::string AddNotification() {
    std::string id = base::IntToString(id_++);
    MessageCenter::Get()->AddNotification(
        NOTIFICATION_TYPE_BASE_FORMAT, id, UTF8ToUTF16("test title"),
        UTF8ToUTF16("test message"), string16() /* display_source */,
        "" /* extension_id */, NULL);
    return id;
  }

  // Assumes there is non-zero pending work.
  void WaitForTransitionsDone() {
    collection_->RunLoopForTest();
  }

  void CloseAllToastsAndWait() {
    // Assumes there is at least one toast to close.
    EXPECT_TRUE(GetToastCounts() > 0);
    MessageCenter::Get()->RemoveAllNotifications(false);
    WaitForTransitionsDone();
  }

  gfx::Rect GetToastRectAt(size_t index) {
    return collection_->GetToastRectAt(index);
  }

 private:
  scoped_ptr<MessagePopupCollection> collection_;
  int id_;
};

TEST_F(MessagePopupCollectionTest, DismissOnClick) {
  std::string id1 = AddNotification();
  std::string id2 = AddNotification();
  WaitForTransitionsDone();
  EXPECT_EQ(2u, GetToastCounts());
  EXPECT_TRUE(IsToastShown(id1));
  EXPECT_TRUE(IsToastShown(id2));

  MessageCenter::Get()->ClickOnNotification(id2);
  WaitForTransitionsDone();
  EXPECT_EQ(1u, GetToastCounts());
  EXPECT_TRUE(IsToastShown(id1));
  EXPECT_FALSE(IsToastShown(id2));

  MessageCenter::Get()->ClickOnNotificationButton(id1, 0);
  WaitForTransitionsDone();
  EXPECT_EQ(0u, GetToastCounts());
  EXPECT_FALSE(IsToastShown(id1));
  EXPECT_FALSE(IsToastShown(id2));
}

TEST_F(MessagePopupCollectionTest, ShutdownDuringShowing) {
  std::string id1 = AddNotification();
  std::string id2 = AddNotification();
  WaitForTransitionsDone();
  EXPECT_EQ(2u, GetToastCounts());
  EXPECT_TRUE(IsToastShown(id1));
  EXPECT_TRUE(IsToastShown(id2));

  // Finish without cleanup of notifications, which may cause use-after-free.
  // See crbug.com/236448
  GetWidget(id1)->CloseNow();
  collection()->OnMouseExited(GetToast(id2));
}

TEST_F(MessagePopupCollectionTest, DefaultPositioning) {
  std::string id0 = AddNotification();
  std::string id1 = AddNotification();
  std::string id2 = AddNotification();
  std::string id3 = AddNotification();
  WaitForTransitionsDone();

  gfx::Rect r0 = GetToastRectAt(0);
  gfx::Rect r1 = GetToastRectAt(1);
  gfx::Rect r2 = GetToastRectAt(2);
  gfx::Rect r3 = GetToastRectAt(3);

  // 3 toasts are shown, equal size, vertical stack.
  EXPECT_TRUE(IsToastShown(id0));
  EXPECT_TRUE(IsToastShown(id1));
  EXPECT_TRUE(IsToastShown(id2));

  EXPECT_EQ(r0.width(), r1.width());
  EXPECT_EQ(r1.width(), r2.width());

  EXPECT_EQ(r0.height(), r1.height());
  EXPECT_EQ(r1.height(), r2.height());

  EXPECT_GT(r0.y(), r1.y());
  EXPECT_GT(r1.y(), r2.y());

  EXPECT_EQ(r0.x(), r1.x());
  EXPECT_EQ(r1.x(), r2.x());

  // The 4th toast is not shown yet.
  EXPECT_FALSE(IsToastShown(id3));
  EXPECT_EQ(0, r3.width());
  EXPECT_EQ(0, r3.height());

  CloseAllToastsAndWait();
  EXPECT_EQ(0u, GetToastCounts());
}

// TODO(dimich): Test repositioning - both normal one and when user is closing
// the toasts.

}  // namespace test
}  // namespace message_center
