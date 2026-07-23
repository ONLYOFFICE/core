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

#include "../Base/Nullable.h"
#include "WritingElement.h"

#include "../Common/SimpleTypes_Word.h"

// TO DO: Missing classes:
//        <w:customXml>
//        <w:del>
//        <w:ins>
//        <w:moveFrom>
//        <w:moveTo>

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CFtnEdn 17.11.2 && 17.11.10 (Part 1)
	//--------------------------------------------------------------------------------	
	class CFtnEdn : public WritingElementWithChilds<>
	{
	public:
		WritingElement_AdditionMethods(CFtnEdn)
		CFtnEdn(OOX::Document *pMain = NULL);
		virtual ~CFtnEdn();

		virtual void ClearItems();

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

		OOX::EElementType                        m_eType; // <w:footnote> or <w:endnote>

		nullable<SimpleTypes::CDecimalNumber>	m_oId;
		nullable<SimpleTypes::CFtnEdn>			m_oType;
	};

	//--------------------------------------------------------------------------------
	// CFtnEdnSepRef 17.11.3 && 17.11.9 (Part 1)
	//--------------------------------------------------------------------------------	
	class CFtnEdnSepRef : public WritingElement
	{
	public:
		CFtnEdnSepRef();
		CFtnEdnSepRef(XmlUtils::CXmlNode& oNode);
		CFtnEdnSepRef(XmlUtils::CXmlLiteReader& oReader);
		virtual ~CFtnEdnSepRef();

		const CFtnEdnSepRef& operator =(const XmlUtils::CXmlNode& oNode);
		const CFtnEdnSepRef& operator =(const XmlUtils::CXmlLiteReader& oReader);

		virtual void fromXML(XmlUtils::CXmlNode& oNode);
		virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

		virtual std::wstring toXML() const;
		virtual EElementType getType() const;

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

	public:
		OOX::EElementType m_eType; // <w:footnote> or <w:endnote>

		nullable<SimpleTypes::CDecimalNumber> m_oId;
	};

} // namespace OOX
