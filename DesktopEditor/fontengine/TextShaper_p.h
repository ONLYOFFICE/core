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
#pragma once

#include <ft2build.h>
#include "ftsnames.h"
#include FT_OUTLINE_H
#include FT_SIZES_H
#include FT_GLYPH_H
#include FT_TRUETYPE_IDS_H
#include FT_TRUETYPE_TABLES_H
#include FT_XFREE86_H
#include FT_ADVANCES_H
#include FT_LCD_FILTER_H
#include FT_DRIVER_H
#include <freetype/internal/tttypes.h>
#include "ftmodapi.h"

#include FT_SFNT_NAMES_H
#include "internal/tttypes.h"
#include "internal/ftstream.h"

void GetUnicodeFromUTF16BE(char* Data, unsigned int Length, int*& DataU, unsigned int& DataULen)
{
	DataU = 0;
	DataULen = 0;

	if (0 == Length)
		return;

	DataULen = Length + 1;
	DataU = new int[DataULen];
	DataULen = 0;

	unsigned int nCount = Length / 2;
	unsigned char* pData = (unsigned char*)Data;

	unsigned int nCurrent = 0;
	while (nCurrent < nCount)
	{
		unsigned short codeBE = (pData[0] << 8) | pData[1];
		if (codeBE < 0xD800 || codeBE > 0xDBFF)
		{
			DataU[DataULen++] = codeBE;
			pData += 2;
			++nCurrent;
		}
		else
		{
			unsigned short codeBE1 = (pData[0] << 8) | pData[1];
			unsigned short codeBE2 = (pData[2] << 8) | pData[3];

			DataU[DataULen++] = (int)(((((codeBE1 - 0xD800) & 0x03FF) << 10) | ((codeBE2 - 0xDC00) & 0x03FF)) + 0x10000);
			pData += 4;
			nCurrent += 2;
		}
	}

	DataU[DataULen] = '\0';
}

void CheckUnicodeFaceName(FT_Face pFace, int*& UName, unsigned int& ULen)
{
	char* face_name = pFace->family_name;
	bool isBadASCII = false;

	unsigned int face_name_len = 0;
	if (NULL != face_name)
	{
		while ('\0' != face_name[face_name_len])
		{
			if ('?' == face_name[face_name_len])
				isBadASCII = true;
			++face_name_len;
		}
	}

	if (face_name_len > 6 &&
			'<' == face_name[0] && 'u' == face_name[1] && 't' == face_name[2] &&
			'f' == face_name[3] && '8' == face_name[4] && '>' == face_name[5])
	{
		ULen = 0;
		unsigned int index = 6;
		UName = new int[face_name_len - index + 1];

		while (index < face_name_len)
		{
			unsigned char byteMain = face_name[index];
			if (0x00 == (byteMain & 0x80))
			{
				// 1 byte
				UName[ULen++] = (int)byteMain;
				++index;
			}
			else if (0x00 == (byteMain & 0x20))
			{
				// 2 byte
				int val = 0;
				if ((index + 1) < face_name_len)
				{
					val = (int)(((byteMain & 0x1F) << 6) |
								(face_name[index + 1] & 0x3F));
				}

				UName[ULen++] = val;
				index += 2;
			}
			else if (0x00 == (byteMain & 0x10))
			{
				// 3 byte
				int val = 0;
				if ((index + 2) < face_name_len)
				{
					val = (int)(((byteMain & 0x0F) << 12) |
								((face_name[index + 1] & 0x3F) << 6) |
							(face_name[index + 2] & 0x3F));
				}

				UName[ULen++] = val;
				index += 3;
			}
			else if (0x00 == (byteMain & 0x0F))
			{
				// 4 byte
				int val = 0;
				if ((index + 3) < face_name_len)
				{
					val = (int)(((byteMain & 0x07) << 18) |
								((face_name[index + 1] & 0x3F) << 12) |
							((face_name[index + 2] & 0x3F) << 6) |
							(face_name[index + 3] & 0x3F));
				}

				UName[ULen++] = val;
				index += 4;
			}
			else if (0x00 == (byteMain & 0x08))
			{
				// 4 byte
				int val = 0;
				if ((index + 3) < face_name_len)
				{
					val = (int)(((byteMain & 0x07) << 18) |
								((face_name[index + 1] & 0x3F) << 12) |
							((face_name[index + 2] & 0x3F) << 6) |
							(face_name[index + 3] & 0x3F));
				}

				UName[ULen++] = val;
				index += 4;
			}
			else if (0x00 == (byteMain & 0x04))
			{
				// 5 byte
				int val = 0;
				if ((index + 4) < face_name_len)
				{
					val = (int)(((byteMain & 0x03) << 24) |
								((face_name[index + 1] & 0x3F) << 18) |
							((face_name[index + 2] & 0x3F) << 12) |
							((face_name[index + 3] & 0x3F) << 6) |
							(face_name[index + 4] & 0x3F));
				}

				UName[ULen++] = val;
				index += 5;
			}
			else
			{
				// 6 byte
				int val = 0;
				if ((index + 5) < face_name_len)
				{
					val = (int)(((byteMain & 0x01) << 30) |
								((face_name[index + 1] & 0x3F) << 24) |
							((face_name[index + 2] & 0x3F) << 18) |
							((face_name[index + 3] & 0x3F) << 12) |
							((face_name[index + 4] & 0x3F) << 6) |
							(face_name[index + 5] & 0x3F));
				}

				UName[ULen++] = val;
				index += 5;
			}
		}

		UName[ULen] = 0;
		return;
	}

	if (!isBadASCII)
	{
		ULen = face_name_len;
		UName = new int[ULen + 1];

		for (unsigned int i = 0; i < ULen; ++i)
			UName[i] = (int)face_name[i];

		UName[ULen] = 0;
		return;
	}

	if (pFace && FT_IS_SFNT(pFace))
	{
		TT_Face pTTFace = (TT_Face)pFace;

		int nNamesCount = (int)pTTFace->num_names;
		TT_NameRec* pNameRecs = pTTFace->name_table.names;

		for (int nNameIndex = 0; nNameIndex < nNamesCount; ++nNameIndex)
		{
			TT_NameRec* rec = pNameRecs + nNameIndex;

			if (rec->nameID != TT_NAME_ID_FONT_FAMILY || rec->stringLength <= 0)
				continue;

			const char* sEncoding = 0;
			switch (rec->platformID)
			{
			case TT_PLATFORM_APPLE_UNICODE:
			{
				sEncoding = "UTF-16BE";
				break;
			}
			case TT_PLATFORM_MACINTOSH:
			{
				break;
			}
			case TT_PLATFORM_MICROSOFT:
			{
				switch (rec->encodingID)
				{
				case TT_MS_ID_SYMBOL_CS:
				case TT_MS_ID_UNICODE_CS:
					sEncoding = "UTF-16BE";
					break;
				case TT_MS_ID_UCS_4:
					//sEncoding = "UCS4"; // см tt_
					sEncoding = "UTF-16BE";
					break;
					//case TT_MS_ID_SJIS:
					//    sEncoding = "Shift-JIS";
					//    break;
					//case TT_MS_ID_GB2312:
					//    sEncoding = "GB2312";
					//    break;
					//case TT_MS_ID_BIG_5:
					//    sEncoding = "Big5";
					//    break;
				default:
					break;
				}
			}
			default:
				break;
			}

			if (0 != sEncoding)
			{
				FT_Stream stream = pTTFace->name_table.stream;
				FT_Memory memory = pFace->memory;
				FT_Error error = 0;

				if ( FT_QNEW_ARRAY ( rec->string, rec->stringLength ) ||
					 FT_STREAM_SEEK( rec->stringOffset )              ||
					 FT_STREAM_READ( rec->string, rec->stringLength ) )
				{
					FT_FREE( rec->string );
					rec->stringLength = 0;
				}
				else
				{
					if (0 == strcmp(sEncoding, "UTF-16BE"))
					{
						GetUnicodeFromUTF16BE((char*)rec->string, (unsigned int)rec->stringLength, UName, ULen);
						return;
					}
				}
			}
		}
	}

	// default
	ULen = face_name_len;
	UName = new int[ULen + 1];

	for (unsigned int i = 0; i < ULen; ++i)
		UName[i] = (int)face_name[i];

	UName[ULen] = 0;
}
