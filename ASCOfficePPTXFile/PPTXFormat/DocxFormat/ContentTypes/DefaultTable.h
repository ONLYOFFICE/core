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
#ifndef OOX_CONTENT_TYPES_DEFAULT_TABLE_INCLUDE_H_
#define OOX_CONTENT_TYPES_DEFAULT_TABLE_INCLUDE_H_

#include "./../WritingVector.h"
#include "Default.h"


namespace PPTX
{
	namespace ContentTypes
	{
		class DefaultTable : public WritingVector<Default>
		{
		public:
			DefaultTable()
			{
				m_items.push_back(Default(_T("rels")));
				m_items.push_back(Default(_T("xml")));
			}
			virtual ~DefaultTable()
			{
			}
			explicit DefaultTable(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
			}
			const DefaultTable& operator =(XmlUtils::CXmlNode& node)
			{
				fromXML(node);
				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.LoadArray(_T("Default"), m_items);
			}

		public:
			void add(const OOX::CPath& path)
			{
				CString ext = path.GetExtention();
				const CString extension = ext.Mid(1);
				
				size_t nCount = m_items.size();
				size_t nIndex = 0;	

				while (nIndex < nCount)
				{
					if (m_items[nIndex] == extension)
						break;
					++nIndex;
				}
				
				if (nIndex == nCount)
					m_items.push_back(Default(extension));
			}
		};
	} // namespace ContentTypes
} // namespace PPTX

#endif // OOX_CONTENT)TYPES_DEFAULT_TABLE_INCLUDE_H_