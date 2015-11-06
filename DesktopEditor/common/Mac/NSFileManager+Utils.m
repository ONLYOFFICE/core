//
//  NSFileManager+Utils.m
//  UTILS
//
//  Created by alexey.musinov on 21.09.15.
//  Copyright © 2015 Ascensio System SIA. All rights reserved.
//

#import "NSFileManager+Utils.h"

@implementation NSFileManager(Utils)

- (BOOL)copyFolderAtPath:(NSString*)sourceFolder toDestinationFolderAtPath:(NSString*)destinationFolder {
    destinationFolder = [destinationFolder stringByAppendingPathComponent:[sourceFolder lastPathComponent]];
    
    NSError * error = nil;
    if ([self fileExistsAtPath:destinationFolder])
    {
        if (![self removeItemAtPath:destinationFolder error:&error])
        {
            NSLog(@"Could not remove old files. Error:%@",error);
            return NO;
        }
    }
    
    error = nil;
    if (!([self copyItemAtPath:sourceFolder toPath:destinationFolder error:&error]))
    {
        NSLog(@"Could not copy report at path %@ to path %@. error %@",sourceFolder, destinationFolder, error);
        return NO;
    }
    
    return YES;
}
- (NSString*)createTemporaryDirectory {
    
    NSError *error = nil;
    NSString* path = [NSString stringWithFormat:@"%@%@", NSTemporaryDirectory(), [NSUUID UUID].UUIDString];
    
    if(![[NSFileManager defaultManager] createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:&error])
    {
        NSLog(@"Could not creat temp path %@. error %@", path, error);
        return @"";
    }
    
    return path;
}

@end
