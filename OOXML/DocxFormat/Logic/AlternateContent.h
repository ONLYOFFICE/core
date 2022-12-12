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
#ifndef OOX_ALTERNATECONTENT_WORD_INCLUDE_H_
#define OOX_ALTERNATECONTENT_WORD_INCLUDE_H_

#include "../../Base/Nullable.h"
#include "../WritingElement.h"
#include "../../XlsxFormat/WritingElement.h"
namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CAnchorLock 14.3.2.1 (Part 4)
		//--------------------------------------------------------------------------------	
		class CAlternateContent : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CAlternateContent)

			CAlternateContent(OOX::Document *pMain = NULL);
			virtual ~CAlternateContent();

			void Clear();
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;
			virtual EElementType getType() const;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader, nullable_string & oRequires);

			nullable_string					m_oChoiceRequires;
	
			std::vector<WritingElement *>	m_arrChoiceItems;
			std::vector<WritingElement *>	m_arrFallbackItems;
		};
	} // namespace Words
} // namespace OOX

#endif // OOX_ALTERNATECONTENT_WORD_INCLUDE_H_
