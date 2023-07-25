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

#ifdef __OBJC__
#import <CoreFoundation/CoreFoundation.h>
#else
#include <objc/objc.h>
#endif

#include "./File.h"
#include <vector>
#import <Foundation/Foundation.h>

namespace NSFile
{
	namespace NSIOS
	{
		std::string GetFileSystemRepresentation(const std::wstring& path)
		{
			if (path.empty())
				return "";

			NSString* _path = [[NSString alloc] initWithBytes:(char*)path.data() length:path.size() * sizeof(wchar_t)
			  encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE)];

			return std::string((const char*)[_path fileSystemRepresentation]);
		}
	}
}

namespace NSDirectory
{
	namespace NSIOS
	{
		void GetFiles2(std::wstring strDirectory, std::vector<std::wstring>& oArray, bool bIsRecursion)
		{
			NSStringEncoding pEncode = CFStringConvertEncodingToNSStringEncoding ( kCFStringEncodingUTF32LE );

			NSString* directoryPath = [[NSString alloc] initWithBytes : (char*)strDirectory.data()
			  length : strDirectory.size() * sizeof(wchar_t) encoding : pEncode];

			NSArray* directoryContent = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:directoryPath error:NULL];
			for (int count = 0; count < (int)[directoryContent count]; count++)
			{
				NSData* pSData = [[directoryContent objectAtIndex:count] dataUsingEncoding : pEncode];
				oArray.emplace_back(std::wstring((wchar_t*)[pSData bytes], [pSData length] / sizeof (wchar_t)));
			}
		}
	}
}
