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
		class CPatternType;
		class CGradientType;
	}
}

namespace OOX
{
	namespace Spreadsheet
	{
		class CPatternFill : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CPatternFill)
            WritingElement_XlsbConstructors(CPatternFill)
			CPatternFill();
			virtual ~CPatternFill();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			void toBin(XLS::BaseObjectPtr obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::Spreadsheet::CPatternType>	m_oPatternType;
			nullable<CColor>									m_oBgColor;
			nullable<CColor>									m_oFgColor;
		};

		class CGradientStop : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CGradientStop)
			CGradientStop();
			virtual ~CGradientStop();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BiffStructure* obj);
			void toBin(XLS::BaseObjectPtr obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CDouble>		m_oPosition;
			nullable<CColor>					m_oColor;
		};

		class CGradientFill : public WritingElementWithChilds<CGradientStop>
		{
		public:
			WritingElement_AdditionMethods(CGradientFill)
            WritingElement_XlsbConstructors(CGradientFill)
			CGradientFill();
			virtual ~CGradientFill();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			void toBin(XLS::BaseObjectPtr obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::CDouble>		m_oBottom;
			nullable<SimpleTypes::CDouble>		m_oDegree;
			nullable<SimpleTypes::CDouble>		m_oLeft;
			nullable<SimpleTypes::CDouble>		m_oRight;
			nullable<SimpleTypes::CDouble>		m_oTop;
			nullable<SimpleTypes::Spreadsheet::CGradientType>		m_oType;
		};

		class CFill : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CFill)
            WritingElement_XlsbConstructors(CFill)
			CFill();
			virtual ~CFill();

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

		public:
			nullable<CGradientFill>		m_oGradientFill;
			nullable<CPatternFill>		m_oPatternFill;
		};

		class CFills : public WritingElementWithChilds<CFill>
		{
		public:
			WritingElement_AdditionMethods(CFills)
            WritingElement_XlsbVectorConstructors(CFills)
			CFills();
			virtual ~CFills();

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
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oCount;
			std::map<int, CFill*>								m_mapFills;
		};
	} //Spreadsheet
} // namespace OOX
