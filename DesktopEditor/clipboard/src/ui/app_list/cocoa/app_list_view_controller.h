// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_APP_LIST_VIEW_CONTROLLER_H_
#define UI_APP_LIST_COCOA_APP_LIST_VIEW_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#include "ui/app_list/app_list_export.h"
#import "ui/app_list/cocoa/apps_pagination_model_observer.h"

namespace app_list {
class AppListViewDelegate;
}

@class AppsGridController;

// Controller for the top-level view of the app list UI. It creates and hosts an
// AppsGridController (displaying an AppListModel), and pager control for
// navigating between pages in the grid.
APP_LIST_EXPORT
@interface AppListViewController :
    NSViewController<AppsPaginationModelObserver> {
 @private
  scoped_nsobject<AppsGridController> appsGridController_;
  scoped_nsobject<NSSegmentedControl> pagerControl_;
  scoped_ptr<app_list::AppListViewDelegate> delegate_;
}

- (AppsGridController*)appsGridController;

- (NSSegmentedControl*)pagerControl;

- (app_list::AppListViewDelegate*)delegate;

- (void)setDelegate:(scoped_ptr<app_list::AppListViewDelegate>)newDelegate;

@end

#endif  // UI_APP_LIST_COCOA_APP_LIST_VIEW_CONTROLLER_H_
