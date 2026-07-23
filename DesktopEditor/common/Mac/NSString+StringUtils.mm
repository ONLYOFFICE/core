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

+ (id)stringWithUtf8Buffer:(const char*)string length:(size_t)len
{
    if (len < 1) {
        return @"";
    }
    
    return [[NSString alloc] initWithBytes:string
                                    length:len*sizeof(char)
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

+ (std::vector<std::wstring>)stringsStdArray:(NSArray<NSString*>*)sources {
    std::vector<std::wstring> strings;
    for (NSString* str in sources) {
        strings.push_back(str.stdwstring);
    }
    return strings;
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

+ (NSString *)queryParameter:(NSURL *)url key:(NSString *)key {
    NSURLComponents *urlComponents = [NSURLComponents componentsWithURL:url
                                                resolvingAgainstBaseURL:NO];
    NSArray *queryItems = urlComponents.queryItems;
    
    if ([queryItems count] == 0)
        return @"";
    
    NSPredicate *predicate = [NSPredicate predicateWithFormat:@"name=%@", key];
    NSURLQueryItem *queryItem = [[queryItems
                                  filteredArrayUsingPredicate:predicate]
                                 firstObject];
    
    return queryItem.value;
}

- (NSDictionary *)toDictionary {
    NSError * error = nil;
    NSData * data = [self dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
    
    if (error) {
        NSLog(@"%@", error);
        return nil;
    }
    
    return json;
}

@end
