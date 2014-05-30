// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_SIGNIN_DELEGATE_OBSERVER_H_
#define UI_APP_LIST_SIGNIN_DELEGATE_OBSERVER_H_

#include "ui/app_list/app_list_export.h"

namespace app_list {

// Abstract interface to allow handling app list signin success.
class APP_LIST_EXPORT SigninDelegateObserver {
 public:
  // Called by the SigninDelegate when signin has successfully completed.
  virtual void OnSigninSuccess() = 0;

 protected:
  virtual ~SigninDelegateObserver() {}
};

}  // namespace app_list

#endif  // UI_APP_LIST_SIGNIN_DELEGATE_OBSERVER_H_
