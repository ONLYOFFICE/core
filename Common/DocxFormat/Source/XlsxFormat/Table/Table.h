/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
		class CTableStyleInfo : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CTableStyleInfo)
			CTableStyleInfo()
			{
			}
			virtual ~CTableStyleInfo()
			{
			}

		public:
            virtual std::wstring      toXML() const
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
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_TableStyleInfo;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),      m_oName )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showColumnStripes"),      m_oShowColumnStripes )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showFirstColumn"),      m_oShowFirstColumn )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showLastColumn"),      m_oShowLastColumn )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("showRowStripes"),      m_oShowRowStripes )

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
			WritingElementSpreadsheet_AdditionConstructors(CTableColumn)
			CTableColumn()
			{
			}
			virtual ~CTableColumn()
			{
			}

		public:
            virtual std::wstring      toXML() const
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

					if(m_oTotalsRowFormula.IsInit())
					{
						WritingStringValEncodeXmlString(L"totalsRowFormula", m_oTotalsRowFormula.get());
					}
					if(m_oCalculatedColumnFormula.IsInit())
					{
						WritingStringValEncodeXmlString(L"calculatedColumnFormula", m_oCalculatedColumnFormula.get());
					}

					writer.WriteString(L"</tableColumn>");
				}
				else
				{
					writer.WriteString(L"/>");
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
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
				return et_TableColumn;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("id"),      m_oId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),      m_oName )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("totalsRowLabel"),      m_oTotalsRowLabel )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("totalsRowFunction"),      m_oTotalsRowFunction )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("dataDxfId"),      m_oDataDxfId )

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
			WritingElementSpreadsheet_AdditionConstructors(CTableColumns)
			CTableColumns()
			{
			}
			virtual ~CTableColumns()
			{
			}

		public:
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_arrItems.size() > 0)
				{
					writer.WriteString(L"<tableColumns");
					WritingStringAttrInt(L"count", m_arrItems.size());
					writer.WriteString(L">");
					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(L"</tableColumns>");
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
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
				return et_TableColumns;
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
			WritingElementSpreadsheet_AdditionConstructors(CTable)
			CTable()
			{
			}
			virtual ~CTable()
			{
			}

		public:
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
			}
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, int nIndex) const
			{
				if(m_oRef.IsInit() && m_oDisplayName.IsInit())
				{
					writer.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><table xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\"");
					WritingStringAttrInt(L"id", nIndex);
					WritingStringAttrEncodeXmlString(L"name", m_oDisplayName.get());
					WritingStringAttrEncodeXmlString(L"displayName", m_oDisplayName.get());
					WritingStringAttrString(L"ref", m_oRef->ToString());
					if(m_oHeaderRowCount.IsInit() && 0 == m_oHeaderRowCount->GetValue())
						writer.WriteString(L" headerRowCount=\"0\"");
					if(m_oTotalsRowCount.IsInit() && m_oTotalsRowCount->GetValue() > 0)
						writer.WriteString(L" totalsRowCount=\"1\"");
					else
						writer.WriteString(L" totalsRowShown=\"0\"");
					WritingStringNullableAttrInt(L"tableBorderDxfId", m_oTableBorderDxfId, m_oTableBorderDxfId->GetValue());
					writer.WriteString(L">");

					if(m_oAutoFilter.IsInit())
						m_oAutoFilter->toXML(writer);
					if(m_oSortState.IsInit())
						m_oSortState->toXML(writer);
					if(m_oTableColumns.IsInit())
						m_oTableColumns->toXML(writer);
					if(m_oTableStyleInfo.IsInit())
						m_oTableStyleInfo->toXML(writer);

					writer.WriteString(L"</table>");
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
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
				}
			}

			virtual EElementType getType () const
			{
				return et_Table;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ref"),      m_oRef )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("headerRowCount"),      m_oHeaderRowCount )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("totalsRowCount"),      m_oTotalsRowCount )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("displayName"),      m_oDisplayName )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("tableBorderDxfId"),      m_oTableBorderDxfId )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CRelationshipId > m_oRef;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oHeaderRowCount;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oTotalsRowCount;
			nullable<std::wstring > m_oDisplayName;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oTableBorderDxfId;

			nullable<CAutofilter > m_oAutoFilter;
			nullable<CSortState > m_oSortState;
			nullable<CTableColumns > m_oTableColumns;
			nullable<CTableStyleInfo > m_oTableStyleInfo;
		};
		class CTablePart : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CTablePart)
			CTablePart()
			{
			}
			virtual ~CTablePart()
			{
			}

		public:
            virtual std::wstring      toXML() const
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
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_TablePart;
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
			WritingElementSpreadsheet_AdditionConstructors(CTableParts)
			CTableParts()
			{
			}
			virtual ~CTableParts()
			{
			}

		public:
            virtual std::wstring      toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				if(m_arrItems.size() > 0)
				{
					writer.WriteString(L"<tableParts");
					WritingStringAttrInt(L"count", m_arrItems.size());
					writer.WriteString(L">");
					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(L"</tableParts>");	
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
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
				return et_TableParts;
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
		class CTableFile : public OOX::FileGlobalEnumerated, public OOX::Spreadsheet::IFileContainer
		{
		public:
			CTableFile()
			{
			}
			CTableFile(const CPath& oRootPath, const CPath& oPath)
			{
				read( oRootPath, oPath );
			}
			virtual ~CTableFile()
			{
			}
		public:

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

		private:
			CPath									m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

		public:
			nullable<CTable>         m_oTable;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_TABLE_FILE_INCLUDE_H_
