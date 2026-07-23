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
#ifndef _ASC_FONTCONVERTER_FONT_FILE_TRUETYPE_H
#define _ASC_FONTCONVERTER_FONT_FILE_TRUETYPE_H

#include "FontFileBase.h"

namespace NSFontConverter
{
    class StringExt;
    class CHash;

    //------------------------------------------------------------------------

    #define ttcfTag 0x74746366

    //------------------------------------------------------------------------

    struct TrueTypeTable
    {
        unsigned int unTag;
        unsigned int unChecksum;
        int          nOffset;
        int          nOrigOffset;
        int          nLen;
    };

    struct TrueTypeCmap
    {
        int nPlatform;
        int nEncoding;
        int nOffset;
        int nLen;
        int nFormat;
    };

    struct TrueTypeLoca
    {
        int nIndex;
        int nOrigOffset;
        int nNewOffset;
        int nLen;
    };

    #define  cffTag 0x43464620
    #define maxpTag 0x6d617870
    #define cmapTag 0x636d6170
    #define glyfTag 0x676c7966
    #define headTag 0x68656164
    #define hheaTag 0x68686561
    #define hmtxTag 0x686d7478
    #define locaTag 0x6c6f6361
    #define nameTag 0x6e616d65
    #define  os2Tag 0x4f532f32
    #define postTag 0x706f7374

    #define  cvtTag 0x63767420
    #define fpgmTag 0x6670676d
    #define prepTag 0x70726570

    static int CompareTrueTypeLocaOffset(const void *pL1, const void *pL2)
    {
        TrueTypeLoca *pLoca1 = (TrueTypeLoca *)pL1;
        TrueTypeLoca *pLoca2 = (TrueTypeLoca *)pL2;

        if ( pLoca1->nOrigOffset == pLoca2->nOrigOffset )
            return pLoca1->nIndex - pLoca2->nIndex;

        return pLoca1->nOrigOffset - pLoca2->nOrigOffset;
    }

    static int CompareTrueTypeLocaIndex(const void *pL1, const void *pL2)
    {
        TrueTypeLoca *pLoca1 = (TrueTypeLoca *)pL1;
        TrueTypeLoca *pLoca2 = (TrueTypeLoca *)pL2;

        return pLoca1->nIndex - pLoca2->nIndex;
    }

    static int CompareTrueTypeTableTag(const void *pTab1, const void *pTab2)
    {
        TrueTypeTable *pTable1 = (TrueTypeTable *)pTab1;
        TrueTypeTable *pTable2 = (TrueTypeTable *)pTab2;

        return (int)pTable1->unTag - (int)pTable2->unTag;
    }

    //------------------------------------------------------------------------

    struct T42Table
    {
        char *sTag;		 // 4-byte tag name
        bool  bRequired; // Required by TrueType specification?
    };

    // TrueType tables to be embedded in Type 42 fonts.
    // NB: the table names must be in alphabetical order here.
    #define nT42Tables 11
    static T42Table t42Tables[nT42Tables] =
    {
        { "cvt ", true  },
        { "fpgm", true  },
        { "glyf", true  },
        { "head", true  },
        { "hhea", true  },
        { "hmtx", true  },
        { "loca", true  },
        { "maxp", true  },
        { "prep", true  },
        { "vhea", false },
        { "vmtx", false }
    };
    #define t42HeadTable  3
    #define t42LocaTable  6
    #define t42GlyfTable  2
    #define t42VheaTable  9
    #define t42VmtxTable 10

    //------------------------------------------------------------------------
    // CFontFileTrueType
    //------------------------------------------------------------------------

    class CFontFileTrueType: public CFontFileBase
    {
    public:

        // Create TTF object from buffer.
        static CFontFileTrueType *LoadFromBuffer(char *sBuffer, int lenA);

        // Create TTF object from file.
        static CFontFileTrueType *LoadFromFile(const wchar_t *wsFileName);

        virtual ~CFontFileTrueType();

        // TRUE if this OpenType font contains CFF format data.
        // FALSE if this is a TrueType font (or OpenType font with TrueType format data).
        bool IsOpenTypeCFF()
        {
            return m_bOpenTypeCFF;
        }

        int  GetCmapsCount();

        int  GetCmapPlatform(int nIndex);

        int  GetCmapEncoding(int nIndex);

        int  FindCmap(int nPlatform, int nEncoding);

        // Returns GID corresponding to character <nChar> in CMap <nIndex>.
        unsigned short MapCodeToGID(int nCMapIndex, int nChar);

        // Returns GID corresponding to <sName> in post table. Returns 0
        // if glyph with this name was not found, or post table doesn't exist.
        int MapNameToGID(char *sName);

        // Returns CIDs to GIDs map, and returns the number of CID elements
        // in *<pnCIDs>. Only for CID fonts (OpenType CFF).
        unsigned short *GetCIDToGIDMap(int *pnCIDs);

        // Font embedding licensing restrictions (according to
        // TrueType specification):

        // * 4: OS/2 table not found or invalid
        // * 3: installable embedding allowed
        // * 2: editable embedding allowed
        // * 1: preview and print embedding allowed
        // * 0: restricted by license
        int GetEmbeddingRestrictions();

        // Convert to a Type 42 font, suitable for embedding in a PostScript
        // file.  <psName> will be used as the PostScript font name (so we
        // don't need to depend on the 'name' table in the font).  The
        // <encoding> array specifies the mapping from char codes to names.
        // If <encoding> is NULL, the encoding is unknown or undefined.  The
        // <codeToGID> array specifies the mapping from char codes to GIDs.
        // (Not useful for OpenType CFF fonts.)
        void ToType42(char *sPSName, char **ppEncoding, unsigned short *pCodeToGID, FontFileOutputFunc pOutputFunc, void *pOutputStream );

        // Convert to a Type 1 font, suitable for embedding in a PostScript
        // file.  This is only useful with 8-bit fonts.  If <newEncoding> is
        // not NULL, it will be used in place of the encoding in the Type 1C
        // font.  If <ascii> is true the eexec section will be hex-encoded,
        // otherwise it will be left as binary data.  If <psName> is
        // non-NULL, it will be used as the PostScript font name.  (Only
        // useful for OpenType CFF fonts.)
        void ToType1(char *sPSName, char **ppNewEncoding, bool bASKII, FontFileOutputFunc pOutputFunc, void *pOutputStream );

        // Convert to a Type 2 CIDFont, suitable for embedding in a
        // PostScript file.  <psName> will be used as the PostScript font
        // name (so we don't need to depend on the 'name' table in the
        // font).  The <cidMap> array maps CIDs to GIDs; it has <nCIDs>
        // entries.  (Not useful for OpenType CFF fonts.)
        void ToCIDType2(char *sPSName, unsigned short *pCIDMap, int nCIDCount, bool bNeedVerticalMetrics, FontFileOutputFunc pOutputFunc, void *pOutputStream);

        // Convert to a Type 0 CIDFont, suitable for embedding in a
        // PostScript file.  <psName> will be used as the PostScript font
        // name.  (Only useful for OpenType CFF fonts.)
        void ToCIDType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

        // Convert to a Type 0 (but non-CID) composite font, suitable for
        // embedding in a PostScript file.  <psName> will be used as the
        // PostScript font name (so we don't need to depend on the 'name'
        // table in the font).  The <cidMap> array maps CIDs to GIDs; it has
        // <nCIDs> entries.  (Not useful for OpenType CFF fonts.)
        void ToType0(char *sPSName, unsigned short *pCIDMap, int nCIDCount, bool bNeedVerticalMetrics, FontFileOutputFunc pOutputFunc, void *pOutputStream);

        // Convert to a Type 0 (but non-CID) composite font, suitable for
        // embedding in a PostScript file.  <psName> will be used as the
        // PostScript font name.  (Only useful for OpenType CFF fonts.)
        void ToType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream);

        // Write TrueTypeFont file, filling in missing tables and correcting
        // various errors. If <sName> parameter is specified, rewrite the 'name'
        // table in the font. If <pCodeToGID> parameter is specified, rewrite
        // the 'cmap' table in the font.
        void WriteTTF(FontFileOutputFunc pOutputFunc, void *pOutputStream, const char *sName = NULL, unsigned short *pCodeToGID = NULL, unsigned char *pUseGlyfs = NULL, long lGlyfsCount = 0) ;

    private:

        CFontFileTrueType(char *sFileName, int nLen, bool bFreeFileData);

        void ConvertEncoding   (char **ppEncoding, FontFileOutputFunc pOutputFunc, void *pOutputStream);
        void ConvertCharStrings(char **ppEncoding, unsigned short *pnCodeToGID, FontFileOutputFunc pOutputFunc, void *pOutputStream);
        void ConvertSfnts      (FontFileOutputFunc pOutputFunc, void *pOutputStream, StringExt *seName, bool bNeedVerticalMetrics);
        void DumpString(unsigned char *sString, int nLength, FontFileOutputFunc pOutputFunc, void *pOutputStream);
        unsigned int ComputeTableChecksum(unsigned char *sData, int nLength);
        void Parse();
        void ReadPostTable();
        int  SeekTable(char *sTag);

    private:

        TrueTypeTable   *m_pTables;
        int              m_nTablesCount;
        TrueTypeCmap    *m_pCMaps;
        int              m_nCMapsCount;
        int              m_nGlyphs;
        int              m_nLocaFormat;
        int              m_arrBBox[4];
        CHash           *m_pNameToGID;
        bool             m_bOpenTypeCFF;

        bool             m_bSuccess;
    };
}

#endif /* _ASC_FONTCONVERTER_FONT_FILE_TRUETYPE_H */
