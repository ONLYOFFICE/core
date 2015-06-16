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

#ifdef CreateFont
#undef CreateFont
#endif

namespace PdfWriter
{
	const char* c_sPdfHeader = "%PDF-1.7\012%ASC\012";
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
	}
	CDocument::~CDocument()
	{
		Close();
	}
	bool              CDocument::CreateNew()
	{
		Close();

		m_pXref = new CXref(0);
		if (!m_pXref)
			return false;

		m_pTrailer = m_pXref->GetTrailer();
		if (!m_pTrailer)
			return false;

		m_pCatalog = new CCatalog(m_pXref);
		if (!m_pCatalog)
			return false;

		m_pCatalog->SetPageMode(pagemode_UseNone);
		m_pCatalog->SetPageLayout(pagelayout_Single);

		m_pPageTree = m_pCatalog->GetRoot();
		if (!m_pPageTree)
			return false;

		m_pInfo = new CInfoDict(m_pXref);
		if (!m_pInfo)
			return false;

		m_pInfo->SetCreationTime();
		m_pInfo->SetInfo(InfoProducer, "Ascensio System SIA Copyright (c) 2015");

		m_nCurPageNum = -1;

		m_vPages.clear();

		return true;
	}
	void              CDocument::Close()
	{
		// Все объекты удаляются внутри CXref
		if (m_pXref)
		{
			delete m_pXref;
			m_pXref = NULL;
		}

		m_pTrailer          = NULL;
		m_pCatalog          = NULL;
		m_pOutlines         = NULL;
		m_pPageTree         = NULL;
		m_pCurPage          = NULL;
		m_bEncrypt          = false;
		m_pEncryptDict      = NULL;
		m_pInfo             = NULL;
		m_unCompressMode    = COMP_NONE;
		memset((void*)m_sTTFontTag, 0x00, 8);

		m_vExtGrStates.clear();
		m_vPages.clear();
		m_vShadings.clear();
	}
	bool              CDocument::SaveToFile(const std::wstring& wsPath)
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
	void              CDocument::SaveToStream(CStream* pStream)
	{
		unsigned long nRet = OK;

		// Пишем заголовок
		pStream->WriteStr(c_sPdfHeader);

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
	void              CDocument::PrepareEncryption()
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

		pID->Add(new CBinaryObject(pEncrypt->m_anEncryptID, ID_LEN));
		pID->Add(new CBinaryObject(pEncrypt->m_anEncryptID, ID_LEN));
	}
	void              CDocument::SetEncryptionMode(EEncryptMode eMode, unsigned int unKeyLen)
	{
		if (!m_bEncrypt)
			return;

		CEncrypt* pEncrypt = m_pEncryptDict->GetEncrypt();
		pEncrypt->SetMode(eMode, unKeyLen);
	}
	void              CDocument::SetPassword(const char* sOwnerPassword, const char* sUserPassword)
	{
		if (!m_pEncryptDict)
			m_pEncryptDict = new CEncryptDict(m_pXref);

		if (!m_pEncryptDict)
			return;

		m_pEncryptDict->SetPassword(sOwnerPassword, sUserPassword);
		m_pTrailer->Add("Encrypt", m_pEncryptDict);
		m_bEncrypt = true;
	}
	CPage*            CDocument::AddPage()
	{
		CPage* pPage = new CPage(m_pXref, m_pPageTree);
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
	void              CDocument::SetPermission(unsigned int unPermission)
	{
		if (!m_bEncrypt)
			return;

		CEncrypt* pEncrypt = m_pEncryptDict->GetEncrypt();
		pEncrypt->SetPermission(unPermission);
	}
	void              CDocument::SetCompressionMode(unsigned int unMode)
	{
		m_unCompressMode = unMode;
	}
	void              CDocument::AddPageLabel(EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix)
	{
		CDictObject* pPageLabel = CreatePageLabel(eStyle, unFirstPage, sPrefix);
		if (!pPageLabel)
			return;

		m_pCatalog->AddPageLabel(m_nCurPageNum, pPageLabel);
	}
	void              CDocument::AddPageLabel(unsigned int unPageNum, EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix)
	{
		CDictObject* pPageLabel = CreatePageLabel(eStyle, unFirstPage, sPrefix);
		if (!pPageLabel)
			return;

		m_pCatalog->AddPageLabel(unPageNum, pPageLabel);
	}
	CDictObject*      CDocument::CreatePageLabel(EPageNumStyle eStyle, unsigned int unFirstPage, const char* sPrefix)
	{
		CDictObject* pLabel = new CDictObject();
		if (!pLabel)
			return NULL;

		eStyle = min(max(eStyle, pagenumstyle_Min), pagenumstyle_Max);
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
	COutline*         CDocument::CreateOutline(COutline* pParent, const char* sTitle)
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
	CDestination*     CDocument::CreateDestination(unsigned int unPageIndex)
	{
		if (unPageIndex >= m_vPages.size())
			return NULL;

		CPage* pPage = m_vPages.at(unPageIndex);
		return new CDestination(pPage, m_pXref);
	}
	CExtGrState*      CDocument::FindExtGrState(double dAlphaStroke, double dAlphaFill, EBlendMode eMode, int nStrokeAdjustment)
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
	CExtGrState*      CDocument::GetExtGState(double dAlphaStroke, double dAlphaFill, EBlendMode eMode, int nStrokeAdjustment)
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
	CAnnotation*      CDocument::CreateTextAnnot(unsigned int unPageNum, TRect oRect, const char* sText)
	{
		CAnnotation* pAnnot = new CTextAnnotation(m_pXref, oRect, sText);
		if (pAnnot)
		{
			CPage* pPage = m_vPages.at(unPageNum);
			pPage->AddAnnotation(pAnnot);
		}

	    return pAnnot;
	}
	CAnnotation*      CDocument::CreateLinkAnnot(unsigned int unPageNum, TRect oRect, CDestination* pDest)
	{
		CAnnotation* pAnnot = new CLinkAnnotation(m_pXref, oRect, pDest);
	    
		if (pAnnot)
		{
			CPage* pPage = m_vPages.at(unPageNum);
			pPage->AddAnnotation(pAnnot);
		}
	
	    return pAnnot;
	}	
	CAnnotation*      CDocument::CreateUriLinkAnnot(unsigned int unPageNum, TRect oRect, const char* sUri)
	{
		CAnnotation* pAnnot = new CUriLinkAnnotation(m_pXref, oRect, sUri);
	
		if (pAnnot)
		{
			CPage* pPage = m_vPages.at(unPageNum);
			pPage->AddAnnotation(pAnnot);
		}

		return pAnnot;
	}
	CImageDict*       CDocument::CreateImage()
	{
		return new CImageDict(m_pXref, this);
	}
	CFont14*          CDocument::CreateFont14(EStandard14Fonts eType)
	{
		return new CFont14(m_pXref, this, eType);
	}
	CFontCidTrueType* CDocument::CreateTrueTypeFont(const std::wstring& wsFontPath, unsigned int unIndex)
	{
		return new CFontCidTrueType(m_pXref, this, wsFontPath, unIndex);
	}
	char*             CDocument::GetTTFontTag()
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
	CJbig2Global*     CDocument::GetJbig2Global()
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
	CShading*         CDocument::CreateAxialShading(double dX0, double dY0, double dX1, double dY1, unsigned char* pColors, double* pPoints, int nCount)
	{
		for (int nIndex = 0, nShadingsCount = m_vShadings.size(); nIndex < nShadingsCount; nIndex++)
		{
			CShading* pShading = m_vShadings.at(nIndex);
			if (shadingtype_Axial == pShading->GetShadingType()
				&& ((CAxialShading*)pShading)->Compare(dX0, dY0, dX1, dY1)
				&& pShading->CompareColors(pColors, pPoints, nCount)
				&& pShading->CompareExtend(true, true))
				return pShading;
		}

		CAxialShading* pShading = new CAxialShading(m_pXref, dX0, dY0, dX1, dY1);
		if (!pShading)
			return NULL;

		pShading->SetColors(pColors, pPoints, nCount);
		pShading->SetExtend(true, true);

		m_vShadings.push_back(pShading);

		return pShading;
	}
	CShading*         CDocument::CreateRaidalShading(double dX0, double dY0, double dR0, double dX1, double dY1, double dR1, unsigned char* pColors, double* pPoints, int nCount)
	{
		for (int nIndex = 0, nShadingsCount = m_vShadings.size(); nIndex < nShadingsCount; nIndex++)
		{
			CShading* pShading = m_vShadings.at(nIndex);
			if (shadingtype_Radial == pShading->GetShadingType()
				&& ((CRadialShading*)pShading)->Compare(dX0, dY0, dR0, dX1, dY1, dR1)
				&& pShading->CompareColors(pColors, pPoints, nCount)
				&& pShading->CompareExtend(true, true))
				return pShading;
		}

		CRadialShading* pShading = new CRadialShading(m_pXref, dX0, dY0, dR0, dX1, dY1, dR1);
		if (!pShading)
			return NULL;

		pShading->SetColors(pColors, pPoints, nCount);
		pShading->SetExtend(true, true);

		m_vShadings.push_back(pShading);

		return pShading;
	}
	CImageTilePattern*CDocument::CreateImageTilePattern(double dW, double dH, CImageDict* pImageDict, EImageTilePatternType eType)
	{
		return new CImageTilePattern(m_pXref, dW, dH, pImageDict, eType);
	}
}
