#ifndef _BASE_64_H
#define _BASE_64_H

#include <iostream>


static const std::string c_sBase64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class CBase64
{
public:

	CBase64()
	{
	}
	~CBase64()
	{
	}

	BYTE*       GetBuffer(void)
	{
		return (BYTE*)m_sBuffer.c_str();
	}
	int         GetSize(void)
	{
		return m_sBuffer.length();
	}


	std::string GetString()
	{
		return m_sBuffer;
	}
	void        Encode(BYTE  *pBytesToEncode,  unsigned int   unSize)
	{
		m_sBuffer = Base64Encode( pBytesToEncode, unSize );
	}
	void        Encode(BYTE **ppBytesToEncode, unsigned int *punSize)
	{
		Encode( *ppBytesToEncode, *punSize );
		
		*punSize = GetSize();
		delete [](*ppBytesToEncode);
		*ppBytesToEncode = new BYTE[*punSize];
		if ( !(*ppBytesToEncode) )
			return;

		::memcpy( *ppBytesToEncode, GetBuffer(), sizeof(BYTE) * *punSize );
	}
	void        Decode(BYTE  *pBytesToDecode,  unsigned int   unSize)
	{
		std::string sEncodedString;
		for ( unsigned int unIndex = 0; unIndex < unSize; unIndex++ )
		{
			sEncodedString += pBytesToDecode[unIndex];
		}

		m_sBuffer = Base64Decode( sEncodedString );
	}

	void        Decode(BYTE **ppBytesToDecode, unsigned int *punSize)
	{
		Decode( *ppBytesToDecode, *punSize );
		
		*punSize = GetSize();
		delete [](*ppBytesToDecode);
		*ppBytesToDecode = new BYTE[*punSize];
		if ( !(*ppBytesToDecode) )
			return;

		::memcpy( *ppBytesToDecode, GetBuffer(), sizeof(BYTE) * *punSize );
	}
private:

	inline bool IsBase64(unsigned char nChar) 
	{
		return ( isalnum(nChar) || ( '+' == nChar ) || ( '/' == nChar ) );
	}

	std::string Base64Encode(unsigned char const *pBuffer, unsigned int unSize) 
	{
		std::string sResult;
		unsigned char arrChar_3[3];
		unsigned char arrChar_4[4];

		int nI = 0;
		int nJ = 0;
		while ( unSize-- ) 
		{
			arrChar_3[nI++] = *(pBuffer++);
			if ( 3 == nI ) 
			{
				arrChar_4[0] =   ( arrChar_3[0] & 0xfc ) >> 2;
				arrChar_4[1] = ( ( arrChar_3[0] & 0x03 ) << 4 ) + ( ( arrChar_3[1] & 0xf0 ) >> 4);
				arrChar_4[2] = ( ( arrChar_3[1] & 0x0f ) << 2 ) + ( ( arrChar_3[2] & 0xc0 ) >> 6);
				arrChar_4[3] =     arrChar_3[2] & 0x3f;

				for( nI = 0; nI < 4 ; nI++ )
				{
					sResult += c_sBase64Chars[arrChar_4[nI]];
				}
				nI = 0;
			}
		}

		if ( nI )
		{
			for( nJ = nI; nJ < 3; nJ++ )
			{
				arrChar_3[nJ] = '\0';
			}

			arrChar_4[0] =   ( arrChar_3[0] & 0xfc ) >> 2;
			arrChar_4[1] = ( ( arrChar_3[0] & 0x03 ) << 4 ) + ( ( arrChar_3[1] & 0xf0 ) >> 4 );
			arrChar_4[2] = ( ( arrChar_3[1] & 0x0f ) << 2 ) + ( ( arrChar_3[2] & 0xc0 ) >> 6 );
			arrChar_4[3] =     arrChar_3[2] & 0x3f;

			for ( nJ = 0; nJ < nI + 1; nJ++ )
			{
				sResult += c_sBase64Chars[arrChar_4[nJ]];
			}

			while( nI++ < 3 )
			{
				sResult += '=';
			}

		}

		return sResult;
	}
	std::string Base64Decode(std::string const& sEncodedString) 
	{
		std::string sResult;
		int nSize = sEncodedString.size();

		int nI = 0;
		int nJ = 0;
		int nPos = 0;
		unsigned char arrChar_3[3];
		unsigned char arrChar_4[4];

		while ( nSize-- && ( sEncodedString[nPos] != '=' ) && IsBase64( sEncodedString[nPos] ) ) 
		{
			arrChar_4[nI++] = sEncodedString[nPos]; 
			nPos++;

			if ( 4 == nI ) 
			{
				for ( nI = 0; nI < 4; nI++ )
				{
					arrChar_4[nI] = c_sBase64Chars.find( arrChar_4[nI] );
				}

				arrChar_3[0] =   ( arrChar_4[0]         << 2 ) + ( ( arrChar_4[1] & 0x30 ) >> 4 );
				arrChar_3[1] = ( ( arrChar_4[1] & 0xf ) << 4 ) + ( ( arrChar_4[2] & 0x3c ) >> 2 );
				arrChar_3[2] = ( ( arrChar_4[2] & 0x3 ) << 6 ) +     arrChar_4[3];

				for ( nI = 0; nI < 3; nI++ )
				{
					sResult += arrChar_3[nI];
				}
				nI = 0;
			}
		}

		if ( nI ) 
		{
			for ( nJ = nI; nJ < 4; nJ++ )
			{
				arrChar_4[nJ] = 0;
			}

			for ( nJ = 0; nJ < 4; nJ++)
			{
				arrChar_4[nJ] = c_sBase64Chars.find( arrChar_4[nJ] );
			}

			arrChar_3[0] =   ( arrChar_4[0]         << 2 ) + ( ( arrChar_4[1] & 0x30 ) >> 4 );
			arrChar_3[1] = ( ( arrChar_4[1] & 0xf ) << 4 ) + ( ( arrChar_4[2] & 0x3c ) >> 2 );
			arrChar_3[2] = ( ( arrChar_4[2] & 0x3 ) << 6 ) +     arrChar_4[3];

			for ( nJ = 0; nJ < nI - 1; nJ++ )
			{
				sResult += arrChar_3[nJ];
			}
		}

		return sResult;
	}

private:

	std::string m_sBuffer;

};

#endif /* _BASE_64_H */