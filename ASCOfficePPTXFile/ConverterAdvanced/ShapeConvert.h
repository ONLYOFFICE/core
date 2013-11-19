#pragma once

#include <atlutil.h>

#include "TableConvert.h"

#include "../PPTXFormat/Logic/PrstGeom.h"
#include "../PPTXFormat/Logic/CustGeom.h"
#include "../PPTXFormat/Logic/Fills/SolidFill.h"
#include "../PPTXFormat/Logic/Fills/NoFill.h"
#include "../PPTXFormat/Slide.h"
#include "../PPTXFormat/SlideLayout.h"
#include "../PPTXFormat/SlideMaster.h"

#include "PathHelpers.h"

namespace PPTX2EditorAdvanced
{
	void VideoElementConvert(const PPTX::Logic::Pic& pPic, CAtlArray<IElement*>& pElements, CPresentationInfo& oPresentation)
	{
		CVideoElement* pVideo	=	new CVideoElement();
		CAudioElement* pAudio	=	new CAudioElement();

		if (NULL != pVideo && NULL == pAudio)
			return;

		pVideo->m_strFileName	=	PathHelpers::ResourceFileSystemPathXml (pPic.GetVideoLink());			
		
		CString fileName		=	PathHelpers::ResourceFileSystemPath (pPic.GetVideoLink());
		BSTR bstrFile			=	fileName.AllocSysString();

		double dDuration		=	0;
		VideoFile::IVideoFile3* pVideoFile = NULL;
		if ( SUCCEEDED(CoCreateInstance(__uuidof(VideoFile::CVideoFile3), NULL, CLSCTX_ALL, __uuidof(VideoFile::IVideoFile3), (void**)(&pVideoFile))) )
		{
			if (pVideoFile)
			{
				if (S_OK == pVideoFile->OpenFile(bstrFile))
				{
					pVideoFile->get_videoDuration(&dDuration);
					pVideoFile->CloseFile();
				}
				else
				{
					// поуэрпоинт смотрит где файл лежит еще
					CString strFileName = oPresentation.m_strDirectory;
					int nIndex = fileName.ReverseFind(TCHAR('\\'));
					if (-1 != nIndex)
					{
						strFileName += fileName.Mid(nIndex);

						SysFreeString(bstrFile);						
						bstrFile = strFileName.AllocSysString();

						if (S_OK == pVideoFile->OpenFile(bstrFile))
						{
							pVideo->m_strFileName = PathHelpers::ResourceFileSystemPathXml (strFileName);

							pVideoFile->get_videoDuration(&dDuration);
							pVideoFile->CloseFile();
						} 
					}
				}

				RELEASEINTERFACE(pVideoFile);
			}
		}

		SysFreeString(bstrFile);

		pVideo->m_lID			=	pPic.nvPicPr.cNvPr.id;
		pVideo->m_dStartTime	=	0;
		pVideo->m_dEndTime		=	pVideo->m_dStartTime + dDuration;

		RECT rect;
		pPic.GetRect(rect);
		pVideo->m_rcBoundsOriginal.left		= (double)rect.left;
		pVideo->m_rcBoundsOriginal.right	= (double)rect.right;
		pVideo->m_rcBoundsOriginal.top		= (double)rect.top;
		pVideo->m_rcBoundsOriginal.bottom	= (double)rect.bottom;

		pAudio->m_dStartTime	= pVideo->m_dStartTime;
		pAudio->m_dEndTime		= pVideo->m_dEndTime;
		pAudio->m_strFileName	= pVideo->m_strFileName;
		pAudio->m_bWithVideo	= true;

		pVideo->NormalizeCoords(c_dScaleElements, c_dScaleElements);

		pElements.Add(pVideo);
		pElements.Add(pAudio);
	}

	void AudioElementConvert (const PPTX::Logic::Pic& pPic, CAtlArray<IElement*>& pElements, CPresentationInfo& oPresentation)
	{
		CAudioElement* pAudio		=	new CAudioElement();
		if (pAudio)
		{
			pAudio->m_lID			=	pPic.nvPicPr.cNvPr.id;
			pAudio->m_strFileName	=	PathHelpers::ResourceFileSystemPathXml (pPic.GetAudioLink());			
		
			CString fileName		=	PathHelpers::ResourceFileSystemPath (pPic.GetAudioLink());
			BSTR bstrFile			=	fileName.AllocSysString();

			VideoFile::IVideoFile3* pVideoFile = NULL;
			if ( SUCCEEDED(CoCreateInstance(__uuidof(VideoFile::CVideoFile3), NULL, CLSCTX_ALL, __uuidof(VideoFile::IVideoFile3), (void**)(&pVideoFile)) ))
			{
				double dDuration = 0;
				if (pVideoFile)
				{
					if (S_OK == pVideoFile->OpenFile(bstrFile))
					{
						pVideoFile->get_audioDuration(&dDuration);
						pVideoFile->CloseFile();
					}
					else
					{
						// поуэрпоинт смотрит где файл лежит еще
						CString strFileName = oPresentation.m_strDirectory;
						int nIndex = fileName.ReverseFind(TCHAR('\\'));
						if (-1 != nIndex)
						{
							strFileName += fileName.Mid(nIndex);
							SysFreeString(bstrFile);
							bstrFile = strFileName.AllocSysString();

							if (S_OK == pVideoFile->OpenFile(bstrFile))
							{
								pAudio->m_strFileName = PathHelpers::ResourceFileSystemPathXml (strFileName);

								pVideoFile->get_audioDuration(&dDuration);
								pVideoFile->CloseFile();
							} 
						}
					}

					RELEASEINTERFACE(pVideoFile);
				}

				pAudio->m_dAudioDuration	=	dDuration;

				SysFreeString(bstrFile);

				pElements.Add(pAudio);

				return;
			}

			SysFreeString(bstrFile);
			RELEASEOBJECT (pAudio);
		}
	}

	void ConnectorConvert(const PPTX::Logic::CxnSp& pCxn, CAtlArray<IElement*>& pElements, CPresentationInfo& oPresentation)
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
		lpShapeElement->m_dStartTime	= 0;
		lpShapeElement->m_dEndTime		= 0;

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

		pElements.Add(lpShapeElement);
	}

	void OleConvert(const PPTX::Logic::GraphicFrame& pOle, CAtlArray<IElement*>& pElements, CPresentationInfo& oPresentation, NSPresentationEditor::CTheme* pTheme)
	{
		if (pOle.Table.is_init())
		{
			TableConvert(pOle, pElements, pTheme);
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

			lpShapeElement->m_dStartTime	= 0;
			lpShapeElement->m_dEndTime		= 0;

			if (!pOle.spid.is_init())
			{
				lpShapeElement->m_oShape.m_oBrush.Alpha1 = 0;
				lpShapeElement->m_oShape.m_oBrush.Alpha2 = 0;

				lpShapeElement->m_oShape.m_oPen.Alpha = 128;
				lpShapeElement->m_oShape.m_oPen.Color = 0x80808080;

				//lpShapeElement->m_oShape.m_oText.m_sText = _T("Ole-object");

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
			pElements.Add(lpShapeElement);
		}
	}

	void ShapeConvert(const PPTX::Logic::Shape& pShape, CAtlArray<IElement*>& pElements, CPresentationInfo& oPresentation, CTheme* pTheme)
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

		lpShapeElement->m_dStartTime				= 0;
		lpShapeElement->m_dEndTime					= 0;

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

		if (TRUE)
		{
			// отменяем схему цветов, а то они потом опять пресчитаются
			// этот код теперь только для видеосорс - поэтому так можно сделать
			lpShapeElement->m_oShape.m_oBrush.Color1.m_lSchemeIndex = -1;
			lpShapeElement->m_oShape.m_oBrush.Color2.m_lSchemeIndex = -1;
			lpShapeElement->m_oShape.m_oPen.Color.m_lSchemeIndex = -1;
		}

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
			PPTX::Logic::CShapeTextProperties ShapeProps;
			localShape.FillShapeTextProperties(ShapeProps);

			lpShapeElement->m_oShape.m_dTextMarginX			= localShape.txBody->bodyPr.lIns.get_value_or(91440) / 36000.0;
			lpShapeElement->m_oShape.m_dTextMarginY			= localShape.txBody->bodyPr.tIns.get_value_or(45720) / 36000.0;
			lpShapeElement->m_oShape.m_dTextMarginRight		= localShape.txBody->bodyPr.rIns.get_value_or(91440) / 36000.0;
			lpShapeElement->m_oShape.m_dTextMarginBottom	= localShape.txBody->bodyPr.bIns.get_value_or(45720) / 36000.0;

			//lpShapeElement->m_oShape.m_oText.m_sText = ShapeText;
			double FontScale = localShape.txBody->bodyPr.Fit.fontScale.get_value_or(100000)/100000.0;
			double LnSpcReduction = localShape.txBody->bodyPr.Fit.lnSpcReduction.get_value_or(0)/100000.0;
			TextParse(pShape.txBody.get(), lpShapeElement->m_oShape.m_oText, ShapeProps, FontScale, LnSpcReduction, pTheme);
		}

		lpShapeElement->NormalizeCoords(c_dScaleElements, c_dScaleElements);
		lpShapeElement->m_oShape.m_rcBounds			= lpShapeElement->m_rcBounds;

		if (pShape.nvSpPr.nvPr.ph.is_init())
		{
			int nID		= XmlUtils::GetInteger(pShape.nvSpPr.nvPr.ph->idx.get_value_or(_T("-1")));

			if ((size_t)nID == 0x7FFFFFFF)
				nID = 1;

			int nType	= PPTX::Limit::GetPhType(pShape.nvSpPr.nvPr.ph->type.get_value_or(_T("body")));

			lpShapeElement->m_lPlaceholderID	= nID;
			lpShapeElement->m_lPlaceholderType	= nType; 

			lpShapeElement->m_oShape.m_oText.m_lPlaceholderType = nType;
		}

		pElements.Add(lpShapeElement);
	}

	void PicConvert(const PPTX::Logic::Pic& pPic, CAtlArray<IElement*>& pElements, CPresentationInfo& oPresentation)
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

		lpShapeElement->m_dStartTime	= 0;
		lpShapeElement->m_dEndTime		= 0;

		lpShapeElement->m_oShape.m_oBrush.Type = (int)NSPresentationEditor::c_BrushTypeTexture;
		if (pPic.blipFill.blip.IsInit())
		{
			lpShapeElement->m_oShape.m_oBrush.TexturePath = pPic.blipFill.blip->GetFullPicName();
		}
		else
		{
			lpShapeElement->m_oShape.m_oBrush.TexturePath = _T("");
		}
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

			if ((lpShapeElement2->m_oShape.m_oBrush.Type != c_BrushTypeTexture) || 
				(lpShapeElement2->m_oShape.m_oBrush.TexturePath != lpShapeElement->m_oShape.m_oBrush.TexturePath))
				pElements.Add(lpShapeElement2);
		}

		pElements.Add(lpShapeElement);

		if (pPic.GetVideoLink() != _T(""))
			VideoElementConvert(pPic, pElements, oPresentation);
		else if (pPic.GetAudioLink() != _T(""))
			AudioElementConvert(pPic, pElements, oPresentation);
	}

	void AddNonPlaceholders(const PPTX::Logic::SpTree& Group, CAtlArray<IElement*>& pElements, CPresentationInfo& oPresentation, CTheme* pTheme)
	{
		size_t nCount = Group.SpTreeElems.GetCount();
		for (size_t i = 0; i < nCount; ++i)
		{
			const PPTX::Logic::SpTreeElem& shape = Group.SpTreeElems[i];

			if (shape.is<PPTX::Logic::Shape>())
			{
				const PPTX::Logic::Shape& lpOriginShape = shape.as<PPTX::Logic::Shape>();
				if(!lpOriginShape.nvSpPr.nvPr.ph.is_init())
					ShapeConvert(lpOriginShape, pElements, oPresentation, pTheme);
			}
			else if(shape.is<PPTX::Logic::Pic>())
			{
				const PPTX::Logic::Pic& lpOriginPic = shape.as<PPTX::Logic::Pic>();
				PicConvert(lpOriginPic, pElements, oPresentation);
			}
			else if(shape.is<PPTX::Logic::CxnSp>())
			{
				const PPTX::Logic::CxnSp& lpOriginCxn = shape.as<PPTX::Logic::CxnSp>();
				ConnectorConvert(lpOriginCxn, pElements, oPresentation);
			}
			else if(shape.is<PPTX::Logic::SpTree>())
			{
				const PPTX::Logic::SpTree& lpOriginGroup = shape.as<PPTX::Logic::SpTree>();
				AddNonPlaceholders(lpOriginGroup, pElements, oPresentation, pTheme);
			}
			else if(shape.is<PPTX::Logic::GraphicFrame>())
			{
				const PPTX::Logic::GraphicFrame& lpOle = shape.as<PPTX::Logic::GraphicFrame>();
				OleConvert(lpOle, pElements, oPresentation, pTheme);
			}
		}
	}

	void GroupConvert(const PPTX::Logic::SpTree& Group, CAtlArray<IElement*>& pElements, CPresentationInfo& oPresentation, CTheme* pTheme)
	{
		size_t nCount = Group.SpTreeElems.GetCount();
		for (size_t i = 0; i < nCount; ++i)
		{
			const PPTX::Logic::SpTreeElem& shape = Group.SpTreeElems[i];

			if (shape.is<PPTX::Logic::Shape>())
			{
				const PPTX::Logic::Shape& lpOriginShape = shape.as<PPTX::Logic::Shape>();
				ShapeConvert(lpOriginShape, pElements, oPresentation, pTheme);
			}
			else if(shape.is<PPTX::Logic::Pic>())
			{
				const PPTX::Logic::Pic& lpOriginPic = shape.as<PPTX::Logic::Pic>();
				PicConvert(lpOriginPic, pElements, oPresentation);
			}
			else if(shape.is<PPTX::Logic::CxnSp>())
			{
				const PPTX::Logic::CxnSp& lpOriginCxn = shape.as<PPTX::Logic::CxnSp>();
				ConnectorConvert(lpOriginCxn, pElements, oPresentation);
			}
			else if(shape.is<PPTX::Logic::SpTree>())
			{
				const PPTX::Logic::SpTree& lpOriginGroup = shape.as<PPTX::Logic::SpTree>();
				GroupConvert(lpOriginGroup, pElements, oPresentation, pTheme);
			}
			else if(shape.is<PPTX::Logic::GraphicFrame>())
			{
				const PPTX::Logic::GraphicFrame& lpOle = shape.as<PPTX::Logic::GraphicFrame>();
				OleConvert(lpOle, pElements, oPresentation, pTheme);
			}
		}
	}
}