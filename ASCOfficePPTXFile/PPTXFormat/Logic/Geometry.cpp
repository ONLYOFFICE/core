//#include "stdafx.h"

#ifdef AVS_USE_CONVERT_PPTX_TOCUSTOM_VML

#include "Geometry.h"
#include "../../../ASCPresentationEditor/OfficeDrawing/Elements.h"

namespace PPTX
{
	namespace Logic
	{
		void Geometry::ConvertToCustomVML(IASCRenderer* pOOXToVMLRenderer, CString& strPath, CString& strRect, LONG& lWidth, LONG& lHeight)
		{
			NSPresentationEditor::CShapeElement* lpShapeElement = NULL;
			if (this->is<PPTX::Logic::PrstGeom>())
			{
				const PPTX::Logic::PrstGeom lpGeom = this->as<PPTX::Logic::PrstGeom>();

				OOXMLShapes::ShapeType _lspt = PPTX2EditorAdvanced::GetShapeTypeFromStr(lpGeom.prst.get());
				if(_lspt == OOXMLShapes::sptNil) 
					return;

				lpShapeElement = new NSPresentationEditor::CShapeElement(NSBaseShape::pptx, (int)_lspt);
				CString strAdjustValues = lpGeom.GetODString();
				lpShapeElement->m_oShape.m_pShape->LoadAdjustValuesList(strAdjustValues);
			}
			else if (this->is<PPTX::Logic::CustGeom>())
			{
				const PPTX::Logic::CustGeom lpGeom = this->as<PPTX::Logic::CustGeom>();
				CString strShape = lpGeom.GetODString();
				lpShapeElement = new NSPresentationEditor::CShapeElement(strShape);
			}

			if (lpShapeElement == NULL)
				return;

			double dCoordSizeX = (double)max(lWidth, 1);
			double dCoordSizeY = (double)max(lHeight, 1);

			LONG lCoordSize = 100000;

			lpShapeElement->m_oShape.m_pShape->SetWidthHeightLogic(dCoordSizeX, dCoordSizeY);
			lpShapeElement->m_oShape.m_pShape->ReCalculate();

			pOOXToVMLRenderer->put_Width((double)lCoordSize / dCoordSizeX);
			pOOXToVMLRenderer->put_Height((double)lCoordSize / dCoordSizeY);						

			CGeomShapeInfo oInfo;
			oInfo.m_dLeft	= 0;
			oInfo.m_dTop	= 0;
			oInfo.m_dWidth	= dCoordSizeX;
			oInfo.m_dHeight	= dCoordSizeY;

			NSPresentationEditor::CPath& oPath = lpShapeElement->m_oShape.m_pShape->m_oPath;
			
			VARIANT var;
			var.vt = VT_I4;
			var.lVal = 0;
			pOOXToVMLRenderer->SetAdditionalParam(L"NewShape", var);

			CGraphicPath oGrPath;			
			CMetricInfo oMetricInfo;

			int nSize = oPath.m_arParts.GetSize();
			for (int nIndex = 0; nIndex < nSize; ++nIndex)
			{
				oGrPath.Clear();
				oPath.m_arParts[nIndex].ToRendererOOX(&oGrPath, oInfo, oMetricInfo, NSBaseShape::pptx);

				pOOXToVMLRenderer->put_PenAlpha(oGrPath.m_bStroke ? 255 : 0);
				pOOXToVMLRenderer->put_BrushAlpha1(oGrPath.m_bFill ? 255 : 0);

				oGrPath.m_dWidthMM = (double)lCoordSize / dCoordSizeX;
				oGrPath.m_dHeightMM = (double)lCoordSize / dCoordSizeY;

				oGrPath.Draw(pOOXToVMLRenderer);
			}

			pOOXToVMLRenderer->GetAdditionalParam(L"ResultPath", &var);
			strPath = (CString)var.bstrVal;

			if (lpShapeElement->m_oShape.m_pShape->m_arTextRects.GetSize() <= 0)
			{
				strRect = _T("0,0,100000,100000");
			}
			else
			{
				RECT& txRect = lpShapeElement->m_oShape.m_pShape->m_arTextRects[0];
				//double dkoefX = (double)lCoordSize / max(1, dCoordSizeX);
				//double dkoefY = (double)lCoordSize / max(1, dCoordSizeY);

				double _dWidth = ShapeSize;
				double _dHeight = ShapeSize;
				lpShapeElement->m_oShape.m_pShape->GetWidthHeightLogic(_dWidth, _dHeight);

				double dkoefX = (double)lCoordSize / max(1, _dWidth);
				double dkoefY = (double)lCoordSize / max(1, _dHeight);

				strRect = _T("");
				strRect.Format(_T("%d,%d,%d,%d"), (int)(dkoefX * txRect.left), (int)(dkoefY * txRect.top),
					(int)(dkoefX * txRect.right), (int)(dkoefY * txRect.bottom));
			}

			SysFreeString((var.bstrVal));
		}
	}
}

#endif