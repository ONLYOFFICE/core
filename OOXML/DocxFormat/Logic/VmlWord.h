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
#ifndef OOX_VML_WORD_INCLUDE_H_
#define OOX_VML_WORD_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../WritingElement.h"

namespace SimpleTypes
{
	class CBorderType;
	class CBorderShadow;
	class CDecimalNumber;
	class CHorizontalAnchor;
	class CVerticalAnchor;
	class CWrapSide;
	class CWrapType;
}

namespace OOX
{
	namespace VmlWord
	{
		//--------------------------------------------------------------------------------
		// CAnchorLock 14.3.2.1 (Part 4)
		//--------------------------------------------------------------------------------	
		class CAnchorLock : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CAnchorLock)
			CAnchorLock(OOX::Document *pMain = NULL);
			virtual ~CAnchorLock();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;
		};

		//--------------------------------------------------------------------------------
		// CBorder 14.3.2.2;14.3.2.3;14.3.2.4;14.3.2.5 (Part 4)
		//--------------------------------------------------------------------------------	
		class CBorder : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CBorder)
			CBorder(OOX::Document *pMain = NULL);
			virtual ~CBorder();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			EElementType                            m_eType;

			nullable<SimpleTypes::CBorderShadow > m_oShadow;
			nullable<SimpleTypes::CBorderType   > m_oType;
			nullable<SimpleTypes::CDecimalNumber> m_oWidth;
		};
		//--------------------------------------------------------------------------------
		// CWrap 14.3.2.6 (Part 4)
		//--------------------------------------------------------------------------------	
		class CWrap : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CWrap)
			CWrap(OOX::Document *pMain = NULL);
			virtual ~CWrap();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CHorizontalAnchor > m_oAnchorX;
			nullable<SimpleTypes::CVerticalAnchor   > m_oAnchorY;
			nullable<SimpleTypes::CWrapSide         > m_oSide;
			nullable<SimpleTypes::CWrapType         > m_oType;
		};

	} // namespace Vml
} // namespace OOX

#endif // OOX_VML_WORD_INCLUDE_H_
