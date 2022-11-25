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
			WritingElement_AdditionConstructors(CQueryTableFields)
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
			WritingElement_AdditionConstructors(CQueryTableDeletedField)
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
			WritingElement_AdditionConstructors(CQueryTableDeletedFields)
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
			WritingElement_AdditionConstructors(CQueryTableRefresh)
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
			WritingElement_AdditionConstructors(CQueryTable)
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
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
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
