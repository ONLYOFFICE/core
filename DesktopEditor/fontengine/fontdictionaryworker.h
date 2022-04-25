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
#ifndef _FONT_DICTIONARY_WORKER_H

#include "../freetype_names/FontMaps/FontDictionary.h"
#include "../common/File.h"

namespace NSFontDictionary
{
	static bool CorrectParamsFromDictionary(NSFonts::CFontSelectFormat& oFormat)
	{
		if (NULL == oFormat.wsName)
			return false;

		int nNameLen = (int)oFormat.wsName->length();
		if (nNameLen == 0)
			return false;

		bool bIsAscii = true;
		const wchar_t* pName = oFormat.wsName->c_str();

		for (int i = 0; i < nNameLen; ++i)
		{
			if (pName[i] > 255 || pName[i] < 0)
			{
				bIsAscii = false;
				break;
			}
		}

		const FD_Font* pFont = NULL;

		if (bIsAscii)
		{
			int nStartIndex = FD_Ascii_Names_Offsets[pName[0]];

			if (-1 != nStartIndex)
			{
				int nIndex = -1;
				for (int i = nStartIndex; i < FONTS_DICT_ASCII_NAMES_COUNT; ++i)
				{
					const char* _name = FD_Ascii_Names[i].m_name;
					if (pName[0] != (wchar_t)_name[0])
						break;

					bool bIsEqual = true;
					for (int j = 1; j < nNameLen; ++j)
					{
                        if ('\0' == _name[j]) // короче чем нужно
                        {
                            bIsEqual = false;
                            break;
                        }
                        if (pName[j] != (wchar_t)_name[j])
						{
							bIsEqual = false;
							break;
						}
					}

                    if (bIsEqual && _name[nNameLen] == '\0') // чтобы не длиннее, чем нужно
					{
						nIndex = i;
						break;
					}
				}

				if (nIndex != -1)
				{
					const FD_FontMapRec* pRec = &FD_Ascii_Names[nIndex];
					int nFontIndex = -1;

					int nStyle = 0;
					if (oFormat.bItalic != NULL && *oFormat.bItalic == TRUE)
						nStyle |= 1;
					if (oFormat.bBold != NULL && *oFormat.bBold == TRUE)
						nStyle |= 2;

					switch (nStyle)
					{
					case 1:
						{
							if (pRec->m_index_i != -1)
								nFontIndex = pRec->m_index_i;
							else if (pRec->m_index_bi != -1)
								nFontIndex = pRec->m_index_bi;
							else if (pRec->m_index_r != -1)
								nFontIndex = pRec->m_index_r;
							else
								nFontIndex = pRec->m_index_b;
							break;
						}
					case 2:
						{
							if (pRec->m_index_b != -1)
								nFontIndex = pRec->m_index_b;
							else if (pRec->m_index_bi != -1)
								nFontIndex = pRec->m_index_bi;
							else if (pRec->m_index_r != -1)
								nFontIndex = pRec->m_index_r;
							else
								nFontIndex = pRec->m_index_i;
							break;
						}
					case 3:
						{
							if (pRec->m_index_bi != -1)
								nFontIndex = pRec->m_index_bi;
							else if (pRec->m_index_b != -1)
								nFontIndex = pRec->m_index_b;
							else if (pRec->m_index_i != -1)
								nFontIndex = pRec->m_index_i;
							else
								nFontIndex = pRec->m_index_r;
							break;
						}
					case 0:
					default:
						{
							if (pRec->m_index_r != -1)
								nFontIndex = pRec->m_index_r;
							else if (pRec->m_index_i != -1)
								nFontIndex = pRec->m_index_i;
							else if (pRec->m_index_b != -1)
								nFontIndex = pRec->m_index_b;
							else
								nFontIndex = pRec->m_index_bi;
							break;
						}
					}

					if (nFontIndex != -1)
						pFont = &FD_Ascii_Files[nFontIndex];
				}
			}
		}
		else
		{
			int nIndex = -1;
			for (int i = 0; i < FONTS_DICT_UNICODE_NAMES_COUNT; ++i)
			{
				const wchar_t* _name = FD_Unicode_Names[i].m_name;
				if (pName[0] != _name[0])
					continue;

				bool bIsEqual = true;
				for (int j = 1; j < nNameLen; ++j)
				{
                    if ('\0' == _name[j]) // короче чем нужно
					{
						bIsEqual = false;
						break;
					}
					if (pName[j] != _name[j])
					{
						bIsEqual = false;
						break;
					}
				}

                if (bIsEqual && _name[nNameLen] == '\0') // чтобы не длиннее, чем нужно
				{
					nIndex = i;
					break;
				}
			}

			if (nIndex != -1)
			{
				const FD_FontMapRecW* pRec = &FD_Unicode_Names[nIndex];
				int nFontIndex = -1;

				int nStyle = 0;
				if (oFormat.bItalic != NULL && *oFormat.bItalic == TRUE)
					nStyle |= 1;
				if (oFormat.bBold != NULL && *oFormat.bBold == TRUE)
					nStyle |= 2;

				switch (nStyle)
				{
				case 1:
					{
						if (pRec->m_index_i != -1)
							nFontIndex = pRec->m_index_i;
						else if (pRec->m_index_bi != -1)
							nFontIndex = pRec->m_index_bi;
						else if (pRec->m_index_r != -1)
							nFontIndex = pRec->m_index_r;
						else
							nFontIndex = pRec->m_index_b;
						break;
					}
				case 2:
					{
						if (pRec->m_index_b != -1)
							nFontIndex = pRec->m_index_b;
						else if (pRec->m_index_bi != -1)
							nFontIndex = pRec->m_index_bi;
						else if (pRec->m_index_r != -1)
							nFontIndex = pRec->m_index_r;
						else
							nFontIndex = pRec->m_index_i;
						break;
					}
				case 3:
					{
						if (pRec->m_index_bi != -1)
							nFontIndex = pRec->m_index_bi;
						else if (pRec->m_index_b != -1)
							nFontIndex = pRec->m_index_b;
						else if (pRec->m_index_i != -1)
							nFontIndex = pRec->m_index_i;
						else
							nFontIndex = pRec->m_index_r;
						break;
					}
				case 0:
				default:
					{
						if (pRec->m_index_r != -1)
							nFontIndex = pRec->m_index_r;
						else if (pRec->m_index_i != -1)
							nFontIndex = pRec->m_index_i;
						else if (pRec->m_index_b != -1)
							nFontIndex = pRec->m_index_b;
						else
							nFontIndex = pRec->m_index_bi;
						break;
					}
				}

				if (nFontIndex != -1)
					pFont = &FD_Ascii_Files[nFontIndex];
			}
		}

		if (NULL == pFont)
			return false;

		// name - делаем ascii, чтобы сработал подбор
		oFormat.Destroy(true);

		LONG nLen = (LONG)strlen(pFont->m_name);

		oFormat.wsName = new std::wstring(NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pFont->m_name, nLen));

		// fixed
		oFormat.bFixedWidth = new INT(pFont->m_bIsFixed == 1 ? TRUE : FALSE);

		// panose
		oFormat.pPanose = new BYTE[10];
		memcpy(oFormat.pPanose, pFont->m_aPanose, 10);

		// ranges
		oFormat.ulRange1 = new UINT(pFont->m_ulUnicodeRange1);
		oFormat.ulRange2 = new UINT(pFont->m_ulUnicodeRange2);
		oFormat.ulRange3 = new UINT(pFont->m_ulUnicodeRange3);
		oFormat.ulRange4 = new UINT(pFont->m_ulUnicodeRange4);
		oFormat.ulCodeRange1 = new UINT(pFont->m_ulCodePageRange1);
		oFormat.ulCodeRange2 = new UINT(pFont->m_ulCodePageRange2);
		
		oFormat.usWeight = new USHORT(pFont->m_usWeigth);
		oFormat.usWidth = new USHORT(pFont->m_usWidth);
		
		oFormat.shAvgCharWidth = new SHORT(pFont->m_shAvgCharWidth);
		oFormat.shAscent = new SHORT(pFont->m_shAscent);
		oFormat.shDescent = new SHORT(pFont->m_shDescent);
		oFormat.shXHeight = new SHORT(pFont->m_shXHeight);
		oFormat.shCapHeight = new SHORT(pFont->m_shCapHeight);

		return true;
	}
}

#endif /* _FONT_DICTIONARY_WORKER_H */
