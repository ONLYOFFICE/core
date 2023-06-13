/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "PptxFormula.h"
#include "../PPTShape/Ppt2PptxShapeConverter.h"

namespace NSGuidesVML
{
    class CConverterPPTXPPT
    {
    private:
        std::vector<CSlice> m_arSlices;
        CPPTShape* pPPTShape;
        std::map<std::wstring, long> m_arMapFormula;
        std::map<std::wstring, long> m_arMapAdj;
        std::vector<CSlicePath> m_arSlicesPath;
        LONG m_lIndexDst;
        LONG m_lIndexAdj;
        SPointType pPointType1;
        SPointType pPointType2;
        SPointType pPointType3;
        Aggplus::POINT pPoint1;
        Aggplus::POINT pPoint2;
        Aggplus::POINT pPoint3;
        LONG m_lPPTX2PPT;
        LONG m_lPPT2PPTX;

    public:
		CConverterPPTXPPT();

        inline CPPTShape* GetConvertedShape()
        {
            return pPPTShape;
        }

		void Convert ( CPPTXShape* pPPTXShape );
		void ConvertHandles (std::vector<CHandle_>& arHnd);
		void ConvertAdjastments( std::vector<long> &arAdj, std::map<std::wstring, long> &mapAdj );
		void ConvertTextRects (std::vector<std::wstring> &arTextRects);
		void ConvertType (OOXMLShapes::ShapeType eType);

		void AddSizeGuides (LONG lWidth, LONG lHeight);

		LONG ConvertFmlaParam (std::wstring strParam, NSGuidesVML::ParamType &eType, std::wstring strKey, std::vector<NSGuidesOOXML::CFormula> &strGuides, std::map<std::wstring, long> &mapGuides);

		void ConvertFmla ( FormulaType eFmlaType, LONG lParam1, ParamType eType1, LONG lParam2, ParamType eType2, LONG lParam3, ParamType eType3 );

		void ConvertFmla ( FormulaType eFmlaType, LONG lParam1, ParamType eType1, LONG lParam2, ParamType eType2 );

		void ConvertFmla ( FormulaType eFmlaType, LONG lParam1, ParamType eType1 );

		void ConvertGuid ( NSGuidesOOXML::CFormula pFormula, std::wstring strKey, std::vector<NSGuidesOOXML::CFormula> &strGuides, std::map<std::wstring, long> &mapGuides);

		void ConvertGuides ( std::vector<NSGuidesOOXML::CFormula> &strGuides, std::map<std::wstring, long> &mapGuides );

		void ConvertPath(const std::wstring& xml, std::vector<NSGuidesOOXML::CFormula> &strGuides, std::map<std::wstring, long> &mapGuides);

		LONG ConvertArcParam (std::wstring strParam, NSGuidesVML::ParamType &eType);

		std::wstring ConvertPathPoint (std::wstring strX, std::wstring strY, bool &bNum, std::vector<NSGuidesOOXML::CFormula> &strGuides, std::map<std::wstring, long> &mapGuides, bool bLPoint);

		std::wstring ConvertPathParam (LONG lParam, ParamType eType, bool &bNum);
    };
}
