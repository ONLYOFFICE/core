/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef _ASC_FONTCONVERTER_FONT_FILE_TYPE1C_H
#define _ASC_FONTCONVERTER_FONT_FILE_TYPE1C_H

#include "FontFileBase.h"
#include "Utils.h"

namespace NSFontConverter
{
    class StringExt;

    //------------------------------------------------------------------------

    struct Type1CIndex
    {
        int nPos;         // Position in file from file start
        int nCount;       // Number of entries
        int nOffsetSize;  // Offset size
        int nStartPos;    // Start position of index data - 1
        int nEndPos;      // Position of next byte after Type1CIndex
    };

    struct Type1CIndexVal
    {
        int nPos;         // Position in file from file start
        int nLen;         // Length in bytes
    };

    struct Type1CTopDict
    {
        int    nFirstOperator;

        int    nVersionSID;
        int    nNoticeSID;
        int    nCopyrightSID;
        int    nFullNameSID;
        int    nFamilyNameSID;
        int    nWeightSID;

        int    nIsFixedPitch;
        double dItalicAngle;
        double dUnderlinePosition;
        double dUnderlineThickness;

        int    nPaintType;
        int    nCharStringType;
        double arrdFontMatrix[6];
        bool   bHasFontMatrix;	// In CID fonts the font matrix may be in FD, not in top dict
        int    nUniqueID;
        double arrdFontBBox[4];
        double dStrokeWidth;
        int    nCharsetOffset;
        int    nEncodingOffset;
        int    nCharStringsOffset;
        int    nPrivateSize;
        int    nPrivateOffset;

        // CIDFont entries
        int    nRegistrySID;
        int    nOrderingSID;
        int    nSupplement;
        int    nFDArrayOffset;
        int    nFDSelectOffset;
    };



    #define type1CMaxBlueValues 14
    #define type1CMaxOtherBlues 10
    #define type1CMaxStemSnap   12

    struct Type1CPrivateDict
    {
        double arrdFontMatrix[6];
        bool   bHasFontMatrix;
        int    arrnBlueValues[type1CMaxBlueValues];
        int    nBlueValues;
        int    arrnOtherBlues[type1CMaxOtherBlues];
        int    nOtherBlues;
        int    arrnFamilyBlues[type1CMaxBlueValues];
        int    nFamilyBlues;
        int    arrnFamilyOtherBlues[type1CMaxOtherBlues];
        int    nFamilyOtherBlues;
        double dBlueScale;
        int    nBlueShift;
        int    nBlueFuzz;
        double dStdHW;
        bool   bHasStdHW;
        double dStdVW;
        bool   bHasStdVW;
        double arrdStemSnapH[type1CMaxStemSnap];
        int    nStemSnapH;
        double arrdStemSnapV[type1CMaxStemSnap];
        int    nStemSnapV;
        bool   bForceBold;
        bool   bHasForceBold;
        double dForceBoldThreshold;
        int    nLanguageGroup;
        double dExpansionFactor;
        int    nInitialRandomSeed;
        int    nSubrsOffset;
        double dDefaultWidthX;
        bool   bDefaultWidthXFP;
        double dNominalWidthX;
        bool   bNominalWidthXFP;
    };

    struct Type1COperator
    {
        bool bIsNumber;   // true -> number, false -> operator
        bool bIsFloat;    // true -> floating point number, false -> int
        union
        {
            double dNumber;
            int    nOperator;
        };
    };

    struct Type1CEexecBuf
    {
        FontFileOutputFunc pOutputFunc;
        void              *pOutputStream;
        bool               bASKII;          // ASCII encoding?
        unsigned short     unEncryptionKey; // eexec encryption key
        int                nLine;           // number of eexec characters remaining on current line
    };

    //------------------------------------------------------------------------
    // CFontFileType1C
    //------------------------------------------------------------------------

    class CFontFileType1C: public CFontFileBase
    {

    public:

        static CFontFileType1C *LoadFromBuffer(char *sBuffer, int nLen);

        static CFontFileType1C *LoadFromFile(const wchar_t *wsFileName);

        virtual ~CFontFileType1C();

        char *GetName();

        // Return encoding as array of 256 names (some may be
        // NULL). Used only for 8-bit fonts.
        char **GetEncoding();

        unsigned short *GetCIDToGIDMap(int *arrCIDs);

        // Convert to a Type 1 font, suitable for embedding in a PostScript
        // file.  This is only useful with 8-bit fonts.  If <newEncoding> is
        // not NULL, it will be used in place of the encoding in the Type 1C
        // font.  If <ascii> is true the eexec section will be hex-encoded,
        // otherwise it will be left as binary data.  If <psName> is non-NULL,
        // it will be used as the PostScript font name.
        void ToType1(char *sPSName, char **ppNewEncoding, bool bASKII, FontFileOutputFunc pOutputFunc, void *pOutputStream);

        // Convert to a Type 0 CIDFont, suitable for embedding in a
        // PostScript file.  <psName> will be used as the PostScript font
        // name.
        void ToCIDType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

        // Convert to a Type 0 (but non-CID) composite font, suitable for
        // embedding in a PostScript file.  <psName> will be used as the
        // PostScript font name.
        void ToType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

        // Convert to OpenType (CFF)
        void ToOpenTypeCFF(FontFileOutputFunc pOutputFunc, void *pOutputStream, FT_Face pFace);

    private:

        CFontFileType1C(char *sBuffer, int nLen, bool bFreeData);
        void EexecConvertGlyph(Type1CEexecBuf *pEexecBuf, char *sGlyphName, int nOffset, int nBytes, Type1CIndex *pSubrIndex, Type1CPrivateDict *pDict);
        void ConvertGlyph(int nOffset, int nBytes, StringExt *seCharBuffer, Type1CIndex *pSubrIndex, Type1CPrivateDict *pDict, bool bTop);
        void ConvertGlyphWidth(bool bUseOperation, StringExt *seCharBuffer, Type1CPrivateDict *pDict);
        void ConvertNum(double dValue, bool bIsFloat, StringExt *seCharBuffer);
        void EexecWrite(Type1CEexecBuf *pEexecBuf, char *sBuffer);
        void EexecWriteCharString(Type1CEexecBuf *pEexecBuf, unsigned char *sBuffer, int nLen);
        bool Parse();
        void ReadTopDict();
        void ReadFD(int nOffset, int nLength, Type1CPrivateDict *pDict);
        void ReadPrivateDict(int nOffset, int nLength, Type1CPrivateDict *pDict);
        void ReadFDSelect();
        void BuildEncoding();
        bool ReadCharset();
        int GetOperator(int nPos, bool bCharString, bool *pbSuccess);
        int GetDeltaIntArray(int *pArray, int nMaxLen);
        int GetDeltaDoubleArray(double *pArray, int nMaxLen);
        void GetIndex(int nPos, Type1CIndex *pIndex, bool *pbSuccess);
        void GetIndexVal(Type1CIndex *pIndex, int nIndex, Type1CIndexVal *pIndexVal, bool *bSuccess);
        char *GetString(int nSID, char *sBuffer, bool *pbSuccess);
        unsigned int ComputeTTTableChecksum(unsigned char *sData, int nLength)
    {
        unsigned int nWord = 0;

        unsigned int nChecksum = 0;
        for ( int nIndex = 0; nIndex + 3 < nLength; nIndex += 4 )
        {
            nWord = ( ( sData[ nIndex ] & 0xff) << 24) + ((sData[ nIndex + 1 ] & 0xff) << 16) + ((sData[ nIndex + 2 ] & 0xff) <<  8) + (sData[ nIndex + 3 ] & 0xff);
            nChecksum += nWord;
        }
        if ( nLength & 3 )
        {
            nWord = 0;
            int nTemp = nLength & ~3;
            switch ( nLength & 3 )
            {
            case 3:
                nWord |= (sData[nTemp + 2] & 0xff) <<  8;
            case 2:
                nWord |= (sData[nTemp + 1] & 0xff) << 16;
            case 1:
                nWord |= (sData[nTemp]     & 0xff) << 24;
                break;
            }
            nChecksum += nWord;
        }
        return nChecksum;
    }



    private:

        StringExt         *m_seName;
        char             **m_arrEncoding;

        Type1CIndex        m_oNameIndex;
        Type1CIndex        m_oTopDictIndex;
        Type1CIndex        m_oStringIndex;
        Type1CIndex        m_oGsubrIndex;
        Type1CIndex        m_oCharStringsIndex;

        Type1CTopDict      m_oTopDict;
        Type1CPrivateDict *m_pPrivateDicts;

        int                m_nGlyphsCount;
        int                m_nFDsCount;
        unsigned char     *m_pnFDSelect;
        unsigned short    *m_pnCharset;
        int                m_nGsubrBias;

        bool               m_bSuccessParsed;

        Type1COperator     m_arrOperators[49];
        int                m_nOperatorsCount;
        int                m_nHints;           // for current symbol
        bool               m_bFirstOperator;
        bool               m_bOpenPath;		   // true if there is an unclosed path
    };
}

#endif /* _ASC_FONTCONVERTER_FONT_FILE_TYPE1C_H */
