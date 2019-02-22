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
#ifndef OOX_QUERYTABLE_FILE_INCLUDE_H_
#define OOX_QUERYTABLE_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "Autofilter.h"

namespace OOX
{
	namespace Spreadsheet
	{
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

				writer.WriteString(L"<queryTable \
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

				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
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
