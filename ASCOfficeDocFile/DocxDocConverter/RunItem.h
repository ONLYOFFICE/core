#pragma once

#include "IRunItem.h"

namespace ASCDocFileFormat
{
	class RunItem : public Aggregat<IRunItem, RunItem>
	{
	public:
		RunItem() : Aggregat()
		{
		}

		explicit RunItem(const IRunItem& oRunItem) : Aggregat(static_cast<IRunItem*>(oRunItem.Clone()))
		{

		}

		RunItem(const RunItem& oRunItem)
		{
			if (NULL != oRunItem.m_item.operator->())
			{
				m_item.reset( static_cast<IRunItem*>(oRunItem.m_item->Clone()));
			}
		}

		RunItem& operator = (const RunItem& oRunItem)
		{
			if (m_item.operator->() != oRunItem.m_item.operator->())
			{
				m_item.reset(static_cast<IRunItem*>(oRunItem.m_item->Clone()));
			}

			return *this;
		}
	};
}