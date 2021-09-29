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
#include "FontTT.h"
#include "FontCidTT.h"
#include "Document.h"
#include "Streams.h"
#include "Utils.h"
#include "FontTTWriter.h"
#include "Encodings.h"

#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/common/File.h"

#include <string>

#include FT_TRUETYPE_TABLES_H

namespace PdfWriter
{
	//----------------------------------------------------------------------------------------
	// CFontTrueType
	//----------------------------------------------------------------------------------------
	CFontTrueType::CFontTrueType(CXref* pXref, CDocument* pDocument, const std::wstring& wsFontPath, unsigned int unIndex) : CFontDict(pXref, pDocument)
	{
		CFontFileTrueType* pFontTT = CFontFileTrueType::LoadFromFile(wsFontPath, unIndex);
		m_pFontFile = pFontTT;

		m_wsFontPath  = wsFontPath;
		m_unFontIndex = unIndex;

		Add("Type", "Font");
		Add("Subtype", "TrueType");
		Add("Encoding", "WinAnsiEncoding");
		Add("FirstChar", 0);
		Add("LastChar", 255);
		
		CDictObject* pFontDescriptor = new CDictObject();
		// FontDescriptor обязательно должен идти ссылкой
		m_pXref->Add(pFontDescriptor);
		pFontDescriptor->Add("Type", "FontDescriptor");
		m_pFontDescriptor = pFontDescriptor;

		// Выставляем бит Symbolic, а бит NonSymbolic убираем
		unsigned int nFlags = 0;
		if (!(nFlags & 4))
			UIntChangeBit(nFlags, 2);
		if (nFlags & 32)
			UIntChangeBit(nFlags, 5);

		pFontDescriptor->Add("Flags", nFlags);

		CArrayObject* pBBox = new CArrayObject();
		int* pFontBBox = m_pFontFile->GetBBox();
		pBBox->Add(pFontBBox[0]);
		pBBox->Add(pFontBBox[1]);
		pBBox->Add(pFontBBox[2]);
		pBBox->Add(pFontBBox[3]);
		pFontDescriptor->Add("FontBBox", pBBox);
		pFontDescriptor->Add("ItalicAngle", 0);
		pFontDescriptor->Add("Ascent", m_pFontFile->GetAscent());
		pFontDescriptor->Add("Descent", m_pFontFile->GetDescent());
		pFontDescriptor->Add("CapHeight", m_pFontFile->GetCapHeight());
		pFontDescriptor->Add("StemV", 80);
		pFontDescriptor->Add("FontWeight", m_pFontFile->GetWeight());

		m_pFontFileDict = new CDictObject(m_pXref);
		pFontDescriptor->Add("FontFile2", m_pFontFileDict);

		Add("FontDescriptor", pFontDescriptor);

		ReadName();
	}
	CFontTrueType::~CFontTrueType()
	{
		if (m_pFontFile)
			delete m_pFontFile;
	}
	void CFontTrueType::BeforeWrite()
	{
		if (m_pFontFile)
		{
			CStream* pStream = m_pFontFileDict->GetStream();
			m_pFontFile->WriteTTF(pStream);
			m_pFontFileDict->Add("Length1", pStream->Size());
			m_pFontFileDict->SetFilter(STREAM_FILTER_FLATE_DECODE);
		}
	}
	void CFontTrueType::ReadName()
	{
		FT_Library pLibrary = m_pDocument->GetFreeTypeLibrary();
		if (!pLibrary)
			return;

		DWORD dwFileSize;
		FT_Byte* pFaceMemory = NULL;
		NSFile::CFileBinary::ReadAllBytes(m_wsFontPath, &pFaceMemory, dwFileSize);
		if (!pFaceMemory)
			return;

		FT_Face pFace = NULL;
		FT_New_Memory_Face(pLibrary, pFaceMemory, dwFileSize, m_unFontIndex, &pFace);

		if (!pFace)
		{
			RELEASEARRAYOBJECTS(pFaceMemory);
			return;
		}

		// Дописываем имя шрифта во все необходимые словари, а также заполняем дескриптор
		std::string sFontName = std::string(pFace->family_name);
		if (pFace->style_flags & FT_STYLE_FLAG_ITALIC)
			sFontName += "-Italic";
		if (pFace->style_flags & FT_STYLE_FLAG_BOLD)
			sFontName += "-Bold";

		const char* sName = sFontName.c_str();

		Add("BaseFont", sName);
		Add("Name", sName);
		m_pFontDescriptor->Add("FontName", sName);

		CArrayObject* pWidths = new CArrayObject();
		Add("Widths", pWidths);

		for (unsigned int unIndex = 0; unIndex < 255; ++unIndex)
		{
			unsigned int unGID = GetGID(pFace, c_arrWinAnsiEncoding[unIndex]);

			if (0 == FT_Load_Glyph(pFace, unGID, FT_LOAD_NO_SCALE | FT_LOAD_NO_RECURSE))
			{
				unsigned int unWidth = 0;
				if (0 != pFace->units_per_EM)
					unWidth = ((unsigned int)pFace->glyph->metrics.horiAdvance * 1000 / pFace->units_per_EM);
				else
					unWidth = ((unsigned int)pFace->glyph->metrics.horiAdvance);

				pWidths->Add(unWidth);
			}
		}

		FT_Done_Face(pFace);
		RELEASEARRAYOBJECTS(pFaceMemory);
	}
}
