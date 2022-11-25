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

		CFontCidTrueType(CXref* pXref, CDocument* pDocument, const std::wstring& wsFontPath, unsigned int unIndex);
		~CFontCidTrueType();
		unsigned short EncodeUnicode(const unsigned int& unUnicode);
		unsigned short EncodeGID(const unsigned int& unGID, const unsigned int* pUnicodes, const unsigned int& unCount);

		bool           HaveChar(const unsigned int& unUnicode);
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
		void CreateCIDFont2(CDictObject* pFont);
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

		unsigned short                           m_ushCodesCount;   // Количество закодированных символов
		std::map<unsigned int, unsigned short>   m_mUnicodeToCode;  // Мап Юникод->код символа
		std::vector<std::vector<unsigned int>>   m_vUnicodes;       // Обратный мап код символа -> юникодЫ

		std::vector<unsigned int>                m_vCodeToGid;
		std::vector<unsigned int>                m_vWidths;     // glyph.advance
		std::map<unsigned short, bool>           m_mGlyphs;
		std::vector<unsigned int>                m_vGlypWidths; // glyph.width

		FT_Face                                  m_pFace;
		FT_Byte*                                 m_pFaceMemory;
		int                                      m_nGlyphsCount;
		int                                      m_nSymbolicCmap;
		bool                                     m_bNeedAddFontName;

		friend class CDocument;
	};
}

#endif // _PDF_WRITER_SRC_FONTCIDTT_H
