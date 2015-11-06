//
//  NSString+StringUtils.m
//  UTILS
//
//  Created by alexey.musinov on 10.08.15.
//  Copyright (c) 2015 Ascensio System SIA. All rights reserved.
//

#import "NSString+StringUtils.h"

@implementation NSString (StringUtils)

+ (id)stringWithWString:(const std::wstring&)string {
    if (string.length() < 1) {
        return @"";
    }
    
    return [[NSString alloc] initWithBytes:(char*)string.data()
                                    length:string.size()*sizeof(wchar_t)
                                  encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE)];
}

+ (id)stringWithAString:(const std::string&)string {
    if (string.length() < 1) {
        return @"";
    }
    
    return [[NSString alloc] initWithBytes:(char*)string.data()
                                    length:string.size()*sizeof(char)
                                  encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF8)];
}

+ (NSMutableArray*)stringsArray:(const std::vector<std::wstring>&)sources {
    size_t count = sources.size();
    NSMutableArray* array = [NSMutableArray arrayWithCapacity:count];
    for (size_t i = 0; i < count; ++i) {
        [array addObject:[NSString stringWithWString:sources[i]]];
    }
    return array;
}

- (std::wstring)stdwstring {
    NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
    NSData* data = [self dataUsingEncoding:encode];    
    return std::wstring((wchar_t*)data.bytes, data.length / sizeof(wchar_t));
}

- (std::string)stdstring {
    NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF8 );
    NSData* data = [self dataUsingEncoding:encode];
    return std::string((char*)data.bytes, data.length);
}

@end