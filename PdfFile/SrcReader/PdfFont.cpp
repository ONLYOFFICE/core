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

#include "PdfFont.h"
#include "Adaptors.h"
#include "MemoryUtils.h"

#include <string_view>

#include "../lib/xpdf/TextString.h"
#include "../lib/xpdf/GfxFont.h"
#include "../lib/xpdf/Lexer.h"
#include "../lib/xpdf/Parser.h"
#include "../lib/xpdf/CharCodeToUnicode.h"
#include "../lib/xpdf/Decrypt.h"
#include "../lib/fofi/FoFiTrueType.h"
#include "../lib/fofi/FoFiType1C.h"
#include "../lib/fofi/FoFiIdentifier.h"
#include "../Resources/BaseFonts.h"

#include "../../DesktopEditor/common/StringExt.h"
#include "../../DesktopEditor/common/StringBuilder.h"
#include "../../DesktopEditor/common/Path.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"
#include "../../DesktopEditor/graphics/BaseThread.h"

#ifndef BUILDING_WASM_MODULE
#define FONTS_USE_AFM_SETTINGS
#else
#include "../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"
#include "FontsWasm.h"
#define FONTS_USE_ONLY_MEMORY_STREAMS
#endif

namespace
{
constexpr std::string_view arrCMap[] = {"GB-EUC-H", "GB-EUC-V", "GB-H", "GB-V", "GBpc-EUC-H", "GBpc-EUC-V", "GBK-EUC-H",
"GBK-EUC-V", "GBKp-EUC-H", "GBKp-EUC-V", "GBK2K-H", "GBK2K-V", "GBT-H", "GBT-V", "GBTpc-EUC-H", "GBTpc-EUC-V",
"UniGB-UCS2-H", "UniGB-UCS2-V", "UniGB-UTF8-H", "UniGB-UTF8-V", "UniGB-UTF16-H", "UniGB-UTF16-V", "UniGB-UTF32-H",
"UniGB-UTF32-V", "B5pc-H", "B5pc-V", "B5-H", "B5-V", "HKscs-B5-H", "HKscs-B5-V", "HKdla-B5-H", "HKdla-B5-V",
"HKdlb-B5-H", "HKdlb-B5-V", "HKgccs-B5-H", "HKgccs-B5-V", "HKm314-B5-H", "HKm314-B5-V", "HKm471-B5-H",
"HKm471-B5-V", "ETen-B5-H", "ETen-B5-V", "ETenms-B5-H", "ETenms-B5-V", "ETHK-B5-H", "ETHK-B5-V", "CNS-EUC-H",
"CNS-EUC-V", "CNS1-H", "CNS1-V", "CNS2-H", "CNS2-V", "UniCNS-UCS2-H", "UniCNS-UCS2-V", "UniCNS-UTF8-H",
"UniCNS-UTF8-V", "UniCNS-UTF16-H", "UniCNS-UTF16-V", "UniCNS-UTF32-H", "UniCNS-UTF32-V", "78-EUC-H", "78-EUC-V",
"78-H", "78-V", "78-RKSJ-H", "78-RKSJ-V", "78ms-RKSJ-H", "78ms-RKSJ-V","83pv-RKSJ-H", "90ms-RKSJ-H", "90ms-RKSJ-V",
"90msp-RKSJ-H", "90msp-RKSJ-V", "90pv-RKSJ-H", "90pv-RKSJ-V", "Add-H", "Add-V", "Add-RKSJ-H", "Add-RKSJ-V",
"EUC-H", "EUC-V", "Ext-RKSJ-H", "Ext-RKSJ-V", "H", "V", "NWP-H", "NWP-V", "RKSJ-H", "RKSJ-V", "UniJIS-UCS2-H",
"UniJIS-UCS2-V", "UniJIS-UCS2-HW-H", "UniJIS-UCS2-HW-V", "UniJIS-UTF8-H", "UniJIS-UTF8-V", "UniJIS-UTF16-H",
"UniJIS-UTF16-V", "UniJIS-UTF32-H", "UniJIS-UTF32-V", "UniJIS2004-UTF8-H", "UniJIS2004-UTF8-V", "UniJIS2004-UTF16-H",
"UniJIS2004-UTF16-V", "UniJIS2004-UTF32-H", "UniJIS2004-UTF32-V", "UniJISPro-UCS2-V", "UniJISPro-UCS2-HW-V",
"UniJISPro-UTF8-V", "UniJISX0213-UTF32-H", "UniJISX0213-UTF32-V", "UniJISX02132004-UTF32-H", "UniJISX02132004-UTF32-V",
"WP-Symbol", "Hankaku", "Hiragana", "Katakana", "Roman", "KSC-EUC-H", "KSC-EUC-V", "KSC-H", "KSC-V", "KSC-Johab-H",
"KSC-Johab-V", "KSCms-UHC-H", "KSCms-UHC-V", "KSCms-UHC-HW-H", "KSCms-UHC-HW-V", "KSCpc-EUC-H", "KSCpc-EUC-V",
"UniKS-UCS2-H", "UniKS-UCS2-V", "UniKS-UTF8-H", "UniKS-UTF8-V", "UniKS-UTF16-H", "UniKS-UTF16-V", "UniKS-UTF32-H",
"UniKS-UTF32-V", "UniAKR-UTF8-H", "UniAKR-UTF16-H", "UniAKR-UTF32-H"};
bool scanCMapFonts(Dict *pResources, int nDepth, std::vector<int>& arrUniqueResources)
{
	if (nDepth > 5)
		return false;

	{
		CObjectGuard oFonts;
		if (pResources->lookup("Font", oFonts.get())->isDict())
		{
			for (int i = 0, nLength = oFonts->dictGetLength(); i < nLength; ++i)
			{
				CObjectGuard oFont, oEncoding;
				if (!oFonts->dictGetVal(i, oFont.get())->isDict() || !oFont->dictLookup("Encoding", oEncoding.get())->isName())
					continue;
				if (std::find(std::begin(arrCMap), std::end(arrCMap), std::string_view(oEncoding->getName())) != std::end(arrCMap))
					return true;
			}
		}
	}

	auto fScanFonts = [pResources, nDepth, &arrUniqueResources](const char* sName)
	{
		CObjectGuard oObject;
		if (!pResources->lookup(sName, oObject.get())->isDict())
			return false;
		for (int i = 0, nLength = oObject->dictGetLength(); i < nLength; ++i)
		{
			CObjectGuard oXObj, oResources;
			if (!oObject->dictGetVal(i, oXObj.get())->isStream() || !oXObj->streamGetDict()->lookup("Resources", oResources.get())->isDict())
				continue;
			CObjectGuard oRef;
			if (oXObj->streamGetDict()->lookupNF("Resources", oRef.get())->isRef() && std::find(arrUniqueResources.begin(), arrUniqueResources.end(), oRef->getRef().num) != arrUniqueResources.end())
				continue;
			arrUniqueResources.push_back(oRef->getRef().num);
			if (scanCMapFonts(oResources->getDict(), nDepth + 1, arrUniqueResources))
				return true;
		}
		return false;
	};

	if (fScanFonts("XObject") || fScanFonts("Pattern"))
		return true;

	CObjectGuard oExtGState;
	if (!pResources->lookup("ExtGState", oExtGState.get())->isDict())
		return false;
	for (int i = 0, nLength = oExtGState->dictGetLength(); i < nLength; ++i)
	{
		CObjectGuard oGS, oSMask, oSMaskGroup, oResources;
		if (!oExtGState->dictGetVal(i, oGS.get())->isDict() || !oGS->dictLookup("SMask", oSMask.get())->isDict() || !oSMask->dictLookup("G", oSMaskGroup.get())->isStream() || !oSMaskGroup->streamGetDict()->lookup("Resources", oResources.get())->isDict())
			continue;
		CObjectGuard oRef;
		if (oSMaskGroup->streamGetDict()->lookupNF("Resources", oRef.get())->isRef() && std::find(arrUniqueResources.begin(), arrUniqueResources.end(), oRef->getRef().num) != arrUniqueResources.end())
			continue;
		arrUniqueResources.push_back(oRef->getRef().num);
		if (scanCMapFonts(oResources->getDict(), nDepth + 1, arrUniqueResources))
			return true;
	}

	return false;
}
bool scanCMapAPFonts(Object* oAnnot, std::vector<int>& arrUniqueResources)
{
	CObjectGuard oAP;
	if (!oAnnot->dictLookup("AP", oAP.get())->isDict())
		return false;
	auto fScanAPView = [&arrUniqueResources](Object* oAP, const char* sName)
	{
		CObjectGuard oAPi, oRes;
		if (!oAP->dictLookup(sName, oAPi.get())->isStream() || !oAPi->streamGetDict()->lookup("Resources", oRes.get())->isDict())
			return false;
		CObjectGuard oRef;
		if (oAPi->streamGetDict()->lookupNF("Resources", oRef.get())->isRef() && std::find(arrUniqueResources.begin(), arrUniqueResources.end(), oRef->getRef().num) != arrUniqueResources.end())
			return false;
		arrUniqueResources.push_back(oRef->getRef().num);
		return scanCMapFonts(oRes->getDict(), 0, arrUniqueResources);
	};
	return fScanAPView(oAP.get(), "N") || fScanAPView(oAP.get(), "D") || fScanAPView(oAP.get(), "R");
}

double SafeStod(const std::string& s, double fallback = 0.0) noexcept
{
	if (s.empty()) return fallback;
	try { return std::stod(s); }
	catch (...) { return fallback; }
}
std::wstring ComputeFontHash(XRef* pXref, GfxFont* pFont)
{
	MD5State oMD5;
	md5Start(&oMD5);

	Ref* pRef = pFont->getID();
	CObjectGuard oFontObj;
	{
		CObjectGuard oRefObj;
		oRefObj->initRef(pRef->num, pRef->gen);
		oRefObj->fetch(pXref, oFontObj.get());
	}

	if (oFontObj->isDict())
	{
		const char* aFontObjNames[] = { "Name", "Subtype", "BaseFont", "Encoding" };
		for (const char* sKey : aFontObjNames)
		{
			CObjectGuard oItem;
			oFontObj->dictLookup(sKey, oItem.get());
			if (oItem->isName())
				md5Append(&oMD5, (BYTE*)oItem->getName(), strlen(oItem->getName()));
		}

		CObjectGuard oDescObj;
		oFontObj->dictLookup("FontDescriptor", oDescObj.get());
		if (!oDescObj->isDict())
		{
			oDescObj->free();
			CObjectGuard oDescendantFonts;
			if (oFontObj->dictLookup("DescendantFonts", oDescendantFonts.get())->isArray())
			{
				CObjectGuard oDescendant;
				if (oDescendantFonts->arrayGet(0, oDescendant.get())->isDict())
					oDescendant->dictLookup("FontDescriptor", oDescObj.get());
			}
		}

		if (oDescObj->isDict())
		{
			{
				CObjectGuard oItem;
				oDescObj->dictLookup("FontName", oItem.get());
				if (oItem->isName())
					md5Append(&oMD5, (BYTE*)oItem->getName(), strlen(oItem->getName()));
			}

			const char* aMetricNames[] =
			{
				"Flags", "ItalicAngle", "Ascent", "Descent",
				"CapHeight", "XHeight", "StemV", "StemH"
			};
			for (const char* sName : aMetricNames)
			{
				CObjectGuard oItem;
				oDescObj->dictLookup(sName, oItem.get());
				if (oItem->isInt())
				{
					int nVal = oItem->getInt();
					md5Append(&oMD5, (BYTE*)&nVal, sizeof(int));
				}
			}

			CObjectGuard oBBox;
			oDescObj->dictLookup("FontBBox", oBBox.get());
			if (oBBox->isArray() && oBBox->arrayGetLength() == 4)
			{
				for (int i = 0; i < 4; i++)
				{
					CObjectGuard oCoord;
					if (oBBox->arrayGet(i, oCoord.get())->isInt())
					{
						int nVal = oCoord->getInt();
						md5Append(&oMD5, (BYTE*)&nVal, sizeof(int));
					}
				}
			}
		}
	}

	Ref oEmbRef;
	if (pFont->getEmbeddedFontID(&oEmbRef))
	{
		CObjectGuard oStreamObj;
		{
			CObjectGuard oRefObj;
			oRefObj->initRef(oEmbRef.num, oEmbRef.gen);
			oRefObj->fetch(pXref, oStreamObj.get());
		}

		if (oStreamObj->isStream())
		{
			oStreamObj->streamReset();
			const int nMaxBytes = 512;
			BYTE aBuf[nMaxBytes];
			int nRead = 0;
			int nChar;
			while (nRead < nMaxBytes && (nChar = oStreamObj->streamGetChar()) != EOF)
				aBuf[nRead++] = (BYTE)nChar;

			oStreamObj->streamClose();
			md5Append(&oMD5, aBuf, nRead);
		}
	}

	md5Finish(&oMD5);

	const char aHexChars[] = "0123456789ABCDEF";
	std::string sHex;
	sHex.reserve(32);
	for (int i = 0; i < 16; i++)
	{
		sHex += aHexChars[(oMD5.digest[i] >> 4) & 0x0F];
		sHex += aHexChars[oMD5.digest[i] & 0x0F];
	}

	return UTF8_TO_U(sHex);
}
bool EraseSubsetTag(std::wstring& sFontName)
{
	if (sFontName.size() <= 7 || sFontName[6] != L'+')
		return false;

	for (size_t i = 0; i < 6; ++i)
	{
		if (sFontName[i] < L'A' || sFontName[i] > L'Z')
			return false;
	}

	sFontName.erase(0, 7);
	return true;
}
void ReadStyleToFontData(const std::string& sData, PdfReader::CFontData* pFont)
{
	if (!pFont || sData.empty())
		return;

	size_t nPos = 0;
	const size_t nLen = sData.size();

	while (nPos < nLen)
	{
		size_t nColonPos = sData.find(':', nPos);
		if (nColonPos == std::string::npos)
			break;

		std::string sProperty = sData.substr(nPos, nColonPos - nPos);

		size_t nSemiPos = sData.find(';', nColonPos + 1);
		size_t valueEnd = (nSemiPos == std::string::npos) ? nLen : nSemiPos;

		std::string sValue = sData.substr(nColonPos + 1, valueEnd - nColonPos - 1);

		auto trim = [](std::string& s)
		{
			s.erase(0, s.find_first_not_of(" \t\r\n"));
			s.erase(s.find_last_not_of(" \t\r\n") + 1);
		};
		trim(sProperty);
		trim(sValue);

		nPos = (nSemiPos == std::string::npos) ? nLen : nSemiPos + 1;

		if (sProperty.empty() || sValue.empty())
			continue;

		if (sProperty == "font-size")
			pFont->dFontSize = SafeStod(sValue);
		else if (sProperty == "text-align")
		{
			// 0 start / left
			if      (sValue == "center" || sValue == "middle") pFont->nAlign = 1;
			else if (sValue == "right"  || sValue == "end")    pFont->nAlign = 2;
			else if (sValue == "justify")                      pFont->nAlign = 3;
			else                                               pFont->nAlign = 0;
		}
		else if (sProperty == "color")
		{
			if (sValue[0] == '#')
			{
				std::string sHex = sValue.substr(1);
				unsigned int r = 0, g = 0, b = 0;

				if (sHex.size() == 6)
				{
					auto hexByte = [&](size_t i) -> unsigned int
					{
						unsigned int v = 0;
						sscanf(sHex.c_str() + i, "%2x", &v);
						return v;
					};
					r = hexByte(0); g = hexByte(2); b = hexByte(4);
				}
				else if (sHex.size() == 3)
				{
					auto hexNibble = [&](size_t i) -> unsigned int
					{
						unsigned int v = 0;
						sscanf(sHex.c_str() + i, "%1x", &v);
						return v * 17; // 0xN -> 0xNN
					};
					r = hexNibble(0); g = hexNibble(1); b = hexNibble(2);
				}

				pFont->dColor[0] = r / 255.0;
				pFont->dColor[1] = g / 255.0;
				pFont->dColor[2] = b / 255.0;
			}
		}
		else if (sProperty == "font-weight")
		{
			// 0 normal / 300 / 400 / 500
			int nWeight = SafeStod(sValue, -1);
			if (sValue == "bold"   || sValue == "bolder" || nWeight >= 600)
				pFont->unFontFlags |= PdfReader::CFontData::FontFlags::eFlagBold;
			else if (sValue == "normal" || sValue == "lighter" || (nWeight > 0 && nWeight <= 500))
				pFont->unFontFlags &= ~PdfReader::CFontData::FontFlags::eFlagBold;
		}
		else if (sProperty == "font-style")
		{
			if (sValue == "normal")
				pFont->unFontFlags &= ~PdfReader::CFontData::FontFlags::eFlagItalic;
			else if (sValue == "italic" || sValue.find("oblique") != std::string::npos)
				pFont->unFontFlags |= PdfReader::CFontData::FontFlags::eFlagItalic;
		}
		else if (sProperty == "font-family")
		{
			if (sValue.size() >= 2 && (sValue.front() == '\'' || sValue.front() == '\"') && sValue.back() == sValue.front())
				pFont->sFontFamily = sValue.substr(1, sValue.size() - 2);
			else
				pFont->sFontFamily = sValue;
		}
		else if (sProperty == "text-decoration")
		{
			if (sValue.find("line-through") != std::string::npos)
				pFont->unFontFlags |= PdfReader::CFontData::FontFlags::eFlagStrike;
			if (sValue.find("word") != std::string::npos || sValue.find("underline") != std::string::npos)
				pFont->unFontFlags |= PdfReader::CFontData::FontFlags::eFlagUnderline;
			if (sValue.find("none") != std::string::npos)
			{
				pFont->unFontFlags &= ~PdfReader::CFontData::FontFlags::eFlagStrike;
				pFont->unFontFlags &= ~PdfReader::CFontData::FontFlags::eFlagUnderline;
			}
		}
		else if (sProperty == "vertical-align")
		{
			pFont->unFontFlags |= PdfReader::CFontData::FontFlags::eFlagVAlign;
			pFont->dVAlign = SafeStod(sValue);
			if (pFont->dVAlign == 0 && sValue[0] == '-')
				pFont->dVAlign = -0.01;
		}
	}
}
bool CheckFontNameStyle (std::wstring& sName, const std::wstring& sStyle)
{
	std::wstring sLower = sName;
	NSStringExt::ToLower(sLower);

	const size_t nStyleLen = sStyle.size();
	bool bFound = false;
	size_t nPos = 0;

	while (std::wstring::npos != (nPos = sLower.find(sStyle, nPos)))
	{
		size_t nEraseLen = nStyleLen;
		if (nPos > 0 && (sLower[nPos - 1] == L'-' || sLower[nPos - 1] == L','))
		{
			--nPos;
			++nEraseLen;
		}

		sName.erase(nPos, nEraseLen);
		sLower.erase(nPos, nEraseLen);
		bFound = true;
	}
	return bFound;
}
double CheckFontStylePDF(std::wstring& sName, bool& bBold, bool& bItalic)
{
	EraseSubsetTag(sName);

	CheckFontNameStyle(sName, L"condensedbold");
	CheckFontNameStyle(sName, L"semibold");
	CheckFontNameStyle(sName, L"regular");

	double dStretch = 1.0;

	if      (CheckFontNameStyle(sName, L"ultraexpanded")) dStretch = 2.0;
	else if (CheckFontNameStyle(sName, L"extraexpanded")) dStretch = 1.5;
	else if (CheckFontNameStyle(sName, L"semiexpanded"))  dStretch = 1.125;
	else if (CheckFontNameStyle(sName, L"expanded"))      dStretch = 1.25;

	if      (CheckFontNameStyle(sName, L"ultracondensed")) dStretch = 0.5;
	else if (CheckFontNameStyle(sName, L"extracondensed")) dStretch = 0.625;
	else if (CheckFontNameStyle(sName, L"semicondensed"))  dStretch = 0.875;
	else if (CheckFontNameStyle(sName, L"condensedlight")) dStretch = 0.75;
	else if (CheckFontNameStyle(sName, L"condensed"))      dStretch = 0.75;
	//CheckFontNameStyle(sName, L"light");

	if (CheckFontNameStyle(sName, L"bold_italic"))  { bBold = true; bItalic = true; }
	if (CheckFontNameStyle(sName, L"bold_oblique")) { bBold = true; bItalic = true; }

	if (CheckFontNameStyle(sName, L"boldmt")) bBold = true;
	if (CheckFontNameStyle(sName, L"bold"))   bBold = true;

	if (CheckFontNameStyle(sName, L"italicmt")) bItalic = true;
	if (CheckFontNameStyle(sName, L"italic"))   bItalic = true;
	if (CheckFontNameStyle(sName, L"oblique"))  bItalic = true;

	//if (CheckFontNameStyle(sName, L"bolditalicmt")) { bBold = true; bItalic = true; }
	//if (CheckFontNameStyle(sName, L"bolditalic")) { bBold = true; bItalic = true; }
	//if (CheckFontNameStyle(sName, L"boldoblique")) { bBold = true; bItalic = true; }

	return dStretch;
}
double CheckFontNamePDF (std::wstring& sName, NSFonts::CFontSelectFormat* format)
{
	bool bBold   = false;
	bool bItalic = false;
	double dStretch = CheckFontStylePDF(sName, bBold, bItalic);
	if (format)
	{
		if (bBold)   format->bBold   = new INT(1);
		if (bItalic) format->bItalic = new INT(1);
	}
	return dStretch;
}
int GetAnnotFontNamePenalty(const std::wstring& sCandNorm, const std::wstring& sReqNorm)
{
	if (sReqNorm.empty())
		return 0;
	if (sCandNorm.empty())
		return 10000;

	if (sCandNorm == sReqNorm)
		return 0;

	bool bCandInReq = sReqNorm.find(sCandNorm) != std::wstring::npos;
	bool bReqInCand = sCandNorm.find(sReqNorm) != std::wstring::npos;
	if (bCandInReq || bReqInCand)
		return 500;

	return 10000;
}
std::wstring Normalize(const std::wstring& wsName)
{
	std::wstring s = wsName;
	bool bIsRemove = false;
	size_t lastSpace = s.find_last_of(L' ');
	if (lastSpace != std::wstring::npos)
	{
		bIsRemove = true;
		for (size_t nIndex = lastSpace + 1; nIndex < s.size(); ++nIndex)
		{
			wchar_t nChar = s.at(nIndex);
			if (nChar < L'0' || nChar > L'F' || (nChar > L'9' && nChar < L'A'))
			{
				bIsRemove = false;
				break;
			}
		}
		if (bIsRemove)
			s.erase(lastSpace + 1);
	}

	bool bDummy1 = false, bDummy2 = false;
	CheckFontStylePDF(s, bDummy1, bDummy2);

	NSStringExt::ToLower(s);
	s.erase(std::remove_if(s.begin(), s.end(), [](wchar_t c){ return c == L' ' || c == L'-' || c == L'_'; }), s.end());
	return s;
};
USHORT StretchToWidthClass(double fStretch)
{
	if (fStretch <= 0.50)  return 1; // Ultra-condensed
	if (fStretch <= 0.625) return 2; // Extra-condensed
	if (fStretch <= 0.75)  return 3; // Condensed
	if (fStretch <= 0.875) return 4; // Semi-condensed
	if (fStretch <= 1.0)   return 5; // Normal
	if (fStretch <= 1.125) return 6; // Semi-expanded
	if (fStretch <= 1.25)  return 7; // Expanded
	if (fStretch <= 1.50)  return 8; // Extra-expanded
	return 9;                        // Ultra-expanded
}
Ref FindMatchInAnnotFonts(PdfReader::CPdfFontList *pFontList, const std::vector<Ref>& annotFonts, const std::wstring& wsRCName, bool bBold, bool bItalic)
{
	std::wstring wsReqNorm = Normalize(wsRCName);

	Ref pBestMatch = { -1, -1 };
	int nBestPenalty = INT_MAX;

	for (Ref ri : annotFonts)
	{
		PdfReader::TFontEntry fi;
		pFontList->GetFont(&ri, &fi);

		int nNamePenalty = GetAnnotFontNamePenalty(Normalize(fi.wsFontName), wsReqNorm);
		if (!fi.wsActualFontName.empty())
		{
			int nActualPenalty = GetAnnotFontNamePenalty(Normalize(fi.wsActualFontName), wsReqNorm);
			if (nActualPenalty < nNamePenalty)
				nNamePenalty = nActualPenalty;
		}

		if (nNamePenalty >= 5000)
			continue;

		int nBoldPenalty   = (fi.bBold   != bBold)   ? 1 : 0;
		int nItalicPenalty = (fi.bItalic != bItalic) ? 4 : 0;

		int nTotalPenalty = nNamePenalty + nBoldPenalty + nItalicPenalty;
		if (nTotalPenalty < nBestPenalty)
		{
			nBestPenalty = nTotalPenalty;
			pBestMatch   = ri;
		}

		if (nTotalPenalty == 0)
			break;
	}

	return pBestMatch;
}
std::map<std::wstring, std::wstring> GetAllFreeTextFonts(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, PdfReader::CPdfFontList* pFontList)
{
	std::map<std::wstring, std::wstring> mFonts;
	for (int nPage = 0, nLastPage = pdfDoc->getNumPages(); nPage < nLastPage; ++nPage)
	{
		Page* pPage = pdfDoc->getCatalog()->getPage(nPage + 1);
		if (!pPage)
			continue;

		CObjectGuard oAnnots;
		if (!pPage->getAnnots(oAnnots.get())->isArray())
			continue;

		for (int i = 0, nNum = oAnnots->arrayGetLength(); i < nNum; ++i)
		{
			CObjectGuard oAnnot;
			if (!oAnnots->arrayGet(i, oAnnot.get())->isDict())
				continue;

			CObjectGuard oSubtype;
			if (!oAnnot->dictLookup("Subtype", oSubtype.get())->isName("FreeText"))
				continue;

			std::string sRC;
			CObjectGuard oObj;
			if (oAnnot->dictLookup("RC", oObj.get())->isString())
			{
				TextString* s = new TextString(oObj->getString());
				sRC = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
				delete s;
			}
			else
			{
				oObj->free();
				if (!oAnnot->dictLookup("Contents", oObj.get())->isString() || !oObj->getString()->getLength())
					continue;

				std::string sDS;
				{
					CObjectGuard oDS;
					if (oAnnot->dictLookup("DS", oDS.get())->isString())
					{
						TextString* s = new TextString(oDS->getString());
						sDS = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
						delete s;
					}
				}

				sRC = PdfReader::GetRCFromDS(sDS, oObj.get(), {});
				if (sRC.find("font-family:Helvetica") != std::string::npos)
				{
					const BYTE* pData14 = NULL;
					unsigned int nSize14 = 0;
					std::wstring wsFontName = L"Helvetica";
					NSFonts::IFontsMemoryStorage* pMemoryStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
					if (pMemoryStorage && !pMemoryStorage->Get(wsFontName) && PdfReader::GetBaseFont(wsFontName, pData14, nSize14))
						pMemoryStorage->Add(wsFontName, (BYTE*)pData14, nSize14, false);
					mFonts[L"Helvetica"] = L"Helvetica";
				}
			}

			CObjectGuard oAnnotRef;
			oAnnots->arrayGetNF(i, oAnnotRef.get());
			std::vector<PdfReader::CFontData*> arrRC = PdfReader::ReadRC(sRC);
			std::map<std::wstring, std::wstring> mFreeText = PdfReader::GetFreeTextFontsRC(pdfDoc, pFontManager, pFontList, oAnnotRef.get(), arrRC);
			mFonts.insert(mFreeText.begin(), mFreeText.end());

			for (int j = 0; j < arrRC.size(); ++j)
				RELEASEOBJECT(arrRC[j]);
		}
	}
	return mFonts;
}
std::map<std::wstring, std::wstring> GetAllWidgetFonts  (PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, PdfReader::CPdfFontList* pFontList)
{
	std::map<std::wstring, std::wstring> mFonts;
	AcroForm* pAcroForms = pdfDoc->getCatalog()->getForm();
	if (!pAcroForms)
		return mFonts;

	std::vector<int> arrUniqueFontsRef;
	auto fnAddFont = [&](Object* pFontRef)
	{
		GetFontData(pdfDoc, pFontManager, pFontList, pFontRef);
		arrUniqueFontsRef.push_back(pFontRef->getRefNum());

		PdfReader::TFontEntry oEntry;
		Ref nFontRef = pFontRef->getRef();
		pFontList->GetFont(&nFontRef, &oEntry);

		if (oEntry.wsActualFontName.empty())
			mFonts.emplace(oEntry.wsFontName, oEntry.wsFilePath);
	};

	for (int nField = 0, nNum = pAcroForms->getNumFields(); nField < nNum; ++nField)
	{
		AcroFormField* pField = pAcroForms->getField(nField);
		if (!pField)
			continue;

		// Font and font size - from DA
		Ref fontID;
		double dFontSize = 0;
		pField->getFont(&fontID, &dFontSize);

		CObjectGuard oFontRef;
		if (fontID.num < 0)
		{
			std::string sFontKey;
			if (!PdfReader::GetFontFromAP(pdfDoc, pField, oFontRef.get(), sFontKey))
				continue;
		}
		else
			oFontRef->initRef(fontID.num, fontID.gen);

		if (std::find(arrUniqueFontsRef.begin(), arrUniqueFontsRef.end(), oFontRef->getRefNum()) != arrUniqueFontsRef.end())
			continue;

		fnAddFont(oFontRef.get());
		oFontRef->free();

		if (pField->getAcroFormFieldType() == acroFormFieldPushbutton && fontID.num >= 0)
		{
			std::string sFontKey;
			if (PdfReader::GetFontFromAP(pdfDoc, pField, oFontRef.get(), sFontKey) && std::find(arrUniqueFontsRef.begin(), arrUniqueFontsRef.end(), oFontRef->getRefNum()) == arrUniqueFontsRef.end())
				fnAddFont(oFontRef.get());
		}
	}
	return mFonts;
}

struct CMemoryFontStream
{
	BYTE* m_pData;
	int m_nSize;
	int m_nPos;
	bool m_bIsAttach;

	CMemoryFontStream()
	{
		m_pData = NULL;
		m_nSize = 0;
		m_nPos = 0;
		m_bIsAttach = false;
	}
	~CMemoryFontStream()
	{
		if (NULL != m_pData && !m_bIsAttach)
			RELEASEARRAYOBJECTS(m_pData);
	}

	void fromStream(std::wstring& sStreamName)
	{
		NSFonts::IFontStream* pStream = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Get(sStreamName);
		if (pStream)
		{
			LONG lSize = 0;
			pStream->GetMemory(m_pData, lSize);
			m_nSize = (int)lSize;
			m_nPos = 0;
			m_bIsAttach = true;
		}
	}
	void fromBuffer(BYTE* pData, int nSize)
	{
		if (pData)
		{
			m_pData = pData;
			m_nSize = nSize;
			m_nPos = 0;
			m_bIsAttach = true;
		}
	}

	void load(Object& oStreamObject)
	{
		int nCurrentSize = 0xFFFF;
		int nCurrentPos = 0;
		BYTE* pStream = new BYTE[nCurrentSize];

		int nChar;
		while ((nChar = oStreamObject.streamGetChar()) != EOF)
		{
			if (nCurrentPos >= nCurrentSize)
			{
				int nNewSize = 2 * nCurrentSize;
				BYTE* pNewBuffer = new BYTE[nNewSize];
				memcpy(pNewBuffer, pStream, nCurrentSize);
				RELEASEARRAYOBJECTS(pStream);
				pStream = pNewBuffer;
				nCurrentSize = nNewSize;
			}
			pStream[nCurrentPos++] = nChar;
		}

		m_pData = pStream;
		m_nSize = nCurrentPos;
		m_nPos = 0;
	}
	int getChar()
	{
		if (m_nPos >= m_nSize)
			return EOF;
		return m_pData[m_nPos++];
	}
	void toStart()
	{
		m_nPos = 0;
	}
};
int readFromMemoryStream(void* data)
{
	return ((CMemoryFontStream*)data)->getChar();
}
}

namespace PdfReader
{
std::map<std::wstring, std::wstring> GetAllFonts(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList)
{
	std::map<std::wstring, std::wstring> mFonts = GetAllWidgetFonts(pdfDoc, pFontManager, pFontList);

	std::map<std::wstring, std::wstring> mFreeText = GetAllFreeTextFonts(pdfDoc, pFontManager, pFontList);
	mFonts.insert(mFreeText.begin(), mFreeText.end());

	return mFonts;
}

bool IsNeedCMap(PDFDoc* pDoc)
{
	std::vector<int> arrUniqueResources;
	for (int nPage = 1, nLastPage = pDoc->getNumPages(); nPage <= nLastPage; ++nPage)
	{
		Page* pPage = pDoc->getCatalog()->getPage(nPage);
		Dict* pResources = pPage->getResourceDict();
		if (pResources && scanCMapFonts(pResources, 0, arrUniqueResources))
			return true;

		CObjectGuard oAnnots;
		if (!pPage->getAnnots(oAnnots.get())->isArray())
			continue;

		for (int i = 0, nNum = oAnnots->arrayGetLength(); i < nNum; ++i)
		{
			CObjectGuard oAnnot;
			if (!oAnnots->arrayGet(i, oAnnot.get())->isDict())
				continue;

			CObjectGuard oRefDR;
			if (!oAnnot->dictLookupNF("DR", oRefDR.get())->isRef() || std::find(arrUniqueResources.begin(), arrUniqueResources.end(), oRefDR->getRef().num) == arrUniqueResources.end())
			{
				arrUniqueResources.push_back(oRefDR->getRef().num);
				CObjectGuard oDR;
				if (oAnnot->dictLookup("DR", oDR.get())->isDict() && scanCMapFonts(oDR->getDict(), 0, arrUniqueResources))
					return true;
			}

			if (scanCMapAPFonts(oAnnot.get(), arrUniqueResources))
				return true;
		}
	}

	AcroForm* pAcroForms = pDoc->getCatalog()->getForm();
	if (!pAcroForms)
		return false;
	Object* oAcroForm = pAcroForms->getAcroFormObj();

	CObjectGuard oRefDR;
	if (!oAcroForm->dictLookupNF("DR", oRefDR.get())->isRef() || std::find(arrUniqueResources.begin(), arrUniqueResources.end(), oRefDR->getRef().num) == arrUniqueResources.end())
	{
		arrUniqueResources.push_back(oRefDR->getRef().num);
		CObjectGuard oDR;
		if (oAcroForm->dictLookup("DR", oDR.get())->isDict() && scanCMapFonts(oDR->getDict(), 0, arrUniqueResources))
			return true;
	}

	for (int i = 0, nNum = pAcroForms->getNumFields(); i < nNum; ++i)
	{
		AcroFormField* pField = pAcroForms->getField(i);
		{
			CObjectGuard oDR;
			if (pField->getResources(oDR.get())->isDict() && scanCMapFonts(oDR->getDict(), 0, arrUniqueResources))
				return true;
		}

		CObjectGuard oWidget;
		{
			CObjectGuard oWidgetRef;
			pField->getFieldRef(oWidgetRef.get());
			oWidgetRef->fetch(pDoc->getXRef(), oWidget.get());
		}

		if (scanCMapAPFonts(oWidget.get(), arrUniqueResources))
			return true;
	}

	return false;
}
bool IsBaseFont(const std::wstring& wsName)
{
	return wsName == L"Courier" || wsName == L"Courier-Bold" || wsName == L"Courier-BoldOblique" || wsName == L"Courier-Oblique" ||
		   wsName == L"Helvetica" || wsName == L"Helvetica-Bold" || wsName == L"Helvetica-BoldOblique" ||
		   wsName == L"Helvetica-Oblique" || wsName == L"Symbol" || wsName == L"Times-Bold" || wsName == L"Times-BoldItalic" ||
		   wsName == L"Times-Italic" || wsName == L"Times-Roman" || wsName == L"ZapfDingbats";
}

void GetFontData(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList, Object* oFontRef)
{
	XRef* xref = pdfDoc->getXRef();
	Object oFont;
	if (!xref->fetch(oFontRef->getRefNum(), oFontRef->getRefGen(), &oFont)->isDict())
	{
		oFont.free();
		return;
	}

	GfxFont* gfxFont = GfxFont::makeFont(xref, "F", oFontRef->getRef(), oFont.getDict());
	oFont.free();
	if (!gfxFont)
		return;

	std::wstring wsFileName, wsFontName;
	GetFont(xref, pFontManager, pFontList, gfxFont, wsFileName, wsFontName);

	RELEASEOBJECT(gfxFont);
}
bool GetFontFromAP(PDFDoc* pdfDoc, AcroFormField* pField, Object* oFontRef, std::string& sFontKey)
{
	bool bFindResources = false;

	CObjectGuard oAP, oN;
	XRef* xref = pdfDoc->getXRef();
	if (!pField->fieldLookup("AP", oAP.get())->isDict() || !oAP->dictLookup("N", oN.get())->isStream())
		return false;

	Parser* parser = new Parser(xref, new Lexer(xref, oN.get()), gFalse);

	bool bFindFont = false;
	CObjectGuard oObj1, oObj2, oObj3;
	parser->getObj(oObj1.get());
	while (!oObj1->isEOF())
	{
		if (oObj1->isName())
		{
			parser->getObj(oObj2.get());
			if (oObj2->isEOF())
				break;
			if (oObj2->isNum())
			{
				parser->getObj(oObj3.get());
				if (oObj3->isEOF())
					break;
				if (oObj3->isCmd("Tf"))
				{
					bFindFont = true;
					break;
				}
			}
		}
		if (oObj2->isName())
		{
			oObj1->free();
			oObj2->copy(oObj1.get());
			oObj2->free(); oObj3->free();
			continue;
		}
		if (oObj3->isName())
		{
			oObj1->free();
			oObj3->copy(oObj1.get());
			oObj3->free(); oObj2->free();
			continue;
		}
		oObj1->free(); oObj2->free(); oObj3->free();

		parser->getObj(oObj1.get());
	}

	if (bFindFont && oObj1->isName())
	{
		CObjectGuard oR, oFonts;
		bFindResources = oN->streamGetDict()->lookup("Resources", oR.get())->isDict() && oR->dictLookup("Font", oFonts.get())->isDict() && oFonts->dictLookupNF(oObj1->getName(), oFontRef)->isRef();
		sFontKey = oObj1->getName();
	}

	RELEASEOBJECT(parser);
	return bFindResources;
}
bool FindFonts(Object* oStream, int nDepth, Object* oResFonts)
{
	if (nDepth > 5)
		return false;

	CObjectGuard oResources;
	if (!oStream->streamGetDict()->lookup("Resources", oResources.get())->isDict())
		return false;

	if (oResources->dictLookup("Font", oResFonts)->isDict())
		return true;

	CObjectGuard oXObject;
	if (oResources->dictLookup("XObject", oXObject.get())->isDict())
	{
		for (int i = 0, nLength = oXObject->dictGetLength(); i < nLength; ++i)
		{
			CObjectGuard oXObj;
			if (!oXObject->dictGetVal(i, oXObj.get())->isStream())
				continue;
			if (FindFonts(oXObj.get(), nDepth + 1, oResFonts))
				return true;
		}
	}
	return false;
}

std::vector<CFontData*> ReadRC(const std::string& sRC)
{
	std::vector<CFontData*> arrRC;
	if (sRC.empty())
		return arrRC;

	XmlUtils::CXmlLiteReader oLightReader;
	if (sRC.empty() || !oLightReader.FromStringA(sRC) || !oLightReader.ReadNextNode() || oLightReader.GetNameA() != "body")
		return arrRC;

	CFontData oFontBase;
	while (oLightReader.MoveToNextAttribute())
	{
		if (oLightReader.GetNameA() == "style")
		{
			ReadStyleToFontData(oLightReader.GetTextA(), &oFontBase);
			break;
		}
	}
	oLightReader.MoveToElement();

	int nDepthP = oLightReader.GetDepth();
	while (oLightReader.ReadNextSiblingNode2(nDepthP))
	{
		if (oLightReader.GetNameA() != "p")
			continue;

		bool bRTL = false;
		while (oLightReader.MoveToNextAttribute())
		{
			if (oLightReader.GetNameA() == "dir" && oLightReader.GetTextA() == "rtl")
			{
				bRTL = true;
				break;
			}
		}
		oLightReader.MoveToElement();

		int nDepthSpan = oLightReader.GetDepth();
		if (oLightReader.IsEmptyNode() || !oLightReader.ReadNextSiblingNode2(nDepthSpan))
			continue;

		do
		{
			std::string sName = oLightReader.GetNameA();
			if (sName == "span")
			{
				CFontData* pFont = new CFontData(oFontBase);
				while (oLightReader.MoveToNextAttribute())
				{
					if (oLightReader.GetNameA() == "style")
					{
						ReadStyleToFontData(oLightReader.GetTextA(), pFont);
						break;
					}
				}
				oLightReader.MoveToElement();

				if (bRTL)
					pFont->unFontFlags |= PdfReader::CFontData::FontFlags::eFlagRTL;
				pFont->sText = oLightReader.GetText2A();
				arrRC.push_back(pFont);
			}
			else if (sName == "#text")
			{
				CFontData* pFont = new CFontData(oFontBase);
				if (bRTL)
					pFont->unFontFlags |= PdfReader::CFontData::FontFlags::eFlagRTL;
				pFont->sText = oLightReader.GetTextA();
				arrRC.push_back(pFont);
			}
		} while (oLightReader.ReadNextSiblingNode2(nDepthSpan));
	}

	return arrRC;
}
std::string GetRCFromDS(const std::string& sDS, Object* pContents, const std::vector<double>& arrCFromDA)
{
	if (!pContents || !pContents->isString())
		return "";

	NSStringUtils::CStringBuilder oRC;
	oRC += L"<?xml version=\"1.0\"?><body xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:xfa=\"http://www.xfa.org/schema/xfa-data/1.0/\" xfa:APIVersion=\"Acrobat:23.8.0\"  xfa:spec=\"2.0.2\"><p dir=\"ltr\"><span style=\"";

	auto appendIfMissing = [&](const char* prop, const wchar_t* value)
	{
		if (sDS.find(prop) == std::string::npos)
			oRC += value;
	};

	appendIfMissing("font-family",     L"font-family:Helvetica;");
	appendIfMissing("font-size",       L"font-size:14.0pt;");
	appendIfMissing("text-align",      L"text-align:left;");
	appendIfMissing("font-weight",     L"font-weight:normal;");
	appendIfMissing("font-style",      L"font-style:normal;");
	appendIfMissing("text-decoration", L"text-decoration:none;");

	if (sDS.find("color") == std::string::npos)
	{
		oRC += L"color:";
		if (arrCFromDA.size() == 3)
		{
			auto toColorByte = [](double v) -> unsigned char
			{
				v = std::max(0.0, std::min(1.0, v));
				return static_cast<unsigned char>(v * 255.0);
			};
			oRC.WriteHexColor3(toColorByte(arrCFromDA[0] * 255.0),
							   toColorByte(arrCFromDA[1] * 255.0),
							   toColorByte(arrCFromDA[2] * 255.0));
		}
		else
			oRC += L"#000000";
		oRC += L";";
	}
	oRC += (UTF8_TO_U(sDS));

	oRC += L"\">";
	TextString* s = new TextString(pContents->getString());
	std::wstring wsContents = NSStringExt::CConverter::GetUnicodeFromUTF32(s->getUnicode(), s->getLength());
	delete s;
	oRC.WriteEncodeXmlString(wsContents);
	oRC += L"</span></p></body>";

	std::wstring wsRC = oRC.GetData();
	return U_TO_UTF8(wsRC);
}
std::map<std::wstring, std::wstring> GetFreeTextFontsRC(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, Object* oAnnotRef, std::vector<CFontData*>& arrRC)
{
	std::map<std::wstring, std::wstring> mRes;

	std::vector<Ref> annotFonts = GetFreeTextFonts(pdfDoc, pFontManager, pFontList, oAnnotRef);

	for (int i = 0; i < (int)arrRC.size(); ++i)
	{
		if (arrRC[i]->bFind)
			continue;

		std::string sFontName = arrRC[i]->sFontFamily;
		std::wstring wsFontName = UTF8_TO_U(sFontName);
		bool bBold   = (bool)((arrRC[i]->unFontFlags >> 0) & 1);
		bool bItalic = (bool)((arrRC[i]->unFontFlags >> 1) & 1);

		if (IsBaseFont(wsFontName))
		{
			if (sFontName == "Times-Roman")
			{
				if (bBold && bItalic) sFontName = "Times-BoldItalic";
				else if (bBold)       sFontName = "Times-Bold";
				else if (bItalic)     sFontName = "Times-Italic";
			}
			else if (sFontName == "Courier" || sFontName == "Helvetica")
			{
				if (bBold && bItalic) sFontName += "-BoldOblique";
				else if (bBold)       sFontName += "-Bold";
				else if (bItalic)     sFontName += "-Oblique";
			}
			wsFontName = UTF8_TO_U(sFontName);

			NSFonts::IFontsMemoryStorage* pMemoryStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
			if (pMemoryStorage && !pMemoryStorage->Get(wsFontName))
			{
				const unsigned char* pData14 = NULL;
				unsigned int nSize14 = 0;
				if (GetBaseFont(wsFontName, pData14, nSize14))
					pMemoryStorage->Add(wsFontName, (BYTE*)pData14, nSize14, false);
			}

			std::string sFontNameBefore = arrRC[i]->sFontFamily;
			arrRC[i]->sFontFamily = sFontName;
			arrRC[i]->bFind = true;
			mRes[wsFontName] = wsFontName;

			for (int j = i; j < (int)arrRC.size(); ++j)
			{
				if (arrRC[j]->sFontFamily == sFontNameBefore &&
					bBold   == (bool)((arrRC[j]->unFontFlags >> 0) & 1) &&
					bItalic == (bool)((arrRC[j]->unFontFlags >> 1) & 1))
				{
					arrRC[j]->sFontFamily = sFontName;
					arrRC[j]->bFind = true;
				}
			}
		}
		else
		{
			Ref nMatch = FindMatchInAnnotFonts(pFontList, annotFonts, wsFontName, bBold, bItalic);
			TFontEntry pMatch;
			pFontList->GetFont(&nMatch, &pMatch);

			std::wstring wsResolvedName;
			std::wstring wsResolvedPath;
			bool bFoundInAnnot = false;

			if (nMatch.num != -1)
			{
				wsResolvedPath = pMatch.wsFilePath;
				wsResolvedName = pMatch.wsActualFontName.empty() ? pMatch.wsFontName : pMatch.wsActualFontName;
				EraseSubsetTag(wsResolvedName);
				bFoundInAnnot = true;
			}
			else
			{
				NSFonts::CFontSelectFormat oFontSelect;
				if (bBold)   oFontSelect.bBold   = new INT(1);
				if (bItalic) oFontSelect.bItalic = new INT(1);
				oFontSelect.wsName = new std::wstring(wsFontName);

				NSFonts::CFontInfo* pFontInfo = pFontManager->GetFontInfoByParams(oFontSelect);
				if (pFontInfo && !pFontInfo->m_wsFontPath.empty())
				{
					wsResolvedPath = pFontInfo->m_wsFontPath;
					wsResolvedName = pFontInfo->m_wsFontName;
					EraseSubsetTag(wsResolvedName);
				}
			}

			if (!wsResolvedName.empty())
			{
				if (bFoundInAnnot)
				{
					arrRC[i]->sFontFamily = U_TO_UTF8(wsResolvedName);
					mRes[wsResolvedName]  = wsResolvedPath;
				}
				else
				{
					arrRC[i]->unFontFlags |= (1 << 6);
					arrRC[i]->sActualFont = U_TO_UTF8(wsResolvedName);
				}
				arrRC[i]->bFind = true;

				std::string sFontNameNew = bFoundInAnnot ? arrRC[i]->sFontFamily : arrRC[i]->sActualFont;
				for (int j = i; j < (int)arrRC.size(); ++j)
				{
					if (arrRC[j]->sFontFamily == sFontName &&
						bBold   == (bool)((arrRC[j]->unFontFlags >> 0) & 1) &&
						bItalic == (bool)((arrRC[j]->unFontFlags >> 1) & 1))
					{
						if (bFoundInAnnot)
							arrRC[j]->sFontFamily = sFontNameNew;
						else
						{
							arrRC[j]->unFontFlags |= (1 << 6);
							arrRC[j]->sActualFont = sFontNameNew;
						}
						arrRC[j]->bFind = true;
					}
				}
			}
		}
	}

	return mRes;
}
std::vector<Ref> GetFreeTextFonts(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, Object* oAnnotRef)
{
	std::vector<Ref> result;

	XRef* pXref = pdfDoc->getXRef();
	CObjectGuard oAnnot;
	oAnnotRef->fetch(pXref, oAnnot.get());

	CObjectGuard oN;
	{
		CObjectGuard oAP;
		if (!oAnnot->dictLookup("AP", oAP.get())->isDict() || !oAP->dictLookup("N", oN.get())->isStream())
			return result;
	}

	CObjectGuard oFonts;
	if (!FindFonts(oN.get(), 0, oFonts.get()))
		return result;

	for (int i = 0, nFonts = oFonts->dictGetLength(); i < nFonts; ++i)
	{
		CObjectGuard oFontRef;
		if (!oFonts->dictGetValNF(i, oFontRef.get())->isRef())
			continue;

		GetFontData(pdfDoc, pFontManager, pFontList, oFontRef.get());

		TFontEntry pEntry;
		Ref nFontRef = oFontRef->getRef();
		pFontList->GetFont(&nFontRef, &pEntry);

		if (pEntry.wsFilePath.empty() || IsBaseFont(pEntry.wsFilePath))
			continue;

		result.push_back(oFontRef->getRef());
	}

	return result;
}

int CollectFontWidths(Dict* pFontDict, std::map<unsigned int, unsigned int>& mGIDToWidth)
{
	int nDefaultWidth = 1000;
	// Widths array (simple fonts)
	{
		CObjectGuard oWidths;
		if (pFontDict->lookup("Widths", oWidths.get())->isArray())
		{
			int nFirstChar = 0;
			CObjectGuard oFirstChar;
			if (pFontDict->lookup("FirstChar", oFirstChar.get())->isInt())
				nFirstChar = oFirstChar->getInt();

			for (int i = 0; i < oWidths->arrayGetLength(); ++i)
			{
				CObjectGuard oWidth;
				if (oWidths->arrayGet(i, oWidth.get())->isNum())
				{
					unsigned int nGID = nFirstChar + i;
					mGIDToWidth[nGID] = (unsigned int)oWidth->getNum();
				}
			}
		}
	}

	// CID fonts: DW + W
	CObjectGuard oDescendantFonts;
	if (!pFontDict->lookup("DescendantFonts", oDescendantFonts.get())->isArray() || oDescendantFonts->arrayGetLength() <= 0)
		return nDefaultWidth;

	CObjectGuard oCIDFont;
	if (!oDescendantFonts->arrayGet(0, oCIDFont.get())->isDict())
		return nDefaultWidth;

	{
		CObjectGuard oDW;
		if (oCIDFont->dictLookup("DW", oDW.get())->isInt())
			nDefaultWidth = oDW->getInt();
	}

	CObjectGuard oW;
	if (!oCIDFont->dictLookup("W", oW.get())->isArray())
		return nDefaultWidth;

	int i = 0;
	while (i < oW->arrayGetLength())
	{
		CObjectGuard oStart;
		if (!oW->arrayGet(i, oStart.get())->isInt())
			break;
		if (i + 1 >= oW->arrayGetLength())
			break;

		CObjectGuard oSecond;
		oW->arrayGet(i + 1, oSecond.get());

		if (oSecond->isArray())
		{
			// Format: c [w1 w2 ... wn]
			int nStartCID = oStart->getInt();
			for (int j = 0; j < oSecond->arrayGetLength(); ++j)
			{
				CObjectGuard oWidth;
				if (oSecond->arrayGet(j, oWidth.get())->isNum())
					mGIDToWidth[nStartCID + j] = (unsigned int)oWidth->getNum();
			}
			i += 2;
		}
		else if (oSecond->isInt())
		{
			// Format: cfirst clast w
			if (i + 2 >= oW->arrayGetLength())
				break;

			CObjectGuard oThird;
			oW->arrayGet(i + 2, oThird.get());
			if (oThird->isNum())
			{
				int nStartCID = oStart->getInt();
				int nEndCID = oSecond->getInt();
				unsigned int nWidth = (unsigned int)oThird->getNum();
				for (int cid = nStartCID; cid <= nEndCID; ++cid)
					mGIDToWidth[cid] = nWidth;
			}
			i += 3;
		}
		else
		{
			break;
		}
	}

	return nDefaultWidth;
}

NSFonts::CFontInfo* GetFontByParams(XRef* pXref, NSFonts::IFontManager* pFontManager, GfxFont* pFont, std::wstring& wsFontBaseName, double& dStretch)
{
	NSFonts::CFontInfo* pFontInfo = NULL;
	if (!pFontManager)
		return pFontInfo;

	Ref* pRef = pFont->getID();
	Object oRefObject, oFontObject;
	oRefObject.initRef(pRef->num, pRef->gen);
	oRefObject.fetch(pXref, &oFontObject);
	oRefObject.free();

	NSFonts::CFontSelectFormat oFontSelect;
	dStretch = CheckFontNamePDF(wsFontBaseName, &oFontSelect);
	if (std::abs(dStretch - 1.0f) > 1e-5f)
		oFontSelect.usWidth = new USHORT(StretchToWidthClass(dStretch));
	if (oFontObject.isDict())
	{
		Dict* pFontDict = oFontObject.getDict();
		Object oFontDescriptor, oDescendantFonts;
		pFontDict->lookup("FontDescriptor", &oFontDescriptor);
		if (!oFontDescriptor.isDict() && pFontDict->lookup("DescendantFonts", &oDescendantFonts)->isArray())
		{
			oFontDescriptor.free(); oFontObject.free();
			if (oDescendantFonts.arrayGet(0, &oFontObject)->isDict())
				oFontObject.dictLookup("FontDescriptor", &oFontDescriptor);
		}
		if (oFontDescriptor.isDict())
		{
			Object oDictItem;
			oFontDescriptor.dictLookup("FontName", &oDictItem);
			if (oDictItem.isName())
				oFontSelect.wsName = AStringToPWString(oDictItem.getName());
			else
				oFontSelect.wsName = new std::wstring(wsFontBaseName);
			oDictItem.free();

			oFontDescriptor.dictLookup("FontFamily", &oDictItem);
			if (oDictItem.isString())
			{
				TextString* s = new TextString(oDictItem.getString());
				oFontSelect.wsAltName = new std::wstring(NSStringExt::CConverter::GetUnicodeFromUTF32(s->getUnicode(), s->getLength()));
				delete s;
			}
			oDictItem.free();

			oFontDescriptor.dictLookup("FontStretch", &oDictItem);
			oDictItem.free();

			oFontDescriptor.dictLookup("FontWeight", &oDictItem);
			oDictItem.free();

			oFontDescriptor.dictLookup("FontBBox", &oDictItem);
			oDictItem.free();

			oFontDescriptor.dictLookup("Flags", &oDictItem);
			if (oDictItem.isInt() && 0 != oDictItem.getInt())
			{
				int nFlags = oDictItem.getInt();
				if (nFlags & 1) // monospaced
					oFontSelect.bFixedWidth = new INT(1);
			}
			oDictItem.free();

			oFontDescriptor.dictLookup("ItalicAngle", &oDictItem);
			if (oDictItem.isInt() && 0 != oDictItem.getInt())
			{
				if (oFontSelect.bItalic) RELEASEOBJECT(oFontSelect.bItalic);
				oFontSelect.bItalic = new INT(1);
			}
			oDictItem.free();

			oFontDescriptor.dictLookup("Ascent", &oDictItem);
			if (oDictItem.isInt()) oFontSelect.shAscent = new SHORT(oDictItem.getInt());
			oDictItem.free();

			oFontDescriptor.dictLookup("Leading", &oDictItem);
			if (oDictItem.isInt()) oFontSelect.shLineGap = new SHORT(oDictItem.getInt());
			oDictItem.free();

			oFontDescriptor.dictLookup("CapHeight", &oDictItem);
			if (oDictItem.isInt()) oFontSelect.shCapHeight = new SHORT(oDictItem.getInt());
			oDictItem.free();

			oFontDescriptor.dictLookup("XHeight", &oDictItem);
			if (oDictItem.isInt()) oFontSelect.shXHeight = new SHORT(oDictItem.getInt());
			oDictItem.free();

			oFontDescriptor.dictLookup("StemV", &oDictItem);
			if (oDictItem.isNum() && !oFontSelect.usWidth)
			{
				double dStemV = oDictItem.getNum();
				if (dStemV > 50.5)
					oFontSelect.usWeight = new USHORT(sqrt(oDictItem.getNum() - 50.5) * 65);
			}
			oDictItem.free();

			oFontDescriptor.dictLookup("StemH", &oDictItem);
			oDictItem.free();

			oFontDescriptor.dictLookup("Descent", &oDictItem);
			if (oDictItem.isInt()) oFontSelect.shDescent = new SHORT(oDictItem.getInt());
			oDictItem.free();

			oFontDescriptor.dictLookup("AvgWidth", &oDictItem);
			if (oDictItem.isInt()) oFontSelect.shAvgCharWidth = new SHORT(oDictItem.getInt());
			oDictItem.free();

			oFontDescriptor.dictLookup("MaxWidth", &oDictItem);
			oDictItem.free();

			oFontDescriptor.dictLookup("MissingWidth", &oDictItem);
			oDictItem.free();
		}
		else
			oFontSelect.wsName = new std::wstring(wsFontBaseName);
		oFontDescriptor.free(); oDescendantFonts.free();
	}
	else
		oFontSelect.wsName = new std::wstring(wsFontBaseName);
	oFontObject.free();

	pFontInfo = pFontManager->GetFontInfoByParams(oFontSelect);
	return pFontInfo;
}
void GetFont(XRef* pXref, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, GfxFont* pFont, std::wstring& wsFileName, std::wstring& wsFontName)
{
	wsFileName = L"";
	wsFontName = L"";
	TFontEntry* pEntry = NULL;
	// MEMERR string dealocation pEntry
	if (!pFontList->Find2((*pFont->getID()), &pEntry))
	{
		GfxFontType eFontType = pFont->getType();
		if (fontType3 == eFontType) // FontType3 is handled by a separate command
		{
			pEntry->bAvailable = true;
			return;
		}

		std::wstring wsTempFileName = L"";
		Ref oEmbRef;
		bool bFontSubstitution = false, bFontBase14 = false;
		std::wstring wsActualFontName;
		std::wstring wsFontBaseName = NSStrings::GetStringFromUTF32(pFont->getName());
		if (wsFontBaseName.empty())
			wsFontBaseName = L"Helvetica";
		const BYTE* pData14 = NULL;
		unsigned int nSize14 = 0;
		double dStretch = 1.0;
#ifdef FONTS_USE_ONLY_MEMORY_STREAMS
		CMemoryFontStream oMemoryFontStream;
#endif
		// 1. If font is embedded, dump it to a temp file.
		// 2. If font is outside pdf but pdf has a reference to it, use that reference.
		// 3. Otherwise, select a font.

		if (pFont->getEmbeddedFontID(&oEmbRef))
		{
			std::wstring wsExt;
			switch (pFont->getType())
			{
			case fontType1:       wsExt = L".pfb_t1";    break;
			case fontType1C:      wsExt = L".pfb_t1c";   break;
			case fontType1COT:    wsExt = L".pfb_t1cot"; break;
			case fontTrueType:    wsExt = L".ttf";       break;
			case fontTrueTypeOT:  wsExt = L".otf";       break;
			case fontCIDType0:    wsExt = L".cid_0";     break;
			case fontCIDType0C:   wsExt = L".cid_0c";    break;
			case fontCIDType0COT: wsExt = L".cid_0cot";  break;
			case fontCIDType2:    wsExt = L".cid_2";     break;
			case fontCIDType2OT:  wsExt = L".cid_2ot";   break;
			}

#ifdef FONTS_USE_ONLY_MEMORY_STREAMS
			if (NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage())
			{
				wsTempFileName = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->GenerateId();
			}
#else
			FILE* pTempFile = NULL;
			if (!NSFile::CFileBinary::OpenTempFile(&wsTempFileName, &pTempFile, L"wb", (wchar_t*)wsExt.c_str(),
												   (wchar_t*)((GlobalParamsAdaptor*)globalParams)->GetTempFolder().c_str(), NULL))
			{
				if (L"" != wsTempFileName)
					NSFile::CFileBinary::Remove(wsTempFileName);

				pEntry->bAvailable = true;
				return;
			}
			wsTempFileName = UTF8_TO_U(NSSystemPath::NormalizePath(U_TO_UTF8(wsTempFileName)));
#endif

			Object oStreamObject;
			{
				CObjectGuard oReferenceObject;
				oReferenceObject->initRef(oEmbRef.num, oEmbRef.gen);
				oReferenceObject->fetch(pXref, &oStreamObject);
			}
			if (!oStreamObject.isStream())
			{
				// Embedded font is incorrectly written
				oStreamObject.free();

#ifndef FONTS_USE_ONLY_MEMORY_STREAMS
				fclose(pTempFile);

				if (L"" != wsTempFileName)
					NSFile::CFileBinary::Remove(wsTempFileName);
#endif

				pEntry->bAvailable = true;
				return;
			}
			oStreamObject.streamReset();

#ifdef FONTS_USE_ONLY_MEMORY_STREAMS
			oMemoryFontStream.load(oStreamObject);
			NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Add(wsTempFileName, oMemoryFontStream.m_pData, (LONG)oMemoryFontStream.m_nSize, true);
#else
			int nChar;
			while ((nChar = oStreamObject.streamGetChar()) != EOF)
			{
				fputc(nChar, pTempFile);
			}
			fclose(pTempFile);
#endif

			oStreamObject.streamClose();
			oStreamObject.free();
			wsFileName = wsTempFileName;

#ifdef FONTS_USE_AFM_SETTINGS
			// For Type1 fonts, need to write Afm file with metrics
			if (fontType1 == pFont->getType() || fontType1C == pFont->getType() || fontType1COT == pFont->getType())
			{
				std::wstring wsSplitFileName, wsSplitFileExt;
				SpitPathExt(wsFileName, &wsSplitFileName, &wsSplitFileExt);
				std::wstring wsAfmPath = wsSplitFileName + L".afm";

				FILE* pFile = NSFile::CFileBinary::OpenFileNative(wsAfmPath, L"wb");
				if (pFile)
				{
					Ref* pRef = pFont->getID();
					CObjectGuard oFontObject;
					{
						CObjectGuard oRefObject;
						oRefObject->initRef(pRef->num, pRef->gen);
						oRefObject->fetch(pXref, oFontObject.get());
					}

					if (oFontObject->isDict())
					{
						std::string sFontName, sFontFamily;
						int nFontWeight = 0, nItalicAngle = 0, nAscent = 0, nDescent = 0;
						int nCapHeight = 0, nXHeight = 0, nStemV = 0, nStemH = 0, nMissingWidth = 0;
						int arrBBox[4] = { 0, 0, 0, 0 };

						CObjectGuard oFontDescriptor;
						if (oFontObject->dictLookup("FontDescriptor", oFontDescriptor.get())->isDict())
						{
							Object oDictItem;
							oFontDescriptor->dictLookup("FontName", &oDictItem);
							if (oDictItem.isName()) sFontName = oDictItem.getName();
							oDictItem.free();

							oFontDescriptor->dictLookup("FontFamily", &oDictItem);
							if (oDictItem.isName()) sFontFamily = oDictItem.getName();
							oDictItem.free();

							oFontDescriptor->dictLookup("FontWeight", &oDictItem);
							if (oDictItem.isInt()) nFontWeight = oDictItem.getInt();
							oDictItem.free();

							if (oFontDescriptor->dictLookup("FontBBox", &oDictItem)->isArray() && oDictItem.arrayGetLength() == 4)
							{
								for (int nIndex = 0; nIndex < 4; nIndex++)
								{
									CObjectGuard oArrayItem;
									if (oDictItem.arrayGet(nIndex, oArrayItem.get())->isInt())
										arrBBox[nIndex] = oArrayItem->getInt();
								}
							}
							oDictItem.free();

							oFontDescriptor->dictLookup("ItalicAngle", &oDictItem);
							if (oDictItem.isInt()) nItalicAngle = oDictItem.getInt();
							oDictItem.free();

							oFontDescriptor->dictLookup("Ascent", &oDictItem);
							if (oDictItem.isInt()) nAscent = oDictItem.getInt();
							oDictItem.free();

							oFontDescriptor->dictLookup("CapHeight", &oDictItem);
							if (oDictItem.isInt()) nCapHeight = oDictItem.getInt();
							oDictItem.free();

							oFontDescriptor->dictLookup("XHeight", &oDictItem);
							if (oDictItem.isInt()) nXHeight = oDictItem.getInt();
							oDictItem.free();

							oFontDescriptor->dictLookup("StemV", &oDictItem);
							if (oDictItem.isInt()) nStemV = oDictItem.getInt();
							oDictItem.free();

							oFontDescriptor->dictLookup("StemH", &oDictItem);
							if (oDictItem.isInt()) nStemH = oDictItem.getInt();
							oDictItem.free();

							oFontDescriptor->dictLookup("Descent", &oDictItem);
							if (oDictItem.isInt()) nDescent = oDictItem.getInt();
							oDictItem.free();

							oFontDescriptor->dictLookup("MissingWidth", &oDictItem);
							if (oDictItem.isInt()) nMissingWidth = oDictItem.getInt();
							oDictItem.free();

						}

						fprintf(pFile, "StartFontMetrics 3.0\n");
						if (!sFontName.empty()) fprintf(pFile, "FontName %s\n", sFontName.c_str());
						if (!sFontFamily.empty()) fprintf(pFile, "FamilyName %s\n", sFontFamily.c_str());
						if (nFontWeight >= 550) fprintf(pFile, "Weight Bold\n");

						fprintf(pFile, "ItalicAngle %d\n", nItalicAngle);

						fprintf(pFile, "FontBBox %d %d %d %d\n", arrBBox[0], arrBBox[1], arrBBox[2], arrBBox[3]);

						fprintf(pFile, "CapHeight %d\n", nCapHeight);
						fprintf(pFile, "XHeight %d\n", nXHeight);
						fprintf(pFile, "Ascender %d\n", nAscent);
						fprintf(pFile, "Descender %d\n", nDescent);
						fprintf(pFile, "StdHW %d\n", nStemH);
						fprintf(pFile, "StdHV %d\n", nStemV);

						int nFirstChar = 0;
						CObjectGuard oDictItem;
						if (oFontObject->dictLookup("FirstChar", oDictItem.get())->isInt()) nFirstChar = oDictItem->getInt();
						oDictItem->free();

						Gfx8BitFont* pT1Font = (Gfx8BitFont*)pFont;
						if (oFontObject->dictLookup("Widths", oDictItem.get())->isArray())
						{
							int nWidthsCount = oDictItem->arrayGetLength();
							fprintf(pFile, "StartCharMetrics %d\n", nWidthsCount);
							for (int nIndex = 0; nIndex < nWidthsCount; nIndex++)
							{
								int nWidth = nMissingWidth;
								CObjectGuard oArrayItem;
								if (oDictItem->arrayGet(nIndex, oArrayItem.get())->isInt()) nWidth = oArrayItem->getInt();

								char** ppEncoding = pT1Font->getEncoding();

								if (ppEncoding && ppEncoding[nIndex])
									fprintf(pFile, "C %d ; WX %d ; N %s ;\n", nIndex + nFirstChar, nWidth, ppEncoding[nIndex]);
								else
									fprintf(pFile, "C %d ; WX %d ;\n", nIndex + nFirstChar, nWidth);
							}
							fprintf(pFile, "EndCharMetrics\n");
						}
					}
				}
				fclose(pFile);
			}
#endif

			// Load the font file itself to determine its exact type
			if (!pFontManager->LoadFontFromFile(wsFileName, 0, 10, 72, 72))
			{
				pEntry->bAvailable = true;
				return;
			}

			std::wstring wsFontType = pFontManager->GetFontType();
			if (L"TrueType" == wsFontType)
			{
				if (eFontType != fontType1COT   && eFontType != fontTrueType
				 && eFontType != fontTrueTypeOT && eFontType != fontCIDType0COT
				 && eFontType != fontCIDType2   && eFontType != fontCIDType2OT)
				{
					if (eFontType == fontType1 || eFontType == fontType1C)
						eFontType = fontType1COT;
					else if (eFontType == fontCIDType0 || eFontType == fontCIDType0C)
						eFontType = fontCIDType0COT;
				}
			}
			else if (L"Type 1" == wsFontType)
			{
				if (eFontType != fontType1 && eFontType != fontType1C)
					eFontType = fontType1;
			}
			else if (L"CID Type 1" == wsFontType)
			{
				if (eFontType != fontCIDType0   && eFontType != fontCIDType0C
				 && eFontType != fontCIDType2OT && eFontType != fontCIDType0COT)
					eFontType = fontCIDType0;
			}
			else if (L"CFF" == wsFontType)
			{
				if (eFontType != fontType1C      && eFontType != fontType1COT
				 && eFontType != fontTrueTypeOT  && eFontType != fontCIDType0C
				 && eFontType != fontCIDType0COT && eFontType != fontCIDType2OT
				 && eFontType != fontCIDType2)
				{
					if (eFontType == fontType1 || eFontType == fontTrueType)
						eFontType = fontType1C;
					else if (eFontType == fontCIDType0)
						eFontType = fontCIDType0C;
				}
			}
		}
#ifndef FONTS_USE_ONLY_MEMORY_STREAMS
		else if (PdfReader::GetBaseFont(wsFontBaseName, pData14, nSize14))
		{
			FILE* pFile = NULL;
			if (!NSFile::CFileBinary::OpenTempFile(&wsTempFileName, &pFile, L"wb", L".base",
												   (wchar_t*)((GlobalParamsAdaptor*)globalParams)->GetTempFolder().c_str(), NULL))
			{
				if (!wsTempFileName.empty())
					NSFile::CFileBinary::Remove(wsTempFileName);

				pEntry->bAvailable = true;
				return;
			}
			fclose(pFile);
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(wsTempFileName);
			oFile.WriteFile((BYTE*)pData14, nSize14);
			oFile.CloseFile();
			wsFileName = wsTempFileName;
			bFontBase14 = true;

			eFontType = fontTrueType;
		}
#else
		else if ([&oMemoryFontStream, wsFontBaseName]()
		{
			const BYTE* pData14 = NULL;
			unsigned int nSize14 = 0;
			if (PdfReader::GetBaseFont(wsFontBaseName, pData14, nSize14))
			{
				oMemoryFontStream.fromBuffer((BYTE*)pData14, nSize14);
				return true;
			}
			return false;
		}())
		{
			wsFileName = wsFontBaseName;
			bFontBase14 = true;
			NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Add(wsFileName, oMemoryFontStream.m_pData, (LONG)oMemoryFontStream.m_nSize, true);
		}
#endif
		else if (!pFont->locateFont(pXref, false) ||
				 (wsFileName = NSStrings::GetStringFromUTF32(pFont->locateFont(pXref, false)->path)).length() == 0)
		{
			NSFonts::CFontInfo* pFontInfo = GetFontByParams(pXref, pFontManager, pFont, wsFontBaseName, dStretch);

			if (pFontInfo && L"" != pFontInfo->m_wsFontPath)
			{
				wsFileName = pFontInfo->m_wsFontPath;
				eFontType  = pFont->isCIDFont() ? fontCIDType2 : fontTrueType;

#ifdef FONTS_USE_ONLY_MEMORY_STREAMS
				if (NSWasm::IsJSEnv())
					wsFileName = pFontInfo->m_wsFontName;

				if (!wsFileName.empty())
				{
					wsFileName = NSWasm::LoadFont(wsFileName, pFontInfo->m_bBold, pFontInfo->m_bItalic);
					if (wsFileName.empty())
					{
						pFontList->Remove(*pFont->getID());
						return;
					}
				}
				oMemoryFontStream.fromStream(wsFileName);
#endif

				bFontSubstitution = true;
				wsActualFontName = pFontInfo->m_wsFontName;
			}
			else // As a last resort, simply don't write anything with this font
			{
				pEntry->bAvailable = true;
				return;
			}
		}
		// Here we load encodings
		int* pCodeToGID = NULL, *pCodeToUnicode = NULL;
		int nLen = 0;
		FoFiTrueType* pTTFontFile  = NULL;
#ifdef FONTS_USE_ONLY_MEMORY_STREAMS
		FoFiIdentifierType fofiType = FoFiIdentifier::identifyStream(&readFromMemoryStream, &oMemoryFontStream);
		oMemoryFontStream.toStart();
#else
		FoFiIdentifierType fofiType = FoFiIdentifier::identifyFile((char*)U_TO_UTF8(wsFileName).c_str());
#endif

		switch (eFontType)
		{
		case fontType1:
		case fontType1C:
		case fontType1COT:
		{
			Gfx8BitFont* pFont8bit = NULL;
			if (fofiType == fofiIdTrueType)
			{
#ifdef FONTS_USE_ONLY_MEMORY_STREAMS
				pTTFontFile = FoFiTrueType::make((char*)oMemoryFontStream.m_pData, oMemoryFontStream.m_nSize, 0);
#else
				pTTFontFile = FoFiTrueType::load((char*)U_TO_UTF8(wsFileName).c_str(), 0);
#endif

				if (pTTFontFile)
				{
					pCodeToGID = ((Gfx8BitFont*)pFont)->getCodeToGIDMap(pTTFontFile);
					nLen = 256;

					delete pTTFontFile;
					pTTFontFile = NULL;
				}
				else
				{
					pCodeToGID = NULL;
					nLen = 0;
				}
			}
			else if (L"" != wsFileName && (pFont8bit = dynamic_cast<Gfx8BitFont*>(pFont)))
			{
				char** ppEncoding = pFont8bit->getEncoding();
				if (!ppEncoding)
					break;

				if (!pFontManager)
					break;

				pFontManager->LoadFontFromFile(wsFileName, 0, 1, 72, 72);
				pCodeToGID = (int*)MemUtilsMallocArray(256, sizeof(int));
				if (!pCodeToGID)
					break;

				nLen = 256;
				for (int nIndex = 0; nIndex < 256; ++nIndex)
				{
					pCodeToGID[nIndex] = 0;
					char* sName = NULL;
					if ((sName = ppEncoding[nIndex]))
					{
						unsigned short ushGID = pFontManager->GetNameIndex(AStringToWString(sName));
						pCodeToGID[nIndex] = ushGID;
					}
				}
			}
			break;
		}
		case fontTrueType:
		case fontTrueTypeOT:
		{
			if (fofiType == fofiIdType1PFB)
			{
				Gfx8BitFont* pFont8bit = dynamic_cast<Gfx8BitFont*>(pFont);
				if (L"" != wsFileName && pFont8bit && pFont8bit->getHasEncoding())
				{
					char** ppEncoding = pFont8bit->getEncoding();
					if (!ppEncoding)
						break;

					if (!pFontManager)
						break;

					pFontManager->LoadFontFromFile(wsFileName, 0, 1, 72, 72);
					pCodeToGID = (int*)MemUtilsMallocArray(256, sizeof(int));
					if (!pCodeToGID)
						break;

					nLen = 256;
					for (int nIndex = 0; nIndex < 256; ++nIndex)
					{
						pCodeToGID[nIndex] = 0;
						char* sName = NULL;
						if ((sName = ppEncoding[nIndex]))
						{
							unsigned short ushGID = pFontManager->GetNameIndex(AStringToWString(sName));
							pCodeToGID[nIndex] = ushGID;
						}
					}
				}
				break;
			}

#ifdef FONTS_USE_ONLY_MEMORY_STREAMS
			pTTFontFile = FoFiTrueType::make((char*)oMemoryFontStream.m_pData, oMemoryFontStream.m_nSize, 0);
#else
			pTTFontFile = FoFiTrueType::load((char*)U_TO_UTF8(wsFileName).c_str(), 0);
#endif
			if (pTTFontFile)
			{
				pCodeToGID = ((Gfx8BitFont*)pFont)->getCodeToGIDMap(pTTFontFile);
				nLen = 256;

				delete pTTFontFile;
				pTTFontFile = NULL;
			}
			else
			{
				pCodeToGID = NULL;
				nLen       = 0;

				if (pFontManager->LoadFontFromFile(wsFileName, 0, 10, 72, 72))
				{
					nLen = 256;
					pCodeToGID = (int*)MemUtilsMallocArray(nLen, sizeof(int));
					for (int nCode = 0; nCode < nLen; ++nCode)
					{
						pCodeToGID[nCode] = pFontManager->GetGIDByUnicode(nCode);
					}
				}
			}
			break;
		}
		case fontCIDType0:
		case fontCIDType0C:
		{
			GfxCIDFont* pFontCID = dynamic_cast<GfxCIDFont*>(pFont);
			if (!bFontSubstitution && pFontCID && pFontCID->getCIDToGID())
			{
				nLen = pFontCID->getCIDToGIDLen();
				if (!nLen)
					break;
				pCodeToGID = (int*)MemUtilsMallocArray(nLen, sizeof(int));
				if (!pCodeToGID)
				{
					nLen = 0;
					break;
				}
				memcpy(pCodeToGID, ((GfxCIDFont*)pFont)->getCIDToGID(), nLen * sizeof(int));
				break;
			}
			/*
			 FoFiType1C* pT1CFontFile = NULL;
#ifdef FONTS_USE_ONLY_MEMORY_STREAMS
			pT1CFontFile = FoFiType1C::make((char*)oMemoryFontStream.m_pData, oMemoryFontStream.m_nSize);
#else
			pT1CFontFile = FoFiType1C::load((char*)U_TO_UTF8(wsFileName).c_str());
#endif
			if (pT1CFontFile)
			{
				pCodeToGID = pT1CFontFile->getCIDToGIDMap(&nLen);

				delete pT1CFontFile;
				pT1CFontFile = NULL;
			}
			else
			{
				pCodeToGID = NULL;
				nLen = 0;
			}
			*/
			pCodeToGID = NULL;
			nLen = 0;
			break;
		}
		case fontCIDType0COT:
		{
			GfxCIDFont* pFontCID = dynamic_cast<GfxCIDFont*>(pFont);
			if (!bFontSubstitution && pFontCID && pFontCID->getCIDToGID())
			{
				nLen = pFontCID->getCIDToGIDLen();
				if (!nLen)
					break;
				pCodeToGID = (int*)MemUtilsMallocArray(nLen, sizeof(int));
				if (!pCodeToGID)
				{
					nLen = 0;
					break;
				}
				memcpy(pCodeToGID, ((GfxCIDFont*)pFont)->getCIDToGID(), nLen * sizeof(int));
				break;
			}
#ifdef FONTS_USE_ONLY_MEMORY_STREAMS
			pTTFontFile = FoFiTrueType::make((char*)oMemoryFontStream.m_pData, oMemoryFontStream.m_nSize, 0);
#else
			pTTFontFile = FoFiTrueType::load((char*)U_TO_UTF8(wsFileName).c_str(), 0);
#endif

			if (pTTFontFile)
			{
				if (pTTFontFile->isOpenTypeCFF())
				{
					pCodeToGID = pTTFontFile->getCIDToGIDMap(&nLen);
				}
				else
				{
					pCodeToGID = NULL;
					nLen = 0;
				}
				delete pTTFontFile;
				pTTFontFile = NULL;
			}
			else
			{
				pCodeToGID = NULL;
				nLen = 0;
			}
			break;
		}
		case fontCIDType2:
		case fontCIDType2OT:
		{
			// Create CID-to-GID map
			// If font was not embedded and was substituted and has ToUnicode map, read GIDs from file based on unicode values.
			// For embedded fonts use CIDtoGID map
			pCodeToGID = NULL;
			nLen = 0;
			if (L"" != wsFileName && bFontSubstitution)
			{
				CharCodeToUnicode* pCodeToUnicode = NULL;
				if ((pCodeToUnicode = ((GfxCIDFont*)pFont)->getToUnicode()))
				{
#ifdef FONTS_USE_ONLY_MEMORY_STREAMS
					pTTFontFile = FoFiTrueType::make((char*)oMemoryFontStream.m_pData, oMemoryFontStream.m_nSize, 0);
#else
					pTTFontFile = FoFiTrueType::load((char*)U_TO_UTF8(wsFileName).c_str(), 0);
#endif
					if (pTTFontFile)
					{
						// Looking for Unicode Cmap
						std::vector<int> arrCMapIndex;
						for (int nCMapIndex = 0; nCMapIndex < pTTFontFile->getNumCmaps(); ++nCMapIndex)
						{
							if ((pTTFontFile->getCmapPlatform(nCMapIndex) == 3 && pTTFontFile->getCmapEncoding(nCMapIndex) == 1) || pTTFontFile->getCmapPlatform(nCMapIndex) == 0)
							{
								arrCMapIndex.push_back(nCMapIndex);
							}
						}
						if (arrCMapIndex.size() > 0)
						{
							// CID -> Unicode -> GID
							nLen = pCodeToUnicode->getLength();
							pCodeToGID = (int*)MemUtilsMallocArray(nLen, sizeof(int));
							for (int nCode = 0; nCode < nLen; ++nCode)
							{
								Unicode arrUnicodeBuffer[8];
								if (pCodeToUnicode->mapToUnicode(nCode, arrUnicodeBuffer, 8) > 0)
								{
									pCodeToGID[nCode] = pTTFontFile->mapCodeToGID(arrCMapIndex[0], arrUnicodeBuffer[0]);
									for (size_t nIndex = 1; nIndex < arrCMapIndex.size(); nIndex++)
									{
										if (0 == pCodeToGID[nCode])
											pCodeToGID[nCode] = pTTFontFile->mapCodeToGID(arrCMapIndex[nIndex], arrUnicodeBuffer[0]);
										else
											break;
									}
								}
								else
								{
									pCodeToGID[nCode] = 0;
								}
							}
						}
						delete pTTFontFile;
						pTTFontFile = NULL;
					}
					pCodeToUnicode->decRefCnt();
				}
			}
			else if (((GfxCIDFont*)pFont)->getCIDToGID())
			{
				nLen = ((GfxCIDFont*)pFont)->getCIDToGIDLen();
				pCodeToGID = (int*)MemUtilsMallocArray(nLen, sizeof(int));
				if (!pCodeToGID)
					break;

				memcpy(pCodeToGID, ((GfxCIDFont*)pFont)->getCIDToGID(), nLen * sizeof(int));
			}

			break;
		}
		default:
		{
			// This should not happen
#ifndef FONTS_USE_ONLY_MEMORY_STREAMS
			if (L"" != wsTempFileName)
				NSFile::CFileBinary::Remove(wsTempFileName);
#endif
			break;
		}
		}
		// Build Code -> Unicode table
		int nToUnicodeLen = 0;
		if (pFont->isCIDFont())
		{
			GfxCIDFont* pCIDFont = (GfxCIDFont*)pFont;
			CharCodeToUnicode* pToUnicode = pCIDFont->getToUnicode();
			if (NULL != pToUnicode)
			{
				nToUnicodeLen = pToUnicode->getLength();
				pCodeToUnicode = (int*)MemUtilsMallocArray(nToUnicodeLen, sizeof(int));

				if (pCodeToUnicode)
				{
					for (int nIndex = 0; nIndex < nToUnicodeLen; ++nIndex)
					{
						Unicode aUnicode[2];
						if (pToUnicode->mapToUnicode(nIndex, aUnicode, 2))
							pCodeToUnicode[nIndex] = aUnicode[0];
						else
							pCodeToUnicode[nIndex] = 0;
					}
				}

				pToUnicode->decRefCnt();
			}
		}
		else
		{
			// memory troubles here

			CharCodeToUnicode* pToUnicode = ((Gfx8BitFont*)pFont)->getToUnicode();
			if (NULL != pToUnicode)
			{
				nToUnicodeLen = pToUnicode->getLength();
				pCodeToUnicode = (int*)MemUtilsMallocArray(nToUnicodeLen, sizeof(int));//literally here

				if (pCodeToUnicode)
				{
					for (int nIndex = 0; nIndex < nToUnicodeLen; ++nIndex)
					{
						Unicode nUnicode = 0;
						if (pToUnicode->mapToUnicode(nIndex, &nUnicode, 1))
							pCodeToUnicode[nIndex] = (unsigned short)nUnicode;
						else
							pCodeToUnicode[nIndex] = nIndex;
					}
				}
				pToUnicode->decRefCnt();
			}
		}

		// Trim index from FontName if present
		if (wsFontName.empty())
			wsFontName = wsFontBaseName;
		if (!bFontBase14 && !bFontSubstitution)
			wsFontName += (L" " + ComputeFontHash(pXref, pFont));

		pEntry->wsFilePath     = wsFileName;
		pEntry->wsFontName     = wsFontName;
		pEntry->pCodeToGID     = pCodeToGID;
		pEntry->pCodeToUnicode = pCodeToUnicode;
		pEntry->unLenGID       = (unsigned int)nLen;
		pEntry->unLenUnicode   = (unsigned int)nToUnicodeLen;
		pEntry->bAvailable     = true;
		pEntry->dStretch       = dStretch;
		pEntry->bFontSubstitution = bFontSubstitution;
		pEntry->bIsIdentity = pFont->isCIDFont() == gTrue ? ((GfxCIDFont*)pFont)->usesIdentityEncoding() || ((GfxCIDFont*)pFont)->usesIdentityCIDToGID() || ((GfxCIDFont*)pFont)->ctuUsesCharCodeToUnicode() || pFont->getType() == fontCIDType0C : false;

		bool bBold = false, bItalic = false;
		CheckFontStylePDF(wsFontName, bBold, bItalic);
		if (!bBold)
			bBold = pFont->isBold();
		if (!bItalic)
			bItalic = pFont->isItalic();
		pEntry->bBold   = bBold;
		pEntry->bItalic = bItalic;
		if (bFontSubstitution)
			pEntry->wsActualFontName = wsActualFontName;
	}
	else if (NULL != pEntry)
	{
		wsFileName = pEntry->wsFilePath;
		wsFontName = pEntry->wsFontName;
	}
}

CType3FontMetrics* BuildType3FontMetrics(XRef* pXref, GfxFont* pFont)
{
	CType3FontMetrics* pMetrics = new CType3FontMetrics();

	Ref* pRef = pFont->getID();
	CObjectGuard oFontObj;
	{
		CObjectGuard oRefObj;
		oRefObj->initRef(pRef->num, pRef->gen);
		oRefObj->fetch(pXref, oFontObj.get());
	}

	if (!oFontObj->isDict())
		return pMetrics;

	{
		CObjectGuard oMatrix;
		if (oFontObj->dictLookup("FontMatrix", oMatrix.get())->isArray() && oMatrix->arrayGetLength() == 6)
		{
			for (int i = 0; i < 6; ++i)
			{
				CObjectGuard oVal;
				if (oMatrix->arrayGet(i, oVal.get())->isNum())
					pMetrics->arrFontMatrix[i] = oVal->getNum();
			}
		}
	}

	if (pMetrics->arrFontMatrix[0] > 0)
		pMetrics->dUnitsPerEm = std::round(1.0 / pMetrics->arrFontMatrix[0]);
	else
		pMetrics->dUnitsPerEm = 1000;

	{
		CObjectGuard oBBox;
		if (oFontObj->dictLookup("FontBBox", oBBox.get())->isArray() && oBBox->arrayGetLength() == 4)
		{
			CObjectGuard oV0, oV1, oV2, oV3;
			if (oBBox->arrayGet(0, oV0.get())->isNum()) pMetrics->dLLx = oV0->getNum();
			if (oBBox->arrayGet(1, oV1.get())->isNum()) pMetrics->dLLy = oV1->getNum();
			if (oBBox->arrayGet(2, oV2.get())->isNum()) pMetrics->dURx = oV2->getNum();
			if (oBBox->arrayGet(3, oV3.get())->isNum()) pMetrics->dURy = oV3->getNum();

			double dTextY1 = pMetrics->arrFontMatrix[3] * pMetrics->dLLy;
			double dTextY2 = pMetrics->arrFontMatrix[3] * pMetrics->dURy;

			pMetrics->dAscent  = std::round(std::max(dTextY2 / pMetrics->arrFontMatrix[0], 0.0));
			pMetrics->dDescent = std::round(std::abs(std::min(dTextY1 / pMetrics->arrFontMatrix[0], 0.0)));

			if (pMetrics->dAscent == 0 && pMetrics->dDescent == 0)
			{
				pMetrics->dAscent  = pMetrics->dUnitsPerEm * 0.8;
				pMetrics->dDescent = pMetrics->dUnitsPerEm * 0.2;
			}
		}
	}

	int nFirstChar = 0;
	{
		CObjectGuard oFirstChar;
		if (oFontObj->dictLookup("FirstChar", oFirstChar.get())->isInt())
			nFirstChar = oFirstChar->getInt();
	}

	{
		CObjectGuard oWidths;
		if (oFontObj->dictLookup("Widths", oWidths.get())->isArray())
		{
			int nWidthsLen = oWidths->arrayGetLength();
			for (int i = 0; i < nWidthsLen; ++i)
			{
				CObjectGuard oVal;
				if (oWidths->arrayGet(i, oVal.get())->isNum())
					pMetrics->mapWidths[nFirstChar + i] = oVal->getNum();
			}
		}
	}

	return pMetrics;
}
//--------------------------------------------------------------------------------------
// CFontList
//--------------------------------------------------------------------------------------
CPdfFontList::CPdfFontList()
{
	m_oCS.InitializeCriticalSection();
	m_oFontMap.clear();
}
CPdfFontList::~CPdfFontList()
{
	m_oCS.DeleteCriticalSection();
	Clear();
}
bool CPdfFontList::Find(Ref oRef, TFontEntry* pEntry)
{
	CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

	bool bResult = (NULL != (pEntry = Lookup(oRef)));

	if (bResult)
	{
		// Font was found, but cannot be used yet because it is being loaded in a parallel thread
		while (!pEntry->bAvailable)
			NSThreads::Sleep(10);
	}

	RELEASEOBJECT(pCS);

	return bResult;
}
bool CPdfFontList::Find2(Ref oRef, TFontEntry** ppEntry)
{
	CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

	bool bResult = (NULL != ((*ppEntry) = Lookup(oRef)));

	if (bResult)
	{
		// Font was found, but cannot be used yet because it is being loaded in a parallel thread
		while (!(*ppEntry)->bAvailable)
			NSThreads::Sleep(10);
	}

	if (!bResult)
	{
		(*ppEntry) = Add(oRef, std::wstring(), NULL, NULL, 0, 0);
		(*ppEntry)->bAvailable = false;
	}

	RELEASEOBJECT(pCS);

	return bResult;
}
TFontEntry* CPdfFontList::Add(Ref oRef, const std::wstring& wsFileName, int* pCodeToGID, int* pCodeToUnicode, unsigned int unLenGID, unsigned int unLenUnicode)
{
	// This function is only called from Find2, so no need to check if this font already exists
	CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

	TFontEntry* pNewEntry = new TFontEntry;
	pNewEntry->wsFilePath     = wsFileName;
	pNewEntry->pCodeToGID     = pCodeToGID;
	pNewEntry->pCodeToUnicode = pCodeToUnicode;
	pNewEntry->unLenGID       = unLenGID;
	pNewEntry->unLenUnicode   = unLenUnicode;

	Add(oRef, pNewEntry);

	RELEASEOBJECT(pCS);

	return pNewEntry;
}
void CPdfFontList::Remove(Ref oRef)
{
	std::map<Ref, TFontEntry*>::iterator oPos = m_oFontMap.find(oRef);
	if (m_oFontMap.end() != oPos)
	{
		TFontEntry* pEntry = oPos->second;
		if (NULL != pEntry)
		{
			MemUtilsFree(pEntry->pCodeToGID);
			MemUtilsFree(pEntry->pCodeToUnicode);
		}
		delete pEntry;
		m_oFontMap.erase(oPos);
	}
}
void CPdfFontList::Clear()
{
	for (auto const &oIt : m_oFontMap)
	{
		TFontEntry* pEntry = oIt.second;
		if (NULL != pEntry)
		{
			MemUtilsFree(pEntry->pCodeToGID);
			MemUtilsFree(pEntry->pCodeToUnicode);
		}
		delete pEntry;
	}
	m_oFontMap.clear();
}
bool CPdfFontList::GetFont(Ref* pRef, TFontEntry* pEntry)
{
	TFontEntry* pFindEntry = Lookup(*pRef);
	if (NULL == pFindEntry)
		return false;

	*pEntry = *pFindEntry;
	return true;
}
TFontEntry* CPdfFontList::Lookup(Ref& oRef)
{
	std::map<Ref, TFontEntry*>::const_iterator oPos = m_oFontMap.find(oRef);
	return m_oFontMap.end() == oPos ? NULL : oPos->second;
}
void CPdfFontList::Add(Ref& oRef, TFontEntry* pFontEntry)
{
	// Before calling this function, need to check if an element with this key exists
	m_oFontMap.insert(std::pair<Ref, TFontEntry*>(oRef, pFontEntry));
}
const std::map<Ref, TFontEntry*>& CPdfFontList::GetFonts()
{
	return m_oFontMap;
}
}
