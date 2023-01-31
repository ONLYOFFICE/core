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
#include "FontCidTT.h"
#include "Document.h"
#include "Streams.h"
#include "Utils.h"
#include "FontTTWriter.h"

#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/common/File.h"

#include <string>

#include FT_TRUETYPE_TABLES_H

namespace PdfWriter
{
	static const char* c_sToUnicodeHeader = "/CIDInit /ProcSet findresource begin\n12 dict begin\nbegincmap\n";
	static const char* c_sToUnicodeInfo = "/CIDSystemInfo\n<< /Registry (Adobe)\n /Ordering (UCS)\n /Supplement 0\n >> def\n/CMapName /Adobe-Identity-UCS def\n/CMapType 2 def\n1 begincodespacerange\n<0000> <FFFF>\nendcodespacerange\n";
	static const char* c_sToUnicodeFooter = "endcmap\nCMapName currentdict /CMap defineresource pop\nend\nend\n";

	static int GetSymbolicCmapIndex(FT_Face pFace)
	{
		TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table(pFace, ft_sfnt_os2);
		if (NULL == pOs2 || 0xFFFF == pOs2->version)
			return -1;

		// Проверяем установлен ли 31 бит
		if (!(pOs2->ulCodePageRange1 & 0x80000000) && !(pOs2->ulCodePageRange1 == 0 && pOs2->ulCodePageRange2 == 0))
			return -1;


		for (int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++)
		{
			// Symbol
			if (0 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id)
				return nIndex;
		}

		return -1;
	}
	//----------------------------------------------------------------------------------------
	// CFontFileBase
	//----------------------------------------------------------------------------------------
	CFontCidTrueType::CFontCidTrueType(CXref* pXref, CDocument* pDocument, const std::wstring& wsFontPath, unsigned int unIndex) : CFontDict(pXref, pDocument)
	{
		m_bNeedAddFontName = true;
		CFontFileTrueType* pFontTT = CFontFileTrueType::LoadFromFile(wsFontPath, unIndex);
		m_pFontFile = pFontTT;

		m_wsFontPath  = wsFontPath;
		m_unFontIndex = unIndex;

		Add("Type", "Font");
		Add("Subtype", "Type0");
		Add("Encoding", "Identity-H");

		CDictObject* pFont = new CDictObject();
		m_pXref->Add(pFont);

		CArrayObject* pDescendantFonts = new CArrayObject();
		pDescendantFonts->Add(pFont);

		Add("DescendantFonts", pDescendantFonts);
		CDictObject* pToUnicodeDict = new CDictObject(m_pXref);
		Add("ToUnicode", pToUnicodeDict);
		pToUnicodeDict->SetFilter(STREAM_FILTER_FLATE_DECODE);
		m_pToUnicodeStream = pToUnicodeDict->GetStream();

		CreateCIDFont2(pFont);

		m_pFace         = NULL;
		m_pFaceMemory   = NULL;	
		m_nGlyphsCount  = 0;
		m_nSymbolicCmap = -1;
		m_ushCodesCount = 0;
	}
	CFontCidTrueType::~CFontCidTrueType()
	{
		if (m_pFontFile)
			delete m_pFontFile;

		if (m_pFace)
			FT_Done_Face(m_pFace);

		if (m_pFaceMemory)
			delete[] m_pFaceMemory;
	}
	void CFontCidTrueType::CreateCIDFont2(CDictObject* pFont)
	{
		m_pFont = pFont;
		pFont->Add("Type", "Font");
		pFont->Add("Subtype", "CIDFontType2");

		CDictObject* pSystemInfo = new CDictObject();
		pSystemInfo->Add("Registry", new CStringObject("Adobe"));
		pSystemInfo->Add("Ordering", new CStringObject("Identity"));
		pSystemInfo->Add("Supplement", 0);

		pFont->Add("CIDSystemInfo", pSystemInfo);

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
			UIntChangeBit(nFlags, 4);

		pFontDescriptor->Add("Flags", nFlags);

		if (m_pFontFile)
		{
			CArrayObject* pBBox = new CArrayObject();
			int* pFontBBox = m_pFontFile->GetBBox();
			pBBox->Add(pFontBBox[0]);
			pBBox->Add(pFontBBox[1]);
			pBBox->Add(pFontBBox[2]);
			pBBox->Add(pFontBBox[3]);
			pFontDescriptor->Add("FontBBox", pBBox);
		}
		pFontDescriptor->Add("ItalicAngle", 0);
		pFontDescriptor->Add("Ascent", m_pFontFile ? m_pFontFile->GetAscent() : 1000);
		pFontDescriptor->Add("Descent", m_pFontFile ? m_pFontFile->GetDescent() : -500);
		pFontDescriptor->Add("CapHeight", m_pFontFile ? m_pFontFile->GetCapHeight() : 800);
		pFontDescriptor->Add("StemV", 0);
		pFontDescriptor->Add("FontWeight", m_pFontFile ? m_pFontFile->GetWeight() : 400);

		m_pFontFileDict = new CDictObject(m_pXref);
		pFontDescriptor->Add("FontFile2", m_pFontFileDict);

		pFont->Add("FontDescriptor", pFontDescriptor);

		CDictObject* pCIDToGIDMapDict = new CDictObject(m_pXref);
		pFont->Add("CIDToGIDMap", pCIDToGIDMapDict);
		pCIDToGIDMapDict->SetFilter(STREAM_FILTER_FLATE_DECODE);
		m_pCidToGidMapStream = pCIDToGIDMapDict->GetStream();

		if (m_pXref->IsPDFA())
		{
			pFontDescriptor->Add("CIDSet", new CDictObject(m_pXref));
		}
	}
	bool CFontCidTrueType::HaveChar(const unsigned int &unUnicode)
	{
		if (!OpenFontFace())
			return false;

		return (!!GetGID(m_pFace, unUnicode));
	}
	unsigned int   CFontCidTrueType::GetWidth(unsigned short ushCode)
	{
		if (ushCode >= m_vWidths.size())
			return 0;

		return m_vWidths.at(ushCode);
	}
	unsigned int   CFontCidTrueType::GetGlyphWidth(unsigned short ushCode)
	{
		if (ushCode >= m_vGlypWidths.size())
			return 0;

		return m_vGlypWidths.at(ushCode);
	}
	bool CFontCidTrueType::IsItalic()
	{
		if (!OpenFontFace() || !m_pFace)
			return false;

		return ((m_pFace->style_flags & FT_STYLE_FLAG_ITALIC) != 0);
	}
	bool CFontCidTrueType::IsBold()
	{
		if (!OpenFontFace() || !m_pFace)
			return false;

		TT_OS2* pOS2 = (TT_OS2*)FT_Get_Sfnt_Table(m_pFace, ft_sfnt_os2);
		if (pOS2 && pOS2->version != 0xFFFF && pOS2->usWeightClass >= 800)
			return true;

		return ((m_pFace->style_flags & FT_STYLE_FLAG_BOLD) != 0);
	}
	void CFontCidTrueType::BeforeWrite()
	{
		if (m_pFontDescriptor)
		{
			CDictObject* pCIDSet = (CDictObject*)m_pFontDescriptor->Get("CIDSet");			
			if (pCIDSet)
			{
#ifndef FILTER_FLATE_DECODE_DISABLED
				pCIDSet->SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif
				CStream* pStream = pCIDSet->GetStream();

				unsigned int unBytes = (m_ushCodesCount) / 8;

				if (unBytes * 8 < m_ushCodesCount)
					unBytes++;

				if (1 == unBytes)
				{
					BYTE nValue = 0xFF;
					nValue = (BYTE)(nValue << (8 - m_ushCodesCount));
					nValue &= 0x7F;
					pStream->WriteChar(nValue);
				}
				else
				{
					BYTE nStartValue = 0x7F, nMidValue = 0xFF;
					pStream->WriteChar(nStartValue);

					for (unsigned int unIndex = 0; unIndex < unBytes - 2; ++unIndex)
					{
						pStream->WriteChar(nMidValue);
					}

					BYTE nEndValue = 0xFF;
					nEndValue = (BYTE)(nEndValue << (unBytes * 8 - m_ushCodesCount));

					pStream->WriteChar(nEndValue);
				}
			}
		}

		if (m_pFontFile)
		{
			unsigned short* pCodeToGid;
			unsigned int*   pWidths;
			unsigned char*  pGlyphs;
			unsigned int    unGlyphsCount;

			if (!GetWidthsAndGids(&pCodeToGid, &pWidths, &pGlyphs, unGlyphsCount))
				return;

			CStream* pStream = m_pFontFileDict->GetStream();
			m_pFontFile->WriteTTF(pStream, NULL, pCodeToGid, m_ushCodesCount, pGlyphs, unGlyphsCount);
			m_pFontFileDict->Add("Length1", pStream->Size());
			m_pFontFileDict->SetFilter(STREAM_FILTER_FLATE_DECODE);

			CArrayObject* pWArray = new CArrayObject();
			m_pFont->Add("W", pWArray);
			pWArray->Add(0);
			CArrayObject* pWidthsArray = new CArrayObject();
			pWArray->Add(pWidthsArray);
			for (unsigned short ushIndex = 0; ushIndex < m_ushCodesCount; ushIndex++)
			{
				pWidthsArray->Add(pWidths[ushIndex]);
			}

			pStream = m_pCidToGidMapStream;
			for (unsigned short ushCode = 0; ushCode < m_ushCodesCount; ushCode++)
			{
				unsigned short ushGid = pCodeToGid[ushCode];
				pStream->WriteChar(((ushGid >> 8) & 0xFF));
				pStream->WriteChar((ushGid & 0xFF));
			}

			RELEASEARRAYOBJECTS(pCodeToGid);
			RELEASEARRAYOBJECTS(pWidths);
			RELEASEARRAYOBJECTS(pGlyphs);

			WriteToUnicode();
		}
	}
	bool CFontCidTrueType::GetWidthsAndGids(unsigned short** ppCodeToGid, unsigned int** ppWidths, unsigned char** ppGlyphs, unsigned int& unGlyphsCount)
	{
		*ppCodeToGid  = NULL;
		*ppWidths     = NULL;
		*ppGlyphs     = NULL;
		unGlyphsCount = 0;
		
		if (!m_nGlyphsCount)
			return false;

		unsigned short* pCodeToGID = new unsigned short[m_ushCodesCount];
		if (!pCodeToGID)
			return false;

		unsigned int* pWidths = new unsigned int[m_ushCodesCount];
		if (!pWidths)
		{
			delete[] pCodeToGID;
			return false;
		}
		memset((void*)pWidths, 0x00, m_ushCodesCount * sizeof(unsigned int));

		for (unsigned short ushCode = 0; ushCode < m_ushCodesCount; ushCode++)
		{
			pCodeToGID[ushCode] = m_vCodeToGid.at(ushCode);
			pWidths[ushCode]    = m_vWidths.at(ushCode);
		}

		unsigned char *pGlyphs = new unsigned char[m_nGlyphsCount];
		if (!pGlyphs)
		{
			delete[] pCodeToGID;
			delete[] pWidths;
			return false;
		}

		memset((void *)pGlyphs, 0x00, m_nGlyphsCount * sizeof(unsigned char));
		for (auto oIt : m_mGlyphs)
		{
			pGlyphs[oIt.first] = 1;
		}

		*ppCodeToGid  = pCodeToGID;
		*ppWidths     = pWidths;
		*ppGlyphs     = pGlyphs;
		unGlyphsCount = m_nGlyphsCount;
		return true;
	}
	void CFontCidTrueType::WriteToUnicode()
	{
		CStream* pS = m_pToUnicodeStream;

		pS->WriteStr(c_sToUnicodeHeader);
		pS->WriteStr(c_sToUnicodeInfo);

		pS->WriteInt(m_ushCodesCount);
		pS->WriteStr(" beginbfchar\n");
		for (unsigned short ushCode = 0; ushCode < m_ushCodesCount; ushCode++)
		{
			pS->WriteChar('<');
			pS->WriteHex(ushCode, 4);
			pS->WriteStr("> <");
			for (unsigned int i = 0, nLen = m_vUnicodes[ushCode].size(); i < nLen; i++)
			{
				unsigned int unUnicode = m_vUnicodes[ushCode][i];
				if (unUnicode < 0x10000)
				{
					pS->WriteHex(unUnicode, 4);
				}
				else
				{
					unUnicode = unUnicode - 0x10000;

					unsigned short ushLo = 0xDC00 | (unUnicode & 0x3FF);
					unsigned short ushHi = 0xD800 | (unUnicode >> 10);

					pS->WriteHex(ushHi, 4);
					pS->WriteHex(ushLo, 4);
				}
			}

			pS->WriteStr(">\n");
		}
		pS->WriteStr("endbfchar\n");
		m_pToUnicodeStream->WriteStr(c_sToUnicodeFooter);
	}
	void CFontCidTrueType::CloseFontFace()
	{
		if (m_pFace)
		{
			FT_Done_Face(m_pFace);
			m_pFace = NULL;
		}

		RELEASEARRAYOBJECTS(m_pFaceMemory);
	}
	bool CFontCidTrueType::OpenFontFace()
	{
		if (m_pFace)
		{
			m_pDocument->AddFreeTypeFont(this);
			return true;
		}

		m_nGlyphsCount  = 0;
		m_nSymbolicCmap = -1;

		FT_Library pLibrary = m_pDocument->GetFreeTypeLibrary();
		if (!pLibrary)
			return false;

		DWORD dwFileSize;
		m_pFaceMemory = NULL;
		NSFile::CFileBinary::ReadAllBytes(m_wsFontPath, &m_pFaceMemory, dwFileSize);
		if (!m_pFaceMemory)
			return false;

		FT_New_Memory_Face(pLibrary, m_pFaceMemory, dwFileSize, m_unFontIndex, &m_pFace);

		if (!m_pFace)
		{
			RELEASEARRAYOBJECTS(m_pFaceMemory);
			return false;
		}

		m_pDocument->AddFreeTypeFont(this);
		m_nGlyphsCount  = m_pFace->num_glyphs;
		m_nSymbolicCmap = GetSymbolicCmapIndex(m_pFace);

		if (m_bNeedAddFontName)
		{
			// Дописываем имя шрифта во все необходимые словари, а также заполняем дескриптор
            std::string sFontName = m_pDocument->GetTTFontTag();
            sFontName += (m_pFace->family_name ? std::string(m_pFace->family_name) : std::string());
			if (m_pFace->style_flags & FT_STYLE_FLAG_ITALIC)
				sFontName += "-Italic";
			if (m_pFace->style_flags & FT_STYLE_FLAG_BOLD)
				sFontName += "-Bold";

			const char* sName = sFontName.c_str();

			Add("BaseFont", sName);
			m_pFont->Add("BaseFont", sName);
			m_pFontDescriptor->Add("FontName", sName);
			m_bNeedAddFontName = false;
		}

		return true;
	}
	unsigned short CFontCidTrueType::EncodeUnicode(const unsigned int &unUnicode)
	{
		std::map<unsigned int, unsigned short>::const_iterator oIter = m_mUnicodeToCode.find(unUnicode);
		if (oIter != m_mUnicodeToCode.end())
			return oIter->second;

		unsigned int unGID = GetGID(m_pFace, unUnicode);
		if (0 == unGID && -1 != m_nSymbolicCmap)
			unGID = GetGID(m_pFace, unUnicode + 0xF000);

		unsigned short ushCode = EncodeGID(unGID, &unUnicode, 1);
		m_mUnicodeToCode.insert(std::pair<unsigned int, unsigned short>(unUnicode, ushCode));
		return ushCode;
	}
	unsigned short CFontCidTrueType::EncodeGID(const unsigned int& unGID, const unsigned int* pUnicodes, const unsigned int& unCount)
	{
		for (unsigned short ushCurCode = 0, ushCodesCount = m_vCodeToGid.size(); ushCurCode < ushCodesCount; ushCurCode++)
		{
			if (unGID == m_vCodeToGid.at(ushCurCode))
				return ushCurCode;
		}

		if (!OpenFontFace())
			return 0;

		unsigned short ushCode = m_ushCodesCount++;

		std::vector<unsigned int> vUnicodes;
		for (unsigned int i = 0; i < unCount; i++)
		{
			vUnicodes.push_back(pUnicodes[i]);
		}

		m_vUnicodes.push_back(vUnicodes);
		m_vCodeToGid.push_back(unGID);

		m_mGlyphs.insert(std::pair<unsigned int, bool>(unGID, true));

		// Если данный символ составной (CompositeGlyf), тогда мы должны учесть все его дочерные символы (subglyfs)
		if (0 == FT_Load_Glyph(m_pFace, unGID, FT_LOAD_NO_SCALE | FT_LOAD_NO_RECURSE))
		{
			for (int nSubIndex = 0; nSubIndex < m_pFace->glyph->num_subglyphs; nSubIndex++)
			{
				FT_Int       nSubGID;
				FT_UInt      unFlags;
				FT_Int       nArg1;
				FT_Int       nArg2;
				FT_Matrix    oMatrix;
				FT_Get_SubGlyph_Info(m_pFace->glyph, nSubIndex, &nSubGID, &unFlags, &nArg1, &nArg2, &oMatrix);

				m_mGlyphs.insert(std::pair<unsigned short, bool>(nSubGID, true));
			}

			if (0 != m_pFace->units_per_EM)
			{
				m_vWidths.push_back((unsigned int)m_pFace->glyph->metrics.horiAdvance * 1000 / m_pFace->units_per_EM);
				m_vGlypWidths.push_back((unsigned int)(m_pFace->glyph->metrics.width) * 1000 / m_pFace->units_per_EM);
			}
			else
			{
				m_vWidths.push_back((unsigned int)m_pFace->glyph->metrics.horiAdvance);
				m_vGlypWidths.push_back((unsigned int)(m_pFace->glyph->metrics.width) * 1000 / m_pFace->units_per_EM);
			}
		}
		else
		{
			m_vWidths.push_back(0);
			m_vGlypWidths.push_back(0);
		}

		return ushCode;
	}
}
