#pragma once

#include <boost\scoped_ptr.hpp>

namespace DocxRenderer
{
  // флаги в CommandParams
  static const long c_nParamFlipX		= 0x0001;
  static const long c_nParamFlipY		= 0x0002;
  static const long c_nFlipNextRotate	= 0x0004;

  // тип в BeginCommand, EndCommand
  static const long c_nNone              = 0x0000;
  static const long c_nPageType			 = 0x0001;
  static const long c_nTextType			 = 0x0002;
  static const long c_nPathType			 = 0x0004;
  static const long c_nHyperlinkType	 = 0x0008;
  static const long c_nWatermarkType     = 0x0010;
  static const long c_nClipType			 = 0x0020;
  static const long c_nResetClipType	 = 0x0040;
	
  struct IClone
  {
	virtual ~IClone() {}
	virtual IClone* New() const = 0;
	virtual IClone* Clone() const = 0;
  };
	
  namespace Utils
  {
    static const double PI = 3.14159265358979323846;
	  
	// 2-byte number
    inline short little_endian_2_big_endian( short s )
    {
      return ( ( s >> 8) & 0xff ) + ( ( s << 8 ) & 0xff00 );
    }

    /*========================================================================================================*/

    // 4-byte number
    inline int little_endian_2_big_endian( int i )
    {
      return ( ( i & 0xff ) << 24 ) + ( ( i & 0xff00 ) << 8 ) + ( ( i & 0xff0000 ) >> 8 ) + ( ( i >> 24 ) & 0xff );
    }

    /*========================================================================================================*/

    inline std::string ToHexString( int i )
    {
      char buffer[( 2 * sizeof(i) ) + 1];

	  memset( buffer, 0, sizeof(buffer) );

      sprintf( buffer, "%08X", i );

      return std::string( buffer );
    }

	/*========================================================================================================*/

    inline std::string ToHexString( unsigned int ui )
    {
      char buffer[( 2 * sizeof(ui) ) + 1];

	  memset( buffer, 0, sizeof(buffer) );

      sprintf( buffer, "%08X", ui );

      return std::string( buffer );
    }

	/*========================================================================================================*/

    inline std::string ToHexString( short s )
    {
      char buffer[( 2 * sizeof(s) ) + 1];

	  memset( buffer, 0, sizeof(buffer) );

      sprintf( buffer, "%04X", s );

      return std::string( buffer );
    }

	/*========================================================================================================*/
	
	inline std::string ToHexString( unsigned short us )
    {
      char buffer[( 2 * sizeof(us) ) + 1];

	  memset( buffer, 0, sizeof(buffer) );

      sprintf( buffer, "%04X", us );

      return std::string( buffer );
    }

	/*========================================================================================================*/

    inline std::string ToHexString( char c )
    {
      char buffer[( 2 * sizeof(c) ) + 1];

	  memset( buffer, 0, sizeof(buffer) );

      sprintf( buffer, "%02X", c );

      return std::string( buffer );
    }

	/*========================================================================================================*/
	
	inline std::string ToHexString( unsigned char uc )
    {
      char buffer[( 2 * sizeof(uc) ) + 1];

	  memset( buffer, 0, sizeof(buffer) );

      sprintf( buffer, "%02X", uc );

      return std::string( buffer );
    }

	/*========================================================================================================*/

	inline std::wstring GetTempDir()
	{
	  const unsigned short BUFFER_SIZE = 512;
	  DWORD dwRetVal = 0;
	  WCHAR lpPathBuffer[BUFFER_SIZE];
	  std::wstring tempDir;
	  
	  // Get the temp path.
      dwRetVal = GetTempPath( BUFFER_SIZE,    // length of the buffer
                              lpPathBuffer ); // buffer for path 

	  if ( ( dwRetVal <= BUFFER_SIZE ) && ( dwRetVal != 0 ) )
	  {
	    tempDir = lpPathBuffer; 
	  }

	  return tempDir;
	}

	/*========================================================================================================*/

	inline std::wstring GetTempFile( const std::wstring& tempDir, const std::wstring& tempFileNamePrefix, bool unique )
	{
	  const unsigned short BUFFER_SIZE = 512;
	  UINT uRetVal = 0;
	  WCHAR lpFileBuffer[BUFFER_SIZE];
	  std::wstring tempFile;
		
	  // Create a temporary file. 
	  uRetVal = GetTempFileName( tempDir.c_str(),                // directory for tmp files
		                         tempFileNamePrefix.c_str(),     // temp file name prefix 
								 ( ( unique ) ? ( 0 ) : ( 1 ) ), // create unique name 
                                 lpFileBuffer );                 // buffer for name

	  if ( uRetVal != 0 )
	  {
	    tempFile = lpFileBuffer;
	  }

	  return tempFile;
	}

	/*========================================================================================================*/
	
    inline bool IsUnicodeSymbol( WCHAR symbol )
	{
	  bool result = false;
		
	  if ( ( 0x0009 == symbol ) || ( 0x000A == symbol ) || ( 0x000D == symbol ) ||
		   ( ( 0x0020 <= symbol ) && ( 0xD7FF >= symbol ) ) || ( ( 0xE000 <= symbol ) && ( symbol <= 0xFFFD ) ) ||
		   ( ( 0x10000 <= symbol ) && symbol ) )
	  {
	    result = true;  
	  }

	  return result;		  
	}
  }
}