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
#include "Table.h"
#include "QueryTable.h"
#include "../../XlsbFormat/Xlsb.h"
#include "../../XlsbFormat/TableStream.h"
#include "../../XlsbFormat/Biff12_unions/LISTPARTS.h"
#include "../../XlsbFormat/Biff12_records/BeginListParts.h"
#include "../../XlsbFormat/Biff12_records/ListPart.h"
#include "../../XlsbFormat/Biff12_unions/TABLE.h"
#include "../../XlsbFormat/Biff12_records/BeginList.h"
#include "../../XlsbFormat/Biff12_records/TableStyleClient.h"
#include "../../XlsbFormat/Biff12_unions/LISTCOLS.h"
#include "../../XlsbFormat/Biff12_unions/LISTCOL.h"
#include "../../XlsbFormat/Biff12_records/BeginListCol.h"
#include "../../XlsbFormat/Biff12_records/ListCCFmla.h"
#include "../../XlsbFormat/Biff12_records/ListTrFmla.h"
#include "../../XlsbFormat/Biff12_records/List14.h"
#include "../../XlsbFormat/Biff12_unions/FRTTABLE.h"

#include "../../XlsbFormat/QueryTableStream.h"
#include "../../XlsbFormat/Biff12_unions/QSI.h"
#include "../../XlsbFormat/Biff12_records/CommonRecords.h"
#include "../../XlsbFormat/Biff12_unions/QSIR.h"
#include "../../XlsbFormat/Biff12_unions/QSIFS.h"
#include "../../XlsbFormat/Biff12_unions/QSIF.h"
#include "../../XlsbFormat/Biff12_unions/DELETEDNAMES.h"
#include "../../XlsbFormat/Biff12_unions/DELETEDNAME.h"
#include "../../XlsbFormat/Biff12_records/BeginDeletedName.h"

namespace OOX
{
namespace Spreadsheet
{
	void CAltTextTable::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<x14:table");
		WritingStringNullableAttrEncodeXmlString(L"altText", m_oAltText, m_oAltText.get());
		WritingStringNullableAttrEncodeXmlString(L"altTextSummary", m_oAltTextSummary, m_oAltTextSummary.get());
		writer.WriteString(L"/>");
	}
	void CAltTextTable::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
    void CAltTextTable::fromBin(XLS::BaseObjectPtr& obj)
    {
        ReadAttributes(obj);
    }
	void CAltTextTable::toBin(XLS::BaseObjectPtr& obj)
	{
		if (obj == nullptr)
			obj = XLS::BaseObjectPtr(new XLSB::List14());

		auto ptr = static_cast<XLSB::List14*>(obj.get());
		if (ptr != nullptr)
		{
			if (m_oAltText.IsInit())
				ptr->stAltText = m_oAltText.get();
			else
				ptr->stAltText = L"";

			if (m_oAltTextSummary.IsInit())
				ptr->stAltTextSummary = m_oAltTextSummary.get();
			else
				ptr->stAltTextSummary = L"";
		}
	}
	void CAltTextTable::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, (L"altText"),      m_oAltText )
			WritingElement_ReadAttributes_Read_if     ( oReader, (L"altTextSummary"),      m_oAltTextSummary )
		WritingElement_ReadAttributes_End( oReader )
	}
    void CAltTextTable::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::List14*>(obj.get());
        if(ptr != nullptr)
        {
            m_oAltText              = ptr->stAltText.value();
            m_oAltTextSummary       = ptr->stAltTextSummary.value();
        }
    }

	void CTableStyleInfo::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		int nShowColumnStripes = 0;
		int nShowFirstColumn = 0;
		int nShowLastColumn = 0;
		int nShowRowStripes = 0;
		if(m_oShowColumnStripes.IsInit() && true == m_oShowColumnStripes->ToBool())
			nShowColumnStripes = 1;
		if(m_oShowFirstColumn.IsInit() && true == m_oShowFirstColumn->ToBool())
			nShowFirstColumn = 1;
		if(m_oShowLastColumn.IsInit() && true == m_oShowLastColumn->ToBool())
			nShowLastColumn = 1;
		if(m_oShowRowStripes.IsInit() && true == m_oShowRowStripes->ToBool())
			nShowRowStripes = 1;

		writer.WriteString(L"<tableStyleInfo");
		WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
		WritingStringAttrInt(L"showFirstColumn", nShowFirstColumn);
		WritingStringAttrInt(L"showLastColumn", nShowLastColumn);
		WritingStringAttrInt(L"showRowStripes", nShowRowStripes);
		WritingStringAttrInt(L"showColumnStripes", nShowColumnStripes);
		writer.WriteString(L"/>");
	}
	void CTableStyleInfo::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
    void CTableStyleInfo::fromBin(XLS::BaseObjectPtr& obj)
    {
        ReadAttributes(obj);
    }
	void CTableStyleInfo::toBin(XLS::BaseObjectPtr& obj)
	{
		if (obj == nullptr)
			obj = XLS::BaseObjectPtr(new XLSB::TableStyleClient());

		auto ptr = static_cast<XLSB::TableStyleClient*>(obj.get());
		if (ptr != nullptr)
		{
			if (m_oName.IsInit())
				ptr->stStyleName = m_oName.get();
			else
				ptr->stStyleName = L"";

			if (m_oShowColumnStripes.IsInit())
				ptr->fColumnStripes = m_oShowColumnStripes->GetValue();
			else
				ptr->fColumnStripes = false;

			if (m_oShowFirstColumn.IsInit())
				ptr->fFirstColumn = m_oShowFirstColumn->GetValue();
			else
				ptr->fFirstColumn = false;

			if (m_oShowLastColumn.IsInit())
				ptr->fLastColumn = m_oShowLastColumn->GetValue();
			else
				ptr->fLastColumn = false;

			if (m_oShowRowStripes.IsInit())
				ptr->fRowStripes = m_oShowRowStripes->GetValue();
			else
				ptr->fRowStripes = false;

		}
	}
    void CTableStyleInfo::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::TableStyleClient*>(obj.get());
        if(ptr != nullptr)
        {
			if(!ptr->stStyleName.value().empty())
				m_oName               = ptr->stStyleName.value();

            m_oShowColumnStripes    = ptr->fColumnStripes;
            m_oShowFirstColumn      = ptr->fFirstColumn;
            m_oShowLastColumn       = ptr->fLastColumn;
            m_oShowRowStripes       = ptr->fRowStripes;
        }
    }
	void CTableStyleInfo::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )

			WritingElement_ReadAttributes_Read_if     ( oReader, (L"name"),				m_oName )
			WritingElement_ReadAttributes_Read_if     ( oReader, (L"showColumnStripes"),	m_oShowColumnStripes )
			WritingElement_ReadAttributes_Read_if     ( oReader, (L"showFirstColumn"),		m_oShowFirstColumn )
			WritingElement_ReadAttributes_Read_if     ( oReader, (L"showLastColumn"),		m_oShowLastColumn )
			WritingElement_ReadAttributes_Read_if     ( oReader, (L"showRowStripes"),		m_oShowRowStripes )

		WritingElement_ReadAttributes_End( oReader )
	}
	void CTableColumn::toXML(NSStringUtils::CStringBuilder& writer) const
	{
        std::wstring sRoot;
		writer.WriteString(L"<tableColumn");
		WritingStringNullableAttrInt(L"id", m_oId, m_oId->GetValue());
		WritingStringNullableAttrEncodeXmlString(L"name", m_oName, *m_oName);
		WritingStringNullableAttrEncodeXmlString(L"uniqueName", m_oUniqueName, *m_oUniqueName);
		WritingStringNullableAttrEncodeXmlString(L"totalsRowLabel", m_oTotalsRowLabel, m_oTotalsRowLabel.get());
		//есть такой баг: при сохранениии "sum" и названия таблицы "Table1" (русский excel), выдается ошибка в формулах
		WritingStringNullableAttrString(L"totalsRowFunction", m_oTotalsRowFunction, m_oTotalsRowFunction->ToString());
		WritingStringNullableAttrInt(L"queryTableFieldId", m_oQueryTableFieldId, m_oQueryTableFieldId->GetValue());
		WritingStringNullableAttrString(L"dataCellStyle", m_oDataCellStyle, *m_oDataCellStyle);		
		WritingStringNullableAttrInt(L"dataDxfId", m_oDataDxfId, m_oDataDxfId->GetValue());
		WritingStringNullableAttrString(L"headerRowCellStyle", m_oHeaderRowCellStyle, *m_oHeaderRowCellStyle);		
		WritingStringNullableAttrInt(L"headerRowDxfId", m_oHeaderRowDxfId, m_oHeaderRowDxfId->GetValue());
		WritingStringNullableAttrString(L"totalsRowCellStyle", m_oTotalsRowCellStyle, *m_oTotalsRowCellStyle);		
		WritingStringNullableAttrInt(L"totalsRowDxfId", m_oTotalsRowDxfId, m_oTotalsRowDxfId->GetValue());
		if(m_oTotalsRowFormula.IsInit() || m_oCalculatedColumnFormula.IsInit())
		{
			writer.WriteString(L">");

			if(m_oCalculatedColumnFormula.IsInit())
			{
				WritingStringValEncodeXmlString(L"calculatedColumnFormula", m_oCalculatedColumnFormula.get());
			}
			if(m_oTotalsRowFormula.IsInit())
			{
				WritingStringValEncodeXmlString(L"totalsRowFormula", m_oTotalsRowFormula.get());
			}

			writer.WriteString(L"</tableColumn>");
		}
		else
		{
			writer.WriteString(L"/>");
		}
	}
	void CTableColumn::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( (L"totalsRowFormula") == sName )
				m_oTotalsRowFormula = oReader.GetText3();
			else if ( (L"calculatedColumnFormula") == sName )
				m_oCalculatedColumnFormula = oReader.GetText3();
		}
	}
    void CTableColumn::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::LISTCOL*>(obj.get());
        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginListCol);

            if(ptr->m_BrtListCCFmla != nullptr)
            {
               auto ptrCCFmla = static_cast<XLSB::ListCCFmla*>(ptr->m_BrtListCCFmla.get());
               if(ptrCCFmla->fArray)
                   m_oCalculatedColumnFormula = ptrCCFmla->arrayFormula.getAssembledFormula();
               else
                   m_oCalculatedColumnFormula = ptrCCFmla->formula.getAssembledFormula();
            }

            if(ptr->m_BrtListTrFmla != nullptr)
            {
                auto ptrListTrFmla = static_cast<XLSB::ListTrFmla*>(ptr->m_BrtListTrFmla.get());
                if(ptrListTrFmla->fArray)
                    m_oTotalsRowFormula = ptrListTrFmla->arrayFormula.getAssembledFormula();
                else
                    m_oTotalsRowFormula = ptrListTrFmla->formula.getAssembledFormula();
            }
        }
    }
	void CTableColumn::toBin(XLS::BaseObjectPtr& obj)
	{
		auto ptr = static_cast<XLSB::LISTCOL*>(obj.get());
		if (ptr != nullptr)
		{
			WriteAttributes(ptr->m_BrtBeginListCol);

			if (m_oCalculatedColumnFormula.IsInit())
			{
				if (ptr->m_BrtListCCFmla == nullptr)
					ptr->m_BrtListCCFmla = XLS::BaseObjectPtr(new XLSB::ListCCFmla());

				auto ptrCCFmla = static_cast<XLSB::ListCCFmla*>(ptr->m_BrtListCCFmla.get());
				if (ptrCCFmla != nullptr)
				{
					ptrCCFmla->fArray = false; //////?
					ptrCCFmla->formula = m_oCalculatedColumnFormula.get();
				}
			}

			if (m_oTotalsRowFormula.IsInit())
			{
				if (ptr->m_BrtListTrFmla == nullptr)
					ptr->m_BrtListTrFmla = XLS::BaseObjectPtr(new XLSB::ListTrFmla());

				auto ptrListTrFmla = static_cast<XLSB::ListTrFmla*>(ptr->m_BrtListTrFmla.get());
				if (ptrListTrFmla != nullptr)
				{
					ptrListTrFmla->fArray = false; //////?
					ptrListTrFmla->formula = m_oTotalsRowFormula.get();
				}
			}

		}
	}
    void CTableColumn::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginListCol*>(obj.get());
        if(ptr != nullptr)
        {
            if(!ptr->stStyleInsertRow.value().empty())
                m_oDataCellStyle        = ptr->stStyleInsertRow.value();

            if(ptr->nDxfInsertRow != 0xFFFFFFFF)
                m_oTotalsRowDxfId       = ptr->nDxfInsertRow;

            if(!ptr->stStyleHeader.value().empty())
                m_oHeaderRowCellStyle   = ptr->stStyleHeader.value();

            if(ptr->nDxfHdr != 0xFFFFFFFF)
                m_oHeaderRowDxfId       = ptr->nDxfHdr;

            if(!ptr->stStyleAgg.value().empty())
                m_oTotalsRowCellStyle   = ptr->stStyleAgg.value();

            if(ptr->nDxfAgg != 0xFFFFFFFF)
                m_oDataDxfId            = ptr->nDxfAgg;

            m_oId                       = ptr->idField;

            if(!ptr->stCaption.value().empty())
                m_oName                 = ptr->stCaption.value();

            m_oQueryTableFieldId        = ptr->idqsif;

            if(!ptr->stTotal.value().empty())
                m_oTotalsRowLabel       = ptr->stTotal.value();

            if(!ptr->stName.value().empty())
                m_oUniqueName           = ptr->stName.value();

            switch (ptr->ilta.value().get())
            {
                //case XLSB::ListTotalRowFunction::ILTA_NONE:
                    //m_oTotalsRowFunction = SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionNone; break;
                case XLSB::ListTotalRowFunction::ILTA_AVERAGE:
                    m_oTotalsRowFunction = SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionAverage; break;
                case XLSB::ListTotalRowFunction::ILTA_COUNT:
                    m_oTotalsRowFunction = SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionCount; break;
                case XLSB::ListTotalRowFunction::ILTA_COUNTNUMS:
                    m_oTotalsRowFunction = SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionCountNums; break;
                case XLSB::ListTotalRowFunction::ILTA_MAX:
                    m_oTotalsRowFunction = SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionMax; break;
                case XLSB::ListTotalRowFunction::ILTA_MIN:
                    m_oTotalsRowFunction = SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionMin; break;
                case XLSB::ListTotalRowFunction::ILTA_SUM:
                    m_oTotalsRowFunction = SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionSum; break;
                case XLSB::ListTotalRowFunction::ILTA_STDDEV:
                    m_oTotalsRowFunction = SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionStdDev; break;
                case XLSB::ListTotalRowFunction::ILTA_VAR:
                    m_oTotalsRowFunction = SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionVar; break;
                case XLSB::ListTotalRowFunction::ILTA_CUSTOM:
                    m_oTotalsRowFunction = SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionCustom; break;
            }
        }
    }
	void CTableColumn::WriteAttributes(XLS::BaseObjectPtr& obj)
	{
		if (obj == nullptr)
			obj = XLS::BaseObjectPtr(new XLSB::BeginListCol());

		auto ptr = static_cast<XLSB::BeginListCol*>(obj.get());
		if (ptr != nullptr)
		{
			if (m_oDataCellStyle.IsInit())
				ptr->stStyleInsertRow = m_oDataCellStyle.get();
			else
				ptr->stStyleInsertRow = L"";

			if (m_oTotalsRowDxfId.IsInit())
				ptr->nDxfInsertRow = m_oTotalsRowDxfId->GetValue();
			else
				ptr->nDxfInsertRow = 0xFFFFFFFF;

			if (m_oHeaderRowCellStyle.IsInit())
				ptr->stStyleHeader = m_oHeaderRowCellStyle.get();
			else
				ptr->stStyleHeader = L"";

			if (m_oHeaderRowDxfId.IsInit())
				ptr->nDxfHdr = m_oHeaderRowDxfId->GetValue();
			else
				ptr->nDxfHdr = 0xFFFFFFFF;

			if (m_oTotalsRowCellStyle.IsInit())
				ptr->stStyleAgg = m_oTotalsRowCellStyle.get();
			else
				ptr->stStyleAgg = L"";

			if (m_oDataDxfId.IsInit())
				ptr->nDxfAgg = m_oDataDxfId->GetValue();
			else
				ptr->nDxfAgg = 0xFFFFFFFF;

			if (m_oId.IsInit())
				ptr->idField = m_oId->GetValue();
			else
				ptr->idField = 0;

			if (m_oName.IsInit())
				ptr->stCaption = m_oName.get();
			else
				ptr->stCaption = L"";

			if (m_oQueryTableFieldId.IsInit())
				ptr->idqsif = m_oQueryTableFieldId->GetValue();
			else
				ptr->idqsif = 0;

			if (m_oTotalsRowLabel.IsInit())
				ptr->stTotal = m_oTotalsRowLabel.get();
			else
				ptr->stTotal = L"";

			if (m_oUniqueName.IsInit())
				ptr->stName = m_oUniqueName.get();
			else
				ptr->stName = L"";

			if (m_oTotalsRowFunction.IsInit())
			{
				switch (m_oTotalsRowFunction->GetValue())
				{
				case SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionNone:
					ptr->ilta = XLSB::ListTotalRowFunction::ILTA_NONE; break;
				case SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionAverage:
					ptr->ilta = XLSB::ListTotalRowFunction::ILTA_AVERAGE; break;
				case SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionCount:
					ptr->ilta = XLSB::ListTotalRowFunction::ILTA_COUNT; break;
				case SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionCountNums:
					ptr->ilta = XLSB::ListTotalRowFunction::ILTA_COUNTNUMS; break;
				case SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionMax:
					ptr->ilta = XLSB::ListTotalRowFunction::ILTA_MAX; break;
				case SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionMin:
					ptr->ilta = XLSB::ListTotalRowFunction::ILTA_MIN; break;
				case SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionSum:
					ptr->ilta = XLSB::ListTotalRowFunction::ILTA_SUM; break;
				case SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionStdDev:
					ptr->ilta = XLSB::ListTotalRowFunction::ILTA_STDDEV; break;
				case SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionVar:
					ptr->ilta = XLSB::ListTotalRowFunction::ILTA_VAR; break;
				case SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionCustom:
					ptr->ilta = XLSB::ListTotalRowFunction::ILTA_CUSTOM; break;
				}
			}
			else
				ptr->ilta = XLSB::ListTotalRowFunction::ILTA_NONE;			
		}
	}
	void CTableColumn::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"dataCellStyle",		m_oDataCellStyle )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"dataDxfId",			m_oDataDxfId )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"headerRowCellStyle",	m_oHeaderRowCellStyle )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"headerRowDxfId",		m_oHeaderRowDxfId )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"id",					m_oId )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"name",				m_oName )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"queryTableFieldId",	m_oQueryTableFieldId )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"totalsRowCellStyle",	m_oTotalsRowCellStyle )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"totalsRowDxfId",		m_oTotalsRowDxfId )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"totalsRowLabel",		m_oTotalsRowLabel )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"totalsRowFunction",	m_oTotalsRowFunction )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"uniqueName",			m_oUniqueName )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"uid",				m_oUid )
		WritingElement_ReadAttributes_End( oReader )
	}
	void CTableColumns::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<tableColumns");
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</tableColumns>");
	}
	void CTableColumns::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( (L"tableColumn") == sName )
				m_arrItems.push_back(new CTableColumn(oReader));
		}
	}
    void CTableColumns::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::LISTCOLS*>(obj.get());
        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_arLISTCOL);

            for(auto &listcol : ptr->m_arLISTCOL)
                m_arrItems.push_back(new CTableColumn(listcol));
        }
    }
	void CTableColumns::toBin(XLS::BaseObjectPtr& obj)
	{
		if (obj == nullptr)
			obj = XLS::BaseObjectPtr(new XLSB::LISTCOLS());

		auto ptr = static_cast<XLSB::LISTCOLS*>(obj.get());
		if (ptr != nullptr)
		{
			ptr->m_arLISTCOL.reserve(m_arrItems.size());
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					XLS::BaseObjectPtr item(new XLSB::LISTCOL());
					m_arrItems[i]->toBin(item);
					ptr->m_arLISTCOL.push_back(item);
				}
			}
		}
	}
    void CTableColumns::ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj)
    {
        if(!obj.empty())
            m_oCount = (int)obj.size();
    }
	void CTableColumns::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
            WritingElement_ReadAttributes_Read_if ( oReader, (L"count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}

	void CTable::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<table \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"xr xr3\" \
xmlns:xr=\"http://schemas.microsoft.com/office/spreadsheetml/2014/revision\" \
xmlns:xr3=\"http://schemas.microsoft.com/office/spreadsheetml/2016/revision3\"");

		WritingStringNullableAttrInt(L"id", m_oId, m_oId->GetValue());
		WritingStringNullableAttrEncodeXmlString(L"name", m_oName, *m_oName);
		WritingStringNullableAttrEncodeXmlString(L"displayName", m_oDisplayName, *m_oDisplayName);
		WritingStringNullableAttrString(L"ref", m_oRef, m_oRef->ToString());
		WritingStringNullableAttrInt(L"connectionId", m_oConnectionId, m_oConnectionId->GetValue());
		WritingStringNullableAttrString(L"tableType", m_oTableType, m_oTableType->ToString());
		WritingStringNullableAttrString(L"comment", m_oComment, *m_oComment);
		WritingStringNullableAttrInt(L"tableBorderDxfId", m_oTableBorderDxfId, m_oTableBorderDxfId->GetValue());
		WritingStringNullableAttrString(L"dataCellStyle", m_oDataCellStyle, *m_oDataCellStyle);
		WritingStringNullableAttrInt(L"dataDxfId", m_oDataDxfId, m_oDataDxfId->GetValue());
		WritingStringNullableAttrString(L"headerRowCellStyle", m_oHeaderRowCellStyle, *m_oHeaderRowCellStyle);
		WritingStringNullableAttrInt(L"headerRowBorderDxfId", m_oHeaderRowBorderDxfId, m_oHeaderRowBorderDxfId->GetValue());
		WritingStringNullableAttrInt(L"headerRowDxfId", m_oHeaderRowDxfId, m_oHeaderRowDxfId->GetValue());
		WritingStringNullableAttrString(L"totalsRowCellStyle", m_oTotalsRowCellStyle, *m_oTotalsRowCellStyle);
		WritingStringNullableAttrInt(L"totalsRowDxfId", m_oTotalsRowDxfId, m_oTotalsRowDxfId->GetValue());
		WritingStringNullableAttrInt(L"totalsRowBorderDxfId", m_oTotalsRowBorderDxfId, m_oTotalsRowBorderDxfId->GetValue());
		
		//if (m_oHeaderRowCount.IsInit() && 0 == m_oHeaderRowCount->GetValue())
		//	WritingStringAttrString(L"headerRowCount", L"1");
		//if (m_oTotalsRowCount.IsInit() && m_oTotalsRowCount->GetValue() > 0)
  //          WritingStringAttrString(L"totalsRowCount", L"1");
  //      else
		//	WritingStringAttrString(L"totalsRowShown", L"0");//m_oTotalsRowShown		
		WritingStringNullableAttrInt(L"headerRowCount", m_oHeaderRowCount, m_oHeaderRowCount->GetValue());
		WritingStringNullableAttrInt(L"totalsRowCount", m_oTotalsRowCount, m_oTotalsRowCount->GetValue());
		WritingStringNullableAttrBool2(L"totalsRowShown", m_oTotalsRowShown);
        
		WritingStringNullableAttrBool2(L"insertRow", m_oInsertRow);
		WritingStringNullableAttrBool2(L"insertRowShift", m_oInsertRowShift);
 		WritingStringNullableAttrBool2(L"published", m_oPublished);

		writer.WriteString(L">");

		if(m_oAutoFilter.IsInit())
			m_oAutoFilter->toXML(writer);
		if(m_oSortState.IsInit())
			m_oSortState->toXML(writer);
		if(m_oTableColumns.IsInit())
			m_oTableColumns->toXML(writer);
		if(m_oTableStyleInfo.IsInit())
			m_oTableStyleInfo->toXML(writer);
		if(m_oExtLst.IsInit())
		{
			writer.WriteString(m_oExtLst->toXMLWithNS(L""));
		}
		writer.WriteString(L"</table>");
	}
	void CTable::toXML2(NSStringUtils::CStringBuilder& writer, int nIndex)
	{
		if(false == m_oRef.IsInit() || false == m_oDisplayName.IsInit()) return;
		
        if(!m_oId.IsInit())
        {
            m_oId.Init();
            m_oId->SetValue((unsigned int)nIndex);
        }

		toXML(writer);
	}
	void CTable::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( (L"autoFilter") == sName )
				m_oAutoFilter = oReader;
			else if ( (L"sortState") == sName )
				m_oSortState = oReader;
			else if ( (L"tableColumns") == sName )
				m_oTableColumns = oReader;
			else if ( (L"tableStyleInfo") == sName )
				m_oTableStyleInfo = oReader;
			else if ((L"extLst") == sName)
				m_oExtLst = oReader;
		}
	}

    void CTable::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::TABLE*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginList);

            if(ptr->m_AUTOFILTER != nullptr)
                m_oAutoFilter = ptr->m_AUTOFILTER;

            if(ptr->m_SORTSTATE != nullptr)
                m_oSortState = ptr->m_SORTSTATE;

            if(ptr->m_LISTCOLS != nullptr)
                m_oTableColumns = ptr->m_LISTCOLS;

            if(ptr->m_BrtTableStyleClient != nullptr)
                m_oTableStyleInfo = ptr->m_BrtTableStyleClient;

            if(ptr->m_FRTTABLE != nullptr)
                m_oExtLst = ptr->m_FRTTABLE;
        }
    }
	void CTable::toBin(XLS::BaseObjectPtr& obj)
	{
		if (obj == nullptr)
			obj = XLS::BaseObjectPtr(new XLSB::TABLE());

		auto ptr = static_cast<XLSB::TABLE*>(obj.get());
		if (ptr != nullptr)
		{
			WriteAttributes(ptr->m_BrtBeginList);

			if (m_oAutoFilter.IsInit())
				m_oAutoFilter->toBin(ptr->m_AUTOFILTER);

			if (m_oSortState.IsInit())
				m_oSortState->toBin(ptr->m_SORTSTATE);

			if (m_oTableColumns.IsInit())
				m_oTableColumns->toBin(ptr->m_LISTCOLS);

			if (m_oTableStyleInfo.IsInit())
				m_oTableStyleInfo->toBin(ptr->m_BrtTableStyleClient);

			if (m_oExtLst.IsInit())
			{
				ptr->m_FRTTABLE = XLS::BaseObjectPtr(new XLSB::FRTTABLE());
				m_oExtLst->toBin(ptr->m_FRTTABLE);
			}
		}
	}
	void CTable::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"ref",					m_oRef )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"name",					m_oName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"headerRowCount",		m_oHeaderRowCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"totalsRowCount",		m_oTotalsRowCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"displayName",			m_oDisplayName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"tableBorderDxfId",		m_oTableBorderDxfId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"comment",				m_oComment )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"connectionId",			m_oConnectionId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dataDxfId",			m_oDataDxfId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dataCellStyle",		m_oDataCellStyle ) 
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"headerRowBorderDxfId",	m_oHeaderRowBorderDxfId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"headerRowCellStyle",	m_oHeaderRowCellStyle )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"headerRowDxfId",		m_oHeaderRowDxfId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"insertRow",			m_oInsertRow )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"insertRowShift",		m_oInsertRowShift )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"published",			m_oPublished )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"id",					m_oId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"tableType",			m_oTableType )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"totalsRowBorderDxfId",	m_oTotalsRowBorderDxfId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"totalsRowCellStyle",	m_oTotalsRowCellStyle )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"totalsRowDxfId",		m_oTotalsRowDxfId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"totalsRowShown",		m_oTotalsRowShown )
		WritingElement_ReadAttributes_End( oReader )
	}

    void CTable::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginList*>(obj.get());
        if(ptr != nullptr)
        {
            m_oRef                      = ptr->rfxList.toString();

            if(!ptr->stName.value().empty())
                m_oName                 = ptr->stName.value();

            if ((bool)ptr->crwHeader)
            {
                if ((bool)ptr->crwTotals)
                    m_oTotalsRowCount = ptr->crwTotals.value().get();
            }
            else
            {
                if ((bool)ptr->crwTotals)
                {
                    m_oHeaderRowCount = ptr->crwHeader.value().get();
                    m_oTotalsRowCount = ptr->crwTotals.value().get();
                }
                else
                    m_oHeaderRowCount = ptr->crwHeader.value().get();
            }

            if(!ptr->stDisplayName.value().empty())
                m_oDisplayName          = ptr->stDisplayName.value();

            if(ptr->nDxfBorder != 0xFFFFFFFF)
                m_oTableBorderDxfId     = ptr->nDxfBorder;

            if(!ptr->stComment.value().empty())
                m_oComment              = ptr->stComment.value();

            m_oConnectionId             = ptr->dwConnID;

            if(ptr->nDxfData != 0xFFFFFFFF)
                m_oDataDxfId            = ptr->nDxfData;

            if(!ptr->stStyleData.value().empty())
                m_oDataCellStyle        = ptr->stStyleData.value();

            if(ptr->nDxfHeaderBorder != 0xFFFFFFFF)
                m_oHeaderRowBorderDxfId = ptr->nDxfHeaderBorder;

            if(!ptr->stStyleHeader.value().empty())
                m_oHeaderRowCellStyle   = ptr->stStyleHeader.value();

            if(ptr->nDxfHeader != 0xFFFFFFFF)
                m_oHeaderRowDxfId       = ptr->nDxfHeader;

			if(ptr->fForceInsertToBeVisible != false)
				m_oInsertRow                = ptr->fForceInsertToBeVisible;

			if (ptr->fInsertRowInsCells != false)
				m_oInsertRowShift           = ptr->fInsertRowInsCells;

			if (ptr->fPublished != false)
				m_oPublished                = ptr->fPublished;

            m_oId                       = ptr->idList;

            switch (ptr->lt.value().get())
            {
                //case XLSB::ListType::LTRANGE:
                    //m_oTableType = SimpleTypes::Spreadsheet::ETableType::typeWorksheet; break;
                case XLSB::ListType::LTXML:
                    m_oTableType = SimpleTypes::Spreadsheet::ETableType::typeXml; break;
                case XLSB::ListType::LTEXTDATA:
                    m_oTableType = SimpleTypes::Spreadsheet::ETableType::typeQueryTable; break;
            }

            if(ptr->nDxfAggBorder != 0xFFFFFFFF)
                m_oTotalsRowBorderDxfId = ptr->nDxfAggBorder;

            if(!ptr->stStyleAgg.value().empty())
                m_oTotalsRowCellStyle   = ptr->stStyleAgg.value();

            if(ptr->nDxfAgg != 0xFFFFFFFF)
                m_oTotalsRowDxfId       = ptr->nDxfAgg;

			if(ptr->fShownTotalRow != true)
				m_oTotalsRowShown           = ptr->fShownTotalRow;

        }
    }
	void CTable::WriteAttributes(XLS::BaseObjectPtr& obj)
	{
		if (obj == nullptr)
			obj = XLS::BaseObjectPtr(new XLSB::BeginList());

		auto ptr = static_cast<XLSB::BeginList*>(obj.get());
		if (ptr != nullptr)
		{
			if (m_oRef.IsInit())
				ptr->rfxList = m_oRef->GetValue();
			else
				ptr->rfxList = std::wstring(L"");

			if (m_oName.IsInit())
				ptr->stName = m_oName.get();
			else
				ptr->stName = L"";			

			if (m_oTotalsRowCount.IsInit())
				ptr->crwTotals = m_oTotalsRowCount->GetValue();
			else
				ptr->crwTotals = 0;

			if (m_oHeaderRowCount.IsInit())
				ptr->crwHeader = m_oHeaderRowCount->GetValue();
			else
				ptr->crwHeader = 1;			

			if (m_oDisplayName.IsInit())
				ptr->stDisplayName = m_oDisplayName.get();
			else
				ptr->stDisplayName = L"";			

			if (m_oTableBorderDxfId.IsInit())
				ptr->nDxfBorder = m_oTableBorderDxfId->GetValue();
			else
				ptr->nDxfBorder = 0xFFFFFFFF;

			if (m_oComment.IsInit())
				ptr->stComment = m_oComment.get();
			else
				ptr->stComment = L"";

			if (m_oConnectionId.IsInit())
				ptr->dwConnID = m_oConnectionId->GetValue();
			else
				ptr->dwConnID = 0;

			if (m_oDataDxfId.IsInit())
				ptr->nDxfData = m_oDataDxfId->GetValue();
			else
				ptr->nDxfData = 0xFFFFFFFF;

			if (m_oDataCellStyle.IsInit())
				ptr->stStyleData = m_oDataCellStyle.get();
			else
				ptr->stStyleData = L"";

			if (m_oHeaderRowBorderDxfId.IsInit())
				ptr->nDxfHeaderBorder = m_oHeaderRowBorderDxfId->GetValue();
			else
				ptr->nDxfHeaderBorder = 0xFFFFFFFF;

			if (m_oHeaderRowCellStyle.IsInit())
				ptr->stStyleHeader = m_oHeaderRowCellStyle.get();
			else
				ptr->stStyleHeader = L"";

			if (m_oHeaderRowDxfId.IsInit())
				ptr->nDxfHeader = m_oHeaderRowDxfId->GetValue();
			else
				ptr->nDxfHeader = 0xFFFFFFFF;

			if (m_oInsertRow.IsInit())
				ptr->fForceInsertToBeVisible = m_oInsertRow.get();
			else
				ptr->fForceInsertToBeVisible = false;

			if (m_oInsertRowShift.IsInit())
				ptr->fInsertRowInsCells = m_oInsertRowShift.get();
			else
				ptr->fInsertRowInsCells = false;

			if (m_oPublished.IsInit())
				ptr->fPublished = m_oPublished.get();
			else
				ptr->fPublished = false;

			if (m_oId.IsInit())
				ptr->idList = m_oId->GetValue();
			else
				ptr->idList = 0;

			if (m_oTableType.IsInit())
			{
				switch (m_oTableType->GetValue())
				{
				case SimpleTypes::Spreadsheet::ETableType::typeWorksheet:
					ptr->lt = XLSB::ListType::LTRANGE; break;
				case SimpleTypes::Spreadsheet::ETableType::typeXml:
					ptr->lt = XLSB::ListType::LTXML; break;
				case SimpleTypes::Spreadsheet::ETableType::typeQueryTable:
					ptr->lt = XLSB::ListType::LTEXTDATA; break;
				}
			}
			else
				ptr->lt = XLSB::ListType::LTRANGE;

			if (m_oTotalsRowBorderDxfId.IsInit())
				ptr->nDxfAggBorder = m_oTotalsRowBorderDxfId->GetValue();
			else
				ptr->nDxfAggBorder = 0xFFFFFFFF;

			if (m_oTotalsRowCellStyle.IsInit())
				ptr->stStyleAgg = m_oTotalsRowCellStyle.get();
			else
				ptr->stStyleAgg = L"";

			if (m_oTotalsRowDxfId.IsInit())
				ptr->nDxfAgg = m_oTotalsRowDxfId->GetValue();
			else
				ptr->nDxfAgg = 0xFFFFFFFF;

			if (m_oTotalsRowShown.IsInit())
				ptr->fShownTotalRow = m_oTotalsRowShown.get();
			else
				ptr->fShownTotalRow = true;
		}
	}

	void CTablePart::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(m_oRId.IsInit())
		{
			writer.WriteString(L"<tablePart");
			WritingStringAttrString(L"r:id", m_oRId->ToString());
			writer.WriteString(L"/>");
		}
	}
	void CTablePart::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
    void CTablePart::fromBin(XLS::BaseObjectPtr& obj)
    {
        ReadAttributes(obj);
    }
	void CTablePart::toBin(XLS::BaseObjectPtr& obj)
	{
		if (obj == nullptr)
			obj = XLS::BaseObjectPtr(new XLSB::ListPart());

		auto ptr = static_cast<XLSB::ListPart*>(obj.get());
		if (ptr != nullptr)
		{		
			if (m_oRId.IsInit())
				ptr->stRelID = m_oRId->GetValue();
			else
				ptr->stRelID = std::wstring(L"");
		}
	}
	void CTablePart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, L"id", m_oRId )
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
    void CTablePart::ReadAttributes(XLS::BaseObjectPtr &obj)
    {
        auto ptr = static_cast<XLSB::ListPart*>(obj.get());
        if(ptr != nullptr)
			if(!ptr->stRelID.value.value().empty())
				m_oRId = ptr->stRelID.value.value();
    }

	void CTableParts::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<tableParts");
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if ( m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</tableParts>");	
	}
	void CTableParts::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( (L"tablePart") == sName )
				m_arrItems.push_back(new CTablePart(oReader));
		}
	}

    void CTableParts::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::LISTPARTS*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginListParts);

            for(auto &tablePart : ptr->m_arBrtListPart)
            {
                m_arrItems.push_back(new CTablePart(tablePart));
            }
        }
    }

	void CTableParts::toBin(XLS::BaseObjectPtr& obj)
	{
		if (obj == nullptr)
			obj = XLS::BaseObjectPtr(new XLSB::LISTPARTS());

		auto pLISTPARTS = static_cast<XLSB::LISTPARTS*>(obj.get());
		if (pLISTPARTS != nullptr)
		{
			pLISTPARTS->m_arBrtListPart.reserve(m_arrItems.size());
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					XLS::BaseObjectPtr item(new XLSB::ListPart());
					m_arrItems[i]->toBin(item);
					pLISTPARTS->m_arBrtListPart.push_back(item);
				}
			}
		}
	}

	void CTableParts::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, (L"count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}

    void CTableParts::ReadAttributes(XLS::BaseObjectPtr &obj)
    {
        auto ptr = static_cast<XLSB::BeginListParts*>(obj.get());
        if(ptr != nullptr)
            m_oCount = ptr->cParts;
    }

    void CTableFile::readBin(const CPath& oPath)
    {
        CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
        if (xlsb)
        {
            XLSB::TableStreamPtr tableStream(new XLSB::TableStream);

            xlsb->ReadBin(oPath, tableStream.get());

            if (tableStream != nullptr)
            {
                if (tableStream->m_TABLE != nullptr)
                    m_oTable = tableStream->m_TABLE;
            }
        }
    }

	void CTableFile::writeBin(const CPath& oPath) const
	{
		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
		if (xlsb)
		{
			XLSB::TableStreamPtr tableStream(new XLSB::TableStream);

			if (tableStream != nullptr)
			{		
				if (m_oTable.IsInit())
				{
					tableStream->m_TABLE = XLS::BaseObjectPtr(new XLSB::TABLE());
					m_oTable->toBin(tableStream->m_TABLE);
				}
			}
			xlsb->WriteBin(oPath, tableStream.get());
		}
	}

	void CTableFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read( oRootPath, oPath );

        if( m_oReadPath.GetExtention() == _T(".bin"))
        {
            readBin(m_oReadPath);
            return;
        }

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oPath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		m_oTable = oReader;
	}
	void CTableFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		if(false == m_oTable.IsInit()) return;

		if (dynamic_cast<CXlsb*>(File::m_pMainDocument) && !dynamic_cast<CXlsb*>(File::m_pMainDocument)->IsWriteToXlsx())
		{
			writeBin(oPath);
		}
		else
		{
			NSStringUtils::CStringBuilder sXml;
			int nGlobalNumber = OOX::FileGlobalEnumerated::GetGlobalNumber();

			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
			m_oTable->toXML2(sXml, nGlobalNumber);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());
		}

		oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
		IFileContainer::Write( oPath, oDirectory, oContent );
	}
//---------------------------------------------------------------------------------------------------------------------
	void CQueryTableField::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<queryTableField");
			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
			WritingStringNullableAttrInt(L"id", m_oId, m_oId->GetValue());
			WritingStringNullableAttrInt(L"tableColumnId", m_oTableColumnId, m_oTableColumnId->GetValue());

			WritingStringNullableAttrBool2(L"rowNumbers",	m_oRowNumbers);
			WritingStringNullableAttrBool2(L"fillFormulas", m_oFillFormulas);
			WritingStringNullableAttrBool2(L"dataBound",	m_oDataBound);
			WritingStringNullableAttrBool2(L"clipped",		m_oClipped);
		writer.WriteString(L"/>");
	}
	void CQueryTableField::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ((L"extLst") == sName)
				m_oExtLst = oReader;
		}
	}
    void CQueryTableField::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::QSIF*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginQSIF);
        }
    }
    void CQueryTableField::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginQSIF*>(obj.get());

        if(ptr != nullptr)
        {
            m_oId               = ptr->idField;
            m_oTableColumnId    = ptr->idList;

            if(!ptr->name.empty())
                m_oName         = ptr->name;

            m_oRowNumbers       = ptr->fRowNums;
            m_oFillFormulas     = ptr->fFillDown;
            m_oDataBound        = ptr->fUserIns;
            m_oClipped          = ptr->fClipped;
        }
    }
	void CQueryTableField::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, (L"id"),			m_oId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, (L"tableColumnId"),	m_oTableColumnId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, (L"name"),			m_oName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, (L"rowNumbers"),	m_oRowNumbers )
			WritingElement_ReadAttributes_Read_else_if	( oReader, (L"fillFormulas"),	m_oFillFormulas )
			WritingElement_ReadAttributes_Read_else_if	( oReader, (L"dataBound"),		m_oDataBound )
			WritingElement_ReadAttributes_Read_else_if	( oReader, (L"clipped"),		m_oClipped )
		WritingElement_ReadAttributes_End( oReader )
	}

	void CQueryTableFields::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<queryTableFields");
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</queryTableFields>");
	}
	void CQueryTableFields::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"queryTableField" == sName )
				m_arrItems.push_back(new CQueryTableField(oReader));
		}
	}
    void CQueryTableFields::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::QSIFS*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (unsigned int)ptr->m_arQSIF.size();

            for(auto &qsif : ptr->m_arQSIF)
                m_arrItems.push_back(new CQueryTableField(qsif));
        }
    }
	void CQueryTableFields::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, (L"count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
	void CQueryTableDeletedField::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<deletedField");
			WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
		writer.WriteString(L"/>");
	}
	void CQueryTableDeletedField::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );
		if ( !oReader.IsEmptyNode() )
			oReader.ReadTillEnd();
	}
    void CQueryTableDeletedField::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::DELETEDNAME*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginDeletedName);
        }
    }
    void CQueryTableDeletedField::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginDeletedName*>(obj.get());

        if(ptr != nullptr)
        {
            if(!ptr->rgb.value().empty())
                m_oName = ptr->rgb.value();
        }
    }
	void CQueryTableDeletedField::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if	( oReader, (L"name"), m_oName )
		WritingElement_ReadAttributes_End( oReader )
	}
	void CQueryTableDeletedFields::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<queryTableDeletedFields");
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</queryTableDeletedFields>");
	}
	void CQueryTableDeletedFields::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"deletedField" == sName )
				m_arrItems.push_back(new CQueryTableDeletedField(oReader));
		}
	}
    void CQueryTableDeletedFields::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::DELETEDNAMES*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (unsigned int)ptr->m_arDELETEDNAME.size();

            for(auto &deletename : ptr->m_arDELETEDNAME)
                m_arrItems.push_back(new CQueryTableDeletedField(deletename));
        }
    }
	void CQueryTableDeletedFields::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, (L"count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
	void CQueryTableRefresh::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<queryTableRefresh");
			WritingStringNullableAttrInt(L"nextId", m_oNextId, m_oNextId->GetValue());
			WritingStringNullableAttrInt(L"unboundColumnsLeft", m_UnboundColumnsLeft, m_UnboundColumnsLeft->GetValue());
			WritingStringNullableAttrInt(L"unboundColumnsRight", m_UnboundColumnsRight, m_UnboundColumnsRight->GetValue());

			WritingStringNullableAttrBool2(L"fieldIdWrapped",	m_FieldIdWrapped);
			WritingStringNullableAttrBool2(L"headersInLastRefresh",	m_HeadersInLastRefresh);
			WritingStringNullableAttrBool2(L"preserveSortFilterLayout",	m_PreserveSortFilterLayout);
			WritingStringNullableAttrInt(L"minimumVersion",	m_oMinimumVersion, m_oMinimumVersion->GetValue());
		writer.WriteString(L">");

		if (m_oQueryTableFields.IsInit())
			m_oQueryTableFields->toXML(writer);

		if (m_oQueryTableDeletedFields.IsInit())
			m_oQueryTableDeletedFields->toXML(writer);

		if (m_oSortState.IsInit())
			m_oSortState->toXML(writer);

		writer.WriteString(L"</queryTableRefresh>");
	}
	void CQueryTableRefresh::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ((L"queryTableFields") == sName)
				m_oQueryTableFields = oReader;
			else if ((L"queryTableDeletedFields") == sName)
				m_oQueryTableDeletedFields = oReader;
			else if ((L"sortState") == sName)
				m_oSortState = oReader;
			else if ((L"extLst") == sName)
				m_oExtLst = oReader;
		}
	}
    void CQueryTableRefresh::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::QSIR*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginQSIR);

            if(ptr->m_QSIFS != nullptr)
                m_oQueryTableFields = ptr->m_QSIFS;

            if(ptr->m_DELETEDNAMES != nullptr)
                m_oQueryTableDeletedFields = ptr->m_DELETEDNAMES;

            if(ptr->m_SORTSTATE != nullptr)
                m_oSortState = ptr->m_SORTSTATE;
        }
    }
    void CQueryTableRefresh::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginQSIR*>(obj.get());

        if(ptr != nullptr)
        {
            m_oNextId                   = ptr->idFieldNext;
            m_oMinimumVersion           = ptr->wVerBeforeRefreshAlert;
            m_FieldIdWrapped            = ptr->fidWrapped;
            m_HeadersInLastRefresh      = ptr->fTitlesOld;
            m_PreserveSortFilterLayout  = ptr->fPersist;
            m_UnboundColumnsLeft        = ptr->ccolExtraLeft;
            m_UnboundColumnsRight       = ptr->ccolExtraRight;
        }
    }
	void CQueryTableRefresh::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, (L"nextId"),				m_oNextId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, (L"minimumVersion"),		m_oMinimumVersion )
			WritingElement_ReadAttributes_Read_else_if	( oReader, (L"fieldIdWrapped"),		m_FieldIdWrapped )
			WritingElement_ReadAttributes_Read_else_if	( oReader, (L"headersInLastRefresh"),	m_HeadersInLastRefresh )
			WritingElement_ReadAttributes_Read_else_if	( oReader, (L"preserveSortFilterLayout"),	m_PreserveSortFilterLayout )
			WritingElement_ReadAttributes_Read_else_if	( oReader, (L"unboundColumnsLeft"),	m_UnboundColumnsLeft )
			WritingElement_ReadAttributes_Read_else_if	( oReader, (L"unboundColumnsRight"),	m_UnboundColumnsRight )
		WritingElement_ReadAttributes_End( oReader )
	}
	
	void CQueryTable::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(false == m_oName.IsInit()) return;

		writer.WriteString(L"<queryTable \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"xr16\" \
xmlns:xr16=\"http://schemas.microsoft.com/office/spreadsheetml/2017/revision16\"");

		WritingStringNullableAttrEncodeXmlString(L"name",		m_oName, m_oName.get());
		WritingStringNullableAttrBool2(L"backgroundRefresh",	m_oBackgroundRefresh);
		WritingStringNullableAttrInt(L"connectionId",			m_oConnectionId, m_oConnectionId->GetValue());
		WritingStringNullableAttrInt(L"autoFormatId",			m_oAutoFormatId, m_oAutoFormatId->GetValue());
		WritingStringNullableAttrBool2(L"adjustColumnWidth",	m_oAdjustColumnWidth);
		WritingStringNullableAttrBool2(L"applyBorderFormats",	m_oApplyBorderFormats);
		WritingStringNullableAttrBool2(L"applyFontFormats",		m_oApplyFontFormats);
		WritingStringNullableAttrBool2(L"applyNumberFormats",	m_oApplyNumberFormats);
		WritingStringNullableAttrBool2(L"applyPatternFormats",	m_oApplyPatternFormats);
		WritingStringNullableAttrBool2(L"applyWidthHeightFormats",m_oApplyWidthHeightFormats);
		WritingStringNullableAttrBool2(L"applyAlignmentFormats",m_oApplyAlignmentFormats);

		WritingStringNullableAttrBool2(L"disableEdit",		m_oDisableEdit);
		WritingStringNullableAttrBool2(L"disableRefresh",	m_oDisableRefresh);
		WritingStringNullableAttrBool2(L"fillFormulas",		m_oFillFormulas);
		WritingStringNullableAttrBool2(L"firstBackgroundRefresh", m_oFirstBackgroundRefresh);

		WritingStringNullableAttrEncodeXmlString(L"growShrinkType",	m_oGrowShrinkType, m_oGrowShrinkType.get());

		WritingStringNullableAttrBool2(L"headers",			m_oHeaders);
		WritingStringNullableAttrBool2(L"intermediate",		m_oIntermediate);
		WritingStringNullableAttrBool2(L"preserveFormatting",m_oPreserveFormatting);
		WritingStringNullableAttrBool2(L"refreshOnLoad",	m_oRefreshOnLoad);
		WritingStringNullableAttrBool2(L"removeDataOnSave",	m_oRemoveDataOnSave);
		WritingStringNullableAttrBool2(L"rowNumbers",		m_oRowNumbers);
		
		writer.WriteString(L">");

		if(m_oQueryTableRefresh.IsInit())
			m_oQueryTableRefresh->toXML(writer);
		if(m_oExtLst.IsInit())
		{
			writer.WriteString(m_oExtLst->toXMLWithNS((L"")));
		}
		writer.WriteString(L"</queryTable>");
	}
	void CQueryTable::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( (L"queryTableRefresh") == sName )
				m_oQueryTableRefresh = oReader;
			else if ((L"extLst") == sName)
				m_oExtLst = oReader;
		}
	}
    void CQueryTable::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::QSI*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginQSI);

            if(ptr->m_QSIR != nullptr)
                m_oQueryTableRefresh = ptr->m_QSIR;

            if(ptr->m_FRTQSI != nullptr)
                m_oExtLst = ptr->m_FRTQSI;
        }
    }
    void CQueryTable::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginQSI*>(obj.get());

        if(ptr != nullptr)
        {

            m_oAdjustColumnWidth        = ptr->fAutoFit;
            m_oApplyAlignmentFormats    = ptr->fibitAtrAlc;
            m_oApplyBorderFormats       = ptr->fibitAtrBdr;
            m_oApplyFontFormats         = ptr->fibitAtrFnt;
            m_oApplyNumberFormats       = ptr->fibitAtrNum;
            m_oApplyPatternFormats      = ptr->fibitAtrPat;
            m_oApplyWidthHeightFormats  = ptr->fibitAtrProt;
            m_oBackgroundRefresh        = ptr->fAsync;

            m_oAutoFormatId             = ptr->itblAutoFmt;
            m_oConnectionId             = ptr->dwConnID;

            m_oDisableEdit              = ptr->fDisableEdit;
            m_oDisableRefresh           = ptr->fDisableRefresh;
            m_oFillFormulas             = ptr->fFill;
            m_oFirstBackgroundRefresh   = ptr->fNewAsync;

            if(ptr->fOverwrite)
                m_oGrowShrinkType = L"overwriteClear";
            else if(ptr->fShrink)
                m_oGrowShrinkType = L"insertDelete";
            else
                m_oGrowShrinkType = L"insertClear";

            m_oHeaders                  = ptr->fTitles;
            m_oIntermediate             = ptr->fDummyList;

            if(!ptr->name.empty())
                m_oName                 = ptr->name;

            m_oPreserveFormatting       = ptr->fPreserveFmt;
            m_oRefreshOnLoad            = ptr->fAutoRefresh;
            m_oRemoveDataOnSave         = !ptr->fSaveData;
            m_oRowNumbers               = ptr->fRowNums;
        }
    }
	void CQueryTable::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"adjustColumnWidth",		m_oAdjustColumnWidth )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"applyAlignmentFormats",	m_oApplyAlignmentFormats )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"applyBorderFormats",		m_oApplyBorderFormats )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"applyFontFormats",			m_oApplyFontFormats )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"applyNumberFormats",		m_oApplyNumberFormats )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"applyPatternFormats",		m_oApplyPatternFormats )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"applyWidthHeightFormats",	m_oApplyWidthHeightFormats )			
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"autoFormatId",			m_oAutoFormatId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"backgroundRefresh",	m_oBackgroundRefresh )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"connectionId",			m_oConnectionId )			
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"disableEdit",			m_oDisableEdit )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"disableRefresh",		m_oDisableRefresh )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"fillFormulas",			m_oFillFormulas )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"firstBackgroundRefresh",m_oFirstBackgroundRefresh )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"growShrinkType",		m_oGrowShrinkType )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"headers",				m_oHeaders )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"intermediate",			m_oIntermediate )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"name",					m_oName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"preserveFormatting",	m_oPreserveFormatting )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"refreshOnLoad",		m_oRefreshOnLoad )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"removeDataOnSave",		m_oRemoveDataOnSave )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"rowNumbers",			m_oRowNumbers )
		WritingElement_ReadAttributes_End( oReader )
	}
    void CQueryTableFile::readBin(const CPath& oPath)
    {
        CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
        if (xlsb)
        {
            XLSB::QueryTableStreamPtr querytableStream(new XLSB::QueryTableStream);

            xlsb->ReadBin(oPath, querytableStream.get());

            if (querytableStream != nullptr)
            {
                if (querytableStream->m_QSI != nullptr)
                    m_oQueryTable = querytableStream->m_QSI;
            }

            //querytableStream.reset();

        }
    }
	void CQueryTableFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read( oRootPath, oPath );

        if( m_oReadPath.GetExtention() == _T(".bin"))
        {
            readBin(m_oReadPath);
            return;
        }

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oPath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		m_oQueryTable = oReader;
	}
	void CQueryTableFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		if(false == m_oQueryTable.IsInit()) return;

		NSStringUtils::CStringBuilder sXml;

		sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		m_oQueryTable->toXML(sXml);

        std::wstring sPath = oPath.GetPath();
        NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

		oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
		IFileContainer::Write( oPath, oDirectory, oContent );
	}
} //Spreadsheet
} // namespace OOX
