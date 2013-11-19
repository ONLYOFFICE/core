#include "stdafx.h"
#include "HeaderAndFooterTable.h"

namespace DocFileFormat
{
	HeaderAndFooterTable::HeaderAndFooterTable (FileInformationBlock* fib, IStream* pTableStream)
	{
		VirtualStreamReader tableReader (pTableStream, fib->m_FibWord97.fcPlcfHdd);

		unsigned int tableSize	=	fib->m_FibWord97.lcbPlcfHdd / 4;

		if ( ( tableSize > 0 ) && ( fib->m_RgLw97.ccpHdr > 0 ) )
		{
			int* table			=	new int[tableSize];

			for (unsigned int i = 0; i < tableSize; ++i)
			{
				table[i]		=	tableReader.ReadInt32();
			}

			int count			=	( tableSize - 8 ) / 6;

			int initialPos		=	fib->m_RgLw97.ccpText + fib->m_RgLw97.ccpFtn;

			//the first 6 _entries are about footnote and endnote formatting
			//so skip these _entries
			int pos = 6;

			for (int i = 0; i < count; ++i)
			{
				//Even Header
				if ( table[pos] == table[pos + 1] )
				{
					m_arEvenHeaders.push_back( NULL );
				}
				else
				{
					m_arEvenHeaders.push_back( new CharacterRange( ( initialPos + table[pos] ), ( table[pos + 1] - table[pos] ) ) );
				}

				pos++;

				//Odd Header
				if ( table[pos] == table[pos + 1] )
				{
					m_arOddHeaders.push_back( NULL );
				}
				else
				{
					m_arOddHeaders.push_back( new CharacterRange( ( initialPos + table[pos] ), ( table[pos + 1] - table[pos] ) ) );
				}

				pos++;

				//Even Footer
				if ( table[pos] == table[pos + 1] )
				{
					m_arEvenFooters.push_back( NULL );
				}
				else
				{
					m_arEvenFooters.push_back( new CharacterRange( ( initialPos + table[pos] ), ( table[pos + 1] - table[pos] ) ) );
				}

				pos++;

				//Odd Footer
				if ( table[pos] == table[pos + 1] )
				{
					m_arOddFooters.push_back( NULL );
				}
				else
				{
					m_arOddFooters.push_back( new CharacterRange( ( initialPos + table[pos] ), ( table[pos + 1] - table[pos] ) ) );
				}

				pos++;

				//First Page Header
				if ( table[pos] == table[pos + 1] )
				{
					m_arFirstHeaders.push_back (NULL);
				}
				else
				{
					m_arFirstHeaders.push_back (new CharacterRange( ( initialPos + table[pos] ), ( table[pos + 1] - table[pos] ) ));
				}

				pos++;

				//First Page Footers
				if ( table[pos] == table[pos + 1] )
				{
					m_arFirstFooters.push_back( NULL );
				}
				else
				{
					m_arFirstFooters.push_back( new CharacterRange( ( initialPos + table[pos] ), ( table[pos + 1] - table[pos] ) ) );
				}

				pos++;
			}

			RELEASEARRAYOBJECTS(table);
		}
	}

	HeaderAndFooterTable::~HeaderAndFooterTable()
	{
		for_each (m_arEvenFooters.begin(),  m_arEvenFooters.end(),  DeleteDynamicObject());
		for_each (m_arEvenHeaders.begin(),  m_arEvenHeaders.end(),  DeleteDynamicObject());
		for_each (m_arFirstFooters.begin(), m_arFirstFooters.end(), DeleteDynamicObject());
		for_each (m_arFirstHeaders.begin(), m_arFirstHeaders.end(), DeleteDynamicObject());
		for_each (m_arOddFooters.begin(),   m_arOddFooters.end(),   DeleteDynamicObject());
		for_each (m_arOddHeaders.begin(),   m_arOddHeaders.end(),   DeleteDynamicObject());
	}
}