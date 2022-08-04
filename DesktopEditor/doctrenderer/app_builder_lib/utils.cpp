/*
 * (c) Copyright Ascensio System SIA 2010-2019
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

#include <string>

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
#include <wchar.h>
#include <windows.h>
#endif

#ifdef _LINUX
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#endif

#ifdef _MAC
#include <mach-o/dyld.h>
#endif

#ifdef CreateFile
#undef CreateFile
#endif

namespace NSUtils
{
#ifdef _WIN32
	#define FILE_SEPARATOR '\\'
	#define WriteCodepoint(code)							\
	if (code < 0x10000)										\
		*unicodes_cur++ = code;								\
	else													\
	{														\
		code -= 0x10000;									\
		*unicodes_cur++ = 0xD800 | ((code >> 10) & 0x03FF);	\
		*unicodes_cur++ = 0xDC00 | (code & 0x03FF);			\
	}
#else
	#define FILE_SEPARATOR '/'
	#define WriteCodepoint(code) *unicodes_cur++ = (wchar_t)code;
#endif

	std::wstring GetStringFromUtf8(const unsigned char* utf8, size_t length)
	{
		wchar_t* unicodes = new wchar_t[length + 1];
		wchar_t* unicodes_cur = unicodes;
		size_t index = 0;

		while (index < length)
		{
			unsigned char byteMain = utf8[index];
			if (0x00 == (byteMain & 0x80))
			{
				// 1 byte
				WriteCodepoint(byteMain)
				++index;
			}
			else if (0x00 == (byteMain & 0x20))
			{
				// 2 byte
				int val = 0;
				if ((index + 1) < length)
				{
					val = (int)(((byteMain & 0x1F) << 6) |
								(utf8[index + 1] & 0x3F));
				}

				WriteCodepoint(val)
				index += 2;
			}
			else if (0x00 == (byteMain & 0x10))
			{
				// 3 byte
				int val = 0;
				if ((index + 2) < length)
				{
					val = (int)(((byteMain & 0x0F) << 12) |
								((utf8[index + 1] & 0x3F) << 6) |
								(utf8[index + 2] & 0x3F));
				}

				WriteCodepoint(val)
				index += 3;
			}
			else if (0x00 == (byteMain & 0x0F))
			{
				// 4 byte
				int val = 0;
				if ((index + 3) < length)
				{
					val = (int)(((byteMain & 0x07) << 18) |
								((utf8[index + 1] & 0x3F) << 12) |
								((utf8[index + 2] & 0x3F) << 6) |
								(utf8[index + 3] & 0x3F));
				}

				WriteCodepoint(val)
				index += 4;
			}
			else if (0x00 == (byteMain & 0x08))
			{
				// 4 byte
				int val = 0;
				if ((index + 3) < length)
				{
					val = (int)(((byteMain & 0x07) << 18) |
								((utf8[index + 1] & 0x3F) << 12) |
								((utf8[index + 2] & 0x3F) << 6) |
								(utf8[index + 3] & 0x3F));
				}

				WriteCodepoint(val)
				index += 4;
			}
			else if (0x00 == (byteMain & 0x04))
			{
				// 5 byte
				int val = 0;
				if ((index + 4) < length)
				{
					val = (int)(((byteMain & 0x03) << 24) |
								((utf8[index + 1] & 0x3F) << 18) |
								((utf8[index + 2] & 0x3F) << 12) |
								((utf8[index + 3] & 0x3F) << 6) |
								(utf8[index + 4] & 0x3F));
				}

				WriteCodepoint(val)
				index += 5;
			}
			else
			{
				// 6 byte
				int val = 0;
				if ((index + 5) < length)
				{
					val = (int)(((byteMain & 0x01) << 30) |
								((utf8[index + 1] & 0x3F) << 24) |
								((utf8[index + 2] & 0x3F) << 18) |
								((utf8[index + 3] & 0x3F) << 12) |
								((utf8[index + 4] & 0x3F) << 6) |
								(utf8[index + 5] & 0x3F));
				}

				WriteCodepoint(val)
				index += 5;
			}
		}

		*unicodes_cur++ = 0;

		std::wstring sOutput(unicodes);

		delete [] unicodes;

		return sOutput;
	}
}

namespace NSUtils
{
	#define NS_FILE_MAX_PATH 32768
	std::wstring GetProcessPath()
	{
#if defined (_WIN64) || defined(_WIN32)
		wchar_t buf [NS_FILE_MAX_PATH];
		GetModuleFileNameW(GetModuleHandle(NULL), buf, NS_FILE_MAX_PATH);
		return std::wstring(buf);
#endif

#if defined(_LINUX) || defined(_MAC)
		char buf[NS_FILE_MAX_PATH];
		memset(buf, 0, NS_FILE_MAX_PATH);
		if (readlink ("/proc/self/exe", buf, NS_FILE_MAX_PATH) <= 0)
		{
#ifdef _MAC
			uint32_t _size = NS_FILE_MAX_PATH;
			_NSGetExecutablePath(buf, &_size);
#endif
		}
		return GetStringFromUtf8((unsigned char*)buf, strlen(buf));
#endif
		return L"";
	}

	std::wstring GetProcessDirectory()
	{
		std::wstring path = GetProcessPath();
		size_t pos = path.find_last_of(FILE_SEPARATOR);
		if (pos != std::wstring::npos)
			path = path.substr(0, pos);
		return path;
	}

	std::wstring GetBuilderDirectory()
	{
		#define VALUE_TO_STRING(x) L###x
		#define VALUE2STR(x) VALUE_TO_STRING(x)

		std::wstring sDirectory = VALUE2STR(DOCUMENT_BUILDER_INSTALL_PATH);
		return sDirectory;
	}
}
