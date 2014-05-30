// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/app_list/signin_delegate.h"
#include "ui/app_list/signin_delegate_observer.h"

namespace app_list {

SigninDelegate::SigninDelegate() {}

SigninDelegate::~SigninDelegate() {}

void SigninDelegate::AddObserver(SigninDelegateObserver* observer) {
  observers_.AddObserver(observer);
}

void SigninDelegate::RemoveObserver(SigninDelegateObserver* observer) {
  observers_.RemoveObserver(observer);
}

void SigninDelegate::NotifySigninSuccess() {
  FOR_EACH_OBSERVER(app_list::SigninDelegateObserver, observers_,
                    OnSigninSuccess());
}

}  // namespace app_list
