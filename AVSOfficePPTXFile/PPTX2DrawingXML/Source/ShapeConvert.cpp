#include "stdafx.h"
#include "./../Converter.h"
#include "Logic/PrstGeom.h"
#include "Logic/CustGeom.h"
#include "Logic/Fills/SolidFill.h"
#include "Logic/Fills/NoFill.h"
#include "Slide.h"
#include "SlideLayout.h"
#include "SlideMaster.h"

#import "..\..\..\..\Redist\AVSVideoFile3.dll" raw_interfaces_only

namespace PPTX2DrawingXML
{
	void Converter::ShapeConvert(const PPTX::Logic::Shape& pShape, CElementsContainer& pEContainer)
	{
		CShapeElement* lpShapeElement;
		PPTX::Logic::Shape localShape;
		pShape.GetShapeFullDescription(localShape);
		if(localShape.spPr->Geometry.is<PPTX::Logic::PrstGeom>())
		{
			const PPTX::Logic::PrstGeom lpGeom = localShape.spPr->Geometry.as<PPTX::Logic::PrstGeom>();

			OOXMLShapes::ShapeType _lspt = GetShapeTypeFromStr(lpGeom.prst.get());
			if(_lspt == OOXMLShapes::sptNil) return;
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)_lspt);
			std::string strAdjustValues = lpGeom.GetODString();
			lpShapeElement->m_oShape.m_pShape->LoadAdjustValuesList(CString(strAdjustValues.c_str()));
		}
		else if(localShape.spPr->Geometry.is<PPTX::Logic::CustGeom>())
		{
			const PPTX::Logic::CustGeom lpGeom = localShape.spPr->Geometry.as<PPTX::Logic::CustGeom>();
			std::string strShape = lpGeom.GetODString();
			lpShapeElement = new CShapeElement(CString(strShape.c_str()));
		}
		else
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);
		if(localShape.spPr->xfrm.is_init())
		{
			lpShapeElement->m_dRotate = localShape.spPr->xfrm->rot.get_value_or(0)/60000.0;
			lpShapeElement->m_bFlipH = localShape.spPr->xfrm->flipH.get_value_or(false);
			lpShapeElement->m_bFlipV = localShape.spPr->xfrm->flipV.get_value_or(false);
		}


		lpShapeElement->m_lID = pShape.nvSpPr->cNvPr->id.get();
		int id = lpShapeElement->m_lID;
		lpShapeElement->m_oMetric.m_lUnitsHor = pEContainer.m_lOriginalWidth;
		lpShapeElement->m_oMetric.m_lUnitsVer = pEContainer.m_lOriginalHeight;
		lpShapeElement->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth/36000;
		lpShapeElement->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight/36000;

		lpShapeElement->m_dStartTime = 0;
		lpShapeElement->m_dEndTime = pEContainer.m_dDuration;

		//используется только Brush !!!
		lpShapeElement->m_oShape.m_oBrush.m_Alpha1 = 0;
		lpShapeElement->m_oShape.m_oBrush.m_Alpha2 = 0;
		PPTX::Logic::UniFill fill;
		DWORD BGRA = localShape.GetFill(fill);
		FillToBrush(fill, BGRA, lpShapeElement->m_oShape.m_oBrush);

		lpShapeElement->m_oShape.m_oPen.m_nAlpha = 0;
		PPTX::Logic::Ln line;
		BGRA = localShape.GetLine(line);
		LnToPen(line, BGRA, lpShapeElement->m_oShape.m_oPen);

		localShape.GetRect(lpShapeElement->m_rcBoundsOriginal);

		//CStringW ShapeText = GetCStringFromUTF8(CString(pShape.GetText().c_str()));
		CStringW ShapeText = CStringW(pShape.GetWText().c_str());
		if((ShapeText != L"") && (ShapeText != L"\n"))
		{
			PPTX::Logic::ShapeProperties ShapeProps;
			localShape.FillShapeProperties(ShapeProps);

			lpShapeElement->m_oShape.m_dTextMarginX = localShape.txBody->bodyPr->lIns.get_value_or(91440) / 36000.0;
			lpShapeElement->m_oShape.m_dTextMarginY = localShape.txBody->bodyPr->tIns.get_value_or(45720) / 36000.0;
			lpShapeElement->m_oShape.m_dTextMarginRight = localShape.txBody->bodyPr->rIns.get_value_or(91440) / 36000.0;
			lpShapeElement->m_oShape.m_dTextMarginBottom = localShape.txBody->bodyPr->bIns.get_value_or(45720) / 36000.0;

			//lpShapeElement->m_oShape.m_oText.m_sText = ShapeText;
			double FontScale = localShape.txBody->bodyPr->Fit.fontScale.get_value_or(100000)/100000.0;
			double LnSpcReduction = localShape.txBody->bodyPr->Fit.lnSpcReduction.get_value_or(0)/100000.0;
			TextParse(pShape.txBody.get(), lpShapeElement->m_oShape.m_oText, ShapeProps, FontScale, LnSpcReduction);
		}

		lpShapeElement->m_oShape.SetProperties(NULL, &pEContainer);
		pEContainer.m_arElements.Add(lpShapeElement);
	}

	void Converter::PicConvert(const PPTX::Logic::Pic& pPic, CElementsContainer& pEContainer)
	{
		CShapeElement* lpShapeElement;
		if(pPic.spPr->Geometry.is<PPTX::Logic::PrstGeom>())
		{
			const PPTX::Logic::PrstGeom lpGeom = pPic.spPr->Geometry.as<PPTX::Logic::PrstGeom>();

			OOXMLShapes::ShapeType _lspt = GetShapeTypeFromStr(lpGeom.prst.get());
			if(_lspt == OOXMLShapes::sptNil) return;
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)_lspt);
			std::string strAdjustValues = lpGeom.GetODString();
			lpShapeElement->m_oShape.m_pShape->LoadAdjustValuesList(CString(strAdjustValues.c_str()));
		}
		else if(pPic.spPr->Geometry.is<PPTX::Logic::CustGeom>())
		{
			const PPTX::Logic::CustGeom lpGeom = pPic.spPr->Geometry.as<PPTX::Logic::CustGeom>();
			std::string strShape = lpGeom.GetODString();
			lpShapeElement = new CShapeElement(CString(strShape.c_str()));
		}
		else
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);

		lpShapeElement->m_lID = pPic.nvPicPr->cNvPr->id.get();
		if(pPic.spPr->xfrm.is_init())
		{
			lpShapeElement->m_dRotate = pPic.spPr->xfrm->rot.get_value_or(0)/60000.0;
			lpShapeElement->m_bFlipH = pPic.spPr->xfrm->flipH.get_value_or(false);
			lpShapeElement->m_bFlipV = pPic.spPr->xfrm->flipV.get_value_or(false);
		}

		lpShapeElement->m_oMetric.m_lUnitsHor = pEContainer.m_lOriginalWidth;
		lpShapeElement->m_oMetric.m_lUnitsVer = pEContainer.m_lOriginalHeight;
		lpShapeElement->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth/36000;
		lpShapeElement->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight/36000;

		lpShapeElement->m_dStartTime = 0;
		lpShapeElement->m_dEndTime = pEContainer.m_dDuration;

		lpShapeElement->m_oShape.m_oBrush.m_nBrushType = (int)BrushTypeTexture;
		lpShapeElement->m_oShape.m_oBrush.m_sTexturePath = CStringW(pPic.blipFill->blip->GetFullPicName().c_str());
		lpShapeElement->m_oShape.m_oBrush.m_nTextureMode = pPic.blipFill->tile.is_init()?(int)BrushTextureModeTile:(int)BrushTextureModeStretch;
		//if(Fill.as<PPTX::Logic::BlipFill>().srcRect.is_init())
		//{
		//	Brush.m_rcBounds.top = 
		//	Brush.m_rcBounds.left = 
		//	Brush.m_rcBounds.rigth = 
		//	Brush.m_rcBounds.bottom = 
		//}

		lpShapeElement->m_oShape.m_oPen.m_nAlpha = 0;
		PPTX::Logic::Ln line;
		DWORD BGRA = pPic.GetLine(line);
		LnToPen(line, BGRA, lpShapeElement->m_oShape.m_oPen);

		pPic.GetRect(lpShapeElement->m_rcBoundsOriginal);

		PPTX::Logic::UniFill fill;
		BGRA = pPic.GetFill(fill);
		if(fill.is_init())
		{
			CShapeElement* lpShapeElement2 = (CShapeElement*)lpShapeElement->CreateDublicate();
			lpShapeElement2->m_oShape.m_oBrush.m_Alpha1 = 0;
			lpShapeElement2->m_oShape.m_oBrush.m_Alpha2 = 0;
			FillToBrush(fill, BGRA, lpShapeElement2->m_oShape.m_oBrush);
			lpShapeElement2->m_oShape.SetProperties(NULL, &pEContainer);
			pEContainer.m_arElements.Add(lpShapeElement2);
		}

		lpShapeElement->m_oShape.SetProperties(NULL, &pEContainer);
		pEContainer.m_arElements.Add(lpShapeElement);

		if(pPic.GetVideoLink() != L"")
			VideoElementConvert(pPic, pEContainer);
		else if(pPic.GetAudioLink() != L"")
			AudioElementConvert(pPic, pEContainer);
	}

	void Converter::VideoElementConvert(const PPTX::Logic::Pic& pPic, CElementsContainer& pEContainer)
	{
		CVideoElement* lpVideoElement = new CVideoElement;
		CAudioElement* lpAudioElement = new CAudioElement;
		lpVideoElement->m_strFileName = CStringW(pPic.GetVideoLink().c_str());

		int lIndex = lpVideoElement->m_strFileName.Find(L"file:///");
		if (0 == lIndex)
		{
			lpVideoElement->m_strFileName = lpVideoElement->m_strFileName.Mid(8);
			//lpVideoElement->m_strFileName.Replace('/', '\\');
			//lpVideoElement->m_strFileName.Replace(L"%20", L" ");
		}
		//lpVideoElement->m_strFileName.Replace(L"&", L"&amp;");
		//lpVideoElement->m_strFileName.Replace('/', '\\');
		//lpVideoElement->m_strFileName.Replace(L"%20", L" ");
		//lpVideoElement->m_strFileName.Replace(L">", L"&gt;");
		//lpVideoElement->m_strFileName.Replace(L"<", L"&lt;");
		//lpVideoElement->m_strFileName.Replace(L"'", L"&apos;");
		//lpVideoElement->m_strFileName.Replace(L"\"", L"&quot;");


		BSTR bstrFile = lpVideoElement->m_strFileName.AllocSysString();

		AVSVideoFile3::IVideoFile3* pVideoFile = NULL;
		CoCreateInstance(__uuidof(AVSVideoFile3::CVideoFile3), NULL, CLSCTX_ALL, __uuidof(AVSVideoFile3::IVideoFile3), (void**)(&pVideoFile));

		double dDuration = 0;
		if (NULL != pVideoFile)
		{
			pVideoFile->OpenFile(bstrFile);
			pVideoFile->get_videoDuration(&dDuration);
			pVideoFile->CloseFile();
		}
		SysFreeString(bstrFile);

		lpVideoElement->m_lID = pPic.nvPicPr->cNvPr->id.get();
		lpVideoElement->m_oMetric.m_lUnitsHor = pEContainer.m_lOriginalWidth;
		lpVideoElement->m_oMetric.m_lUnitsVer = pEContainer.m_lOriginalHeight;
		lpVideoElement->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth/36000;
		lpVideoElement->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight/36000;

		lpVideoElement->m_dStartTime = pEContainer.m_dStartTime;
		if(pEContainer.m_bDefaultDuration)
		{
			lpVideoElement->m_dEndTime = lpVideoElement->m_dStartTime + dDuration;
			if(dDuration > pEContainer.m_dEndTime - pEContainer.m_dStartTime)
			{
				pEContainer.m_dEndTime = pEContainer.m_dStartTime + dDuration;
				pEContainer.m_dDuration = dDuration;
			}
		}
		else
		{
			lpVideoElement->m_dEndTime = min(pEContainer.m_dEndTime, lpVideoElement->m_dStartTime + dDuration);
		}

		pPic.GetRect(lpVideoElement->m_rcBoundsOriginal);

		lpAudioElement->m_dStartTime = lpVideoElement->m_dStartTime;
		lpAudioElement->m_dEndTime = lpVideoElement->m_dEndTime;
		lpAudioElement->m_strFileName = lpVideoElement->m_strFileName;
		lpAudioElement->m_bWithVideo = true;

		pEContainer.m_arElements.Add(lpVideoElement);
		pEContainer.m_arElements.Add(lpAudioElement);
	}

	void Converter::AudioElementConvert(const PPTX::Logic::Pic& pPic, CElementsContainer& pEContainer)
	{
		CAudioElement* lpAudioElement = new CAudioElement;
		lpAudioElement->m_strFileName = CStringW(pPic.GetAudioLink().c_str());

		int lIndex = lpAudioElement->m_strFileName.Find(L"file:///");
		if (0 == lIndex)
		{
			lpAudioElement->m_strFileName = lpAudioElement->m_strFileName.Mid(8);
			//lpAudioElement->m_strFileName.Replace('/', '\\');
			//lpAudioElement->m_strFileName.Replace(L"%20", L" ");
		}
		//lpAudioElement->m_strFileName.Replace(L"&", L"&amp;");
		//lpAudioElement->m_strFileName.Replace('/', '\\');
		//lpAudioElement->m_strFileName.Replace(L"%20", L" ");
		//lpAudioElement->m_strFileName.Replace(L">", L"&gt;");
		//lpAudioElement->m_strFileName.Replace(L"<", L"&lt;");
		//lpAudioElement->m_strFileName.Replace(L"'", L"&apos;");
		//lpAudioElement->m_strFileName.Replace(L"\"", L"&quot;");


		BSTR bstrFile = lpAudioElement->m_strFileName.AllocSysString();
		AVSVideoFile3::IVideoFile3* pVideoFile = NULL;
		CoCreateInstance(__uuidof(AVSVideoFile3::CVideoFile3), NULL, CLSCTX_ALL, __uuidof(AVSVideoFile3::IVideoFile3), (void**)(&pVideoFile));

		double dDuration = 0;
		if (NULL != pVideoFile)
		{
			pVideoFile->OpenFile(bstrFile);
			pVideoFile->get_audioDuration(&dDuration);
			pVideoFile->CloseFile();
		}
		SysFreeString(bstrFile);

		lpAudioElement->m_dStartTime = pEContainer.m_dStartTime;
		lpAudioElement->m_dEndTime = pEContainer.m_dStartTime + dDuration;

		pEContainer.m_arElements.Add(lpAudioElement);
	}

	void Converter::ConnectorConvert(const PPTX::Logic::CxnSp& pCxn, CElementsContainer& pEContainer)
	{
		CShapeElement* lpShapeElement;
		if(pCxn.spPr->Geometry.is<PPTX::Logic::PrstGeom>())
		{
			const PPTX::Logic::PrstGeom lpGeom = pCxn.spPr->Geometry.as<PPTX::Logic::PrstGeom>();

			OOXMLShapes::ShapeType _lspt = GetShapeTypeFromStr(lpGeom.prst.get());
			if(_lspt == OOXMLShapes::sptNil) return;
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)_lspt);
			std::string strAdjustValues = lpGeom.GetODString();
			lpShapeElement->m_oShape.m_pShape->LoadAdjustValuesList(CString(strAdjustValues.c_str()));
		}
		else if(pCxn.spPr->Geometry.is<PPTX::Logic::CustGeom>())
		{
			const PPTX::Logic::CustGeom lpGeom = pCxn.spPr->Geometry.as<PPTX::Logic::CustGeom>();
			std::string strShape = lpGeom.GetODString();
			lpShapeElement = new CShapeElement(CString(strShape.c_str()));
		}
		else
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCStraightConnector1);

		lpShapeElement->m_lID = pCxn.nvCxnSpPr->cNvPr->id.get();
		if(pCxn.spPr->xfrm.is_init())
		{
			lpShapeElement->m_dRotate = pCxn.spPr->xfrm->rot.get_value_or(0)/60000.0;
			lpShapeElement->m_bFlipH = pCxn.spPr->xfrm->flipH.get_value_or(false);
			lpShapeElement->m_bFlipV = pCxn.spPr->xfrm->flipV.get_value_or(false);
		}
		lpShapeElement->m_oMetric.m_lUnitsHor = pEContainer.m_lOriginalWidth;
		lpShapeElement->m_oMetric.m_lUnitsVer = pEContainer.m_lOriginalHeight;
		lpShapeElement->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth/36000;
		lpShapeElement->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight/36000;

		lpShapeElement->m_dStartTime = 0;
		lpShapeElement->m_dEndTime = pEContainer.m_dDuration;

		//используется только Brush !!!
		lpShapeElement->m_oShape.m_oBrush.m_Alpha1 = 0;
		lpShapeElement->m_oShape.m_oBrush.m_Alpha2 = 0;
		PPTX::Logic::UniFill fill;
		DWORD BGRA = pCxn.GetFill(fill);
		FillToBrush(fill, BGRA, lpShapeElement->m_oShape.m_oBrush);

		lpShapeElement->m_oShape.m_oPen.m_nAlpha = 0;
		PPTX::Logic::Ln line;
		BGRA = pCxn.GetLine(line);
		LnToPen(line, BGRA, lpShapeElement->m_oShape.m_oPen);

		pCxn.GetRect(lpShapeElement->m_rcBoundsOriginal);

		lpShapeElement->m_oShape.SetProperties(NULL, &pEContainer);
		pEContainer.m_arElements.Add(lpShapeElement);
	}

	void Converter::OleConvert(const PPTX::Logic::GraphicFrame& pOle, CElementsContainer& pEContainer)
	{
		if(pOle.Table.is_init())
		{
			TableConvert(pOle, pEContainer);
		}
		else
		{
			CShapeElement* lpShapeElement;
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);

			pOle.GetRect(lpShapeElement->m_rcBoundsOriginal);

			lpShapeElement->m_oMetric.m_lUnitsHor = pEContainer.m_lOriginalWidth;
			lpShapeElement->m_oMetric.m_lUnitsVer = pEContainer.m_lOriginalHeight;
			lpShapeElement->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth/36000;
			lpShapeElement->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight/36000;

			lpShapeElement->m_dStartTime = 0;
			lpShapeElement->m_dEndTime = pEContainer.m_dDuration;

			if(!pOle.spid.is_init())
			{
				lpShapeElement->m_oShape.m_oBrush.m_Alpha1 = 0;
				lpShapeElement->m_oShape.m_oBrush.m_Alpha2 = 0;

				lpShapeElement->m_oShape.m_oPen.m_nAlpha = 128;
				lpShapeElement->m_oShape.m_oPen.m_oColor = 0x80808080;

				lpShapeElement->m_oShape.m_oText.m_sText = _T("Ole-object");
				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_nTextAlignVertical = 1;
				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;

				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_oTextEdge.m_nVisible = 1;
				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_oTextEdge.m_nDistance = 1;
				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_oTextEdge.m_oColor = 0;
				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_oTextEdge.m_nAlpha = 128;

				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_oTextBrush.m_oColor1 = 0x80808080;
				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_oTextBrush.m_Alpha1 = 128;
			}
			else
			{
				lpShapeElement->m_oShape.m_oBrush.m_nBrushType = (int)BrushTypeTexture;
				lpShapeElement->m_oShape.m_oBrush.m_sTexturePath = CStringW(pOle.GetPathBySpid().string().c_str());
				lpShapeElement->m_oShape.m_oBrush.m_nTextureMode = (int)BrushTextureModeStretch;
				lpShapeElement->m_oShape.m_oBrush.m_nTextureAlpha = 255;

				lpShapeElement->m_oShape.m_oPen.m_nAlpha = 0;
			}

			lpShapeElement->m_oShape.SetProperties(NULL, &pEContainer);
			pEContainer.m_arElements.Add(lpShapeElement);
		}
	}
}