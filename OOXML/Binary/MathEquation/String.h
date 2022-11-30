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
#pragma once

#include <string>

namespace MathEquation
{
#define STRING_NOTFOUND std::string::npos

	class String 
	{
	public:

		String() {}
		String(const char* val) : Value(val){}
		String(std::string val) : Value(val){}

		String &operator=(String &other)
		{
			Value = other.Value;
			return (*this);
		}

		String &operator+=(const String &other)
		{
			Value += other.Value;
			return (*this);
		}

		String &operator+=(const char* sChars)
		{
			Value += std::string(sChars);
			return (*this);
		}

		String &operator+=(const char unChar)
		{
			char oChars[2];
			oChars[0] = unChar; oChars[1] = 0x00;
			Value += std::string((char*)&oChars);
			return (*this);
		}

		void Append(const char* str)
		{
			Value.append(str);
		}

		void Append(String str)
		{
			Value.append(str.Value);
		}

		void Append(unsigned short ushUnicode)
		{
			char oChars[2];
			oChars[0] = (char)ushUnicode; oChars[1] = 0x00;
			Value += std::string((char*)&oChars);
		}

		void AppendAscii(const char *sChars)
		{
			Value += std::string(sChars);
		}

		void Erase(int nPos = 0, int nLen = std::string::npos)
		{
			Value.erase(nPos, nLen);
		}

		int Len()
		{
			return (int)Value.length();
		}

		BYTE GetChar(int nPos)
		{
			return Value[nPos];
		}

		void InsertAscii(const char* str, int nPos)
		{
			Value.insert(nPos, str);
		}

		void Insert(String str, int nPos)
		{
			Value.insert(nPos, str.Value);
		}

		int SearchBackward(char nChar)
		{
			return (int)Value.rfind(nChar);
		}

		static String CreateFromInt32(int nValue)
		{
			#if defined(_WIN32) || defined (_WIN64)
				char intStr[32];
				_itoa_s(nValue, (char *)&intStr, 32, 10);
				return String(intStr);
			#else
				return std::to_string(nValue);;
			#endif
		}

		std::string GetValue()
		{
			return Value;
		}

	private:

		std::string Value;
	};
}
