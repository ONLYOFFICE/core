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


#include "XORCrypt.h"

#include <boost/scoped_array.hpp>

typedef unsigned char SVBT16[2];
inline void ShortToSVBT16( unsigned short n, SVBT16 p ) { p[0] = (unsigned char) n;
                                                      p[1] = (unsigned char)(n >>  8); }

template< typename Type >
inline void lclRotateLeft( Type& rnValue, int nBits )
{
    //OSL_ASSERT(
    //    nBits >= 0 &&
    //    sal::static_int_cast< unsigned int >(nBits) < sizeof( Type ) * 8 );
    rnValue = static_cast< Type >( (rnValue << nBits) | (rnValue >> (sizeof( Type ) * 8 - nBits)) );
}

template< typename Type >
inline void lclRotateLeft( Type& rnValue, unsigned char nBits, unsigned char nWidth )
{
    //OSL_ASSERT( (nBits < nWidth) && (nWidth < sizeof( Type ) * 8) );
    Type nMask = static_cast< Type >( (1UL << nWidth) - 1 );
    rnValue = static_cast< Type >(
        ((rnValue << nBits) | ((rnValue & nMask) >> (nWidth - nBits))) & nMask );
}
namespace CRYPT
{
size_t lclGetLen( const unsigned char* pnPassData, size_t nBufferSize )
{
    size_t nLen = 0;
    while( (nLen < nBufferSize) && pnPassData[ nLen ] ) ++nLen;
    return nLen;
}

unsigned short lclGetKey( const unsigned char* pnPassData, size_t nBufferSize )
{
    size_t nLen = lclGetLen( pnPassData, nBufferSize );
    if( !nLen ) return 0;

    unsigned short nKey = 0;
    unsigned short nKeyBase = 0x8000;
    unsigned short nKeyEnd = 0xFFFF;
   
	const unsigned char* pnChar = pnPassData + nLen - 1;
    for( size_t nIndex = 0; nIndex < nLen; ++nIndex, --pnChar )
    {
        unsigned char cChar = *pnChar & 0x7F;
        for( unsigned char nBit = 0; nBit < 8; ++nBit )
        {
            lclRotateLeft( nKeyBase, 1 );
            if( nKeyBase & 1 ) nKeyBase ^= 0x1020;
            if( cChar & 1 ) nKey ^= nKeyBase;
            cChar >>= 1;
            lclRotateLeft( nKeyEnd, 1 );
            if( nKeyEnd & 1 ) nKeyEnd ^= 0x1020;
        }
    }
    return nKey ^ nKeyEnd;
}

unsigned short lclGetHash( const unsigned char* pnPassData, size_t nBufferSize )
{
    size_t nLen = lclGetLen( pnPassData, nBufferSize );

    unsigned short nHash = static_cast< unsigned short >( nLen );
    if( nLen )
        nHash ^= 0xCE4B;

    const unsigned char* pnChar = pnPassData;
    for( size_t nIndex = 0; nIndex < nLen; ++nIndex, ++pnChar )
    {
        unsigned short cChar = *pnChar;
        unsigned char nRot = static_cast< unsigned char >( (nIndex + 1) % 15 );
        lclRotateLeft( cChar, nRot, 15 );
        nHash ^= cChar;
    }
    return nHash;
}



XORCrypt::XORCrypt(int type, unsigned short key, unsigned short hash, std::wstring password) :
    m_nOffset(0),
    m_nKey(0),
    m_nHash(0)
{
	if (type == 1) m_nRotateDistance = 7; //doc
	else if (type == 2) m_nRotateDistance = 2; //xls

    memset( m_pnKey, 0, sizeof( m_pnKey ) );

	m_VerifyPassword = false;

	size_t nLen = password.length();
	if (nLen > 16) return;

	m_sPassword = std::string(password.begin(), password.end());

	unsigned char* pnPassData = (unsigned char*)m_sPassword.c_str();

    m_nKey = lclGetKey( pnPassData, 16 );
    m_nHash = lclGetHash( pnPassData, 16 );

    memcpy( m_pnKey, pnPassData, 16 );

    static const unsigned char spnFillChars[] =
    {
        0xBB, 0xFF, 0xFF, 0xBA, 0xFF, 0xFF, 0xB9, 0x80, 0x00, 0xBE, 0x0F, 0x00, 0xBF, 0x0F, 0x00
    };

    nLen = lclGetLen( pnPassData, 16 );
   
	const unsigned char* pnFillChar = spnFillChars;
    
	for ( size_t nIndex = nLen; nIndex < sizeof( m_pnKey ); ++nIndex, ++pnFillChar )
        m_pnKey[ nIndex ] = *pnFillChar;

    SVBT16 pnOrigKey;
    ShortToSVBT16( m_nKey, pnOrigKey );
    
	unsigned char* pnKeyChar = m_pnKey;
    for ( size_t nIndex = 0; nIndex < sizeof( m_pnKey ); ++nIndex, ++pnKeyChar )
    {
        *pnKeyChar ^= pnOrigKey[ nIndex & 0x01 ];
        lclRotateLeft( *pnKeyChar, m_nRotateDistance );
    }

    m_VerifyPassword = (key == m_nKey) && (hash == m_nHash);
}


bool XORCrypt::IsVerify()
{
	return m_VerifyPassword;
}
void XORCrypt::Init(const unsigned long val)
{
	m_nOffset = val & 0x0F;
}
void XORCrypt::Decrypt(char* data, const size_t size, const unsigned long block_index)
{
	unsigned char* pnData = (unsigned char*)data;
    
	const unsigned char* pnCurrKey = m_pnKey + m_nOffset;
    const unsigned char* pnKeyLast = m_pnKey + 0x0F;

	if (m_nRotateDistance == 7)
	{
		for( const unsigned char* pnDataEnd = pnData + size; pnData < pnDataEnd; ++pnData )
		{
			const unsigned char cChar = *pnData ^ *pnCurrKey;
			if (*pnData && cChar)
				*pnData = cChar;

			if( pnCurrKey < pnKeyLast )
				++pnCurrKey;
			else
				pnCurrKey = m_pnKey;
		}
	}
	if (m_nRotateDistance == 2)
	{
		for( const unsigned char* pnDataEnd = pnData + size; pnData < pnDataEnd; ++pnData )
		{
			lclRotateLeft( *pnData, 3 );
			*pnData ^= *pnCurrKey;
	       
			if( pnCurrKey < pnKeyLast )
				++pnCurrKey; 
			else
				pnCurrKey = m_pnKey;
		}
	}

	Skip(size);
}
void XORCrypt::Decrypt(char* data, const size_t size, const unsigned long stream_pos, const size_t block_size)
{
	Init(stream_pos + size);

	unsigned char* pnData = (unsigned char*)data;
    
	const unsigned char* pnCurrKey = m_pnKey + m_nOffset;
    const unsigned char* pnKeyLast = m_pnKey + 0x0F;

	if (m_nRotateDistance == 7)
	{
		for( const unsigned char* pnDataEnd = pnData + size; pnData < pnDataEnd; ++pnData )
		{
			const unsigned char cChar = *pnData ^ *pnCurrKey;
			if (*pnData && cChar)
				*pnData = cChar;

			if( pnCurrKey < pnKeyLast )
				++pnCurrKey;
			else
				pnCurrKey = m_pnKey;
		}
	}
	if (m_nRotateDistance == 2)
	{
		for( const unsigned char* pnDataEnd = pnData + size; pnData < pnDataEnd; ++pnData )
		{
			lclRotateLeft( *pnData, 3 );
			*pnData ^= *pnCurrKey;
	       
			if( pnCurrKey < pnKeyLast )
				++pnCurrKey; 
			else
				pnCurrKey = m_pnKey;
		}
	}
}
void XORCrypt::Skip( size_t size )
{
    m_nOffset = (m_nOffset + size) & 0x0F;
}

};


