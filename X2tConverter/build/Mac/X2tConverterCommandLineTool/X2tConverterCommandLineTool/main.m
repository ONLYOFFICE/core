//
//  main.m
//  X2tConverterCommandLineTool
//
//  Created by alexey.musinov on 01.04.15.
//  Copyright (c) 2015 Ascensio System SIA. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "X2tConverter.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        //int * crash = NULL;//test
        //*crash = 1;
        
        NSArray *arguments = [[NSProcessInfo processInfo] arguments];
        if (arguments && arguments.count >= 4) {
            
            NSDate *startTime = [NSDate date];
            
            NSString* fullFileName = arguments[1];
            NSString* fullFileNameTo = arguments[2];
            NSString* tempDir = arguments[3];
            NSString* fontsPath = arguments[4];
             
            X2tConverter* converter = [[X2tConverter alloc]init];
            [converter sdk_docx2doct_bin:fullFileName nsTo:fullFileNameTo nsTemp:tempDir nsFontPath:fontsPath];

            NSLog(@"%@ - time conversion: %fs", fullFileName, [[NSDate date] timeIntervalSinceDate:startTime]);
        }
    }
    return 0;
}
