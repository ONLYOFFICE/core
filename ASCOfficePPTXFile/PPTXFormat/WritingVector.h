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
#ifndef PPTXOOX_WRITING_VECTOR_INCLUDE_H_
#define PPTXOOX_WRITING_VECTOR_INCLUDE_H_

#include "WritingElement.h"

namespace PPTX
{
	template <typename T>
	class WritingVector : public WritingElement
	{
	public:
		std::vector<T> m_items;

	public:
		WritingVector() : m_items()  {}
		virtual ~WritingVector()	 {}
		explicit WritingVector(XmlUtils::CXmlNode& node) 
		{
			fromXML(node);
		}

		WritingVector& operator =(XmlUtils::CXmlNode& node) 
		{
			fromXML(node); 
			return *this;
		}

	public:
		virtual void fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();

				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode nodeTemp;
					oNodes.GetAt(i, nodeTemp);

					m_items.push_back(T(nodeTemp));
				}			
			}
		}

		virtual std::wstring toXML() const
		{
			std::wstring strResult = _T("");
			size_t nCount = m_items.size();

			for (size_t i = 0; i < nCount; ++i)
			{
				strResult += m_items[i].toXML();
			}

			return strResult;
		}
		virtual EElementType getType() const
		{
			return et_Unknown;
		}
	};
} // namespace PPTX

#endif // PPTXOOX_WRITING_VECTOR_INCLUDE_H_