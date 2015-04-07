
#include "ListData.h"

#include <algorithm>

namespace DocFileFormat
{
	ListData::~ListData()
	{
		for_each(rglvl->begin(), rglvl->end(), DeleteDynamicObject());

		RELEASEOBJECT(rglvl);
		RELEASEARRAYOBJECTS(_rawBytes);
	}

	// Parses the StreamReader to retrieve a ListData

	ListData::ListData(VirtualStreamReader* reader, int length) : rglvl(NULL), _rawBytes(NULL)
	{
		long startPos = reader->GetPosition();

		this->lsid = reader->ReadInt32();
		this->tplc = reader->ReadInt32();

		for ( int i = 0; i < 9; i++ )
		{
			this->rgistd.push_back( reader->ReadInt16() );
		}

		//parse flagbyte
		int flag = (int)reader->ReadByte();
		this->fSimpleList = FormatUtils::BitmaskToBool( flag, 0x01 );

		if ( this->fSimpleList )
		{
			this->rglvl = new vector<ListLevel*>( 1 );
		}
		else
		{
			this->rglvl = new vector<ListLevel*>( 9 );
		}

		this->fRestartHdn = FormatUtils::BitmaskToBool( flag, 0x02 );
		this->fAutoNum = FormatUtils::BitmaskToBool( flag, 0x04 );
		this->fPreRTF = FormatUtils::BitmaskToBool( flag, 0x08 );
		this->fHybrid = FormatUtils::BitmaskToBool( flag, 0x10 );

		this->grfhic = reader->ReadByte();

        reader->Seek( startPos, 0/*STREAM_SEEK_SET */);
		_rawBytes = reader->ReadBytes( LSTF_LENGTH, true );
	}
}
