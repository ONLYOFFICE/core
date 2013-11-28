#pragma once

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include <share.h>
#include <iosfwd>
#include <sstream>

#include <math.h>
#include <list>
#include <vector>
#include <string>
#include <bitset>
#include <set>
#include <map>
#include <algorithm>
#include <limits.h>

#include "utf8.h"

#include "AVSUtils.h"

using namespace std;

namespace AVSDocFormatUtils
{
	typedef byte Bool8;
	typedef unsigned short Bool16;
	typedef unsigned int Bool32;

	struct DeleteDynamicObject
	{
		template <typename T> void operator()( const T* ptr ) const
		{
			RELEASEOBJECT( ptr );
		}
	};

	typedef enum Encoding
	{
		ENCODING_INVALID_VALUE = 0x00000000,
		ENCODING_WINDOWS_1251 = 0x00000001,
		ENCODING_UNICODE = 0x00000002,
	} Encoding;

	typedef pair <int, int> Int_Pair;

	static const int gc_nZeroWidth = 222;

	class FormatUtils
	{
	public:
		static inline std::wstring XmlEncode(std::wstring data)
		{
			std::wstring buffer;
			buffer.reserve(data.size());
			for(size_t pos = 0; pos != data.size(); ++pos)
			{
				switch(data[pos])
				{
				case '&':  buffer.append(_T("&amp;"));      break;
				case '\"': buffer.append(_T("&quot;"));     break;
				case '\'': buffer.append(_T("&apos;"));     break;
				case '<':  buffer.append(_T("&lt;"));       break;
				case '>':  buffer.append(_T("&gt;"));       break;
				default:   buffer.append(&data[pos], 1);	break;
				}
			}
			return buffer;
		}

		static inline bool BitmaskToBool( int value, int mask )
		{
			return ( ( value & mask ) == mask );
		}

		/*========================================================================================================*/

		static inline int BytesToInt32( const unsigned char *b, unsigned int start, unsigned int length )
		{
			if ( ( b == NULL ) || ( ( start + 3 ) >= length ) )
			{
				return 0;
			}
			else
			{
				BYTE b0 = b[start];
				BYTE b1 = b[start+1];
				BYTE b2 = b[start+2];
				BYTE b3 = b[start+3];


				long intValue = ((b3 << 24) | (b2 << 16) | (b1 << 8) | b0);

				//int b0 = (int)b[start];
				//int b1 = (int)b[start+1] << 8;
				//int b2 = (int)b[start+2] << 16;
				//int b3 = (int)b[start+3] << 24;

				//int intValue = ( b0 ) | ( b1 ) | ( b2 ) | ( b3 );

				return intValue;
			}
		}

		/*========================================================================================================*/

		static inline unsigned int BytesToUInt32( const unsigned char *b, unsigned int start, unsigned int length )
		{
			if ( ( b == NULL ) || ( ( start + 3 ) >= length ) )
			{
				return 0;
			}
			else
			{
				unsigned int b0 = (unsigned int)b[start];
				unsigned int b1 = (unsigned int)b[start+1] << 8;
				unsigned int b2 = (unsigned int)b[start+2] << 16;
				unsigned int b3 = (unsigned int)b[start+3] << 24;

				unsigned int intValue = ( b0 ) | ( b1 ) | ( b2 ) | ( b3 );

				return intValue;
			}
		}

		/*========================================================================================================*/

		static inline short BytesToInt16( const unsigned char *b, unsigned int start, unsigned int length )
		{
			if ( ( b == NULL ) || ( ( start + 1 ) >= length ) )
			{
				return 0;
			}
			else
			{
				short b0 = (short)b[start];
				short b1 = (short)b[start+1] << 8;

				short shortValue = ( b0 ) | ( b1 );

				return shortValue;
			}
		}

		/*========================================================================================================*/

		static inline unsigned short BytesToUInt16( const unsigned char *b, unsigned int start, unsigned int length )
		{
			if ( ( b == NULL ) || ( ( start + 1 ) >= length ) )
			{
				return 0;
			}
			else
			{
				BYTE b0 = b[start];
				BYTE b1 = b[start+1];

				unsigned short shortValue = ((b1 << 8) | b0);

				//unsigned short b0 = (unsigned short)b[start];
				//unsigned short b1 = (unsigned short)b[start+1] << 8;

				//unsigned short shortValue = ( b0 ) | ( b1 );

				return shortValue;
			}
		}

		static inline unsigned char BytesToUChar (const unsigned char* pBuffer, unsigned int start, unsigned int length)
		{
			if ( (NULL == pBuffer) || (start >= length) )
				return 0;

			return pBuffer [start];
		}

		/*========================================================================================================*/

		/*static list<wchar_t>* ToUnicodeChars( const list<byte> *chars )
		{
		list<byte>::const_iterator iter = chars->begin();
		list<wchar_t> *unicodeChars = new list<wchar_t>();

		byte bytes[2] = { 0, 0 };

		while( iter != chars->end() )
		{
		bytes[0] = *iter++;
		bytes[1] = *iter++;

		unicodeChars->push_back( FormatUtils::BytesToUInt16( bytes, 0, 2 ) );
		}

		return unicodeChars;
		}*/

		/*========================================================================================================*/

		static inline bool IsControlSymbol( unsigned short c )
		{
			return ( c <= 31 ) ? ( true ) : ( false );  
		}

		/*========================================================================================================*/

		static inline wstring GetXMLSymbol( const WCHAR c )
		{
			wstring result;

			switch ( c )
			{
			case _T( '&' ):
				{
					result = wstring( _T( "&amp;" ) );
				}
				break;  

			case _T( '<' ):
				{
					result = wstring( _T( "&lt;" ) );
				}
				break;

			case _T( '>' ):
				{
					result = wstring( _T( "&gt;" ) );
				}
				break;

			case _T( '\"' ):
				{
					result = wstring( _T( "&quot;" ) );
				}
				break;

			case _T( '\'' ):
				{
					result = wstring( _T( "&apos;" ) );
				}
				break;

			default:
				{
					WCHAR res[2] = { c, _T( '\0' ) };
					result = wstring( res );
				}
				break;
			}

			return result;
		}

		/*========================================================================================================*/

		static inline WCHAR MapByteToWChar( byte byteSymbol )
		{
			WCHAR wcharSymbol = 0;

			switch ( byteSymbol )
			{
			case 0x82:
				{
					wcharSymbol = 0x201A;
				}
				break;

			case 0x83:
				{
					wcharSymbol = 0x0192;
				}
				break;

			case 0x84:
				{
					wcharSymbol = 0x201E;
				}
				break;

			case 0x85:
				{
					wcharSymbol = 0x2026;
				}
				break;

			case 0x86:
				{
					wcharSymbol = 0x2020;
				}
				break;

			case 0x87:
				{
					wcharSymbol = 0x2021;
				}
				break;

			case 0x88:
				{
					wcharSymbol = 0x02C6;
				}
				break;

			case 0x89:
				{
					wcharSymbol = 0x2030;
				}
				break;

			case 0x8A:
				{
					wcharSymbol = 0x0160;
				}
				break;

			case 0x8B:
				{
					wcharSymbol = 0x2039;
				}
				break;

			case 0x8C:
				{
					wcharSymbol = 0x0152;
				}
				break;

			case 0x91:
				{
					wcharSymbol = 0x2018;
				}
				break;

			case 0x92:
				{
					wcharSymbol = 0x2019;
				}
				break;

			case 0x93:
				{
					wcharSymbol = 0x201C;
				}
				break;

			case 0x94:
				{
					wcharSymbol = 0x201D;
				}
				break;

			case 0x95:
				{
					wcharSymbol = 0x2022;
				}
				break;

			case 0x96:
				{
					wcharSymbol = 0x2013;
				}
				break;

			case 0x97:
				{
					wcharSymbol = 0x2014;
				}
				break;

			case 0x98:
				{
					wcharSymbol = 0x02DC;
				}
				break;

			case 0x99:
				{
					wcharSymbol = 0x2122;
				}
				break;

			case 0x9A:
				{
					wcharSymbol = 0x0161;
				}
				break;

			case 0x9B:
				{
					wcharSymbol = 0x203A;
				}
				break;

			case 0x9C:
				{
					wcharSymbol = 0x0153;
				}
				break;

			case 0x9F:
				{
					wcharSymbol = 0x0178;
				}
				break;

			default:
				{
					wcharSymbol = (WCHAR)byteSymbol;
				}
				break;
			}

			return wcharSymbol;
		}

		/*========================================================================================================*/

		template<class T> static bool GetSTLCollectionFromBytes( T *STLCollection, byte *bytes, int size, Encoding encoding )
		{
			if ( ( STLCollection == NULL ) || ( bytes == NULL ) )
			{
				return false;
			}

			int i = 0;

			if ( encoding == ENCODING_UNICODE )
			{
				while ( i < size )
				{
					STLCollection->push_back( FormatUtils::BytesToUInt16( bytes, i, size ) );

					i += 2;
				}

				return true;
			}
			else if ( encoding == ENCODING_WINDOWS_1251 )
			{
				WCHAR wch = 0;

				while ( i < size )
				{
					wch = MapByteToWChar( bytes[i++] );

					STLCollection->push_back( wch );
				}

				return true;
			}

			return false;
		}

		/*========================================================================================================*/

		static int BitmaskToInt( int value, int mask )
		{
			int ret = value & mask;

			//shift for all trailing zeros
			bitset<sizeof(int)*8> bits( mask );

			for ( unsigned int i = 0; i < bits.size(); i++ )
			{
				if ( !bits[i] )
				{
					ret >>= 1;
				}
				else
				{
					break;
				}
			}

			return ret;
		}

		/*========================================================================================================*/

		static inline int GetIntFromBits( int value, unsigned int startBit, unsigned int bitsCount  )
		{
			int retval = value >> startBit; // Move to start bit.

			int bitmask = 0xFFFFFFFF >> ( 32 - bitsCount ); // Prapare mask.

			return ( retval & bitmask ); // Get value.
		}

		/*========================================================================================================*/

		static inline bool GetBitFromInt( int value, unsigned int bit )
		{
			if ( bit >= ( sizeof(int) * 8 ) )
			{
				return 0;
			}

			return ( ( value >> bit ) & 0x00000001 );
		}

		/*========================================================================================================*/

		static inline unsigned int GetUIntFromBytesBits( byte* bytes, unsigned int size, unsigned int startBit, unsigned int bitsCount )
		{
			if ( ( bytes == NULL ) || ( startBit >= ( size * 8 ) ) || ( bitsCount > ( ( size * 8 ) - startBit  ) ) )
			{
				return 0;
			}

			byte startByte = bytes[startBit/8]; // Get start byte.
			byte startBitInByte = startBit%8; // Get start bit in start byte.
			unsigned int result = 0;
			int bitValue = 0;

			for ( unsigned int i = startBitInByte, j = 0, k = 0; k < bitsCount; k++, i++ )
			{
				if ( i >= 8 )
				{
					j++;
					startByte = bytes[startBit/8 + j];
					i = 0;
				}

				bitValue = GetBitFromInt( startByte, i ); // Get bit value.
				result |= ( bitValue << k ); // Insert bit value to the result.
			}

			return result;
		}

		/*========================================================================================================*/

		static inline bool GetBitFromBytes( byte* bytes, unsigned int size, unsigned int bit )
		{
			if ( ( bytes == NULL ) || ( bit >= ( size * 8 ) ) )
			{
				return 0;
			}

			return (bool)GetBitFromInt( bytes[bit/8], bit%8 );
		}

		/*========================================================================================================*/

		static int ArraySum( byte* values, int size )
		{
			int ret = 0;

			if ( values != NULL )
			{
				for ( int i = 0; i < size; i++ )
				{
					ret += values[i];
				}
			}

			return ret;
		}

		/*========================================================================================================*/

		static inline byte* GetBytes( int value )
		{
			byte *bytes = new byte[4];

			bytes[0] = value & 0x000000FF;
			bytes[1] = ( value >> 8 ) & 0x000000FF;
			bytes[2] = ( value >> 16 ) & 0x000000FF;
			bytes[3] = ( value >> 24 ) & 0x000000FF;

			return bytes;
		}

		static inline wstring IntToWideString(int value, int radix = 10)
		{
			const int size = 33;

			WCHAR strValue[size];

			_itow_s(value, strValue, size, radix);

			return wstring(strValue);
		}

		static inline std::wstring DoubleToWideString(double value)
		{
			std::wstringstream src;
			
			src << value;
			
			return std::wstring(src.str());
		}

		static inline string IntToString(int value, int radix = 10)
		{
			const int size = 33;

			char strValue[size];

			_itoa_s(value, strValue, size, radix);

			return string(strValue);
		}

		static inline string DoubleToString(double value)
		{
			std::stringstream src;
			
			src << value;
			
			return std::string(src.str());
		}

		static inline wstring MapValueToWideString( unsigned int value, const WCHAR* mapArray, unsigned int size1, unsigned int size2 )
		{
			if ( mapArray == NULL )
			{
				return wstring( _T( "" ) ); 
			}

			if ( value < size1 )
			{
				return wstring( &mapArray[size2*value] );
			}
			else
			{
				return FormatUtils::IntToWideString( value );
			}
		}

		/*========================================================================================================*/

		static inline wstring IntToFormattedWideString( int value, const WCHAR* format )
		{
			const int size = 33;

			WCHAR strValue[size] = _T( "\0" );

			if ( format != NULL )
			{
				swprintf_s( strValue, size, format, value );
			}

			return wstring( strValue );
		}

		/*========================================================================================================*/

		static inline wstring DoubleToFormattedWideString( double value, WCHAR* format )
		{
			wstring wstr;

			if ( format != NULL )
			{
				const int size = 64;

				WCHAR wstrValue[size];

				if ( wstrValue != NULL )
				{
					memset( wstrValue, 0, ( sizeof(WCHAR) * size ) );

					swprintf_s( wstrValue, size, format, value );

					wstr = wstring( wstrValue );
				}
			}

			return wstr;
		}

		/*========================================================================================================*/

		static inline void SetBytes( byte *bytes, int value )
		{
			if ( bytes != NULL )
			{
				bytes[0] = value & 0x000000FF;
				bytes[1] = ( value >> 8 ) & 0x000000FF;
				bytes[2] = ( value >> 16 ) & 0x000000FF;
				bytes[3] = ( value >> 24 ) & 0x000000FF;
			}
		}

		/*========================================================================================================*/

		static inline void SetBytes( byte *bytes, short value )
		{
			if ( bytes != NULL )
			{
				bytes[0] = value & 0x000000FF;
				bytes[1] = ( value >> 8 ) & 0x000000FF;
			}
		}

		/*========================================================================================================*/

		static inline void SetBytes( byte *bytes, unsigned int value )
		{
			if ( bytes != NULL )
			{
				bytes[0] = value & 0x000000FF;
				bytes[1] = ( value >> 8 ) & 0x000000FF;
				bytes[2] = ( value >> 16 ) & 0x000000FF;
				bytes[3] = ( value >> 24 ) & 0x000000FF;
			}
		}

		/*========================================================================================================*/

		static inline void SetBytes( byte *bytes, unsigned short value )
		{
			if ( bytes != NULL )
			{
				bytes[0] = value & 0x000000FF;
				bytes[1] = ( value >> 8 ) & 0x000000FF;
			}
		}

		/*========================================================================================================*/

		static wstring UTF8Decode( const string& text )
		{
			wstring wstrText( text.size(), 0 );

			utf8_decode( text.begin(), text.end(), wstrText.begin() );

			return wstrText;
		}
	};

	struct Rectangle
	{
		Rectangle()
		{
			topLeftAngle.x	=	0;
			topLeftAngle.y	=	0;

			size.cx			=	0;
			size.cy			=	0;
		}

		Rectangle(const POINT& oTopLeftAngle, const SIZE& oSize)
		{
			topLeftAngle.x	=	oTopLeftAngle.x;
			topLeftAngle.y	=	oTopLeftAngle.y;

			size.cx			=	oSize.cx;
			size.cy			=	oSize.cy;
		}

		POINT topLeftAngle;
		SIZE size;
	};

	class BitSet
	{
	public:
		BitSet(unsigned int _size = 0): bytes(NULL), size(_size)
		{
			if (size)
			{
				bytes = new byte[size];

				if (bytes)
					memset(bytes, 0, size);
			}
		}

		inline void SetBit(bool value, unsigned int index)
		{
			if ( bytes && ( index < ( 8 * size ) ) )
			{
				unsigned int startByteIndex = ( index / 8 );
				byte* startByte = &this->bytes[startByteIndex]; // Get start byte.
				byte startBitInByteIndex = ( index % 8 ); // Get start bit in start byte.

				byte bit = ( ( value ) ? ( 1 ) : ( 0 ) );
				*startByte |= (byte)( bit << startBitInByteIndex );
			}
		}

		template<class T> inline void SetBits(T bits, unsigned int startBit, unsigned int countOfBits = ( 8 * sizeof(T) ))
		{
			if ( ( this->bytes != NULL ) && ( startBit < ( 8 * this->size ) ) )
			{
				unsigned int currentBitInValueIndex = 0;

				while ( currentBitInValueIndex < countOfBits )
				{
					unsigned int startByteIndex = ( startBit / 8 );
					byte* startByte = &this->bytes[startByteIndex]; // Get start byte.
					byte startBitInByteIndex = ( startBit % 8 ); // Get start bit in start byte.

					byte bit = ( ( bits >> currentBitInValueIndex ) & 1 );
					*startByte |= (byte)( bit << startBitInByteIndex );

					currentBitInValueIndex++;
					startBit++;
				}
			}
		}

		inline byte* GetBytes() const
		{
			return bytes;
		}

		inline unsigned int GetSize() const
		{
			return size;
		}

	private:

		unsigned int size;
		byte* bytes;
	};
}
