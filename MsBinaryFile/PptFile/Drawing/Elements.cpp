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

#include "Slide.h"
#include "../../Common/Vml/PPTShape/Ppt2PptxShapeConverter.h"

#ifndef DISABLE_FILE_DOWNLOADER
    #include "../../../Common/Network/FileTransporter/include/FileTransporter.h"
	#include "../../../DesktopEditor/raster/ImageFileFormatChecker.h"
#endif

std::wstring PPT::CImageElement::DownloadImage(const std::wstring& strFile)
{
#ifndef DISABLE_FILE_DOWNLOADER
    NSNetwork::NSFileTransport::CFileDownloader oDownloader(strFile, true);
	if ( oDownloader.DownloadSync() )
	{
		m_strImageFileName = oDownloader.GetFilePath();
		
		CImageFileFormatChecker checker;
		if (false == checker.isImageFile(m_strImageFileName))
		{
			m_strImageFileName.clear();
		}
	}
#endif
    return m_strImageFileName;
}
void PPT::CShapeElement::CalculateColor(CColor& oColor, CSlide* pSlide, CTheme* pTheme, CLayout* pLayout)
{
	LONG lOldIndex = oColor.m_lSchemeIndex;
	if (-1 == oColor.m_lSchemeIndex)
		return;

    std::vector<CColor>* pArray = NULL;

    if (pTheme) pArray = &pTheme->m_arColorScheme;

    if ((NULL != pLayout) && (!pLayout->m_bUseThemeColorScheme))
		pArray = &pLayout->m_arColorScheme;

	if (NULL != pSlide)
	{
		if (!pSlide->m_bUseLayoutColorScheme)
			pArray = &pSlide->m_arColorScheme;
	}

    if ((0 > oColor.m_lSchemeIndex) || ((pArray) && (oColor.m_lSchemeIndex >= (LONG)pArray->size())))
		return;

    if (pArray)
    {
        oColor = pArray->at(oColor.m_lSchemeIndex);
    }
	oColor.m_lSchemeIndex = lOldIndex;
}

void PPT::CShapeElement::SetupTextProperties(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout)
{
	PPT::CTextAttributesEx* pAttributes = &m_pShape->m_oText;
	int nCountColors = 0;
	if (NULL != pTheme)
		nCountColors = (int)pTheme->m_arColorScheme.size();

	size_t nCount = pAttributes->m_arParagraphs.size();
	for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
	{
		if (pAttributes->m_arParagraphs[nIndex].m_oPFRun.bulletColor.is_init())
		{
			int nColorIndex = (int)pAttributes->m_arParagraphs[nIndex].m_oPFRun.bulletColor->m_lSchemeIndex;
			if (0 <= nColorIndex && nColorIndex < nCountColors)
			{
				CalculateColor(pAttributes->m_arParagraphs[nIndex].m_oPFRun.bulletColor.get(), pSlide, pTheme, pLayout);
				pAttributes->m_arParagraphs[nIndex].m_oPFRun.bulletColor->m_lSchemeIndex = nColorIndex;
			}
		}

		size_t nCountCFs = pAttributes->m_arParagraphs[nIndex].m_arSpans.size();
		for (size_t i = 0; i < nCountCFs; ++i)
		{
			CTextCFRun* pRun = &pAttributes->m_arParagraphs[nIndex].m_arSpans[i].m_oRun;
			
			if (pRun->Color.is_init())
			{
				int nColorIndex = (int)pRun->Color->m_lSchemeIndex;
				if (0 <= nColorIndex && nColorIndex < nCountColors)
				{
					CalculateColor(pRun->Color.get(), pSlide, pTheme, pLayout);
					pRun->Color->m_lSchemeIndex = nColorIndex;
				}
			}
			if (NULL != pTheme)
			{
				if ((pRun->fontRef.is_init()) && (pRun->fontRef.get() < pTheme->m_arFonts.size()))
				{
					pRun->font.font = new CFontProperty(pTheme->m_arFonts[pRun->fontRef.get()]);

					//if (1 < pRun->fontRef.get())
					//	pRun->fontRef.reset();
				}
				if ((pRun->ansiFontRef.is_init()) && (pRun->ansiFontRef.get() < pTheme->m_arFonts.size()))
				{
					pRun->font.ansi = new CFontProperty(pTheme->m_arFonts[pRun->ansiFontRef.get()]);
				}
				if ((pRun->eaFontRef.is_init()) && (pRun->eaFontRef.get() < pTheme->m_arFonts.size()))
				{
					pRun->font.ea = new CFontProperty(pTheme->m_arFonts[pRun->eaFontRef.get()]);
				}
				if ((pRun->symbolFontRef.is_init()) && (pRun->symbolFontRef.get() < pTheme->m_arFonts.size()))
				{
					pRun->font.sym = new CFontProperty(pTheme->m_arFonts[pRun->symbolFontRef.get()]);
				}
			}
		}
	}
}

bool PPT::CShapeElement::SetUpTextPlaceholder(std::wstring newText)
{
	bool result = false;
	PPT::CTextAttributesEx* pText = &m_pShape->m_oText;

	for (size_t p = 0 ; p < pText->m_arParagraphs.size(); p++) //тут по всем -> 1-(33).ppt
	{
		if (pText->m_arParagraphs[p].m_arSpans.size() >0)//??? по всем?
		{
			int pos = (int)pText->m_arParagraphs[p].m_arSpans[0].m_strText.find(L"*");
			
			if (pos >= 0)
			{
				CSpan first = pText->m_arParagraphs[p].m_arSpans[0];
				CSpan last = pText->m_arParagraphs[p].m_arSpans[0];
				
				first.m_strText	= pText->m_arParagraphs[p].m_arSpans[0].m_strText.substr(0, pos);
				last.m_strText	= pText->m_arParagraphs[p].m_arSpans[0].m_strText.substr(pos + 1);

				pText->m_arParagraphs[p].m_arSpans[0].m_strText = newText;
				pText->m_arParagraphs[p].m_arSpans[0].m_bField	= true;

				if (last.m_strText.empty() == false)
					pText->m_arParagraphs[p].m_arSpans.insert(pText->m_arParagraphs[p].m_arSpans.begin() + 1, last);
				if (first.m_strText.empty() == false)
					pText->m_arParagraphs[p].m_arSpans.insert(pText->m_arParagraphs[p].m_arSpans.begin(), first);

				result = true;
			}
		}
	}
	return result;
}
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
#pragma once

#include "Theme.h"
#include "Shapes/Shape.h"
#include "../../Common/Vml/PPTShape/Ppt2PptxShapeConverter.h"

namespace PPT
{

	CImageElement::CImageElement() : CElement()
	{
		m_etType = etPicture;

		m_nAlpha = 0xFF;

		m_bCropEnabled = false;

		m_lcropFromRight = 0;
		m_lcropFromLeft = 0;
		m_lcropFromTop = 0;
		m_lcropFromBottom = 0;

		m_lpictureContrast = 0x10000;
		m_lpictureBrightness = 0;

		m_bStretch = true;
		m_bTile = false;
		m_bOLE = false;
		m_bImagePresent = false;

	}
	CImageElement::~CImageElement()
	{
	}
	CElementPtr CImageElement::CreateDublicate()
	{
		CImageElement* pImageElement = new CImageElement();

		CElementPtr	pElement = CElementPtr(pImageElement);

		SetProperiesToDublicate(pElement);

		pImageElement->m_strImageFileName = m_strImageFileName;
		pImageElement->m_nAlpha = m_nAlpha;

		pImageElement->m_bCropEnabled = m_bCropEnabled;

		pImageElement->m_lcropFromRight = m_lcropFromRight;
		pImageElement->m_lcropFromLeft = m_lcropFromLeft;
		pImageElement->m_lcropFromTop = m_lcropFromTop;
		pImageElement->m_lcropFromBottom = m_lcropFromBottom;

		pImageElement->m_bStretch = m_bStretch;
		pImageElement->m_bTile = m_bTile;
		pImageElement->m_bImagePresent = m_bImagePresent;
		pImageElement->m_bOLE = m_bOLE;

		return pElement;
	}
	std::wstring CImageElement::ConvertPPTShapeToPPTX(bool bIsNamespace)
	{
		NSGuidesVML::CFormParam pParamCoef;
		pParamCoef.m_eType = ptValue;
		pParamCoef.m_lParam = 65536;
		pParamCoef.m_lCoef = 65536;

		return ConvertPPTtoPPTX(/*pPPTShape,*/ pParamCoef, bIsNamespace);
	}


	std::wstring CImageElement::ConvertPPTtoPPTX(/*CPPTShape* pPPTShape, */const NSGuidesVML::CFormParam& pCoef, bool bIsNamespace)
	{
		if (bIsNamespace)
		{
			return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"rect\"><a:avLst/></a:prstGeom>");
		}
		return _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");

		std::wstring strXmlPPTX = bIsNamespace ? _T("<a:custGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">") : _T("<a:custGeom>");

		strXmlPPTX += _T("</a:custGeom>");
		return strXmlPPTX;
	}

	CShapeElement::CShapeElement(NSBaseShape::ClassType ClassType, int eType) : CElement()
	{
		m_lShapeType = eType;
		m_ClassType = ClassType;
		m_etType = etShape;

		m_pShape = CShapePtr(new CShape(ClassType, eType));

		m_pShape->m_rcBounds = m_bChildAnchorEnabled ? m_rcChildAnchor : m_rcAnchor;

		m_pShape->m_dStartTime = m_dStartTime;
		m_pShape->m_dEndTime = m_dEndTime;

		m_bShapePreset = false;

	}	
	CShapeElement::CShapeElement::CShapeElement() : CElement()
	{
		m_lShapeType = 0x1000;
		m_etType = etShape;
		m_bShapePreset = false;

		m_pShape = CShapePtr(new CShape(NSBaseShape::unknown, 0x1000));
	}

	CShapeElement::CShapeElement(const std::wstring& str) : CElement()
	{
		m_lShapeType = 0x1000;
		m_bShapePreset = false;

		m_pShape = CShapePtr(new CShape(NSBaseShape::unknown, 0x1000));
		m_pShape->LoadFromXML(str);

		m_ClassType = m_pShape->getBaseShape()->GetClassType();
	}
			//virtual void NormalizeCoordsByMetric()
			//{
			//	CElement::NormalizeCoordsByMetric();
	
			//	double dScaleX				= (double)m_oMetric.m_lUnitsHor / m_oMetric.m_lMillimetresHor;
			//	double dScaleY				= (double)m_oMetric.m_lUnitsVer	/ m_oMetric.m_lMillimetresVer;
	
			//	m_pShape->m_oText.m_oBounds.left		= (int)(dScaleX * m_pShape->m_oText.m_oBounds.left);
			//	m_pShape->m_oText.m_oBounds.right	= (int)(dScaleX * m_pShape->m_oText.m_oBounds.right);
			//	m_pShape->m_oText.m_oBounds.top		= (int)(dScaleY * m_pShape->m_oText.m_oBounds.top);
			//	m_pShape->m_oText.m_oBounds.bottom	= (int)(dScaleY * m_pShape->m_oText.m_oBounds.bottom);
			//}
	CShapeElement::~CShapeElement()
	{
	}

	CElementPtr CShapeElement::CreateDublicate()
	{
		CShapeElement* pShapeElement = new CShapeElement(m_ClassType, m_lShapeType);

		CElementPtr	pElement = CElementPtr(pShapeElement);

		SetProperiesToDublicate(pElement);

		pShapeElement->m_lShapeType = m_lShapeType;
		pShapeElement->m_bShapePreset = m_bShapePreset;

		m_pShape->SetToDublicate(pShapeElement->m_pShape.get());

		return pElement;
	}

	void CShapeElement::SetupProperties(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout)
	{
		m_pShape->m_oText.m_lPlaceholderType = m_lPlaceholderType;
		m_pShape->m_oText.m_lPlaceholderID = m_lPlaceholderID;

		m_pShape->getBaseShape()->ReCalculate();

		SetupTextProperties(pSlide, pTheme, pLayout);

		CalculateColor(m_oPen.Color, pSlide, pTheme, pLayout);
		CalculateColor(m_oBrush.Color1, pSlide, pTheme, pLayout);
		CalculateColor(m_oBrush.Color2, pSlide, pTheme, pLayout);
	}
	std::wstring CShapeElement::ConvertPPTShapeToPPTX(bool bIsNamespace)
	{
		CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(m_pShape->getBaseShape().get());
		if (NULL == pPPTShape)
		{
			// такого быть не может
			return _T("");
		}

		NSGuidesVML::CFormParam pParamCoef;
		pParamCoef.m_eType = ptValue;
		pParamCoef.m_lParam = 65536;
		pParamCoef.m_lCoef = 65536;

		return ConvertPPTtoPPTX(pPPTShape, pParamCoef, bIsNamespace);
	}

	std::wstring CShapeElement::ConvertPPTtoPPTX(CPPTShape* pPPTShape, const NSGuidesVML::CFormParam& pCoef, bool bIsNamespace)
	{
		if (pPPTShape->m_eType == PPTShapes::sptCRect || pPPTShape->m_eType == PPTShapes::sptCFrame || pPPTShape->m_eType == PPTShapes::sptCTextBox)
		{
			if (bIsNamespace)
			{
				return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"rect\"><a:avLst/></a:prstGeom>");
			}
			return _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");
		}
		else if (pPPTShape->m_eType == PPTShapes::sptCLine)
		{
			if (bIsNamespace)
			{
				return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"line\"><a:avLst/></a:prstGeom>");
			}
			return _T("<a:prstGeom prst=\"line\"><a:avLst/></a:prstGeom>");
		}
		else if (pPPTShape->m_eType == PPTShapes::sptCEllipse)
		{
			if (bIsNamespace)
			{
				return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"ellipse\"><a:avLst/></a:prstGeom>");
			}
			return _T("<a:prstGeom prst=\"ellipse\"><a:avLst/></a:prstGeom>");
		}
		std::wstring strXmlPPTX = bIsNamespace ? _T("<a:custGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">") : _T("<a:custGeom>");

		CFormulaConverter pFormulaConverter;

		//coeff
		pFormulaConverter.ConvertCoef(pCoef);

		//guids----------------------------------------
		int nGuidCount = (int)pPPTShape->m_oManager.m_arFormulas.size();
		if (0 != nGuidCount)
		{
			pFormulaConverter.ConvertFormula(pPPTShape->m_oManager.m_arFormulas);
		}

		//path------------------------------------------
		int nPathCount = (int)pPPTShape->m_strPath.length();
		if (0 != nPathCount && pPPTShape->m_eType != 1)
		{
			pFormulaConverter.ConvertPath(pPPTShape->m_strPath, pPPTShape->m_oPath);

			//string rect
			int nRectCount = (int)pPPTShape->m_arStringTextRects.size();
			if (0 != nRectCount && !pPPTShape->m_arStringTextRects[0].empty())
			{
				if (pPPTShape->m_eType == PPTShapes::sptCNotchedCircularArrow)
				{
					pFormulaConverter.SetTextRectDefault();
				}
				else
				{
					pFormulaConverter.ConvertTextRect(pPPTShape->m_arStringTextRects[0]);
				}
			}

			int nHandlesCount = (int)pPPTShape->m_arHandles.size();
			int nAdjCount = (int)pPPTShape->m_arAdjustments.size();

			//handles
			if (0 != nHandlesCount || 0 != nAdjCount)
			{
				pFormulaConverter.ConvertHandle(pPPTShape->m_arHandles, pPPTShape->m_arAdjustments, pPPTShape->m_eType);
			}

			//adj----------------------------
			if (pFormulaConverter.m_oAdjRes.GetSize() == 0)
				strXmlPPTX += _T("<a:avLst/>");
			else
				strXmlPPTX += _T("<a:avLst>") + pFormulaConverter.m_oAdjRes.GetXmlString() + _T("</a:avLst>");

			//guids--------------------------
			if (pFormulaConverter.m_oGuidsRes.GetSize() == 0)
				strXmlPPTX += _T("<a:gdLst>") + pFormulaConverter.m_oCoef.GetXmlString() + _T("</a:gdLst>");
			else
				strXmlPPTX += _T("<a:gdLst>") + pFormulaConverter.m_oCoef.GetXmlString() + pFormulaConverter.m_oGuidsRes.GetXmlString() + _T("</a:gdLst>");

			//handles---------------------------
			if (pFormulaConverter.m_oHandleRes.GetSize() == 0)
				strXmlPPTX += _T("<a:ahLst/>");
			else
				strXmlPPTX += _T("<a:ahLst>") + pFormulaConverter.m_oHandleRes.GetXmlString() + _T("</a:ahLst>");

			//connectors-------------------------
			strXmlPPTX += _T("<a:cxnLst/>");

			//textRect---------------------------
			if (pFormulaConverter.m_oTextRect.GetSize() != 0)
				strXmlPPTX += pFormulaConverter.m_oTextRect.GetXmlString();

			//path------------------------------
			strXmlPPTX += _T("<a:pathLst>");
			strXmlPPTX += pFormulaConverter.m_oPathRes.GetXmlString();
			strXmlPPTX += _T("</a:pathLst>");
		}
		else
		{
			return _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");
		}

		strXmlPPTX += _T("</a:custGeom>");
		return strXmlPPTX;
	}

	CAudioElement::CAudioElement() : CImageElement()
	{
		m_etType = etAudio;

		m_nAmplify = 100;
		m_bWithVideo = false;

		m_dAudioDuration = 0.0;
		m_bLoop = false;

		m_dClipStartTime = 0.0;
		m_dClipEndTime = -1.0;
	}

	CAudioElement::~CAudioElement()
	{
	}

	CElementPtr CAudioElement::CreateDublicate()
	{
		CAudioElement* pAudioElement = new CAudioElement();
		CElementPtr	pElement = CElementPtr(pAudioElement);

		SetProperiesToDublicate(pElement);

		pAudioElement->m_strAudioFileName = m_strAudioFileName;
		pAudioElement->m_nAmplify = m_nAmplify;
		pAudioElement->m_bWithVideo = m_bWithVideo;

		pAudioElement->m_dAudioDuration = m_dAudioDuration;
		pAudioElement->m_bLoop = m_bLoop;

		pAudioElement->m_dClipStartTime = m_dClipStartTime;
		pAudioElement->m_dClipEndTime = m_dClipEndTime;

		return pElement;
	}

	CVideoElement::CVideoElement() : CImageElement()
	{
		m_etType = etVideo;

		m_dVideoDuration = 0.0;
		m_nAlpha = 0xFF;

		m_dClipStartTime = -1.0;
		m_dClipEndTime = -1.0;
		m_bLoop = false;
	}

	CVideoElement::~CVideoElement()
	{
	}

	CElementPtr CVideoElement::CreateDublicate()
	{
		CVideoElement* pVideoElement = new CVideoElement();

		CElementPtr	pElement = CElementPtr(pVideoElement);

		SetProperiesToDublicate(pElement);

		pVideoElement->m_strVideoFileName = m_strVideoFileName;
		pVideoElement->m_nAlpha = m_nAlpha;

		pVideoElement->m_dVideoDuration = m_dVideoDuration;

		pVideoElement->m_dClipStartTime = m_dClipStartTime;
		pVideoElement->m_dClipEndTime = m_dClipEndTime;
		pVideoElement->m_bLoop = m_bLoop;

		return pElement;
	}

	CGroupElement::CGroupElement() : CElement()
	{
		m_etType = etGroup;
	}
	CGroupElement::~CGroupElement()
	{
	}
	CElementPtr CGroupElement::CreateDublicate()
	{
		CGroupElement* pGroupElement = new CGroupElement();

		CElementPtr	pElement = CElementPtr(pGroupElement);

		SetProperiesToDublicate(pElement);

		return pElement;
	}
	std::wstring CGroupElement::ConvertPPTShapeToPPTX(bool bIsNamespace)
	{

		return L"";
	}

	CTableElement::CTableElement() : CElement()
	{
		m_etType = etTable;
	}
	CTableElement::~CTableElement()
	{
	}
	CElementPtr CTableElement::CreateDublicate()
	{
		CTableElement* pTableElement = new CTableElement();

		CElementPtr	pElement = CElementPtr(pTableElement);

		SetProperiesToDublicate(pElement);

		return pElement;
	}
	int CTableElement::GetWidth()const
	{
		double multip = m_bAnchorEnabled ? 1587.5 : 1;
		return round(m_rcAnchor.GetWidth()  * multip);
	}
	int CTableElement::GetHeight()const
	{
		double multip = m_bAnchorEnabled ? 1587.5 : 1;
		return round(m_rcAnchor.GetHeight()  * multip);
	}
	std::wstring CTableElement::ConvertPPTShapeToPPTX(bool bIsNamespace)
	{

		return L"";
	}
}
