#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2OfficeDrawing
{
	void Converter::GroupConvert(const PPTX::Logic::SpTree& Group, CAtlArray<IElement*>& pElements, bool bMergeWithLevelUp)
	{
		for(std::list<PPTX::Logic::SpTreeElem>::const_iterator i = Group.SpTreeElems->begin(); i != Group.SpTreeElems->end(); i++)
		{
			if(i->is<PPTX::Logic::Shape>())
			{
				const PPTX::Logic::Shape& lpOriginShape = i->as<PPTX::Logic::Shape>();
				ShapeConvert(lpOriginShape, pElements, bMergeWithLevelUp);
			}
			else if(i->is<PPTX::Logic::Pic>())
			{
				const PPTX::Logic::Pic& lpOriginPic = i->as<PPTX::Logic::Pic>();
				PicConvert(lpOriginPic, pElements);
			}
			else if(i->is<PPTX::Logic::CxnSp>())
			{
				const PPTX::Logic::CxnSp& lpOriginCxn = i->as<PPTX::Logic::CxnSp>();
				ConnectorConvert(lpOriginCxn, pElements);
			}
			else if(i->is<PPTX::Logic::SpTree>())
			{
				const PPTX::Logic::SpTree& lpOriginGroup = i->as<PPTX::Logic::SpTree>();
				GroupConvert(lpOriginGroup, pElements, bMergeWithLevelUp);
			}
			else if(i->is<PPTX::Logic::GraphicFrame>())
			{
				const PPTX::Logic::GraphicFrame& lpOle = i->as<PPTX::Logic::GraphicFrame>();
				OleConvert(lpOle, pElements);
			}
		}
	}

	void Converter::BackgroundConvert(const nullable_property<PPTX::Logic::Bg> pBg, CAtlArray<IElement*>& pElements)
	{
		if(pBg.is_init())
		{
			CShapeElement* lpSlideBackground = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);

			//lpSlideBackground->m_dStartTime = 0;
			//lpSlideBackground->m_dEndTime = lEContainer->m_dDuration;

			lpSlideBackground->m_oShape.m_oPen.m_nAlpha = 0;

			PPTX::Logic::BgPr bg;
			DWORD ARGB = 0;
			pBg->GetBackground(bg, ARGB);
			DWORD BGRA = (ARGB & 0xFF) << 24;
			BGRA += (ARGB & 0xFF00) << 8;
			BGRA += (ARGB & 0xFF0000) >> 8;
			BGRA += (ARGB & 0xFF000000) >> 24;
			FillToBrush(bg.Fill, lpSlideBackground->m_oShape.m_oBrush, BGRA);

			lpSlideBackground->m_bIsChangeable = false;
			//lpSlideBackground->m_oShape.SetProperties(NULL, lEContainer);
			pElements.Add((IElement*)lpSlideBackground);
		}
	}

	void Converter::AddNonPlaceholders(const PPTX::Logic::SpTree& Group, CAtlArray<IElement*>& pElements)
	{
		for(std::list<PPTX::Logic::SpTreeElem>::const_iterator i = Group.SpTreeElems->begin(); i != Group.SpTreeElems->end(); i++)
		{
			if(i->is<PPTX::Logic::Shape>())
			{
				const PPTX::Logic::Shape& lpOriginShape = i->as<PPTX::Logic::Shape>();
				if(!lpOriginShape.nvSpPr->nvPr->ph.is_init())
					ShapeConvert(lpOriginShape, pElements, false);
			}
			else if(i->is<PPTX::Logic::Pic>())
			{
				const PPTX::Logic::Pic& lpOriginPic = i->as<PPTX::Logic::Pic>();
				PicConvert(lpOriginPic, pElements);
			}
			else if(i->is<PPTX::Logic::CxnSp>())
			{
				const PPTX::Logic::CxnSp& lpOriginCxn = i->as<PPTX::Logic::CxnSp>();
				ConnectorConvert(lpOriginCxn, pElements);
			}
			else if(i->is<PPTX::Logic::SpTree>())
			{
				const PPTX::Logic::SpTree& lpOriginGroup = i->as<PPTX::Logic::SpTree>();
				AddNonPlaceholders(lpOriginGroup, pElements);
			}
			else if(i->is<PPTX::Logic::GraphicFrame>())
			{
				const PPTX::Logic::GraphicFrame& lpOle = i->as<PPTX::Logic::GraphicFrame>();
				OleConvert(lpOle, pElements);
			}
		}
	}
}