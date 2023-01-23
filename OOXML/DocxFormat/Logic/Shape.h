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

#include "../WritingElement.h"
#include "../../Base/Nullable.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CTxbxContent 9.8.1.1 (Part 4)
		// 20.4.2.38 txbxContent (Rich Text Box Content Container)
		//--------------------------------------------------------------------------------	
		class CTxbxContent : public WritingElementWithChilds<>
		{
		public:
			CTxbxContent(OOX::Document *pMain = NULL);
			CTxbxContent(XmlUtils::CXmlNode &oNode);
			CTxbxContent(XmlUtils::CXmlLiteReader& oReader);
			virtual ~CTxbxContent();

			const CTxbxContent &operator =(const XmlUtils::CXmlNode& oNode);
			const CTxbxContent &operator =(const XmlUtils::CXmlLiteReader& oReader);

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

			// Childs
		};

		//--------------------------------------------------------------------------------
		// 20.4.2.37 txbx (Textual contents of shape)
		//--------------------------------------------------------------------------------	
		class CTextBody : public WritingElement
		{
		public:
			CTextBody(XmlUtils::CXmlNode &oNode);
			CTextBody(XmlUtils::CXmlLiteReader& oReader);
			CTextBody();
			virtual ~CTextBody();

		public:
			virtual std::wstring toXML() const;
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<OOX::Logic::CTxbxContent> m_oTxtbxContent;
		};
	} //Logic
} // namespace OOX
