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
