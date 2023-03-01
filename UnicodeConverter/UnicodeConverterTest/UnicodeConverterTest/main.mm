/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
//
//  main.m
//  UnicodeConverterTest
//
//  Created by Oleg Korshul on 23.11.15.
//  Copyright © 2015 Ascensio System. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "../../UnicodeConverter.h"

namespace NSCommon
{
    static std::wstring NSStringToStringW ( NSString* Str )
    {
        NSStringEncoding pEncode    =   CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE );
        NSData* pSData              =   [ Str dataUsingEncoding : pEncode ];
        
        return std::wstring ( (wchar_t*) [ pSData bytes ], [ pSData length] / sizeof ( wchar_t ) );
    }
    
    static std::string NSStringToStringA ( NSString* Str )
    {
        NSStringEncoding pEncode    =   CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingISOLatin1 );
        NSData* pSData              =   [ Str dataUsingEncoding : pEncode ];
        
        return std::string ( (char*) [ pSData bytes ], [ pSData length] );
    }
    
    static NSString* StringWToNSString ( const std::wstring& Str )
    {
        NSString* pString = [ [ NSString alloc ]
                             initWithBytes : (char*)Str.data()
                             length : Str.size() * sizeof(wchar_t)
                             encoding : CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE ) ];
        return pString;
    }
    
    static NSString* StringAToNSString ( const std::string& Str )
    {
        NSString* pString = [ [ NSString alloc ]
                             initWithBytes : (char*)Str.data()
                             length : Str.size() * sizeof(char)
                             encoding : CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingISOLatin1 ) ];
        return pString;
    }
}

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        NSUnicodeConverter::CUnicodeConverter oConverter;
        
        std::string str1 = oConverter.fromUnicode(L"рус", "windows-1251");
        
        NSLog(NSCommon::StringAToNSString(str1));
        
        std::wstring str2 = oConverter.toUnicode(str1, "windows-1251");
        NSLog(NSCommon::StringWToNSString(str2));
        
        std::string str3 = oConverter.fromUnicode(str2, "windows-1251");
        
        NSLog(NSCommon::StringAToNSString(str3));
    }
    return 0;
}
