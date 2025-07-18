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

#define errMemory 12

#include "PdfReader.h"

#include "../DesktopEditor/graphics/IRenderer.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/common/StringExt.h"
#include "../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"

#include "SrcReader/Adaptors.h"
#include "SrcReader/PdfAnnot.h"
#include "Resources/BaseFonts.h"

#include "lib/xpdf/PDFDoc.h"
#include "lib/xpdf/GlobalParams.h"
#include "lib/xpdf/ErrorCodes.h"
#include "lib/xpdf/TextString.h"
#include "lib/xpdf/Lexer.h"
#include "lib/xpdf/Parser.h"
#include "lib/xpdf/Outline.h"
#include "lib/xpdf/Link.h"
#include "lib/xpdf/TextOutputDev.h"
#include "lib/xpdf/AcroForm.h"
#include "lib/goo/GList.h"

NSFonts::IFontManager* InitFontManager(NSFonts::IApplicationFonts* pAppFonts)
{
	NSFonts::IFontManager* m_pFontManager = pAppFonts->GenerateFontManager();
	NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
	pMeasurerCache->SetStreams(pAppFonts->GetStreams());
	m_pFontManager->SetOwnerCache(pMeasurerCache);
	pMeasurerCache->SetCacheSize(1);
	return m_pFontManager;
}
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

CPdfReaderContext::~CPdfReaderContext()
{
	if (m_pFontList)
	{
		m_pFontList->Clear();
		RELEASEOBJECT(m_pFontList)
	}
	RELEASEOBJECT(m_pDocument);
}

CPdfReader::CPdfReader(NSFonts::IApplicationFonts* pAppFonts)
{
	m_nFileLength  = 0;

	globalParams  = new GlobalParamsAdaptor(NULL);
#ifndef _DEBUG
	globalParams->setErrQuiet(gTrue);
#endif

	// Создаем менеджер шрифтов с собственным кэшем
	m_pFontManager = InitFontManager(pAppFonts);
#ifndef BUILDING_WASM_MODULE
	globalParams->setupBaseFonts(NULL);
	SetCMapFile(NSFile::GetProcessDirectory() + L"/cmap.bin");
#else
	globalParams->setDrawFormFields(gFalse);
	globalParams->setDrawAnnotations(gFalse);
	SetCMapMemory(NULL, 0);
#endif

	m_eError = errNone;
}
CPdfReader::~CPdfReader()
{
	Clear();

	if (!m_wsTempFolder.empty())
		NSDirectory::DeleteDirectory(m_wsTempFolder);

	RELEASEOBJECT(globalParams);
	RELEASEINTERFACE(m_pFontManager);
}
void CPdfReader::Clear()
{
	for (CPdfReaderContext* pPDFContext : m_vPDFContext)
		delete pPDFContext;
	m_vPDFContext.clear();
}
void CPdfReader::CleanUp()
{
	while(UnmergePages());
	m_eError = errNone;
}

bool CPdfReader::IsNeedCMap()
{
	if (!((GlobalParamsAdaptor*)globalParams)->IsNeedCMap())
		return false;

	if (m_vPDFContext.empty())
		return false;

	std::vector<std::string> arrCMap = {"GB-EUC-H", "GB-EUC-V", "GB-H", "GB-V", "GBpc-EUC-H", "GBpc-EUC-V", "GBK-EUC-H",
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
	std::vector<int> arrUniqueResources;

	for (CPdfReaderContext* pPDFContext : m_vPDFContext)
	{
		PDFDoc* pDoc = pPDFContext->m_pDocument;
		if (!pDoc || !pDoc->getCatalog())
			continue;

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
			continue;
		Object oDR;
		Object* oAcroForm = pAcroForms->getAcroFormObj();
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
	}
	return false;
}
void CPdfReader::SetCMapMemory(BYTE* pData, DWORD nSizeData)
{
	((GlobalParamsAdaptor*)globalParams)->SetCMapMemory(pData, nSizeData);

	if (m_vPDFContext.empty())
		return;
	CPdfReaderContext* pPDFContext = m_vPDFContext.back();
	std::map<std::wstring, std::wstring> mFonts = PdfReader::CAnnotFonts::GetAllFonts(pPDFContext->m_pDocument, m_pFontManager, pPDFContext->m_pFontList, false);
	m_mFonts.insert(mFonts.begin(), mFonts.end());
}
void CPdfReader::SetCMapFolder(const std::wstring& sFolder)
{
	((GlobalParamsAdaptor*)globalParams)->SetCMapFolder(sFolder);
}
void CPdfReader::SetCMapFile(const std::wstring& sFile)
{
	((GlobalParamsAdaptor*)globalParams)->SetCMapFile(sFile);
}

bool CPdfReader::LoadFromFile(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsSrcPath, const std::wstring& wsOwnerPassword, const std::wstring& wsUserPassword)
{
	Clear();
	RELEASEINTERFACE(m_pFontManager);
	m_pFontManager = InitFontManager(pAppFonts);

	if (m_wsTempFolder == L"")
		SetTempDirectory(NSDirectory::GetTempPath());

	m_eError = errNone;
	NSFile::CFileBinary oFile;
	if (oFile.OpenFile(wsSrcPath))
	{
		m_nFileLength = oFile.GetFileSize();
		oFile.CloseFile();
	}
	return MergePages(wsSrcPath, wsOwnerPassword);
}
bool CPdfReader::LoadFromMemory(NSFonts::IApplicationFonts* pAppFonts, BYTE* data, DWORD length, const std::wstring& wsOwnerPassword, const std::wstring& wsUserPassword)
{
	Clear();
	RELEASEINTERFACE(m_pFontManager);
	m_pFontManager = InitFontManager(pAppFonts);

	m_eError = errNone;
	m_nFileLength = length;

	// Все LoadFromMemory копируют память в свои классы
	// Кроме того MemStream использует malloc/free память
	BYTE* pCopy = (BYTE*)malloc(length);
	memcpy(pCopy, data, length);
	data = pCopy;

	return MergePages(data, length, wsOwnerPassword);
}
void CPdfReader::Close()
{
	Clear();
	m_mFonts.clear();
}
void CPdfReader::SetParams(COfficeDrawingPageParams* pParams)
{
	if (!pParams)
		return;

	GBool bDraw = pParams->m_bNeedDrawAnnotation ? gTrue : gFalse;
	globalParams->setDrawFormFields(bDraw);
	globalParams->setDrawAnnotations(bDraw);
}

int CPdfReader::GetStartRefID(PDFDoc* _pDoc)
{
	for (CPdfReaderContext* pPDFContext : m_vPDFContext)
	{
		if (!pPDFContext || !pPDFContext->m_pDocument)
			continue;
		PDFDoc* pDoc = pPDFContext->m_pDocument;
		if (_pDoc == pDoc)
			return pPDFContext->m_nStartID;
	}
	return -1;
}
int CPdfReader::GetNumPagesBefore(PDFDoc* _pDoc)
{
	int nPagesBefore = 0;
	for (CPdfReaderContext* pPDFContext : m_vPDFContext)
	{
		if (!pPDFContext || !pPDFContext->m_pDocument)
			continue;
		PDFDoc* pDoc = pPDFContext->m_pDocument;
		if (_pDoc == pDoc)
			return nPagesBefore;
		nPagesBefore += pDoc->getNumPages();
	}
	return -1;
}
int CPdfReader::FindRefNum(int nObjID, PDFDoc** _pDoc, int* _nStartRefID)
{
	for (CPdfReaderContext* pPDFContext : m_vPDFContext)
	{
		if (!pPDFContext || !pPDFContext->m_pDocument)
			continue;
		PDFDoc* pDoc = pPDFContext->m_pDocument;
		if (nObjID < pPDFContext->m_nStartID + pDoc->getXRef()->getNumObjects())
		{
			if (_pDoc)
				*_pDoc = pDoc;
			if (_nStartRefID)
				*_nStartRefID = pPDFContext->m_nStartID;
			return nObjID - pPDFContext->m_nStartID;
		}
	}
	return -1;
}
int CPdfReader::GetPageIndex(int nAbsPageIndex, PDFDoc** _pDoc, PdfReader::CPdfFontList** pFontList, int* nStartRefID)
{
	int nTotalPages = 0;
	for (CPdfReaderContext* pPDFContext : m_vPDFContext)
	{
		if (!pPDFContext || !pPDFContext->m_pDocument)
			continue;
		PDFDoc* pDoc = pPDFContext->m_pDocument;

		int nPages = pDoc->getNumPages();
		if (nAbsPageIndex < nTotalPages + nPages)
		{
			if (_pDoc)
				*_pDoc = pDoc;
			if (pFontList)
				*pFontList = pPDFContext->m_pFontList;
			if (nStartRefID)
				*nStartRefID = pPDFContext->m_nStartID;
			return nAbsPageIndex - nTotalPages + 1;
		}
		nTotalPages += nPages;
	}
	return -1;
}
int CPdfReader::GetError()
{
	if (m_vPDFContext.empty())
		return m_eError;

	for (CPdfReaderContext* pPDFContext : m_vPDFContext)
	{
		PDFDoc* pDoc = pPDFContext->m_pDocument;
		if (!pDoc)
			return m_eError;

		if (pDoc->isOk() == gFalse)
			return pDoc->getErrorCode();
	}
	return 0;
}
void CPdfReader::GetPageInfo(int _nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	PDFDoc* pDoc = NULL;
	int nPageIndex = GetPageIndex(_nPageIndex, &pDoc);
	if (nPageIndex < 0 || !pDoc)
		return;

#ifdef BUILDING_WASM_MODULE
	*pdWidth  = pDoc->getPageCropWidth(nPageIndex);
	*pdHeight = pDoc->getPageCropHeight(nPageIndex);
#else
	int nRotate = pDoc->getPageRotate(nPageIndex);
	if (nRotate % 180 == 0)
	{
		*pdWidth  = pDoc->getPageCropWidth(nPageIndex);
		*pdHeight = pDoc->getPageCropHeight(nPageIndex);
	}
	else
	{
		*pdHeight = pDoc->getPageCropWidth(nPageIndex);
		*pdWidth  = pDoc->getPageCropHeight(nPageIndex);
	}
#endif

	*pdDpiX = 72.0;
	*pdDpiY = 72.0;
}
int CPdfReader::GetRotate(int _nPageIndex)
{
	PDFDoc* pDoc = NULL;
	int nPageIndex = GetPageIndex(_nPageIndex, &pDoc);
	if (nPageIndex < 0 || !pDoc)
		return 0;
	return pDoc->getPageRotate(nPageIndex);
}
int CPdfReader::GetMaxRefID()
{
	if (!m_vPDFContext.empty())
		return m_vPDFContext.back()->m_nStartID + m_vPDFContext.back()->m_pDocument->getXRef()->getNumObjects();
	return 0;
}
int CPdfReader::GetNumPages()
{
	int nNumPages = 0;
	for (CPdfReaderContext* pPDFContext : m_vPDFContext)
	{
		if (!pPDFContext || !pPDFContext->m_pDocument)
			continue;
		nNumPages += pPDFContext->m_pDocument->getNumPages();
	}
	return nNumPages;
}
bool CPdfReader::ValidMetaData()
{
	if (m_vPDFContext.empty() || m_vPDFContext.size() != 1)
		return false;

	CPdfReaderContext* pPDFContext = m_vPDFContext.front();
	if (!pPDFContext || !pPDFContext->m_pDocument)
		return false;

	XRef* xref = pPDFContext->m_pDocument->getXRef();
	Object oMeta, oID;
	if (!xref->fetch(1, 0, &oMeta)->isStream("MetaOForm") || !oMeta.streamGetDict()->lookup("ID", &oID)->isString())
	{
		oMeta.free(); oID.free();
		return false;
	}
	oMeta.free();

	Object oTID, oID2;
	Object* pTrailerDict = xref->getTrailerDict();
	if (!pTrailerDict->dictLookup("ID", &oTID)->isArray() || !oTID.arrayGet(1, &oID2)->isString())
	{
		oID.free(); oTID.free(); oID2.free();
		return false;
	}
	oTID.free();

	bool bRes = oID2.getString()->cmp(oID.getString()) == 0;
	oID.free();

	int nNumXRefTables = xref->getNumXRefTables();
	if (bRes && nNumXRefTables > 1)
	{
		GFileOffset nOffeset = xref->getXRefTablePos(nNumXRefTables - 2);
		BaseStream* str = pPDFContext->m_pDocument->getBaseStream();
		str->setPos(nOffeset);
		Object oDict, obj1, obj2, obj3, obj4;
		char buf[100];

		int i, n = str->getBlock(buf, 100);
		for (i = 0; i < n && Lexer::isSpace(buf[i]); ++i);
		// xref table
		if (i + 4 < n && buf[i] == 'x' && buf[i+1] == 'r' && buf[i+2] == 'e' && buf[i+3] == 'f' && Lexer::isSpace(buf[i+4]))
		{
			Stream* pSubStr = str->makeSubStream(nOffeset + i + 5, gFalse, 0, &oDict);
			int c = pSubStr->getChar();
			while (c != 't')
				c = pSubStr->getChar();
			pSubStr->getBlock(buf, 6);

			Parser* parser = new Parser(NULL, new Lexer(NULL, pSubStr->getBaseStream()->makeSubStream(pSubStr->getPos(), gFalse, 0, &oDict)), gTrue);
			parser->getObj(&obj1);
			delete parser;
			delete pSubStr;
			if (obj1.isDict() && obj1.dictLookup("ID", &oTID)->isArray() && oTID.arrayGet(1, &oID)->isString())
			{
				bRes = oID2.getString()->cmp(oID.getString()) != 0;
			}
			obj1.free();
			oTID.free();
		}
		else // xref stream
		{
			Stream* pSubStr = str->makeSubStream(nOffeset, gFalse, 0, &oDict);
			Parser* parser = new Parser(NULL, new Lexer(NULL, pSubStr), gTrue);
			parser->getObj(&obj1);
			parser->getObj(&obj2);
			parser->getObj(&obj3);
			parser->getObj(&obj4);
			if (obj1.isInt() && obj2.isInt() && obj3.isCmd("obj") && obj4.isStream())
			{
				Dict* pPrevXRefDict = obj4.streamGetDict();
				if (pPrevXRefDict->lookup("ID", &oTID)->isArray() && oTID.arrayGet(1, &oID)->isString())
				{
					bRes = oID2.getString()->cmp(oID.getString()) != 0;
				}
				oTID.free();
			}
			obj4.free();
			obj3.free();
			obj2.free();
			obj1.free();
			delete parser;
		}
	}
	oID2.free(); oID.free();

	return bRes;
}
bool CPdfReader::MergePages(BYTE* pData, DWORD nLength, const std::wstring& wsPassword, int nMaxID, const std::string& sPrefixForm)
{
	if (m_eError)
	{
		free(pData);
		return false;
	}

	GString* owner_pswd = NSStrings::CreateString(wsPassword);
	GString* user_pswd  = NSStrings::CreateString(wsPassword);

	Object obj;
	obj.initNull();
	// будет освобожден в деструкторе PDFDoc
	// Время его жизни > copy и makeSubStream из MemStream
	BaseStream *str = new MemStream((char*)pData, 0, nLength, &obj, gTrue);
	CPdfReaderContext* pContext = new CPdfReaderContext();
	pContext->m_pDocument = new PDFDoc(str, owner_pswd, user_pswd);
	pContext->m_pFontList = new PdfReader::CPdfFontList();
	pContext->m_sPrefixForm = sPrefixForm;
	if (nMaxID != 0)
		pContext->m_nStartID = nMaxID;
	else if (!m_vPDFContext.empty())
		pContext->m_nStartID = m_vPDFContext.back()->m_nStartID + m_vPDFContext.back()->m_pDocument->getXRef()->getNumObjects();
	PDFDoc* pDoc = pContext->m_pDocument;
	m_vPDFContext.push_back(pContext);

	delete owner_pswd;
	delete user_pswd;

	m_eError = pDoc ? pDoc->getErrorCode() : errMemory;
	if (!pDoc || !pDoc->isOk())
	{
		// pData освобождается
		delete pContext;
		m_vPDFContext.pop_back();
		return false;
	}

	std::map<std::wstring, std::wstring> mFonts = PdfReader::CAnnotFonts::GetAllFonts(pDoc, m_pFontManager, pContext->m_pFontList, IsNeedCMap());
	m_mFonts.insert(mFonts.begin(), mFonts.end());

	return true;
}
bool CPdfReader::MergePages(const std::wstring& wsFile, const std::wstring& wsPassword, int nMaxID, const std::string& sPrefixForm)
{
	if (m_eError)
			return false;
	GString* owner_pswd = NSStrings::CreateString(wsPassword);
	GString* user_pswd  = NSStrings::CreateString(wsPassword);
	// конвертим путь в utf8 - под виндой они сконвертят в юникод, а на остальных - так и надо
	std::string sPathUtf8 = U_TO_UTF8(wsFile);

	CPdfReaderContext* pContext = new CPdfReaderContext();
	pContext->m_pDocument = new PDFDoc((char*)sPathUtf8.c_str(), owner_pswd, user_pswd);
	pContext->m_pFontList = new PdfReader::CPdfFontList();
	pContext->m_sPrefixForm = sPrefixForm;
	if (nMaxID != 0)
		pContext->m_nStartID = nMaxID;
	else if (!m_vPDFContext.empty())
		pContext->m_nStartID = m_vPDFContext.back()->m_nStartID + m_vPDFContext.back()->m_pDocument->getXRef()->getNumObjects();
	PDFDoc* pDoc = pContext->m_pDocument;
	m_vPDFContext.push_back(pContext);

	delete owner_pswd;
	delete user_pswd;

	m_eError = pDoc ? pDoc->getErrorCode() : errMemory;
	if (!pDoc || !pDoc->isOk())
	{
		delete pContext;
		m_vPDFContext.pop_back();
		return false;
	}

	std::map<std::wstring, std::wstring> mFonts = PdfReader::CAnnotFonts::GetAllFonts(pDoc, m_pFontManager, pContext->m_pFontList, IsNeedCMap());
	m_mFonts.insert(mFonts.begin(), mFonts.end());

	return true;
}
bool CPdfReader::UnmergePages()
{
	if (m_vPDFContext.size() <= 1)
		return false;
	CPdfReaderContext* pPDFContext = m_vPDFContext.back();
	delete pPDFContext;
	m_vPDFContext.pop_back();
	return true;
}
void CPdfReader::DrawPageOnRenderer(IRenderer* pRenderer, int _nPageIndex, bool* pbBreak)
{
	PDFDoc* pDoc = NULL;
	PdfReader::CPdfFontList* pFontList = NULL;
	int nPageIndex = GetPageIndex(_nPageIndex, &pDoc, &pFontList);
	if (nPageIndex < 0 || !pDoc || !pFontList)
		return;

	PdfReader::RendererOutputDev oRendererOut(pRenderer, m_pFontManager, pFontList);
	oRendererOut.NewPDF(pDoc->getXRef());
	oRendererOut.SetBreak(pbBreak);
	int nRotate = 0;
#ifdef BUILDING_WASM_MODULE
	nRotate = -pDoc->getPageRotate(nPageIndex);
#endif
	pDoc->displayPage(&oRendererOut, nPageIndex, 72.0, 72.0, nRotate, gFalse, gTrue, gFalse);
}
void CPdfReader::SetTempDirectory(const std::wstring& wsTempFolder)
{
	if (!m_wsTempFolder.empty())
		NSDirectory::DeleteDirectory(m_wsTempFolder);

	if (!wsTempFolder.empty())
	{
		std::wstring wsFolderName = wsTempFolder + L"/pdftemp";
		std::wstring wsFolder = wsFolderName;
		int nCounter = 0;
		while (NSDirectory::Exists(wsFolder))
		{
			nCounter++;
			wsFolder = wsFolderName + L"_" + std::to_wstring(nCounter);
		}
		NSDirectory::CreateDirectory(wsFolder);
		m_wsTempFolder = wsFolder;
	}
	else
		m_wsTempFolder = L"";

	if (globalParams)
		((GlobalParamsAdaptor*)globalParams)->SetTempFolder(m_wsTempFolder);
}
std::wstring CPdfReader::GetTempDirectory()
{
	return m_wsTempFolder;
}
std::wstring CPdfReader::ToXml(const std::wstring& wsFilePath, bool isPrintStream)
{
	XMLConverter oConverter(m_vPDFContext.front()->m_pDocument->getXRef(), isPrintStream);
	std::wstring wsXml = oConverter.GetXml();

	if (wsFilePath != L"")
	{
		NSFile::CFileBinary oFile;
		if (!oFile.CreateFileW(wsFilePath))
			return wsXml;

		oFile.WriteStringUTF8(wsXml);
		oFile.CloseFile();
	}

	return wsXml;
}
PDFDoc* CPdfReader::GetLastPDFDocument()
{
	if (m_vPDFContext.empty())
		return NULL;

	CPdfReaderContext* pPDFContext = m_vPDFContext.back();
	if (pPDFContext)
		return pPDFContext->m_pDocument;
	return NULL;
}
PDFDoc* CPdfReader::GetPDFDocument(int PDFIndex)
{
	if (PDFIndex >= 0 && PDFIndex < m_vPDFContext.size())
		return m_vPDFContext[PDFIndex]->m_pDocument;
	return NULL;
}

std::wstring CPdfReader::GetInfo()
{
	std::wstring sRes;
	if (m_vPDFContext.empty())
		return sRes;

	CPdfReaderContext* pPDFContext = m_vPDFContext.front();
	if (!pPDFContext || !pPDFContext->m_pDocument)
		return sRes;

	PDFDoc* pDoc = pPDFContext->m_pDocument;
	XRef* xref = pDoc->getXRef();
	BaseStream* str = pDoc->getBaseStream();
	if (!xref || !str)
		return NULL;

	sRes = L"{";

	Object oInfo;
	if (pDoc->getDocInfo(&oInfo)->isDict())
	{
		auto fDictLookup = [&oInfo](const char* sName, const wchar_t* wsName)
		{
			std::wstring sRes;
			Object obj1;
			if (oInfo.dictLookup(sName, &obj1)->isString())
			{
				TextString* s = new TextString(obj1.getString());
				std::wstring sValue = NSStringExt::CConverter::GetUnicodeFromUTF32(s->getUnicode(), s->getLength());
				delete s;
				NSStringExt::Replace(sValue, L"\\", L"\\\\");
				NSStringExt::Replace(sValue, L"\"", L"\\\"");
				sValue.erase(std::remove_if(sValue.begin(), sValue.end(), [] (const wchar_t& wc) { return wc < 0x20; } ), sValue.end());
				if (!sValue.empty())
				{
					sRes += L"\"";
					sRes += wsName;
					sRes += L"\":\"";
					sRes += sValue;
					sRes += L"\",";
				}
			}
			obj1.free();
			return sRes;
		};
		sRes += fDictLookup("Title",    L"Title");
		sRes += fDictLookup("Author",   L"Author");
		sRes += fDictLookup("Subject",  L"Subject");
		sRes += fDictLookup("Keywords", L"Keywords");
		sRes += fDictLookup("Creator",  L"Creator");
		sRes += fDictLookup("Producer", L"Producer");

		auto fDictLookupDate = [&oInfo](const char* sName, const wchar_t* wsName)
		{
			std::wstring sRes;
			Object obj1;
			if (!oInfo.dictLookup(sName, &obj1)->isString() || !obj1.getString()->getLength())
			{
				obj1.free();
				return sRes;
			}

			TextString* s = new TextString(obj1.getString());
			std::wstring sNoDate = NSStringExt::CConverter::GetUnicodeFromUTF32(s->getUnicode(), s->getLength());
			if (sNoDate.length() > 16)
			{
				std::wstring sDate = sNoDate.substr(2,  4) + L'-' + sNoDate.substr(6,  2) + L'-' + sNoDate.substr(8,  2) + L'T' +
									 sNoDate.substr(10, 2) + L':' + sNoDate.substr(12, 2) + L':' + sNoDate.substr(14, 2);
				if (sNoDate.length() > 21 && (sNoDate[16] == L'+' || sNoDate[16] == L'-'))
					sDate += (L".000" + sNoDate.substr(16, 3) + L':' + sNoDate.substr(20, 2));
				else
					sDate += L"Z";
				NSStringExt::Replace(sDate, L"\\", L"\\\\");
				NSStringExt::Replace(sDate, L"\"", L"\\\"");
				sDate.erase(std::remove_if(sDate.begin(), sDate.end(), [] (const wchar_t& wc) { return wc < 0x20; } ), sDate.end());
				sRes += L"\"";
				sRes += wsName;
				sRes += L"\":\"";
				sRes += sDate;
				sRes += L"\",";
			}
			delete s;

			obj1.free();
			return sRes;
		};
		sRes += fDictLookupDate("CreationDate", L"CreationDate");
		sRes += fDictLookupDate("ModDate", L"ModDate");
	}
	oInfo.free();

	std::wstring version = std::to_wstring(pDoc->getPDFVersion());
	std::wstring::size_type posDot = version.find('.');
	if (posDot != std::wstring::npos)
		version.resize(posDot + 2);
	if (!version.empty())
		sRes += (L"\"Version\":" + version + L",");

	double nW = 0;
	double nH = 0;
	double nDpi = 0;
	GetPageInfo(0, &nW, &nH, &nDpi, &nDpi);
	sRes += L"\"PageWidth\":";
	sRes += std::to_wstring((int)(nW * 100));
	sRes += L",\"PageHeight\":";
	sRes += std::to_wstring((int)(nH * 100));
	sRes += L",\"NumberOfPages\":";
	sRes += std::to_wstring(GetNumPages());
	sRes += L",\"FastWebView\":";

	bool bLinearized = false;
	if (m_vPDFContext.size() == 1)
	{
		Object obj1, obj2, obj3, obj4, obj5, obj6;
		obj1.initNull();
		Parser* parser = new Parser(xref, new Lexer(xref, str->makeSubStream(str->getStart(), gFalse, 0, &obj1)), gTrue);
		parser->getObj(&obj1);
		parser->getObj(&obj2);
		parser->getObj(&obj3);
		parser->getObj(&obj4);
		if (obj1.isInt() && obj2.isInt() && obj3.isCmd("obj") && obj4.isDict())
		{
			obj4.dictLookup("Linearized", &obj5);
			if (obj5.isNum() && obj5.getNum() > 0 && obj4.dictLookup("L", &obj6)->isNum())
			{
				unsigned long size = (unsigned long)obj6.getNum();
				bLinearized = size == m_nFileLength;
			}
			obj6.free();
			obj5.free();
		}
		obj4.free();
		obj3.free();
		obj2.free();
		obj1.free();
		delete parser;
	}

	sRes += bLinearized ? L"true" : L"false";
	sRes += L",\"Tagged\":";

	bool bTagged = false;
	Object catDict, markInfoObj;
	if (xref->getCatalog(&catDict)->isDict() && catDict.dictLookup("MarkInfo", &markInfoObj)->isDict())
	{
		Object marked, suspects;
		if (markInfoObj.dictLookup("Marked", &marked)->isBool() && marked.getBool() == gTrue)
		{
			bTagged = true;
			// If Suspects is true, the document may not completely conform to Tagged PDF conventions.
			if (markInfoObj.dictLookup("Suspects", &suspects)->isBool() && suspects.getBool() == gTrue)
				bTagged = false;
		}
		marked.free();
		suspects.free();
	}
	markInfoObj.free();
	catDict.free();

	sRes += bTagged ? L"true}" : L"false}";

	return sRes;
}
std::wstring CPdfReader::GetFontPath(const std::wstring& wsFontName, bool bSave)
{
	std::map<std::wstring, std::wstring>::const_iterator oIter = m_mFonts.find(wsFontName);
	return oIter == m_mFonts.end() ? std::wstring() : oIter->second;
}
void getBookmarks(PDFDoc* pdfDoc, OutlineItem* pOutlineItem, NSWasm::CData& out, int level, int nStartPage)
{
	LinkAction* pLinkAction = pOutlineItem->getAction();
	if (!pLinkAction || pLinkAction->getKind() != actionGoTo)
		return;

	GString* str = ((LinkGoTo*)pLinkAction)->getNamedDest();
	LinkDest* pLinkDest = str ? pdfDoc->findDest(str) : ((LinkGoTo*)pLinkAction)->getDest();
	if (!pLinkDest)
		return;
	int pg;
	if (pLinkDest->isPageRef())
	{
		Ref pageRef = pLinkDest->getPageRef();
		pg = pdfDoc->findPage(pageRef.num, pageRef.gen);
	}
	else
		pg = pLinkDest->getPageNum();
	if (pg == 0)
		pg = 1;

	double dy = 0;
	double dTop = pLinkDest->getTop();
	double dHeight = pdfDoc->getPageCropHeight(pg);
	/*
	if (pdfDoc->getPageRotate(pg) % 180 != 0)
	{
		dHeight = pdfDoc->getPageCropWidth(pg);
		dTop = pLinkDest->getLeft();
	}
	*/
	if (dTop > 0 && dTop < dHeight)
		dy = dHeight - dTop;

	if (str)
		RELEASEOBJECT(pLinkDest);

	std::string sTitle = NSStringExt::CConverter::GetUtf8FromUTF32(pOutlineItem->getTitle(), pOutlineItem->getTitleLength());

	out.AddInt(nStartPage + pg - 1);
	out.AddInt(level);
	out.AddDouble(dy);
	out.WriteString((BYTE*)sTitle.c_str(), (unsigned int)sTitle.length());

	pOutlineItem->open();
	GList* pList = pOutlineItem->getKids();
	if (!pList)
	{
		pOutlineItem->close();
		return;
	}
	for (int i = 0, num = pList->getLength(); i < num; i++)
	{
		OutlineItem* pOutlineItemKid = (OutlineItem*)pList->get(i);
		if (pOutlineItemKid)
			getBookmarks(pdfDoc, pOutlineItemKid, out, level + 1, nStartPage);
	}
	pOutlineItem->close();
}
BYTE* CPdfReader::GetStructure()
{
	if (m_vPDFContext.empty())
		return NULL;

	NSWasm::CData oRes;
	oRes.SkipLen();

	int nStartPage = 0;
	for (int iPDF = 0; iPDF < m_vPDFContext.size(); ++iPDF)
	{
		PDFDoc* pDoc = m_vPDFContext[iPDF]->m_pDocument;
		Outline* pOutline = pDoc->getOutline();
		if (!pOutline)
		{
			nStartPage += pDoc->getNumPages();
			continue;
		}

		GList* pList = pOutline->getItems();
		if (!pList)
		{
			nStartPage += pDoc->getNumPages();
			continue;
		}

		if (iPDF > 0)
		{
			oRes.AddInt(nStartPage);
			oRes.AddInt(1);
			oRes.AddDouble(0);
			oRes.WriteString(std::to_string(iPDF)); // TODO Писать имя файла как Adobe?
		}

		for (int i = 0, num = pList->getLength(); i < num; i++)
		{
			OutlineItem* pOutlineItem = (OutlineItem*)pList->get(i);
			if (pOutlineItem)
				getBookmarks(pDoc, pOutlineItem, oRes, iPDF > 0 ? 2 : 1, nStartPage);
		}
		nStartPage += pDoc->getNumPages();
	}

	oRes.WriteLen();

	BYTE* bRes = oRes.GetBuffer();
	oRes.ClearWithoutAttack();
	return bRes;
}
BYTE* CPdfReader::GetLinks(int _nPageIndex)
{
	// TODO Links должны стать частью Annots
	PDFDoc* pDoc = NULL;
	int nPageIndex = GetPageIndex(_nPageIndex, &pDoc);
	if (nPageIndex < 0 || !pDoc || !pDoc->getCatalog())
		return NULL;

	Page* pPage = pDoc->getCatalog()->getPage(nPageIndex);
	if (!pPage)
		return NULL;

	NSWasm::CPageLink oLinks;

	// Гиперссылка
	Links* pLinks = pDoc->getLinks(nPageIndex);
	if (pLinks)
	{
		PDFRectangle* cropBox = pPage->getCropBox();
		for (int i = 0, num = pLinks->getNumLinks(); i < num; i++)
		{
			Link* pLink = pLinks->getLink(i);
			if (!pLink)
				continue;

			GString* str = NULL;
			double x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0, dy = 0.0;
			pLink->getRect(&x1, &y1, &x2, &y2);
			x1 = x1 - cropBox->x1;
			y1 = cropBox->y2 - y1;
			x2 = x2 - cropBox->x1;
			y2 = cropBox->y2 - y2;

			LinkAction* pLinkAction = pLink->getAction();
			if (!pLinkAction)
				continue;
			LinkActionKind kind = pLinkAction->getKind();
			if (kind == actionGoTo)
			{
				str = ((LinkGoTo*)pLinkAction)->getNamedDest();
				LinkDest* pLinkDest = str ? pDoc->findDest(str) : ((LinkGoTo*)pLinkAction)->getDest()->copy();
				if (pLinkDest)
				{
					int pg;
					if (pLinkDest->isPageRef())
					{
						Ref pageRef = pLinkDest->getPageRef();
						pg = pDoc->findPage(pageRef.num, pageRef.gen);
					}
					else
						pg = pLinkDest->getPageNum();
					if (0 == pg)
						++pg;

					std::string sLink = "#" + std::to_string(pg - 1);
					str = new GString(sLink.c_str());
					dy  = pDoc->getPageCropHeight(pg) - pLinkDest->getTop();
				}
				else
					str = NULL;
				RELEASEOBJECT(pLinkDest);
			}
			else if (kind == actionURI)
				str = ((LinkURI*)pLinkAction)->getURI()->copy();
			else if (kind == actionNamed)
			{
				str = ((LinkNamed*)pLinkAction)->getName();
				int pg = 1;
				if (!str->cmp("NextPage"))
				{
					pg = nPageIndex + 1;
					if (pg > pDoc->getNumPages())
						pg = pDoc->getNumPages();
				}
				else if (!str->cmp("PrevPage"))
				{
					pg = nPageIndex - 1;
					if (pg < 1)
						pg = 1;
				}
				else if (!str->cmp("LastPage"))
					pg = pDoc->getNumPages();

				std::string sLink = "#" + std::to_string(pg - 1);
				str = new GString(sLink.c_str());
			}

			oLinks.m_arLinks.push_back({str ? std::string(str->getCString(), str->getLength()) : "", dy, x1, y2, x2 - x1, y1 - y2});
			RELEASEOBJECT(str);
		}
	}
	RELEASEOBJECT(pLinks);

	int nRotate = 0;
#ifdef BUILDING_WASM_MODULE
	nRotate = -pDoc->getPageRotate(nPageIndex);
#endif

	// Текст-ссылка
	TextOutputControl textOutControl;
	textOutControl.mode = textOutReadingOrder;
	TextOutputDev* pTextOut = new TextOutputDev(NULL, &textOutControl, gFalse);
	pDoc->displayPage(pTextOut, nPageIndex, 72.0, 72.0, nRotate, gFalse, gTrue, gFalse);
	pDoc->processLinks(pTextOut, nPageIndex);
	TextWordList* pWordList = pTextOut->makeWordList();
	for (int i = 0; i < pWordList->getLength(); i++)
	{
		TextWord* pWord = pWordList->get(i);
		if (!pWord)
			continue;
		GString* sLink = pWord->getText();
		if (!sLink)
			continue;
		std::string link(sLink->getCString(), sLink->getLength());
		RELEASEOBJECT(sLink);
		size_t find = link.find("http://");
		if (find == std::string::npos)
			find = link.find("https://");
		if (find == std::string::npos)
			find = link.find("www.");
		if (find != std::string::npos)
		{
			link.erase(0, find);
			double x1, y1, x2, y2;
			pWord->getBBox(&x1, &y1, &x2, &y2);
			oLinks.m_arLinks.push_back({link, 0, x1, y1, x2 - x1, y2 - y1});
		}
	}
	RELEASEOBJECT(pWordList);
	RELEASEOBJECT(pTextOut);

	return oLinks.Serialize();
}
BYTE* CPdfReader::GetWidgets()
{
	NSWasm::CData oRes;
	oRes.SkipLen();

	std::map<std::string, std::string> mForms;
	int nStartPage = 0;
	for (int iPDF = 0; iPDF < m_vPDFContext.size(); ++iPDF)
	{
		PDFDoc* pDoc = m_vPDFContext[iPDF]->m_pDocument;
		if (!pDoc || !pDoc->getCatalog())
			continue;
		if (!pDoc->getCatalog()->getForm() || !pDoc->getXRef())
		{
			nStartPage += pDoc->getNumPages();
			continue;
		}

		PdfReader::CAnnots* pAnnots = new PdfReader::CAnnots(pDoc, m_pFontManager, m_vPDFContext[iPDF]->m_pFontList, nStartPage, m_vPDFContext[iPDF]->m_nStartID);
		if (pAnnots)
		{
			const std::vector<PdfReader::CAnnotWidget*>& arrAnnots = pAnnots->GetAnnots();
			for (int i = 0; i < arrAnnots.size(); ++i)
			{
				const std::string& sFullName = arrAnnots[i]->GetFullName();
				std::map<std::string, std::string>::iterator it = mForms.find(sFullName);
				if (it == mForms.end())
					mForms[sFullName] = arrAnnots[i]->GetType();
				else if (mForms[sFullName] != arrAnnots[i]->GetType())
				{
					if (iPDF == 0)
					{
						// error
						// throw "Same full names for forms of different types within the same file";
					}
					else
					{
						int nPrefix = 0;
						std::string sPrefix = m_vPDFContext[iPDF]->m_sPrefixForm + "_" + std::to_string(nPrefix);
						while (!pAnnots->ChangeFullNameAnnot(i, sPrefix))
							sPrefix = m_vPDFContext[iPDF]->m_sPrefixForm + "_" + std::to_string(++nPrefix);
					}
				}
			}

			pAnnots->ToWASM(oRes);
		}
		RELEASEOBJECT(pAnnots);
		nStartPage += pDoc->getNumPages();
	}

	oRes.WriteLen();
	BYTE* bRes = oRes.GetBuffer();
	oRes.ClearWithoutAttack();
	return bRes;
}
BYTE* CPdfReader::GetFonts(bool bStandart)
{
	NSWasm::CData oRes;
	oRes.SkipLen();

	int nFonts = 0;
	int nFontsPos = oRes.GetSize();
	oRes.AddInt(nFonts);

	for (std::map<std::wstring, std::wstring>::const_iterator it = m_mFonts.begin(); it != m_mFonts.end(); ++it)
	{
		if (PdfReader::CAnnotFonts::IsBaseFont(it->second))
		{
			if (bStandart)
			{
				oRes.WriteString(it->first);
				nFonts++;
			}
		}
		else if (!bStandart)
		{
			oRes.WriteString(it->first);
			nFonts++;
		}
	}

	oRes.AddInt(nFonts, nFontsPos);

	oRes.WriteLen();
	BYTE* bRes = oRes.GetBuffer();
	oRes.ClearWithoutAttack();
	return bRes;
}
BYTE* CPdfReader::VerifySign(const std::wstring& sFile, ICertificate* pCertificate, int nWidget)
{
	if (m_vPDFContext.empty())
		return NULL;

	CPdfReaderContext* pPDFContext = m_vPDFContext.front();
	if (!pPDFContext || !pPDFContext->m_pDocument || !pPDFContext->m_pDocument->getCatalog())
		return NULL;

	AcroForm* pAcroForms = pPDFContext->m_pDocument->getCatalog()->getForm();
	if (!pAcroForms)
		return NULL;

	BYTE* pFileData = NULL;
	DWORD nFileSize;
	if (!NSFile::CFileBinary::ReadAllBytes(sFile, &pFileData, nFileSize))
		return NULL;

	NSWasm::CData oRes;
	oRes.SkipLen();

	for (int i = 0, nNum = pAcroForms->getNumFields(); i < nNum; ++i)
	{
		AcroFormField* pField = pAcroForms->getField(i);
		AcroFormFieldType oType = pField->getAcroFormFieldType();
		if (oType != acroFormFieldSignature || (nWidget >= 0 && i != nWidget))
			continue;

		Object oObj, oObj1;
		if (!pField->fieldLookup("V", &oObj)->isDict())
			continue;

		std::vector<int> arrByteOffset, arrByteLength;
		if (oObj.dictLookup("ByteRange", &oObj1)->isArray())
		{
			for (int j = 0; j < oObj1.arrayGetLength(); ++j)
			{
				Object oObjJ;
				if (oObj1.arrayGet(j, &oObjJ)->isInt())
				{
					if (j % 2 == 0)
						arrByteOffset.push_back(oObjJ.getInt());
					else
						arrByteLength.push_back(oObjJ.getInt());
				}
				oObjJ.free();
			}
		}
		oObj1.free();

		DWORD dwLenDataForVerify = 0;
		for (int j = 0; j < arrByteLength.size(); ++j)
			dwLenDataForVerify += arrByteLength[j];
		BYTE* pDataForVerify = new BYTE[dwLenDataForVerify];
		int nByteOffset = 0;
		for (int j = 0; j < arrByteOffset.size(); ++j)
		{
			// TODO проверка длины файла и ByteRange
			memcpy(pDataForVerify + nByteOffset, pFileData + arrByteOffset[j], arrByteLength[j]);
			nByteOffset += arrByteLength[j];
		}

		BYTE* pPKCS7Data = NULL;
		DWORD nPKCS7Size = 0;
		if (oObj.dictLookup("Contents", &oObj1)->isString())
		{
			GString* sContents = oObj1.getString();
			pPKCS7Data = (BYTE*)sContents->getCString();
			nPKCS7Size = sContents->getLength();
		}

		int nRes = pCertificate->VerifyPKCS7(pPKCS7Data, nPKCS7Size, pDataForVerify, dwLenDataForVerify);
		RELEASEARRAYOBJECTS(pDataForVerify);
		oObj1.free();

		// Номер аннотации для сопоставления с AP
		oRes.AddInt(i);
		oRes.AddInt(nRes);
	}

	oRes.WriteLen();
	BYTE* bRes = oRes.GetBuffer();
	oRes.ClearWithoutAttack();
	return bRes;
}
BYTE* CPdfReader::GetAPWidget(int nRasterW, int nRasterH, int nBackgroundColor, int _nPageIndex, int nWidget, const char* sView, const char* sButtonView)
{
	PDFDoc* pDoc = NULL;
	PdfReader::CPdfFontList* pFontList = NULL;
	int nStartRefID = 0;
	int nPageIndex = GetPageIndex(_nPageIndex, &pDoc, &pFontList, &nStartRefID);
	if (nPageIndex < 0 || !pDoc || !pFontList || !pDoc->getCatalog())
		return NULL;

	AcroForm* pAcroForms = pDoc->getCatalog()->getForm();
	if (!pAcroForms)
		return NULL;

	NSWasm::CData oRes;
	oRes.SkipLen();

	for (int i = 0, nNum = pAcroForms->getNumFields(); i < nNum; ++i)
	{
		AcroFormField* pField = pAcroForms->getField(i);
		Object oRef;
		if (pField->getPageNum() != nPageIndex || (nWidget >= 0 && pField->getFieldRef(&oRef) && oRef.getRefNum() + nStartRefID != nWidget))
		{
			oRef.free();
			continue;
		}
		oRef.free();

		PdfReader::CAnnotAP* pAP = new PdfReader::CAnnotAP(pDoc, m_pFontManager, pFontList, nRasterW, nRasterH, nBackgroundColor, nPageIndex, sView, sButtonView, pField, nStartRefID);
		if (pAP)
			pAP->ToWASM(oRes);
		RELEASEOBJECT(pAP);
	}

	oRes.WriteLen();
	BYTE* bRes = oRes.GetBuffer();
	oRes.ClearWithoutAttack();
	return bRes;
}
BYTE* CPdfReader::GetButtonIcon(int nBackgroundColor, int _nPageIndex, bool bBase64, int nButtonWidget, const char* sIconView)
{
	PDFDoc* pDoc = NULL;
	PdfReader::CPdfFontList* pFontList = NULL;
	int nStartRefID = 0;
	int nPageIndex = GetPageIndex(_nPageIndex, &pDoc, &pFontList, &nStartRefID);
	if (nPageIndex < 0 || !pDoc || !pDoc->getCatalog())
		return NULL;

	AcroForm* pAcroForms = pDoc->getCatalog()->getForm();
	if (!pAcroForms)
		return NULL;

	NSWasm::CData oRes;
	oRes.SkipLen();

	std::vector<int> arrUniqueImage;
	for (int i = 0, nNum = pAcroForms->getNumFields(); i < nNum; ++i)
	{
		AcroFormField* pField = pAcroForms->getField(i);
		Object oRef;
		if (pField->getPageNum() != nPageIndex || pField->getAcroFormFieldType() != acroFormFieldPushbutton ||
			(nButtonWidget >= 0 && pField->getFieldRef(&oRef) && oRef.getRefNum() + nStartRefID != nButtonWidget))
		{
			oRef.free();
			continue;
		}
		oRef.free();

		Object oMK;
		if (!pField->fieldLookup("MK", &oMK)->isDict())
		{
			oMK.free();
			continue;
		}
		Object oAP;
		pField->fieldLookup("AP", &oAP);

		bool bFirst = true;
		int nMKPos  = -1;
		unsigned int nMKLength = 0;
		std::vector<const char*> arrMKName { "I", "RI", "IX" };
		std::vector<const char*> arrAPName { "N", "D",  "R" };
		for (unsigned int j = 0; j < arrMKName.size(); ++j)
		{
			if (sIconView && strcmp(sIconView, arrMKName[j]) != 0)
				continue;
			std::string sMKName(arrMKName[j]);
			Object oStr, oXObject, oIm;;
			if (oMK.dictLookup(sMKName.c_str(), &oStr)->isStream())
			{
				// Получение единственного XObject из Resources, если возможно
				Object oResources;
				if (!oStr.streamGetDict()->lookup("Resources", &oResources)->isDict() || !oResources.dictLookup("XObject", &oXObject)->isDict() ||
					oXObject.dictGetLength() != 1 || !oXObject.dictGetVal(0, &oIm)->isStream())
				{
					oStr.free(); oResources.free(); oXObject.free(); oIm.free();
					continue;
				}
				oStr.free(); oResources.free();
			}
			else if ((oStr.free(), true) && oMK.dictLookup("I", &oStr)->isNull() && oAP.isDict() && (oStr.free(), true) && oAP.dictLookup(arrAPName[j], &oStr)->isStream())
			{
				// Получение единственного XObject из Resources, если возможно
				Object oResources;
				if (!oStr.streamGetDict()->lookup("Resources", &oResources)->isDict() || !oResources.dictLookup("XObject", &oXObject)->isDict() ||
					oXObject.dictGetLength() != 1 || !oXObject.dictGetVal(0, &oIm)->isStream())
				{
					oStr.free(); oResources.free(); oXObject.free(); oIm.free();
					continue;
				}
				oStr.free(); oResources.free();

				Object oSubtype;
				Dict *oImDict = oIm.streamGetDict();
				if (oImDict->is("XObject") && oImDict->lookup("Subtype", &oSubtype)->isName() && !oSubtype.isName("Image"))
				{
					if (!oImDict->lookup("Resources", &oResources)->isDict() || (oXObject.free(), false) || !oResources.dictLookup("XObject", &oXObject)->isDict() ||
						oXObject.dictGetLength() != 1 || (oIm.free(), false) || !oXObject.dictGetVal(0, &oIm)->isStream())
					{
						oSubtype.free();
						oResources.free(); oXObject.free(); oIm.free();
						continue;
					}
					oResources.free();
				}
				oSubtype.free();
			}
			else
			{
				oStr.free();
				continue;
			}

			if (bFirst)
			{
				Object oFieldRef;
				pField->getFieldRef(&oFieldRef);
				// Номер аннотации для сопоставления с AP
				oRes.AddInt(oFieldRef.getRefNum() + nStartRefID);
				oFieldRef.free();

				// Количество иконок 1-3
				nMKPos = oRes.GetSize();
				oRes.AddInt(nMKLength);
				bFirst = false;
			}

			Dict *oImDict = oIm.streamGetDict();
			Object oSubtype;
			if (!oImDict->is("XObject") || !oImDict->lookup("Subtype", &oSubtype)->isName("Image"))
			{
				oSubtype.free();
				oXObject.free(); oIm.free();
				continue;
			}
			oSubtype.free();

			oRes.WriteString(sMKName);
			Object oStrRef;
			oXObject.dictGetValNF(0, &oStrRef);
			int nView = oStrRef.getRefNum() + nStartRefID;
			oRes.AddInt(nView);
			oStrRef.free(); oXObject.free();
			if (std::find(arrUniqueImage.begin(), arrUniqueImage.end(), nView) != arrUniqueImage.end())
			{
				oIm.free();
				oRes.WriteBYTE(0);
				nMKLength++;
				continue;
			}
			arrUniqueImage.push_back(nView);
			oRes.WriteBYTE(1);

			// Width & Height
			Object oWidth, oHeight;
			int nWidth  = 0;
			int nHeight = 0;
			if (oImDict->lookup("Width", &oWidth)->isNull())
			{
				oWidth.free();
				oImDict->lookup("W", &oWidth);
			}
			if (oWidth.isNum())
				nWidth = oWidth.getNum();
			if (oImDict->lookup("Height", &oHeight)->isNull())
			{
				oHeight.free();
				oImDict->lookup("H", &oHeight);
			}
			if (oHeight.isNum())
				nHeight = oHeight.getNum();
			oRes.AddInt(nWidth);
			oRes.AddInt(nHeight);
			oWidth.free(); oHeight.free();

			// Mask
			int nMaskWidth  = 0;
			int nMaskHeight = 0;
			bool bHaveSoftMask = false;
			GfxImageColorMap* pMaskColorMap = NULL;

			Object oMaskObj, oSMaskObj;
			oImDict->lookup("Mask", &oMaskObj);
			oImDict->lookup("SMask", &oSMaskObj);
			if (oSMaskObj.isStream())
			{
				Dict* oMaskDict = oSMaskObj.streamGetDict();

				if (oMaskDict->lookup("Width", &oWidth)->isNull())
				{
					oWidth.free();
					oMaskDict->lookup("W", &oWidth);
				}
				if (oWidth.isNum())
					nMaskWidth = oWidth.getNum();
				if (oMaskDict->lookup("Height", &oHeight)->isNull())
				{
					oHeight.free();
					oMaskDict->lookup("H", &oHeight);
				}
				if (oHeight.isNum())
					nMaskHeight = oHeight.getNum();
				oWidth.free(); oHeight.free();

				Object oBits;
				if (oMaskDict->lookup("BitsPerComponent", &oBits)->isNull())
				{
					oBits.free();
					oMaskDict->lookup("BPC", &oBits);
				}
				int nMaskBits = oBits.isInt() ? oBits.getInt() : 8;
				oBits.free();

				GfxColorSpace* maskColorSpace = NULL;
				Object oColorSpace;
				if (oMaskDict->lookup("ColorSpace", &oColorSpace)->isNull())
				{
					oColorSpace.free();
					oMaskDict->lookup("CS", &oColorSpace);
				}
				if (oColorSpace.isName("DeviceGray"))
					maskColorSpace = GfxColorSpace::create(csDeviceGray);
				oColorSpace.free();

				Object oDecode;
				if (oMaskDict->lookup("Decode", &oDecode)->isNull())
				{
					oDecode.free();
					oMaskDict->lookup("D", &oDecode);
				}

				pMaskColorMap = new GfxImageColorMap(nMaskBits, &oDecode, maskColorSpace);
				oDecode.free();

				// TODO Matte

				bHaveSoftMask = true;
			}
			else if (oMaskObj.isArray())
			{

			}
			else if (oMaskObj.isStream())
			{

			}

			if (bBase64 && !bHaveSoftMask)
			{
				int nLength = 0;
				Object oLength;
				if (oImDict->lookup("Length", &oLength)->isInt())
					nLength = oLength.getInt();
				oLength.free();
				if (oImDict->lookup("DL", &oLength)->isInt())
					nLength = oLength.getInt();
				oLength.free();

				bool bNew = false;
				BYTE* pBuffer = NULL;
				Stream* pImage = oIm.getStream()->getUndecodedStream();
				pImage->reset();
				MemStream* pMemory = dynamic_cast<MemStream*>(pImage);
				if (pImage->getKind() == strWeird && pMemory)
				{
					if (pMemory->getBufPtr() + nLength == pMemory->getBufEnd())
						pBuffer = (BYTE*)pMemory->getBufPtr();
					else
						nLength = 0;
				}
				else
				{
					bNew = true;
					pBuffer = new BYTE[nLength];
					BYTE* pBufferPtr = pBuffer;
					for (int nI = 0; nI < nLength; ++nI)
						*pBufferPtr++ = (BYTE)pImage->getChar();
				}

				char* cData64 = NULL;
				int nData64Dst = 0;
				NSFile::CBase64Converter::Encode(pBuffer, nLength, cData64, nData64Dst, NSBase64::B64_BASE64_FLAG_NOCRLF);

				oRes.WriteString((BYTE*)cData64, nData64Dst);

				nMKLength++;
				if (bNew)
					RELEASEARRAYOBJECTS(pBuffer);
				RELEASEARRAYOBJECTS(cData64);
				continue;
			}

			// else
			BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];
			unsigned int nColor = (unsigned int)nBackgroundColor;
			unsigned int nSize  = (unsigned int)(nWidth * nHeight);
			unsigned int* pTemp = (unsigned int*)pBgraData;
			for (unsigned int k = 0; k < nSize; ++k)
				*pTemp++ = nColor;

			int bits = 0;
			StreamColorSpaceMode csMode = streamCSNone;
			oIm.getStream()->getImageParams(&bits, &csMode);

			if (bits == 0)
			{
				Object oBits;
				if (oImDict->lookup("BitsPerComponent", &oBits)->isNull())
				{
					oBits.free();
					oImDict->lookup("BPC", &oBits);
				}
				bits = oBits.isInt() ? oBits.getInt() : 8;
				oBits.free();
			}

			GfxColorSpace* colorSpace = NULL;
			Object oColorSpace;
			if (oImDict->lookup("ColorSpace", &oColorSpace)->isNull())
			{
				oColorSpace.free();
				oImDict->lookup("CS", &oColorSpace);
			}
			if (oColorSpace.isName())
			{
				// TODO
			}
			if (!oColorSpace.isNull())
				colorSpace = GfxColorSpace::parse(&oColorSpace);
			else if (csMode == streamCSDeviceGray)
				colorSpace = GfxColorSpace::create(csDeviceGray);
			else if (csMode == streamCSDeviceRGB)
				colorSpace = GfxColorSpace::create(csDeviceRGB);
			else if (csMode == streamCSDeviceCMYK)
				colorSpace = GfxColorSpace::create(csDeviceCMYK);
			else
				colorSpace = NULL;
			oColorSpace.free();

			Object oDecode;
			if (oImDict->lookup("Decode", &oDecode)->isNull())
			{
				oDecode.free();
				oImDict->lookup("D", &oDecode);
			}

			GfxImageColorMap* pColorMap = new GfxImageColorMap(bits, &oDecode, colorSpace);
			oDecode.free();

			ImageStream *pImageStream = new ImageStream(oIm.getStream(), nWidth, pColorMap->getNumPixelComps(), pColorMap->getBits());
			pImageStream->reset();

			int nComps = pImageStream->getComps();
			int nCheckWidth = std::min(nWidth, pImageStream->getVals() / nComps);

			int nColorMapType = pColorMap->getFillType();
			GfxColorComp** pColorMapLookup = pColorMap->getLookup();
			if (!pColorMapLookup)
				nColorMapType = 0;

			for (int nY = 0; nY < nHeight; ++nY)
			{
				unsigned char* pLine = pImageStream->getLine();
				unsigned char* pLineDst = pBgraData + 4 * nWidth * nY;

				if (!pLine)
				{
					memset(pLineDst, 0, 4 * nWidth);
					continue;
				}

				for (int nX = 0; nX < nCheckWidth; ++nX)
				{
					if (2 == nColorMapType)
					{
						pLineDst[0] = colToByte(clip01(pColorMapLookup[0][pLine[0]]));
						pLineDst[1] = colToByte(clip01(pColorMapLookup[1][pLine[1]]));
						pLineDst[2] = colToByte(clip01(pColorMapLookup[2][pLine[2]]));
					}
					else if (1 == nColorMapType)
					{
						pLineDst[0] = pLineDst[1] = pLineDst[2] = colToByte(clip01(pColorMapLookup[0][pLine[0]]));
					}
					else
					{
						GfxRGB oRGB;
						pColorMap->getRGB(pLine, &oRGB, gfxRenderingIntentAbsoluteColorimetric);
						pLineDst[0] = colToByte(oRGB.r);
						pLineDst[1] = colToByte(oRGB.g);
						pLineDst[2] = colToByte(oRGB.b);
					}

					pLineDst[3] = 255;
					pLine += nComps;
					pLineDst += 4;
				}
			}
			delete pImageStream;
			delete pColorMap;
			oIm.free();

			if (bHaveSoftMask && nWidth == nMaskWidth && nHeight == nMaskHeight)
			{
				ImageStream* pSMaskStream = new ImageStream(oSMaskObj.getStream(), nMaskWidth, pMaskColorMap->getNumPixelComps(), pMaskColorMap->getBits());
				pSMaskStream->reset();

				BYTE unAlpha = 0;
				for (int nY = 0; nY < nMaskHeight; ++nY)
				{
					int nIndex = 4 * nY * nMaskWidth;
					for (int nX = 0; nX < nMaskWidth; ++nX)
					{
						pSMaskStream->getPixel(&unAlpha);
						GfxGray oGray;
						pMaskColorMap->getGray(&unAlpha, &oGray, GfxRenderingIntent::gfxRenderingIntentAbsoluteColorimetric);
						pBgraData[nIndex + 3] = colToByte(oGray);
						nIndex += 4;
					}
				}
				delete pSMaskStream;
			}
			RELEASEOBJECT(pMaskColorMap);
			oMaskObj.free(); oSMaskObj.free();

			nMKLength++;
			if (bBase64)
			{
				CBgraFrame oFrame;
				oFrame.put_Data(pBgraData);
				oFrame.put_Width(nWidth);
				oFrame.put_Height(nHeight);
				oFrame.put_Stride(4 * nWidth);
				oFrame.put_IsRGBA(true);

				BYTE* pPNG = NULL;
				int nPNGSize = 0;
				oFrame.Encode(pPNG, nPNGSize, 4);
				oFrame.ClearNoAttack();
				RELEASEARRAYOBJECTS(pBgraData);

				char* cData64 = NULL;
				int nData64Dst = 0;
				NSFile::CBase64Converter::Encode(pPNG, nPNGSize, cData64, nData64Dst, NSBase64::B64_BASE64_FLAG_NOCRLF);

				oRes.WriteString((BYTE*)cData64, nData64Dst);

				RELEASEARRAYOBJECTS(pPNG);
				RELEASEARRAYOBJECTS(cData64);
			}
			else
			{
				unsigned long long npSubMatrix = (unsigned long long)pBgraData;
				unsigned int npSubMatrix1 = npSubMatrix & 0xFFFFFFFF;
				oRes.AddInt(npSubMatrix1);
				oRes.AddInt(npSubMatrix >> 32);
			}
		}
		oMK.free(); oAP.free();

		if (nMKPos > 0)
			oRes.AddInt(nMKLength, nMKPos);
	}

	oRes.WriteLen();
	BYTE* bRes = oRes.GetBuffer();
	oRes.ClearWithoutAttack();
	return bRes;
}
BYTE* CPdfReader::StreamToCData(BYTE* pSteam, int nLength)
{
	NSWasm::CData oRes;
	oRes.SkipLen();

	oRes.Write(pSteam, nLength);

	oRes.WriteLen();

	BYTE* bRes = oRes.GetBuffer();
	oRes.ClearWithoutAttack();
	return bRes;
}
int GetPageAnnots(PDFDoc* pdfDoc, NSFonts::IFontManager* pFontManager, PdfReader::CPdfFontList *pFontList, NSWasm::CData& oRes, int nPageIndex, int nStartPage, int nStartRefID)
{
	Page* pPage = pdfDoc->getCatalog()->getPage(nPageIndex);
	if (!pPage)
		return 0;

	Object oAnnots;
	if (!pPage->getAnnots(&oAnnots)->isArray())
	{
		oAnnots.free();
		return 0;
	}

	int nRes = 0;
	for (int i = 0, nNum = oAnnots.arrayGetLength(); i < nNum; ++i)
	{
		Object oAnnot;
		if (!oAnnots.arrayGet(i, &oAnnot)->isDict())
		{
			oAnnot.free();
			continue;
		}

		Object oSubtype;
		std::string sType;
		if (oAnnot.dictLookup("Subtype", &oSubtype)->isName())
			sType = oSubtype.getName();
		oSubtype.free(); oAnnot.free();

		Object oAnnotRef;
		PdfReader::CAnnot* pAnnot = NULL;
		oAnnots.arrayGetNF(i, &oAnnotRef);
		if (sType == "Text")
		{
			pAnnot = new PdfReader::CAnnotText(pdfDoc, &oAnnotRef, nPageIndex, nStartRefID);
		}
		else if (sType == "Link")
		{

		}
		else if (sType == "FreeText")
		{
			PdfReader::CAnnotFreeText* pFreeText = new PdfReader::CAnnotFreeText(pdfDoc, &oAnnotRef, nPageIndex, nStartRefID);
			pFreeText->SetFont(pdfDoc, &oAnnotRef, pFontManager, pFontList);
			pAnnot = pFreeText;
		}
		else if (sType == "Line")
		{
			pAnnot = new PdfReader::CAnnotLine(pdfDoc, &oAnnotRef, nPageIndex, nStartRefID);
		}
		else if (sType == "Square" || sType == "Circle")
		{
			pAnnot = new PdfReader::CAnnotSquareCircle(pdfDoc, &oAnnotRef, nPageIndex, nStartRefID);
		}
		else if (sType == "Polygon" || sType == "PolyLine")
		{
			pAnnot = new PdfReader::CAnnotPolygonLine(pdfDoc, &oAnnotRef, nPageIndex, nStartRefID);
		}
		else if (sType == "Highlight" ||
				 sType == "Underline" ||
				 sType == "Squiggly"  ||
				 sType == "StrikeOut")
		{
			pAnnot = new PdfReader::CAnnotTextMarkup(pdfDoc, &oAnnotRef, nPageIndex, nStartRefID);
		}
		else if (sType == "Stamp")
		{
			pAnnot = new PdfReader::CAnnotStamp(pdfDoc, &oAnnotRef, nPageIndex, nStartRefID);
		}
		else if (sType == "Caret")
		{
			pAnnot = new PdfReader::CAnnotCaret(pdfDoc, &oAnnotRef, nPageIndex, nStartRefID);
		}
		else if (sType == "Ink")
		{
			pAnnot = new PdfReader::CAnnotInk(pdfDoc, &oAnnotRef, nPageIndex, nStartRefID);
		}
		else if (sType == "Redact")
		{
			PdfReader::CAnnotRedact* pRedact = new PdfReader::CAnnotRedact(pdfDoc, &oAnnotRef, nPageIndex, nStartRefID);
			pRedact->SetFont(pdfDoc, pFontManager, pFontList, &oAnnotRef);
			pAnnot = pRedact;
		}
		// else if (sType == "FileAttachment")
		// {
		// 	pAnnot = new PdfReader::CAnnotFileAttachment(pdfDoc, &oAnnotRef, nPageIndex, nStartRefID);
		// }
		// else if (sType == "Popup")
		// {
		// 	pAnnot = new PdfReader::CAnnotPopup(pdfDoc, &oAnnotRef, nPageIndex, nStartRefID);
		// }
		// TODO Все аннотации
		oAnnotRef.free();

		if (pAnnot)
		{
			pAnnot->SetPage(nStartPage + nPageIndex);
			pAnnot->ToWASM(oRes);
			nRes++;
		}
		RELEASEOBJECT(pAnnot);
	}

	oAnnots.free();
	return nRes;
}
BYTE* CPdfReader::GetAnnots(int _nPageIndex)
{
	if (m_vPDFContext.empty())
		return NULL;

	NSWasm::CData oRes;
	oRes.SkipLen();

	if (_nPageIndex >= 0)
	{
		PDFDoc* pDoc = NULL;
		PdfReader::CPdfFontList* pFontList = NULL;
		int nStartRefID = 0;
		int nPageIndex = GetPageIndex(_nPageIndex, &pDoc, &pFontList, &nStartRefID);
		if (nPageIndex < 0 || !pDoc || !pFontList || !pDoc->getCatalog())
			return NULL;

		int nAnnots = 0;
		int nPosAnnots = oRes.GetSize();
		oRes.AddInt(nAnnots);

		nAnnots = GetPageAnnots(pDoc, m_pFontManager, pFontList, oRes, nPageIndex, _nPageIndex + 1 - nPageIndex, nStartRefID);

		oRes.AddInt(nAnnots, nPosAnnots);
	}
	else
	{
		int nStartPage = 0;
		for (CPdfReaderContext* pPDFContext : m_vPDFContext)
		{
			PDFDoc* pDoc = pPDFContext->m_pDocument;
			if (!pDoc || !pDoc->getCatalog() || !pPDFContext->m_pFontList)
				continue;
			int nAnnots = 0;
			int nPosAnnots = oRes.GetSize();
			oRes.AddInt(nAnnots);

			for (int nPage = 1, nPages = pDoc->getNumPages(); nPage <= nPages; ++nPage)
				nAnnots += GetPageAnnots(pDoc, m_pFontManager, pPDFContext->m_pFontList, oRes, nPage, nStartPage, pPDFContext->m_nStartID);

			oRes.AddInt(nAnnots, nPosAnnots);
			nStartPage += pDoc->getNumPages();
		}
	}

	oRes.WriteLen();
	BYTE* bRes = oRes.GetBuffer();
	oRes.ClearWithoutAttack();
	return bRes;
}
BYTE* CPdfReader::GetAPAnnots(int nRasterW, int nRasterH, int nBackgroundColor, int _nPageIndex, int nAnnot, const char* sView)
{
	PDFDoc* pDoc = NULL;
	PdfReader::CPdfFontList* pFontList = NULL;
	int nStartRefID = 0;
	int nPageIndex = GetPageIndex(_nPageIndex, &pDoc, &pFontList, &nStartRefID);
	if (nPageIndex < 0 || !pDoc || !pFontList || !pDoc->getCatalog())
		return NULL;

	Object oAnnots;
	Page* pPage = pDoc->getCatalog()->getPage(nPageIndex);
	if (!pPage || !pPage->getAnnots(&oAnnots)->isArray())
	{
		oAnnots.free();
		return NULL;
	}

	NSWasm::CData oRes;
	oRes.SkipLen();

	for (int i = 0, nNum = oAnnots.arrayGetLength(); i < nNum; ++i)
	{
		Object oAnnotRef;
		if (!oAnnots.arrayGetNF(i, &oAnnotRef)->isRef() || (nAnnot >= 0 && oAnnotRef.getRefNum() + nStartRefID != nAnnot))
		{
			oAnnotRef.free();
			continue;
		}

		Object oAnnot, oObj;
		std::string sType;
		oAnnots.arrayGet(i, &oAnnot);
		if (oAnnot.dictLookup("Subtype", &oObj)->isName())
			sType = oObj.getName();
		oObj.free(); oAnnot.free();

		if (sType == "Widget")
		{
			oAnnotRef.free();
			continue;
		}

		PdfReader::CAnnotAP* pAP = new PdfReader::CAnnotAP(pDoc, m_pFontManager, pFontList, nRasterW, nRasterH, nBackgroundColor, nPageIndex, sView, &oAnnotRef, nStartRefID);
		if (pAP)
			pAP->ToWASM(oRes);
		RELEASEOBJECT(pAP);

		oAnnotRef.free();
	}

	oAnnots.free();

	oRes.WriteLen();
	BYTE* bRes = oRes.GetBuffer();
	oRes.ClearWithoutAttack();
	return bRes;
}
