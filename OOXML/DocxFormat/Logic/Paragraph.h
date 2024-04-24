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

#include "../../Base/Nullable.h"
#include "../WritingElement.h"

namespace SimpleTypes
{
	class CLongHexNumber;
}

namespace OOX
{
	namespace Logic
	{
		class CParagraphProperty;
		//--------------------------------------------------------------------------------
		// CParagraph 17.3.1.22 (Part 1)
		//--------------------------------------------------------------------------------	
		class CParagraph : public WritingElementWithChilds<>
		{
		public:
			CParagraph(OOX::Document *pMain = NULL, WritingElement *parent = NULL);
			virtual ~CParagraph();

			const CParagraph &operator =(const XmlUtils::CXmlNode& oNode);
			const CParagraph &operator =(const XmlUtils::CXmlLiteReader& oReader);

			virtual void ClearItems();

			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;

		private:
			void fromXML(int nDepth, XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable<SimpleTypes::CLongHexNumber> m_oParaId;
			nullable<SimpleTypes::CLongHexNumber> m_oTextId;

			nullable<SimpleTypes::CLongHexNumber> m_oRsidDel;
			nullable<SimpleTypes::CLongHexNumber> m_oRsidP;
			nullable<SimpleTypes::CLongHexNumber> m_oRsidR;
			nullable<SimpleTypes::CLongHexNumber> m_oRsidRDefault;
			nullable<SimpleTypes::CLongHexNumber> m_oRsidRPr;

			CParagraphProperty *m_oParagraphProperty; // копия того что в m_arrItems...  - для быстрого доступа/анализа
			// по идее нужно сделать как в Drawing::Paragraph - то есть единственные подобъекты вынести отдельно
			WritingElement *m_oParent = NULL;
		};
	} // namespace Logic
} // namespace OOX
