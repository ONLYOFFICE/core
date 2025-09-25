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
#include "FormatUtils.h"

namespace DocFileFormat
{
#define WriteWCharPadding(STLCollection, padding) \
for (int i = 0; i < padding; ++i)\
{\
STLCollection->push_back((wchar_t)0);\
}
	static void WriteUtf16ToWChar(int code, std::vector<wchar_t>* STLCollection, int padding)
	{
		int used = 0;
		if (code < 0x10000)
		{
			STLCollection->push_back((wchar_t)code);
			used = 1;
		}
		else
		{
			code -= 0x10000;
			STLCollection->push_back((wchar_t)(0xD800 | ((code >> 10) & 0x03FF)));
			STLCollection->push_back((wchar_t)(0xDC00 | (code & 0x03FF)));
			used = 2;
		}
		WriteWCharPadding(STLCollection, padding - used);

	}

	static bool GetSTLCollectionFromUtf8(std::vector<wchar_t>* STLCollection, unsigned char* pBuffer, int lCount)
	{
		if ((STLCollection == NULL) || (pBuffer == NULL))
		{
			return false;
		}
		int lIndex = 0;
		if (sizeof(wchar_t) == 2)//utf8 -> utf16
		{
			while (lIndex < lCount)
			{
				BYTE byteMain = pBuffer[lIndex];
				if (0x00 == (byteMain & 0x80))
				{
					// 1 byte
					STLCollection->push_back((WCHAR)byteMain);
					++lIndex;
				}
				else if (0x00 == (byteMain & 0x20))
				{
					// 2 byte
					int val = 0;
					if ((lIndex + 1) < lCount)
					{
						val = (int)(((byteMain & 0x1F) << 6) |
							(pBuffer[lIndex + 1] & 0x3F));
					}
					STLCollection->push_back((wchar_t)val);
					STLCollection->push_back((wchar_t)0);
					lIndex += 2;
				}
				else if (0x00 == (byteMain & 0x10))
				{
					// 3 byte
					int val = 0;
					if ((lIndex + 2) < lCount)
					{
						val = (int)(((byteMain & 0x0F) << 12) |
							((pBuffer[lIndex + 1] & 0x3F) << 6) |
							(pBuffer[lIndex + 2] & 0x3F));
					}
					WriteUtf16ToWChar(val, STLCollection, 3);
					lIndex += 3;
				}
				else if (0x00 == (byteMain & 0x0F))
				{
					// 4 byte
					int val = 0;
					if ((lIndex + 3) < lCount)
					{
						val = (int)(((byteMain & 0x07) << 18) |
							((pBuffer[lIndex + 1] & 0x3F) << 12) |
							((pBuffer[lIndex + 2] & 0x3F) << 6) |
							(pBuffer[lIndex + 3] & 0x3F));
					}
					WriteUtf16ToWChar(val, STLCollection, 4);
					lIndex += 4;
				}
				else if (0x00 == (byteMain & 0x08))
				{
					// 4 byte
					int val = 0;
					if ((lIndex + 3) < lCount)
					{
						val = (int)(((byteMain & 0x07) << 18) |
							((pBuffer[lIndex + 1] & 0x3F) << 12) |
							((pBuffer[lIndex + 2] & 0x3F) << 6) |
							(pBuffer[lIndex + 3] & 0x3F));
					}
					WriteUtf16ToWChar(val, STLCollection, 4);
					lIndex += 4;
				}
				else if (0x00 == (byteMain & 0x04))
				{
					// 5 byte
					int val = 0;
					if ((lIndex + 4) < lCount)
					{
						val = (int)(((byteMain & 0x03) << 24) |
							((pBuffer[lIndex + 1] & 0x3F) << 18) |
							((pBuffer[lIndex + 2] & 0x3F) << 12) |
							((pBuffer[lIndex + 3] & 0x3F) << 6) |
							(pBuffer[lIndex + 4] & 0x3F));
					}
					WriteUtf16ToWChar(val, STLCollection, 5);
					lIndex += 5;
				}
				else
				{
					// 6 byte
					int val = 0;
					if ((lIndex + 5) < lCount)
					{
						val = (int)(((byteMain & 0x01) << 30) |
							((pBuffer[lIndex + 1] & 0x3F) << 24) |
							((pBuffer[lIndex + 2] & 0x3F) << 18) |
							((pBuffer[lIndex + 3] & 0x3F) << 12) |
							((pBuffer[lIndex + 4] & 0x3F) << 6) |
							(pBuffer[lIndex + 5] & 0x3F));
					}
					WriteUtf16ToWChar(val, STLCollection, 6);
					lIndex += 6;
				}
			}
		}
		else //utf8 -> utf32
		{
			while (lIndex < lCount)
			{
				BYTE byteMain = pBuffer[lIndex];
				if (0x00 == (byteMain & 0x80))
				{
					// 1 byte
					STLCollection->push_back((WCHAR)byteMain);
					++lIndex;
				}
				else if (0x00 == (byteMain & 0x20))
				{
					// 2 byte
					int val = 0;
					if ((lIndex + 1) < lCount)
					{
						val = (int)(((byteMain & 0x1F) << 6) |
							(pBuffer[lIndex + 1] & 0x3F));
					}

					STLCollection->push_back((WCHAR)val);
					STLCollection->push_back((WCHAR)0);
					lIndex += 2;
				}
				else if (0x00 == (byteMain & 0x10))
				{
					// 3 byte
					int val = 0;
					if ((lIndex + 2) < lCount)
					{
						val = (int)(((byteMain & 0x0F) << 12) |
							((pBuffer[lIndex + 1] & 0x3F) << 6) |
							(pBuffer[lIndex + 2] & 0x3F));
					}
					STLCollection->push_back((WCHAR)val);
					WriteWCharPadding(STLCollection, 2);
					lIndex += 3;
				}
				else if (0x00 == (byteMain & 0x0F))
				{
					// 4 byte
					int val = 0;
					if ((lIndex + 3) < lCount)
					{
						val = (int)(((byteMain & 0x07) << 18) |
							((pBuffer[lIndex + 1] & 0x3F) << 12) |
							((pBuffer[lIndex + 2] & 0x3F) << 6) |
							(pBuffer[lIndex + 3] & 0x3F));
					}

					STLCollection->push_back((WCHAR)val);
					WriteWCharPadding(STLCollection, 3);
					lIndex += 4;
				}
				else if (0x00 == (byteMain & 0x08))
				{
					// 4 byte
					int val = 0;
					if ((lIndex + 3) < lCount)
					{
						val = (int)(((byteMain & 0x07) << 18) |
							((pBuffer[lIndex + 1] & 0x3F) << 12) |
							((pBuffer[lIndex + 2] & 0x3F) << 6) |
							(pBuffer[lIndex + 3] & 0x3F));
					}
					STLCollection->push_back((WCHAR)val);
					WriteWCharPadding(STLCollection, 3);
					lIndex += 4;
				}
				else if (0x00 == (byteMain & 0x04))
				{
					// 5 byte
					int val = 0;
					if ((lIndex + 4) < lCount)
					{
						val = (int)(((byteMain & 0x03) << 24) |
							((pBuffer[lIndex + 1] & 0x3F) << 18) |
							((pBuffer[lIndex + 2] & 0x3F) << 12) |
							((pBuffer[lIndex + 3] & 0x3F) << 6) |
							(pBuffer[lIndex + 4] & 0x3F));
					}
					STLCollection->push_back((WCHAR)val);
					WriteWCharPadding(STLCollection, 4);
					lIndex += 5;
				}
				else
				{
					// 6 byte
					int val = 0;
					if ((lIndex + 5) < lCount)
					{
						val = (int)(((byteMain & 0x01) << 30) |
							((pBuffer[lIndex + 1] & 0x3F) << 24) |
							((pBuffer[lIndex + 2] & 0x3F) << 18) |
							((pBuffer[lIndex + 3] & 0x3F) << 12) |
							((pBuffer[lIndex + 4] & 0x3F) << 6) |
							(pBuffer[lIndex + 5] & 0x3F));
					}
					STLCollection->push_back((WCHAR)val);
					WriteWCharPadding(STLCollection, 5);
					lIndex += 6;
				}
			}
		}
		return true;
	}
}

bool DocFileFormat::FormatUtils::GetWStringFromBytes(std::wstring& string, unsigned char* bytes, int size, int code_page)
{
	if (bytes == NULL)
	{
		return false;
	}

	if (code_page == ENCODING_UTF8)
	{
		string = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(bytes, size);
		return true;
	}
	else if (code_page == ENCODING_UTF16)
	{
		string = NSFile::CUtf8Converter::GetWStringFromUTF16((unsigned short*)bytes, size / 2);
	}
	else if (code_page == ENCODING_WINDOWS_1250)
	{
		wchar_t wch = 0;
		int i = 0;
		while (i < size)
		{
			wch = MapByteToWChar(bytes[i++]);

			string += (wch);
		}
	}
	else
	{
		std::string sCodePage;
		std::map<int, std::string>::const_iterator pFind = NSUnicodeConverter::mapEncodingsICU.find(code_page);
		if (pFind != NSUnicodeConverter::mapEncodingsICU.end())
		{
			sCodePage = pFind->second;
		}

		if (sCodePage.empty())
			sCodePage = "CP1250"/* + std::to_string(code_page)*/;

		NSUnicodeConverter::CUnicodeConverter oConverter;
		string = oConverter.toUnicode((char*)bytes, (unsigned int)size, sCodePage.c_str());
	}

	return true;
}

bool DocFileFormat::FormatUtils::GetSTLCollectionFromBytes(std::vector<wchar_t>* STLCollection, unsigned char* bytes, int size, int code_page)
{
	if (bytes == NULL)
	{
		return false;
	}

	if (code_page == ENCODING_UTF8)
	{
		return GetSTLCollectionFromUtf8(STLCollection, bytes, size);
	}
	else if (code_page == ENCODING_UTF16)
	{
		int i = 0;
#if !defined(_WIN32) && !defined(_WIN64)
		int nCount = size / 2;
		unsigned short* pShort = (unsigned short*)bytes;

		int nCurrent = 0;
		while (nCurrent < nCount)
		{
			if (*pShort < 0xD800 || *pShort > 0xDBFF)
			{
				STLCollection->push_back((wchar_t)(*pShort));
				++pShort;
				++nCurrent;
			}
			else
			{
				STLCollection->push_back((wchar_t)(((((pShort[0] - 0xD800) & 0x03FF) << 10) | ((pShort[1] - 0xDC00) & 0x03FF)) + 0x10000));
				STLCollection->push_back((wchar_t)0);
				pShort += 2;
				nCurrent += 2;
			}
		}
#else
		while (i < size)
		{
			STLCollection->push_back(FormatUtils::BytesToUInt16(bytes, i, size));

			i += 2;
		}
#endif
	}
	else if (code_page == ENCODING_WINDOWS_1250)
	{
		wchar_t wch = 0;
		int i = 0;
		while (i < size)
		{
			wch = MapByteToWChar(bytes[i++]);

			STLCollection->push_back(wch);
		}
	}
	else
	{
		std::string sCodePage;
		std::map<int, std::string>::const_iterator pFind = NSUnicodeConverter::mapEncodingsICU.find(code_page);
		if (pFind != NSUnicodeConverter::mapEncodingsICU.end())
		{
			sCodePage = pFind->second;
		}

		if (sCodePage.empty())
			sCodePage = "CP1250"/* + std::to_string(code_page)*/;

		NSUnicodeConverter::CUnicodeConverter oConverter;
		std::wstring unicode_string = oConverter.toUnicode((char*)bytes, (unsigned int)size, sCodePage.c_str());

		for (size_t i = 0; i < unicode_string.size(); i++)
		{
			STLCollection->push_back(unicode_string[i]);
		}
	}

	return true;
}
