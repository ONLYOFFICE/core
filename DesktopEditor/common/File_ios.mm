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
