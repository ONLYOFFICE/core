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
		unsigned char* EncodeString(unsigned int* pUnicodes, unsigned int unLen, const unsigned int* pGids = NULL);
		unsigned int   GetWidth(unsigned short ushCode);
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
		std::vector<unsigned int>                m_vUnicodes;       // Обратный мап код символа -> юникод

		std::vector<unsigned int>                m_vCodeToGid;
		std::vector<unsigned int>                m_vWidths;
		std::map<unsigned short, bool>           m_mGlyphs;

		FT_Face                                  m_pFace;
		FT_Byte*                                 m_pFaceMemory;
		int                                      m_nGlyphsCount;
		int                                      m_nSymbolicCmap;
		bool                                     m_bNeedAddFontName;

		friend class CDocument;
	};
}

#endif // _PDF_WRITER_SRC_FONTCIDTT_H
