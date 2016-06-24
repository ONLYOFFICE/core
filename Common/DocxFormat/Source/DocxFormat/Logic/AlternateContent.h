/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
			CAlternateContent()
			{
			}
			virtual ~CAlternateContent()
			{
				Clear();
			}
			void Clear()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrChoiceItems.size(); nIndex++ )
				{
					if ( m_arrChoiceItems[nIndex] )
						delete m_arrChoiceItems[nIndex];
					m_arrChoiceItems[nIndex] = NULL;
				}
				m_arrChoiceItems.clear();
				for (unsigned int nIndex = 0; nIndex < m_arrFallbackItems.size(); nIndex++ )
				{
					if ( m_arrFallbackItems[nIndex] )
						delete m_arrFallbackItems[nIndex];
					m_arrFallbackItems[nIndex] = NULL;
				}
				m_arrFallbackItems.clear();
				for (unsigned int nIndex = 0; nIndex < m_arrSpreadsheetChoiceItems.size(); nIndex++ )
				{
					if ( m_arrSpreadsheetChoiceItems[nIndex] )
						delete m_arrSpreadsheetChoiceItems[nIndex];
					m_arrSpreadsheetChoiceItems[nIndex] = NULL;
				}
				m_arrSpreadsheetChoiceItems.clear();
				for (unsigned int nIndex = 0; nIndex < m_arrSpreadsheetFallbackItems.size(); nIndex++ )
				{
					if ( m_arrSpreadsheetFallbackItems[nIndex] )
						delete m_arrSpreadsheetFallbackItems[nIndex];
					m_arrSpreadsheetFallbackItems[nIndex] = NULL;
				}
				m_arrSpreadsheetFallbackItems.clear();
			}
		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual EElementType getType() const
			{
				return OOX::et_mc_alternateContent;
			}
		public:
			nullable<CString > m_sXml;
			
			std::vector<WritingElement *> m_arrChoiceItems;
			std::vector<WritingElement *> m_arrFallbackItems;
			
			std::vector<OOX::Spreadsheet::WritingElement *> m_arrSpreadsheetChoiceItems;
			std::vector<OOX::Spreadsheet::WritingElement *> m_arrSpreadsheetFallbackItems;
		};
	} // namespace Words
} // namespace OOX

#endif // OOX_ALTERNATECONTENT_WORD_INCLUDE_H_