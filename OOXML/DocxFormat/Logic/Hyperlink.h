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

#include "../../Base/Nullable.h"
#include "../WritingElement.h"

namespace SimpleTypes
{
	class COnOff;
	class CRelationshipId;
}

namespace ComplexTypes
{
	namespace Word
	{
		class CMatchSrc;
	}
}

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// Hyperlink 17.16.22 (Part 1)
		//--------------------------------------------------------------------------------	
		class CHyperlink : public WritingElementWithChilds<>
		{
		public:
			CHyperlink(OOX::Document *pMain = NULL);			
			virtual ~CHyperlink();

			const CHyperlink &operator =(const XmlUtils::CXmlNode& oNode);
			const CHyperlink &operator =(const XmlUtils::CXmlLiteReader& oReader);

			virtual void ClearItems();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string											m_sAnchor;
			nullable_string											m_sDestinition;
			nullable_string											m_sDocLocation;
			nullable<SimpleTypes::COnOff>							m_oHistory;
			nullable<SimpleTypes::CRelationshipId>					m_oId;
			nullable_string											m_sTgtFrame;
			nullable_string											m_sTooltip;
		};

		//--------------------------------------------------------------------------------
		// AltChunkPr 17.17.2.2 (Part 1)
		//--------------------------------------------------------------------------------	
		class CAltChunkPr : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CAltChunkPr)

			CAltChunkPr(OOX::Document *pMain = NULL);
			virtual ~CAltChunkPr();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			nullable<ComplexTypes::Word::CMatchSrc> m_oMatchSrc;
		};

		//--------------------------------------------------------------------------------
		// AltChunk 17.17.2.1 (Part 1)
		//--------------------------------------------------------------------------------	
		class CAltChunk : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CAltChunk)

			CAltChunk();
			CAltChunk(OOX::Document *pMain);
			virtual ~CAltChunk();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<CAltChunkPr>					m_oAltChunkPr;
			nullable<SimpleTypes::CRelationshipId>	m_oId;
		};

	} // namespace Logic
} // namespace OOX

