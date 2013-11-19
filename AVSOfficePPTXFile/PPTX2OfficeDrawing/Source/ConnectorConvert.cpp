#include "stdafx.h"
#include "./../Converter.h"
#include "Logic/PrstGeom.h"
#include "Logic/CustGeom.h"

namespace PPTX2OfficeDrawing
{
	void Converter::ConnectorConvert(const PPTX::Logic::CxnSp& pCxn, CAtlArray<IElement*>& pElements)
	{
		CShapeElement* lpShapeElement;
		if(pCxn.spPr->Geometry.is<PPTX::Logic::PrstGeom>())
		{
			const PPTX::Logic::PrstGeom& lpGeom = pCxn.spPr->Geometry.as<PPTX::Logic::PrstGeom>();

			OOXMLShapes::ShapeType _lspt = GetShapeTypeFromStr(lpGeom.prst.get());
			if(_lspt == OOXMLShapes::sptNil) return;
			lpShapeElement = new CShapeElement(NSBaseShape::pptx, (int)_lspt);
			std::string strAdjustValues = lpGeom.GetODString();
			lpShapeElement->m_oShape.m_pShape->LoadAdjustValuesList(CString(strAdjustValues.c_str()));
		}
		else if(pCxn.spPr->Geometry.is<PPTX::Logic::CustGeom>())
		{
			const PPTX::Logic::CustGeom& lpGeom = pCxn.spPr->Geometry.as<PPTX::Logic::CustGeom>();
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

		//используется только Brush !!!
		lpShapeElement->m_oShape.m_oBrush.m_Alpha1 = 0;
		lpShapeElement->m_oShape.m_oBrush.m_Alpha2 = 0;
		PPTX::Logic::UniFill fill;
		DWORD BGRA = pCxn.GetFill(fill);
		FillToBrush(fill, lpShapeElement->m_oShape.m_oBrush, BGRA);

		lpShapeElement->m_oShape.m_oPen.m_nAlpha = 0;
		PPTX::Logic::Ln line;
		BGRA = pCxn.GetLine(line);
		LineToPen(line, lpShapeElement->m_oShape.m_oPen, BGRA);

		RECT lRect;
		pCxn.GetRect(lRect);
		lpShapeElement->m_rcBoundsOriginal.top = lRect.top;
		lpShapeElement->m_rcBoundsOriginal.left = lRect.left;
		lpShapeElement->m_rcBoundsOriginal.right = lRect.right;
		lpShapeElement->m_rcBoundsOriginal.bottom = lRect.bottom;

		//lpShapeElement->m_oShape.SetProperties(NULL, &pEContainer);
		pElements.Add(lpShapeElement);
	}
}
