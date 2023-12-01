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

#include "CalcPr.h"
#include "../../XlsbFormat/Biff12_records/CalcProp.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CCalcPr::CCalcPr()
		{
		}
		CCalcPr::~CCalcPr()
		{
		}
		void CCalcPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CCalcPr::toXML() const
		{
			return _T("");
		}
		void CCalcPr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<calcPr"));
			WritingStringNullableAttrInt(L"calcId", m_oCalcId, m_oCalcId->GetValue());
			WritingStringNullableAttrString(L"calcMode", m_oCalcMode, m_oCalcMode->ToString());
			WritingStringNullableAttrBool(L"fullCalcOnLoad", m_oFullCalcOnLoad);
			WritingStringNullableAttrString(L"refMode", m_oRefMode, m_oRefMode->ToString());
			WritingStringNullableAttrBool(L"iterate", m_oIterate);
			WritingStringNullableAttrInt(L"iterateCount", m_oIterateCount, m_oIterateCount->GetValue());
			WritingStringNullableAttrDouble(L"iterateDelta", m_oIterateDelta, m_oIterateDelta->GetValue());
			WritingStringNullableAttrBool(L"fullPrecision", m_oFullPrecision);
			WritingStringNullableAttrBool(L"calcCompleted", m_oCalcCompleted);
			WritingStringNullableAttrBool(L"calcOnSave", m_oCalcOnSave);
			WritingStringNullableAttrBool(L"concurrentCalc", m_oConcurrentCalc);
			WritingStringNullableAttrInt(L"concurrentManualCount", m_oConcurrentManualCount, m_oConcurrentManualCount->GetValue());
			WritingStringNullableAttrBool(L"forceFullCalc", m_oForceFullCalc);
			writer.WriteString(_T("/>"));
		}
		void CCalcPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CCalcPr::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		XLS::BaseObjectPtr CCalcPr::toBin()
		{
			auto ptr(new XLSB::CalcProp);
			XLS::BaseObjectPtr objectPtr(ptr);

            if(m_oCalcId.IsInit())
                ptr->recalcID = m_oCalcId->GetValue();
            else
                ptr->recalcID = 0;
            if(m_oCalcMode.IsInit())
                ptr->fAutoRecalc = m_oCalcMode->GetValue();
            else
                ptr->fAutoRecalc = 1;
            if(m_oFullCalcOnLoad.IsInit())
                ptr->fFullCalcOnLoad = m_oFullCalcOnLoad->GetValue();
            else
                ptr->fFullCalcOnLoad = false;
            if(m_oRefMode.IsInit())
                ptr->fRefA1 = !m_oRefMode->GetValue();
            if(m_oIterate.IsInit())
                ptr->fIter = m_oIterate->GetValue();
			else
				ptr->fIter = 0;
            if(m_oIterateCount.IsInit())
                ptr->cCalcCount = m_oIterateCount->GetValue();
			else
                ptr->cCalcCount = 0;
            if(m_oIterateDelta.IsInit())
                ptr->xnumDelta.data.value = m_oIterateDelta->GetValue();
			else
                ptr->xnumDelta.data.value = 0;
            if(m_oFullPrecision.IsInit())
                ptr->fFullPrec = m_oFullPrecision->GetValue();
			else
                ptr->fFullPrec = true;
            if(m_oCalcCompleted.IsInit())
                ptr->fSomeUncalced = m_oCalcCompleted->GetValue();
			else
				ptr->fSomeUncalced = false;
            if(m_oCalcOnSave.IsInit())
                ptr->fSaveRecalc = m_oCalcOnSave->GetValue();
			else
                ptr->fSaveRecalc = true;
            if(m_oConcurrentCalc.IsInit())
                ptr->fMTREnabled = m_oConcurrentCalc->GetValue();
			else
                ptr->fMTREnabled = true;
            if(m_oConcurrentManualCount.IsInit())
                ptr->cUserThreadCount = m_oConcurrentManualCount->GetValue();
			else
				ptr->cUserThreadCount = 1;
            if(m_oForceFullCalc.IsInit())
                ptr->fNoDeps = m_oForceFullCalc->GetValue();
			else
				ptr->fNoDeps = false;

			return objectPtr;
		}
		EElementType CCalcPr::getType () const
		{
			return et_x_CalcPr;
		}
		void CCalcPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )

			WritingElement_ReadAttributes_Read_if		( oReader, _T("calcId"),				m_oCalcId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("calcMode"),				m_oCalcMode )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("fullCalcOnLoad"),		m_oFullCalcOnLoad )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("refMode"),				m_oRefMode )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("iterate"),				m_oIterate )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("iterateCount"),			m_oIterateCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("iterateDelta"),			m_oIterateDelta )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("fullPrecision"),			m_oFullPrecision )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("calcCompleted"),			m_oCalcCompleted )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("calcOnSave"),			m_oCalcOnSave )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("concurrentCalc"),		m_oConcurrentCalc )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("concurrentManualCount"), m_oConcurrentManualCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("forceFullCalc"),			m_oForceFullCalc )

			WritingElement_ReadAttributes_End( oReader )
		}
		void CCalcPr::ReadAttributes(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::CalcProp*>(obj.get());
				m_oCalcId                   = ptr->recalcID;
				m_oCalcMode                 = (SimpleTypes::Spreadsheet::ECalcMode)ptr->fAutoRecalc;
				m_oFullCalcOnLoad           = ptr->fFullCalcOnLoad;
				m_oRefMode                  = (SimpleTypes::Spreadsheet::ERefMode)!ptr->fRefA1;
				m_oIterate                  = ptr->fIter;
				m_oIterateCount             = ptr->cCalcCount;
				m_oIterateDelta             = ptr->xnumDelta.data.value;
				m_oFullPrecision            = ptr->fFullPrec;
				m_oCalcCompleted            = ptr->fSomeUncalced;
				m_oCalcOnSave               = ptr->fSaveRecalc;
				m_oConcurrentCalc           = ptr->fMTREnabled;
				m_oConcurrentManualCount    = ptr->cUserThreadCount;
				m_oForceFullCalc            = ptr->fNoDeps;

			}

	} //Spreadsheet
} // namespace OOX
