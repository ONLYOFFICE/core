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

#include "../Drawing/TextAttributesEx.h"
#include "SlidePersist.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"

namespace CRYPT
{
class ECMADecryptor;
}
namespace PPT
{
struct SRatioAtom
{
    LONG Number;
    LONG Denom;

    void FromStream(POLE::Stream* pStream);
};

struct SScalingAtom
{
    SRatioAtom X;
    SRatioAtom Y;

    void FromStream(POLE::Stream* pStream);
};

struct SSlideLayoutAtom
{
    INT		m_nGeom;
    BYTE	m_pPlaceHolderID[8];
    _UINT64 m_hash;
};

struct SFileIdCluster
{
    UINT DrawingGroupID;
    UINT CurrentShapeID;

    void ReadFromStream(POLE::Stream* pStream);
};
#if !defined(_WIN32) && !defined (_WIN64)
#ifndef customTagPoint
#define customTagPoint
typedef struct tagPOINT
{
    _INT32  x;
    _INT32  y;
} POINT;
typedef struct tagRECT
{
    _INT32    left;
    _INT32    top;
    _INT32    right;
    _INT32    bottom;
} RECT;
typedef struct tagSIZE
{
    _INT32 cx;
    _INT32 cy;
}SIZE;
#endif
#endif

namespace Gdiplus
{
typedef struct
{
    short           Left;
    short           Top;
    short           Right;
    short           Bottom;
} PWMFRect16;

typedef struct
{
    _UINT32   iType;              // Record type EMR_HEADER
    _UINT32   nSize;              // Record size in bytes.  This may be greater
    // than the sizeof(ENHMETAHEADER).
    RECT   rclBounds;          // Inclusive-inclusive bounds in device units
    RECT   rclFrame;           // Inclusive-inclusive Picture Frame .01mm unit
    _UINT32   dSignature;         // Signature.  Must be ENHMETA_SIGNATURE.
    _UINT32   nVersion;           // Version number
    _UINT32   nBytes;             // Size of the metafile in bytes
    _UINT32   nRecords;           // Number of records in the metafile
    WORD    nHandles;           // Number of handles in the handle table
    // Handle index zero is reserved.
    WORD    sReserved;          // Reserved.  Must be zero.
    _UINT32   nDescription;       // Number of chars in the unicode desc string
    // This is 0 if there is no description string
    _UINT32   offDescription;     // Offset to the metafile description record.
    // This is 0 if there is no description string
    _UINT32   nPalEntries;        // Number of entries in the metafile palette.
    SIZE   szlDevice;          // Size of the reference device in pels
    SIZE   szlMillimeters;     // Size of the reference device in millimeters
} ENHMETAHEADER3;

typedef struct
{
    _UINT32          Key;            // GDIP_WMF_PLACEABLEKEY
    short           Hmf;            // Metafile HANDLE number (always 0)
    PWMFRect16      BoundingBox;    // Coordinates in metafile units
    short           Inch;           // Number of metafile units per inch
    _UINT32          Reserved;       // Reserved (always 0)
    short           Checksum;       // Checksum value for previous 10 WORDs
}WmfPlaceableFileHeader;
}

class CMetaHeader
{

public:
    _UINT32 cbSize;
    RECT rcBounds;
    POINT ptSize;
    _UINT32 cbSave;
    BYTE compression;
    BYTE filter;

public:
    CMetaHeader();
    void FromStream(POLE::Stream* pStream, CRYPT::ECMADecryptor *pDecryptor = NULL);

    void ToEMFHeader(Gdiplus::ENHMETAHEADER3* pHeader);
    void ToWMFHeader(Gdiplus::WmfPlaceableFileHeader* pHeader);
    void ToPICTHeader(BYTE *& pHeader, int & size);
};

class CMetaFileBuffer
{
public:
    bool			m_bIsCompressed;

    bool			m_bIsValid;
    std::wstring	m_sExtension;
private:
    BYTE* m_pMetaHeader;
    BYTE* m_pMetaFile;

    LONG m_lMetaHeaderSize;
    LONG m_lMetaFileSize;

public:
    CMetaFileBuffer();
    ~CMetaFileBuffer();

    void SetHeader(BYTE* pHeader, LONG lSize);

    void SetData(BYTE* pCompress, LONG lCompressSize, LONG lUncompressSize, bool bIsCompressed);

    void ToFile(NSFile::CFileBinary* pFile);
};

namespace NSStreamReader
{
void Read(POLE::Stream* pStream, ODRAW::SPointAtom& oAtom);

void Read(POLE::Stream* pStream, ODRAW::SColorAtom& oAtom);

void Read(POLE::Stream* pStream, PPT::CTextSIRun& oRun, bool bIsIndentation = true);
void Read(POLE::Stream* pStream, PPT::CTextRuler& oRun);
}

class CTextPFRunRecord
{
public:
    PPT::CTextPFRun	m_oRun;
    LONG					m_lLevel;
    LONG					m_lCount;

    CTextPFRunRecord();
    CTextPFRunRecord(const CTextPFRunRecord& oSrc);
    CTextPFRunRecord& operator=(const CTextPFRunRecord& oSrc);

    void LoadFromStream(POLE::Stream* pStream, bool bIsIndentation = true);
};

class CTextCFRunRecord
{
public:
    PPT::CTextCFRun	m_oRun;
    LONG					m_lCount;

    CTextCFRunRecord();
    CTextCFRunRecord(const CTextCFRunRecord& oSrc);
    CTextCFRunRecord& operator=(const CTextCFRunRecord& oSrc);

    void LoadFromStream(POLE::Stream* pStream, bool bIsIndentation = true);
};

void ConvertPPTTextToEditorStructure(std::vector<CTextPFRunRecord>& oArrayPF, std::vector<CTextCFRunRecord>& oArrayCF,
                                     std::wstring& strText, PPT::CTextAttributesEx& oAttributes);
}
