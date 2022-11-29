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


namespace OOX
{
	namespace Spreadsheet
	{
		//Sort
		class CSortCondition : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSortCondition)
			WritingElement_XlsbConstructors(CSortCondition)
			CSortCondition();
			virtual ~CSortCondition();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::COnOff>					m_oDescending;
			nullable<SimpleTypes::CRelationshipId>			m_oRef;
			nullable<SimpleTypes::Spreadsheet::CSortBy>		m_oSortBy;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oDxfId;
		};

		class CSortState : public WritingElementWithChilds<CSortCondition>
		{
		public:
			WritingElement_AdditionConstructors(CSortState)
			WritingElement_XlsbConstructors(CSortState)
			CSortState();
			virtual ~CSortState();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::CRelationshipId>				m_oRef;
			nullable<SimpleTypes::COnOff>						m_oCaseSensitive;
			nullable<SimpleTypes::COnOff>						m_oColumnSort;
			nullable<SimpleTypes::Spreadsheet::CSortMethod>		m_oSortMethod;
		};

		//Filters
		class CColorFilter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CColorFilter)
			WritingElement_XlsbConstructors(CColorFilter)
			CColorFilter();
			virtual ~CColorFilter();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::COnOff> m_oCellColor;
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oDxfId;
		};

		class CDynamicFilter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDynamicFilter)
			WritingElement_XlsbConstructors(CDynamicFilter)
			CDynamicFilter();
			virtual ~CDynamicFilter();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::Spreadsheet::CDynamicFilterType > m_oType;
			nullable<SimpleTypes::CDouble > m_oVal;
			nullable<SimpleTypes::CDouble > m_oMaxVal;
		};

		class CCustomFilter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CCustomFilter)
			WritingElement_XlsbConstructors(CCustomFilter)
			CCustomFilter();
			virtual ~CCustomFilter();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::Spreadsheet::CCustomFilter > m_oOperator;
			nullable<std::wstring > m_oVal;
		};

		class CCustomFilters : public WritingElementWithChilds<CCustomFilter>
		{
		public:
			WritingElement_AdditionConstructors(CCustomFilters)
			WritingElement_XlsbConstructors(CCustomFilters)
			CCustomFilters();
			virtual ~CCustomFilters();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::COnOff> m_oAnd;
		};

		class CFilter : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFilter)
			WritingElement_XlsbConstructors(CFilter)
			CFilter();
			virtual ~CFilter();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);;

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<std::wstring > m_oVal;
		};

		class CDateGroupItem : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CDateGroupItem)
			WritingElement_XlsbConstructors(CDateGroupItem)
			CDateGroupItem();
			virtual ~CDateGroupItem();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::Spreadsheet::CDateTimeGroup > m_oDateTimeGrouping;
			nullable<SimpleTypes::CUnsignedDecimalNumber > m_oDay;
			nullable<SimpleTypes::CUnsignedDecimalNumber > m_oHour;
			nullable<SimpleTypes::CUnsignedDecimalNumber > m_oMinute;
			nullable<SimpleTypes::CUnsignedDecimalNumber > m_oMonth;
			nullable<SimpleTypes::CUnsignedDecimalNumber > m_oSecond;
			nullable<SimpleTypes::CUnsignedDecimalNumber > m_oYear;
		};

		class CFilters : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CFilters)
			WritingElement_XlsbConstructors(CFilters)
			CFilters();
			virtual ~CFilters();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::COnOff> m_oBlank;
		};

		class CTop10 : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTop10)
			WritingElement_XlsbConstructors(CTop10)
			CTop10();
			virtual ~CTop10();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::CDouble>	m_oFilterVal;
			nullable<SimpleTypes::COnOff>	m_oPercent;
			nullable<SimpleTypes::COnOff>	m_oTop;
			nullable<SimpleTypes::CDouble>	m_oVal;
		};

		class CFilterColumn : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFilterColumn)
			WritingElement_XlsbConstructors(CFilterColumn)
			CFilterColumn();
			virtual ~CFilterColumn();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oColId;
			nullable<SimpleTypes::COnOff>					m_oHiddenButton;
			nullable<SimpleTypes::COnOff>					m_oShowButton;

			nullable<CColorFilter > m_oColorFilter;
			nullable<CDynamicFilter > m_oDynamicFilter;
			nullable<CCustomFilters > m_oCustomFilters;
			nullable<CFilters > m_oFilters;
			nullable<CTop10 > m_oTop10;
		};

		class CAutofilter : public WritingElementWithChilds<CFilterColumn>
		{
		public:
			WritingElement_AdditionConstructors(CAutofilter)
			WritingElement_XlsbConstructors(CAutofilter)
			CAutofilter();
			virtual ~CAutofilter();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::CRelationshipId > m_oRef;
			nullable<CSortState >					m_oSortState;
		};
	} //Spreadsheet
} // namespace OOX
