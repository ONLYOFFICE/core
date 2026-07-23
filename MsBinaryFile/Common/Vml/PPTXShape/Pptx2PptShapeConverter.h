/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
