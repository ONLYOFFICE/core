//
//  main.m
//  DocumentConverter
//
//  Created by Nct Devices on 21.10.2019.
//  Copyright © 2019 Nct Devices. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

#include "../../../../../../src/dylib/x2t.h"

int main(int argc, char * argv[]) {
    
    int result = X2T_Convert(0, NULL);
    
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
