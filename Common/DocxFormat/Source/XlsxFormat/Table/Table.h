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
#pragma once
#ifndef OOX_TABLE_FILE_INCLUDE_H_
#define OOX_TABLE_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "Autofilter.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CAltTextTable : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAltTextTable)
			CAltTextTable()
			{
			}
			virtual ~CAltTextTable()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<x14:table");
				WritingStringNullableAttrEncodeXmlString(L"altText", m_oAltText, m_oAltText.get());
				WritingStringNullableAttrEncodeXmlString(L"altTextSummary", m_oAltTextSummary, m_oAltTextSummary.get());
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_AltTextTable;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

						WritingElement_ReadAttributes_Read_if     ( oReader, _T("altText"),      m_oAltText )
						WritingElement_ReadAttributes_Read_if     ( oReader, _T("altTextSummary"),      m_oAltTextSummary )

						WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<std::wstring > m_oAltText;
			nullable<std::wstring > m_oAltTextSummary;
		};
		class CTableStyleInfo : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTableStyleInfo)
			CTableStyleInfo()
			{
			}
			virtual ~CTableStyleInfo()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_TableStyleInfo;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),				m_oName )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showColumnStripes"),	m_oShowColumnStripes )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showFirstColumn"),		m_oShowFirstColumn )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showLastColumn"),		m_oShowLastColumn )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showRowStripes"),		m_oShowRowStripes )

				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<std::wstring > m_oName;
			nullable<SimpleTypes::COnOff<> > m_oShowColumnStripes;
			nullable<SimpleTypes::COnOff<> > m_oShowFirstColumn;
			nullable<SimpleTypes::COnOff<> > m_oShowLastColumn;
			nullable<SimpleTypes::COnOff<> > m_oShowRowStripes;
		};
		class CTableColumn : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTableColumn)
			CTableColumn()
			{
			}
			virtual ~CTableColumn()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
                std::wstring sRoot;
				writer.WriteString(L"<tableColumn");
				WritingStringNullableAttrInt(L"id", m_oId, m_oId->GetValue());
				WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
				WritingStringNullableAttrEncodeXmlString(L"totalsRowLabel", m_oTotalsRowLabel, m_oTotalsRowLabel.get());
				//есть такой баг: при сохранениии "sum" и названия таблицы "Table1" (русский excel), выдается ошибка в формулах
				WritingStringNullableAttrString(L"totalsRowFunction", m_oTotalsRowFunction, m_oTotalsRowFunction->ToString());
				WritingStringNullableAttrInt(L"dataDxfId", m_oDataDxfId, m_oDataDxfId->GetValue());
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

			virtual EElementType getType () const
			{
				return et_x_TableColumn;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("id"),				m_oId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),			m_oName )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("totalsRowLabel"),	m_oTotalsRowLabel )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("totalsRowFunction"),m_oTotalsRowFunction )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("dataDxfId"),		m_oDataDxfId )

				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oId;
			nullable<std::wstring > m_oName;
			nullable<std::wstring > m_oTotalsRowLabel;
			nullable<SimpleTypes::Spreadsheet::CTotalsRowFunction<> > m_oTotalsRowFunction;
			nullable<std::wstring > m_oTotalsRowFormula;
			nullable<std::wstring > m_oCalculatedColumnFormula;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oDataDxfId;
		};
		class CTableColumns : public WritingElementWithChilds<CTableColumn>
		{
		public:
			WritingElement_AdditionConstructors(CTableColumns)
			CTableColumns()
			{
			}
			virtual ~CTableColumns()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

			virtual EElementType getType () const
			{
				return et_x_TableColumns;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oCount;
		};
		class CTable : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTable)
			CTable()
			{
			}
			virtual ~CTable()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<table \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"xr xr3\" \
xmlns:xr=\"http://schemas.microsoft.com/office/spreadsheetml/2014/revision\" \
xmlns:xr3=\"http://schemas.microsoft.com/office/spreadsheetml/2016/revision3\"");

				WritingStringNullableAttrInt(L"id", m_oId, m_oId->GetValue());
				WritingStringAttrEncodeXmlString(L"name", m_oDisplayName.get());
				WritingStringAttrEncodeXmlString(L"displayName", m_oDisplayName.get());
				WritingStringAttrString(L"ref", m_oRef->ToString());
				WritingStringNullableAttrInt(L"connectionId", m_oConnectionId, m_oConnectionId->GetValue());
				WritingStringNullableAttrString(L"tableType", m_oTableType, m_oTableType->ToString());
			
				WritingStringNullableAttrString(L"totalsRowDxfId", m_oComment, *m_oComment);
				WritingStringNullableAttrInt(L"totalsRowDxfId", m_oTotalsRowDxfId, m_oTotalsRowDxfId->GetValue());

				WritingStringNullableAttrInt(L"tableBorderDxfId", m_oTableBorderDxfId, m_oTableBorderDxfId->GetValue());
				WritingStringNullableAttrString(L"dataCellStyle", m_oDataCellStyle, *m_oDataCellStyle);
				WritingStringNullableAttrInt(L"headerRowBorderDxfId", m_oHeaderRowBorderDxfId, m_oHeaderRowBorderDxfId->GetValue());
				WritingStringNullableAttrString(L"headerRowCellStyle", m_oHeaderRowCellStyle, *m_oHeaderRowCellStyle);
				WritingStringNullableAttrInt(L"headerRowDxfId", m_oHeaderRowDxfId, m_oHeaderRowDxfId->GetValue());
				WritingStringNullableAttrInt(L"totalsRowBorderDxfId", m_oTotalsRowBorderDxfId, m_oTotalsRowBorderDxfId->GetValue());
				WritingStringNullableAttrInt(L"totalsRowDxfId", m_oTotalsRowDxfId, m_oTotalsRowDxfId->GetValue());
				WritingStringNullableAttrString(L"totalsRowCellStyle", m_oTotalsRowCellStyle, *m_oTotalsRowCellStyle);
			
				if(m_oHeaderRowCount.IsInit() && 0 == m_oHeaderRowCount->GetValue())
					writer.WriteString(L" headerRowCount=\"0\"");
				if(m_oTotalsRowCount.IsInit() && m_oTotalsRowCount->GetValue() > 0)
                    writer.WriteString(L" totalsRowCount=\"1\"");
                else
                    writer.WriteString(L" totalsRowShown=\"0\"");//m_oTotalsRowShown
                
                bool bInsertRow = m_oInsertRow.IsInit();                
                if (bInsertRow) {
                    WritingStringAttrString(L"insertRow",	*m_oInsertRow ? L"1" : L"0");
                }
                if (m_oInsertRowShift.IsInit())	{
                    WritingStringAttrString(L"insertRowShift",	*m_oInsertRowShift ? L"1" : L"0");
                }
                
                if (m_oPublished.IsInit()) {
                    WritingStringAttrString(L"published",	*m_oPublished ? L"1" : L"0");
                }
                
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
					writer.WriteString(m_oExtLst->toXMLWithNS(_T("")));
				}
				writer.WriteString(L"</table>");
			}
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, int nIndex)
			{
				if(false == m_oRef.IsInit() || false == m_oDisplayName.IsInit()) return;
				
				m_oId.Init();
				m_oId->SetValue((unsigned int)nIndex);

				toXML(writer);
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

			virtual EElementType getType () const
			{
				return et_x_Table;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"ref",					m_oRef )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"headerRowCount",		m_oHeaderRowCount )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"totalsRowCount",		m_oTotalsRowCount )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"displayName",			m_oDisplayName )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"tableBorderDxfId",		m_oTableBorderDxfId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"comment",				m_oComment )
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"connectionId",			m_oConnectionId )
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
		public:
			nullable<SimpleTypes::CRelationshipId >				m_oRef;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oHeaderRowCount;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oTotalsRowCount;
			nullable_string										m_oDisplayName;
			nullable_string										m_oName;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oTableBorderDxfId;

			nullable_string										m_oComment;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oConnectionId;
			nullable_string										m_oDataCellStyle;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oHeaderRowBorderDxfId;
			nullable_string										m_oHeaderRowCellStyle;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oHeaderRowDxfId;
			nullable_bool										m_oInsertRow;
			nullable_bool										m_oInsertRowShift;
			nullable_bool										m_oPublished;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oId;
			nullable<SimpleTypes::Spreadsheet::CTableType<>>	m_oTableType;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oTotalsRowBorderDxfId;
			nullable_string										m_oTotalsRowCellStyle;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oTotalsRowDxfId;
			nullable_bool										m_oTotalsRowShown;

			nullable<CAutofilter >								m_oAutoFilter;
			nullable<CSortState >								m_oSortState;
			nullable<CTableColumns >							m_oTableColumns;
			nullable<CTableStyleInfo >							m_oTableStyleInfo;

			nullable<OOX::Drawing::COfficeArtExtensionList>		m_oExtLst;
		};
		class CTablePart : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTablePart)
			CTablePart()
			{
			}
			virtual ~CTablePart()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_oRId.IsInit())
				{
					writer.WriteString(L"<tablePart");
					WritingStringAttrString(L"r:id", m_oRId->ToString());
					writer.WriteString(L"/>");
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_TablePart;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),      m_oRId )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oRId;
		};
		class CTableParts : public WritingElementWithChilds<CTablePart>
		{
		public:
			WritingElement_AdditionConstructors(CTableParts)
			CTableParts()
			{
			}
			virtual ~CTableParts()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

			virtual EElementType getType () const
			{
				return et_x_TableParts;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oCount;
		};
		//необработанные child:
		//<extLst>
		class CTableFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CTableFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
			}
			CTableFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				read( oRootPath, oPath );
			}
			virtual ~CTableFile()
			{
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
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
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				if(m_oTable.IsInit())
				{
					NSStringUtils::CStringBuilder sXml;
					int nGlobalNumber = OOX::FileGlobalEnumerated::GetGlobalNumber();

					m_oTable->toXML2(sXml, nGlobalNumber);

                    std::wstring sPath = oPath.GetPath();
                    NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

					oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
					IFileContainer::Write( oPath, oDirectory, oContent );
				}
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Table;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}

			nullable<CTable>	m_oTable;
		private:
			CPath				m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
//-------------------------------------------------------------------------------------------------
		class CQueryTableField : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CQueryTableField)
			
			CQueryTableField(){}
			virtual ~CQueryTableField() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<queryTableField");
					WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
					WritingStringNullableAttrInt(L"id", m_oId, m_oId->GetValue());
					WritingStringNullableAttrInt(L"tableColumnId", m_oTableColumnId, m_oTableColumnId->GetValue());

					if (m_oRowNumbers.IsInit())		WritingStringAttrString(L"rowNumbers",	*m_oRowNumbers ? L"1" : L"0");
					if (m_oFillFormulas.IsInit())	WritingStringAttrString(L"fillFormulas",*m_oFillFormulas ? L"1" : L"0");
					if (m_oDataBound.IsInit())		WritingStringAttrString(L"dataBound",	*m_oDataBound ? L"1" : L"0");
					if (m_oClipped.IsInit())		WritingStringAttrString(L"clipped",		*m_oClipped ? L"1" : L"0");


				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

			virtual EElementType getType () const
			{
				return et_x_QueryTableField;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("id"),			m_oId )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("tableColumnId"),	m_oTableColumnId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("name"),			m_oName )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("rowNumbers"),	m_oRowNumbers )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("fillFormulas"),	m_oFillFormulas )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("dataBound"),		m_oDataBound )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("clipped"),		m_oClipped )
				WritingElement_ReadAttributes_End( oReader )
			}

			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oId;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oTableColumnId;
			nullable<std::wstring>								m_oName;
			
			nullable_bool	m_oRowNumbers;
			nullable_bool	m_oFillFormulas;
			nullable_bool	m_oDataBound;
			nullable_bool	m_oClipped;
			
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};
		class CQueryTableFields : public WritingElementWithChilds<CQueryTableField>
		{
		public:
			WritingElement_AdditionConstructors(CQueryTableFields)
			
			CQueryTableFields(){}
			virtual ~CQueryTableFields() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
			virtual EElementType getType () const
			{
				return et_x_QueryTableFields;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("count"), m_oCount )
				WritingElement_ReadAttributes_End( oReader )
			}

			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oCount;
		};

		class CQueryTableDeletedField : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CQueryTableDeletedField)
			
			CQueryTableDeletedField(){}
			virtual ~CQueryTableDeletedField() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<deletedField");
					WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
				writer.WriteString(L"/>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

			}
			virtual EElementType getType () const
			{
				return et_x_QueryTableDeletedField;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if	( oReader, _T("name"), m_oName )
				WritingElement_ReadAttributes_End( oReader )
			}
			nullable<std::wstring> m_oName;
		};
		class CQueryTableDeletedFields : public WritingElementWithChilds<CQueryTableDeletedField>
		{
		public:
			WritingElement_AdditionConstructors(CQueryTableDeletedFields)
			
			CQueryTableDeletedFields(){}
			virtual ~CQueryTableDeletedFields() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
			virtual EElementType getType () const
			{
				return et_x_QueryTableFields;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, _T("count"), m_oCount )
				WritingElement_ReadAttributes_End( oReader )
			}

			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oCount;
		};
		class CQueryTableRefresh : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CQueryTableRefresh)
			
			CQueryTableRefresh(){}
			virtual ~CQueryTableRefresh() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<queryTableRefresh");
					WritingStringNullableAttrInt(L"nextId", m_oNextId, m_oNextId->GetValue());
					WritingStringNullableAttrInt(L"unboundColumnsLeft", m_UnboundColumnsLeft, m_UnboundColumnsLeft->GetValue());
					WritingStringNullableAttrInt(L"unboundColumnsRight", m_UnboundColumnsRight, m_UnboundColumnsRight->GetValue());

					if (m_FieldIdWrapped.IsInit())			WritingStringAttrString(L"fieldIdWrapped",	*m_FieldIdWrapped ? L"1" : L"0");
					if (m_HeadersInLastRefresh.IsInit())	WritingStringAttrString(L"headersInLastRefresh",	*m_HeadersInLastRefresh ? L"1" : L"0");
					if (m_PreserveSortFilterLayout.IsInit())WritingStringAttrString(L"preserveSortFilterLayout",	*m_PreserveSortFilterLayout ? L"1" : L"0");
					if (m_oMinimumVersion.IsInit())			WritingStringAttrInt(L"minimumVersion",	m_oMinimumVersion->GetValue());
				writer.WriteString(L">");

				if (m_oQueryTableFields.IsInit())
					m_oQueryTableFields->toXML(writer);

				if (m_oQueryTableDeletedFields.IsInit())
					m_oQueryTableDeletedFields->toXML(writer);

				if (m_oSortState.IsInit())
					m_oSortState->toXML(writer);

				writer.WriteString(L"</queryTableRefresh");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

			virtual EElementType getType () const
			{
				return et_x_QueryTableRefresh;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("id"),					m_oNextId )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("minimumVersion"),		m_oMinimumVersion )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("fieldIdWrapped"),		m_FieldIdWrapped )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("headersInLastRefresh"),	m_HeadersInLastRefresh )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("preserveSortFilterLayout"),	m_PreserveSortFilterLayout )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("unboundColumnsLeft"),		m_UnboundColumnsLeft )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("unboundColumnsRight"),		m_UnboundColumnsRight )
				WritingElement_ReadAttributes_End( oReader )
			}

			
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oNextId;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oMinimumVersion;			
			nullable_bool										m_FieldIdWrapped;
			nullable_bool										m_HeadersInLastRefresh;
			nullable_bool										m_PreserveSortFilterLayout;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_UnboundColumnsLeft;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_UnboundColumnsRight;
			
			nullable<CQueryTableFields>						m_oQueryTableFields;
			nullable<CQueryTableDeletedFields>				m_oQueryTableDeletedFields;
			nullable<CSortState>							m_oSortState;
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};
		class CQueryTable : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CQueryTable)
			CQueryTable()
			{
			}
			virtual ~CQueryTable()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(false == m_oName.IsInit()) return;

				writer.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<queryTable \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"xr16\" \
xmlns:xr16=\"http://schemas.microsoft.com/office/spreadsheetml/2017/revision16\"");

				if (m_oAdjustColumnWidth.IsInit())		WritingStringAttrString(L"adjustColumnWidth",		*m_oAdjustColumnWidth ? L"1" : L"0");
				if (m_oApplyAlignmentFormats.IsInit())	WritingStringAttrString(L"applyAlignmentFormats",	*m_oApplyAlignmentFormats ? L"1" : L"0");
				if (m_oApplyBorderFormats.IsInit())		WritingStringAttrString(L"applyBorderFormats",		*m_oApplyBorderFormats ? L"1" : L"0");
				if (m_oApplyFontFormats.IsInit())		WritingStringAttrString(L"applyFontFormats",		*m_oApplyFontFormats ? L"1" : L"0");
				if (m_oApplyNumberFormats.IsInit())		WritingStringAttrString(L"applyNumberFormats",		*m_oApplyNumberFormats ? L"1" : L"0");
				if (m_oApplyPatternFormats.IsInit())		WritingStringAttrString(L"applyPatternFormats",		*m_oApplyPatternFormats ? L"1" : L"0");
				if (m_oApplyWidthHeightFormats.IsInit())	WritingStringAttrString(L"applyWidthHeightFormats", *m_oApplyWidthHeightFormats ? L"1" : L"0");
				if (m_oBackgroundRefresh.IsInit())		WritingStringAttrString(L"backgroundRefresh",		*m_oBackgroundRefresh ? L"1" : L"0");

				WritingStringNullableAttrEncodeXmlString(L"name",	m_oName, m_oName.get());
				WritingStringNullableAttrInt(L"connectionId",		m_oConnectionId, m_oConnectionId->GetValue());
				WritingStringNullableAttrInt(L"autoFormatId",		m_oAutoFormatId, m_oAutoFormatId->GetValue());

				if (m_oDisableEdit.IsInit())			WritingStringAttrString(L"disableEdit",		*m_oDisableEdit ? L"1" : L"0");
				if (m_oDisableRefresh.IsInit())			WritingStringAttrString(L"disableRefresh",	*m_oDisableRefresh ? L"1" : L"0");
				if (m_oFillFormulas.IsInit())			WritingStringAttrString(L"fillFormulas",	*m_oFillFormulas ? L"1" : L"0");
				if (m_oFirstBackgroundRefresh.IsInit())	WritingStringAttrString(L"firstBackgroundRefresh", *m_oFirstBackgroundRefresh ? L"1" : L"0");

				WritingStringNullableAttrEncodeXmlString(L"growShrinkType",	m_oGrowShrinkType, m_oGrowShrinkType.get());

				if (m_oHeaders.IsInit())			WritingStringAttrString(L"headers",			*m_oHeaders ? L"1" : L"0");
				if (m_oIntermediate.IsInit())		WritingStringAttrString(L"intermediate",	*m_oIntermediate ? L"1" : L"0");
				if (m_oPreserveFormatting.IsInit())	WritingStringAttrString(L"preserveFormatting", *m_oPreserveFormatting ? L"1" : L"0");
				if (m_oRefreshOnLoad.IsInit())		WritingStringAttrString(L"refreshOnLoad",	*m_oRefreshOnLoad ? L"1" : L"0");
				if (m_oRemoveDataOnSave.IsInit())	WritingStringAttrString(L"removeDataOnSave", *m_oRemoveDataOnSave ? L"1" : L"0");
				if (m_oRowNumbers.IsInit())			WritingStringAttrString(L"rowNumbers",		*m_oRowNumbers ? L"1" : L"0");
				
				writer.WriteString(L">");

				if(m_oQueryTableRefresh.IsInit())
					m_oQueryTableRefresh->toXML(writer);
				if(m_oExtLst.IsInit())
				{
					writer.WriteString(m_oExtLst->toXMLWithNS(_T("")));
				}
				writer.WriteString(L"</queryTable>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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
			virtual EElementType getType () const
			{
				return et_x_QueryTable;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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

			nullable_bool	m_oAdjustColumnWidth;
			nullable_bool	m_oApplyAlignmentFormats;
			nullable_bool	m_oApplyBorderFormats;
			nullable_bool	m_oApplyFontFormats;
			nullable_bool	m_oApplyNumberFormats;
			nullable_bool	m_oApplyPatternFormats;
			nullable_bool	m_oApplyWidthHeightFormats;
			nullable_bool	m_oBackgroundRefresh;
			
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oAutoFormatId;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oConnectionId;
			
			nullable_bool	m_oDisableEdit;
			nullable_bool	m_oDisableRefresh;
			nullable_bool	m_oFillFormulas;
			nullable_bool	m_oFirstBackgroundRefresh;

			nullable_string	m_oGrowShrinkType;

			nullable_bool	m_oHeaders;
			nullable_bool	m_oIntermediate;

			nullable_string	m_oName;

			nullable_bool	m_oPreserveFormatting;
			nullable_bool	m_oRefreshOnLoad;
			nullable_bool	m_oRemoveDataOnSave;
			nullable_bool	m_oRowNumbers;

			nullable<CQueryTableRefresh> m_oQueryTableRefresh;
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};
		class CQueryTableFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CQueryTableFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
			}
			CQueryTableFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				read( oRootPath, oPath );
			}
			virtual ~CQueryTableFile()
			{
			}
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath)
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
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
			{
				if(false == m_oQueryTable.IsInit()) return;

				NSStringUtils::CStringBuilder sXml;

				m_oQueryTable->toXML(sXml);

                std::wstring sPath = oPath.GetPath();
                NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

				oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
				IFileContainer::Write( oPath, oDirectory, oContent );
			}
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::QueryTable;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}

			nullable<CQueryTable>	m_oQueryTable;
		private:
			CPath				m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_TABLE_FILE_INCLUDE_H_
