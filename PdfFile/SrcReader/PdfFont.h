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
#ifndef _PDF_READER_FONT_H
#define _PDF_READER_FONT_H

#include <vector>
#include <string>
#include <map>

#include "../lib/xpdf/PDFDoc.h"
#include "../lib/xpdf/AcroForm.h"

#include "../../DesktopEditor/graphics/pro/Fonts.h"

#include "RendererOutputDev.h"
#include "PdfAnnot.h"

namespace PdfReader
{
struct CAnnotFontInfo
{
	std::wstring wsFontName;
	std::wstring wsFontPath;
	std::wstring wsActualFontName;
	bool bBold   = false;
	bool bItalic = false;
};

std::string GetRCFromDS(const std::string& sDS, Object* pContents, const std::vector<double>& arrCFromDA);
bool IsNeedCMap(PDFDoc* pDoc);
bool IsBaseFont(const std::wstring& wsName);
std::map<std::wstring, std::wstring> GetAllFonts(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, bool bIsNeedCMap);
std::wstring GetFontData(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList, Object* oFontRef, std::string& sFontName, std::string& sActualFontName, bool& bBold, bool& bItalic, bool bIsNeedCMap = false);
bool GetFontFromAP(PDFDoc* pdfDoc, AcroFormField* pField, Object* oFontRef, std::string& sFontKey);
std::vector<CAnnotFontInfo> GetAnnotFontInfos(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, Object* oAnnotRef);
std::map<std::wstring, std::wstring> GetFreeTextFont(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, Object* oAnnotRef, std::vector<CAnnotMarkup::CFontData*>& arrRC);
bool FindFonts(Object* oStream, int nDepth, Object* oResFonts);
void CollectFontWidths(GfxFont* gfxFont, Dict* pFontDict, std::map<unsigned int, unsigned int>& mGIDToWidth);
void CheckFontStylePDF(std::wstring& sName, bool& bBold, bool& bItalic);
bool EraseSubsetTag(std::wstring& sFontName);
}

#endif // _PDF_READER_FONT_H
