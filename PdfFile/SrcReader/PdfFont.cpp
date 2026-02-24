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

#include "PdfFont.h"
#include "Adaptors.h"

#include "../lib/xpdf/TextString.h"
#include "../lib/xpdf/GfxFont.h"
#include "../lib/xpdf/Lexer.h"
#include "../lib/xpdf/Parser.h"
#include "../Resources/BaseFonts.h"

#include "../../DesktopEditor/common/StringExt.h"
#include "../../DesktopEditor/common/StringBuilder.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

const std::vector<std::string> arrCMap = {"GB-EUC-H", "GB-EUC-V", "GB-H", "GB-V", "GBpc-EUC-H", "GBpc-EUC-V", "GBK-EUC-H",
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

bool scanFonts(Dict *pResources, const std::vector<std::string>& arrCMap, int nDepth, std::vector<int>& arrUniqueResources)
{
	if (nDepth > 5)
		return false;
	Object oFonts;
	if (pResources->lookup("Font", &oFonts)->isDict())
	{
		for (int i = 0, nLength = oFonts.dictGetLength(); i < nLength; ++i)
		{
			Object oFont, oEncoding;
			if (!oFonts.dictGetVal(i, &oFont)->isDict() || !oFont.dictLookup("Encoding", &oEncoding)->isName())
			{
				oFont.free(); oEncoding.free();
				continue;
			}
			oFont.free();
			char* sName = oEncoding.getName();
			if (std::find(arrCMap.begin(), arrCMap.end(), sName) != arrCMap.end())
			{
				oEncoding.free(); oFonts.free();
				return true;
			}
			oEncoding.free();
		}
	}
	oFonts.free();

	auto fScanFonts = [pResources, nDepth, &arrUniqueResources](const std::vector<std::string>& arrCMap, const char* sName)
	{
		Object oObject;
		if (!pResources->lookup(sName, &oObject)->isDict())
		{
			oObject.free();
			return false;
		}
		for (int i = 0, nLength = oObject.dictGetLength(); i < nLength; ++i)
		{
			Object oXObj, oResources;
			if (!oObject.dictGetVal(i, &oXObj)->isStream() || !oXObj.streamGetDict()->lookup("Resources", &oResources)->isDict())
			{
				oXObj.free(); oResources.free();
				continue;
			}
			Object oRef;
			if (oXObj.streamGetDict()->lookupNF("Resources", &oRef)->isRef() && std::find(arrUniqueResources.begin(), arrUniqueResources.end(), oRef.getRef().num) != arrUniqueResources.end())
			{
				oXObj.free(); oResources.free(); oRef.free();
				continue;
			}
			arrUniqueResources.push_back(oRef.getRef().num);
			oXObj.free(); oRef.free();
			if (scanFonts(oResources.getDict(), arrCMap, nDepth + 1, arrUniqueResources))
			{
				oResources.free(); oObject.free();
				return true;
			}
			oResources.free();
		}
		oObject.free();
		return false;
	};

	if (fScanFonts(arrCMap, "XObject") || fScanFonts(arrCMap, "Pattern"))
		return true;

	Object oExtGState;
	if (!pResources->lookup("ExtGState", &oExtGState)->isDict())
	{
		oExtGState.free();
		return false;
	}
	for (int i = 0, nLength = oExtGState.dictGetLength(); i < nLength; ++i)
	{
		Object oGS, oSMask, oSMaskGroup, oResources;
		if (!oExtGState.dictGetVal(i, &oGS)->isDict() || !oGS.dictLookup("SMask", &oSMask)->isDict() || !oSMask.dictLookup("G", &oSMaskGroup)->isStream() || !oSMaskGroup.streamGetDict()->lookup("Resources", &oResources)->isDict())
		{
			oGS.free(); oSMask.free(); oSMaskGroup.free(); oResources.free();
			continue;
		}
		oGS.free(); oSMask.free();
		Object oRef;
		if (oSMaskGroup.streamGetDict()->lookupNF("Resources", &oRef)->isRef() && std::find(arrUniqueResources.begin(), arrUniqueResources.end(), oRef.getRef().num) != arrUniqueResources.end())
		{
			oSMaskGroup.free(); oResources.free(); oRef.free();
			continue;
		}
		arrUniqueResources.push_back(oRef.getRef().num);
		oSMaskGroup.free(); oRef.free();
		if (scanFonts(oResources.getDict(), arrCMap, nDepth + 1, arrUniqueResources))
		{
			oResources.free(); oExtGState.free();
			return true;
		}
		oResources.free();
	}
	oExtGState.free();

	return false;
}
bool scanAPfonts(Object* oAnnot, const std::vector<std::string>& arrCMap, std::vector<int>& arrUniqueResources)
{
	Object oAP;
	if (!oAnnot->dictLookup("AP", &oAP)->isDict())
	{
		oAP.free();
		return false;
	}
	auto fScanAPView = [&arrUniqueResources](Object* oAP, const std::vector<std::string>& arrCMap, const char* sName)
	{
		Object oAPi, oRes;
		if (!oAP->dictLookup(sName, &oAPi)->isStream() || !oAPi.streamGetDict()->lookup("Resources", &oRes)->isDict())
		{
			oAPi.free(); oRes.free();
			return false;
		}
		Object oRef;
		if (oAPi.streamGetDict()->lookupNF("Resources", &oRef)->isRef() && std::find(arrUniqueResources.begin(), arrUniqueResources.end(), oRef.getRef().num) != arrUniqueResources.end())
		{
			oAPi.free(); oRes.free(); oRef.free();
			return false;
		}
		arrUniqueResources.push_back(oRef.getRef().num);
		oAPi.free(); oRef.free();
		bool bRes = scanFonts(oRes.getDict(), arrCMap, 0, arrUniqueResources);
		oRes.free();
		return bRes;
	};
	bool bRes = fScanAPView(&oAP, arrCMap, "N") || fScanAPView(&oAP, arrCMap, "D") || fScanAPView(&oAP, arrCMap, "R");
	oAP.free();
	return bRes;
}
bool CheckFontNameStyle(std::wstring& sName, const std::wstring& sStyle)
{
	size_t nPos = 0;
	size_t nLenReplace = sStyle.length();
	bool bRet = false;

	std::wstring sName2 = sName;
	NSStringExt::ToLower(sName2);

	while (std::wstring::npos != (nPos = sName2.find(sStyle, nPos)))
	{
		size_t nOffset = 0;
		if ((nPos > 0) && (sName2.at(nPos - 1) == '-' || sName2.at(nPos - 1) == ','))
		{
			--nPos;
			++nOffset;
		}

		bRet = true;
		sName.erase(nPos, nLenReplace + nOffset);
		sName2.erase(nPos, nLenReplace + nOffset);
	}
	return bRet;
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
			if (nChar < '0' || nChar > 'F' || (nChar > '9' && nChar < 'A'))
			{
				bIsRemove = false;
				break;
			}
		}
		if (bIsRemove)
			s.erase(lastSpace + 1);
	}

	bool bDummy1 = false, bDummy2 = false;
	PdfReader::CheckFontStylePDF(s, bDummy1, bDummy2);

	NSStringExt::ToLower(s);
	s.erase(std::remove_if(s.begin(), s.end(), [](wchar_t c){ return c == L' ' || c == L'-' || c == L'_'; }), s.end());
	return s;
};

namespace PdfReader
{
std::string GetRCFromDS(const std::string& sDS, Object* pContents, const std::vector<double>& arrCFromDA)
{
	NSStringUtils::CStringBuilder oRC;

	oRC += L"<?xml version=\"1.0\"?><body xmlns=\"http://www.w3.org/1999/xhtml\" xmlns:xfa=\"http://www.xfa.org/schema/xfa-data/1.0/\" xfa:APIVersion=\"Acrobat:23.8.0\"  xfa:spec=\"2.0.2\"><p dir=\"ltr\"><span style=\"";
	if (sDS.find("font-family") == std::string::npos)
		oRC += L"font-family:Helvetica;";
	if (sDS.find("font-size") == std::string::npos)
		oRC += L"font-size:14.0pt;";
	if (sDS.find("text-align") == std::string::npos)
		oRC += L"text-align:left;";
	if (sDS.find("font-weight") == std::string::npos)
		oRC += L"font-weight:normal;";
	if (sDS.find("font-style") == std::string::npos)
		oRC += L"font-style:normal;";
	if (sDS.find("text-decoration") == std::string::npos)
		oRC += L"text-decoration:none;";
	if (sDS.find("color") == std::string::npos)
	{
		oRC += L"color:";
		if (arrCFromDA.size() == 3)
			oRC.WriteHexColor3((unsigned char)(arrCFromDA[0] * 255.0),
							   (unsigned char)(arrCFromDA[1] * 255.0),
							   (unsigned char)(arrCFromDA[2] * 255.0));
		else
			oRC += L"#000000";
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
bool IsNeedCMap(PDFDoc* pDoc)
{
	std::vector<int> arrUniqueResources;
	for (int nPage = 1, nLastPage = pDoc->getNumPages(); nPage <= nLastPage; ++nPage)
	{
		Page* pPage = pDoc->getCatalog()->getPage(nPage);
		Dict* pResources = pPage->getResourceDict();
		if (pResources && scanFonts(pResources, arrCMap, 0, arrUniqueResources))
			return true;

		Object oAnnots;
		if (!pPage->getAnnots(&oAnnots)->isArray())
		{
			oAnnots.free();
			continue;
		}
		for (int i = 0, nNum = oAnnots.arrayGetLength(); i < nNum; ++i)
		{
			Object oAnnot;
			if (!oAnnots.arrayGet(i, &oAnnot)->isDict())
			{
				oAnnot.free();
				continue;
			}

			Object oRefDR;
			if (oAnnot.dictLookupNF("DR", &oRefDR)->isRef() && std::find(arrUniqueResources.begin(), arrUniqueResources.end(), oRefDR.getRef().num) != arrUniqueResources.end())
			{
				oRefDR.free(); oAnnot.free();
				continue;
			}
			arrUniqueResources.push_back(oRefDR.getRef().num);
			oRefDR.free();

			Object oDR;
			if (oAnnot.dictLookup("DR", &oDR)->isDict() && scanFonts(oDR.getDict(), arrCMap, 0, arrUniqueResources))
			{
				oDR.free(); oAnnot.free(); oAnnots.free();
				return true;
			}
			oDR.free();

			if (scanAPfonts(&oAnnot, arrCMap, arrUniqueResources))
			{
				oAnnot.free(); oAnnots.free();
				return true;
			}
			oAnnot.free();
		}
		oAnnots.free();
	}

	AcroForm* pAcroForms = pDoc->getCatalog()->getForm();
	if (!pAcroForms)
		return false;
	Object oDR;
	Object* oAcroForm = pAcroForms->getAcroFormObj();

	Object oRefDR;
	if (oAcroForm->dictLookupNF("DR", &oRefDR)->isRef() && std::find(arrUniqueResources.begin(), arrUniqueResources.end(), oRefDR.getRef().num) != arrUniqueResources.end())
	{
		oRefDR.free();
		return false;
	}
	arrUniqueResources.push_back(oRefDR.getRef().num);
	oRefDR.free();

	if (oAcroForm->dictLookup("DR", &oDR)->isDict() && scanFonts(oDR.getDict(), arrCMap, 0, arrUniqueResources))
	{
		oDR.free();
		return true;
	}
	oDR.free();

	for (int i = 0, nNum = pAcroForms->getNumFields(); i < nNum; ++i)
	{
		AcroFormField* pField = pAcroForms->getField(i);

		if (pField->getResources(&oDR)->isDict() && scanFonts(oDR.getDict(), arrCMap, 0, arrUniqueResources))
		{
			oDR.free();
			return true;
		}
		oDR.free();

		Object oWidgetRef, oWidget;
		pField->getFieldRef(&oWidgetRef);
		oWidgetRef.fetch(pDoc->getXRef(), &oWidget);
		oWidgetRef.free();

		if (scanAPfonts(&oWidget, arrCMap, arrUniqueResources))
		{
			oWidget.free();
			return true;
		}
		oWidget.free();
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
std::map<std::wstring, std::wstring> GetAllFonts(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, bool bIsNeedCMap)
{
	std::map<std::wstring, std::wstring> mFonts;

	AcroForm* pAcroForms = pdfDoc->getCatalog()->getForm();
	if (pAcroForms)
	{
		std::vector<int> arrUniqueFontsRef;
		for (int nField = 0, nNum = pAcroForms->getNumFields(); nField < nNum; ++nField)
		{
			AcroFormField* pField = pAcroForms->getField(nField);
			if (!pField)
				continue;

			// Шрифт и размер шрифта - из DA
			Ref fontID;
			double dFontSize = 0;
			pField->getFont(&fontID, &dFontSize);

			Object oFontRef;
			if (fontID.num < 0)
			{
				std::string sFontKey;
				if (!GetFontFromAP(pdfDoc, pField, &oFontRef, sFontKey))
				{
					oFontRef.free();
					continue;
				}
			}
			else
				oFontRef.initRef(fontID.num, fontID.gen);

			if (std::find(arrUniqueFontsRef.begin(), arrUniqueFontsRef.end(), oFontRef.getRefNum()) != arrUniqueFontsRef.end())
			{
				oFontRef.free();
				continue;
			}

			std::string sFontName;
			std::string sActualFontName;
			std::wstring wsFileName;
			bool bBold = false, bItalic = false;
			wsFileName = GetFontData(pdfDoc, pFontManager, pFontList, &oFontRef, sFontName, sActualFontName, bBold, bItalic, bIsNeedCMap);

			if (!sActualFontName.empty())
			{
				oFontRef.free();
				continue;
			}

			if (!sFontName.empty())
			{
				std::wstring wsFontName = UTF8_TO_U(sFontName);
				if (mFonts.find(wsFontName) == mFonts.end())
				{
					arrUniqueFontsRef.push_back(oFontRef.getRefNum());
					mFonts[wsFontName] = wsFileName;
				}
			}
			oFontRef.free();

			if (pField->getAcroFormFieldType() == acroFormFieldPushbutton && fontID.num >= 0)
			{
				std::string sFontKey;
				if (GetFontFromAP(pdfDoc, pField, &oFontRef, sFontKey) && std::find(arrUniqueFontsRef.begin(), arrUniqueFontsRef.end(), oFontRef.getRefNum()) == arrUniqueFontsRef.end())
				{
					wsFileName = GetFontData(pdfDoc, pFontManager, pFontList, &oFontRef, sFontName, sActualFontName, bBold, bItalic, bIsNeedCMap);

					std::wstring wsFontName = UTF8_TO_U(sFontName);
					if (sActualFontName.empty() && mFonts.find(wsFontName) == mFonts.end())
					{
						arrUniqueFontsRef.push_back(oFontRef.getRefNum());
						mFonts[wsFontName] = wsFileName;
					}
				}
			}
			oFontRef.free();
		}
	}

	for (int nPage = 0, nLastPage = pdfDoc->getNumPages(); nPage < nLastPage; ++nPage)
	{
		Page* pPage = pdfDoc->getCatalog()->getPage(nPage + 1);
		if (!pPage)
			continue;

		Object oAnnots;
		if (!pPage->getAnnots(&oAnnots)->isArray())
		{
			oAnnots.free();
			continue;
		}

		for (int i = 0, nNum = oAnnots.arrayGetLength(); i < nNum; ++i)
		{
			Object oAnnot;
			if (!oAnnots.arrayGet(i, &oAnnot)->isDict())
			{
				oAnnot.free();
				continue;
			}

			Object oSubtype;
			if (!oAnnot.dictLookup("Subtype", &oSubtype)->isName("FreeText"))
			{
				oSubtype.free(); oAnnot.free();
				continue;
			}
			oSubtype.free();

			std::string sRC;
			Object oObj;
			if (!oAnnot.dictLookup("RC", &oObj)->isString())
			{
				oObj.free();
				if (oAnnot.dictLookup("Contents", &oObj)->isString() && oObj.getString()->getLength())
				{
					std::string sDS;
					Object oObj2;
					if (oAnnot.dictLookup("DS", &oObj2)->isString())
					{
						TextString* s = new TextString(oObj2.getString());
						sDS = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
						delete s;
					}
					oObj2.free();

					sRC = GetRCFromDS(sDS, &oObj, {});
					if (sRC.find("font-family:Helvetica") != std::string::npos)
					{
						const unsigned char* pData14 = NULL;
						unsigned int nSize14 = 0;
						std::wstring wsFontName = L"Helvetica";
						NSFonts::IFontsMemoryStorage* pMemoryStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
						if (pMemoryStorage && !pMemoryStorage->Get(wsFontName) && GetBaseFont(wsFontName, pData14, nSize14))
							pMemoryStorage->Add(wsFontName, (BYTE*)pData14, nSize14, false);
						mFonts[L"Helvetica"] = L"Helvetica";
					}
				}
			}
			else
			{
				TextString* s = new TextString(oObj.getString());
				sRC = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
				delete s;
			}
			oObj.free(); oAnnot.free();

			Object oAnnotRef;
			oAnnots.arrayGetNF(i, &oAnnotRef);
			std::vector<PdfReader::CAnnotMarkup::CFontData*> arrRC = CAnnotMarkup::ReadRC(sRC);
			std::map<std::wstring, std::wstring> mFreeText = GetFreeTextFont(pdfDoc, pFontManager, pFontList, &oAnnotRef, arrRC);
			for (std::map<std::wstring, std::wstring>::iterator it = mFreeText.begin(); it != mFreeText.end(); ++it)
			{
				if (mFonts.find(it->first) != mFonts.end())
					continue;
				mFonts[it->first] = it->second;
			}
			oAnnotRef.free();
			for (int j = 0; j < arrRC.size(); ++j)
				RELEASEOBJECT(arrRC[j]);
		}
		oAnnots.free();
	}

	return mFonts;
}
std::wstring GetFontData(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList *pFontList, Object* oFontRef, std::string& sFontName, std::string& sActualFontName, bool& bBold, bool& bItalic, bool bIsNeedCMap)
{
	bBold = false, bItalic = false;
	XRef* xref = pdfDoc->getXRef();

	Object oFont;
	if (!xref->fetch(oFontRef->getRefNum(), oFontRef->getRefGen(), &oFont)->isDict())
	{
		oFont.free();
		return L"";
	}

	GfxFont* gfxFont = GfxFont::makeFont(xref, "F", oFontRef->getRef(), oFont.getDict());
	oFont.free();
	if (!gfxFont)
		return L"";

	Ref oEmbRef;
	std::wstring wsFontBaseName = NSStrings::GetStringFromUTF32(gfxFont->getName());
	std::wstring wsFileName;

	if (gfxFont->getEmbeddedFontID(&oEmbRef) || IsBaseFont(wsFontBaseName))
	{
		std::wstring wsFontName;
		RendererOutputDev::GetFont(xref, pFontManager, pFontList, gfxFont, wsFileName, wsFontName, false);

		sFontName = U_TO_UTF8(wsFontName);
		CheckFontStylePDF(wsFontName, bBold, bItalic);
		if (!bBold)
			bBold = gfxFont->isBold();
		if (!bItalic)
			bItalic = gfxFont->isItalic();
	}
	else
	{
		std::wstring wsFBN = wsFontBaseName;
		NSFonts::CFontInfo* pFontInfo = RendererOutputDev::GetFontByParams(xref, pFontManager, gfxFont, wsFBN);
		if (pFontInfo && !pFontInfo->m_wsFontPath.empty())
		{
			EraseSubsetTag(wsFontBaseName);

			wsFileName = pFontInfo->m_wsFontPath;
			sFontName  = U_TO_UTF8(wsFontBaseName);
			sActualFontName = U_TO_UTF8(pFontInfo->m_wsFontName);
			bBold = pFontInfo->m_bBold;
			bItalic = pFontInfo->m_bItalic;
		}
	}

	RELEASEOBJECT(gfxFont);
	return wsFileName;
}
bool GetFontFromAP(PDFDoc* pdfDoc, AcroFormField* pField, Object* oFontRef, std::string& sFontKey)
{
	bool bFindResources = false;

	Object oAP, oN;
	XRef* xref = pdfDoc->getXRef();
	if (pField->fieldLookup("AP", &oAP)->isDict() && oAP.dictLookup("N", &oN)->isStream())
	{
		Parser* parser = new Parser(xref, new Lexer(xref, &oN), gFalse);

		bool bFindFont = false;
		Object oObj1, oObj2, oObj3;
		parser->getObj(&oObj1);
		while (!oObj1.isEOF())
		{
			if (oObj1.isName())
			{
				parser->getObj(&oObj2);
				if (oObj2.isEOF())
					break;
				if (oObj2.isNum())
				{
					parser->getObj(&oObj3);
					if (oObj3.isEOF())
						break;
					if (oObj3.isCmd("Tf"))
					{
						bFindFont = true;
						break;
					}
				}
			}
			if (oObj2.isName())
			{
				oObj1.free();
				oObj2.copy(&oObj1);
				oObj2.free(); oObj3.free();
				continue;
			}
			if (oObj3.isName())
			{
				oObj1.free();
				oObj3.copy(&oObj1);
				oObj3.free(); oObj2.free();
				continue;
			}
			oObj1.free(); oObj2.free(); oObj3.free();

			parser->getObj(&oObj1);
		}

		if (bFindFont && oObj1.isName())
		{
			Object oR, oFonts;
			bFindResources = oN.streamGetDict()->lookup("Resources", &oR)->isDict() && oR.dictLookup("Font", &oFonts)->isDict() && oFonts.dictLookupNF(oObj1.getName(), oFontRef)->isRef();
			sFontKey = oObj1.getName();
			oR.free(); oFonts.free();
		}

		oObj1.free(); oObj2.free(); oObj3.free();
		RELEASEOBJECT(parser);
	}
	oAP.free(); oN.free();

	return bFindResources;
}
bool FindFonts(Object* oStream, int nDepth, Object* oResFonts)
{
	if (nDepth > 5)
		return false;

	Object oResources;
	if (!oStream->streamGetDict()->lookup("Resources", &oResources)->isDict())
	{
		oResources.free();
		return false;
	}

	if (oResources.dictLookup("Font", oResFonts)->isDict())
	{
		oResources.free();
		return true;
	}

	Object oXObject;
	if (oResources.dictLookup("XObject", &oXObject)->isDict())
	{
		for (int i = 0, nLength = oXObject.dictGetLength(); i < nLength; ++i)
		{
			Object oXObj;
			if (!oXObject.dictGetVal(i, &oXObj)->isStream())
			{
				oXObj.free();
				continue;
			}
			if (FindFonts(&oXObj, nDepth + 1, oResFonts))
			{
				oXObj.free(); oXObject.free(); oResources.free();
				return true;
			}
			oXObj.free();
		}
	}
	oXObject.free(); oResources.free();
	return false;
}
std::vector<CAnnotFontInfo> GetAnnotFontInfos(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, Object* oAnnotRef)
{
	std::vector<CAnnotFontInfo> result;

	Object oAnnot, oObj;
	XRef* pXref = pdfDoc->getXRef();
	oAnnotRef->fetch(pXref, &oAnnot);

	Object oAP, oN;
	if (!oAnnot.dictLookup("AP", &oAP)->isDict() || !oAP.dictLookup("N", &oN)->isStream())
	{
		oAP.free(); oN.free(); oAnnot.free();
		return result;
	}
	oAP.free();

	Object oFonts;
	if (!FindFonts(&oN, 0, &oFonts))
	{
		oN.free(); oFonts.free(); oAnnot.free();
		return result;
	}
	oN.free();

	for (int i = 0, nFonts = oFonts.dictGetLength(); i < nFonts; ++i)
	{
		Object oFontRef;
		if (!oFonts.dictGetValNF(i, &oFontRef)->isRef())
		{
			oFontRef.free();
			continue;
		}

		CAnnotFontInfo info;
		std::string sFontName, sActualFontName;
		info.wsFontPath = GetFontData(pdfDoc, pFontManager, pFontList, &oFontRef, sFontName, sActualFontName, info.bBold, info.bItalic);
		oFontRef.free();

		if (info.wsFontPath.empty() || IsBaseFont(info.wsFontPath))
			continue;

		info.wsFontName = UTF8_TO_U(sFontName);
		if (!sActualFontName.empty())
			info.wsActualFontName = UTF8_TO_U(sActualFontName);

		result.push_back(info);
	}

	oFonts.free(); oAnnot.free();
	return result;
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
const CAnnotFontInfo* FindMatchInAnnotFonts(const std::vector<CAnnotFontInfo>& annotFonts, const std::wstring& wsRCName, bool bBold, bool bItalic)
{
	std::wstring wsReqNorm = Normalize(wsRCName);

	const CAnnotFontInfo* pBestMatch = nullptr;
	int nBestPenalty = INT_MAX;

	for (const auto& fi : annotFonts)
	{
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
		int nItalicPenalty = (fi.bItalic != bItalic)  ? 4 : 0;

		int nTotalPenalty = nNamePenalty + nBoldPenalty + nItalicPenalty;

		if (nTotalPenalty < nBestPenalty)
		{
			nBestPenalty = nTotalPenalty;
			pBestMatch   = &fi;
		}

		if (nTotalPenalty == 0)
			break;
	}

	return pBestMatch;
}

std::map<std::wstring, std::wstring> GetFreeTextFont(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, CPdfFontList* pFontList, Object* oAnnotRef, std::vector<CAnnotMarkup::CFontData*>& arrRC)
{
	std::map<std::wstring, std::wstring> mRes;

	std::vector<CAnnotFontInfo> annotFonts = GetAnnotFontInfos(pdfDoc, pFontManager, pFontList, oAnnotRef);

	NSFonts::IFontsMemoryStorage* pMemoryStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
	CFontList* pAppFontList = (CFontList*)pFontManager->GetApplication()->GetList();

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
				if (bBold && bItalic)        sFontName = "Times-BoldItalic";
				else if (bBold)              sFontName = "Times-Bold";
				else if (bItalic)            sFontName = "Times-Italic";
			}
			else if (sFontName == "Courier" || sFontName == "Helvetica")
			{
				if (bBold && bItalic)        sFontName += "-BoldOblique";
				else if (bBold)              sFontName += "-Bold";
				else if (bItalic)            sFontName += "-Oblique";
			}
			wsFontName = UTF8_TO_U(sFontName);

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
			const CAnnotFontInfo* pMatch = FindMatchInAnnotFonts(annotFonts, wsFontName, bBold, bItalic);

			std::wstring wsResolvedName;
			std::wstring wsResolvedPath;
			bool bFoundInAnnot = false;

			if (pMatch)
			{
				wsResolvedPath = pMatch->wsFontPath;
				wsResolvedName = pMatch->wsActualFontName.empty() ? pMatch->wsFontName : pMatch->wsActualFontName;
				EraseSubsetTag(wsResolvedName);
				bFoundInAnnot = true;
			}
			else
			{
				NSFonts::CFontSelectFormat oFontSelect;
				if (bBold)   oFontSelect.bBold   = new INT(1);
				if (bItalic) oFontSelect.bItalic = new INT(1);
				oFontSelect.wsName = new std::wstring(wsFontName);

				NSFonts::CFontInfo* pFontInfo = pAppFontList->GetByParams(oFontSelect);
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
void CollectFontWidths(GfxFont* gfxFont, Dict* pFontDict, std::map<unsigned int, unsigned int>& mGIDToWidth)
{
	// Пытаемся получить ширины из словаря Widths
	Object oWidths;
	if (pFontDict->lookup("Widths", &oWidths)->isArray())
	{
		Object oFirstChar;
		int nFirstChar = 0;
		if (pFontDict->lookup("FirstChar", &oFirstChar)->isInt())
			nFirstChar = oFirstChar.getInt();
		oFirstChar.free();

		for (int i = 0; i < oWidths.arrayGetLength(); ++i)
		{
			Object oWidth;
			if (oWidths.arrayGet(i, &oWidth)->isNum())
			{
				unsigned int nGID = nFirstChar + i;
				unsigned int nWidth = (unsigned int)oWidth.getNum();
				mGIDToWidth[nGID] = nWidth;
			}
			oWidth.free();
		}
	}
	oWidths.free();

	// Для CID шрифтов обрабатываем DW и W
	Object oDescendantFonts;
	if (pFontDict->lookup("DescendantFonts", &oDescendantFonts)->isArray() && oDescendantFonts.arrayGetLength() > 0)
	{
		Object oCIDFont;
		if (oDescendantFonts.arrayGet(0, &oCIDFont)->isDict())
		{
			// Получаем DW (default width)
			Object oDW;
			int nDefaultWidth = 1000;
			if (oCIDFont.dictLookup("DW", &oDW)->isInt())
				nDefaultWidth = oDW.getInt();
			oDW.free();

			// Получаем W (widths array)
			Object oW;
			if (oCIDFont.dictLookup("W", &oW)->isArray())
			{
				int i = 0;
				while (i < oW.arrayGetLength())
				{
					Object oStart, oSecond, oThird;
					if (!oW.arrayGet(i, &oStart)->isInt())
					{
						oStart.free();
						break;
					}

					if (i + 1 >= oW.arrayGetLength())
					{
						oStart.free();
						break;
					}

					oW.arrayGet(i + 1, &oSecond);

					if (oSecond.isArray())
					{
						// Format: c [w1 w2 ... wn]
						int nStartCID = oStart.getInt();
						for (int j = 0; j < oSecond.arrayGetLength(); ++j)
						{
							Object oWidth;
							if (oSecond.arrayGet(j, &oWidth)->isNum())
								mGIDToWidth[nStartCID + j] = (unsigned int)oWidth.getNum();
							oWidth.free();
						}
						i += 2;
					}
					else if (oSecond.isInt())
					{
						// Format: cfirst clast w
						if (i + 2 >= oW.arrayGetLength())
						{
							oStart.free(); oSecond.free();
							break;
						}

						oW.arrayGet(i + 2, &oThird);
						if (oThird.isNum())
						{
							int nStartCID = oStart.getInt();
							int nEndCID = oSecond.getInt();
							unsigned int nWidth = (unsigned int)oThird.getNum();
							for (int cid = nStartCID; cid <= nEndCID; ++cid)
								mGIDToWidth[cid] = nWidth;
						}
						oThird.free();
						i += 3;
					}
					else
					{
						oStart.free(); oSecond.free();
						break;
					}

					oStart.free(); oSecond.free();
				}
			}
			oW.free();
		}
		oCIDFont.free();
	}
	oDescendantFonts.free();
}
void CheckFontStylePDF(std::wstring& sName, bool& bBold, bool& bItalic)
{
	EraseSubsetTag(sName);

	CheckFontNameStyle(sName, L"condensedbold");
	CheckFontNameStyle(sName, L"semibold");
	CheckFontNameStyle(sName, L"regular");

	CheckFontNameStyle(sName, L"ultraexpanded");
	CheckFontNameStyle(sName, L"extraexpanded");
	CheckFontNameStyle(sName, L"semiexpanded");
	CheckFontNameStyle(sName, L"expanded");

	CheckFontNameStyle(sName, L"ultracondensed");
	CheckFontNameStyle(sName, L"extracondensed");
	CheckFontNameStyle(sName, L"semicondensed");
	CheckFontNameStyle(sName, L"condensedlight");
	CheckFontNameStyle(sName, L"condensed");
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
}
bool EraseSubsetTag(std::wstring& sFontName)
{
	bool bIsRemove = false;
	if (sFontName.length() > 7 && sFontName.at(6) == '+')
	{
		bIsRemove = true;
		for (int nIndex = 0; nIndex < 6; nIndex++)
		{
			wchar_t nChar = sFontName.at(nIndex);
			if (nChar < 'A' || nChar > 'Z')
			{
				bIsRemove = false;
				break;
			}
		}
		if (bIsRemove)
			sFontName.erase(0, 7);
	}
	return bIsRemove;
}
}
