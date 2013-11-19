#include "stdafx.h"
#include "StyleSheet.h"

namespace DocFileFormat
{
	StyleSheet::~StyleSheet()
	{
		RELEASEOBJECT (stshi);

		if ( NULL != Styles )
		{
			for ( vector<StyleSheetDescription*>::iterator iter = Styles->begin(); iter != Styles->end(); iter++ )
			{
				RELEASEOBJECT( *iter );  
			}

			RELEASEOBJECT( Styles );
		}
	}

	/*========================================================================================================*/

	/// Parses the streams to retrieve a StyleSheet.
	StyleSheet::StyleSheet (FileInformationBlock* fib, IStream* tableStream, IStream* dataStream) : stshi(NULL), Styles(NULL)
	{
		VirtualStreamReader tableReader( tableStream, fib->m_FibWord97.fcStshf );

		//read size of the STSHI
		int stshiLengthBytesSize = 2;
		byte* stshiLengthBytes = tableReader.ReadBytes( stshiLengthBytesSize, true );
		short cbStshi = FormatUtils::BytesToInt16( stshiLengthBytes, 0, stshiLengthBytesSize );
		RELEASEARRAYOBJECTS( stshiLengthBytes );

		//read the bytes of the STSHI
		tableReader.Seek( ( fib->m_FibWord97.fcStshf + 2 ), STREAM_SEEK_SET ); 
		byte* stshi = tableReader.ReadBytes( cbStshi, true );

		//parses STSHI
		this->stshi = new StyleSheetInformation( stshi, cbStshi );
		RELEASEARRAYOBJECTS( stshi );

		//create list of STDs
		this->Styles = new vector<StyleSheetDescription*>();

		for ( int i = 0; i < this->stshi->cstd; i++ )
		{
			//get the cbStd
			unsigned short cbStd = tableReader.ReadUInt16();

			if ( cbStd != 0 )
			{
				//read the STD bytes
				byte* std = tableReader.ReadBytes( cbStd, true );

				//parse the STD bytes
				this->Styles->push_back( new StyleSheetDescription( std, cbStd, (int)this->stshi->cbSTDBaseInFile, dataStream ) );

				RELEASEARRAYOBJECTS( std );
			}
			else
			{
				this->Styles->push_back( NULL );
			}
		}
	}
}