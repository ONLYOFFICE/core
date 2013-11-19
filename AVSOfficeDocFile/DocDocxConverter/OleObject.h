#pragma once

#include "IVisitable.h"
#include "StructuredStorageReader.h"

namespace DocFileFormat
{
  class OleObject: public IVisitable
  {
    friend class OleObjectMapping;  
  
    private:
	  CComPtr<IStorage> oleStorage;
	  
    public:
	  enum LinkUpdateOption
      {
        NoLink = 0,
        Always = 1,
        OnCall = 3
      };

      wstring ObjectId;
	  CLSID ClassId;
      /// The the value is true, the object is a linked object
      bool fLinked;
      /// Display name of the linked object or embedded object.
      wstring UserType;
      wstring ClipboardFormat;
      wstring Link;
      wstring Program;
      LinkUpdateOption updateMode;
	  wstring UpdateMode;

	  OleObject( const CharacterPropertyExceptions* chpx, StructuredStorageReader* docStorage ):
	  fLinked(false), updateMode(NoLink)
      {
	    this->ClassId = GUID_NULL;
		  
		if ( ( chpx != NULL ) && ( docStorage != NULL ) )
		{
		  HRESULT res = S_OK;
			
		  CComPtr<IStorage> ObjectPoolStorage;

		  res = docStorage->GetStorage()->OpenStorage( _T( "ObjectPool" ), NULL, STGM_SHARE_EXCLUSIVE, NULL, 0, &ObjectPoolStorage );

		  if ( res == S_OK )
		  {
		    this->ObjectId = getOleEntryName( chpx );
			  
			res = ObjectPoolStorage->OpenStorage( this->ObjectId.c_str(), NULL, STGM_SHARE_EXCLUSIVE, NULL, 0, &this->oleStorage );
		  }

		  if ( res == S_OK )
		  {
		    processOleStream( _T( "\01Ole" ) );

			if ( this->fLinked )
            {
              processLinkInfoStream( _T( "\03LinkInfo" ) );
            }
            else
            {
              processCompObjStream( _T( "\01CompObj" ) );
            }

			STATSTG stg;

			this->oleStorage->Stat( &stg, STATFLAG_NONAME );

			this->ClassId = stg.clsid;
		  }
 	 	}
      }

	  virtual ~OleObject()
	  {
	  }

    private:
      void processLinkInfoStream( const wstring& linkStream )
      {
        try
        {
          CComPtr<IStream> pLinkStream;
		  HRESULT res = S_OK;

		  res = this->oleStorage->OpenStream( linkStream.c_str(), NULL, STGM_SHARE_EXCLUSIVE, NULL, &pLinkStream );

		  if ( res == S_OK )
		  {
		    VirtualStreamReader reader( pLinkStream );

            //there are two versions of the Link string, one contains ANSI characters, the other contains
            //unicode characters.
            //Both strings seem not to be standardized:
            //The length prefix is a character count EXCLUDING the terminating zero

            //Read the ANSI version
            short cch = reader.ReadInt16();
            byte* str = reader.ReadBytes( cch, true );
			FormatUtils::GetSTLCollectionFromBytes<wstring>( &this->Link, str, cch, ENCODING_WINDOWS_1251 );
			RELEASEARRAYOBJECTS( str );
                
            //skip the terminating zero of the ANSI string
            //even if the characters are ANSI chars, the terminating zero has 2 bytes
            reader.ReadBytes( 2, false );

            //skip the next 4 bytes (flags?)
            reader.ReadBytes( 4, false );

            //Read the Unicode version
			this->Link.clear();

            cch = reader.ReadInt16();
            str = reader.ReadBytes( ( cch * 2 ), true );
            FormatUtils::GetSTLCollectionFromBytes<wstring>( &this->Link, str, ( cch * 2 ), ENCODING_UNICODE );
			RELEASEARRAYOBJECTS( str );

            //skip the terminating zero of the Unicode string
            reader.ReadBytes( 2, false );
		  }
        }
        catch (...)
		{
		}
      }

      void processCompObjStream( const wstring& compStream )
      {
        try
        {
          CComPtr<IStream> pCompStream;
		  HRESULT res = S_OK;

		  res = this->oleStorage->OpenStream( compStream.c_str(), NULL, STGM_SHARE_EXCLUSIVE, NULL, &pCompStream );

          if ( res == S_OK )
		  {
		    VirtualStreamReader reader( pCompStream );

            //skip the CompObjHeader
            reader.ReadBytes( 28, false );

			this->UserType = reader.ReadLengthPrefixedAnsiString();
			this->ClipboardFormat = reader.ReadLengthPrefixedAnsiString();
			this->Program = reader.ReadLengthPrefixedAnsiString();
		  }
	    }
        catch (...)
		{
		}
      }

      void processOleStream( const wstring& oleStream )
      {
        try
        {
          CComPtr<IStream> pOleStream;
		  HRESULT res = S_OK;

		  res = this->oleStorage->OpenStream( oleStream.c_str(), NULL, STGM_SHARE_EXCLUSIVE, NULL, &pOleStream );

		  if ( res == S_OK )
		  {
		    VirtualStreamReader reader( pOleStream );

            //skip version
            reader.ReadBytes( 4, false );

            //read the embedded/linked flag
            int flag = reader.ReadInt32();
			this->fLinked = FormatUtils::BitmaskToBool( flag, 0x1 );

            //Link update option
			this->updateMode = (LinkUpdateOption)reader.ReadInt32();

			switch ( this->updateMode )
			{
			  case NoLink:
			  {
			    this->UpdateMode = _T( "NoLink" );
			  }
			  break;
              
			  case Always:
			  {
 			    this->UpdateMode = _T( "Always" );
			  }
			  break;
			  
			  case OnCall:
			  {
			    this->UpdateMode = _T( "OnCall" );
			  }
			  break;
			}
          }
		}
        catch (...)
		{ 
		}
      }

      wstring getOleEntryName( const CharacterPropertyExceptions* chpx )
      {
        wstring ret;

		if ( chpx != NULL )
		{
          for ( list<SinglePropertyModifier>::const_iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); iter++ )
          {
            if ( iter->OpCode == sprmCPicLocation )
            {
			  ret = ( _T( "_" ) + FormatUtils::IntToWideString( FormatUtils::BytesToUInt32( iter->Arguments, 0, iter->argumentsSize ) ) );
              
			  break;
            }
          }
		}

        return ret;
      }
  };
}