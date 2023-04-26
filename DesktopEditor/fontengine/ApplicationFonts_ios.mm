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
#include "ApplicationFonts.h"
#include "../common/Directory.h"

#ifdef BOOL
#undef BOOL
#endif

#import <UIKit/UIKit.h>

std::vector<std::wstring> CApplicationFonts::GetSetupFontFiles_ios()
{
    std::vector<std::wstring> oArray;
    NSDirectory::GetFiles2(L"/System/Library/Fonts", oArray, true);
    
    if (oArray.size() == 0)
    {
        NSDirectory::GetFiles2(L"/Library/Fonts", oArray, true);
    }
    
    return oArray;
}

namespace NSDirectory
{
    void GetFiles2_ios(std::wstring strDirectory, std::vector<std::wstring>& oArray, bool bIsRecursion)
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
            oArray.push_back(sTmp);
        }
    }
}
