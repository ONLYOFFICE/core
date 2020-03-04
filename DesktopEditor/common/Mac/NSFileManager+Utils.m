/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
