#pragma once

#include "IVisitable.h"
#include "StructuredStorageReader.h"

namespace DocFileFormat
{
  class OleObject: public IVisitable
  {
    friend class OleObjectMapping;  
  
    private:
	  POLE::Storage	*oleStorage;
	  
    public:
	  enum LinkUpdateOption
      {
        NoLink = 0,
        Always = 1,
        OnCall = 3
      };

	wstring ObjectId;
	wstring ClassId;
	//  CLSID ClassId;
	/// The the value is true, the object is a linked object
	bool fLinked;
	/// Display name of the linked object or embedded object.
	wstring UserType;
	wstring ClipboardFormat;
	wstring Link;
	wstring Program;
	LinkUpdateOption updateMode;
	wstring UpdateMode;

	bool			isEquation;
	bool			isEmbedded;
	std::string	emeddedData;

	OleObject( const CharacterPropertyExceptions* chpx, StructuredStorageReader* docStorage )
		: fLinked(false), updateMode(NoLink)
	{
		isEquation = isEmbedded = false;
		
		oleStorage = docStorage->GetStorage();
		  
		if ( ( chpx != NULL ) && ( docStorage != NULL ) )
		{
			HRESULT res = S_OK;
			
			POLE::Stream* ObjectPoolStorage = new POLE::Stream(oleStorage, "ObjectPool");

			if (ObjectPoolStorage)
			{
				ObjectId = getOleEntryName( chpx );
				
				std::string sObjectId( ObjectId.begin(), ObjectId.end() );
				//if (oleStorage->enterDirectory(sObjectId))
				{		
					std::string name = "ObjectPool/" + sObjectId + "/";
					processOleStream(  name + "Ole"  );

					if ( this->fLinked )
					{
					  processLinkInfoStream( name +  "LinkInfo"  );
					}
					else
					{
					  processCompObjStream(  name +  "CompObj"  );
					}

					processEquationNativeStream(  name +  "Equation Native"  );
				}
				//oleStorage->leaveDirectory();
				delete  ObjectPoolStorage;
			}

			//STATSTG stg;
			//this->oleStorage->Stat( &stg, STATFLAG_NONAME );
			//this->ClassId = stg.clsid;
		}
	  }

	  virtual ~OleObject()
	  {
	  }

    private:
      void processLinkInfoStream( const string& linkStream )
      {
        try
        {
          POLE::Stream* pLinkStream = NULL;
		  HRESULT res = S_OK;

		  pLinkStream = //oleStorage->stream(linkStream);
			  new POLE::Stream(oleStorage, linkStream);

		  if ( pLinkStream )
		  {
		    VirtualStreamReader reader( pLinkStream );

            //there are two versions of the Link string, one contains ANSI characters, the other contains
            //unicode characters.
            //Both strings seem not to be standardized:
            //The length prefix is a character count EXCLUDING the terminating zero

            //Read the ANSI version
            short cch = reader.ReadInt16();
            unsigned char* str = reader.ReadBytes( cch, true );
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

			delete pLinkStream;
		  }
        }
        catch (...)
		{
		}
      }
      
	  void processEquationNativeStream( const string& eqStream )
      {
        try
        {
          POLE::Stream* pCompStream = NULL;
		  HRESULT res = S_OK;

		  pCompStream = new POLE::Stream(oleStorage, eqStream);

          if ( pCompStream )
		  {
		    VirtualStreamReader reader( pCompStream );

			int sz = reader.GetSize();

			unsigned char *Buffer = reader.ReadBytes( sz, true );

			if (Buffer)
			{
				delete []Buffer;
			}

			delete pCompStream;
		  }
	    }
        catch (...)
		{
		}
      }

      void processCompObjStream( const string& compStream )
      {
        try
        {
          POLE::Stream* pCompStream = NULL;
		  HRESULT res = S_OK;

		  pCompStream = new POLE::Stream(oleStorage, compStream);

          if ( pCompStream )
		  {
		    VirtualStreamReader reader( pCompStream );

            //skip the CompObjHeader
            reader.ReadBytes( 28, false );

			int sz_obj = reader.GetSize() - reader.GetPosition();

			if (sz_obj > 4)
			{
				//todooo сделать по нормальному CompObjHeader - psc3a.doc
				//UserType		= reader.ReadLengthPrefixedAnsiString();

				//sz_obj = reader.GetSize() - reader.GetPosition();
				//if (sz_obj > 4)
				//	ClipboardFormat	= reader.ReadLengthPrefixedAnsiString();

				//sz_obj = reader.GetSize() - reader.GetPosition();
				//if (sz_obj > 4)
				//	Program			= reader.ReadLengthPrefixedAnsiString();
			}
			delete pCompStream;
		  }
	    }
        catch (...)
		{
		}
      }

      void processOleStream( const string& oleStreamName )
      {
        try
        {
          POLE::Stream* pOleStream;
		  HRESULT res = S_OK;

		  pOleStream  = new POLE::Stream(oleStorage, oleStreamName);

		  if ( pOleStream )
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
			delete pOleStream;
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
		  for ( std::list<SinglePropertyModifier>::const_iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); iter++ )
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
