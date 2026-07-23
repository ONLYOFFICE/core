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
#include "../WritingElement.h"
#include "../../Base/Nullable.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CFont : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CFont)
            WritingElement_XlsbConstructors(CFont)
			CFont();
			virtual ~CFont();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			XLS::BaseObjectPtr toXLS();

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oBold;
            nullable<CCharset>														m_oCharset;
			nullable<CColor>														m_oColor;
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oCondense;
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oExtend;
			nullable<CFontFamily >													m_oFamily;
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oItalic;
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oOutline;
			nullable<ComplexTypes::Spreadsheet::String>								m_oRFont;
			nullable<CFontScheme>													m_oScheme;
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oShadow;
			nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oStrike;
			nullable<ComplexTypes::Spreadsheet::CDouble>							m_oSz;
			nullable<CUnderline>													m_oUnderline;
			nullable<CVerticalAlign>												m_oVertAlign;
		};

		class CFonts : public WritingElementWithChilds<CFont>
		{
		public:
			WritingElement_AdditionMethods(CFonts)
            WritingElement_XlsbVectorConstructors(CFonts)
			CFonts();
			virtual ~CFonts();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void fromBin(std::vector<XLS::BaseObjectPtr>& obj);
			XLS::BaseObjectPtr toBin();
			std::vector<XLS::BaseObjectPtr> toXLS();
			virtual EElementType getType () const;

			void AddFont (CFont* pFont);

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber>	m_oCount;
			std::map<int, CFont*>							m_mapFonts;
		};

	} //Spreadsheet
} // namespace OOX

