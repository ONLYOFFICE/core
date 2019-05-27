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

#include "Geometry.h"
#include "../../Editor/Drawing/Elements.h"
#include "../../Editor/Drawing/Shapes/BaseShape/toVmlConvert.h"

namespace PPTX
{
	namespace Logic
	{
		void Geometry::ConvertToCustomVML(IRenderer* pOOXToVMLRenderer, std::wstring& strPath, std::wstring& strRect, LONG& lWidth, LONG& lHeight)
		{
			if (!pOOXToVMLRenderer) return;

			PPT_FORMAT::CShapeElement* lpShapeElement = NULL;
			if (this->is<PPTX::Logic::PrstGeom>())
			{
				const PPTX::Logic::PrstGeom & lpGeom = this->as<PPTX::Logic::PrstGeom>();

				OOXMLShapes::ShapeType _lspt = PPTX2EditorAdvanced::GetShapeTypeFromStr(lpGeom.prst.get());
				if(_lspt == OOXMLShapes::sptNil) 
					return;

				lpShapeElement = new PPT_FORMAT::CShapeElement(NSBaseShape::pptx, (int)_lspt);
				std::wstring strAdjustValues = lpGeom.GetODString();
				
				lpShapeElement->m_pShape->getBaseShape()->LoadAdjustValuesList(strAdjustValues);
			}
			else if (this->is<PPTX::Logic::CustGeom>())
			{
				const PPTX::Logic::CustGeom & lpGeom = this->as<PPTX::Logic::CustGeom>();
				std::wstring strShape = lpGeom.GetODString();
				lpShapeElement = new PPT_FORMAT::CShapeElement(strShape);
			}

			if (lpShapeElement == NULL)
				return;

            double dCoordSizeX = (double)(std::max)((int)lWidth, 1);
            double dCoordSizeY = (double)(std::max)((int)lHeight, 1);

			LONG lCoordSize = 100000;

			lpShapeElement->m_pShape->getBaseShape()->SetWidthHeightLogic(dCoordSizeX, dCoordSizeY);
			lpShapeElement->m_pShape->getBaseShape()->ReCalculate();

			pOOXToVMLRenderer->put_Width((double)lCoordSize / dCoordSizeX);
			pOOXToVMLRenderer->put_Height((double)lCoordSize / dCoordSizeY);						

			CGeomShapeInfo oInfo;
			
			oInfo.m_dLeft	= 0;
			oInfo.m_dTop	= 0;
			
			oInfo.m_dWidth	= dCoordSizeX;
			oInfo.m_dHeight	= dCoordSizeY;

			ODRAW::CPath& oPath = lpShapeElement->m_pShape->getBaseShape()->m_oPath;
			
			COOXToVMLGeometry* pOOXToVMLGeometry = dynamic_cast<COOXToVMLGeometry*>(pOOXToVMLRenderer);

            if(NULL != pOOXToVMLGeometry)
				pOOXToVMLGeometry->NewShape();

			ODRAW::CGraphicPath oGrPath;			

			for (size_t nIndex = 0; nIndex < oPath.m_arParts.size(); ++nIndex)
			{
				oGrPath.Clear();
				oPath.m_arParts[nIndex].ToRendererOOX(&oGrPath, oInfo, NSBaseShape::pptx);

				pOOXToVMLRenderer->put_PenAlpha(oGrPath.m_bStroke ? 255 : 0);
				pOOXToVMLRenderer->put_BrushAlpha1(oGrPath.m_bFill ? 255 : 0);

				oGrPath.Draw(pOOXToVMLRenderer);
			}
			if(NULL != pOOXToVMLGeometry)
				pOOXToVMLGeometry->ResultPath(&strPath);

			if (lpShapeElement->m_pShape->getBaseShape()->m_arTextRects.size() <= 0)
			{
				strRect = _T("0,0,100000,100000");
			}
			else
			{
				Aggplus::RECT& txRect = lpShapeElement->m_pShape->getBaseShape()->m_arTextRects[0];
				//double dkoefX = (double)lCoordSize / max(1, dCoordSizeX);
				//double dkoefY = (double)lCoordSize / max(1, dCoordSizeY);

				double _dWidth = ShapeSize;
				double _dHeight = ShapeSize;
				lpShapeElement->m_pShape->getBaseShape()->GetWidthHeightLogic(_dWidth, _dHeight);

                double dkoefX = (double)lCoordSize / (std::max)(1., _dWidth);
                double dkoefY = (double)lCoordSize / (std::max)(1., _dHeight);

                strRect =   std::to_wstring((int)(dkoefX * txRect.left))    + L"," +
                            std::to_wstring((int)(dkoefY * txRect.top))     + L"," +
                            std::to_wstring((int)(dkoefX * txRect.right))   + L"," +
                            std::to_wstring((int)(dkoefY * txRect.bottom));
			}
            if (lpShapeElement)
                delete lpShapeElement;
        }
	}
}

