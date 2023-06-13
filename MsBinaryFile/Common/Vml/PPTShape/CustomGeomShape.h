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
#include "BinaryReader.h"
#include "ElementSettings.h"
#include "../BaseShape.h"
//#include "../../../PptFile/Drawing/Attributes.h"
#include "PptFormula.h"
#include "../GraphicsPath.h"

namespace NSCustomVML
{
    class CSegment
    {
    public:
		ODRAW::RulesType m_eRuler;
        WORD m_nCount;

    public:
		CSegment(ODRAW::RulesType eType = ODRAW::rtMoveTo, WORD nCount = 2);
		CSegment(const CSegment& oSrc);
		CSegment& operator=(const CSegment& oSrc);
		~CSegment();

		int Read(WORD value); //from rtf segments
		void Read(POLE::Stream* pStream); //from binary ms segments
		void Read(ODRAW::CBinaryReader& oReader); //from binary ms segments
    };

    class CGuide
    {
    public:
		NSGuidesVML::FormulaType m_eType;

        BYTE m_param_type1;
        BYTE m_param_type2;
        BYTE m_param_type3;

        WORD m_param_value1;
        WORD m_param_value2;
        WORD m_param_value3;

        LONG m_lShapeWidth;
        LONG m_lShapeHeight;

		CGuide();
		CGuide(const CGuide& oSrc);
		CGuide& operator=(const CGuide& oSrc);

		void SetToFormula(NSGuidesVML::CFormula& oFormula);

		void Read(POLE::Stream* pStream);
		void Read(ODRAW::CBinaryReader& oReader);

    private:
		void SetParam(BYTE type, WORD param, ParamType& param_type, LONG& param_value);
    };

    class CCustomVML
    {
	public:
		ODRAW::RulesType m_ePath;

        std::vector<Aggplus::POINT>		m_arVertices;
        std::vector<CSegment>			m_arSegments;
        std::vector<CGuide>				m_arGuides;
        std::vector<long>*				m_pAdjustValues;
        std::vector<Aggplus::POINT>		m_arConnectionSites;
		std::vector<Aggplus::RECT>		m_arInscribe;
        std::vector<double>				m_arConnectionSitesDir;

        bool m_bIsVerticesPresent;
        bool m_bIsPathPresent;

		ODRAW::CBrush	m_oBrush;
		ODRAW::CPen		m_oPen;

    public:
		CCustomVML();
		CCustomVML(const CCustomVML& oSrc);
		CCustomVML& operator=(const CCustomVML& oSrc);
		~CCustomVML();

		bool IsCustom();
		void SetPath(ODRAW::RulesType ePath);

		void LoadVertices(std::vector<std::pair<int,int>> values);
		void LoadConnectionSitesDir(CProperty* pProperty);
		void LoadConnectionSites(CProperty* pProperty);
		void LoadVertices(CProperty* pProperty);
		void LoadAHs(CProperty* pProperty);
		void LoadSegments(std::vector<int> values);
		void LoadSegments(CProperty* pProperty);
		void LoadGuides(CProperty* pProperty);
		void LoadInscribe(CProperty* pProperty);
		void LoadAdjusts(LONG lIndex, LONG lValue);

		void addSegment(ODRAW::RulesType eRuler, _UINT16	nCount);
		void addGuide(CGuide & oInfo);
		void addAdjust(int lIndex, int lValue);

		void SetAdjusts(std::vector<long>* pList);
		void ToCustomShape(ODRAW::CBaseShape* pShape, NSGuidesVML::CFormulasManager& oManager);
	};
}
