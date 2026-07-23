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

#include "Autofilter.h"

namespace OOX
{
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}

	namespace Spreadsheet
	{
		class CQueryTableField : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CQueryTableField)
			WritingElement_XlsbConstructors(CQueryTableField)

			CQueryTableField(){}
			virtual ~CQueryTableField() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
                        void fromBin(XLS::BaseObjectPtr& obj);
						XLS::BaseObjectPtr toBin();
			virtual EElementType getType () const
			{
				return et_x_QueryTableField;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oId;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oTableColumnId;
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
			WritingElement_AdditionMethods(CQueryTableFields)
			WritingElement_XlsbConstructors(CQueryTableFields)

			CQueryTableFields() {}
			virtual ~CQueryTableFields() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType () const
			{
				return et_x_QueryTableFields;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};

		class CQueryTableDeletedField : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CQueryTableDeletedField)
			WritingElement_XlsbConstructors(CQueryTableDeletedField)

			CQueryTableDeletedField() {}
			virtual ~CQueryTableDeletedField() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType () const
			{
				return et_x_QueryTableDeletedField;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			
			nullable<std::wstring> m_oName;
		};

		class CQueryTableDeletedFields : public WritingElementWithChilds<CQueryTableDeletedField>
		{
		public:
			WritingElement_AdditionMethods(CQueryTableDeletedFields)
			WritingElement_XlsbConstructors(CQueryTableDeletedFields)
			CQueryTableDeletedFields() {}
			virtual ~CQueryTableDeletedFields() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType () const
			{
				return et_x_QueryTableFields;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};

		class CQueryTableRefresh : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CQueryTableRefresh)
			WritingElement_XlsbConstructors(CQueryTableRefresh)
			CQueryTableRefresh() {}
			virtual ~CQueryTableRefresh() {}
			
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType () const
			{
				return et_x_QueryTableRefresh;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oNextId;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oMinimumVersion;
			nullable_bool										m_FieldIdWrapped;
			nullable_bool										m_HeadersInLastRefresh;
			nullable_bool										m_PreserveSortFilterLayout;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_UnboundColumnsLeft;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_UnboundColumnsRight;
			
			nullable<CQueryTableFields>						m_oQueryTableFields;
			nullable<CQueryTableDeletedFields>				m_oQueryTableDeletedFields;
			nullable<CSortState>							m_oSortState;
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};

		class CQueryTable : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CQueryTable)
			WritingElement_XlsbConstructors(CQueryTable)
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
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType () const
			{
				return et_x_QueryTable;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			
			nullable_bool	m_oAdjustColumnWidth;
			nullable_bool	m_oApplyAlignmentFormats;
			nullable_bool	m_oApplyBorderFormats;
			nullable_bool	m_oApplyFontFormats;
			nullable_bool	m_oApplyNumberFormats;
			nullable_bool	m_oApplyPatternFormats;
			nullable_bool	m_oApplyWidthHeightFormats;
			nullable_bool	m_oBackgroundRefresh;
			
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oAutoFormatId;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oConnectionId;
			
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
                        void readBin(const CPath& oPath);
			XLS::BaseObjectPtr WriteBin() const;
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;
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
