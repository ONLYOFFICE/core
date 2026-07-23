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

#include "rPr.h"

namespace XLS
{
	class BiffStructure;
}

namespace SimpleTypes
{
	namespace Spreadsheet
	{
		class CBorderStyle;
	}
}

namespace OOX
{
	namespace Spreadsheet
	{
		class CBorderProp : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CBorderProp)
			CBorderProp();
			virtual ~CBorderProp();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual EElementType getType () const;

			void fromBin(XLS::BiffStructure* obj);
			void toBin(XLS::BiffStructure* obj);
			bool IsEmpty();

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::Spreadsheet::CBorderStyle>	m_oStyle;
			nullable<CColor>									m_oColor;
			nullable_string										m_oType;

			bool bBorderContinuous = false; // merge cells border (2003)
		};

		class CBorder : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CBorder)
            WritingElement_XlsbConstructors(CBorder)
			CBorder();
			virtual ~CBorder();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::COnOff>	m_oDiagonalDown;
			nullable<SimpleTypes::COnOff>	m_oDiagonalUp;
			nullable<SimpleTypes::COnOff>	m_oOutline;

			nullable<CBorderProp>	m_oBottom;
			nullable<CBorderProp>	m_oDiagonal;
			nullable<CBorderProp>	m_oEnd;
			nullable<CBorderProp>	m_oHorizontal;
			nullable<CBorderProp>	m_oStart;
			nullable<CBorderProp>	m_oTop;
			nullable<CBorderProp>	m_oVertical;

			bool bBorderContinuous = false;
		};

		class CBorders : public WritingElementWithChilds<CBorder>
		{
		public:
			WritingElement_AdditionMethods(CBorders)
            WritingElement_XlsbVectorConstructors(CBorders)
			CBorders();
			virtual ~CBorders();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;
			std::map<int, CBorder*>							m_mapBorders;
		};
	} //Spreadsheet
} // namespace OOX
