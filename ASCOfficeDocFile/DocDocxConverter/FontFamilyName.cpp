#include "stdafx.h"
#include "FontFamilyName.h"

namespace DocFileFormat
{
	FontFamilyName::~FontFamilyName()
	{
	  RELEASEARRAYOBJECTS( this->panose );

	  this->xszFtn.clear();
      this->xszAlt.clear();
	}

	/*========================================================================================================*/

	FontFamilyName::FontFamilyName():
	fTrueType(false), ff(0), wWeight(0), chs(0), prq(0), panose(NULL), panoseSize(0)
	{
	  this->fs.CodePageBitfield0 = 0;
	  this->fs.CodePageBitfield1 = 0;
	  this->fs.UnicodeSubsetBitfield0 = 0;
	  this->fs.UnicodeSubsetBitfield1 = 0;
	  this->fs.UnicodeSubsetBitfield2 = 0;
	  this->fs.UnicodeSubsetBitfield3 = 0;
	}

	/*========================================================================================================*/

	ByteStructure* FontFamilyName::ConstructObject( VirtualStreamReader* reader, int length )
	{
	  FontFamilyName *newObject = new FontFamilyName();
	  
	  long startPos = reader->GetPosition();

      //FFID
      int ffid = (int)reader->ReadByte();

	  newObject->prq = (byte)FormatUtils::BitmaskToInt( ffid, 0x03 );
	  newObject->fTrueType = FormatUtils::BitmaskToBool( ffid, 0x04 );
	  newObject->ff = (byte)FormatUtils::BitmaskToInt( ffid, 0x70 );
      newObject->wWeight = reader->ReadInt16();
	  newObject->chs = reader->ReadByte();

      //skip byte 5
      reader->ReadByte();

      //read the 10 bytes panose
      newObject->panoseSize = 10;
	  newObject->panose = reader->ReadBytes( newObject->panoseSize, true );
	  
      //read the 24 bytes FontSignature
      newObject->fs.UnicodeSubsetBitfield0 = reader->ReadUInt32();
      newObject->fs.UnicodeSubsetBitfield1 = reader->ReadUInt32();
      newObject->fs.UnicodeSubsetBitfield2 = reader->ReadUInt32();
      newObject->fs.UnicodeSubsetBitfield3 = reader->ReadUInt32();
      newObject->fs.CodePageBitfield0 = reader->ReadUInt32();
      newObject->fs.CodePageBitfield1 = reader->ReadUInt32();

      //read the next \0 terminated string
	  long strStart = reader->GetPosition();
      long strEnd = searchTerminationZero( reader );

	  byte *bytes = reader->ReadBytes( (int)( strEnd - strStart ), true );
	  
	  FormatUtils::GetSTLCollectionFromBytes<wstring>( &(newObject->xszFtn), bytes, (int)( strEnd - strStart ), (Encoding)ENCODING_UNICODE );

	  RELEASEARRAYOBJECTS( bytes );

	  long readBytes = reader->GetPosition() - startPos;
      
	  if( readBytes < length )
      {
        //read the next \0 terminated string
		strStart = reader->GetPosition();
        strEnd = searchTerminationZero( reader );

		bytes = reader->ReadBytes( (int)( strEnd - strStart ), true );

		FormatUtils::GetSTLCollectionFromBytes<wstring>( &(newObject->xszAlt), bytes, (int)( strEnd - strStart ), ENCODING_UNICODE );

		RELEASEARRAYOBJECTS( bytes );
      }

	  return static_cast<ByteStructure*>( newObject );
    }

	/*========================================================================================================*/

	long FontFamilyName::searchTerminationZero( VirtualStreamReader *reader )
    {
	  long strStart = reader->GetPosition();
      
	  while ( reader->ReadInt16() != 0 )
      {
        ;
      }
      
	  long pos = reader->GetPosition();
	  
	  reader->Seek( (int)strStart, STREAM_SEEK_SET );
      
	  return pos;
    }
}