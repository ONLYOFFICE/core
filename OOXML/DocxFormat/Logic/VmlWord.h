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
#ifndef OOX_VML_WORD_INCLUDE_H_
#define OOX_VML_WORD_INCLUDE_H_

#include "../../Base/Nullable.h"

#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/SimpleTypes_Vml.h"

#include "../WritingElement.h"
#include "../RId.h"

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
			WritingElement_AdditionConstructors(CAnchorLock)
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
			WritingElement_AdditionConstructors(CBorder)
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
			WritingElement_AdditionConstructors(CWrap)
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
