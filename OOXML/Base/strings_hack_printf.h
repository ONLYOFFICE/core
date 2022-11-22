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
#ifndef STRINGS_HACK_PRINTF_H
#define STRINGS_HACK_PRINTF_H

#include <string>
#include <string.h>
#include <wchar.h>
#include <stdarg.h>

#ifndef STD_BUF_SIZE
#define STD_BUF_SIZE 1024
#endif

static int strings_hack_printf_internal(wchar_t* _buffer, size_t _size_alloc, wchar_t* _format, va_list va)
{
	size_t write_size = 0;

	wchar_t* tmp_format = _format;
	wchar_t* tmp_buffer = _buffer;
	do
	{
		wchar_t* buffer_cur = tmp_format;
		int nSkip = 0;
		while (*buffer_cur)
		{
			if (buffer_cur[0] == '%')
			{
				if (buffer_cur[1] == 's') // not crash (last symbol - 0)
				{
					nSkip = 2;
					break;
				}
				else if (buffer_cur[1] == 'l' && buffer_cur[2] == 's') // not crash (last symbol - 0)
				{
					nSkip = 3;
					break;
				}
			}
			++buffer_cur;
		}

		if (nSkip > 0)
		{
			buffer_cur[0] = '\0';
		}

		if (write_size >= _size_alloc)
			return -1;

		if (*tmp_format)
		{
#ifndef _IOS
			// crash on ios.
			int fmt_size = vswprintf(tmp_buffer, _size_alloc - write_size, tmp_format, va);
#else
			va_list arg_copy;
			va_copy(arg_copy, va);

			for (wchar_t* tmp = tmp_format; *tmp != '\0'; ++tmp)
			{
				if ('%' == *tmp)
				{
					if ('%' == tmp[1])
						++tmp;
					else
						void* p = va_arg(va, void*);
				}
			}

			int fmt_size = vswprintf(tmp_buffer, _size_alloc - write_size, tmp_format, arg_copy);
#endif
			if (fmt_size < 0)
				return -1;

			tmp_buffer += fmt_size;
			write_size += fmt_size;
		}

		if (nSkip)
		{
			wchar_t* _string_param = va_arg(va, wchar_t*);
			size_t _len = wcslen(_string_param);

			if (_size_alloc > (_len + write_size))
				memcpy(tmp_buffer, _string_param, _len * sizeof(wchar_t));

			write_size += _len;
			tmp_buffer += _len;
		}

		tmp_format = buffer_cur + nSkip;
	} while (*tmp_format);

	if (write_size >= _size_alloc)
		return -1;

	tmp_buffer[0] = '\0';
	return (int)write_size;
}

static int strings_hack_printf_internal_a(char* _buffer, size_t _size_alloc, char* _format, va_list va)
{
	size_t write_size = 0;

	char* tmp_format = _format;
	char* tmp_buffer = _buffer;
	do
	{
		char* buffer_cur = tmp_format;
		int nSkip = 0;
		while (*buffer_cur)
		{
			if (buffer_cur[0] == '%')
			{
				if (buffer_cur[1] == 's') // not crash (last symbol - 0)
				{
					nSkip = 2;
					break;
				}
				else if (buffer_cur[1] == 'l' && buffer_cur[2] == 's') // not crash (last symbol - 0)
				{
					nSkip = 3;
					break;
				}
			}
			++buffer_cur;
		}

		if (nSkip > 0)
		{
			buffer_cur[0] = '\0';
		}

		if (write_size >= _size_alloc)
			return -1;

		if (*tmp_format)
		{
#ifndef _IOS
			// crash on ios.
			int fmt_size = vsnprintf(tmp_buffer, _size_alloc - write_size, tmp_format, va);
#else
			va_list arg_copy;
			va_copy(arg_copy, va);

			for (char* tmp = tmp_format; *tmp != '\0'; ++tmp)
			{
				if ('%' == *tmp)
				{
					if ('%' == tmp[1])
						++tmp;
					else
						void* p = va_arg(va, void*);
				}
			}

			int fmt_size = vsnprintf(tmp_buffer, _size_alloc - write_size, tmp_format, arg_copy);
#endif
			if (fmt_size < 0)
				return -1;

			tmp_buffer += fmt_size;
			write_size += fmt_size;
		}

		if (nSkip)
		{
			char* _string_param = va_arg(va, char*);
			size_t _len = strlen(_string_param);

			if (_size_alloc > (_len + write_size))
				memcpy(tmp_buffer, _string_param, _len * sizeof(char));

			write_size += _len;
			tmp_buffer += _len;
		}

		tmp_format = buffer_cur + nSkip;
	} while (*tmp_format);

	if (write_size >= _size_alloc)
		return -1;

	tmp_buffer[0] = '\0';
	return (int)write_size;
}

template <typename T>
static int strings_hack_printf(T& str, const wchar_t *format, va_list argptr)
{
	int buf_size = STD_BUF_SIZE;

	size_t nFormatLen = wcslen(format);
	if (0 == nFormatLen)
		return -1;

	wchar_t* tmp_format = new wchar_t[nFormatLen + 1];
	memcpy(tmp_format, format, nFormatLen * sizeof(wchar_t));
	tmp_format[nFormatLen] = '\0';

	while (buf_size < STD_BUF_SIZE * STD_BUF_SIZE)
	{
		va_list args;
		va_copy(args, argptr);
		wchar_t buffer[buf_size];

		int fmt_size = strings_hack_printf_internal(buffer, sizeof(buffer)/sizeof(buffer[0]), tmp_format, args);
		if (fmt_size >= 0)
		{
			str = T(buffer);
			return fmt_size;
		}

		buf_size *= 2;
	}

	delete [] tmp_format;

	return -1;
}
template <typename T>
static int strings_hack_printf(T& str, const char* format, va_list argptr)
{
	int buf_size = STD_BUF_SIZE;

	size_t nFormatLen = strlen(format);
	if (0 == nFormatLen)
		return -1;

	char* tmp_format = new char[nFormatLen + 1];
	memcpy(tmp_format, format, nFormatLen * sizeof(char));
	tmp_format[nFormatLen] = '\0';

	while (buf_size < STD_BUF_SIZE * STD_BUF_SIZE)
	{
		va_list args;
		va_copy(args, argptr);
		char buffer[buf_size];

		int fmt_size = strings_hack_printf_internal_a(buffer, sizeof(buffer)/sizeof(buffer[0]), tmp_format, args);
		if (fmt_size >= 0)
		{
			str = T(buffer);
			return fmt_size;
		}

		buf_size *= 2;
	}

	delete [] tmp_format;

	return -1;
}

template<typename T, typename CHAR_TYPE>
static void strings_hack_printf_exec(T& sRes, const CHAR_TYPE* szFmt, ...)
{
	va_list argList;

	va_start(argList, szFmt);
	strings_hack_printf(sRes, szFmt, argList);
	va_end(argList);
}

#endif // STRINGS_HACK_PRINTF_H
