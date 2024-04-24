﻿/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

#include "../Base/Nullable.h"
#include "WritingElement.h"

#include "../Common/SimpleTypes_Word.h"

// TO DO: Нехватающие классы:
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

		OOX::EElementType                        m_eType; // <w:footnote> или <w:endnote>

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
		OOX::EElementType m_eType; // <w:footnote> или <w:endnote>

		nullable<SimpleTypes::CDecimalNumber> m_oId;
	};

} // namespace OOX
