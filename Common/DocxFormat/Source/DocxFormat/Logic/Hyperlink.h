﻿/*
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
#ifndef OOX_LOGIC_HYPERLINK_INCLUDE_H_
#define OOX_LOGIC_HYPERLINK_INCLUDE_H_

#include "../../Base/Nullable.h"

#include "../WritingElement.h"
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/ComplexTypes.h"

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
			CHyperlink(OOX::Document *pMain = NULL) : WritingElementWithChilds<>(pMain)
			{
			}
			CHyperlink(XmlUtils::CXmlNode &oNode)
			{
				fromXML( oNode );
			}
			CHyperlink(XmlUtils::CXmlLiteReader& oReader)
			{
				fromXML( oReader );
			}
			virtual ~CHyperlink()
			{
			}
			const CHyperlink &operator =(const XmlUtils::CXmlNode& oNode)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlNode&)oNode );
				return *this;
			}
			const CHyperlink &operator =(const XmlUtils::CXmlLiteReader& oReader)
			{
				ClearItems();
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}
			virtual void ClearItems()
			{
				m_sAnchor.reset();
				m_sDocLocation.reset();
				m_oHistory.reset();
				m_oId.reset();
				m_sTgtFrame.reset();
				m_sTooltip.reset();
				m_sDestinition.reset();

				WritingElementWithChilds::ClearItems();
			}

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return et_w_hyperlink;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		public:
			nullable_string											m_sAnchor;
			nullable_string											m_sDestinition;
			nullable_string											m_sDocLocation;
			nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse>>	m_oHistory;
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
			WritingElement_AdditionConstructors(CAltChunkPr)
			CAltChunkPr(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CAltChunkPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return et_w_altChunkPr;
			}
			nullable<ComplexTypes::Word::CMatchSrc> m_oMatchSrc;
		};
		//--------------------------------------------------------------------------------
		// AltChunk 17.17.2.1 (Part 1)
		//--------------------------------------------------------------------------------	
		class CAltChunk : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAltChunk)
			CAltChunk(OOX::Document *pMain) : WritingElement(pMain)
			{
			}
			virtual ~CAltChunk()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const
			{
				return et_w_altChunk;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_Read_if( oReader, L"id", m_oId)
				WritingElement_ReadAttributes_End_No_NS( oReader )

			}

		public:
			nullable<CAltChunkPr>					m_oAltChunkPr;
			nullable<SimpleTypes::CRelationshipId>	m_oId;
		};

	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_HYPERLINK_INCLUDE_H_
