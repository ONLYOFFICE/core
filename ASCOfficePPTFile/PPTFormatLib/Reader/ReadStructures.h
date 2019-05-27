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

#include "../../../ASCOfficePPTXFile/Editor/Drawing/TextAttributesEx.h"
#include "SlidePersist.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"


using namespace PPT_FORMAT;

namespace CRYPT
{
	class ECMADecryptor;
}
/**************************************************************
теперь все структуры...
которые участвуют в записях
**************************************************************/
struct SRatioAtom 
{ 
	LONG Number; 
	LONG Denom; 

	void FromStream(POLE::Stream* pStream)
	{
		Number	= StreamUtils::ReadLONG(pStream);
		Denom	= StreamUtils::ReadLONG(pStream);
	}
}; 
 
struct SScalingAtom 
{ 
	SRatioAtom X; 
	SRatioAtom Y; 

	void FromStream(POLE::Stream* pStream)
	{
		X.FromStream(pStream);
		Y.FromStream(pStream);
	}
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

	void ReadFromStream(POLE::Stream* pStream)
	{
		DrawingGroupID = (UINT)StreamUtils::ReadDWORD(pStream);
		CurrentShapeID = (UINT)StreamUtils::ReadDWORD(pStream);
	}
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
	CMetaHeader()
	{
	}
	void FromStream(POLE::Stream* pStream, CRYPT::ECMADecryptor *pDecryptor = NULL);

	void ToEMFHeader	(Gdiplus::ENHMETAHEADER3* pHeader);
	void ToWMFHeader	(Gdiplus::WmfPlaceableFileHeader* pHeader);	
	void ToPICTHeader	(BYTE *& pHeader, int & size);
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
	CMetaFileBuffer()
	{
		m_bIsCompressed		= false;
        m_bIsValid			= false;

		m_pMetaHeader		= NULL;
		m_pMetaFile			= NULL;

		m_lMetaHeaderSize	= 0;
		m_lMetaFileSize		= 0;
	}
	~CMetaFileBuffer()
	{
		RELEASEARRAYOBJECTS(m_pMetaHeader);
		RELEASEARRAYOBJECTS(m_pMetaFile);

		if (m_bIsCompressed)
			RELEASEARRAYOBJECTS(m_pMetaFile);
		m_bIsCompressed = false;
	}

	void SetHeader(BYTE* pHeader, LONG lSize)
	{
		m_pMetaHeader		= pHeader;
		m_lMetaHeaderSize	= lSize;
	}

    void SetData(BYTE* pCompress, LONG lCompressSize, LONG lUncompressSize, bool bIsCompressed)
	{
		m_bIsCompressed = bIsCompressed;
		if (!m_bIsCompressed)
		{
			m_pMetaFile		= pCompress;
			m_lMetaFileSize = lUncompressSize;
		}
		else
		{
			ULONG lSize = lUncompressSize;
			m_pMetaFile = new BYTE[lUncompressSize];
            bool bRes	= NSZip::Decompress(pCompress, (ULONG)lCompressSize, m_pMetaFile, lSize);
			if (bRes)
			{
				m_lMetaFileSize = (LONG)lSize;
				m_bIsCompressed = true;
			}
			else
			{
				RELEASEARRAYOBJECTS(m_pMetaFile);
			
				m_pMetaFile		= pCompress;
				m_lMetaFileSize = lUncompressSize;
				m_bIsCompressed = false;
			}
		}
	}

	void ToFile(CFile* pFile)
	{
		if (NULL != m_pMetaHeader)
		{
			pFile->WriteFile((void*)m_pMetaHeader, m_lMetaHeaderSize);
		}
		if (NULL != m_pMetaFile)
		{
			pFile->WriteFile((void*)m_pMetaFile, m_lMetaFileSize);
		}
	}
};

namespace NSStreamReader
{
	static inline void Read(POLE::Stream* pStream, SPointAtom& oAtom)
	{
		oAtom.X = StreamUtils::ReadLONG(pStream);
		oAtom.Y = StreamUtils::ReadLONG(pStream);
	}

	static inline void Read(POLE::Stream* pStream, SColorAtom& oAtom)
	{
		oAtom.R				= StreamUtils::ReadBYTE(pStream);
		oAtom.G				= StreamUtils::ReadBYTE(pStream);
		oAtom.B				= StreamUtils::ReadBYTE(pStream);
		oAtom.Index			= StreamUtils::ReadBYTE(pStream);

		oAtom.bPaletteIndex	= oAtom.bPaletteRGB = oAtom.bSystemRGB	= oAtom.bSysIndex = oAtom.bSchemeIndex = false;

		if (oAtom.Index != 0xFF)
		{
			oAtom.bPaletteRGB	= (oAtom.Index == 0xFE);
			oAtom.bSchemeIndex	= (oAtom.Index != 0xFE);
		}
	}
	
	void Read(POLE::Stream* pStream, PPT_FORMAT::CTextSIRun& oRun, bool bIsIndentation = true);
	void Read(POLE::Stream* pStream, PPT_FORMAT::CTextRuler& oRun);
}

class CTextPFRunRecord
{
public:
	PPT_FORMAT::CTextPFRun	m_oRun;
	LONG					m_lLevel;
	LONG					m_lCount;

	CTextPFRunRecord() : m_oRun()
	{
		m_lLevel = -1;
		m_lCount = 0;
	}
	CTextPFRunRecord(const CTextPFRunRecord& oSrc)
	{
		*this = oSrc;
	}
	CTextPFRunRecord& operator=(const CTextPFRunRecord& oSrc)
	{
		m_oRun		= oSrc.m_oRun;
		m_lLevel	= oSrc.m_lLevel;
		m_lCount	= oSrc.m_lCount;
		return *this;
	}

	void LoadFromStream(POLE::Stream* pStream, bool bIsIndentation = true);
};

class CTextCFRunRecord
{
public:
	PPT_FORMAT::CTextCFRun	m_oRun;
	LONG					m_lCount;

	CTextCFRunRecord() : m_oRun()
	{
		m_lCount = 0;
	}
	CTextCFRunRecord(const CTextCFRunRecord& oSrc)
	{
		*this = oSrc;
	}
	CTextCFRunRecord& operator=(const CTextCFRunRecord& oSrc)
	{
		m_oRun		= oSrc.m_oRun;
		m_lCount	= oSrc.m_lCount;
		return *this;
	}

	void LoadFromStream(POLE::Stream* pStream, bool bIsIndentation = true);
};

namespace PPT_FORMAT
{
	void ConvertPPTTextToEditorStructure(std::vector<CTextPFRunRecord>& oArrayPF, std::vector<CTextCFRunRecord>& oArrayCF, 
		std::wstring& strText, PPT_FORMAT::CTextAttributesEx& oAttributes);
}
