/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include <iosfwd>
#include <sstream>

#include <list>
#include <vector>
#include <string>
#include <bitset>
#include <set>
#include <map>

#include <limits.h>
#include <math.h>

#include "utf8.h"

#if defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#else
    #include <string.h>
#endif

#include "../../../OOXML/Base/Base.h"
#include "../../../DesktopEditor/common/Types.h"
#include "../../../OOXML/Base/unicode_util.h"
#include "../../../UnicodeConverter/UnicodeConverter.h"

#include <boost/format.hpp>

#define GETBITS(from, numL, numH) ((from & (((1 << (numH - numL + 1)) - 1) << numL)) >> numL)
#define GETBIT(from, num) ((from & (1 << num)) != 0)

namespace DocFileFormat
{
    typedef unsigned char   Bool8;
    typedef unsigned short  Bool16;
    typedef unsigned int    Bool32;

#if !defined(_WIN32) && !defined(_WIN64)
    struct POINT
    {
        int32_t x;
        int32_t y;
    };
    struct SIZE
    {
        int32_t cx;
        int32_t cy;
    };
    struct RECT
    {
        int32_t left;
        int32_t top;
        int32_t right;
        int32_t bottom;
    };
#endif

    struct DeleteDynamicObject
	{
		template <typename T> void operator()( const T* ptr ) const
		{
			RELEASEOBJECT(ptr);
		}
	};


	typedef std::pair <int, int> Int_Pair;

#define ENCODING_UTF16			1200
#define ENCODING_WINDOWS_1250	1250
#define ENCODING_UTF8			65001

	class FormatUtils
	{
	public:
		static inline bool IsUnicodeSymbol( wchar_t symbol )
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
		static inline std::wstring XmlEncode(std::wstring data, bool bDeleteNoUnicode = false)
		{
			std::wstring buffer;
			buffer.reserve(data.size());
			
			if(bDeleteNoUnicode)
			{
				for(size_t pos = 0; pos < data.size(); ++pos)
				{
					switch(data[pos])
					{
                        case '&':  buffer.append(L"&amp;");      break;
                        case '\"': buffer.append(L"&quot;");     break;
                        case '\'': buffer.append(L"&apos;");     break;
                        case '<':  buffer.append(L"&lt;");       break;
                        case '>':  buffer.append(L"&gt;");       break;
						default:   
						{
							if ( false == IsUnicodeSymbol( data[pos] ) )
							{
								wchar_t symbol1 = data[pos];
								if(0xD800 <= symbol1 && symbol1 <= 0xDFFF && pos + 1 < data.size())
								{
									pos++;
									wchar_t symbol2 = data[pos];
									if (symbol1 < 0xDC00 && symbol2 >= 0xDC00 && symbol2 <= 0xDFFF)
									{
										buffer.append(&data[pos-1], 2);
									}
								}
							}
							else
								buffer.append(&data[pos], 1);	
						}break;
					}
				}
			}
			else
			{
				for(size_t pos = 0; pos < data.size(); ++pos)
				{
					switch(data[pos])
					{
                        case '&':  buffer.append(L"&amp;");      break;
                        case '\"': buffer.append(L"&quot;");     break;
                        case '\'': buffer.append(L"&apos;");     break;
                        case '<':  buffer.append(L"&lt;");       break;
                        case '>':  buffer.append(L"&gt;");       break;
						case '\0':  
							return buffer;
						default:   buffer.append(&data[pos], 1);	break;
					}
				}
			}
		

			return buffer;
		}

		static inline bool BitmaskToBool( int value, int mask )
		{
			return ( ( value & mask ) == mask );
		}

		static inline int BytesToInt32( const unsigned char *b, unsigned int start, unsigned int length )
		{
			if ( ( b == NULL ) || ( ( start + 3 ) >= length ) )
			{
				return 0;
			}
			else
			{
				return ((_INT32*)(b + start))[0];
			}
		}

		static inline unsigned int BytesToUInt32( const unsigned char *b, unsigned int start, unsigned int length )
		{
			if ( ( b == NULL ) || ( ( start + 3 ) >= length ) )
			{
				return 0;
			}
			else
			{
				return ((_UINT32*)(b + start))[0];
			}
		}
		static inline _UINT64 BytesToInt64(const unsigned char *b, unsigned int start, unsigned int length)
		{
			if ((b == NULL) || ((start + 7) >= length))
			{
				return 0;
			}
			else
			{
				return ((_INT64*)(b + start))[0];
			}
		}
		static inline _UINT64 BytesToUInt64(const unsigned char *b, unsigned int start, unsigned int length)
		{
			if ((b == NULL) || ((start + 7) >= length))
			{
				return 0;
			}
			else
			{
				return ((_UINT64*)(b + start))[0];
			}
		}
		static inline short BytesToInt16( const unsigned char *b, unsigned int start, unsigned int length )
		{
			if ( ( b == NULL ) || ( ( start + 1 ) >= length ) )
			{
				return 0;
			}
			else
			{
				return ((_INT16*)(b + start))[0];
			}
		}

		static inline unsigned short BytesToUInt16( const unsigned char *b, unsigned int start, unsigned int length )
		{
			if ( ( b == NULL ) || ( ( start + 1 ) >= length ) )
			{
				return 0;
			}
			else
			{
				return ((_UINT16*)(b + start))[0];
			}
		}

		static inline unsigned char BytesToUChar (const unsigned char* pBuffer, unsigned int start, unsigned int length)
		{
			if ( (NULL == pBuffer) || (start >= length) )
				return 0;

			return pBuffer [start];
		}

		static inline bool IsControlSymbol( unsigned short c )
		{
			return ( c <= 31 ) ? ( true ) : ( false );  
		}

		static inline std::wstring GetXMLSymbol( const wchar_t c )
		{
			std::wstring result;

			switch ( c )
			{
            case L'&':
				{
                    result = std::wstring( L"&amp;" );
				}
				break;  

            case L'<':
				{
                    result = std::wstring( L"&lt;" );
				}
				break;

            case L'>':
				{
                    result = std::wstring( L"&gt;" );
				}
				break;

            case L'\"':
				{
                    result = std::wstring( L"&quot;" );
				}
				break;

            case L'\'':
				{
                    result = std::wstring( L"&apos;" );
				}
				break;

			default:
				{
                    wchar_t res[2] = { c, 0 };
                    result = std::wstring( res );
				}
				break;
			}

			return result;
		}

		static inline wchar_t MapByteToWChar( unsigned char byteSymbol )
		{
			wchar_t wcharSymbol = 0;

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
					wcharSymbol = (wchar_t)byteSymbol;
				}
				break;
			}

			return wcharSymbol;
		}

		template<class T> static bool GetSTLCollectionFromLocale( T *STLCollection, unsigned char *bytes, int size)
		{
			if ( ( STLCollection == NULL ) || ( bytes == NULL ) )
			{
				return false;
			}

			std::locale loc("");
			std::ctype<wchar_t> const &facet = std::use_facet<std::ctype<wchar_t> >(loc);

			std::wstring result;
			result.resize(size);
    
			facet.widen((char*)bytes, (char*)bytes + size, &result[0]);
			
			for (size_t i=0; i < result.length(); i++)
			{
				STLCollection->push_back(result[i]);
			}
            return true;
		}

		template<class T> static bool GetSTLCollectionFromUtf8( T *STLCollection, unsigned char *bytes, int size)
		{
			if ( ( STLCollection == NULL ) || ( bytes == NULL ) )
			{
				return false;
			}
			if (sizeof(wchar_t) == 2)//utf8 -> utf16
			{
				unsigned int nLength = size;

				UTF16 *pStrUtf16 = new UTF16 [nLength+1];
				memset ((void *) pStrUtf16, 0, sizeof (UTF16) * (nLength+1));

				UTF8 *pStrUtf8 = (UTF8 *) bytes;

				// this values will be modificated
				const UTF8 *pStrUtf8_Conv	= pStrUtf8;
				UTF16 *pStrUtf16_Conv		= pStrUtf16;

				ConversionResult eUnicodeConversionResult = ConvertUTF8toUTF16 (&pStrUtf8_Conv,	 &pStrUtf8[nLength]
						, &pStrUtf16_Conv, &pStrUtf16 [nLength]
						, strictConversion);

				if (conversionOK != eUnicodeConversionResult)
				{
					delete [] pStrUtf16;
					return GetSTLCollectionFromLocale(STLCollection, bytes,size);
				}
				for (unsigned int i = 0; i < nLength; i++)
				{
					STLCollection->push_back(pStrUtf16[i]);
				}
				delete [] pStrUtf16;
				return true;
			}
			else //utf8 -> utf32
			{
				unsigned int nLength = size;

				UTF32 *pStrUtf32 = new UTF32 [nLength+1];
				memset ((void *) pStrUtf32, 0, sizeof (UTF32) * (nLength+1));


				UTF8 *pStrUtf8 = (UTF8 *) bytes;

				// this values will be modificated
				const UTF8 *pStrUtf8_Conv = pStrUtf8;
				UTF32 *pStrUtf32_Conv = pStrUtf32;

				ConversionResult eUnicodeConversionResult = ConvertUTF8toUTF32 (&pStrUtf8_Conv, &pStrUtf8[nLength]
						, &pStrUtf32_Conv, &pStrUtf32 [nLength]
						, strictConversion);

				if (conversionOK != eUnicodeConversionResult)
				{
					delete [] pStrUtf32;
					return GetSTLCollectionFromLocale(STLCollection, bytes, size);
				}
				for (unsigned int i = 0; i < nLength; i++)
				{
					STLCollection->push_back(pStrUtf32[i]);
				}
				delete [] pStrUtf32;
				return true;
			}
		}

		template<class T> static bool GetSTLCollectionFromBytes( T *STLCollection, unsigned char *bytes, int size, int code_page )
		{
			if ( ( STLCollection == NULL ) || ( bytes == NULL ) )
			{
				return false;
			}

			if (code_page == ENCODING_UTF8)
			{
				return GetSTLCollectionFromUtf8(STLCollection, bytes, size);
			}
			else if (code_page == ENCODING_UTF16)
			{
				int i = 0;
#if !defined(_WIN32) && !defined(_WIN64)
               size /= 2;
               ConversionResult eUnicodeConversionResult;
               UTF32 *pStrUtf32 = new UTF32 [size + 1];

               memset ((void *) pStrUtf32, 0, sizeof (UTF32) * (size + 1));

               const   UTF16 *pStrUtf16_Conv = (const UTF16 *) bytes;
                       UTF32 *pStrUtf32_Conv =                 pStrUtf32;

    eUnicodeConversionResult = ConvertUTF16toUTF32 ( &pStrUtf16_Conv
                                               , &pStrUtf16_Conv[size]
                                               , &pStrUtf32_Conv
                                               , &pStrUtf32 [size]
                                               , strictConversion);

                if (conversionOK != eUnicodeConversionResult)
                {
                    delete [] pStrUtf32; pStrUtf32 = NULL;
                    return false;
                }
                for (long i=0; i < size; i++)
                {
                    STLCollection->push_back(pStrUtf32[i]);
                }
                if (pStrUtf32) delete [] pStrUtf32;
#else
                while ( i < size )
				{
					STLCollection->push_back( FormatUtils::BytesToUInt16( bytes, i, size ) );

					i += 2;
                }
#endif
			}
			else if (code_page == ENCODING_WINDOWS_1250)
			{
				wchar_t wch = 0;
				int i = 0;
				while ( i < size )
				{
					wch = MapByteToWChar( bytes[i++] );

					STLCollection->push_back( wch );
				}
			}
			else
            {
				std::string sCodePage;
				std::map<int, std::string>::const_iterator pFind = NSUnicodeConverter::mapEncodingsICU.find(code_page);
				if (pFind != NSUnicodeConverter::mapEncodingsICU.end())
				{
					sCodePage = pFind->second;
				}

				if (sCodePage.empty())
					sCodePage = "CP1250"/* + std::to_string(code_page)*/;

				NSUnicodeConverter::CUnicodeConverter oConverter;
                std::wstring unicode_string = oConverter.toUnicode((char*)bytes, (unsigned int)size, sCodePage.c_str());
			
				for (size_t i = 0; i < unicode_string.size(); i++)
				{
					STLCollection->push_back(unicode_string[i]);
				}
			}

			return true;
		}

		static int BitmaskToInt( int value, int mask )
		{
			int ret = value & mask;

			//shift for all trailing zeros
			std::bitset<sizeof(int)*8> bits( mask );

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

		static inline int GetIntFromBits( int value, unsigned int startBit, unsigned int bitsCount  )
		{
			int retval = value >> startBit; // Move to start bit.

			int bitmask = 0xFFFFFFFF >> ( 32 - bitsCount ); // Prapare mask.

			return ( retval & bitmask ); // Get value.
		}

		static inline bool GetBitFromInt( int value, unsigned int bit )
		{
			if ( bit >= ( sizeof(int) * 8 ) )
			{
				return 0;
			}

			return ( ( value >> bit ) & 0x00000001 );
		}

		static inline unsigned int GetUIntFromBytesBits( unsigned char* bytes, unsigned int size, unsigned int startBit, unsigned int bitsCount )
		{
			if ( ( bytes == NULL ) || ( startBit >= ( size * 8 ) ) || ( bitsCount > ( ( size * 8 ) - startBit  ) ) )
			{
				return 0;
			}

			unsigned char startByte = bytes[startBit/8]; // Get start unsigned char.
			unsigned char startBitInByte = startBit%8; // Get start bit in start unsigned char.
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

		static inline bool GetBitFromBytes( unsigned char* bytes, unsigned int size, unsigned int bit )
		{
			if ( ( bytes == NULL ) || ( bit >= ( size * 8 ) ) )
			{
				return 0;
			}

			return (bool)GetBitFromInt( bytes[bit/8], bit%8 );
		}

		static int ArraySum( unsigned char* values, int size )
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

		static inline unsigned char* GetBytes( int value )
		{
			unsigned char *bytes = new unsigned char[4];

			bytes[0] = value & 0x000000FF;
			bytes[1] = ( value >> 8 ) & 0x000000FF;
			bytes[2] = ( value >> 16 ) & 0x000000FF;
			bytes[3] = ( value >> 24 ) & 0x000000FF;

			return bytes;
		}
		static inline std::wstring IntToWideString(unsigned int value)
		{
            return (std::to_wstring(value));
		}
        static inline std::wstring IntToWideString(int value)
		{
            return (std::to_wstring(value));
		}  
        static inline std::wstring SizeTToWideString(size_t value)
		{
            return (std::to_wstring((unsigned int)value));
		}
		static inline std::wstring DoubleToWideString(double value)
		{
			std::wstringstream src;			
			src << value;
			
			return std::wstring(src.str());
		}

		static inline std::string DoubleToString(double value)
		{
			std::stringstream src;			
			src << value;
			
			return std::string(src.str());
		}

		static inline std::wstring MapValueToWideString( unsigned int value, const wchar_t* mapArray, unsigned int size1, unsigned int size2 )
		{
            std::wstring out;
            if ( mapArray == NULL )
			{
                out = std::wstring( L"" );
			}

			if ( value < size1 )
			{
                out = std::wstring( &mapArray[size2*value] );
			}
			else
			{
                out = FormatUtils::IntToWideString( value );
            }
            return out;
		}

		static inline std::wstring IntToFormattedWideString( int value, const wchar_t* format )
		{
			if ( format == NULL ) return L"";
			std::wstringstream sstream;
			sstream << boost::wformat(format) % value;
			return sstream.str();
		}

		static inline std::wstring DoubleToFormattedWideString( double value, wchar_t* format )
		{
			if ( format == NULL ) return L"";

			std::wstringstream sstream;
			sstream << boost::wformat(format) % value;
			return sstream.str();
		}

		static inline void SetBytes( unsigned char *bytes, int value )
		{
			if ( bytes != NULL )
			{
				bytes[0] = value & 0x000000FF;
				bytes[1] = ( value >> 8 ) & 0x000000FF;
				bytes[2] = ( value >> 16 ) & 0x000000FF;
				bytes[3] = ( value >> 24 ) & 0x000000FF;
			}
		}

		static inline void SetBytes( unsigned char *bytes, short value )
		{
			if ( bytes != NULL )
			{
				bytes[0] = value & 0x000000FF;
				bytes[1] = ( value >> 8 ) & 0x000000FF;
			}
		}

		static inline void SetBytes( unsigned char *bytes, unsigned int value )
		{
			if ( bytes != NULL )
			{
				bytes[0] = value & 0x000000FF;
				bytes[1] = ( value >> 8 ) & 0x000000FF;
				bytes[2] = ( value >> 16 ) & 0x000000FF;
				bytes[3] = ( value >> 24 ) & 0x000000FF;
			}
		}

		static inline void SetBytes( unsigned char *bytes, unsigned short value )
		{
			if ( bytes != NULL )
			{
				bytes[0] = value & 0x000000FF;
				bytes[1] = ( value >> 8 ) & 0x000000FF;
			}
		}

		static std::wstring UTF8Decode( const std::string& text )
		{
			std::wstring wstrText( text.size(), 0 );

			utf8_decode( text.begin(), text.end(), wstrText.begin() );

			return wstrText;
		}
	};

	struct Rectangle
	{
	public:
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

	public:
		POINT	topLeftAngle;
		SIZE	size;
	};

	class BitSet
	{
	public:
		BitSet(unsigned int _size = 0): bytes(NULL), size(_size)
		{
			if (size)
			{
				bytes = new unsigned char[size];

				if (bytes)
					memset(bytes, 0, size);
			}
		}

		inline void SetBit(bool value, unsigned int index)
		{
			if ( bytes && ( index < ( 8 * size ) ) )
			{
				unsigned int startByteIndex = ( index / 8 );
				unsigned char* startByte = &this->bytes[startByteIndex]; // Get start unsigned char.
				unsigned char startBitInByteIndex = ( index % 8 ); // Get start bit in start unsigned char.

				unsigned char bit = ( ( value ) ? ( 1 ) : ( 0 ) );
				*startByte |= (unsigned char)( bit << startBitInByteIndex );
			}
		}

		template<class T> inline void SetBits(T bits, unsigned int startBit, unsigned int countOfBits = (8 * sizeof(T)))
		{
			if ( ( this->bytes != NULL ) && ( startBit < ( 8 * this->size ) ) )
			{
				unsigned int currentBitInValueIndex = 0;

				while ( currentBitInValueIndex < countOfBits )
				{
					unsigned int startByteIndex = ( startBit / 8 );
					unsigned char* startByte = &this->bytes[startByteIndex]; // Get start unsigned char.
					unsigned char startBitInByteIndex = ( startBit % 8 ); // Get start bit in start unsigned char.

					unsigned char bit = ( ( bits >> currentBitInValueIndex ) & 1 );
					*startByte |= (unsigned char)( bit << startBitInByteIndex );

					currentBitInValueIndex++;
					startBit++;
				}
			}
		}

		inline unsigned char* GetBytes() const
		{
			return bytes;
		}

		inline unsigned int GetSize() const
		{
			return size;
		}

	private:
		unsigned int	size;
		unsigned char*			bytes;
	};
}

