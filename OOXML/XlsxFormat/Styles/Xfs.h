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
		class CAligment : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAligment)
            WritingElement_XlsbConstructors(CAligment)
			CAligment();
			virtual ~CAligment();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

			bool IsEmpty();

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::Spreadsheet::CHorizontalAlignment>		m_oHorizontal;
			nullable_uint													m_oIndent;
			nullable<SimpleTypes::COnOff>									m_oJustifyLastLine;
			nullable_uint													m_oReadingOrder; //todooo to simple
			nullable_int													m_oRelativeIndent;
			nullable<SimpleTypes::COnOff>									m_oShrinkToFit;
			nullable_uint													m_oTextRotation;
			nullable<SimpleTypes::Spreadsheet::CVerticalAlignment>			m_oVertical;
			nullable<SimpleTypes::COnOff>									m_oWrapText;
		};

		class CProtection : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CProtection)
            WritingElement_XlsbConstructors(CProtection)
			CProtection();
			virtual ~CProtection();

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
			nullable<SimpleTypes::COnOff> m_oHidden;
			nullable<SimpleTypes::COnOff> m_oLocked;
		};

		//нереализован:
		//<extLst>
		class CXfs : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CXfs)
            WritingElement_XlsbConstructors(CXfs)
			CXfs();
			virtual ~CXfs();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

			void fromBin(XLS::BaseObjectPtr& obj);

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::COnOff>					m_oApplyAlignment;
			nullable<SimpleTypes::COnOff>					m_oApplyBorder;
			nullable<SimpleTypes::COnOff>					m_oApplyFill;
			nullable<SimpleTypes::COnOff>					m_oApplyFont;
			nullable<SimpleTypes::COnOff>					m_oApplyNumberFormat;
			nullable<SimpleTypes::COnOff>					m_oApplyProtection;

			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oBorderId;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oFillId;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oFontId;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oNumFmtId;
			nullable<SimpleTypes::COnOff>					m_oPivotButton;
			nullable<SimpleTypes::COnOff>					m_oQuotePrefix;
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oXfId;

			nullable<CAligment>								m_oAligment;
			nullable<CProtection>							m_oProtection;
			
		};

		class CCellXfs  : public WritingElementWithChilds<CXfs>
		{
		public:
			WritingElement_AdditionConstructors(CCellXfs)
            WritingElement_XlsbVectorConstructors(CCellXfs)
			CCellXfs();
			virtual ~CCellXfs();

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
		};

		class CCellStyleXfs  : public WritingElementWithChilds<CXfs>
		{
		public:
			WritingElement_AdditionConstructors(CCellStyleXfs)
            WritingElement_XlsbVectorConstructors(CCellStyleXfs)
			CCellStyleXfs();
			virtual ~CCellStyleXfs();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oCount;
		};

	} //Spreadsheet
} // namespace OOX

