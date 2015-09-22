//
//  NSFileManager+Utils.h
//  Utils
//
//  Created by alexey.musinov on 21.09.15.
//  Copyright © 2015 Ascensio System SIA. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSFileManager (Utils)
- (BOOL)copyFolderAtPath:(NSString *)sourceFolder toDestinationFolderAtPath:(NSString*)destinationFolder;
@end
