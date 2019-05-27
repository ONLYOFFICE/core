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
#include "Document.h"
#include "Info.h"
#include "Catalog.h"
#include "Streams.h"
#include "EncryptDictionary.h"
#include "Encrypt.h"
#include "Pages.h"
#include "Outline.h"
#include "Destination.h"
#include "GState.h"
#include "Annotation.h"
#include "Image.h"
#include "Font14.h"
#include "FontCidTT.h"
#include "Shading.h"
#include "Pattern.h"

#include "../../DesktopEditor/agg-2.4/include/agg_span_hatch.h"
#include "../../DesktopEditor/common/SystemUtils.h"

#ifdef CreateFont
#undef CreateFont
#endif

namespace PdfWriter
{
	const char* c_sPdfHeader = "%PDF-1.7\015%\315\312\322\251\015";
	const char* c_sPdfAHeader = "%PDF-1.4\015%\315\312\322\251\015";
	//----------------------------------------------------------------------------------------
	// CDocument
	//----------------------------------------------------------------------------------------
	CDocument::CDocument()
	{
		m_pCatalog          = NULL;
		m_pOutlines         = NULL;
		m_pXref             = NULL;
		m_pPageTree         = NULL;
		m_pCurPage          = NULL;
		m_nCurPageNum       = -1;
		m_pInfo             = NULL;
		m_pTrailer          = NULL;
		m_pResources        = NULL;
		m_bEncrypt          = false;
		m_pEncryptDict      = NULL;
		m_unCompressMode    = COMP_NONE;
		m_pJbig2            = NULL;
		memset((void*)m_sTTFontTag, 0x00, 8);
		m_pTransparencyGroup = NULL;
		m_pFreeTypeLibrary  = NULL;

		m_bPDFAConformance	= false;
	}
	CDocument::~CDocument()
	{
		Close();
	}
    bool CDocument::CreateNew()
	{
		Close();

		m_pXref = new CXref(this, 0);
		if (!m_pXref)
			return false;

		m_pTrailer = m_pXref->GetTrailer();
		if (!m_pTrailer)
			return false;

		m_pCatalog = new CCatalog(m_pXref);
		if (!m_pCatalog)
			return false;

		m_pCatalog->SetPageMode(pagemode_UseNone);
		m_pCatalog->SetPageLayout(pagelayout_OneColumn);

		m_pPageTree = m_pCatalog->GetRoot();
		if (!m_pPageTree)
			return false;

		m_pInfo = new CInfoDict(m_pXref);
		if (!m_pInfo)
			return false;

		m_pInfo->SetCreationTime();
		std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvCompanyName);
		if (sApplication.empty())
			sApplication = NSSystemUtils::gc_EnvCompanyNameDefault;
		std::string sApplicationA = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sApplication);
		m_pInfo->SetInfo(InfoProducer, sApplicationA.c_str());

		CMetadata* pMetadata = m_pCatalog->AddMetadata(m_pXref, m_pInfo);
		if (IsPDFA())
		{
			CArrayObject* pID = (CArrayObject*)m_pTrailer->Get("ID");
			if (!pID)
			{
				BYTE arrId[16];
				CEncryptDict::CreateId(m_pInfo, m_pXref, (BYTE*)arrId);

				pID = new CArrayObject();
				m_pTrailer->Add("ID", pID);

				pID->Add(new CBinaryObject(arrId, 16));
				pID->Add(new CBinaryObject(arrId, 16));
			}
		}

		m_nCurPageNum = -1;

		m_vPages.clear();
		m_vExtGrStates.clear();
		m_vFillAlpha.clear();
		m_vStrokeAlpha.clear();

		m_pTransparencyGroup = NULL;

		return true;
	}
    void CDocument::Close()
	{
		// Все объекты удаляются внутри CXref
		RELEASEOBJECT(m_pXref);

		m_pTrailer          = NULL;
		m_pResources        = NULL;
		m_pCatalog          = NULL;
		m_pOutlines         = NULL;
		m_pPageTree         = NULL;
		m_pCurPage          = NULL;
		m_nCurPageNum       = 0;
		m_bEncrypt          = false;
		m_pEncryptDict      = NULL;
		m_pInfo             = NULL;
		m_unCompressMode    = COMP_NONE;
		m_pJbig2            = NULL;
		m_pTransparencyGroup= NULL;
		memset((void*)m_sTTFontTag, 0x00, 8);

		m_vPages.clear();
		m_vExtGrStates.clear();
		m_vStrokeAlpha.clear();
		m_vFillAlpha.clear();
		m_vShadings.clear();
		m_vTTFonts.clear();
		m_vFreeTypeFonts.clear();
		if (m_pFreeTypeLibrary)
		{
			FT_Done_FreeType(m_pFreeTypeLibrary);
			m_pFreeTypeLibrary = NULL;
		}
	}
    bool CDocument::SaveToFile(const std::wstring& wsPath)
	{
		CFileStream* pStream = new CFileStream();
		if (!pStream || !pStream->OpenFile(wsPath, true))
			return false;

		if (m_pJbig2)
			m_pJbig2->FlushStreams();

		SaveToStream((CStream*)pStream);
		delete pStream;

		return true;
	}
    void CDocument::SaveToStream(CStream* pStream)
	{
		unsigned long nRet = OK;

		// Пишем заголовок
		if (IsPDFA())
			pStream->WriteStr(c_sPdfAHeader);
		else
			pStream->WriteStr(c_sPdfHeader);
		
		if (false == m_wsDocumentID.empty())
		{
			std::string sDocumentID = "%DocumentID " + NSFile::CUtf8Converter::GetUtf8StringFromUnicode(m_wsDocumentID);
			pStream->WriteStr(sDocumentID.c_str());
		}

		// Добавляем в Trailer необходимые элементы 
		m_pTrailer->Add("Root", m_pCatalog);
		m_pTrailer->Add("Info", m_pInfo);

		// Шифруем документ, если это необходимо
		CEncrypt* pEncrypt = NULL;
		if (m_bEncrypt)
		{
			pEncrypt = m_pEncryptDict->GetEncrypt();
			PrepareEncryption();
		}

		m_pXref->WriteToStream(pStream, pEncrypt);
	}
    void CDocument::PrepareEncryption()
	{
		CEncrypt* pEncrypt = m_pEncryptDict->GetEncrypt();
		if (!pEncrypt)
			return;

		m_pEncryptDict->Prepare(m_pInfo, m_pXref);

		CArrayObject* pID = (CArrayObject*)m_pTrailer->Get("ID");
		if (!pID)
		{
			pID = new CArrayObject();
			m_pTrailer->Add("ID", pID);
		}
		else
			pID->Clear();

        pID->Add(new CBinaryObject(pEncrypt->m_anEncryptID, 16));
        pID->Add(new CBinaryObject(pEncrypt->m_anEncryptID, 16));
	}
    void CDocument::SetPasswords(const std::wstring & wsOwnerPassword, const std::wstring & wsUserPassword)
	{
		if (IsPDFA())
			return;

		if (!m_pEncryptDict)
			m_pEncryptDict = new CEncryptDict(m_pXref);

		if (!m_pEncryptDict)
			return;

        m_pEncryptDict->SetPasswords(wsOwnerPassword, wsUserPassword);

        m_pTrailer->Add("Encrypt", m_pEncryptDict);
		m_bEncrypt = true;
	}
    CPage* CDocument::AddPage()
	{
		CPage* pPage = new CPage(m_pXref, m_pPageTree, this);
		m_pPageTree->AddPage(pPage);
		m_pCurPage = pPage;
		m_vPages.push_back(pPage);


#ifndef FILTER_FLATE_DECODE_DISABLED
		if (m_unCompressMode & COMP_TEXT)
			pPage->SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif

		m_nCurPageNum++;
		return pPage;
	}
	void CDocument::SetDocumentID(const std::wstring & documentID)
	{
		m_wsDocumentID = documentID;		
	}

    void CDocument::SetPermission(unsigned int unPermission)
	{
		if (!m_bEncrypt)
			return;

		CEncrypt* pEncrypt = m_pEncryptDict->GetEncrypt();
		pEncrypt->SetPermission(unPermission);
	}
    void CDocument::SetCompressionMode(unsigned int unMode)
	{
		m_unCompressMode = unMode;
	}
	void CDocument::SetPDFAConformanceMode(bool isPDFA)
	{
		m_bPDFAConformance = isPDFA;		
	}
	bool CDocument::IsPDFA() const
	{
		return m_bPDFAConformance;
	}
    void CDocument::AddPageLabel(EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix)
	{
		CDictObject* pPageLabel = CreatePageLabel(eStyle, unFirstPage, sPrefix);
		if (!pPageLabel)
			return;

		m_pCatalog->AddPageLabel(m_nCurPageNum, pPageLabel);
	}
    void CDocument::AddPageLabel(unsigned int unPageNum, EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix)
	{
		CDictObject* pPageLabel = CreatePageLabel(eStyle, unFirstPage, sPrefix);
		if (!pPageLabel)
			return;

		m_pCatalog->AddPageLabel(unPageNum, pPageLabel);
	}
    CDictObject* CDocument::CreatePageLabel(EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix)
	{
		CDictObject* pLabel = new CDictObject();
		if (!pLabel)
			return NULL;

        eStyle = std::min(std::max(eStyle, pagenumstyle_Min), pagenumstyle_Max);
		switch (eStyle)
		{
		case pagenumstyle_UpperRoman: pLabel->Add("S", "R"); break;
		case pagenumstyle_LowerRoman: pLabel->Add("S", "r"); break;
		case pagenumstyle_UpperLetters: pLabel->Add("S", "A"); break;
		case pagenumstyle_LowerLetters: pLabel->Add("S", "a"); break;
		case pagenumstyle_Decimal: pLabel->Add("S", "D"); break;
		}

		if (sPrefix && 0 != sPrefix[0])
			pLabel->Add("P", new CStringObject(sPrefix));

		if (0 != unFirstPage)
			pLabel->Add("St", unFirstPage);

		return pLabel;
	}
    COutline* CDocument::CreateOutline(COutline* pParent, const char* sTitle)
	{
		if (!pParent)
		{
			if (!m_pOutlines)
			{
				m_pOutlines = new COutline(m_pXref);
				if (m_pOutlines)
					m_pCatalog->Add("Outlines", m_pOutlines);
				else
					return NULL;
			}

			pParent = m_pOutlines;
		}

		return new COutline(pParent, sTitle, m_pXref);
	}
    CDestination* CDocument::CreateDestination(unsigned int unPageIndex)
	{
		if (unPageIndex >= m_vPages.size())
			return NULL;

		CPage* pPage = m_vPages.at(unPageIndex);
		return new CDestination(pPage, m_pXref);
	}
    CExtGrState* CDocument::FindExtGrState(double dAlphaStroke, double dAlphaFill, EBlendMode eMode, int nStrokeAdjustment)
	{
		CExtGrState* pExtGrState = NULL;
		for (unsigned int unIndex = 0, unCount = m_vExtGrStates.size(); unIndex < unCount; unIndex++)
		{
			pExtGrState = m_vExtGrStates.at(unIndex);

			if (dAlphaStroke != pExtGrState->GetAlphaStroke())
				continue;

			if (dAlphaFill != pExtGrState->GetAlphaFill())
				continue;

			if (eMode != pExtGrState->GetBlendMode())
				continue;

			if ((0 == nStrokeAdjustment ? false : true) != pExtGrState->GetStrokeAdjustment())
				continue;

			return pExtGrState;
		}

		return NULL;
	}
    CExtGrState* CDocument::GetExtGState(double dAlphaStroke, double dAlphaFill, EBlendMode eMode, int nStrokeAdjustment)
	{
		if (IsPDFA())
			return NULL;

		CExtGrState* pExtGrState = FindExtGrState(dAlphaStroke, dAlphaFill, eMode, nStrokeAdjustment);

		if (!pExtGrState)
		{
			pExtGrState = new CExtGrState(m_pXref);
			if (!pExtGrState)
				return NULL;

			if (-1 != dAlphaStroke)
				pExtGrState->SetAlphaStroke(dAlphaStroke);

			if (-1 != dAlphaFill)
				pExtGrState->SetAlphaFill(dAlphaFill);

			if (blendmode_Unknown != eMode)
				pExtGrState->SetBlendMode(eMode);

			if (-1 != nStrokeAdjustment)
				pExtGrState->SetStrokeAdjustment(0 == nStrokeAdjustment ? false : true);

			m_vExtGrStates.push_back(pExtGrState);
		}

		return pExtGrState;
	}
    CExtGrState* CDocument::GetStrokeAlpha(double dAlpha)
	{
		if (IsPDFA())
			return NULL;

		CExtGrState* pExtGrState = NULL;
		for (unsigned int unIndex = 0, unCount = m_vStrokeAlpha.size(); unIndex < unCount; unIndex++)
		{
			pExtGrState = m_vStrokeAlpha.at(unIndex);

			if (fabs(dAlpha - pExtGrState->GetAlphaStroke()) < 0.001)
				return pExtGrState;
		}

		pExtGrState = new CExtGrState(m_pXref);
		if (!pExtGrState)
			return NULL;

		pExtGrState->SetAlphaStroke(dAlpha);

		m_vStrokeAlpha.push_back(pExtGrState);
		return pExtGrState;
	}
    CExtGrState* CDocument::GetFillAlpha(double dAlpha)
	{
		if (IsPDFA())
			return NULL;

		CExtGrState* pExtGrState = NULL;
		for (unsigned int unIndex = 0, unCount = m_vFillAlpha.size(); unIndex < unCount; unIndex++)
		{
			pExtGrState = m_vFillAlpha.at(unIndex);

			if (fabs(dAlpha - pExtGrState->GetAlphaFill()) < 0.001)
				return pExtGrState;
		}

		pExtGrState = new CExtGrState(m_pXref);
		if (!pExtGrState)
			return NULL;

		pExtGrState->SetAlphaFill(dAlpha);

		m_vFillAlpha.push_back(pExtGrState);
		return pExtGrState;
	}
    CAnnotation* CDocument::CreateTextAnnot(unsigned int unPageNum, TRect oRect, const char* sText)
	{
		CAnnotation* pAnnot = new CTextAnnotation(m_pXref, oRect, sText);
		if (pAnnot)
		{
			CPage* pPage = m_vPages.at(unPageNum);
			pPage->AddAnnotation(pAnnot);
		}

	    return pAnnot;
	}
    CAnnotation* CDocument::CreateLinkAnnot(unsigned int unPageNum, TRect oRect, CDestination* pDest)
	{
		CAnnotation* pAnnot = new CLinkAnnotation(m_pXref, oRect, pDest);
	    
		if (pAnnot)
		{
			CPage* pPage = m_vPages.at(unPageNum);
			pPage->AddAnnotation(pAnnot);
		}
	
	    return pAnnot;
	}	
    CAnnotation* CDocument::CreateUriLinkAnnot(unsigned int unPageNum, TRect oRect, const char* sUri)
	{
		CAnnotation* pAnnot = new CUriLinkAnnotation(m_pXref, oRect, sUri);
	
		if (pAnnot)
		{
			CPage* pPage = m_vPages.at(unPageNum);
			pPage->AddAnnotation(pAnnot);
		}

		return pAnnot;
	}
    CImageDict* CDocument::CreateImage()
	{
		return new CImageDict(m_pXref, this);
	}
    CFont14* CDocument::CreateFont14(EStandard14Fonts eType)
	{
		return new CFont14(m_pXref, this, eType);
	}
	CFontCidTrueType* CDocument::CreateTrueTypeFont(const std::wstring& wsFontPath, unsigned int unIndex)
	{
		for (int nIndex = 0, nCount = m_vTTFonts.size(); nIndex < nCount; nIndex++)
		{
			TFontInfo& oInfo = m_vTTFonts.at(nIndex);
			if (wsFontPath == oInfo.wsPath && unIndex == oInfo.unIndex)
				return oInfo.pFont;
		}

		CFontCidTrueType* pFont = new CFontCidTrueType(m_pXref, this, wsFontPath, unIndex);
		if (!pFont)
			return NULL;

		// 0 GID всегда используется для .notdef символа, не используем данный код для настоящих символов
		unsigned int unUnicode = 0, unGid = 0;
		unsigned char* pString = pFont->EncodeString(&unUnicode, 1, &unGid);
		if (pString)
			delete pString;

		m_vTTFonts.push_back(TFontInfo(wsFontPath, unIndex, pFont));
		return pFont;
	}
    char* CDocument::GetTTFontTag()
	{
		if (0 == m_sTTFontTag[0])
		{
			MemCpy((BYTE*)m_sTTFontTag, (BYTE*)"BAAAAA+", 7);
		}
		else
		{
			for (unsigned int nIndex = 0; nIndex <= 5; nIndex++)
			{
				m_sTTFontTag[nIndex] += 1;
				if (m_sTTFontTag[nIndex] > 'Z')
					m_sTTFontTag[nIndex] = 'A';
				else
					break;
			}
		}

		return m_sTTFontTag;
	}
    void CDocument::AddFreeTypeFont(CFontCidTrueType* pFont)
	{
		for (int nIndex = 0, nCount = m_vFreeTypeFonts.size(); nIndex < nCount; nIndex++)
		{
			if (pFont == m_vFreeTypeFonts.at(nIndex))
			{
				if (nIndex >= 10)
				{
					m_vFreeTypeFonts.erase(m_vFreeTypeFonts.begin() + nIndex);
					m_vFreeTypeFonts.insert(m_vFreeTypeFonts.begin(), pFont);
				}
				return;
			}
		}

		m_vFreeTypeFonts.insert(m_vFreeTypeFonts.begin(), pFont);

		int nFontsCount = m_vFreeTypeFonts.size();
		if (nFontsCount > MAX_OPENED_FT_FACES)
		{
			for (int nFontIndex = MAX_OPENED_FT_FACES; nFontIndex < nFontsCount; nFontIndex++)
			{
				CFontCidTrueType* pFont = m_vFreeTypeFonts.at(nFontIndex);
				pFont->CloseFontFace();
			}
			m_vFreeTypeFonts.erase(m_vFreeTypeFonts.begin() + MAX_OPENED_FT_FACES, m_vFreeTypeFonts.end());
		}
	}
    FT_Library CDocument::GetFreeTypeLibrary()
	{
		if (!m_pFreeTypeLibrary)
			FT_Init_FreeType(&m_pFreeTypeLibrary);

		return m_pFreeTypeLibrary;
	}
    CJbig2Global* CDocument::GetJbig2Global()
	{
		if (m_pJbig2 && m_pJbig2->GetImagesCount() > 4)
		{
			// Удалять не надо, т.к. объект удалится в CXref
			m_pJbig2->FlushStreams();
			m_pJbig2 = NULL;
		}

		if (!m_pJbig2)
			m_pJbig2 = new CJbig2Global(m_pXref);

		return m_pJbig2;
	}
    CShading* CDocument::CreateShading(CPage* pPage, double *pPattern, bool bAxial, unsigned char* pColors, unsigned char* pAlphas, double* pPoints, int nCount, CExtGrState*& pExtGrState)
	{
		pExtGrState = NULL;

		bool bNeedAlpha = false;
		unsigned char* pA = new unsigned char[3 * nCount];
		if (!pA)
			return NULL;

		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			pA[3 * nIndex + 0] = pAlphas[nIndex];
			pA[3 * nIndex + 1] = pAlphas[nIndex];
			pA[3 * nIndex + 2] = pAlphas[nIndex];

			if (255 != pAlphas[nIndex])
				bNeedAlpha = true;
		}

		if (!bNeedAlpha)
		{
			delete[] pA;
			if (bAxial)
				return CreateAxialShading(pPattern[0], pPattern[1], pPattern[2], pPattern[3], pColors, pPoints, nCount);
			else
				return CreateRadialShading(pPattern[0], pPattern[1], pPattern[2], pPattern[3], pPattern[4], pPattern[5], pColors, pPoints, nCount);
		}

		// Создаем 2 shading-объекта, один цветной RGB, второй серый со значениями альфа-канала
		CShading* pColorShading = NULL;
		CShading* pAlphaShading = NULL;

		if (bAxial)
		{
			pColorShading = CreateAxialShading(pPattern[0], pPattern[1], pPattern[2], pPattern[3], pColors, pPoints, nCount);
			pAlphaShading = CreateAxialShading(pPattern[0], pPattern[1], pPattern[2], pPattern[3], pA, pPoints, nCount);
		}
		else
		{
			pColorShading = CreateRadialShading(pPattern[0], pPattern[1], pPattern[2], pPattern[3], pPattern[4], pPattern[5], pColors, pPoints, nCount);
			pAlphaShading = CreateRadialShading(pPattern[0], pPattern[1], pPattern[2], pPattern[3], pPattern[4], pPattern[5], pA, pPoints, nCount);
		}
		delete[] pA;

        if (!IsPDFA())
        {
            if (!m_pTransparencyGroup)
            {
                m_pTransparencyGroup = new CDictObject();
                m_pTransparencyGroup->Add("Type", "Group");
                m_pTransparencyGroup->Add("S", "Transparency");
                m_pTransparencyGroup->Add("CS", "DeviceRGB");
            }

            pPage->Add("Group", m_pTransparencyGroup);
        }
		double dWidth  = pPage->GetWidth();
		double dHeight = pPage->GetHeight();

		// Создаем графический объект, который будет альфа-маской
		CDictObject* pXObject = new CDictObject(m_pXref);
		pXObject->Add("Type", "XObject");
		pXObject->Add("Subtype", "Form");
		pXObject->Add("BBox", CArrayObject::CreateBox(0, 0, dWidth, dHeight));
        if (m_pTransparencyGroup)
            pXObject->Add("Group", m_pTransparencyGroup);
		CDictObject* pResources = new CDictObject();
		pXObject->Add("Resources", pResources);
		CDictObject* pResShadings = new CDictObject();
		pResources->Add("Shading", pResShadings);
		pResShadings->Add("S1", pAlphaShading);
		CStream* pStream = pXObject->GetStream();

		pStream->WriteStr("0 0 ");
		pStream->WriteReal(dWidth);
		pStream->WriteChar(' ');
		pStream->WriteReal(dHeight);
		pStream->WriteStr(" re\012W\012\n\012/S1 sh\012");

		// Создаем обект-маску для графического состояние
		CDictObject* pMask = new CDictObject();
		m_pXref->Add(pMask);
		pMask->Add("Type", "Mask");
		pMask->Add("S", "Luminosity");
		pMask->Add("G", pXObject);

		if (!IsPDFA())
		{
			// Создаем ExtGState объект, в который мы запишем альфа-маску
			pExtGrState = new CExtGrState(m_pXref);
			pExtGrState->Add("BM", "Normal");
			pExtGrState->Add("ca", 1);
			pExtGrState->Add("SMask", pMask);
		}

		return pColorShading;
	}
    CShading* CDocument::CreateAxialShading(double dX0, double dY0, double dX1, double dY1, unsigned char* pColors, double* pPoints, int nCount)
	{
		for (int nIndex = 0, nShadingsCount = m_vShadings.size(); nIndex < nShadingsCount; nIndex++)
		{
			CShading* pShading = m_vShadings.at(nIndex);
			if (shadingtype_Axial == pShading->GetShadingType()
				&& ((CAxialShading*)pShading)->Compare(dX0, dY0, dX1, dY1)
				&& pShading->CompareColors(pColors, pPoints, nCount, true)
				&& pShading->CompareExtend(true, true))
				return pShading;
		}

		CAxialShading* pShading = new CAxialShading(m_pXref, dX0, dY0, dX1, dY1);
		if (!pShading)
			return NULL;

		pShading->SetRgbColors(pColors, pPoints, nCount);
		pShading->SetExtend(true, true);

		m_vShadings.push_back(pShading);

		return pShading;
	}
    CShading* CDocument::CreateRadialShading(double dX0, double dY0, double dR0, double dX1, double dY1, double dR1, unsigned char* pColors, double* pPoints, int nCount)
	{
		for (int nIndex = 0, nShadingsCount = m_vShadings.size(); nIndex < nShadingsCount; nIndex++)
		{
			CShading* pShading = m_vShadings.at(nIndex);
			if (shadingtype_Radial == pShading->GetShadingType()
				&& ((CRadialShading*)pShading)->Compare(dX0, dY0, dR0, dX1, dY1, dR1)
				&& pShading->CompareColors(pColors, pPoints, nCount, true)
				&& pShading->CompareExtend(true, true))
				return pShading;
		}

		CRadialShading* pShading = new CRadialShading(m_pXref, dX0, dY0, dR0, dX1, dY1, dR1);
		if (!pShading)
			return NULL;

		pShading->SetRgbColors(pColors, pPoints, nCount);
		pShading->SetExtend(true, true);

		m_vShadings.push_back(pShading);

		return pShading;
	}
	CImageTilePattern*CDocument::CreateImageTilePattern(double dW, double dH, CImageDict* pImageDict, CMatrix* pMatrix, EImageTilePatternType eType)
	{
		return new CImageTilePattern(m_pXref, dW, dH, pImageDict, pMatrix, eType);
	}
	CImageTilePattern*CDocument::CreateHatchPattern(double dW, double dH, const BYTE& nR1, const BYTE& nG1, const BYTE& nB1, const BYTE& nAlpha1, const BYTE& nR2, const BYTE& nG2, const BYTE& nB2, const BYTE& nAlpha2, const std::wstring& wsHatch)
	{
		// TODO: Надо бы сделать мап, чтобы не создавать одинаковых паттернов

		CImageDict* pImage = CreateImage();
		BYTE* pBuffer = new BYTE[3 * HATCH_TX_SIZE * HATCH_TX_SIZE];
		if (!pBuffer)
			return NULL;

		TColor oColor1(nR1, nG1, nB1);
		TColor oColor2(nR2, nG2, nB2);
		agg::GetHatchPattern<TColor>(wsHatch, (TColor*)pBuffer, oColor1, oColor2);
		pImage->LoadRaw(pBuffer, 3 * HATCH_TX_SIZE * HATCH_TX_SIZE, HATCH_TX_SIZE, HATCH_TX_SIZE);
		delete[] pBuffer;

		if (255 != nAlpha1 || 255 != nAlpha2)
		{
			BYTE* pSMask = new BYTE[HATCH_TX_SIZE * HATCH_TX_SIZE];
			if (pSMask)
			{
				agg::GetHatchPattern<BYTE>(wsHatch, pSMask, nAlpha1, nAlpha2);
				pImage->LoadSMask(pSMask, (unsigned int)HATCH_TX_SIZE * HATCH_TX_SIZE, (unsigned int)HATCH_TX_SIZE, (unsigned int)HATCH_TX_SIZE);
				delete[] pSMask;
			}
		}

		return CreateImageTilePattern(dW, dH, pImage, NULL, imagetilepatterntype_Default);
	}
    CShading* CDocument::CreateAxialShading(CPage* pPage, double dX0, double dY0, double dX1, double dY1, unsigned char* pColors, unsigned char* pAlphas, double* pPoints, int nCount, CExtGrState*& pExtGrState)
	{
		double pPattern[] ={ dX0, dY0, dX1, dY1 };
		return CreateShading(pPage, pPattern, true, pColors, pAlphas, pPoints, nCount, pExtGrState);
	}
    CShading* CDocument::CreateRadialShading(CPage* pPage, double dX0, double dY0, double dR0, double dX1, double dY1, double dR1, unsigned char* pColors, unsigned char* pAlphas, double* pPoints, int nCount, CExtGrState*& pExtGrState)
	{
		double pPattern[] ={ dX0, dY0, dR0, dX1, dY1, dR1 };
		return CreateShading(pPage, pPattern, false, pColors, pAlphas, pPoints, nCount, pExtGrState);
	}
}
