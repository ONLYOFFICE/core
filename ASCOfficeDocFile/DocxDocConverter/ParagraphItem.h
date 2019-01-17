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

#include "IParagraphItem.h"
#include "../Common/Aggregat.h"

namespace ASCDocFileFormat
{
	class ParagraphItem : public Aggregat<IParagraphItem, ParagraphItem>
	{
	public:
		ParagraphItem () : Aggregat(), m_nItemOffset(0)
		{
		}

		explicit ParagraphItem (const IParagraphItem& oItem) : Aggregat(static_cast<IParagraphItem*>(oItem.Clone())), m_nItemOffset(0)
		{
		}

		explicit ParagraphItem (const IParagraphItem& oItem, unsigned int nItemOffset) :  Aggregat(static_cast<IParagraphItem*>(oItem.Clone())), m_nItemOffset(nItemOffset)
		{
		}

		ParagraphItem (const ParagraphItem& oItem) : m_nItemOffset(oItem.m_nItemOffset)
		{
			if (oItem.m_item.operator->())
			{
				m_item.reset(static_cast<IParagraphItem*>(oItem.m_item->Clone()));  
			}
		}

		ParagraphItem& operator = (const ParagraphItem& oItem)
		{
			if (m_item.operator->() != oItem.m_item.operator->())
			{
				m_item.reset(static_cast<IParagraphItem*>(oItem.m_item->Clone()));
				m_nItemOffset	=	oItem.m_nItemOffset;
			}

			return *this;
		}

		inline unsigned int GetOffset() const
		{
			return m_nItemOffset;
		}

	private:

		unsigned int m_nItemOffset;
	};
}