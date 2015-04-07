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