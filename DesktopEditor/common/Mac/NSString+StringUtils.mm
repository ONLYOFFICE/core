/*
 * (c) Copyright Ascensio System SIA 2010-2020
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
