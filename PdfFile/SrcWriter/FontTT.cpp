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
	CFontTrueType::CFontTrueType(CXref* pXref, CDocument* pDocument, const std::wstring& wsFontPath, unsigned int unIndex) : CFontDict(pXref, pDocument), m_bCanEmbed(false)
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

		Add("FontDescriptor", pFontDescriptor);

		ReadName();

		if (m_bCanEmbed)
		{
			// Выставляем бит Symbolic, а бит NonSymbolic убираем
			unsigned int nFlags = 0;
			if (!(nFlags & 4))
				UIntChangeBit(nFlags, 2);
			if (nFlags & 32)
				UIntChangeBit(nFlags, 5);

			pFontDescriptor->Add("Flags", nFlags);

			m_pFontFileDict = new CDictObject(m_pXref);
			pFontDescriptor->Add("FontFile2", m_pFontFileDict);
		}
		else
		{
			// Ставим флаг NonSymbolic, т.к. Adobe плохо подбирает шрифт с Symbolic флагом
			// TODO: Хорошо бы сделать проверку, стоит ли ставить флаг Symbolic
			unsigned int nFlags = 0;
			UIntChangeBit(nFlags, 5);
			pFontDescriptor->Add("Flags", nFlags);
			m_pFontFileDict = NULL;
		}

	}
	CFontTrueType::~CFontTrueType()
	{
		if (m_pFontFile)
			delete m_pFontFile;
	}
	void CFontTrueType::BeforeWrite()
	{
		if (m_pFontFile && m_pFontFileDict)
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
		std::string sFontName = pFace->family_name ? std::string(pFace->family_name) : std::string();
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

		unsigned int unUnitsPerEm = pFace->units_per_EM;
		double dKoef = 1000.0 / unUnitsPerEm;

		for (unsigned int unIndex = 0; unIndex < 255; ++unIndex)
		{
			unsigned int unGID = GetGID(pFace, c_arrWinAnsiEncoding[unIndex]);

			if (0 == FT_Load_Glyph(pFace, unGID, FT_LOAD_NO_SCALE | FT_LOAD_NO_RECURSE))
			{
				unsigned int unWidth = 0;
				if (unUnitsPerEm)
					unWidth = ((unsigned int)pFace->glyph->metrics.horiAdvance * 1000 / unUnitsPerEm);
				else
					unWidth = ((unsigned int)pFace->glyph->metrics.horiAdvance);

				pWidths->Add(unWidth);
			}
		}

		int xMin = (int)(unUnitsPerEm ? pFace->bbox.xMin * dKoef : pFace->bbox.xMin);
		int yMin = (int)(unUnitsPerEm ? pFace->bbox.yMin * dKoef : pFace->bbox.yMin);
		int xMax = (int)(unUnitsPerEm ? pFace->bbox.xMax * dKoef : pFace->bbox.xMax);
		int yMax = (int)(unUnitsPerEm ? pFace->bbox.yMax * dKoef : pFace->bbox.yMax);

		m_nLineHeight = yMax - yMin;
		m_nAscent     = yMax;

		CArrayObject* pBBox = new CArrayObject();
		pBBox->Add(xMin);
		pBBox->Add(yMin);
		pBBox->Add(xMax);
		pBBox->Add(yMax);
		m_pFontDescriptor->Add("FontBBox", pBBox);
		m_pFontDescriptor->Add("Ascent", yMax);
		m_pFontDescriptor->Add("Descent", yMin);
		m_pFontDescriptor->Add("CapHeight", m_pFontFile ? (int)(unUnitsPerEm ? m_pFontFile->GetCapHeight() * dKoef : m_pFontFile->GetCapHeight()) : 800);
		m_pFontDescriptor->Add("ItalicAngle", 0);
		m_pFontDescriptor->Add("StemV", 80);
		m_pFontDescriptor->Add("FontWeight", m_pFontFile ? m_pFontFile->GetWeight() : 400);

		// Сейчас мы этот класс используем для внедрения шрифтов, которые будут использоваться для заполнения
		// внутри форм. Если класс будет использоваться для чего-то другого, тогда надо задавать ограничения на внедрение
		FT_UShort fsType = FT_Get_FSType_Flags(pFace);
		m_bCanEmbed = NSFonts::CFontInfo::CanEmbedForEdit(fsType);

		FT_Done_Face(pFace);
		RELEASEARRAYOBJECTS(pFaceMemory);
	}
}
