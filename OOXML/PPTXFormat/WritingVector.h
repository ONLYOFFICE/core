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
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				size_t nCount = oNodes.size();

				for (size_t i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode & nodeTemp = oNodes[i];

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