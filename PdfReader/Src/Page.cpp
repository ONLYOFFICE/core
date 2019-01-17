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
#include <stddef.h>
#include "GlobalParams.h"
#include "Object.h"
#include "Array.h"
#include "Dict.h"
#include "XRef.h"
#include "Link.h"
#include "RendererOutputDev.h"
#include "OutputDevice.h"
#include "Graphics.h"
#include "GState.h"
#include "Annot.h"
#include "Catalog.h"
#include "Page.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// PDFRectangle
	//------------------------------------------------------------------------

	void PDFRectangle::ClipTo(PDFRectangle *pRect)
	{
		if (m_dLeft < pRect->m_dLeft)
		{
			m_dLeft = pRect->m_dLeft;
		}
		else if (m_dLeft > pRect->m_dRight)
		{
			m_dLeft = pRect->m_dRight;
		}
		if (m_dRight < pRect->m_dLeft)
		{
			m_dRight = pRect->m_dLeft;
		}
		else if (m_dRight > pRect->m_dRight)
		{
			m_dRight = pRect->m_dRight;
		}
		if (m_dBottom < pRect->m_dBottom)
		{
			m_dBottom = pRect->m_dBottom;
		}
		else if (m_dBottom > pRect->m_dTop)
		{
			m_dBottom = pRect->m_dTop;
		}
		if (m_dTop < pRect->m_dBottom)
		{
			m_dTop = pRect->m_dBottom;
		}
		else if (m_dTop > pRect->m_dTop)
		{
			m_dTop = pRect->m_dTop;
		}
	}

	//------------------------------------------------------------------------
	// PageAttrs
	//------------------------------------------------------------------------

	PageAttrs::PageAttrs(PageAttrs *pAttrs, Dict *pDict)
	{
		Object obj1;

		if (pAttrs) // Предыдущие настройки
		{
			m_oMediaBox    = pAttrs->m_oMediaBox;
			m_pCropBox     = pAttrs->m_pCropBox;
			m_bHaveCropBox = pAttrs->m_bHaveCropBox;
			m_nRotate      = pAttrs->m_nRotate;
			pAttrs->m_oResources.Copy(&m_oResources);
		}
		else // Настройки по умолчанию
		{
			m_oMediaBox.m_dLeft    = 0;
			m_oMediaBox.m_dBottom  = 0;
			m_oMediaBox.m_dRight   = 612;
			m_oMediaBox.m_dTop     = 792;

			m_pCropBox.m_dLeft = m_pCropBox.m_dBottom = m_pCropBox.m_dRight = m_pCropBox.m_dTop = 0;
			m_bHaveCropBox = false;
			m_nRotate = 0;
			m_oResources.InitNull();
		}

		// MediaBox
		ReadBox(pDict, "MediaBox", &m_oMediaBox);

		// CropBox
		if (ReadBox(pDict, "CropBox", &m_pCropBox))
		{
			m_bHaveCropBox = true;
		}
		if (!m_bHaveCropBox)
		{
			m_pCropBox = m_oMediaBox;
		}

		// Остальные
		m_oBleedBox = m_pCropBox;
		ReadBox(pDict, "BleedBox", &m_oBleedBox);

		m_oTrimBox = m_pCropBox;
		ReadBox(pDict, "TrimBox", &m_oTrimBox);

		m_oArtBox = m_pCropBox;
		ReadBox(pDict, "ArtBox", &m_oArtBox);

		// Подгоняем размеры под m_oMediaBox 
		m_pCropBox.ClipTo(&m_oMediaBox);
		m_oBleedBox.ClipTo(&m_oMediaBox);
		m_oTrimBox.ClipTo(&m_oMediaBox);
		m_oArtBox.ClipTo(&m_oMediaBox);

		// Поворот
		Object oTemp;
		pDict->Search("Rotate", &oTemp);
		if (oTemp.IsInt())
		{
			m_nRotate = oTemp.GetInt();
		}
		oTemp.Free();
		while (m_nRotate < 0)
		{
			m_nRotate += 360;
		}
		while (m_nRotate >= 360)
		{
			m_nRotate -= 360;
		}

		pDict->Search("LastModified", &m_oLastModified);
		pDict->Search("BoxColorInfo", &m_oBoxColorInfo);
		pDict->Search("Group", &m_oGroup);
		pDict->Search("Metadata", &m_oMetadata);
		pDict->Search("PieceInfo", &m_oPieceInfo);
		pDict->Search("SeparationInfo", &m_oSeparationInfo);

		// Resources
		pDict->Search("Resources", &oTemp);
		if (oTemp.IsDict())
		{
			m_oResources.Free();
			oTemp.Copy(&m_oResources);
		}
		oTemp.Free();
	}

	PageAttrs::~PageAttrs()
	{
		m_oLastModified.Free();
		m_oBoxColorInfo.Free();
		m_oGroup.Free();
		m_oMetadata.Free();
		m_oPieceInfo.Free();
		m_oSeparationInfo.Free();
		m_oResources.Free();
	}

	bool PageAttrs::ReadBox(Dict *pDict, char *sKey, PDFRectangle *pBox)
	{
		PDFRectangle oTempBox;
		bool bSuccess = false;

		Object oBox;
		pDict->Search(sKey, &oBox);
		if (oBox.IsArray() && oBox.ArrayGetLength() == 4)
		{
			bSuccess = true;
			Object oTemp;
			oBox.ArrayGet(0, &oTemp);
			if (oTemp.IsNum())
			{
				oTempBox.m_dLeft = oTemp.GetNum();
			}
			else
			{
				bSuccess = false;
			}
			oTemp.Free();
			oBox.ArrayGet(1, &oTemp);
			if (oTemp.IsNum())
			{
				oTempBox.m_dBottom = oTemp.GetNum();
			}
			else
			{
				bSuccess = false;
			}
			oTemp.Free();
			oBox.ArrayGet(2, &oTemp);
			if (oTemp.IsNum())
			{
				oTempBox.m_dRight = oTemp.GetNum();
			}
			else
			{
				bSuccess = false;
			}
			oTemp.Free();
			oBox.ArrayGet(3, &oTemp);
			if (oTemp.IsNum())
			{
				oTempBox.m_dTop = oTemp.GetNum();
			}
			else
			{
				bSuccess = false;
			}
			oTemp.Free();
			if (bSuccess)
			{
				if (oTempBox.m_dLeft > oTempBox.m_dRight)
				{
					double dTempValue = oTempBox.m_dLeft;
					oTempBox.m_dLeft = oTempBox.m_dRight;
					oTempBox.m_dRight = dTempValue;
				}
				if (oTempBox.m_dBottom > oTempBox.m_dTop)
				{
					double dTempValue = oTempBox.m_dBottom;
					oTempBox.m_dBottom = oTempBox.m_dTop;
					oTempBox.m_dTop = dTempValue;
				}
				*pBox = oTempBox;
			}
		}
		else
		{
			bSuccess = false;
		}
		oBox.Free();
		return bSuccess;
	}

	//------------------------------------------------------------------------
	// Page
	//------------------------------------------------------------------------

	Page::Page(GlobalParams *pGlobalParams, XRef *pXref, int nNum, Dict *pPageDict, PageAttrs *pAttrs)
	{
		m_pGlobalParams = pGlobalParams;

		m_bValid = true;
		m_pXref = pXref;
		m_nNumber = nNum;

		// Attributes
		m_pAttrs = pAttrs;

		// Annotations
		pPageDict->SearchAndCopy("Annots", &m_oAnnots);
		if (!(m_oAnnots.IsRef() || m_oAnnots.IsArray() || m_oAnnots.IsNull()))
		{
			// TO DO: Error "Page annotations object is wrong type"
			m_oAnnots.Free();
			m_oAnnots.InitNull();
			m_oContents.InitNull();
			m_bValid = false;
		}

		// Contents
		pPageDict->SearchAndCopy("Contents", &m_oContents);
		if (!(m_oContents.IsRef() || m_oContents.IsArray() || m_oContents.IsNull()))
		{
			// TO DO: Error "Page contents object is wrong type"
			m_oContents.Free();
			m_oContents.InitNull();
			m_bValid = false;
		}

		////----------------------------------------------------------------
		//
		//	Object oContent;
		//	m_oContents.Fetch( m_pXref, &oContent );
		//	if ( oContent.IsArray() ) 
		//	{
		//		int m_nStreamsCount = oContent.ArrayGetLength();
		//		for ( int nIndex = 0; nIndex < oContent.ArrayGetLength(); ++nIndex ) 
		//		{
		//			Object oTemp;
		//			oContent.ArrayGet( nIndex, &oTemp );
		//			if ( oTemp.IsStream() ) 
		//			{
		//				Stream *pStream = oTemp.GetStream();
		//				pStream->Reset();
		//				while ( EOF != pStream->GetChar() );
		//			}
		//			oTemp.Free();
		//		}
		//	} 
		//	else if ( oContent.IsStream() ) 
		//	{
		//		Stream *pStream = oContent.GetStream();
		//		pStream->Reset();
		//
		//		int nSize = 128, nPos = 0;
		//		char *sBuffer = (char *)MemUtilsMalloc( nSize );
		//		int nChar = 0;
		//		while ( EOF != ( nChar = pStream->GetChar() ) )
		//		{
		//			if ( nPos + 1 >= nSize )
		//			{
		//				nSize *= 2;
		//				sBuffer = (char *)MemUtilsRealloc( (void *)sBuffer, nSize );
		//			}
		//			sBuffer[nPos] = nChar;
		//			nPos++;
		//		}
		//		sBuffer[nPos] = '\0';
		//
		//		Object oTemp;
		//		Stream *pNewStream = new MemoryStream( sBuffer, 0, nPos, &oTemp );
		//
		//		//int m_nStreamsCount = 1;
		//		//m_ppStreams = (Stream**)MemUtilsMalloc( sizeof(Stream*) );
		//		//m_ppStreams[0] = oContent.GetStream();
		//		//m_ppStreams[0]->Reset();
		//
		//		//while ( EOF != m_ppStreams[0]->GetChar() );
		//
		//		m_oContents2.InitStream( pNewStream );
		//	}
		//	
		//	oContent.Free();
		////	
		//////----------------------------------------------------------------
		return;
	}

	Page::~Page()
	{
		delete m_pAttrs;
		m_oAnnots.Free();
		m_oContents.Free();
	}

	Links *Page::GetLinks(Catalog *pCatalog)
	{
		Object oTemp;

		Links *pLinks = new Links(GetAnnots(&oTemp), pCatalog->GetBaseURI());
		oTemp.Free();
		return pLinks;
	}

	void Page::Display(OutputDev *pOut, double dHorizDPI, double dVertDPI, int nRotate, bool bUseMediaBox, bool bCrop, bool bPrinting, Catalog *pCatalog, bool(*abortCheckCbk)(void *pData), void *abortCheckCbkData)
	{
		DisplaySlice(pOut, dHorizDPI, dVertDPI, nRotate, bUseMediaBox, bCrop, -1, -1, -1, -1, bPrinting, pCatalog, abortCheckCbk, abortCheckCbkData);
	}

	void Page::DisplaySlice(OutputDev *pOut, double dHorizDPI, double dVertDPI, int nRotate, bool bUseMediaBox, bool bCrop, int nSliceX, int nSliceY, int nSliceW, int nSliceH, bool bPrinting, Catalog *pCatalog, bool(*abortCheckCbk)(void *pData), void *abortCheckCbkData)
	{
		if (!pOut->CheckPageSlice(this, dHorizDPI, dVertDPI, nRotate, bUseMediaBox, bCrop, nSliceX, nSliceY, nSliceW, nSliceH, bPrinting, pCatalog, abortCheckCbk, abortCheckCbkData))
		{
			return;
		}

		nRotate += GetRotate();
		if (nRotate >= 360)
		{
			nRotate -= 360;
		}
		else if (nRotate < 0)
		{
			nRotate += 360;
		}

		PDFRectangle oBox;
		MakeBox(dHorizDPI, dVertDPI, nRotate, bUseMediaBox, pOut->UpSideDown(), nSliceX, nSliceY, nSliceW, nSliceH, &oBox, &bCrop);
		PDFRectangle *pCropBox = GetCropBox();

		Graphics *pGraphics = new Graphics(m_pGlobalParams, m_pXref, pOut, m_nNumber, m_pAttrs->GetResourceDict(), dHorizDPI, dVertDPI, &oBox, bCrop ? pCropBox : (PDFRectangle *)NULL, nRotate, abortCheckCbk, abortCheckCbkData);
		if (pGraphics)
		{
			Object oTemp;
			m_oContents.Fetch(m_pXref, &oTemp);
			if (!oTemp.IsNull())
			{
				pGraphics->SaveGState();
				pGraphics->Display(&oTemp);
				pGraphics->RestoreGState();
			}
			oTemp.Free();

			Annots *pAnnotList = new Annots(m_pGlobalParams, m_pXref, pCatalog, GetAnnots(&oTemp));
			oTemp.Free();
			if (pAnnotList)
			{
				Dict *pAcroForm = pCatalog->GetAcroForm()->IsDict() ? pCatalog->GetAcroForm()->GetDict() : NULL;
				if (pAcroForm)
				{
					if (pAcroForm->Search("NeedAppearances", &oTemp))
					{
						if (oTemp.IsBool() && oTemp.GetBool())
						{
							pAnnotList->GenerateAppearances(pAcroForm);
						}
					}
					oTemp.Free();
				}
				if (pAnnotList->GetAnnotsCount() > 0)
				{
					for (int nIndex = 0; nIndex < pAnnotList->GetAnnotsCount(); ++nIndex)
					{
						pAnnotList->GetAnnot(nIndex)->Draw(pGraphics, bPrinting);
					}
					pOut->Dump();
				}
				delete pAnnotList;
			}
			delete pGraphics;
		}
	}

	void Page::MakeBox(double dHorizDPI, double dVertDPI, int nRotate, bool bUseMediaBox, bool bUpSideDown, double dSliceX, double dSliceY, double dSliceW, double dSliceH, PDFRectangle *pBox, bool *pbCrop)
	{
		PDFRectangle *pMediaBox = GetMediaBox();
		PDFRectangle *pCropBox  = GetCropBox();

		if (dSliceW >= 0 && dSliceH >= 0)
		{
			PDFRectangle *pBaseBox = bUseMediaBox ? pMediaBox : pCropBox;
			double dKoefX = 72.0 / dHorizDPI;
			double dKoefY = 72.0 / dVertDPI;
			if (nRotate == 90)
			{
				if (bUpSideDown)
				{
					pBox->m_dLeft  = pBaseBox->m_dLeft + dKoefY *  dSliceY;
					pBox->m_dRight = pBaseBox->m_dLeft + dKoefY * (dSliceY + dSliceH);
				}
				else
				{
					pBox->m_dLeft  = pBaseBox->m_dRight - dKoefY * (dSliceY + dSliceH);
					pBox->m_dRight = pBaseBox->m_dRight - dKoefY *  dSliceY;
				}
				pBox->m_dBottom = pBaseBox->m_dBottom + dKoefX *  dSliceX;
				pBox->m_dTop    = pBaseBox->m_dBottom + dKoefX * (dSliceX + dSliceW);
			}
			else if (nRotate == 180)
			{
				pBox->m_dLeft  = pBaseBox->m_dRight - dKoefX * (dSliceX + dSliceW);
				pBox->m_dRight = pBaseBox->m_dRight - dKoefX *  dSliceX;
				if (bUpSideDown)
				{
					pBox->m_dBottom = pBaseBox->m_dBottom + dKoefY *  dSliceY;
					pBox->m_dTop    = pBaseBox->m_dBottom + dKoefY * (dSliceY + dSliceH);
				}
				else
				{
					pBox->m_dBottom = pBaseBox->m_dTop - dKoefY * (dSliceY + dSliceH);
					pBox->m_dTop    = pBaseBox->m_dTop - dKoefY *  dSliceY;
				}
			}
			else if (nRotate == 270)
			{
				if (bUpSideDown)
				{
					pBox->m_dLeft  = pBaseBox->m_dRight - dKoefY * (dSliceY + dSliceH);
					pBox->m_dRight = pBaseBox->m_dRight - dKoefY *  dSliceY;
				}
				else
				{
					pBox->m_dLeft  = pBaseBox->m_dLeft + dKoefY *  dSliceY;
					pBox->m_dRight = pBaseBox->m_dLeft + dKoefY * (dSliceY + dSliceH);
				}
				pBox->m_dBottom = pBaseBox->m_dTop - dKoefX * (dSliceX + dSliceW);
				pBox->m_dTop    = pBaseBox->m_dTop - dKoefX *  dSliceX;
			}
			else
			{
				pBox->m_dLeft  = pBaseBox->m_dLeft + dKoefX *  dSliceX;
				pBox->m_dRight = pBaseBox->m_dLeft + dKoefX * (dSliceX + dSliceW);
				if (bUpSideDown)
				{
					pBox->m_dBottom = pBaseBox->m_dTop - dKoefY * (dSliceY + dSliceH);
					pBox->m_dTop    = pBaseBox->m_dTop - dKoefY *  dSliceY;
				}
				else
				{
					pBox->m_dBottom = pBaseBox->m_dBottom + dKoefY *  dSliceY;
					pBox->m_dTop    = pBaseBox->m_dBottom + dKoefY * (dSliceY + dSliceH);
				}
			}
		}
		else if (bUseMediaBox)
		{
			*pBox = *pMediaBox;
		}
		else
		{
			*pBox = *pCropBox;
			*pbCrop = false;
		}
	}

	void Page::ProcessLinks(OutputDev *pOut, Catalog *pCatalog)
	{
		Links *pLinks = GetLinks(pCatalog);
		if (pLinks)
		{
			for (int nIndex = 0; nIndex < pLinks->GetLinksCount(); ++nIndex)
			{
				pOut->ProcessLink(pLinks->GetLink(nIndex), pCatalog);
			}
			delete pLinks;
		}
	}

	void Page::GetDefaultCTM(double *pCTM, double dHorizDPI, double dVertDPI, int nRotate, bool bUseMediaBox, bool bUpSideDown)
	{
		nRotate += GetRotate();
		if (nRotate >= 360)
		{
			nRotate -= 360;
		}
		else if (nRotate < 0)
		{
			nRotate += 360;
		}
		GrState *pGState = new GrState(dHorizDPI, dVertDPI, bUseMediaBox ? GetMediaBox() : GetCropBox(), nRotate, bUpSideDown);
		if (pGState)
		{
			for (int nIndex = 0; nIndex < 6; ++nIndex)
			{
				pCTM[nIndex] = pGState->GetCTM()[nIndex];
			}
			delete pGState;
		}
	}
}