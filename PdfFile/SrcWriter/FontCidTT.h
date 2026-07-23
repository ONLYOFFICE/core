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
#ifndef _PDF_WRITER_SRC_FONTCIDTT_H
#define _PDF_WRITER_SRC_FONTCIDTT_H

#include "Font.h"
#include "Objects.h"
#include <string>
#include <map>
#include <vector>

#include "../../DesktopEditor/common/File.h"

#include <ft2build.h>
#include FT_TRUETYPE_TABLES_H

namespace PdfWriter
{
	static unsigned int GetGID(FT_Face pFace, unsigned int unUnicode)
	{
		int nCharIndex = 0;

		if (!pFace)
			return nCharIndex;

		for (int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++)
		{
			FT_CharMap pCharMap = pFace->charmaps[nIndex];

			if (FT_Set_Charmap(pFace, pCharMap))
				continue;
			FT_Encoding pEncoding = pCharMap->encoding;

			if (FT_ENCODING_UNICODE == pEncoding)
			{
				if (nCharIndex = FT_Get_Char_Index(pFace, unUnicode))
					return nCharIndex;
			}

			if (FT_ENCODING_NONE == pEncoding || FT_ENCODING_MS_SYMBOL == pEncoding || FT_ENCODING_APPLE_ROMAN == pEncoding)
			{
				nCharIndex = FT_Get_Char_Index(pFace, unUnicode);
			}
			/*else if ( FT_ENCODING_ADOBE_STANDARD == pEncoding )
			{
			nCharIndex = FT_Get_Char_Index( pFace, unUnicode );
			}
			else if ( FT_ENCODING_ADOBE_CUSTOM == pEncoding )
			{
			nCharIndex = FT_Get_Char_Index( pFace, unUnicode );
			}
			else if ( FT_ENCODING_ADOBE_EXPERT == pEncoding )
			{
			nCharIndex = FT_Get_Char_Index( pFace, unUnicode );
			}*/
		}

		return nCharIndex;
	}


	class CXref;
	class CStream;
	class CFontFileTrueType;
	class CDocument;
	//----------------------------------------------------------------------------------------
	// CFontFileBase
	//----------------------------------------------------------------------------------------
	class CFontCidTrueType : public CFontDict
	{
	public:

		CFontCidTrueType(CXref* pXref, CDocument* pDocument, const std::wstring& wsFontPath, unsigned int unIndex, CFontFileTrueType* pFontTT);
		~CFontCidTrueType();
		unsigned short EncodeUnicode(const unsigned int& unUnicode);
		unsigned short EncodeGID(const unsigned int& unGID, const unsigned int* pUnicodes, const unsigned int& unCount);
		int GetUnicodeByGID(int gid);

		bool           HaveChar(const unsigned int& unUnicode);
		unsigned int   GetChar(const unsigned int& unUnicode);
		unsigned int   GetWidth(unsigned short ushCode);
		unsigned int   GetGlyphWidth(unsigned short ushCode);
		bool           IsItalic();
		bool           IsBold();
		EFontType      GetFontType()
		{
			return fontCIDType2;
		}

	private:

		void BeforeWrite();
		bool GetWidthsAndGids(unsigned short** ppCodeToGid, unsigned int** pWidths, unsigned char** ppGlyphs, unsigned int& unGlyphsCount);
		void CreateCIDFont(CDictObject* pFont);
		void CreateCIDFont2(CDictObject* pFont);
		void CreateCIDFont0(CDictObject* pFont);
		void WriteToUnicode();
		bool OpenFontFace();
		void CloseFontFace();

	private:

		std::wstring                             m_wsFontPath;
		unsigned int                             m_unFontIndex;

		CFontFileTrueType*                       m_pFontFile;
		CDictObject*                             m_pFontFileDict;
		CStream*                                 m_pCidToGidMapStream;
		CStream*                                 m_pToUnicodeStream;
		CDictObject*                             m_pFont;
		CDictObject*                             m_pFontDescriptor;

		unsigned short                           m_ushCodesCount;   // Number of encoded characters
		std::map<unsigned int, unsigned short>   m_mUnicodeToCode;  // Map Unicode->character code
		std::vector<std::vector<unsigned int>>   m_vUnicodes;       // Reverse map character code -> unicodes

		std::vector<unsigned int>                m_vCodeToGid;
		std::vector<unsigned int>                m_vWidths;     // glyph.advance
		std::map<unsigned short, bool>           m_mGlyphs;
		std::vector<unsigned int>                m_vGlypWidths; // glyph.width

		FT_Face                                  m_pFace;
		FT_Byte*                                 m_pFaceMemory;
		int                                      m_nGlyphsCount;
		int                                      m_nSymbolicCmap;
		bool                                     m_bNeedAddFontName;
		bool                                     m_bFromMemory;

		friend class CDocument;
	};
}

#endif // _PDF_WRITER_SRC_FONTCIDTT_H
