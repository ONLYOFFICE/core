
#include "ListTable.h"

#include <algorithm>

namespace DocFileFormat
{
	ListTable::~ListTable()
	{
		for_each( this->listData.begin(), this->listData.end(), DeleteDynamicObject() );
	}

	ListTable::ListTable( FileInformationBlock* fib, POLE::Stream* tableStream )
	{
		if ( fib->m_FibWord97.lcbPlfLst > 0 )
		{
			VirtualStreamReader reader( tableStream, fib->m_FibWord97.fcPlfLst );

			//the ListTable is not a real plex:
			//it starts with a count, followed by the array of LSTF structs,
			//followed by the array of LVLF structs

			//read count
			short count = reader.ReadInt16();

			//read the LSTF structs
			for ( int i = 0; i < count; i++ )
			{
				listData.push_back( new ListData( &reader, ListData::VARIABLE_LENGTH ) );
			}

			//read the LVLF structs
			for ( list<ListData*>::iterator iter = listData.begin(); iter != listData.end(); iter++ )
			{
				for ( unsigned int j = 0; j < (*iter)->rglvl->size(); j++ )
				{
					(*iter)->rglvl->operator []( j ) = new ListLevel( &reader, ListData::VARIABLE_LENGTH );
				}
			}
		}
	}
}