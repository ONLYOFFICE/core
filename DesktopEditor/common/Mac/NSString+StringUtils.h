//
//  NSString+StringUtils.h
//  UTILS
//
//  Created by alexey.musinov on 10.08.15.
//  Copyright (c) 2015 Ascensio System SIA. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <string>
#import <vector>

@interface NSString (StringUtils)
+ (id)stringWithWString:(const std::wstring&)string;
+ (id)stringWithAString:(const std::string&)string;
+ (NSMutableArray*)stringsArray:(const std::vector<std::wstring>&)sources;
- (std::wstring)stdwstring;
- (std::string)stdstring;
@end
