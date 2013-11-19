#include "stdafx.h"
#include "./../Converter.h"
#include "../../PPTXFormat/Logic/SpTreeElem.h"
#include "../../PPTXFormat/Logic/BgPr.h"
#include "../../PPTXFormat/SlideLayout.h"
#include "../../PPTXFormat/SlideMaster.h"

namespace PPTX2EditorSimple
{
	void Converter::SlideConvert(const smart_ptr<PPTX::Slide> Slide, const DWORD SldNum, double* Duration, const int cx, const int cy, NSPresentationEditor::CDocument& pDocument)
	{
		pDocument.m_arSlides.Add();
		NSPresentationEditor::CSlide& lEContainer = pDocument.m_arSlides[pDocument.m_arSlides.GetCount() - 1];
		
		lEContainer.m_lOriginalWidth	= cx;
		lEContainer.m_lOriginalHeight	= cy;
		lEContainer.m_lWidth			= cx/36000;//pDocument->m_lWidth;
		lEContainer.m_lHeight			= cy/36000;//pDocument->m_lHeight;

		lEContainer.m_dStartTime = *Duration;
		if(Slide->transition.is_init())
		{
			int time = 0;
			time = TransitionConvert(SldNum, Slide, pDocument);
			if(time > 0)
			{
				lEContainer.m_dDuration = time;
				//lEContainer->m_bDefaultDuration = false;
				*Duration += time;
			}
			else
			{
				lEContainer.m_dDuration = 30000.0;
				*Duration += 30000.0;
			}
		}

		//lEContainer->m_dDuration = 30000.0;
		//*Duration += 30000.0;
		lEContainer.m_dEndTime = *Duration;

		CShapeElement* lpSlideBackground = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);

		lpSlideBackground->m_dStartTime = 0;
		lpSlideBackground->m_dEndTime = lEContainer.m_dDuration;

		lpSlideBackground->m_rcBoundsOriginal.top		= 0;
		lpSlideBackground->m_rcBoundsOriginal.left		= 0;
		lpSlideBackground->m_rcBoundsOriginal.right		= cx;
		lpSlideBackground->m_rcBoundsOriginal.bottom	= cy;

		lpSlideBackground->m_oMetric.m_lUnitsHor		= cx;
		lpSlideBackground->m_oMetric.m_lUnitsVer		= cy;
		lpSlideBackground->m_oMetric.m_lMillimetresHor	= cx/36000;
		lpSlideBackground->m_oMetric.m_lMillimetresVer	= cy/36000;

		lpSlideBackground->NormalizeCoords(c_dScaleElements, c_dScaleElements);

		lpSlideBackground->m_bIsBackground = true;
		lpSlideBackground->m_bIsChangeable = false;

		lpSlideBackground->m_oShape.m_oPen.Alpha = 0;

		PPTX::Logic::BgPr bg;
		DWORD ARGB = 0;
		Slide->GetBackground(bg, ARGB);
		DWORD BGRA = (ARGB & 0xFF) << 24;
		BGRA += (ARGB & 0xFF00) << 8;
		BGRA += (ARGB & 0xFF0000) >> 8;
		BGRA += (ARGB & 0xFF000000) >> 24;
		FillToBrush(bg.Fill, BGRA, lpSlideBackground->m_oShape.m_oBrush);

		lEContainer.m_arElements.Add(lpSlideBackground);

		//boost::shared_ptr<PPTX::SlideMaster> master = Slide->Master;
		//boost::shared_ptr<PPTX::SlideLayout> layout = Slide->Layout;
		if((Slide->Layout->showMasterSp.get_value_or(true)) && (Slide->showMasterSp.get_value_or(true)))
		{
			if(Slide->Master.IsInit())
				AddParentPics(Slide->Master->cSld.spTree, lEContainer);
		}

		if(Slide->Layout.IsInit())
			AddParentPics(Slide->Layout->cSld.spTree, lEContainer);

		if(Slide->cSld.IsInit())
		{
			PPTX::Logic::SpTree& group = Slide->cSld->spTree;
			GroupConvert(group, lEContainer);
		}

		//if(Slide->transition.is_init())
		//{
		//	int time = 0;
		//	time = TransitionConvert(SldNum, Slide->transition.get(), pDocument);
		//	if(time > 0)
		//}
		//if(pFolder.Slides.Slides->at(SldNum)->timing.is_init())
		//{
		//	TimingConvert(SldNum, pFolder.Slides.Slides->at(SldNum)->timing.get(), pDocument);
		//}
	}

	void Converter::GroupConvert(const PPTX::Logic::SpTree& Group, NSPresentationEditor::CSlide& pEContainer)
	{
		size_t nCountElems = Group.SpTreeElems.GetCount();
		for (size_t nIndex = 0; nIndex < nCountElems; ++nIndex)
		{
			const PPTX::Logic::SpTreeElem* pElem = &Group.SpTreeElems[nIndex];

			if(pElem->is<PPTX::Logic::Shape>())
			{
				const PPTX::Logic::Shape& lpOriginShape = pElem->as<PPTX::Logic::Shape>();
				ShapeConvert(lpOriginShape, pEContainer);
			}
			else if(pElem->is<PPTX::Logic::Pic>())
			{
				const PPTX::Logic::Pic& lpOriginPic = pElem->as<PPTX::Logic::Pic>();
				PicConvert(lpOriginPic, pEContainer);
			}
			else if(pElem->is<PPTX::Logic::CxnSp>())
			{
				const PPTX::Logic::CxnSp& lpOriginCxn = pElem->as<PPTX::Logic::CxnSp>();
				ConnectorConvert(lpOriginCxn, pEContainer);
			}
			else if(pElem->is<PPTX::Logic::SpTree>())
			{
				const PPTX::Logic::SpTree& lpOriginGroup = pElem->as<PPTX::Logic::SpTree>();
				GroupConvert(lpOriginGroup, pEContainer);
			}
			else if(pElem->is<PPTX::Logic::GraphicFrame>())
			{
				const PPTX::Logic::GraphicFrame& lpOle = pElem->as<PPTX::Logic::GraphicFrame>();
				OleConvert(lpOle, pEContainer);
			}
		}
	}

	void Converter::AddParentPics(const PPTX::Logic::SpTree& Group, NSPresentationEditor::CSlide& pEContainer)
	{
		size_t nCountElems = Group.SpTreeElems.GetCount();
		for (size_t nIndex = 0; nIndex < nCountElems; ++nIndex)
		{
			const PPTX::Logic::SpTreeElem* pElem = &Group.SpTreeElems[nIndex];

			size_t lCountElemsOld = pEContainer.m_arElements.GetCount();

			if(pElem->is<PPTX::Logic::Shape>())
			{
				const PPTX::Logic::Shape& lpOriginShape = pElem->as<PPTX::Logic::Shape>();
				if(!lpOriginShape.nvSpPr.nvPr.ph.is_init())
					ShapeConvert(lpOriginShape, pEContainer);
			}
			else if(pElem->is<PPTX::Logic::Pic>())
			{
				const PPTX::Logic::Pic& lpOriginPic = pElem->as<PPTX::Logic::Pic>();
				PicConvert(lpOriginPic, pEContainer);
			}
			else if(pElem->is<PPTX::Logic::CxnSp>())
			{
				const PPTX::Logic::CxnSp& lpOriginCxn = pElem->as<PPTX::Logic::CxnSp>();
				if(!lpOriginCxn.nvCxnSpPr.nvPr.ph.is_init())
					ConnectorConvert(lpOriginCxn, pEContainer);
			}
			else if(pElem->is<PPTX::Logic::SpTree>())
			{
				const PPTX::Logic::SpTree& lpOriginGroup = pElem->as<PPTX::Logic::SpTree>();
				AddParentPics(lpOriginGroup, pEContainer);
			}
			else if(pElem->is<PPTX::Logic::GraphicFrame>())
			{
				const PPTX::Logic::GraphicFrame& lpOle = pElem->as<PPTX::Logic::GraphicFrame>();
				OleConvert(lpOle, pEContainer);
			}

			size_t lCountElemsNew = pEContainer.m_arElements.GetCount();

			for (size_t i = lCountElemsOld; i < lCountElemsNew; ++i)
			{
				pEContainer.m_arElements[i]->m_bIsBackground = false;
				pEContainer.m_arElements[i]->m_bIsChangeable = false;
			}
		}

	}
}