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

#include "rPr.h"


namespace OOX
{
	namespace Spreadsheet
	{
		class CPatternFill : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPatternFill)
            WritingElement_XlsbConstructors(CPatternFill)
			CPatternFill();
			virtual ~CPatternFill();

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
			nullable<SimpleTypes::Spreadsheet::CPatternType>	m_oPatternType;
			nullable<CColor>									m_oBgColor;
			nullable<CColor>									m_oFgColor;
		};

		class CGradientStop : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGradientStop)
			CGradientStop();
			virtual ~CGradientStop();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BiffStructure* obj);

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
			WritingElement_AdditionConstructors(CGradientFill)
            WritingElement_XlsbConstructors(CGradientFill)
			CGradientFill();
			virtual ~CGradientFill();

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
			WritingElement_AdditionConstructors(CFill)
            WritingElement_XlsbConstructors(CFill)
			CFill();
			virtual ~CFill();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

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
			WritingElement_AdditionConstructors(CFills)
            WritingElement_XlsbVectorConstructors(CFills)
			CFills();
			virtual ~CFills();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);
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
