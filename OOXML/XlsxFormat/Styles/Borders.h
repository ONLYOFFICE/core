﻿/*
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
