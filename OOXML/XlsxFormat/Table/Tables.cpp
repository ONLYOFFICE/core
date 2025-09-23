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
#include "../../XlsbFormat/Biff12_records/BeginListCols.h"
#include "../../XlsbFormat/Biff12_records/ListCCFmla.h"
#include "../../XlsbFormat/Biff12_records/ListTrFmla.h"
#include "../../XlsbFormat/Biff12_records/List14.h"
#include "../../XlsbFormat/Biff12_records/BeginDeletedName.h"
#include "../../XlsbFormat/Biff12_records/BeginDeletedNames.h"

#include "../../XlsbFormat/QueryTableStream.h"
#include "../../XlsbFormat/Biff12_unions/QSI.h"
#include "../../XlsbFormat/Biff12_records/CommonRecords.h"
#include "../../XlsbFormat/Biff12_unions/QSIR.h"
#include "../../XlsbFormat/Biff12_unions/QSIFS.h"
#include "../../XlsbFormat/Biff12_records/BeginQSIFs.h"
#include "../../XlsbFormat/Biff12_unions/QSIF.h"
#include "../../XlsbFormat/Biff12_unions/DELETEDNAMES.h"
#include "../../XlsbFormat/Biff12_unions/DELETEDNAME.h"
#include "../../XlsbFormat/Biff12_unions/FRTTABLE.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"
#include "../../DocxFormat/Drawing/DrawingExt.h"

#include "../../Binary/XlsbFormat/FileTypes_SpreadsheetBin.h"
#include "../../../MsBinaryFile/XlsFile/Format/Binary/CFStreamCacheWriter.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/Feature11.h"
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
	XLS::BaseObjectPtr CAltTextTable::toBin()
	{
		auto ptr(new XLSB::List14);
		XLS::BaseObjectPtr objectPtr(ptr);
		if(m_oAltText.IsInit())
			ptr->stAltText = m_oAltText.get();
		else
			ptr->stAltText.setSize(0xFFFFFFFF);
		if(m_oAltTextSummary.IsInit())
			ptr->stAltTextSummary = m_oAltTextSummary.get();
		else
			ptr->stAltTextSummary.setSize(0xFFFFFFFF);
		return objectPtr;
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
	XLS::BaseObjectPtr CTableStyleInfo::toBin()
	{
		auto ptr(new XLSB::TableStyleClient);
		XLS::BaseObjectPtr objectPtr(ptr);
		if(m_oName.IsInit())
			ptr->stStyleName = m_oName.get();
		else
			ptr->stStyleName.setSize(0xFFFFFFFF);
		if(m_oShowColumnStripes.IsInit())
			ptr->fColumnStripes = m_oShowColumnStripes->GetValue();
		else
            ptr->fColumnStripes = false;
		if(m_oShowFirstColumn.IsInit())
			ptr->fFirstColumn = m_oShowFirstColumn->GetValue();
		else
			ptr->fFirstColumn = false;
		if(m_oShowLastColumn.IsInit())
			ptr->fLastColumn = m_oShowLastColumn->GetValue();
		else
			ptr->fLastColumn = false;
		if(m_oShowRowStripes.IsInit())
			ptr->fRowStripes = m_oShowRowStripes->GetValue();
		else
			ptr->fRowStripes = false;
		return objectPtr;
	}
    void CTableStyleInfo::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::TableStyleClient*>(obj.get());
        if(ptr != nullptr)
        {
            m_oName                 = ptr->stStyleName.value();
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
	//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CXmlColumnPr::getType() const
	{
		return et_x_xmlColumnPr;
	}
	void CXmlColumnPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "mapId", mapId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "xpath", xpath)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "xmlDataType", xmlDataType)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "denormalized", denormalized)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CXmlColumnPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	void CXmlColumnPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, mapId);
		pWriter->WriteString2(1, xpath);
		if (xmlDataType.IsInit())
			pWriter->WriteByte1(2, xmlDataType->GetValue());
		pWriter->WriteBool2(1, denormalized);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CXmlColumnPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				mapId = pReader->GetULong();
			}break;
			case 1:
			{
				xpath = pReader->GetString2();
			}break;
			case 2:
			{
				xmlDataType.Init();
				xmlDataType->SetValueFromByte(pReader->GetUChar());
			}break;
			case 3:
			{
				denormalized = pReader->GetBool();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CXmlColumnPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"xmlColumnPr");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"mapId", mapId);
		pWriter->WriteAttribute2(L"xpath", xpath);
		if (xmlDataType.IsInit()) pWriter->WriteAttribute(L"xmlDataType", xmlDataType->ToString());
		pWriter->WriteAttribute(L"denormalized", denormalized);
		pWriter->EndAttributes();

		pWriter->WriteNodeEnd(L"xmlColumnPr");
	}
	void CXmlColumnPr::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<xmlColumnPr");
		WritingStringNullableAttrInt2(L"mapId", mapId);
		WritingStringNullableAttrEncodeXmlString(L"xpath", xpath, *xpath);
		WritingStringNullableAttrString(L"xmlDataType", xmlDataType, xmlDataType->ToString());
		WritingStringNullableAttrBool2(L"denormalized", denormalized);
		writer.WriteString(L"/>");
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
		
		if (m_oTotalsRowFormula.IsInit() || m_oCalculatedColumnFormula.IsInit() || m_oXmlColumnPr.IsInit())
		{
			writer.WriteString(L">");

			if (m_oXmlColumnPr.IsInit())
			{
				m_oXmlColumnPr->toXML(writer);
			}
			if (m_oCalculatedColumnFormula.IsInit())
			{
				WritingStringValEncodeXmlString(L"calculatedColumnFormula", m_oCalculatedColumnFormula.get());
			}
			if (m_oTotalsRowFormula.IsInit())
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

			if (L"totalsRowFormula" == sName)
				m_oTotalsRowFormula = oReader.GetText3();
			else if (L"calculatedColumnFormula" == sName)
				m_oCalculatedColumnFormula = oReader.GetText3();
			else if (L"xmlColumnPr" == sName)
				m_oXmlColumnPr = oReader;
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
	XLS::BaseObjectPtr CTableColumn::toBin()
	{
		auto ptr(new XLSB::LISTCOL);
		XLS::BaseObjectPtr objectPtr(ptr);

        auto ptr1(new XLSB::BeginListCol);
        ptr->m_BrtBeginListCol = XLS::BaseObjectPtr{ptr1};
        if(m_oDataCellStyle.IsInit())
            ptr1->stStyleInsertRow = m_oDataCellStyle.get();
        else
            ptr1->stStyleInsertRow.setSize(0xFFFFFFFF);
        if(m_oTotalsRowDxfId.IsInit())
            ptr1->nDxfInsertRow = m_oTotalsRowDxfId->GetValue();
        else
            ptr1->nDxfInsertRow = 0xFFFFFFFF;

        if(m_oHeaderRowCellStyle.IsInit())
            ptr1->stStyleHeader = m_oHeaderRowCellStyle.get();
        else
            ptr1->stStyleHeader.setSize(0xFFFFFFFF);
        if(m_oHeaderRowDxfId.IsInit())
            ptr1->nDxfHdr = m_oHeaderRowDxfId->GetValue();
        else
            ptr1->nDxfHdr = 0xFFFFFFFF;
        if(m_oTotalsRowCellStyle.IsInit())
            ptr1->stStyleAgg = m_oTotalsRowCellStyle.get();
        else
            ptr1->stStyleAgg.setSize(0xFFFFFFFF);

        if(m_oDataDxfId.IsInit())
            ptr1->nDxfAgg = m_oDataDxfId->GetValue();
        else
            ptr1->nDxfAgg = 0xFFFFFFFF;
        if(m_oId.IsInit())
            ptr1->idField = m_oId->GetValue();
        else
            ptr1->idField = 0;

        if(m_oName.IsInit())
            ptr1->stCaption = m_oName.get();
        else
            ptr1->stCaption.setSize(0xFFFFFFFF);
        if(m_oQueryTableFieldId.IsInit())
            ptr1->idqsif = m_oQueryTableFieldId->GetValue();
        else
            ptr1->idqsif = 0;

        if(m_oTotalsRowLabel.IsInit())
            ptr1->stTotal = m_oTotalsRowLabel.get();
        else
            ptr1->stTotal.setSize(0xFFFFFFFF);

        if(m_oUniqueName.IsInit())
            ptr1->stName = m_oUniqueName.get();
        else
            ptr1->stName.setSize(0xFFFFFFFF);
        if (m_oTotalsRowFunction == SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionNone)
            ptr1->ilta = XLSB::ListTotalRowFunction::ILTA_NONE;
        else if (m_oTotalsRowFunction == SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionAverage)
            ptr1->ilta = XLSB::ListTotalRowFunction::ILTA_AVERAGE;
        else if (m_oTotalsRowFunction == SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionCount)
            ptr1->ilta = XLSB::ListTotalRowFunction::ILTA_COUNT;
        else if (m_oTotalsRowFunction == SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionCountNums)
            ptr1->ilta = XLSB::ListTotalRowFunction::ILTA_COUNTNUMS;
        else if (m_oTotalsRowFunction == SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionMax)
            ptr1->ilta = XLSB::ListTotalRowFunction::ILTA_MAX;
        else if (m_oTotalsRowFunction == SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionMin)
            ptr1->ilta = XLSB::ListTotalRowFunction::ILTA_MIN;
        else if (m_oTotalsRowFunction == SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionSum)
            ptr1->ilta = XLSB::ListTotalRowFunction::ILTA_SUM;
        else if (m_oTotalsRowFunction == SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionStdDev)
            ptr1->ilta = XLSB::ListTotalRowFunction::ILTA_STDDEV;
        else if (m_oTotalsRowFunction == SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionVar)
            ptr1->ilta = XLSB::ListTotalRowFunction::ILTA_VAR;
        else if (m_oTotalsRowFunction == SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionCustom)
            ptr1->ilta = XLSB::ListTotalRowFunction::ILTA_CUSTOM;
        else
        	ptr1->ilta = XLSB::ListTotalRowFunction::ILTA_NONE;

        if(m_oCalculatedColumnFormula.IsInit())
        {
            auto fmla(new XLSB::ListCCFmla);
            fmla->fArray = false;
            fmla->formula = m_oCalculatedColumnFormula.get();
            ptr->m_BrtListCCFmla = XLS::BaseObjectPtr{fmla};
        }
        if(m_oTotalsRowFormula.IsInit())
        {
            auto fmla(new XLSB::ListTrFmla);
            fmla->fArray = false;
            fmla->formula = m_oTotalsRowFormula.get();
            ptr->m_BrtListTrFmla = XLS::BaseObjectPtr{fmla};
        }
        return objectPtr;
    }
	XLS::BiffStructurePtr CTableColumn::toXLS()
	{
		auto ptr = new XLS::Feat11FieldDataItem(0, 0, 0);
		if(m_oId.IsInit())
			ptr->idField = m_oId->GetValue();
		if(m_oName.IsInit())
			ptr->strFieldName = m_oName.get();
		return XLS::BiffStructurePtr(ptr);
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
                case XLSB::ListTotalRowFunction::ILTA_NONE:
                    m_oTotalsRowFunction = SimpleTypes::Spreadsheet::ETotalsRowFunction::totalrowfunctionNone; break;
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
			{
				CTableColumn* pTableColumn = new CTableColumn();
				*pTableColumn = oReader;
				m_arrItems.push_back(pTableColumn);
			}
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
	XLS::BaseObjectPtr CTableColumns::toBin()
	{
		auto ptr(new XLSB::LISTCOLS);
		XLS::BaseObjectPtr objectPtr(ptr);
        for(auto i:m_arrItems)
            ptr->m_arLISTCOL.push_back(i->toBin());
		return objectPtr;
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
		if(!m_oName.IsInit() && m_oDisplayName.IsInit())
			m_oName = m_oDisplayName.get();
		
		if(m_oTableColumns.IsInit() && !m_oTableColumns->m_arrItems.empty())
		{	
			XLS::GlobalWorkbookInfo::mapTableColumnNames_static.emplace(m_oId->GetValue(),
				std::vector<std::wstring>(m_oTableColumns->m_arrItems.size()));	
				auto colInd = 0;
			for(auto i:m_oTableColumns->m_arrItems)
			{
				if(i->m_oName.IsInit())
                {
					i->m_oName = boost::algorithm::replace_all_copy(i->m_oName.get(), L"_x000a_", L"\n");
					std::unordered_map<int, std::vector<std::wstring>>::iterator pFind = XLS::GlobalWorkbookInfo::mapTableColumnNames_static.find(m_oId->GetValue());
					if (pFind != XLS::GlobalWorkbookInfo::mapTableColumnNames_static.end())
					{
						if (colInd < pFind->second.size())
						{
							pFind->second[colInd] = i->m_oName.get();
						}
					}
                }
				colInd++;
			}
		}
        XLS::GlobalWorkbookInfo::mapTableNames_static.emplace(m_oId->GetValue(), m_oName.get());
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
	XLS::BaseObjectPtr CTable::toBin()
	{
		auto ptr(new XLSB::TABLE);
		auto ptr1(new XLSB::BeginList);
		ptr->m_BrtBeginList = XLS::BaseObjectPtr{ptr1};
		XLS::BaseObjectPtr objectPtr(ptr);

		if(m_oRef.IsInit())
        {
            ptr1->rfxList = m_oRef->GetValue();
        }
        if(m_oName.IsInit())
            ptr1->stName = m_oName.get();
        else if(m_oDisplayName.IsInit())
            ptr1->stName = m_oDisplayName.get();
        else
            ptr1->stName.setSize(0xFFFFFFFF);
        if(m_oTotalsRowCount.IsInit())
            ptr1->crwTotals = m_oTotalsRowCount->GetValue();
        else
            ptr1->crwTotals = false;

        if(m_oHeaderRowCount.IsInit())
            ptr1->crwHeader = m_oHeaderRowCount->GetValue();
        else
            ptr1->crwHeader = true;

        if(m_oDisplayName.IsInit())
            ptr1->stDisplayName = m_oDisplayName.get();
        else
            ptr1->stDisplayName.setSize(0xFFFFFFFF);

        if(m_oTableBorderDxfId.IsInit())
            ptr1->nDxfBorder = m_oTableBorderDxfId->GetValue();
        else
            ptr1->nDxfBorder = 0xFFFFFFFF;

        if(m_oComment.IsInit())
            ptr1->stComment = m_oComment.get();
        else
            ptr1->stComment.setSize(0xFFFFFFFF);

        if(m_oConnectionId.IsInit())
            ptr1->dwConnID = m_oConnectionId->GetValue();
        else
            ptr1->dwConnID = 0;

        if(m_oDataDxfId.IsInit())
            ptr1->nDxfData = m_oDataDxfId->GetValue();
        else
            ptr1->nDxfData = 0xFFFFFFFF;

        if(m_oDataCellStyle.IsInit())
            ptr1->stStyleData = m_oDataCellStyle.get();
        else
            ptr1->stStyleData.setSize(0xFFFFFFFF);

        if(m_oHeaderRowBorderDxfId.IsInit())
            ptr1->nDxfHeaderBorder = m_oHeaderRowBorderDxfId->GetValue();
        else
            ptr1->nDxfHeaderBorder = 0xFFFFFFFF;

        if(m_oHeaderRowCellStyle.IsInit())
            ptr1->stStyleHeader = m_oHeaderRowCellStyle.get();
        else
            ptr1->stStyleHeader.setSize(0xFFFFFFFF);

        if(m_oHeaderRowDxfId.IsInit())
            ptr1->nDxfHeader = m_oHeaderRowDxfId->GetValue();
        else
            ptr1->nDxfHeader = 0xFFFFFFFF;
        if(m_oInsertRow.IsInit())
            ptr1->fForceInsertToBeVisible = m_oInsertRow.get();
        else
            ptr1->fForceInsertToBeVisible = false;
        if(m_oInsertRowShift.IsInit())
            ptr1->fInsertRowInsCells = m_oInsertRowShift.get();
        else
            ptr1->fInsertRowInsCells = false;
        if(m_oPublished.IsInit())
            ptr1->fPublished = m_oPublished.get();
        else
            ptr1->fPublished = false;
        if(m_oId.IsInit())
            ptr1->idList = m_oId->GetValue();
		else
			ptr1->idList = 0;

        if(m_oTableType.IsInit())
        {
            if(m_oTableType == SimpleTypes::Spreadsheet::ETableType::typeWorksheet)
                ptr1->lt = XLSB::ListType::LTRANGE;
            if(m_oTableType == SimpleTypes::Spreadsheet::ETableType::typeXml)
                ptr1->lt = XLSB::ListType::LTXML;
            if(m_oTableType == SimpleTypes::Spreadsheet::ETableType::typeQueryTable)
                ptr1->lt = XLSB::ListType::LTEXTDATA;
        }
        else
        {
            ptr1->lt = XLSB::ListType::LTRANGE;
        }

        if(m_oTotalsRowBorderDxfId.IsInit())
            ptr1->nDxfAggBorder = m_oTotalsRowBorderDxfId->GetValue();
        else
            ptr1->nDxfAggBorder  = 0xFFFFFFFF;
        if(m_oTotalsRowCellStyle.IsInit())
            ptr1->stStyleAgg = m_oTotalsRowCellStyle.get();
        else
            ptr1->stStyleAgg.setSize(0xFFFFFFFF);

        if(m_oTotalsRowDxfId.IsInit())
            ptr1->nDxfAgg = m_oTotalsRowDxfId->GetValue();
        else
           ptr1->nDxfAgg = 0xFFFFFFFF;

        if(m_oTotalsRowShown.IsInit())
            ptr1->fShownTotalRow = m_oTotalsRowShown.get();
        else if(m_oTotalsRowCount.IsInit() && m_oTotalsRowCount->GetValue() > 0)
            ptr1->fShownTotalRow = true;
        else
            ptr1->fShownTotalRow = false;
        ptr1->fSingleCell = false;
        if(m_oAutoFilter.IsInit())
            ptr->m_AUTOFILTER = m_oAutoFilter->toBin();
        if(m_oSortState.IsInit())
            ptr->m_SORTSTATE = m_oSortState->toBin();
        if(m_oTableColumns.IsInit())
            ptr->m_LISTCOLS = m_oTableColumns->toBin();
        if(m_oTableStyleInfo.IsInit())
            ptr->m_BrtTableStyleClient = m_oTableStyleInfo->toBin();
        if(m_oExtLst.IsInit())
            ptr->m_FRTTABLE = m_oExtLst->toBinTable();
		return objectPtr;
	}
	XLS::BaseObjectPtr CTable::toXLS()
	{
		auto ptr = new XLS::Feature11;
		if(m_oRef.IsInit())
		{
			auto tempref = new XLS::Ref8U;
			tempref->fromString(m_oRef->GetValue());
			ptr->refs2.push_back(XLS::BiffStructurePtr(tempref));
			ptr->frtRefHeaderU.ref8 = *tempref;
		}
		if(m_oHeaderRowCount.IsInit() && m_oHeaderRowCount.get() == 0)
		{
			ptr->rgbFeat.crwHeader = 0;
		}
		if(m_oTotalsRowShown.IsInit() && m_oTotalsRowShown.get())
		{
			ptr->rgbFeat.crwTotals = 1;
		}
		if(m_oAutoFilter.IsInit())
		{
			ptr->rgbFeat.fPersistAutoFilter = true;
			ptr->rgbFeat.fAutoFilter = true;
			ptr->rgbFeat.fApplyAutoFilter  = true;
		}
		if(m_oName.IsInit())
			ptr->rgbFeat.rgbName = m_oName.get();
		if(m_oId.IsInit())
		{
			ptr->rgbFeat.idList = m_oId->GetValue();
		}
		if(m_oTableColumns.is_init())
		{
			for(auto i : m_oTableColumns->m_arrItems)
				ptr->rgbFeat.arFieldData.push_back(i->toXLS());
		}
		return XLS::BaseObjectPtr(ptr);
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
            m_oRef                      = ptr->rfxList.toString(true, true);

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

            m_oInsertRow                = ptr->fForceInsertToBeVisible;
            m_oInsertRowShift           = ptr->fInsertRowInsCells;
            m_oPublished                = ptr->fPublished;
            m_oId                       = ptr->idList;

            switch (ptr->lt.value().get())
            {
                case XLSB::ListType::LTRANGE:
                    m_oTableType = SimpleTypes::Spreadsheet::ETableType::typeWorksheet; break;
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

            m_oTotalsRowShown           = ptr->fShownTotalRow;

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
	XLS::BaseObjectPtr CTablePart::toBin()
	{
		auto ptr(new XLSB::ListPart);
		XLS::BaseObjectPtr objectPtr(ptr);
		if(m_oRId.IsInit())
			ptr->stRelID.value = m_oRId->GetValue();
		return objectPtr;
	}
    void CTablePart::toBin(XLS::StreamCacheWriterPtr& writer)
    {
        auto record = writer->getNextRecord(XLSB::rt_ListPart);
        XLSB::RelID stRelID;
        if(m_oRId.IsInit())
            stRelID = m_oRId->GetValue();
        *record << stRelID;
        writer->storeNextRecord(record);
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
			{
				CTablePart* pTablePart = new CTablePart();
				*pTablePart = oReader;
				m_arrItems.push_back(pTablePart);
			}
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
	XLS::BaseObjectPtr CTableParts::toBin()
	{
		auto ptr(new XLSB::LISTPARTS);
		XLS::BaseObjectPtr objectPtr(ptr);

		if(m_oCount.IsInit())
		{
			auto beginlistParts(new XLSB::BeginListParts);
			ptr->m_BrtBeginListParts = XLS::BaseObjectPtr{beginlistParts};
			beginlistParts->cParts = m_oCount->m_eValue;
		}
		for(auto i:m_arrItems)
		{
			ptr->m_arBrtListPart.push_back(i->toBin());
		}
		return objectPtr;
	}
    void CTableParts::toBin(XLS::StreamCacheWriterPtr& writer)
    {
        {
            auto begin = writer->getNextRecord(XLSB::rt_BeginListParts);
            _UINT32 count = m_arrItems.size();
            *begin << count;
            writer->storeNextRecord(begin);
        }
        for(auto i:m_arrItems)
        {
            i->toBin(writer);
        }
        {
            auto end = writer->getNextRecord(XLSB::rt_EndListParts);
            writer->storeNextRecord(end);
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

            //tableStream.reset();

        }
    }
	XLS::BaseObjectPtr CTableFile::WriteBin() const
	{
		XLSB::TableStreamPtr tableStream(new XLSB::TableStream);
		XLS::BaseObjectPtr objectPtr(tableStream);

		tableStream->m_TABLE = m_oTable->toBin();

		return objectPtr;
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

		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if ((xlsb) && (xlsb->m_bWriteToXlsb))
			{
				XLS::BaseObjectPtr object = WriteBin();
				xlsb->WriteBin(oPath, object.get());
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
	const OOX::FileType CTableFile::type() const
	{
		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
		if ((xlsb) && (xlsb->m_bWriteToXlsb))
		{
			return OOX::SpreadsheetBin::FileTypes::TableBin;
		}
			return OOX::Spreadsheet::FileTypes::Table;
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
	XLS::BaseObjectPtr CQueryTableField::toBin()
	{
		auto ptr1(new XLSB::QSIF);
		auto ptr(new XLSB::BeginQSIF);
		ptr1->m_BrtBeginQSIF = XLS::BaseObjectPtr{ptr};

		if(m_oId.IsInit())
			ptr->idField = m_oId->GetValue();
        else
            ptr->idField = 0;
		if(m_oTableColumnId.IsInit())
			ptr->idList = m_oTableColumnId->GetValue();
        else
            ptr->idList = 0;
		if(m_oName.IsInit())
			ptr->name = m_oName.get();
        if(m_oRowNumbers.IsInit())
			ptr->fRowNums = m_oRowNumbers.get();
		if(m_oFillFormulas.IsInit())
			ptr->fFillDown = m_oFillFormulas.get();
        else
            ptr->fFillDown = false;
		if(m_oDataBound.IsInit())
			ptr->fUserIns = m_oDataBound.get();
        else
            ptr->fUserIns = false;
        if(m_oClipped.IsInit())
			ptr->fClipped = m_oClipped.get();
        else
            ptr->fClipped = false;
           
		return XLS::BaseObjectPtr{ptr1};
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
			{
				CQueryTableField* pQueryTableField = new CQueryTableField();
				*pQueryTableField = oReader;
				m_arrItems.push_back(pQueryTableField);
			}
		}
	}
	XLS::BaseObjectPtr CQueryTableFields::toBin()
	{
		auto ptr(new XLSB::QSIFS);
		auto ptr1(new XLSB::BeginQSIFs);
		ptr->m_BrtBeginQSIFs = XLS::BaseObjectPtr{ptr1};
		for(auto i:m_arrItems)
			ptr->m_arQSIF.push_back(i->toBin());
		ptr1->nCols = m_arrItems.size();
		return XLS::BaseObjectPtr{ptr};
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
	XLS::BaseObjectPtr CQueryTableDeletedField::toBin()
	{
		auto ptr1(new XLSB::DELETEDNAME);
		auto ptr(new XLSB::BeginDeletedName);
		ptr1->m_BrtBeginDeletedName = XLS::BaseObjectPtr{ptr};
		if(m_oName.IsInit())
			ptr->rgb = m_oName.get();

		return XLS::BaseObjectPtr{ptr1};
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
			{
				CQueryTableDeletedField* pQueryTableDeletedField = new CQueryTableDeletedField();
				*pQueryTableDeletedField = oReader;
				m_arrItems.push_back(pQueryTableDeletedField);
			}
		}
	}
	XLS::BaseObjectPtr CQueryTableDeletedFields::toBin()
	{
		auto ptr(new XLSB::DELETEDNAMES);
		auto ptr1(new XLSB::BeginDeletedNames);
		ptr->m_BrtBeginDeletedNames = XLS::BaseObjectPtr{ptr1};

		for(auto i:m_arrItems)
			ptr->m_arDELETEDNAME.push_back(i->toBin());

		ptr1->nCols = ptr->m_arDELETEDNAME.size();
		return XLS::BaseObjectPtr{ptr};
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
	XLS::BaseObjectPtr CQueryTableRefresh::toBin()
	{
		auto ptr1(new XLSB::QSIR);
		auto ptr(new XLSB::BeginQSIR);
		ptr1->m_BrtBeginQSIR = XLS::BaseObjectPtr{ptr};

		if(m_oNextId.IsInit())
			ptr->idFieldNext = m_oNextId->GetValue();
		else
			ptr->idFieldNext = 0;
		if(m_oMinimumVersion.IsInit())
			ptr->wVerBeforeRefreshAlert = m_oMinimumVersion->GetValue();
		else
			ptr->wVerBeforeRefreshAlert = 0;
		if(m_FieldIdWrapped.IsInit())
			ptr->fidWrapped = m_FieldIdWrapped.get();
        else
            ptr->fidWrapped = false;
		if(m_HeadersInLastRefresh.IsInit())
			ptr->fTitlesOld = m_HeadersInLastRefresh.get();
		else
			ptr->fTitlesOld = false;;
		if(m_PreserveSortFilterLayout.IsInit())
			ptr->fPersist = m_PreserveSortFilterLayout.get();
		else
			ptr->fPersist = false;
		if(m_UnboundColumnsLeft.IsInit())
			ptr->ccolExtraLeft = m_UnboundColumnsLeft->GetValue();
        else
            ptr->ccolExtraLeft = 0;
		if(m_UnboundColumnsRight.IsInit())
			ptr->ccolExtraRight = m_UnboundColumnsRight->GetValue();
        else
            ptr->ccolExtraRight = 0;

		if(m_oQueryTableFields.IsInit())
			ptr1->m_QSIFS = m_oQueryTableFields->toBin();
		if(m_oQueryTableDeletedFields.IsInit())
			ptr1->m_DELETEDNAMES = m_oQueryTableDeletedFields->toBin();
		if(m_oSortState.IsInit())
			ptr1->m_SORTSTATE = m_oSortState->toBin();

		return XLS::BaseObjectPtr{ptr1};
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
	XLS::BaseObjectPtr CQueryTable::toBin()
	{
		auto ptr1(new XLSB::QSI);
		auto ptr(new XLSB::BeginQSI);
		ptr1->m_BrtBeginQSI = XLS::BaseObjectPtr{ptr};

		if(m_oAdjustColumnWidth.IsInit())
			ptr->fAutoFit = m_oAdjustColumnWidth.get();
        else
            ptr->fAutoFit = true;
		if(m_oApplyAlignmentFormats.IsInit())
        	ptr->fibitAtrAlc = m_oApplyAlignmentFormats.get();
        else
            ptr->fibitAtrAlc = false;
		if(m_oApplyBorderFormats.IsInit())
            ptr->fibitAtrBdr = m_oApplyBorderFormats.get();
        else
            ptr->fibitAtrBdr = false;
		if(m_oApplyFontFormats.IsInit())
            ptr->fibitAtrFnt = m_oApplyFontFormats.get();
        else
            ptr->fibitAtrFnt = false;
		if(m_oApplyNumberFormats.IsInit())
            ptr->fibitAtrNum = m_oApplyNumberFormats.get();
        else
            ptr->fibitAtrNum = false;
		if(m_oApplyPatternFormats.IsInit())
            ptr->fibitAtrPat = m_oApplyPatternFormats.get();
        else
            ptr->fibitAtrPat = false;
		if(m_oApplyWidthHeightFormats.IsInit())
            ptr->fibitAtrProt = m_oApplyWidthHeightFormats.get();
        else
            ptr->fibitAtrProt = false;
		if(m_oBackgroundRefresh.IsInit())
            ptr->fAsync = m_oBackgroundRefresh.get();
        else
            ptr->fAsync = true;
	
		if(m_oAutoFormatId.IsInit())
           ptr->itblAutoFmt = m_oAutoFormatId->GetValue();
        else
            ptr->itblAutoFmt = 0;
		if(m_oConnectionId.IsInit())
           ptr->dwConnID = m_oConnectionId->GetValue();
        else
            ptr->dwConnID = 0;
		if(m_oDisableEdit.IsInit())
            ptr->fDisableEdit = m_oDisableEdit.get();
        else
            ptr->fDisableEdit = false;
		if(m_oDisableRefresh.IsInit())
            ptr->fDisableRefresh = m_oDisableRefresh.get();
        else
            ptr->fDisableRefresh = false;
		if(m_oFillFormulas.IsInit())
            ptr->fFill = m_oFillFormulas.get();
        else
            ptr->fFill = false;
		if(m_oFirstBackgroundRefresh.IsInit())
            ptr->fNewAsync = m_oFirstBackgroundRefresh.get();
        else
            ptr->fNewAsync = true;
		ptr->fOverwrite = false;
		ptr->fShrink = false;

		if(m_oGrowShrinkType == L"overwriteClear" )
			ptr->fOverwrite = true;
		else if(m_oGrowShrinkType == L"insertDelete")
			ptr->fShrink = true;
		if(m_oHeaders.IsInit())
			ptr->fTitles = m_oHeaders.get();
		if(m_oIntermediate.IsInit())
			ptr->fDummyList = m_oIntermediate.get();
        else
            ptr->fDummyList = 0;

        if(m_oName.IsInit())
            ptr->name = m_oName.get();
		if(m_oPreserveFormatting.IsInit())
			ptr->fPreserveFmt = m_oPreserveFormatting.get();
		if(m_oRefreshOnLoad.IsInit())
			ptr->fAutoRefresh = m_oRefreshOnLoad.get();
        else
            ptr->fAutoRefresh = false;
		if(m_oRemoveDataOnSave.IsInit())
            ptr->fSaveData = !m_oRemoveDataOnSave.get();
        else
            ptr->fSaveData = true;
		if(m_oRowNumbers.IsInit())
            ptr->fRowNums = m_oRowNumbers.get();
        else
            ptr->fRowNums = false;

		if(m_oQueryTableRefresh.IsInit())
			ptr1->m_QSIR = m_oQueryTableRefresh->toBin();

		return XLS::BaseObjectPtr{ptr1};
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
	XLS::BaseObjectPtr CQueryTableFile::WriteBin() const
	{
		XLSB::QueryTableStreamPtr querytableStream(new XLSB::QueryTableStream);
		if(m_oQueryTable.IsInit())
			querytableStream->m_QSI = m_oQueryTable->toBin();
		return XLS::BaseObjectPtr{querytableStream};
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

		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
		if ((xlsb) && (xlsb->m_bWriteToXlsb))
		{
			XLS::BaseObjectPtr object = WriteBin();
			xlsb->WriteBin(oPath, object.get());
		}
		else
		{
			NSStringUtils::CStringBuilder sXml;

			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
			m_oQueryTable->toXML(sXml);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());
		}
		oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
		IFileContainer::Write( oPath, oDirectory, oContent );
	}
	const OOX::FileType CQueryTableFile::type() const
	{
		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
		if ((xlsb) && (xlsb->m_bWriteToXlsb))
		{
			return OOX::SpreadsheetBin::FileTypes::QueryTableBin;
		}
		return OOX::Spreadsheet::FileTypes::QueryTable;
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CXmlPr::getType() const
	{
		return et_x_xmlPr;
	}
	void CXmlPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "mapId", mapId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "xpath", xpath)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "xmlDataType", xmlDataType)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CXmlPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (!oReader.IsEmptyNode())
			oReader.ReadTillEnd();
	}
	void CXmlPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, mapId);
		pWriter->WriteString2(1, xpath);
		if (xmlDataType.IsInit()) 
			pWriter->WriteByte1(2, xmlDataType->GetValue());
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
	}
	void CXmlPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				mapId = pReader->GetULong();
			}break;			
			case 1:
			{
				xpath = pReader->GetString2();
			}break;
			case 2:
			{
				xmlDataType.Init();
				xmlDataType->SetValueFromByte(pReader->GetUChar());
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CXmlPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"xmlPr");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"mapId", mapId);
		pWriter->WriteAttribute2(L"xpath", xpath);
		if (xmlDataType.IsInit()) pWriter->WriteAttribute(L"xmlDataType", xmlDataType->ToString());
		pWriter->EndAttributes();

		pWriter->WriteNodeEnd(L"xmlPr");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CXmlCellPr::getType() const
	{
		return et_x_xmlCellPr;
	}
	void CXmlCellPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "uniqueName", uniqueName)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "id", id)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CXmlCellPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetNameNoNS();

			if (L"xmlPr" == sName)
			{
				xmlPr = oReader;
			}
		}
	}
	void CXmlCellPr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteString2(0, uniqueName);
		pWriter->WriteUInt2(1, id);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, xmlPr);
	}
	void CXmlCellPr::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				uniqueName = pReader->GetString2();
			}break;
			case 1:
			{
				id = pReader->GetULong();
			}break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				xmlPr.Init();
				xmlPr->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CXmlCellPr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"xmlCellPr");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"id", id);
		pWriter->WriteAttribute2(L"uniqueName", uniqueName);
		pWriter->EndAttributes();

		if (xmlPr.IsInit())
			xmlPr->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"xmlCellPr");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	EElementType CSingleXmlCell::getType() const
	{
		return et_x_SingleXmlCell;
	}
	void CSingleXmlCell::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_StartChar_No_NS(oReader)
			WritingElement_ReadAttributes_Read_ifChar(oReader, "connectionId", connectionId)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "id", id)
			WritingElement_ReadAttributes_Read_else_ifChar(oReader, "r", r)
		WritingElement_ReadAttributes_EndChar_No_NS(oReader)
	}
	void CSingleXmlCell::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes(oReader);

		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetNameNoNS();

			if (L"xmlCellPr" == sName)
			{
				xmlCellPr = oReader;
			}
		}
	}
	void CSingleXmlCell::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
		pWriter->WriteUInt2(0, connectionId);
		pWriter->WriteUInt2(1, id);
		pWriter->WriteString2(2, r);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		pWriter->WriteRecord2(0, xmlCellPr);
	}
	void CSingleXmlCell::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
		pReader->Skip(1); // start attributes
		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;
			switch (_at)
			{
			case 0:
			{
				connectionId = pReader->GetULong();
			}break;
			case 1:
			{
				id = pReader->GetULong();
			}break;
			case 2:
			{
				r = pReader->GetString2();
			}break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				xmlCellPr.Init();
				xmlCellPr->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(_end_rec);
	}
	void CSingleXmlCell::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"singleXmlCell");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"id", id);
		pWriter->WriteAttribute2(L"r", r);
		pWriter->WriteAttribute2(L"connectionId", connectionId);
		pWriter->EndAttributes();

		if (xmlCellPr.IsInit())
			xmlCellPr->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"singleXmlCell");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	void CSingleXmlCells::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		if (oReader.IsEmptyNode())
			return;

		int nParentDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nParentDepth))
		{
			std::wstring sName = oReader.GetNameNoNS();

			if (L"singleXmlCell" == sName)
			{
				CSingleXmlCell* pItem = new CSingleXmlCell();
				*pItem = oReader;

				if (pItem)
					m_arrItems.push_back(pItem);
			}
		}
	}
	EElementType CSingleXmlCells::getType() const
	{
		return et_x_SingleXmlCells;
	}
	void CSingleXmlCells::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();
			switch (_rec)
			{
			case 0:
			{
				m_arrItems.push_back(new CSingleXmlCell());
				m_arrItems.back()->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void CSingleXmlCells::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		for (size_t i = 0; i < m_arrItems.size(); ++i)
			pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
	}
	void CSingleXmlCells::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"singleXmlCells");
		pWriter->StartAttributes();
		pWriter->WriteAttribute2(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");
		pWriter->EndAttributes();

		for (size_t i = 0; i < m_arrItems.size(); ++i)
			m_arrItems[i]->toXmlWriter(pWriter);

		pWriter->WriteNodeEnd(L"singleXmlCells");
	}
//-----------------------------------------------------------------------------------------------------------------------------
	CTableSingleCellsFile::CTableSingleCellsFile(OOX::Document* pMain) : OOX::File(pMain)
	{
	}
	CTableSingleCellsFile::CTableSingleCellsFile(OOX::Document* pMain, const CPath& uri) : OOX::File(pMain)
	{
		read(uri.GetDirectory(), uri);
	}
	CTableSingleCellsFile::CTableSingleCellsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain)
	{
		read(oRootPath, oPath);
	}
	CTableSingleCellsFile::~CTableSingleCellsFile()
	{
	}
	void CTableSingleCellsFile::read(const CPath& oFilePath)
	{
		CPath oRootPath;
		read(oRootPath, oFilePath);
	}
	void CTableSingleCellsFile::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		XmlUtils::CXmlLiteReader oReader;

		if (!oReader.FromFile(oFilePath.GetPath()))
			return;

		if (!oReader.ReadNextNode())
			return;

		singleXmlCells = oReader;
	}
	void CTableSingleCellsFile::write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		NSBinPptxRW::CXmlWriter oXmlWriter;
		oXmlWriter.WriteString((L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"));

		oXmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_XLSX;
		
		if (singleXmlCells.IsInit())
			singleXmlCells->toXmlWriter(&oXmlWriter);

		NSFile::CFileBinary::SaveToFile(oFilePath.GetPath(), oXmlWriter.GetXmlString());

		oContent.Registration(type().OverrideType(), oDirectory, oFilePath.GetFilename());
	}
	const OOX::FileType CTableSingleCellsFile::type() const
	{
		return FileTypes::TableSingleCells;
	}
	const OOX::CPath CTableSingleCellsFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath CTableSingleCellsFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CTableSingleCellsFile::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
			case 0:
			{
				singleXmlCells.Init();
				singleXmlCells->fromPPTY(pReader);
			}break;
			default:
			{
				pReader->SkipRecord();
			}break;
			}
		}
		pReader->Seek(end);
	}
	void CTableSingleCellsFile::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteRecord2(0, singleXmlCells);
	}
} //Spreadsheet
} // namespace OOX
