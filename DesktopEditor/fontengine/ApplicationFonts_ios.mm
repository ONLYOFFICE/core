#include "ApplicationFonts.h"
#include "../common/Directory.h"

#ifdef BOOL
#undef BOOL
#endif

#ifdef _IOS

#import <UIKit/UIKit.h>

CArray<std::wstring> CApplicationFonts::GetSetupFontFiles_ios()
{
    CArray<std::wstring> oArray;
    NSDirectory::GetFiles2(L"/System/Library/Fonts", oArray, true);
    
    if (oArray.GetCount() == 0)
    {
        NSDirectory::GetFiles2(L"/Library/Fonts", oArray, true);
    }
    
    return oArray;
}

namespace NSDirectory
{
    void GetFiles2_ios(std::wstring strDirectory, CArray<std::wstring>& oArray, bool bIsRecursion)
    {
        NSString* directoryPath = [ [ NSString alloc ]
                             initWithBytes : (char*)strDirectory.data()
                             length : strDirectory.size() * sizeof(wchar_t)
                             encoding : CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE ) ];
        
        int count;
        
        NSArray* directoryContent = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:directoryPath error:NULL];
        for (count = 0; count < (int)[directoryContent count]; count++)
        {
            NSStringEncoding pEncode    =   CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE );
            NSData* pSData              =   [[ directoryContent objectAtIndex:count] dataUsingEncoding : pEncode ];
            
            std::wstring sTmp( (wchar_t*) [ pSData bytes ], [ pSData length] / sizeof ( wchar_t ) );
            oArray.Add(sTmp);
        }
    }
}

#endif
