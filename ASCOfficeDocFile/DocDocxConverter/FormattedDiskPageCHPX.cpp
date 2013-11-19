#include "stdafx.h"
#include "FormattedDiskPageCHPX.h"

namespace DocFileFormat
{
	FormattedDiskPageCHPX::~FormattedDiskPageCHPX()
	{
		RELEASEARRAYOBJECTS( this->rgfc );
		RELEASEARRAYOBJECTS( this->rgb );

		if ( this->grpchpx != NULL )
		{
			for ( unsigned int i = 0; i < this->grpchpxSize; i++ )
			{
				RELEASEOBJECT( this->grpchpx[i] );
			}

			RELEASEARRAYOBJECTS( this->grpchpx );
		}
	}

	/*========================================================================================================*/

	FormattedDiskPageCHPX::FormattedDiskPageCHPX( IStream* wordStream, int offset ):
	FormattedDiskPage(), rgb(NULL), grpchpxSize(NULL), grpchpx(NULL) 
	{
		this->Type = Character;
		this->WordStream = wordStream;

		//read the 512 bytes (FKP)
		byte* bytes = NULL;
		bytes = new byte[512];

		LARGE_INTEGER nSeekPos = { offset, 0 };
		this->WordStream->Seek( nSeekPos, STREAM_SEEK_SET, NULL );
		this->WordStream->Read( bytes, 512, NULL );

		//get the count first
		this->crun = bytes[511];

		//create and fill the array with the adresses
		this->rgfcSize = this->crun + 1;
		this->rgfc = new int[this->rgfcSize];

		int j = 0;

		for ( unsigned int i = 0; i < this->rgfcSize; i++ )
		{
			rgfc[i] = FormatUtils::BytesToInt32( bytes, j, 512 );
			j += 4;
		}

		//create arrays
		this->rgb = new byte[this->crun];
		this->grpchpxSize = this->crun;
		this->grpchpx = new CharacterPropertyExceptions*[this->grpchpxSize];

		j = 4 * ( this->crun + 1 );

		byte* chpx = NULL;

		for ( int i = 0; i < this->crun; i++ )
		{
			//fill the rgb array
			byte wordOffset = bytes[j];
			this->rgb[i] = wordOffset;
			j++;

			if ( wordOffset != 0 )
			{
				//read first byte of CHPX
				//it's the count of bytes
				byte cb = bytes[wordOffset * 2];

				//read the bytes of chpx
				chpx = new byte[cb];
				//Array.Copy(bytes, (wordOffset * 2) + 1, chpx, 0, chpx.Length);
				memcpy( chpx, ( bytes + (wordOffset * 2) + 1 ), cb );

				//parse CHPX and fill grpchpx
				grpchpx[i] = new CharacterPropertyExceptions( chpx, cb );

				RELEASEARRAYOBJECTS( chpx );
			}
			else
			{
				//create a CHPX which doesn't modify anything
				grpchpx[i] = new CharacterPropertyExceptions();
			}
		}

		RELEASEARRAYOBJECTS( bytes );
	}

	/*========================================================================================================*/

	/// Parses the 0Table (or 1Table) for FKP _entries containing CHPX
	list<FormattedDiskPageCHPX*>* FormattedDiskPageCHPX::GetAllCHPXFKPs( FileInformationBlock* fib, IStream* wordStream, IStream* tableStream )
	{
		list<FormattedDiskPageCHPX*>* CHPXlist = new list<FormattedDiskPageCHPX*>();

		//get bintable for CHPX
		byte* binTableChpx = new byte[fib->m_FibWord97.lcbPlcfBteChpx];

		LARGE_INTEGER nSeekPos = { (int)fib->m_FibWord97.fcPlcfBteChpx, 0 };
		tableStream->Seek( nSeekPos, STREAM_SEEK_SET, NULL );
		tableStream->Read( binTableChpx, fib->m_FibWord97.lcbPlcfBteChpx, NULL );

		//there are n offsets and n-1 fkp's in the bin table
		int n = ( ( (int)fib->m_FibWord97.lcbPlcfBteChpx - 4 ) / 8 ) + 1;

		//Get the indexed CHPX FKPs
		for ( unsigned int i = (n * 4); i < fib->m_FibWord97.lcbPlcfBteChpx; i += 4 )
		{
			//indexed FKP is the 6th 512byte page
			int fkpnr = FormatUtils::BytesToInt32( binTableChpx, i, fib->m_FibWord97.lcbPlcfBteChpx );

			//so starts at:
			int offset = fkpnr * 512;

			//parse the FKP and add it to the list
			CHPXlist->push_back( new FormattedDiskPageCHPX( wordStream, offset ) );
		}

		RELEASEARRAYOBJECTS( binTableChpx );

		return CHPXlist;
	}
}