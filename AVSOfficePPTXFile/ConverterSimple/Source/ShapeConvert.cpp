#include "stdafx.h"
#include "./../Converter.h"
#include "../../PPTXFormat/Logic/PrstGeom.h"
#include "../../PPTXFormat/Logic/CustGeom.h"
#include "../../PPTXFormat/Logic/Fills/SolidFill.h"
#include "../../PPTXFormat/Logic/Fills/NoFill.h"
#include "../../PPTXFormat/Slide.h"
#include "../../PPTXFormat/SlideLayout.h"
#include "../../PPTXFormat/SlideMaster.h"

namespace PPTX2EditorSimple
{
	void Converter::ShapeConvert(const PPTX::Logic::Shape& pShape, NSPresentationEditor::CSlide& pEContainer)
	{
		NSPresentationEditor::CShapeElement* lpShapeElement = NULL;
		PPTX::Logic::Shape localShape;
		pShape.GetShapeFullDescription(localShape);
		if(localShape.spPr.Geometry.is<PPTX::Logic::PrstGeom>())
		{
			const PPTX::Logic::PrstGeom lpGeom = localShape.spPr.Geometry.as<PPTX::Logic::PrstGeom>();

			OOXMLShapes::ShapeType _lspt = GetShapeTypeFromStr(lpGeom.prst.get());
			if(_lspt == OOXMLShapes::sptNil) 
				return;
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)_lspt);
			CString strAdjustValues = lpGeom.GetODString();
			lpShapeElement->m_oShape.m_pShape->LoadAdjustValuesList(strAdjustValues);
		}
		else if(localShape.spPr.Geometry.is<PPTX::Logic::CustGeom>())
		{
			const PPTX::Logic::CustGeom lpGeom = localShape.spPr.Geometry.as<PPTX::Logic::CustGeom>();
			CString strShape = lpGeom.GetODString();
			lpShapeElement = new CShapeElement(strShape);
		}
		else
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);
		
		if(localShape.spPr.xfrm.is_init())
		{
			lpShapeElement->m_dRotate	= localShape.spPr.xfrm->rot.get_value_or(0)/60000.0;
			lpShapeElement->m_bFlipH	= localShape.spPr.xfrm->flipH.get_value_or(false);
			lpShapeElement->m_bFlipV	= localShape.spPr.xfrm->flipV.get_value_or(false);
		}

		lpShapeElement->m_lID = pShape.nvSpPr.cNvPr.id;
		int id = lpShapeElement->m_lID;
		lpShapeElement->m_oMetric.m_lUnitsHor		= pEContainer.m_lOriginalWidth;
		lpShapeElement->m_oMetric.m_lUnitsVer		= pEContainer.m_lOriginalHeight;
		lpShapeElement->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth/36000;
		lpShapeElement->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight/36000;

		lpShapeElement->m_dStartTime				= 0;
		lpShapeElement->m_dEndTime					= pEContainer.m_dDuration;

		//используется только Brush !!!
		lpShapeElement->m_oShape.m_oBrush.Alpha1 = 0;
		lpShapeElement->m_oShape.m_oBrush.Alpha2 = 0;
		PPTX::Logic::UniFill fill;
		DWORD BGRA = localShape.GetFill(fill);
		FillToBrush(fill, BGRA, lpShapeElement->m_oShape.m_oBrush);

		lpShapeElement->m_oShape.m_oPen.Alpha = 0;
		PPTX::Logic::Ln line;
		BGRA = localShape.GetLine(line);
		LnToPen(line, BGRA, lpShapeElement->m_oShape.m_oPen);

		RECT rect;
		localShape.GetRect(rect);
		lpShapeElement->m_rcBoundsOriginal.left		= (double)rect.left;
		lpShapeElement->m_rcBoundsOriginal.right	= (double)rect.right;
		lpShapeElement->m_rcBoundsOriginal.top		= (double)rect.top;
		lpShapeElement->m_rcBoundsOriginal.bottom	= (double)rect.bottom;

		//CStringW ShapeText = GetCStringFromUTF8(CString(pShape.GetText().c_str()));
		CString ShapeText = pShape.GetText();
		if((ShapeText != _T("")) && (ShapeText != _T("\n")))
		{
			PPTX::Logic::ShapeProperties ShapeProps;
			localShape.FillShapeProperties(ShapeProps);

			lpShapeElement->m_oShape.m_dTextMarginX			= localShape.txBody->bodyPr.lIns.get_value_or(91440) / 36000.0;
			lpShapeElement->m_oShape.m_dTextMarginY			= localShape.txBody->bodyPr.tIns.get_value_or(45720) / 36000.0;
			lpShapeElement->m_oShape.m_dTextMarginRight		= localShape.txBody->bodyPr.rIns.get_value_or(91440) / 36000.0;
			lpShapeElement->m_oShape.m_dTextMarginBottom	= localShape.txBody->bodyPr.bIns.get_value_or(45720) / 36000.0;

			//lpShapeElement->m_oShape.m_oText.m_sText = ShapeText;
			double FontScale = localShape.txBody->bodyPr.Fit.fontScale.get_value_or(100000)/100000.0;
			double LnSpcReduction = localShape.txBody->bodyPr.Fit.lnSpcReduction.get_value_or(0)/100000.0;
			TextParse(pShape.txBody.get(), lpShapeElement->m_oShape.m_oText, ShapeProps, FontScale, LnSpcReduction);
		}

		lpShapeElement->NormalizeCoords(c_dScaleElements, c_dScaleElements);
		lpShapeElement->m_oShape.m_rcBounds			= lpShapeElement->m_rcBounds;
		
		pEContainer.m_arElements.Add(lpShapeElement);
	}

	void Converter::PicConvert(const PPTX::Logic::Pic& pPic, NSPresentationEditor::CSlide& pEContainer)
	{
		NSPresentationEditor::CShapeElement* lpShapeElement;
		if(pPic.spPr.Geometry.is<PPTX::Logic::PrstGeom>())
		{
			const PPTX::Logic::PrstGeom lpGeom = pPic.spPr.Geometry.as<PPTX::Logic::PrstGeom>();

			OOXMLShapes::ShapeType _lspt = GetShapeTypeFromStr(lpGeom.prst.get());
			if (_lspt == OOXMLShapes::sptNil) 
				return;
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)_lspt);
			CString strAdjustValues = lpGeom.GetODString();
			lpShapeElement->m_oShape.m_pShape->LoadAdjustValuesList(strAdjustValues);
		}
		else if(pPic.spPr.Geometry.is<PPTX::Logic::CustGeom>())
		{
			const PPTX::Logic::CustGeom lpGeom = pPic.spPr.Geometry.as<PPTX::Logic::CustGeom>();
			CString strShape = lpGeom.GetODString();
			lpShapeElement = new CShapeElement(strShape);
		}
		else
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);

		lpShapeElement->m_lID = pPic.nvPicPr.cNvPr.id;
		if(pPic.spPr.xfrm.is_init())
		{
			lpShapeElement->m_dRotate	= pPic.spPr.xfrm->rot.get_value_or(0)/60000.0;
			lpShapeElement->m_bFlipH	= pPic.spPr.xfrm->flipH.get_value_or(false);
			lpShapeElement->m_bFlipV	= pPic.spPr.xfrm->flipV.get_value_or(false);
		}

		lpShapeElement->m_oMetric.m_lUnitsHor		= pEContainer.m_lOriginalWidth;
		lpShapeElement->m_oMetric.m_lUnitsVer		= pEContainer.m_lOriginalHeight;
		lpShapeElement->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth/36000;
		lpShapeElement->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight/36000;

		lpShapeElement->m_dStartTime	= 0;
		lpShapeElement->m_dEndTime		= pEContainer.m_dDuration;

		lpShapeElement->m_oShape.m_oBrush.Type = (int)NSPresentationEditor::c_BrushTypeTexture;
		lpShapeElement->m_oShape.m_oBrush.TexturePath = pPic.blipFill.blip->GetFullPicName();
		lpShapeElement->m_oShape.m_oBrush.TextureMode = pPic.blipFill.tile.is_init()?(int)NSPresentationEditor::c_BrushTextureModeTile:(int)NSPresentationEditor::c_BrushTextureModeStretch;
		//if(Fill.as<PPTX::Logic::BlipFill>().srcRect.is_init())
		//{
		//	Brush.m_rcBounds.top = 
		//	Brush.m_rcBounds.left = 
		//	Brush.m_rcBounds.rigth = 
		//	Brush.m_rcBounds.bottom = 
		//}

		lpShapeElement->m_oShape.m_oPen.Alpha = 0;
		PPTX::Logic::Ln line;
		DWORD BGRA = pPic.GetLine(line);
		LnToPen(line, BGRA, lpShapeElement->m_oShape.m_oPen);

		RECT rect;
		pPic.GetRect(rect);
		lpShapeElement->m_rcBoundsOriginal.left		= (double)rect.left;
		lpShapeElement->m_rcBoundsOriginal.right	= (double)rect.right;
		lpShapeElement->m_rcBoundsOriginal.top		= (double)rect.top;
		lpShapeElement->m_rcBoundsOriginal.bottom	= (double)rect.bottom;

		lpShapeElement->NormalizeCoords(c_dScaleElements, c_dScaleElements);

		lpShapeElement->m_oShape.m_rcBounds			= lpShapeElement->m_rcBounds;

		PPTX::Logic::UniFill fill;
		BGRA = pPic.GetFill(fill);
		if(fill.is_init())
		{
			CShapeElement* lpShapeElement2 = (CShapeElement*)lpShapeElement->CreateDublicate();
			lpShapeElement2->m_oShape.m_oBrush.Alpha1 = 0;
			lpShapeElement2->m_oShape.m_oBrush.Alpha2 = 0;
			FillToBrush(fill, BGRA, lpShapeElement2->m_oShape.m_oBrush);
			pEContainer.m_arElements.Add(lpShapeElement2);
		}

		pEContainer.m_arElements.Add(lpShapeElement);

		if (pPic.GetVideoLink() != _T(""))
			VideoElementConvert(pPic, pEContainer);
		else if (pPic.GetAudioLink() != _T(""))
			AudioElementConvert(pPic, pEContainer);
	}

	void Converter::VideoElementConvert(const PPTX::Logic::Pic& pPic, NSPresentationEditor::CSlide& pEContainer)
	{
		NSPresentationEditor::CVideoElement* lpVideoElement = new NSPresentationEditor::CVideoElement();
		NSPresentationEditor::CAudioElement* lpAudioElement = new NSPresentationEditor::CAudioElement();
		lpVideoElement->m_strFileName = pPic.GetVideoLink();

		int lIndex = lpVideoElement->m_strFileName.Find(_T("file:///"));
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

		VideoFile::IVideoFile3* pVideoFile = NULL;
		CoCreateInstance(__uuidof(VideoFile::CVideoFile3), NULL, CLSCTX_ALL, __uuidof(VideoFile::IVideoFile3), (void**)(&pVideoFile));

		double dDuration = 0;
		if (NULL != pVideoFile)
		{
			pVideoFile->OpenFile(bstrFile);
			pVideoFile->get_videoDuration(&dDuration);
			pVideoFile->CloseFile();
		}
		SysFreeString(bstrFile);
		RELEASEINTERFACE(pVideoFile);

		lpVideoElement->m_lID = pPic.nvPicPr.cNvPr.id;
		lpVideoElement->m_oMetric.m_lUnitsHor		= pEContainer.m_lOriginalWidth;
		lpVideoElement->m_oMetric.m_lUnitsVer		= pEContainer.m_lOriginalHeight;
		lpVideoElement->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth / 36000;
		lpVideoElement->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight/ 36000;

		lpVideoElement->m_dStartTime = pEContainer.m_dStartTime;
		if (FALSE)
		{
			lpVideoElement->m_dEndTime = lpVideoElement->m_dStartTime + dDuration;
			if(dDuration > pEContainer.m_dEndTime - pEContainer.m_dStartTime)
			{
				pEContainer.m_dEndTime  = pEContainer.m_dStartTime + dDuration;
				pEContainer.m_dDuration = dDuration;
			}
		}
		else
		{
			lpVideoElement->m_dEndTime = min(pEContainer.m_dEndTime, lpVideoElement->m_dStartTime + dDuration);
		}

		RECT rect;
		pPic.GetRect(rect);
		lpVideoElement->m_rcBoundsOriginal.left		= (double)rect.left;
		lpVideoElement->m_rcBoundsOriginal.right	= (double)rect.right;
		lpVideoElement->m_rcBoundsOriginal.top		= (double)rect.top;
		lpVideoElement->m_rcBoundsOriginal.bottom	= (double)rect.bottom;

		lpAudioElement->m_dStartTime	= lpVideoElement->m_dStartTime;
		lpAudioElement->m_dEndTime		= lpVideoElement->m_dEndTime;
		lpAudioElement->m_strFileName	= lpVideoElement->m_strFileName;
		lpAudioElement->m_bWithVideo	= true;

		lpVideoElement->NormalizeCoords(c_dScaleElements, c_dScaleElements);

		pEContainer.m_arElements.Add(lpVideoElement);
		pEContainer.m_arElements.Add(lpAudioElement);
	}

	void Converter::AudioElementConvert(const PPTX::Logic::Pic& pPic, NSPresentationEditor::CSlide& pEContainer)
	{
		NSPresentationEditor::CAudioElement* lpAudioElement = new NSPresentationEditor::CAudioElement();
		lpAudioElement->m_strFileName = pPic.GetAudioLink();

		int lIndex = lpAudioElement->m_strFileName.Find(_T("file:///"));
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
		VideoFile::IVideoFile3* pVideoFile = NULL;
		CoCreateInstance(__uuidof(VideoFile::CVideoFile3), NULL, CLSCTX_ALL, __uuidof(VideoFile::IVideoFile3), (void**)(&pVideoFile));

		double dDuration = 0;
		if (NULL != pVideoFile)
		{
			pVideoFile->OpenFile(bstrFile);
			pVideoFile->get_audioDuration(&dDuration);
			pVideoFile->CloseFile();
		}
		SysFreeString(bstrFile);
		RELEASEINTERFACE(pVideoFile);

		lpAudioElement->m_dStartTime	= pEContainer.m_dStartTime;
		lpAudioElement->m_dEndTime		= pEContainer.m_dStartTime + dDuration;

		pEContainer.m_arElements.Add(lpAudioElement);
	}

	void Converter::ConnectorConvert(const PPTX::Logic::CxnSp& pCxn, NSPresentationEditor::CSlide& pEContainer)
	{
		NSPresentationEditor::CShapeElement* lpShapeElement = NULL;
		if(pCxn.spPr.Geometry.is<PPTX::Logic::PrstGeom>())
		{
			const PPTX::Logic::PrstGeom lpGeom = pCxn.spPr.Geometry.as<PPTX::Logic::PrstGeom>();

			OOXMLShapes::ShapeType _lspt = GetShapeTypeFromStr(lpGeom.prst.get());
			if (_lspt == OOXMLShapes::sptNil) 
				return;
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)_lspt);
			CString strAdjustValues = lpGeom.GetODString();
			lpShapeElement->m_oShape.m_pShape->LoadAdjustValuesList(strAdjustValues);
		}
		else if(pCxn.spPr.Geometry.is<PPTX::Logic::CustGeom>())
		{
			const PPTX::Logic::CustGeom lpGeom = pCxn.spPr.Geometry.as<PPTX::Logic::CustGeom>();
			CString strShape = lpGeom.GetODString();
			lpShapeElement = new CShapeElement(strShape);
		}
		else
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCStraightConnector1);

		lpShapeElement->m_lID = pCxn.nvCxnSpPr.cNvPr.id;
		if(pCxn.spPr.xfrm.is_init())
		{
			lpShapeElement->m_dRotate	= pCxn.spPr.xfrm->rot.get_value_or(0)/60000.0;
			lpShapeElement->m_bFlipH	= pCxn.spPr.xfrm->flipH.get_value_or(false);
			lpShapeElement->m_bFlipV	= pCxn.spPr.xfrm->flipV.get_value_or(false);
		}
		lpShapeElement->m_oMetric.m_lUnitsHor		= pEContainer.m_lOriginalWidth;
		lpShapeElement->m_oMetric.m_lUnitsVer		= pEContainer.m_lOriginalHeight;
		lpShapeElement->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth / 36000;
		lpShapeElement->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight/ 36000;

		lpShapeElement->m_dStartTime = 0;
		lpShapeElement->m_dEndTime = pEContainer.m_dDuration;

		//используется только Brush !!!
		lpShapeElement->m_oShape.m_oBrush.Alpha1 = 0;
		lpShapeElement->m_oShape.m_oBrush.Alpha2 = 0;
		PPTX::Logic::UniFill fill;
		DWORD BGRA = pCxn.GetFill(fill);
		FillToBrush(fill, BGRA, lpShapeElement->m_oShape.m_oBrush);

		lpShapeElement->m_oShape.m_oPen.Alpha = 0;
		PPTX::Logic::Ln line;
		BGRA = pCxn.GetLine(line);
		LnToPen(line, BGRA, lpShapeElement->m_oShape.m_oPen);

		RECT rect;
		pCxn.GetRect(rect);
		
		lpShapeElement->m_rcBoundsOriginal.left		= (double)rect.left;
		lpShapeElement->m_rcBoundsOriginal.top		= (double)rect.top;
		lpShapeElement->m_rcBoundsOriginal.right	= (double)rect.right;
		lpShapeElement->m_rcBoundsOriginal.bottom	= (double)rect.bottom;

		lpShapeElement->NormalizeCoords(c_dScaleElements, c_dScaleElements);
		lpShapeElement->m_oShape.m_rcBounds			= lpShapeElement->m_rcBounds;

		pEContainer.m_arElements.Add(lpShapeElement);
	}

	void Converter::OleConvert(const PPTX::Logic::GraphicFrame& pOle, NSPresentationEditor::CSlide& pEContainer)
	{
		if (pOle.Table.is_init())
		{
			TableConvert(pOle, pEContainer);
		}
		else
		{
			NSPresentationEditor::CShapeElement* lpShapeElement = NULL;
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);

			RECT rect;
			pOle.GetRect(rect);
			lpShapeElement->m_rcBoundsOriginal.left		= (double)rect.left;
			lpShapeElement->m_rcBoundsOriginal.right	= (double)rect.right;
			lpShapeElement->m_rcBoundsOriginal.top		= (double)rect.top;
			lpShapeElement->m_rcBoundsOriginal.bottom	= (double)rect.bottom;

			lpShapeElement->m_oMetric.m_lUnitsHor		= pEContainer.m_lOriginalWidth;
			lpShapeElement->m_oMetric.m_lUnitsVer		= pEContainer.m_lOriginalHeight;
			lpShapeElement->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth / 36000;
			lpShapeElement->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight/ 36000;

			lpShapeElement->m_dStartTime = 0;
			lpShapeElement->m_dEndTime = pEContainer.m_dDuration;

			if (!pOle.spid.is_init())
			{
				lpShapeElement->m_oShape.m_oBrush.Alpha1 = 0;
				lpShapeElement->m_oShape.m_oBrush.Alpha2 = 0;

				lpShapeElement->m_oShape.m_oPen.Alpha = 128;
				lpShapeElement->m_oShape.m_oPen.Color = 0x80808080;

				lpShapeElement->m_oShape.m_oText.m_sText = _T("Ole-object");
				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_nTextAlignVertical = 1;
				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_nTextAlignHorizontal = 1;

				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_oTextEdge.Visible	= 1;
				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_oTextEdge.Dist		= 1;
				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_oTextEdge.Color	= 0;
				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_oTextEdge.Alpha	= 128;

				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_oTextBrush.Color1	= 0x80808080;
				lpShapeElement->m_oShape.m_oText.m_oAttributes.m_oTextBrush.Alpha1	= 128;
			}
			else
			{
				lpShapeElement->m_oShape.m_oBrush.Type			= (int)NSPresentationEditor::c_BrushTypeTexture;
				lpShapeElement->m_oShape.m_oBrush.TexturePath	= pOle.GetPathBySpid().m_strFilename;
				lpShapeElement->m_oShape.m_oBrush.TextureMode	= (int)NSPresentationEditor::c_BrushTextureModeStretch;
				lpShapeElement->m_oShape.m_oBrush.TextureAlpha	= 255;

				lpShapeElement->m_oShape.m_oPen.Alpha = 0;
			}

			lpShapeElement->NormalizeCoords(c_dScaleElements, c_dScaleElements);
			pEContainer.m_arElements.Add(lpShapeElement);
		}
	}
}