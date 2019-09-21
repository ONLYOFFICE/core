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
	void CAltTextTable::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("altText"),      m_oAltText )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("altTextSummary"),      m_oAltTextSummary )
		WritingElement_ReadAttributes_End( oReader )
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
	void CTableStyleInfo::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )

			WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),				m_oName )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("showColumnStripes"),	m_oShowColumnStripes )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("showFirstColumn"),		m_oShowFirstColumn )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("showLastColumn"),		m_oShowLastColumn )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("showRowStripes"),		m_oShowRowStripes )

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

			if ( _T("totalsRowFormula") == sName )
				m_oTotalsRowFormula = oReader.GetText3();
			else if ( _T("calculatedColumnFormula") == sName )
				m_oCalculatedColumnFormula = oReader.GetText3();
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

			if ( _T("tableColumn") == sName )
				m_arrItems.push_back(new CTableColumn(oReader));
		}
	}
	void CTableColumns::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, _T("count"), m_oCount )
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
		
		m_oId.Init();
		m_oId->SetValue((unsigned int)nIndex);

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

			if ( _T("autoFilter") == sName )
				m_oAutoFilter = oReader;
			else if ( _T("sortState") == sName )
				m_oSortState = oReader;
			else if ( _T("tableColumns") == sName )
				m_oTableColumns = oReader;
			else if ( _T("tableStyleInfo") == sName )
				m_oTableStyleInfo = oReader;
			else if (_T("extLst") == sName)
				m_oExtLst = oReader;
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
	void CTablePart::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),      m_oRId )
		WritingElement_ReadAttributes_End( oReader )
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

			if ( _T("tablePart") == sName )
				m_arrItems.push_back(new CTablePart(oReader));
		}
	}

	void CTableParts::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, _T("count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}

	void CTableFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read( oRootPath, oPath );

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

		NSStringUtils::CStringBuilder sXml;
		int nGlobalNumber = OOX::FileGlobalEnumerated::GetGlobalNumber();

		sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
		m_oTable->toXML2(sXml, nGlobalNumber);

        std::wstring sPath = oPath.GetPath();
        NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

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

			if (_T("extLst") == sName)
				m_oExtLst = oReader;
		}
	}
	void CQueryTableField::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, _T("id"),			m_oId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("tableColumnId"),	m_oTableColumnId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("name"),			m_oName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("rowNumbers"),	m_oRowNumbers )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("fillFormulas"),	m_oFillFormulas )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("dataBound"),		m_oDataBound )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("clipped"),		m_oClipped )
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
	void CQueryTableFields::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, _T("count"), m_oCount )
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
	void CQueryTableDeletedField::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if	( oReader, _T("name"), m_oName )
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
	void CQueryTableDeletedFields::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, _T("count"), m_oCount )
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

			if (_T("queryTableFields") == sName)
				m_oQueryTableFields = oReader;
			else if (_T("queryTableDeletedFields") == sName)
				m_oQueryTableDeletedFields = oReader;
			else if (_T("sortState") == sName)
				m_oSortState = oReader;
			else if (_T("extLst") == sName)
				m_oExtLst = oReader;
		}
	}
	void CQueryTableRefresh::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, _T("nextId"),				m_oNextId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("minimumVersion"),		m_oMinimumVersion )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("fieldIdWrapped"),		m_FieldIdWrapped )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("headersInLastRefresh"),	m_HeadersInLastRefresh )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("preserveSortFilterLayout"),	m_PreserveSortFilterLayout )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("unboundColumnsLeft"),	m_UnboundColumnsLeft )
			WritingElement_ReadAttributes_Read_else_if	( oReader, _T("unboundColumnsRight"),	m_UnboundColumnsRight )
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
			writer.WriteString(m_oExtLst->toXMLWithNS(_T("")));
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

			if ( _T("queryTableRefresh") == sName )
				m_oQueryTableRefresh = oReader;
			else if (_T("extLst") == sName)
				m_oExtLst = oReader;
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
	void CQueryTableFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read( oRootPath, oPath );

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
