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
		bool bFromMemory = false;
		BYTE* pData = NULL;
		LONG  lLength = 0;

#ifdef BUILDING_WASM_MODULE
		NSFonts::IFontsMemoryStorage* pMemoryStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
		if (pMemoryStorage)
		{
			NSFonts::IFontStream* pStream = pMemoryStorage->Get(wsFontPath);
			if (pStream)
			{
				bFromMemory = true;
				pStream->GetMemory(pData, lLength);
			}
		}
#endif
		CFontFileTrueType* pFontTT = NULL;
		if (bFromMemory)
			CFontFileTrueType::LoadFromBuffer((char*)pData, lLength, unIndex);
		else
			CFontFileTrueType::LoadFromFile(wsFontPath, unIndex);
		m_pFontFile = pFontTT;

		m_wsFontPath  = wsFontPath;
		m_unFontIndex = unIndex;

		Add("Type", "Font");
		Add("Subtype", "TrueType");
		Add("Encoding", "WinAnsiEncoding");
		Add("FirstChar", 0);
		Add("LastChar", 255);
		
		CDictObject* pFontDescriptor = new CDictObject();
		// FontDescriptor must be a reference
		m_pXref->Add(pFontDescriptor);
		pFontDescriptor->Add("Type", "FontDescriptor");
		m_pFontDescriptor = pFontDescriptor;

		Add("FontDescriptor", pFontDescriptor);

		ReadName();

		if (m_bCanEmbed)
		{
			// Set Symbolic bit and clear NonSymbolic bit
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
			// Set NonSymbolic flag, because Adobe poorly selects fonts with Symbolic flag
			// TODO: Should add check whether to set Symbolic flag
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
			if (m_pFontFile->GetOpenTypeCFF())
				m_pFontFile->WriteOTF(pStream);
			else
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

		bool bFromMemory = false;
		DWORD dwFileSize;
		FT_Byte* pFaceMemory = NULL;

#ifdef BUILDING_WASM_MODULE
		NSFonts::IFontsMemoryStorage* pMemoryStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
		if (pMemoryStorage)
		{
			NSFonts::IFontStream* pStream = pMemoryStorage->Get(m_wsFontPath);
			if (pStream)
			{
				bFromMemory = true;
				LONG lLength = 0;
				pStream->GetMemory(pFaceMemory, lLength);
				dwFileSize = lLength;
			}
		}
#else
		NSFile::CFileBinary::ReadAllBytes(m_wsFontPath, &pFaceMemory, dwFileSize);
#endif
		if (!pFaceMemory)
			return;

		FT_Face pFace = NULL;
		FT_New_Memory_Face(pLibrary, pFaceMemory, dwFileSize, m_unFontIndex, &pFace);

		if (!pFace)
		{
			if (!bFromMemory)
				RELEASEARRAYOBJECTS(pFaceMemory);
			return;
		}

		// Write font name to all required dictionaries and fill descriptor
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

		m_dHeight  = pFace->height;
		m_dDescent = pFace->descender;
		m_dAscent  = pFace->ascender;
		m_dUnitsPerEm = pFace->units_per_EM;
		m_dMaxY = pFace->bbox.yMax;
		m_dMinY = pFace->bbox.yMin;

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

		// Currently this class is used for embedding fonts that will be used for filling
		// within forms. If the class is used for something else, embedding restrictions need to be set
		FT_UShort fsType = FT_Get_FSType_Flags(pFace);
		m_bCanEmbed = NSFonts::CFontInfo::CanEmbedForEdit(fsType);

		FT_Done_Face(pFace);
		if (!bFromMemory)
			RELEASEARRAYOBJECTS(pFaceMemory);
	}
}
