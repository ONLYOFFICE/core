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

#include "../WritingElement.h"
#include "../../Base/Nullable.h"

namespace SimpleTypes
{
	class COnOff;
	class CUnsignedDecimalNumber;

	namespace Spreadsheet
	{
		class CHorizontalAlignment;
		class CVerticalAlignment;
	}
}

namespace OOX
{
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}
	namespace Spreadsheet
	{
		class CAligment : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CAligment)
            WritingElement_XlsbConstructors(CAligment)
			CAligment();
			virtual ~CAligment();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			void toBin(XLS::BaseObjectPtr& obj);

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
			WritingElement_AdditionMethods(CProtection)
            WritingElement_XlsbConstructors(CProtection)
			CProtection();
			virtual ~CProtection();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			void toBin(XLS::BaseObjectPtr& obj);
			void toXLS(XLS::BaseObjectPtr& obj);


			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::COnOff> m_oHidden;
			nullable<SimpleTypes::COnOff> m_oLocked;
		};

		class CXfs : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CXfs)
            WritingElement_XlsbConstructors(CXfs)
			CXfs();
			virtual ~CXfs();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			XLS::BaseObjectPtr toXLS();

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
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};

		class CCellXfs  : public WritingElementWithChilds<CXfs>
		{
		public:
			WritingElement_AdditionMethods(CCellXfs)
            WritingElement_XlsbVectorConstructors(CCellXfs)
			CCellXfs();
			virtual ~CCellXfs();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);
			XLS::BaseObjectPtr toBin();
			void toXLS(XLS::BaseObjectPtr Xfs);

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
			WritingElement_AdditionMethods(CCellStyleXfs)
            WritingElement_XlsbVectorConstructors(CCellStyleXfs)
			CCellStyleXfs();
			virtual ~CCellStyleXfs();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);
			XLS::BaseObjectPtr toBin();
			void toXLS(XLS::BaseObjectPtr Xfs);

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oCount;
		};

	} //Spreadsheet
} // namespace OOX

