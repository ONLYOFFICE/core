#pragma once

#include "VirtualStreamReader.h"
#include "WideString.h"
#include "IVisitable.h"

#include "../Common/FormatUtils.h"

namespace DocFileFormat
{
  template<class T> class StringTable: public IVisitable
  {
    friend class CharacterPropertiesMapping;
	friend class FontTableMapping;
	friend class StyleSheetMapping;	
	friend class DocumentMapping;
  
    private:
		ASCDocFormatUtils::Encoding _enc;
		bool fExtend;
		int cData;
		unsigned short cbExtra;
		vector<ByteStructure*> Data;

    public:
	  virtual ~StringTable()
	  {
	    for ( vector<ByteStructure*>::iterator iter = this->Data.begin(); iter != this->Data.end(); iter++ )
		{
		  RELEASEOBJECT( *iter );
		}
	  }

	  StringTable( VirtualStreamReader *reader ): 
	  _enc(ENCODING_INVALID_VALUE), fExtend(false), cData(0), cbExtra(0)
      {
        parse( reader, (unsigned int)reader->GetPosition() );
      }

	  StringTable( POLE::Stream* tableStream, unsigned int fc, unsigned int lcb ):
	  _enc(ENCODING_INVALID_VALUE), fExtend(false), cData(0), cbExtra(0)
      {
        if ( lcb > 0 )
        {
	      VirtualStreamReader reader( tableStream, fc );
		
		  parse( &reader, fc ) ;
        }
      }

	  ByteStructure* operator [] ( unsigned int index ) const
	  {
		if ( index < this->Data.size() )
	    {
	      return this->Data.at( index );
	    }
	    else
	    {
	      return NULL;
	    }
	  }

    private:
	  void parse( VirtualStreamReader *reader, unsigned int fc )
	  {
	    if ( reader != NULL )
		{
          //read fExtend
          if ( reader->ReadUInt16() == 0xFFFF )
          {
            //if the first 2 bytes are 0xFFFF the STTB contains unicode characters
		    this->fExtend = true;
            _enc = ENCODING_UNICODE;
          }
          else
          {
            //else the STTB contains 1byte characters and the fExtend field is non-existend
            //seek back to the beginning
		    this->fExtend = false;
            _enc = ENCODING_WINDOWS_1251;

            reader->Seek( (int)fc, 0/*STREAM_SEEK_SET*/ );
	      }

          //read cData
	      long cDataStart = reader->GetPosition();
          unsigned short c = reader->ReadUInt16();
      
	      if ( c != 0xFFFF )
          {
            //cData is a 2byte unsigned Integer and the read bytes are already cData
            this->cData = (int)c;
          }
          else
          {
            //cData is a 4byte signed Integer, so we need to seek back
            reader->Seek( (int)( fc + cDataStart ), 0/*STREAM_SEEK_SET*/ );
		
            this->cData = reader->ReadInt32();
          }

          //read cbExtra
          this->cbExtra = reader->ReadUInt16();

	      //read the strings and extra datas
          for ( int i = 0; i < this->cData; i++ )
	      {   
            int cchData = 0;
            int cbData = 0;
        
		    if ( this->fExtend )
            {
              cchData = (int)reader->ReadUInt16();
              cbData = cchData * 2;
            }
            else
            {
              cchData = (int)reader->ReadByte();
              cbData = cchData;
            }

		    long posBeforeType = reader->GetPosition();

			this->Data.push_back( T().ConstructObject( reader, cbData ) );

            reader->Seek( (int)( posBeforeType + cbData ), 0/*STREAM_SEEK_SET */);
        
            //skip the extra unsigned char
            reader->ReadBytes( cbExtra, false );
          }
		}
	  }
  };
}
