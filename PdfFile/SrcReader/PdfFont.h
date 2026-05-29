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
#ifndef _PDF_READER_FONT_H
#define _PDF_READER_FONT_H

#include <vector>
#include <string>
#include <map>

#include "../lib/xpdf/PDFDoc.h"
#include "../lib/xpdf/AcroForm.h"

#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

namespace PdfReader
{
struct TFontEntry
{
	std::wstring wsFilePath;     // Path to font on disk
	std::wstring wsFontName;     // Font name as written in PDF (branch for cases when font name is not specified in font itself)
	int*         pCodeToGID;     // Code to glyph number in font table
	int*         pCodeToUnicode; // Code to unicode value table
	unsigned int unLenGID;
	unsigned int unLenUnicode;
	bool         bAvailable;     // Is font available. Made for multithreading
	bool         bFontSubstitution = false;
	bool         bIsIdentity = false;
	double       dStretch = 1.0;

	std::wstring wsActualFontName;
	bool bBold   = false;
	bool bItalic = false;

};

class CPdfFontList
{
public:
	CPdfFontList();
	~CPdfFontList();
	bool Find(Ref oRef, TFontEntry* pEntry);
	bool Find2(Ref oRef, TFontEntry** ppEntry);
	void Remove(Ref oRef);
	TFontEntry* Add(Ref oRef, const std::wstring& wsFileName, int* pCodeToGID, int* pCodeToUnicode, unsigned int unLenGID, unsigned int unLenUnicode);
	void Clear();
	bool GetFont(Ref* pRef, TFontEntry* pEntry);
	const std::map<Ref, TFontEntry*>& GetFonts();
private:
	TFontEntry* Lookup(Ref& oRef);
	void Add(Ref& oRef, TFontEntry* pFontEntry);

private:
	std::map<Ref, TFontEntry*>          m_oFontMap;
	NSCriticalSection::CRITICAL_SECTION m_oCS; // Critical section
};
struct CType3FontMetrics
{
	double dLLx, dLLy, dURx, dURy;
	double arrFontMatrix[6];
	std::map<int, double> mapWidths;
	double dUnitsPerEm;
	double dAscent;
	double dDescent;

	CType3FontMetrics() : dLLx(0), dLLy(0), dURx(0), dURy(1000), dUnitsPerEm(1000), dAscent(800), dDescent(200)
	{
		arrFontMatrix[0] = 0.001; arrFontMatrix[1] = 0;
		arrFontMatrix[2] = 0;     arrFontMatrix[3] = 0.001;
		arrFontMatrix[4] = 0;     arrFontMatrix[5] = 0;
	}
	double GetScale() const
	{
		return arrFontMatrix[0] > 0 ? arrFontMatrix[0] * dUnitsPerEm : 1.0;
	}
	double GetGlyphWidth(int nCode) const
	{
		auto it = mapWidths.find(nCode);
		if (it != mapWidths.end())
			return it->second;
		return dUnitsPerEm * 0.5;
	}
};
struct CFontData
{
	bool bFind;
	BYTE nAlign;
	unsigned int unFontFlags; // 0 Bold, 1 Italic, 3 strikethrough, 4 underline, 5 vertical-align, 6 actual font, 7 RTL
	double dFontSise;
	double dVAlign;
	double dColor[3];
	std::string sFontFamily;
	std::string sActualFont;
	std::string sText;

	CFontData() : bFind(false), nAlign(0), unFontFlags(4), dFontSise(10), dVAlign(0), dColor{0, 0, 0} {}
	CFontData(const CFontData& oFont) : bFind(oFont.bFind), nAlign(oFont.nAlign), unFontFlags(oFont.unFontFlags), dFontSise(oFont.dFontSise), dVAlign(oFont.dVAlign),
		dColor{oFont.dColor[0], oFont.dColor[1], oFont.dColor[2]}, sFontFamily(oFont.sFontFamily), sActualFont(oFont.sActualFont), sText(oFont.sText) {}
};

std::vector<CFontData*> ReadRC(const std::string& sRC);
std::string GetRCFromDS(const std::string& sDS, Object* pContents, const std::vector<double>& arrCFromDA);
bool IsNeedCMap(PDFDoc* pDoc);
bool IsBaseFont(const std::wstring& wsName);
std::map<std::wstring, std::wstring> GetAllFonts(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList);
void GetFontData(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList, Object* oFontRef);
bool GetFontFromAP(PDFDoc* pdfDoc, AcroFormField* pField, Object* oFontRef, std::string& sFontKey);
std::vector<Ref> GetAnnotFontInfos(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, Object* oAnnotRef);
std::map<std::wstring, std::wstring> GetFreeTextFont(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, Object* oAnnotRef, std::vector<CFontData*>& arrRC);
bool FindFonts(Object* oStream, int nDepth, Object* oResFonts);
int CollectFontWidths(Dict* pFontDict, std::map<unsigned int, unsigned int>& mGIDToWidth);

NSFonts::CFontInfo* GetFontByParams(XRef* pXref, NSFonts::IFontManager* pFontManager, GfxFont* pFont, std::wstring& wsFontBaseName, double& dStretch);
void GetFont(XRef* pXref, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList, GfxFont* pFont, std::wstring& wsFileName, std::wstring& wsFontName, bool bNotFullName = true);

CType3FontMetrics* BuildType3FontMetrics(XRef* pXref, GfxFont* pFont);
}

#endif // _PDF_READER_FONT_H
