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
#include "FontTT.h"
#include "FontTTWriter.h"
#include "Shading.h"
#include "Pattern.h"
#include "AcroForm.h"
#include "Field.h"
#include "ResourcesDictionary.h"
#include "Metadata.h"

#include "../../DesktopEditor/agg-2.4/include/agg_span_hatch.h"
#include "../../DesktopEditor/common/SystemUtils.h"

#ifdef CreateFont
#undef CreateFont
#endif

#ifndef VALUE2STR
#define VALUE_TO_STRING(x) #x
#define VALUE2STR(x) VALUE_TO_STRING(x)
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
		m_pLastXref         = NULL;
		m_pPageTree         = NULL;
		m_pCurPage          = NULL;
		m_nCurPageNum       = -1;
		m_pCurImage         = NULL;
		m_pInfo             = NULL;
		m_pTrailer          = NULL;
		m_pResources        = NULL;
		m_pMetaData         = NULL;
		m_bEncrypt          = false;
		m_pEncryptDict      = NULL;
		m_unFormFields      = 0;
		m_unCompressMode    = COMP_NONE;
		memset((void*)m_sTTFontTag, 0x00, 8);
		m_pJbig2            = NULL;
		m_pDefaultCheckBoxFont = NULL;
		m_pTransparencyGroup = NULL;
		m_pFreeTypeLibrary  = NULL;
		m_bPDFAConformance	= false;
		m_pAcroForm         = NULL;
		m_pFieldsResources  = NULL;
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

		m_pMetaData = new CStreamData(m_pXref);
		if (!m_pMetaData)
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

		m_pInfo->SetTime(InfoCreationDate);
		m_pInfo->SetTime(InfoModaDate);

		std::wstring sCreator = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
		if (sCreator.empty())
			sCreator = NSSystemUtils::gc_EnvApplicationNameDefault;
		std::string sCreatorA = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sCreator);

#if defined(INTVER)
		std::string sVersion = VALUE2STR(INTVER);
		sCreatorA += ("/" + sVersion);
#endif

		m_pInfo->SetInfo(InfoProducer, sCreatorA.c_str());
		m_pInfo->SetInfo(InfoCreator, sCreatorA.c_str());

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

		m_vExtGrStates.clear();
		m_vFillAlpha.clear();
		m_vStrokeAlpha.clear();
		m_vRadioGroups.clear();
		m_vMetaOForms.clear();
		m_vImages.clear();

		m_pTransparencyGroup = NULL;

		return true;
	}
    void CDocument::Close()
	{
		// Все объекты удаляются внутри CXref
		RELEASEOBJECT(m_pXref);

		m_pLastXref         = NULL;
		m_pTrailer          = NULL;
		m_pResources        = NULL;
		m_pCatalog          = NULL;
		m_pOutlines         = NULL;
		m_pPageTree         = NULL;
		m_pCurPage          = NULL;
		m_nCurPageNum       = 0;
		m_pCurImage         = NULL;
		m_unFormFields      = 0;
		m_bEncrypt          = false;
		m_pEncryptDict      = NULL;
		m_pInfo             = NULL;
		m_unCompressMode    = COMP_NONE;
		m_pJbig2            = NULL;
		m_pTransparencyGroup= NULL;
		m_pAcroForm         = NULL;
		m_pFieldsResources  = NULL;
		memset((void*)m_sTTFontTag, 0x00, 8);
		m_pDefaultCheckBoxFont = NULL;

		m_vExtGrStates.clear();
		m_vStrokeAlpha.clear();
		m_vFillAlpha.clear();
		m_vShadings.clear();
		m_vCidTTFonts.clear();
		m_vTTFonts.clear();
		m_vFreeTypeFonts.clear();
		m_vSignatures.clear();
		m_vMetaOForms.clear();
		m_vImages.clear();
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

		Sign(wsPath, m_pXref->GetSizeXRef());

		return true;
	}
	bool CDocument::SaveToMemory(BYTE** pData, int* pLength)
	{
		CMemoryStream* pStream = new CMemoryStream();
		if (!pStream)
			return false;

		if (m_pJbig2)
			m_pJbig2->FlushStreams();

		SaveToStream(pStream);

		*pData = pStream->GetBuffer();
		*pLength = pStream->Size();
		pStream->ClearWithoutAttack();
		return true;
	}
    void CDocument::SaveToStream(CStream* pStream)
	{
		m_pCatalog->AddMetadata(m_pXref, m_pInfo);

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

		m_pXref->WriteToStream(pStream, pEncrypt, true);
	}
	bool CDocument::SaveNewWithPassword(CXref* pXref, CXref* _pXref, const std::wstring& wsPath, const std::wstring& wsOwnerPassword, const std::wstring& wsUserPassword, CDictObject* pTrailer)
	{
		if (!pXref || !pTrailer || !_pXref)
			return false;
		m_pTrailer = pTrailer;

		CEncrypt* pEncrypt = NULL;
		if (!wsOwnerPassword.empty())
		{
			m_pEncryptDict = new CEncryptDict(_pXref);
			m_pEncryptDict->SetPasswords(wsOwnerPassword, wsUserPassword);
			m_pTrailer->Add("Encrypt", m_pEncryptDict);

			pEncrypt = m_pEncryptDict->GetEncrypt();
			PrepareEncryption();
		}

		CFileStream* pStream = new CFileStream();
		if (!pStream || !pStream->OpenFile(wsPath, true))
			return false;
		pStream->WriteStr(c_sPdfHeader);

		pXref->WriteToStream(pStream, pEncrypt);

		delete pStream;
		return true;
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

		if (m_pMetaData)
			m_pMetaData->SetID(new CBinaryObject(pEncrypt->m_anEncryptID, 16));

		for (int i = 0; i < m_vMetaOForms.size(); ++i)
			m_vMetaOForms[i]->Add("ID", new CBinaryObject(pEncrypt->m_anEncryptID, 16));
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

#ifndef FILTER_FLATE_DECODE_DISABLED
		if (m_unCompressMode & COMP_TEXT)
			pPage->SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif

		m_nCurPageNum++;
		return pPage;
	}
	CPage* CDocument::GetPage(const unsigned int &unPage)
	{
		if (unPage >= m_pPageTree->GetCount())
			return NULL;

		return m_pPageTree->GetPage(unPage);
	}
	CPage* CDocument::GetEditPage(const unsigned int& unPage)
	{
		CPage* pRes = NULL;
		std::map<int, CPage*>::iterator p = m_mEditPages.find(unPage);
		if (p != m_mEditPages.end())
			pRes = p->second;
		return pRes;
	}
	int CDocument::FindPage(CPage* pPage)
	{
		int nI = 0;
		return m_pPageTree->Find(pPage, nI) ? nI : -1;
	}
	unsigned int CDocument::GetPagesCount() const
	{
		return m_pPageTree->GetCount();
	}
    void CDocument::SetDocumentID(const std::wstring& documentID)
	{
		m_wsDocumentID = documentID;		
	}
    void CDocument::SetTitle(const std::string& sTitle)
    {
        if (!m_pInfo)
            return;
        m_pInfo->SetInfo(InfoTitle, sTitle.c_str());
    }
    void CDocument::SetAuthor(const std::string& sAuthor)
    {
        if (!m_pInfo)
            return;
        m_pInfo->SetInfo(InfoAuthor, sAuthor.c_str());
    }
    void CDocument::SetSubject(const std::string& sSubject)
    {
        if (!m_pInfo)
            return;
        m_pInfo->SetInfo(InfoSubject, sSubject.c_str());
    }
    void CDocument::SetKeywords(const std::string& sKeywords)
    {
        if (!m_pInfo)
            return;
        m_pInfo->SetInfo(InfoKeyWords, sKeywords.c_str());
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
	unsigned int CDocument::GetCompressionMode() const
	{
		return m_unCompressMode;
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
	bool CDocument::AddMetaData(const std::wstring& sMetaName, BYTE* pMetaData, DWORD nMetaLength)
	{
		if (!m_pMetaData)
			return false;

		CBinaryObject* sID = NULL;
		CArrayObject* pID = (CArrayObject*)m_pTrailer->Get("ID");
		if (!pID)
		{
			BYTE arrId[16];
			CEncryptDict::CreateId(m_pInfo, m_pXref, (BYTE*)arrId);

			pID = new CArrayObject();
			m_pTrailer->Add("ID", pID);

			pID->Add(new CBinaryObject(arrId, 16));
			pID->Add(new CBinaryObject(arrId, 16));

			sID = new CBinaryObject(arrId, 16);
		}
		else
			sID = (CBinaryObject*)pID->Get(1)->Copy();

		m_pMetaData->SetID(sID);

		return m_pMetaData->AddMetaData(sMetaName, pMetaData, nMetaLength);
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
	CDestination* CDocument::CreateDestination(CPage* pPage, bool bInline)
	{
		if (pPage)
			return new CDestination(pPage, m_pXref, bInline);
		return NULL;
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
	CAnnotation* CDocument::CreateTextAnnot()
	{
		CTextAnnotation* pNew = new CTextAnnotation(m_pXref);
		pNew->SetC({ 1.0, 0.8, 0.0 });
		return pNew;
	}
	CAnnotation* CDocument::CreateLinkAnnot(const TRect& oRect, CDestination* pDest)
	{
		CAnnotation* pAnnot = new CLinkAnnotation(m_pXref, pDest);
		pAnnot->SetRect(oRect);
		m_pXref->Add(pAnnot);
		return pAnnot;
	}
	CAnnotation* CDocument::CreateUriLinkAnnot(const TRect& oRect, const char* sUrl)
	{
		CAnnotation* pAnnot = new CUriLinkAnnotation(m_pXref, sUrl);
		pAnnot->SetRect(oRect);
		m_pXref->Add(pAnnot);
		return pAnnot;
	}
	CAnnotation* CDocument::CreateInkAnnot()
	{
		return new CInkAnnotation(m_pXref);
	}
	CAnnotation* CDocument::CreateLineAnnot()
	{
		return new CLineAnnotation(m_pXref);
	}
	CAnnotation* CDocument::CreateTextMarkupAnnot()
	{
		return new CTextMarkupAnnotation(m_pXref);
	}
	CAnnotation* CDocument::CreateSquareCircleAnnot()
	{
		return new CSquareCircleAnnotation(m_pXref);
	}
	CAnnotation* CDocument::CreatePolygonLineAnnot()
	{
		return new CPolygonLineAnnotation(m_pXref);
	}
	CAnnotation* CDocument::CreatePopupAnnot()
	{
		return new CPopupAnnotation(m_pXref);
	}
	CAnnotation* CDocument::CreateFreeTextAnnot()
	{
		return new CFreeTextAnnotation(m_pXref);
	}
	CAnnotation* CDocument::CreateCaretAnnot()
	{
		return new CCaretAnnotation(m_pXref);
	}
	CAnnotation* CDocument::CreateStampAnnot()
	{
		return new CStampAnnotation(m_pXref);
	}
	CAnnotation* CDocument::CreateWidgetAnnot()
	{
		if (!CheckAcroForm())
			return NULL;

		CWidgetAnnotation* pWidget = new CWidgetAnnotation(m_pXref, EAnnotType::AnnotWidget);
		m_pXref->Add(pWidget);

		CArrayObject* ppFields = (CArrayObject*)m_pAcroForm->Get("Fields");
		ppFields->Add(pWidget);

		return pWidget;
	}
	CAnnotation* CDocument::CreatePushButtonWidget()
	{
		if (!CheckAcroForm())
			return NULL;

		CWidgetAnnotation* pWidget = new CPushButtonWidget(m_pXref);
		m_pXref->Add(pWidget);
		pWidget->Add("FT", "Btn");
		CArrayObject* ppFields = (CArrayObject*)m_pAcroForm->Get("Fields");
		ppFields->Add(pWidget);

		return pWidget;
	}
	CAnnotation* CDocument::CreateCheckBoxWidget()
	{
		if (!CheckAcroForm())
			return NULL;

		CWidgetAnnotation* pWidget = new CCheckBoxWidget(m_pXref);
		m_pXref->Add(pWidget);
		pWidget->Add("FT", "Btn");
		CArrayObject* ppFields = (CArrayObject*)m_pAcroForm->Get("Fields");
		ppFields->Add(pWidget);

		return pWidget;
	}
	CAnnotation* CDocument::CreateTextWidget()
	{
		if (!CheckAcroForm())
			return NULL;

		CAnnotation* pWidget = new CTextWidget(m_pXref);
		m_pXref->Add(pWidget);
		pWidget->Add("FT", "Tx");
		CArrayObject* ppFields = (CArrayObject*)m_pAcroForm->Get("Fields");
		ppFields->Add(pWidget);

		return pWidget;
	}
	CAnnotation* CDocument::CreateChoiceWidget()
	{
		if (!CheckAcroForm())
			return NULL;

		CAnnotation* pWidget = new CChoiceWidget(m_pXref);
		m_pXref->Add(pWidget);
		pWidget->Add("FT", "Ch");
		CArrayObject* ppFields = (CArrayObject*)m_pAcroForm->Get("Fields");
		ppFields->Add(pWidget);

		return pWidget;
	}
	CAnnotation* CDocument::CreateSignatureWidget()
	{
		return new CSignatureWidget(m_pXref);
	}
	CAction* CDocument::CreateAction(BYTE nType)
	{
		switch (nType)
		{
		case 1:  return new CActionGoTo(m_pXref);
		case 6:  return new CActionURI(m_pXref);
		case 9:  return new CActionHide(m_pXref);
		case 10: return new CActionNamed(m_pXref);
		case 12: return new CActionResetForm(m_pXref);
		case 14: return new CActionJavaScript(m_pXref);
		}

		return NULL;
	}
	void CDocument::AddAnnotation(const int& nID, CAnnotation* pAnnot)
	{
		m_mAnnotations[nID] = pAnnot;
	}
    CImageDict* CDocument::CreateImage()
	{
		return new CImageDict(m_pXref, this);
	}
	CXObject* CDocument::CreateForm(CImageDict* pImage, const std::string& sName)
	{
		if (!pImage)
			return NULL;

		std::string sFrmName = "FRM" + sName;
		std::string sImgName = "Img" + sName;

		CXObject* pForm = new CXObject();
		CStream* pStream = new CMemoryStream();
		pForm->SetStream(m_pXref, pStream);

#ifndef FILTER_FLATE_DECODE_DISABLED
		if (m_unCompressMode & COMP_TEXT)
			pForm->SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif
		double dOriginW = pImage->GetWidth();
		double dOriginH = pImage->GetHeight();
		pForm->SetWidth(dOriginW);
		pForm->SetHeight(dOriginH);

		CArrayObject* pBBox = new CArrayObject();
		pForm->Add("BBox", pBBox);
		pBBox->Add(0);
		pBBox->Add(0);
		pBBox->Add(dOriginW);
		pBBox->Add(dOriginH);
		pForm->Add("FormType", 1);
		CArrayObject* pFormMatrix = new CArrayObject();
		pForm->Add("Matrix", pFormMatrix);
		pFormMatrix->Add(1);
		pFormMatrix->Add(0);
		pFormMatrix->Add(0);
		pFormMatrix->Add(1);
		pFormMatrix->Add(0);
		pFormMatrix->Add(0);
		pForm->Add("Name", sFrmName.c_str());
		pForm->SetName(sFrmName);

		CDictObject* pFormRes = new CDictObject();
		CArrayObject* pFormResProcset = new CArrayObject();
		pFormRes->Add("ProcSet", pFormResProcset);
		pFormResProcset->Add(new CNameObject("PDF"));
		pFormResProcset->Add(new CNameObject("ImageC"));
		CDictObject* pFormResXObject = new CDictObject();
		pFormRes->Add("XObject", pFormResXObject);

		pFormResXObject->Add(sImgName, pImage);
		pForm->Add("Resources", pFormRes);

		pForm->Add("Subtype", "Form");
		pForm->Add("Type", "XObject");

		pStream->WriteStr("q\012");
		pStream->WriteReal(dOriginW);
		pStream->WriteStr(" 0 0 ");
		pStream->WriteReal(dOriginH);
		pStream->WriteStr(" 0 0 cm\012/");
		pStream->WriteStr(sImgName.c_str());
		pStream->WriteStr(" Do\012Q");

		GetFieldsResources()->AddXObjectWithName(sFrmName.c_str(), pForm);

		return pForm;
	}
	CFont14* CDocument::CreateFont14(const std::wstring& wsFontPath, unsigned int unIndex, EStandard14Fonts eType)
	{
		CFont14* pFont = FindFont14(wsFontPath, unIndex);
		if (pFont)
			return pFont;
		pFont = new CFont14(m_pXref, this, eType);
		m_vFonts14.push_back(TFontInfo(wsFontPath, unIndex, pFont));
		return pFont;
	}
	CFont14* CDocument::FindFont14(const std::wstring& wsFontPath, unsigned int unIndex)
	{
		for (int nIndex = 0, nCount = m_vFonts14.size(); nIndex < nCount; nIndex++)
		{
			TFontInfo& oInfo = m_vFonts14.at(nIndex);
			if (wsFontPath == oInfo.wsPath && unIndex == oInfo.unIndex)
				return (CFont14*)oInfo.pFont;
		}
		return NULL;
	}
	CFontCidTrueType* CDocument::CreateCidTrueTypeFont(const std::wstring& wsFontPath, unsigned int unIndex)
	{
		CFontCidTrueType* pFont = FindCidTrueTypeFont(wsFontPath, unIndex);
		if (pFont)
			return pFont;

		CFontFileTrueType* pFontTT = CFontFileTrueType::LoadFromFile(wsFontPath, unIndex);
		if (!pFontTT)
			return NULL;

		pFont = new CFontCidTrueType(m_pXref, this, wsFontPath, unIndex, pFontTT);
		if (!pFont)
			return NULL;

		// 0 GID всегда используется для .notdef символа, не используем данный код для настоящих символов
		unsigned int unUnicode = 0;
		pFont->EncodeGID(0, &unUnicode, 1);

		m_vCidTTFonts.push_back(TFontInfo(wsFontPath, unIndex, pFont));
		return pFont;
	}
	CFontCidTrueType* CDocument::FindCidTrueTypeFont(const std::wstring &wsFontPath, unsigned int unIndex)
	{
		for (int nIndex = 0, nCount = m_vCidTTFonts.size(); nIndex < nCount; nIndex++)
		{
			TFontInfo& oInfo = m_vCidTTFonts.at(nIndex);
			if (wsFontPath == oInfo.wsPath && unIndex == oInfo.unIndex)
				return (CFontCidTrueType*)oInfo.pFont;
		}

		return NULL;
	}
	CFontTrueType* CDocument::CreateTrueTypeFont(const std::wstring& wsFontPath, unsigned int unIndex)
	{
		for (int nIndex = 0, nCount = m_vTTFonts.size(); nIndex < nCount; nIndex++)
		{
			TFontInfo& oInfo = m_vTTFonts.at(nIndex);
			if (wsFontPath == oInfo.wsPath && unIndex == oInfo.unIndex)
				return (CFontTrueType*)oInfo.pFont;
		}

		CFontTrueType* pFont = new CFontTrueType(m_pXref, this, wsFontPath, unIndex);
		if (!pFont)
			return NULL;

		m_vTTFonts.push_back(TFontInfo(wsFontPath, unIndex, pFont));
		return pFont;
	}
	CFontTrueType* CDocument::CreateTrueTypeFont(CFontCidTrueType* pCidFont)
	{
		for (int nIndex = 0, nCount = m_vCidTTFonts.size(); nIndex < nCount; nIndex++)
		{
			TFontInfo& oInfo = m_vCidTTFonts.at(nIndex);
			if (pCidFont == (CFontCidTrueType*)oInfo.pFont)
			{
				return CreateTrueTypeFont(oInfo.wsPath, oInfo.unIndex);
			}
		}

		return NULL;
	}
	CFont14* CDocument::GetDefaultCheckboxFont()
	{
		if (!m_pDefaultCheckBoxFont)
			m_pDefaultCheckBoxFont = new CFont14(m_pXref, this, EStandard14Fonts::standard14fonts_ZapfDingbats);

		return m_pDefaultCheckBoxFont;
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
	CImageTilePattern*CDocument::CreateImageTilePattern(double dW, double dH, CImageDict* pImageDict, CMatrix* pMatrix, EImageTilePatternType eType, double dXStepSpacing, double dYStepSpacing)
	{
		return new CImageTilePattern(m_pXref, dW, dH, pImageDict, pMatrix, eType, dXStepSpacing, dYStepSpacing);
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
	CResourcesDict* CDocument::GetFieldsResources()
	{
		if (!m_pFieldsResources)
		{
			if (!CheckAcroForm())
				return NULL;
			
			m_pFieldsResources = new CResourcesDict(m_pXref, false, true);
			m_pAcroForm->Add("DR", m_pFieldsResources);
		}

		return m_pFieldsResources;
	}
	CTextField* CDocument::CreateTextField()
	{
		if (!CheckAcroForm())
			return NULL;

		CTextField* pField = new CTextField(m_pXref, this);
		if (!pField)
			return NULL;

		CArrayObject* ppFields = (CArrayObject*)m_pAcroForm->Get("Fields");
		ppFields->Add(pField);

		return pField;
	}
	CChoiceField* CDocument::CreateChoiceField()
	{
		if (!CheckAcroForm())
			return NULL;

		CChoiceField* pField = new CChoiceField(m_pXref, this);
		if (!pField)
			return NULL;

		CArrayObject* ppFields = (CArrayObject*)m_pAcroForm->Get("Fields");
		ppFields->Add(pField);

		return pField;
	}
	CSignatureField* CDocument::CreateSignatureField()
	{
		if (!CheckAcroForm())
			return NULL;

		CSignatureField* pField = new CSignatureField(m_pXref, this);
		if (!pField)
			return NULL;

		CArrayObject* ppFields = (CArrayObject*)m_pAcroForm->Get("Fields");
		ppFields->Add(pField);

		return pField;
	}
	CDateTimeField* CDocument::CreateDateTimeField()
	{
		if (!CheckAcroForm())
			return NULL;
		
		CDateTimeField* pField = new CDateTimeField(m_pXref, this);
		if (!pField)
			return NULL;
		
		CArrayObject* ppFields = (CArrayObject*)m_pAcroForm->Get("Fields");
		ppFields->Add(pField);
		
		return pField;
	}
	CCheckBoxField* CDocument::CreateCheckBoxField()
	{
		if (!CheckAcroForm())
			return NULL;

		CCheckBoxField* pField = new CCheckBoxField(m_pXref, this);
		if (!pField)
			return NULL;

		CArrayObject* ppFields = (CArrayObject*)m_pAcroForm->Get("Fields");
		ppFields->Add(pField);

		return pField;
	}
	CRadioGroupField* CDocument::GetRadioGroupField(const std::wstring& wsGroupName)
	{
		CRadioGroupField* pField = FindRadioGroupField(wsGroupName);
		if (!pField)
		{
			if (!CheckAcroForm())
				return NULL;

			pField = new CRadioGroupField(m_pXref, this);
			if (!pField)
				return NULL;

			m_vRadioGroups.push_back(pField);

			pField->SetFieldName(wsGroupName);
			CArrayObject* ppFields = (CArrayObject*)m_pAcroForm->Get("Fields");
			ppFields->Add(pField);
		}

		return pField;
	}
	CRadioGroupField* CDocument::FindRadioGroupField(const std::wstring& wsGroupName)
	{
		CRadioGroupField* pField = NULL;
		for (unsigned int unIndex = 0, unCount = m_vRadioGroups.size(); unIndex < unCount; ++unIndex)
		{
			pField = m_vRadioGroups.at(unIndex);

			if (pField->GetFieldName() == wsGroupName)
				return pField;
		}

		return NULL;
	}
	CPictureField* CDocument::CreatePictureField()
	{
		if (!CheckAcroForm())
			return NULL;

		CPictureField* pField = new CPictureField(m_pXref, this);
		if (!pField)
			return NULL;

		CArrayObject* ppFields = (CArrayObject*)m_pAcroForm->Get("Fields");
		ppFields->Add(pField);

		return pField;
	}
	bool CDocument::HasImage(const std::wstring& wsImagePath, BYTE nAlpha)
	{
		for (size_t i = 0, nSize = m_vImages.size(); i < nSize; ++i)
		{
			if (m_vImages[i].wsImagePath == wsImagePath && m_vImages[i].nAlpha == nAlpha)
				return true;
		}
		return false;
	}
	CImageDict* CDocument::GetImage(const std::wstring& wsImagePath, BYTE nAlpha)
	{
		for (size_t i = 0, nSize = m_vImages.size(); i < nSize; ++i)
		{
			if (m_vImages[i].wsImagePath == wsImagePath && m_vImages[i].nAlpha == nAlpha)
			{
				m_pCurImage = m_vImages[i].pImage;
				return m_vImages[i].pImage;
			}
		}
		return NULL;
	}
	void CDocument::AddImage(const std::wstring& wsImagePath, BYTE nAlpha, CImageDict* pImage)
	{
		if (!pImage)
			return;
		m_pCurImage = pImage;
		m_vImages.push_back({wsImagePath, nAlpha, pImage});
	}
	void CDocument::AddObject(CObjectBase* pObj)
	{
		m_pXref->Add(pObj);
	}
	bool CDocument::CheckFieldName(CFieldBase* pField, const std::string& sName)
	{
		CFieldBase* pBase = m_mFields[sName];
		if (pBase)
		{
			if (!pBase->GetKidsCount())
			{
				CFieldBase* pParent = new CFieldBase(m_pXref, this);
				pParent->SetFieldName(sName, true);
				pParent->Add("Ff", pBase->GetFieldFlag());
				pParent->Add("FT", pBase->GetFieldType());

				CTextField* pTextField = dynamic_cast<CTextField*>(pBase);
				int nMaxLen = 0;
				if (pTextField)
				{
					CObjectBase* pT = pBase->Get("T");
					if (pT && pT->GetType() == object_type_STRING)
						pParent->Add("V", pT->Copy());

					if (0 != (nMaxLen = pTextField->GetMaxLen()))
					{
						pBase->Remove("MaxLen");
						pParent->Add("MaxLen", nMaxLen);
					}
				}

				pBase->SetParent(pParent);
				pBase->ClearKidRecords();
				pParent->AddKid(pBase);

				m_mFields[sName] = pParent;
				pField->ClearKidRecords();
				pField->SetParent(pParent);
				pParent->AddKid(pField);

				CChoiceField* pChoice = dynamic_cast<CChoiceField*>(pBase);
				if (pChoice)
					pChoice->UpdateSelectedIndexToParent();

				pParent->UpdateKidsPlaceHolder();
			}
			else
			{
				pField->ClearKidRecords();
				pField->SetParent(pBase);
				pBase->AddKid(pField);

				CChoiceField* pChoice = dynamic_cast<CChoiceField*>(pBase);
				if (pChoice)
					pChoice->UpdateSelectedIndexToParent();

				pBase->UpdateKidsPlaceHolder();
			}

			return true;
		}
		else
		{
			m_mFields[sName] = pField;
			return false;
		}
	}
	bool CDocument::CheckAcroForm()
	{
		if (!m_pXref || !m_pCatalog)
			return false;

		if (!m_pAcroForm)
		{
			m_pAcroForm = new CDictObject();
			if (!m_pAcroForm)
				return false;

			m_pCatalog->Add("AcroForm", m_pAcroForm);
			m_pAcroForm->Add("Fields", new CArrayObject());
		}

		return (!!m_pAcroForm);
	}
	void CDocument::SetAcroForm(CDictObject* pObj)
	{
		if (!m_pXref || !m_pCatalog)
			return;
		m_pCatalog->Add("AcroForm", pObj);
		m_pAcroForm = pObj;
	}
	CResourcesDict* CDocument::CreateResourcesDict(bool bInline, bool bProcSet)
	{
		return new CResourcesDict(m_pXref, bInline, bProcSet);
	}
	bool CDocument::CreatePageTree(CXref* pXref, CPageTree* pPageTree)
	{
		if (!pPageTree || !EditXref(pXref))
			return false;

		if (!m_pPageTree)
			m_pPageTree = pPageTree;
		else
			m_pPageTree->Join(pPageTree);

		return true;
	}
	bool CDocument::EditPdf(int nPosLastXRef, int nSizeXRef, CXref* pXref, CCatalog* pCatalog, CEncryptDict* pEncrypt, int nFormField)
	{
		if (!pXref || !pCatalog)
			return false;
		Close();

		m_pXref = new CXref(this, nSizeXRef);
		if (!m_pXref)
			return false;
		m_pXref->SetPrevAddr(nPosLastXRef);
		m_pLastXref = m_pXref;

		m_pTrailer = m_pXref->GetTrailer();
		if (!m_pTrailer)
			return false;

		SetCompressionMode(COMP_ALL);

		m_pCatalog = pCatalog;
		pXref->SetPrev(m_pLastXref);
		m_pLastXref = pXref;

		CObjectBase* pAcroForm = m_pCatalog->Get("AcroForm");
		if (pAcroForm && pAcroForm->GetType() == object_type_DICT)
			m_pAcroForm = (CDictObject*)pAcroForm;

		if (pEncrypt)
		{
			m_pEncryptDict = pEncrypt;
			m_bEncrypt = true;
		}

		m_unFormFields = nFormField;
		return true;
	}
	bool CDocument::EditResources(CXref* pXref, CResourcesDict* pResources)
	{
		if (!pResources || !EditXref(pXref))
			return false;

		CheckAcroForm();
		m_pAcroForm->Add("DR", pResources);
		m_pFieldsResources = pResources;
		return true;
	}
	std::pair<int, int> CDocument::GetPageRef(int nPageIndex)
	{
		std::pair<int, int> pRes = std::make_pair(0, 0);
		if (!m_pPageTree)
			return pRes;

		CObjectBase* pObj = m_pPageTree->GetObj(nPageIndex);
		if (pObj)
		{
			pRes.first  = pObj->GetObjId();
			pRes.second = pObj->GetGenNo();
		}

		return pRes;
	}
	bool CDocument::EditPage(CXref* pXref, CPage* pPage, int nPageIndex)
	{
		if (!pPage || !EditXref(pXref))
			return false;

		pPage->AddContents(m_pXref);
#ifndef FILTER_FLATE_DECODE_DISABLED
		if (m_unCompressMode & COMP_TEXT)
			pPage->SetFilter(STREAM_FILTER_FLATE_DECODE);
#endif

		m_pCurPage = pPage;
		m_mEditPages[nPageIndex] = pPage;

		if (m_pPageTree)
			m_pPageTree->ReplacePage(nPageIndex, pPage);

		return true;
	}
	void CDocument::AddEditPage(CPage* pPage, int nPageIndex)
	{
		m_mEditPages[nPageIndex] = pPage;
	}
	bool CDocument::EditAnnot(CXref* pXref, CAnnotation* pAnnot, int nID)
	{
		if (!pAnnot || !EditXref(pXref))
			return false;

		pAnnot->SetXref(m_pXref);
		m_mAnnotations[nID] = pAnnot;

		return true;
	}
	void CDocument::AddParent(int nID, CDictObject* pParent)
	{
		m_mParents[nID] = pParent;
	}
	CDictObject* CDocument::CreateParent(int nID)
	{
		CDictObject* pParent = new CDictObject();
		m_pXref->Add(pParent);
		m_mParents[nID] = pParent;
		return pParent;
	}
	bool CDocument::EditParent(CXref* pXref, CDictObject* pParent, int nID)
	{
		if (!pParent || !EditXref(pXref))
			return false;
		m_mParents[nID] = pParent;
		return true;
	}
	bool CDocument::EditXref(CXref* pXref)
	{
		if (!pXref)
			return true;

		pXref->SetPrev(m_pLastXref);
		m_pLastXref = pXref;

		return true;
	}
	bool CDocument::DeleteAnnot(int nObjNum, int nObjGen)
	{
		if (m_pCurPage && m_pCurPage->DeleteAnnotation(nObjNum))
		{
			CXref* pXref = new CXref(this, nObjNum, nObjGen);
			if (!pXref)
				return false;

			pXref->SetPrev(m_pLastXref);
			m_pLastXref = pXref;
			return true;
		}
		return false;
	}
	CAnnotation* CDocument::GetAnnot(int nID)
	{
		std::map<int, CAnnotation*>::iterator p = m_mAnnotations.find(nID);
		if (p != m_mAnnotations.end())
			return p->second;
		return NULL;
	}
	CDictObject* CDocument::GetParent(int nID)
	{
		std::map<int, CDictObject*>::iterator p = m_mParents.find(nID);
		if (p != m_mParents.end())
			return p->second;
		return NULL;
	}
	std::string CDocument::SetParentKids(int nParentID)
	{
		CDictObject* pParent = GetParent(nParentID);
		if (!pParent)
			return "";

		for (auto it = m_mAnnotations.begin(); it != m_mAnnotations.end(); it++)
		{
			CAnnotation* pAnnot = it->second;
			if (pAnnot->GetAnnotationType() != AnnotWidget)
				continue;

			CWidgetAnnotation* pWidget = (CWidgetAnnotation*)pAnnot;
			int nWidgetParentID = pWidget->GetParentID();
			if (nWidgetParentID != nParentID)
				continue;

			pWidget->SetParent(pParent);

			CObjectBase* pFT = pParent->Get("FT");
			CObjectBase* pWidgetFT = pWidget->Get("FT");
			if (!pFT && pParent->Get("T") && pWidgetFT)
				pParent->Add("FT", pWidgetFT->Copy());

			CArrayObject* pKids = dynamic_cast<CArrayObject*>(pParent->Get("Kids"));
			if (!pKids)
			{
				pKids = new CArrayObject();
				pParent->Add("Kids", pKids);
			}
			bool bReplase = false;
			int nID = pWidget->GetObjId();
			for (int i = 0; i < pKids->GetCount(); ++i)
			{
				CObjectBase* pKid = pKids->Get(i);
				if (pKid->GetObjId() == nID)
				{
					pKids->Insert(pKid, pWidget, true);
					bReplase = true;
					break;
				}
			}
			if (!bReplase)
				pKids->Add(pWidget);
		}

		CObjectBase* pFT = pParent->Get("FT");
		if (pFT && pFT->GetType() == object_type_NAME)
			return ((CNameObject*)pFT)->Get();
		return "";
	}
	CPage* CDocument::CreateFakePage()
	{
		return new CPage(this, NULL);
	}
	bool CDocument::EditCO(const std::vector<int>& arrCO)
	{
		if (arrCO.empty())
			return true;

		if (!CheckAcroForm())
			return false;

		CArrayObject* pArray = new CArrayObject();
		if (!pArray)
			return false;

		m_pAcroForm->Add("CO", pArray);

		for (int CO : arrCO)
		{
			CDictObject* pObj = GetParent(CO);
			if (pObj)
				pArray->Add(pObj);
			else
			{
				CAnnotation* pAnnot = m_mAnnotations[CO];
				if (pAnnot)
					pArray->Add(pAnnot);
			}
		}

		return true;
	}
	CPage* CDocument::AddPage(int nPageIndex, CPage* _pNewPage)
	{
		if (!m_pPageTree)
			return NULL;

		CPage* pNewPage = _pNewPage ? _pNewPage : new CPage(m_pXref, NULL, this);
		if (!pNewPage)
			return NULL;
		bool bRes = m_pPageTree->InsertPage(nPageIndex, pNewPage);
		if (!bRes)
			return NULL;

		if (!_pNewPage)
			pNewPage->SetFilter(STREAM_FILTER_FLATE_DECODE);
		m_pCurPage = pNewPage;
		return pNewPage;
	}
	bool CDocument::DeletePage(int nPageIndex)
	{
		if (!m_pPageTree)
			return false;

		CObjectBase* pObj = m_pPageTree->RemovePage(nPageIndex);
		if (pObj)
		{
			if (pObj->IsIndirect())
				return true;
			CXref* pXref = new CXref(this, pObj->GetObjId(), pObj->GetGenNo());
			delete pObj;
			if (!pXref)
				return false;

			pXref->SetPrev(m_pLastXref);
			m_pLastXref = pXref;
			return true;
		}
		return false;
	}
	bool CDocument::MovePage(int nPageIndex, int nPos)
	{
		if (m_pPageTree)
		{
			CObjectBase* pObj = m_pPageTree->RemovePage(nPageIndex);
			if (pObj->GetType() == object_type_DICT && ((CDictObject*)pObj)->GetDictType() == dict_type_PAGE)
				return m_pPageTree->InsertPage(nPos, (CPage*)pObj);
		}
		return false;
	}
	bool CDocument::AddToFile(const std::wstring& wsPath, CXref* pXref, CDictObject* pTrailer, CXref* pInfoXref, CInfoDict* pInfo)
	{
		if (!pTrailer || wsPath.empty())
			return false;

		CFileStream* pStream = new CFileStream();
		if (!pStream)
			return false;

		if (!pStream->OpenFile(wsPath, false))
		{
			RELEASEOBJECT(pStream);
			return false;
		}

		m_pTrailer = pTrailer;
		m_pInfo = pInfo;
		if (!m_pInfo)
			m_pInfo = new PdfWriter::CInfoDict(m_pXref);

		std::wstring sCreator = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
		if (sCreator.empty())
			sCreator = NSSystemUtils::gc_EnvApplicationNameDefault;
		std::string sCreatorA = U_TO_UTF8(sCreator);

#if defined(INTVER)
		sCreatorA += "/";
		sCreatorA += VALUE2STR(INTVER);
#endif

		const char* cCreator = m_pInfo->GetInfo(InfoProducer);
		m_pInfo->SetInfo(InfoCreator, cCreator ? cCreator : sCreatorA.c_str());
		m_pInfo->SetInfo(InfoProducer, sCreatorA.c_str());

		if (pInfoXref)
		{
			pInfoXref->SetPrev(m_pLastXref);
			m_pLastXref = pInfoXref;
		}
		pXref->SetPrev(m_pLastXref);
		m_pLastXref = pXref;

		// Вторая часть идентификатора должна обновляться
		CObjectBase* pID = m_pTrailer->Get("ID");
		if ((pID && pID->GetType() == object_type_ARRAY) || !m_vMetaOForms.empty())
		{
			BYTE arrId[16];
			CEncryptDict::CreateId(m_pInfo, m_pXref, (BYTE*)arrId);

			CArrayObject* pArrID = (CArrayObject*)pID;
			if (pArrID)
			{
				CObjectBase* pObject = pArrID->Get(1, false);
				pArrID->Insert(pObject, new CBinaryObject(arrId, 16), true);
			}
			else
			{
				pArrID = new CArrayObject();
				m_pTrailer->Add("ID", pArrID);

				pArrID->Add(new CBinaryObject(arrId, 16));
				pArrID->Add(new CBinaryObject(arrId, 16));
			}

			for (int i = 0; i < m_vMetaOForms.size(); ++i)
				m_vMetaOForms[i]->Add("ID", new CBinaryObject(arrId, 16));
		}

		CEncrypt* pEncrypt = NULL;
		if (m_bEncrypt)
			pEncrypt = m_pEncryptDict->GetEncrypt();

		// Если m_pTrailer поток перекрестных ссылок, то при дозаписи тоже должен быть поток
		m_pTrailer->Remove("XRefStm");
		bool bNeedStreamXRef = false;
		pStream->WriteChar('\n');
		if (m_pTrailer->Get("Type"))
		{
			m_pTrailer->Remove("Length");
			m_pTrailer->Remove("Filter");
			m_pTrailer->Remove("DecodeParms");
			m_pTrailer->Remove("F");
			m_pTrailer->Remove("FFilter");
			m_pTrailer->Remove("FDecodeParms");
			m_pTrailer->Remove("DL");
			m_pTrailer->Remove("Type");
			m_pTrailer->Remove("Index");
			m_pTrailer->Remove("W");
			bNeedStreamXRef = true;

			m_pLastXref->WriteToStream(pStream, pEncrypt, bNeedStreamXRef);
		}
		else
			m_pLastXref->WriteToStream(pStream, pEncrypt);

		RELEASEOBJECT(pStream);
		unsigned int nSizeXRef = m_pXref->GetSizeXRef();
		m_pXref = m_pLastXref;
		Sign(wsPath, nSizeXRef, bNeedStreamXRef);
		RELEASEOBJECT(m_pEncryptDict);

		return true;
	}
	void CDocument::Sign(const TRect& oRect, CImageDict* pImage, ICertificate* pCertificate)
	{
		m_vSignatures.push_back({ oRect, m_pCurPage ? m_pCurPage : m_pPageTree->GetPage(0), pImage, pCertificate });
	}
	void CDocument::Sign(const std::wstring& wsPath, unsigned int nSizeXRef, bool bNeedStreamXRef)
	{
		unsigned int nPrevAddr = m_pXref->GetPrevAddr();
		std::vector<CXref*> vXRefForWrite;
		for (unsigned int i = 0; i < m_vSignatures.size(); i++)
		{
			CXref* pXrefBefore = m_pXref;
			m_pXref = new CXref(this, nSizeXRef);
			if (!m_pXref)
			{
				m_pXref = pXrefBefore;
				continue;
			}
			m_pXref->SetPrevAddr(nPrevAddr);

			CSignatureField* pField = CreateSignatureField();
			if (!pField)
			{
				RELEASEOBJECT(m_pXref);
				m_pXref = pXrefBefore;
				continue;
			}

			m_pAcroForm->Add("SigFlags", 3);
			pField->GetSignatureDict()->SetCert(m_vSignatures[i].pCertificate);
			pField->GetSignatureDict()->SetDate();
			pField->AddPageRect(m_vSignatures[i].pPage, m_vSignatures[i].oRect);
			pField->Add("F", 132);
			pField->SetFieldName("Sig" + std::to_string(i + m_unFormFields + 1));
			if (m_vSignatures[i].pImage)
				pField->SetAppearance(m_vSignatures[i].pImage);

			CFileStream* pStream = new CFileStream();
			if (!pStream || !pStream->OpenFile(wsPath, false))
			{
				RELEASEOBJECT(m_pXref);
				m_pXref = pXrefBefore;
				continue;
			}

			CXref* pXrefCatalog = new CXref(this, m_pCatalog->GetObjId());
			if (pXrefCatalog)
			{
				pXrefCatalog->Add(m_pCatalog->Copy(), m_pCatalog->GetGenNo());
				pXrefCatalog->SetPrev(m_pXref);
			}

			CXref* pXrefPage = new CXref(this, m_vSignatures[i].pPage->GetObjId());
			if (pXrefPage)
			{
				pXrefPage->Add(m_vSignatures[i].pPage->Copy(), m_vSignatures[i].pPage->GetGenNo());
				pXrefPage->SetPrev(pXrefCatalog);
			}

			CXref* pXref = new CXref(this, 0, 65535);
			if (pXref)
			{
				pXref->SetPrev(pXrefPage);
				CDictObject* pTrailer = pXref->GetTrailer();
				m_pTrailer->Copy(pTrailer);

				CEncrypt* pEncrypt = NULL;
				if (m_bEncrypt && m_pEncryptDict)
					pEncrypt = m_pEncryptDict->GetEncrypt();

				pXref->WriteToStream(pStream, pEncrypt, bNeedStreamXRef);
				nPrevAddr = pXref->GetPrevAddr();
				nSizeXRef = m_pXref->GetSizeXRef();
				vXRefForWrite.push_back(pXref);
			}

			RELEASEOBJECT(pStream);
			pStream = new CFileStream();
			if (pStream && pStream->OpenFile(wsPath, false))
				pField->GetSignatureDict()->WriteToStream(pStream, pStream->Size());

			m_pXref = pXrefBefore;
			RELEASEOBJECT(pStream);
		}
		for (CXref* XRef : vXRefForWrite)
			RELEASEOBJECT(XRef);
		vXRefForWrite.clear();
	}
	void CDocument::AddShapeXML(const std::string& sXML)
	{
		CDictObject* pResources = (CDictObject*)m_pCurPage->GetResourcesItem();
		if (!pResources)
		{
			pResources = new CResourcesDict(NULL, true, false);
			m_pCurPage->Add("Resources", pResources);
		}
		CDictObject* pProperties = (CDictObject*)pResources->Get("Properties");
		if (!pProperties)
		{
			pProperties = new CDictObject();
			pResources->Add("Properties", pProperties);
		}
		CObjectBase* pObj = pProperties->Get("OShapes");
		if (pObj && pObj->GetType() != object_type_DICT)
		{
			pProperties->Remove("OShapes");
			pObj = NULL;
		}

		CBinaryObject* sID = NULL;
		CArrayObject* pID = (CArrayObject*)m_pTrailer->Get("ID");
		if (!pID)
		{
			BYTE arrId[16];
			CEncryptDict::CreateId(m_pInfo, m_pXref, (BYTE*)arrId);

			pID = new CArrayObject();
			m_pTrailer->Add("ID", pID);

			pID->Add(new CBinaryObject(arrId, 16));
			pID->Add(new CBinaryObject(arrId, 16));

			sID = new CBinaryObject(arrId, 16);
		}
		else
			sID = (CBinaryObject*)pID->Get(1)->Copy();

		CDictObject* pMetaOForm = (CDictObject*)pObj;
		if (!pMetaOForm)
		{
			pMetaOForm = new CDictObject();
			m_pXref->Add(pMetaOForm);
			pMetaOForm->Add("Type", "OShapes");
			pProperties->Add("OShapes", pMetaOForm);
			m_vMetaOForms.push_back(pMetaOForm);
			pMetaOForm->Add("IDF", sID->Copy());
			pMetaOForm->Add("ID", sID->Copy());
		}
		CArrayObject* pArrayMeta = (CArrayObject*)pMetaOForm->Get("Metadata");
		if (!pArrayMeta)
		{
			pArrayMeta = new CArrayObject();
			pMetaOForm->Add("Metadata", pArrayMeta);
			CArrayObject* pArrayImage = new CArrayObject();
			pMetaOForm->Add("Image", pArrayImage);
		}
		pArrayMeta->Add(new CStringObject(sXML.c_str()));

		CDictObject* pBDC = new CDictObject();
		pBDC->Add("MCID", pArrayMeta->GetCount() - 1);
		pBDC->Add("IDF", sID);
		m_pCurPage->BeginMarkedContentDict("OShapes", pBDC);
		RELEASEOBJECT(pBDC);
	}
	void CDocument::EndShapeXML()
	{
		CDictObject* pResources = (CDictObject*)m_pCurPage->GetResourcesItem();
		if (!pResources)
			return;
		CDictObject* pProperties = (CDictObject*)pResources->Get("Properties");
		if (!pProperties)
			return;
		CObjectBase* pObj = pProperties->Get("OShapes");
		if (!pObj || pObj->GetType() != object_type_DICT)
			return;
		CDictObject* pMetaOForm = (CDictObject*)pObj;
		CArrayObject* pArrayImage = (CArrayObject*)pMetaOForm->Get("Image");
		if (!pArrayImage)
			return;

		pObj = m_pCurImage;
		if (!pObj)
			pObj = new PdfWriter::CNullObject();
		pArrayImage->Add(pObj);

		m_pCurPage->EndMarkedContent();
	}
	void CDocument::ClearPage()
	{
		m_pCurPage->ClearContent(m_pXref);
		m_pCurPage->StartTransform(1, 0, 0, 1, 0, 0);
	}
}
