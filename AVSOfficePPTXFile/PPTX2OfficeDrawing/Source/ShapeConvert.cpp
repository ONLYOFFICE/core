#include "stdafx.h"
#include "./../Converter.h"
#include "Logic/PrstGeom.h"
#include "Logic/CustGeom.h"

namespace PPTX2OfficeDrawing
{
	void Converter::ShapeConvert(const PPTX::Logic::Shape& pShape, CAtlArray<IElement*>& pElements, bool bMergeWithLevelUp)
	{
		if(!pShape.nvSpPr->nvPr->ph.is_init())
		{
			CShapeElement* lpShape;

			if(pShape.spPr->Geometry.is<PPTX::Logic::PrstGeom>())
			{
				const PPTX::Logic::PrstGeom& lpGeom = pShape.spPr->Geometry.as<PPTX::Logic::PrstGeom>();

				OOXMLShapes::ShapeType _lspt = GetShapeTypeFromStr(lpGeom.prst.get());
				if(_lspt == OOXMLShapes::sptNil) return;
				lpShape = new CShapeElement(NSBaseShape::pptx, (int)_lspt);
				std::string strAdjustValues = lpGeom.GetODString();
				lpShape->m_oShape.m_pShape->LoadAdjustValuesList(CString(strAdjustValues.c_str()));
			}
			else if(pShape.spPr->Geometry.is<PPTX::Logic::CustGeom>())
			{
				const PPTX::Logic::CustGeom& lpGeom = pShape.spPr->Geometry.as<PPTX::Logic::CustGeom>();
				std::string strShape = lpGeom.GetODString();
				lpShape = new CShapeElement(CString(strShape.c_str()));
			}
			else
				lpShape = new CShapeElement(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);

			if(pShape.spPr->xfrm.is_init())
			{
				lpShape->m_dRotate	= pShape.spPr->xfrm->rot.get_value_or(0)/60000.0;
				lpShape->m_bFlipH	= pShape.spPr->xfrm->flipH.get_value_or(false);
				lpShape->m_bFlipV	= pShape.spPr->xfrm->flipV.get_value_or(false);
			}

			RECT lRect;
			pShape.GetRect(lRect);
			lpShape->m_rcBoundsOriginal.top = lRect.top;
			lpShape->m_rcBoundsOriginal.left = lRect.left;
			lpShape->m_rcBoundsOriginal.right = lRect.right;
			lpShape->m_rcBoundsOriginal.bottom = lRect.bottom;

			lpShape->m_lID = pShape.nvSpPr->cNvPr->id.get();
			//int id = lpShapeElement->m_lID;

			//используется только Brush !!!
			lpShape->m_oShape.m_oBrush.m_Alpha1 = 0;
			lpShape->m_oShape.m_oBrush.m_Alpha2 = 0;
			PPTX::Logic::UniFill fill;
			DWORD BGRA = pShape.GetFill(fill);
			FillToBrush(fill, lpShape->m_oShape.m_oBrush, BGRA);

			lpShape->m_oShape.m_oPen.m_nAlpha = 0;
			PPTX::Logic::Ln line;
			BGRA = pShape.GetLine(line);
			LineToPen(line, lpShape->m_oShape.m_oPen, BGRA);

			//CStringW ShapeText = GetCStringFromUTF8(CString(pShape.GetText().c_str()));
			CStringW ShapeText = CStringW(pShape.GetWText().c_str());
			if((ShapeText != L"") && (ShapeText != L"\n"))
			{
				lpShape->m_oShape.m_dTextMarginX = pShape.txBody->bodyPr->lIns.get_value_or(91440) / 36000.0;
				lpShape->m_oShape.m_dTextMarginY = pShape.txBody->bodyPr->tIns.get_value_or(45720) / 36000.0;
				lpShape->m_oShape.m_dTextMarginRight = pShape.txBody->bodyPr->rIns.get_value_or(91440) / 36000.0;
				lpShape->m_oShape.m_dTextMarginBottom = pShape.txBody->bodyPr->bIns.get_value_or(45720) / 36000.0;

/*
				double FontScale = pShape.txBody->bodyPr->Fit.fontScale.get_value_or(100000)/100000.0;
				double LnSpcReduction = pShape.txBody->bodyPr->Fit.lnSpcReduction.get_value_or(0)/100000.0;
*/
				TextParse(pShape.txBody.get(), lpShape->m_oShape.m_oText/*, ShapeProps, FontScale, LnSpcReduction*/);
			}

//			lpShapeElement->m_oShape.SetProperties(NULL, &pEContainer);
			pElements.Add((IElement*)lpShape);
		}
		else if(bMergeWithLevelUp)
		{
			//CShapeElement* lpShape = new
			
			// вызкано из Layout
			
			// нужно пользовать GetShapeFullDescription (merge with master)
			// и идти в следующую ветку			
		}
		else
		{
			//CShapeElement* lpShape = new
			// в этой ветке заполняются ТОЛЬКО проперти выставленные. Ничего мержить не нужно
		}
	}
}
