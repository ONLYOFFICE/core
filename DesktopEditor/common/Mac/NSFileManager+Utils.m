/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

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
    
    if(![self createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:&error])
    {
        NSLog(@"Could not creat temp path %@. error %@", path, error);
        return @"";
    }
    
    return path;
}

- (void)createDirectory:(NSString*)directory {
    NSError *error = nil;
    if(![self createDirectoryAtPath:directory withIntermediateDirectories:YES attributes:nil error:&error]) {
        NSLog(@"Failed to create directory \"%@\". Error: %@", directory, error);
    }
}

- (NSString*)createCacheDirectory:(NSString*)prefix {
    NSError *error = nil;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    if (paths) {
        NSString *cacheDirectory = [paths objectAtIndex:0];
        if (cacheDirectory) {
            NSString* path = [NSString stringWithFormat:@"%@/%@", cacheDirectory, prefix];
            if(![self createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:&error])
            {
                NSLog(@"Could not creat temp path %@. error %@", path, error);
                return @"";
            }
            
            return path;            
        }
    }
    
    return @"";
}

@end
