#include "stdafx.h"
#include "ListLevel.h"

namespace DocFileFormat
{
	  ListLevel::~ListLevel()
	  {
	    RELEASEOBJECT( this->grpprlPapx );
        RELEASEOBJECT( this->grpprlChpx );

		RELEASEARRAYOBJECTS( this->_rawBytes );
	  }

	  /*========================================================================================================*/
      
      /// Parses the given StreamReader to retrieve a LVL struct
	  ListLevel::ListLevel( VirtualStreamReader* reader, int length ):
	  grpprlPapx(NULL), grpprlChpx(NULL), _rawBytes(NULL)
      {
	    long startPos = reader->GetPosition();

        //parse the fix part
        this->iStartAt = reader->ReadInt32();
        this->nfc = reader->ReadByte();
        int flag = reader->ReadByte();
        this->jc = (byte)( flag & 0x03 );
		this->fLegal = FormatUtils::BitmaskToBool( flag, 0x04 );
        this->fNoRestart = FormatUtils::BitmaskToBool( flag, 0x08 );
        this->fPrev = FormatUtils::BitmaskToBool( flag, 0x10 );
        this->fPrevSpace = FormatUtils::BitmaskToBool( flag, 0x20 );
        this->fWord6 = FormatUtils::BitmaskToBool( flag, 0x40 );
        
		for ( int i = 0; i < 9; i++ )
        {
		  rgbxchNums.push_back( reader->ReadByte() );
        }

        this->ixchFollow = (FollowingChar)reader->ReadByte();

        this->dxaSpace = reader->ReadInt32();
        this->dxaIndent = reader->ReadInt32();

        this->cbGrpprlChpx = reader->ReadByte();
        this->cbGrpprlPapx = reader->ReadByte();

        this->ilvlRestartLim = reader->ReadByte();
        this->grfhic = reader->ReadByte();
            
        //parse the variable part

        //read the group of papx sprms
        //this papx has no istd, so use PX to parse it
		byte *bytes = reader->ReadBytes( this->cbGrpprlPapx, true );
        PropertyExceptions* px = new PropertyExceptions( bytes, this->cbGrpprlPapx );
		this->grpprlPapx = new ParagraphPropertyExceptions( *(px->grpprl) );
		RELEASEOBJECT( px );
        RELEASEARRAYOBJECTS( bytes );

        //read the group of chpx sprms
		bytes = reader->ReadBytes( this->cbGrpprlChpx, true );
        this->grpprlChpx = new CharacterPropertyExceptions( bytes, this->cbGrpprlChpx );
		RELEASEARRAYOBJECTS( bytes );

        //read the number text
		short strLen = reader->ReadInt16();
		bytes = reader->ReadBytes( ( strLen * 2 ), true );
		FormatUtils::GetSTLCollectionFromBytes<wstring>( &(this->xst), bytes, ( strLen * 2 ), ENCODING_UNICODE );
		RELEASEARRAYOBJECTS( bytes );

		long endPos = reader->GetPosition();
		reader->Seek( startPos, STREAM_SEEK_SET );
        _rawBytes = reader->ReadBytes( (int)( endPos - startPos ), true );
      }
}