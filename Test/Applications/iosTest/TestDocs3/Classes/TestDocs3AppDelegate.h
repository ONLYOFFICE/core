//
//  TestDocs3AppDelegate.h
//  TestDocs3
//
//  Created by naceka on 24.04.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class TestDocs3ViewController;

@interface TestDocs3AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    TestDocs3ViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet TestDocs3ViewController *viewController;

@end

