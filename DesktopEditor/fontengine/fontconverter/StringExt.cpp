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
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "MemoryUtils.h"
#include "StringExt.h"
#include "Utils.h"

namespace NSFontConverter
{
    //------------------------------------------------------------------------

    union StringExtFormatArg
    {
        int            iValue;
        unsigned int  uiValue;
        long           lValue;
        unsigned long ulValue;
        double         fValue;
        char           cValue;
        char          *sValue;
        StringExt    *seValue;
    };

    enum StringExtFormatType
    {
        fmtIntDecimal,
        fmtIntHex,
        fmtIntOctal,
        fmtIntBinary,
        fmtUIntDecimal,
        fmtUIntHex,
        fmtUIntOctal,
        fmtUIntBinary,
        fmtLongDecimal,
        fmtLongHex,
        fmtLongOctal,
        fmtLongBinary,
        fmtULongDecimal,
        fmtULongHex,
        fmtULongOctal,
        fmtULongBinary,
        fmtDouble,
        fmtDoubleTrim,
        fmtChar,
        fmtString,
        fmtStringExt,
        fmtSpace
    };

    static char *c_arrsFormatStrings[] =
    {
         "d",  "x",  "o",  "b",  "ud",  "ux",  "uo",  "ub",
        "ld", "lx", "lo", "lb", "uld", "ulx", "ulo", "ulb",
         "f", "g",
         "c",
         "s",
         "t",
         "w",
          NULL
    };

    //------------------------------------------------------------------------

    static inline int Size(int nLen)
    {
        int nDelta;
        for ( nDelta = 8; nDelta < nLen && nDelta < 0x100000; nDelta <<= 1 ) ;
        // ((nLen + 1) + (delta - 1)) & ~(delta - 1)
        return (nLen + nDelta) & ~( nDelta - 1);
    }

    inline void StringExt::Resize(int nLength)
    {
        char *sTemp = NULL;

        if ( !m_sData )
            m_sData = new char[Size(nLength)];
        else if ( Size(nLength) != Size(m_nLength) )
        {
            sTemp = new char[ Size(nLength) ];
            if ( nLength < m_nLength )
            {
                memcpy( sTemp, m_sData, nLength);
                sTemp[nLength] = '\0';
            }
            else
                memcpy( sTemp, m_sData, m_nLength + 1);
            delete[] m_sData;
            m_sData = sTemp;
        }
    }

    StringExt::StringExt()
    {
        m_sData = NULL;
        Resize( m_nLength = 0 );
        m_sData[0] = '\0';
    }

    StringExt::StringExt(const char *sSrc)
    {
        int nLen = (int)strlen( sSrc );

        m_sData = NULL;
        Resize(m_nLength = nLen);
        memcpy(m_sData, sSrc, nLen + 1);
    }

    StringExt::StringExt(const char *sSrc, int nLength)
    {
        m_sData = NULL;
        Resize( m_nLength = nLength );
        memcpy( m_sData, sSrc, m_nLength * sizeof(char) );
        m_sData[m_nLength] = '\0';
    }

    StringExt::StringExt(StringExt *seSrc, int nIndex, int nLength)
    {
        m_sData = NULL;
        Resize( m_nLength = nLength );
        memcpy( m_sData, seSrc->GetBuffer() + nIndex, m_nLength);
        m_sData[ m_nLength ] = '\0';
    }

    StringExt::StringExt(StringExt *seSrc)
    {
        m_sData = NULL;
        Resize( m_nLength = seSrc->GetLength() );
        memcpy( m_sData, seSrc->GetBuffer(), m_nLength + 1);
    }

    StringExt::StringExt(StringExt *seString1, StringExt *seString2)
    {
        int nLen1 = seString1->GetLength();
        int nLen2 = seString2->GetLength();

        m_sData = NULL;
        Resize( m_nLength = nLen1 + nLen2 );
        memcpy( m_sData, seString1->GetBuffer(), nLen1 );
        memcpy( m_sData + nLen1, seString2->GetBuffer(), nLen2 + 1 );
    }

    StringExt *StringExt::FromInt(int nValue)
    {
        char sBuffer[24];
        char *pData = NULL;
        int   nLen;

        FormatInt( nValue, sBuffer, sizeof( sBuffer ), false, 0, 10, &pData, &nLen);
        return new StringExt( pData, nLen );
    }

    StringExt *StringExt::Format(char *sFormat, ...)
    {
        va_list sArgList;
        StringExt *seResult = new StringExt();
        va_start( sArgList, sFormat );
        seResult->AppendFormatV( sFormat, sArgList);
        va_end( sArgList );
        return seResult;
    }

    StringExt *StringExt::FormatV(char *sFormat, va_list sArgList)
    {
        StringExt *seResult = new StringExt();
        seResult->AppendFormatV( sFormat, sArgList );
        return seResult;
    }

    StringExt::~StringExt()
    {
        delete[] m_sData;
    }

    StringExt *StringExt::Clear()
    {
        m_sData[ m_nLength = 0 ] = '\0';
        Resize(0);
        return this;
    }

    StringExt *StringExt::Append(char nChar)
    {
        Resize( m_nLength + 1 );
        m_sData[ m_nLength++ ] = nChar;
        m_sData[ m_nLength ] = '\0';
        return this;
    }

    StringExt *StringExt::Append(StringExt *seString)
    {
        int nLen = seString->GetLength();

        Resize( m_nLength + nLen );
        memcpy( m_sData + m_nLength, seString->GetBuffer(), nLen + 1 );
        m_nLength += nLen;
        return this;
    }

    StringExt *StringExt::Append(const char *sString)
    {
        int nLen = strlen( sString );

        Resize( m_nLength + nLen );
        memcpy( m_sData + m_nLength, sString, nLen + 1 );
        m_nLength += nLen;
        return this;
    }

    StringExt *StringExt::Append(const char *sString, int nLength)
    {
        Resize( m_nLength + nLength );
        memcpy( m_sData + m_nLength, sString, nLength );
        m_nLength += nLength;
        m_sData[ m_nLength ] = '\0';
        return this;
    }

    StringExt *StringExt::AppendFormat(char *sFormat, ...)
    {
        va_list sArgList;
        va_start( sArgList, sFormat );
        AppendFormatV( sFormat, sArgList );
        va_end( sArgList );
        return this;
    }

    StringExt *StringExt::AppendFormatV(char *sFormat, va_list sArgList)
    {
        StringExtFormatArg uArg;
        int nIndex, nWidth, nPrecision;
        bool bReverseAlign, bZeroFill;
        StringExtFormatType eFormatType;
        char sBuffer[65];
        int nLen;
        char *pCur, *pTemp, *sTemp;

        int nArgsLen = 0;
        int nArgsSize = 8;
        StringExtFormatArg *arrArgs = (StringExtFormatArg *)MemUtilsMallocArray( nArgsSize, sizeof(StringExtFormatArg));

        pCur = sFormat;
        while ( *pCur )
        {
            if ( *pCur == '{' )
            {
                ++pCur;
                if ( *pCur == '{' )
                {
                    ++pCur;
                    Append('{');
                }
                else
                {
                    // Разбираем форматированную строку
                    if ( !(*pCur >= '0' && *pCur <= '9') )
                        break;
                    nIndex = *pCur - '0';
                    for (++pCur; *pCur >= '0' && *pCur <= '9'; ++pCur )
                        nIndex = 10 * nIndex + (*pCur - '0');

                    if ( *pCur != ':' )
                        break;
                    ++pCur;
                    if ( *pCur == '-' )
                    {
                        bReverseAlign = true;
                        ++pCur;
                    }
                    else
                        bReverseAlign = false;
                    nWidth = 0;
                    bZeroFill = *pCur == '0';
                    for (; *pCur >= '0' && *pCur <= '9'; ++pCur )
                        nWidth = 10 * nWidth + (*pCur - '0');
                    if ( *pCur == '.' )
                    {
                        ++pCur;
                        nPrecision = 0;
                        for (; *pCur >= '0' && *pCur <= '9'; ++pCur )
                        {
                            nPrecision = 10 * nPrecision + (*pCur - '0');
                        }
                    }
                    else
                    {
                        nPrecision = 0;
                    }
                    for ( eFormatType = (StringExtFormatType)0;
                        c_arrsFormatStrings[ eFormatType ];
                    eFormatType = (StringExtFormatType)( eFormatType + 1 ) )
                    {
                        if (!strncmp( pCur, c_arrsFormatStrings[ eFormatType ], strlen(c_arrsFormatStrings[ eFormatType ])))
                        {
                            break;
                        }
                    }
                    if ( !c_arrsFormatStrings[ eFormatType ] )
                    {
                        break;
                    }
                    pCur += strlen( c_arrsFormatStrings[ eFormatType ] );
                    if (*pCur != '}')
                    {
                        break;
                    }
                    ++pCur;
                    // fetch the argument
                    if ( nIndex > nArgsLen )
                    {
                        break;
                    }
                    if ( nIndex == nArgsLen )
                    {
                        if ( nArgsLen == nArgsSize )
                        {
                            nArgsSize *= 2;
                            arrArgs = (StringExtFormatArg *)MemUtilsReallocArray( arrArgs, nArgsSize, sizeof(StringExtFormatArg));
                        }
                        switch ( eFormatType )
                        {
                        case fmtIntDecimal:
                        case fmtIntHex:
                        case fmtIntOctal:
                        case fmtIntBinary:
                        case fmtSpace:
                            arrArgs[nArgsLen].iValue = va_arg( sArgList, int );
                            break;
                        case fmtUIntDecimal:
                        case fmtUIntHex:
                        case fmtUIntOctal:
                        case fmtUIntBinary:
                            arrArgs[nArgsLen].uiValue = va_arg( sArgList, unsigned int );
                            break;
                        case fmtLongDecimal:
                        case fmtLongHex:
                        case fmtLongOctal:
                        case fmtLongBinary:
                            arrArgs[nArgsLen].lValue = va_arg( sArgList, long );
                            break;
                        case fmtULongDecimal:
                        case fmtULongHex:
                        case fmtULongOctal:
                        case fmtULongBinary:
                            arrArgs[nArgsLen].ulValue = va_arg( sArgList, unsigned long );
                            break;
                        case fmtDouble:
                        case fmtDoubleTrim:
                            arrArgs[nArgsLen].fValue = va_arg( sArgList, double );
                            break;
                        case fmtChar:
                            arrArgs[nArgsLen].cValue = (char)va_arg( sArgList, int );
                            break;
                        case fmtString:
                            arrArgs[nArgsLen].sValue = va_arg( sArgList, char * );
                            break;
                        case fmtStringExt:
                            arrArgs[nArgsLen].seValue = va_arg(sArgList, StringExt *);
                            break;
                        }
                        ++nArgsLen;
                    }

                    uArg = arrArgs[ nIndex ];
                    switch ( eFormatType )
                    {
                    case fmtIntDecimal:
                        FormatInt( uArg.iValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 10, &sTemp, &nLen );
                        break;
                    case fmtIntHex:
                        FormatInt( uArg.iValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 16, &sTemp, &nLen );
                        break;
                    case fmtIntOctal:
                        FormatInt( uArg.iValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 8, &sTemp, &nLen );
                        break;
                    case fmtIntBinary:
                        FormatInt( uArg.iValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 2, &sTemp, &nLen );
                        break;
                    case fmtUIntDecimal:
                        FormatUInt( uArg.uiValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 10, &sTemp, &nLen );
                        break;
                    case fmtUIntHex:
                        FormatUInt( uArg.uiValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 16, &sTemp, &nLen );
                        break;
                    case fmtUIntOctal:
                        FormatUInt( uArg.uiValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 8, &sTemp, &nLen );
                        break;
                    case fmtUIntBinary:
                        FormatUInt( uArg.uiValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 2, &sTemp, &nLen );
                        break;
                    case fmtLongDecimal:
                        FormatInt( uArg.lValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 10, &sTemp, &nLen );
                        break;
                    case fmtLongHex:
                        FormatInt( uArg.lValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 16, &sTemp, &nLen );
                        break;
                    case fmtLongOctal:
                        FormatInt( uArg.lValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 8, &sTemp, &nLen );
                        break;
                    case fmtLongBinary:
                        FormatInt( uArg.lValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 2, &sTemp, &nLen );
                        break;
                    case fmtULongDecimal:
                        FormatUInt( uArg.ulValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 10, &sTemp, &nLen );
                        break;
                    case fmtULongHex:
                        FormatUInt( uArg.ulValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 16, &sTemp, &nLen );
                        break;
                    case fmtULongOctal:
                        FormatUInt( uArg.ulValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 8, &sTemp, &nLen );
                        break;
                    case fmtULongBinary:
                        FormatUInt( uArg.ulValue, sBuffer, sizeof(sBuffer), bZeroFill, nWidth, 2, &sTemp, &nLen );
                        break;
                    case fmtDouble:
                        FormatDouble( uArg.fValue, sBuffer, sizeof(sBuffer), nPrecision, false, &sTemp, &nLen );
                        break;
                    case fmtDoubleTrim:
                        FormatDouble( uArg.fValue, sBuffer, sizeof(sBuffer), nPrecision, true, &sTemp, &nLen );
                        break;
                    case fmtChar:
                        sBuffer[0] = uArg.cValue;
                        sTemp = sBuffer;
                        nLen = 1;
                        bReverseAlign = !bReverseAlign;
                        break;
                    case fmtString:
                        sTemp = uArg.sValue;
                        nLen = strlen( sTemp );
                        bReverseAlign = !bReverseAlign;
                        break;
                    case fmtStringExt:
                        sTemp = uArg.seValue->GetBuffer();
                        nLen = uArg.seValue->GetLength();
                        bReverseAlign = !bReverseAlign;
                        break;
                    case fmtSpace:
                        sTemp = sBuffer;
                        nLen = 0;
                        nWidth = uArg.iValue;
                        break;
                    }
                    // Добавляем аргумент в нужном формате, с нужным прилеганием
                    if ( !bReverseAlign && nLen < nWidth )
                    {
                        for (int nCounter = nLen; nCounter < nWidth; ++nCounter )
                            Append(' ');
                    }
                    Append( sTemp, nLen);
                    if ( bReverseAlign && nLen < nWidth )
                    {
                        for (int nCounter = nLen; nCounter < nWidth; ++nCounter )
                            Append(' ');
                    }
                }
            }
            else if ( *pCur == '}' )
            {
                ++pCur;
                if ( *pCur  == '}' )
                    ++pCur;
                Append('}');
            }
            else
            {
                for ( pTemp = pCur + 1; *pTemp && *pTemp != '{' && *pTemp != '}'; ++pTemp ) ;
                Append( pCur, pTemp - pCur );
                pCur = pTemp;
            }
        }
        MemUtilsFree( arrArgs );
        return this;
    }

    void StringExt::FormatInt (         long nValue, char *sBuffer, int nBufferSize, bool bZeroFill, int nWidth, int nBase, char **ppData, int *pnLen)
    {
        static char c_sValues[17] = "0123456789abcdef";
        bool bNegative = false;
        int nStart;

        int nCur = nBufferSize;
        if ( ( bNegative = nValue < 0 ) )
        {
            nValue = -nValue;
        }
        nStart = bNegative ? 1 : 0;
        if ( nValue == 0 )
        {
            sBuffer[--nCur] = '0';
        }
        else
        {
            while ( nCur > nStart && nValue )
            {
                sBuffer[--nCur] = c_sValues[ nValue % nBase ];
                nValue /= nBase;
            }
        }
        if ( bZeroFill )
        {
            for (int nCounter = nBufferSize - nCur; nCur > nStart && nCounter < nWidth - nStart; ++nCounter )
            {
                sBuffer[--nCur] = '0';
            }
        }
        if ( bNegative )
        {
            sBuffer[--nCur] = '-';
        }
        *ppData = sBuffer + nCur;
        *pnLen = nBufferSize - nCur;
    }

    void StringExt::FormatUInt(unsigned long nValue, char *sBuffer, int nBufferSize, bool bZeroFill, int nWidth, int nBase, char **ppData, int *pnLen)
    {
        static char c_sValues[17] = "0123456789abcdef";

        int nCur = nBufferSize;
        if (nValue == 0)
        {
            sBuffer[--nCur] = '0';
        }
        else
        {
            while (nCur > 0 && nValue)
            {
                sBuffer[--nCur] = c_sValues[nValue % nBase];
                nValue /= nBase;
            }
        }
        if (bZeroFill)
        {
            for (int nCounter = nBufferSize - nCur; nCur > 0 && nCounter < nWidth; ++nCounter )
            {
                sBuffer[--nCur] = '0';
            }
        }
        *ppData = sBuffer + nCur;
        *pnLen = nBufferSize - nCur;
    }

    void StringExt::FormatDouble(     double dValue, char *sBuffer, int nBufferSize, int nPrecision, bool bTrim,            char **ppData, int *pnLen)
    {
        bool bNegative = false, bStarted = false;
        double dTemp = 0;
        int nInt;

        if ( bNegative = ( dValue < 0 ) )
            dValue = -dValue;

        dValue = floor( dValue * pow( (double)10, nPrecision) + 0.5);
        int nCur = nBufferSize;
        bStarted = !bTrim;
        for (int nCounter = 0; nCounter < nPrecision && nCur > 1; ++nCounter )
        {
            dTemp = floor(0.1 * (dValue + 0.5));
            nInt = (int)floor( dValue - 10 * dTemp + 0.5);
            if ( bStarted || nInt != 0 )
            {
                sBuffer[--nCur] = '0' + nInt;
                bStarted = true;
            }
            dValue = dTemp;
        }
        if ( nCur > 1 && bStarted )
            sBuffer[--nCur] = '.';
        if ( nCur > 1 )
        {
            do {
                dTemp = floor( 0.1 * ( dValue + 0.5 ) );
                nInt = (int)floor(dValue - 10 * dTemp + 0.5);
                sBuffer[--nCur] = '0' + nInt;
                dValue = dTemp;
            } while ( nCur > 1 && dValue );
        }
        if (bNegative)
            sBuffer[--nCur] = '-';

        *ppData = sBuffer + nCur;
        *pnLen = nBufferSize - nCur;
    }

    StringExt *StringExt::Insert(int nIndex, char nChar)
    {
        Resize( m_nLength + 1 );
        for (int nJ = m_nLength + 1; nJ > nIndex; --nJ )
            m_sData[ nJ ] = m_sData[ nJ - 1 ];
        m_sData[ nIndex ] = nChar;
        ++m_nLength;
        return this;
    }

    StringExt *StringExt::Insert(int nIndex, StringExt *seString)
    {
        int nLen = seString->GetLength();
        Resize( m_nLength + nLen );
        for (int nJ = m_nLength; nJ >= nIndex; --nJ )
            m_sData[ nJ + nLen ] = m_sData[ nJ ];
        memcpy( m_sData + nIndex, seString->GetBuffer(), nLen );
        m_nLength += nLen;
        return this;
    }

    StringExt *StringExt::Insert(int nIndex, const char *sString)
    {
        int nLen = strlen( sString );

        Resize(m_nLength + nLen);
        for (int nJ = m_nLength; nJ >= nIndex; --nJ )
            m_sData[ nJ + nLen ] = m_sData[ nJ ];
        memcpy( m_sData + nIndex, sString, nLen);
        m_nLength += nLen;
        return this;
    }

    StringExt *StringExt::Insert(int nIndex, const char *sString, int nLength)
    {
        Resize( m_nLength + nLength );
        for (int nJ = m_nLength; nJ >= nIndex; --nJ )
            m_sData[ nJ + nLength ] = m_sData[ nJ ];
        memcpy( m_sData + nIndex, sString, nLength );
        m_nLength += nLength;
        return this;
    }

    StringExt *StringExt::Delete(int nIndex, int nCount)
    {
        if ( nCount > 0 )
        {
            if ( nIndex + nCount > m_nLength)
                nCount = m_nLength - nIndex;
            for (int nJ = nIndex; nJ <= m_nLength - nCount; ++nJ )
            {
                m_sData[ nJ ] = m_sData[ nJ + nCount ];
            }
            Resize( m_nLength -= nCount );
        }
        return this;
    }

    StringExt *StringExt::MakeUpper()
    {
        for ( int nIndex = 0; nIndex < m_nLength; ++nIndex )
        {
            if ( islower( m_sData[ nIndex ] ) )
                m_sData[nIndex] = toupper( m_sData[nIndex] );
        }
        return this;
    }

    StringExt *StringExt::MakeLower()
    {
        for (int nIndex = 0; nIndex < m_nLength; ++nIndex)
        {
            if (isupper(m_sData[nIndex]))
                m_sData[nIndex] = tolower(m_sData[nIndex]);
        }
        return this;
    }

    int StringExt::Compare (StringExt *seOther)
    {
        int nIndex = 0;
        char *pThis, *pOther;

        int nThisLen = m_nLength;
        int nOtherLen = seOther->m_nLength;
        for (nIndex = 0, pThis = m_sData, pOther = seOther->m_sData; nIndex < nThisLen && nIndex < nOtherLen; ++nIndex, ++pThis, ++pOther)
        {
            int nRes = *pThis - *pOther;
            if ( 0 != nRes )
                return nRes;
        }
        return nThisLen - nOtherLen;
    }

    int StringExt::CompareN(StringExt *seOther, int nCount)
    {
        int nIndex;
        char *pThis, *pOther;

        int nThisLen = m_nLength;
        int nOtherLen = seOther->m_nLength;
        for (nIndex = 0, pThis = m_sData, pOther = seOther->m_sData; nIndex < nThisLen && nIndex < nOtherLen && nIndex < nCount; ++nIndex, ++pThis, ++pOther)
        {
            int nRes = *pThis - *pOther;
            if ( 0 != nRes )
                return nRes;
        }
        if (nIndex == nCount)
            return 0;
        return nThisLen - nOtherLen;
    }

    int StringExt::Compare (const char *sOther)
    {
        int nIndex;
        const char *pThis, *pOther;

        int nThisLen = m_nLength;
        for (nIndex = 0, pThis = m_sData, pOther = sOther; nIndex < nThisLen && *pOther; ++nIndex, ++pThis, ++pOther)
        {
            int nRes = *pThis - *pOther;
            if ( 0 != nRes )
                return nRes;
        }
        if ( nIndex < nThisLen )
            return 1;
        if ( *pOther )
            return -1;
        return 0;
    }

    int StringExt::CompareN(const char *sOther, int nCount)
    {
        int nIndex;
        const char *pThis, *pOther;

        int nThisLen = m_nLength;
        for (nIndex = 0, pThis = m_sData, pOther = sOther; nIndex < nThisLen && *pOther && nIndex < nCount; ++nIndex, ++pThis, ++pOther )
        {
            int nRes = *pThis - *pOther;
            if ( 0 != nRes )
                return nRes;
        }
        if ( nIndex == nCount )
            return 0;
        if ( nIndex < nThisLen)
            return 1;
        if ( *pOther )
            return -1;
        return 0;
    }
}
