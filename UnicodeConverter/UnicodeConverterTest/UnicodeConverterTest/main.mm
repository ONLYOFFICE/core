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
