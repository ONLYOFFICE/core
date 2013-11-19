#pragma once

#include "IRunItem.h"

namespace AVSDocFileFormat
{
	class RunItem: public Aggregat<IRunItem, RunItem>
	{
	public:
		RunItem() : Aggregat()
		{
		}

		explicit RunItem( const IRunItem& _runItem ) : Aggregat(static_cast<IRunItem*>(_runItem.Clone()))
		{
		}

		RunItem( const RunItem& _runItem )
		{
			if ( _runItem.m_item.get() != NULL )
			{
				this->m_item.reset( static_cast<IRunItem*>(_runItem.m_item->Clone()) );
			}
		}

		RunItem& operator = ( const RunItem& _runItem )
		{
			if ( this->m_item != _runItem.m_item )
			{
				this->m_item.reset( static_cast<IRunItem*>(_runItem.m_item->Clone()) );
			}

			return *this;
		}
	};
}