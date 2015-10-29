#pragma once

#include "../../../ASCPresentationEditor/OfficeDrawing/TextAttributesEx.h"
#include "SlidePersist.h"


using namespace NSPresentationEditor;


namespace NSZLib
{
    bool Decompress(const BYTE* pSrcBuffer, const ULONG& lSrcBufferLen,
								BYTE* pDstBuffer, ULONG& lDstBufferLen);
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
    struct POINT
    {
        long x;
        long y;
    };
    struct SIZE
    {
        long cx;
        long cy;
    };
    struct RECT
    {
        long left;
        long top;
        long right;
        long bottom;
    };

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
        DWORD   iType;              // Record type EMR_HEADER
        DWORD   nSize;              // Record size in bytes.  This may be greater
                                    // than the sizeof(ENHMETAHEADER).
        RECT   rclBounds;          // Inclusive-inclusive bounds in device units
        RECT   rclFrame;           // Inclusive-inclusive Picture Frame .01mm unit
        DWORD   dSignature;         // Signature.  Must be ENHMETA_SIGNATURE.
        DWORD   nVersion;           // Version number
        DWORD   nBytes;             // Size of the metafile in bytes
        DWORD   nRecords;           // Number of records in the metafile
        WORD    nHandles;           // Number of handles in the handle table
                                    // Handle index zero is reserved.
        WORD    sReserved;          // Reserved.  Must be zero.
        DWORD   nDescription;       // Number of chars in the unicode desc string
                                    // This is 0 if there is no description string
        DWORD   offDescription;     // Offset to the metafile description record.
                                    // This is 0 if there is no description string
        DWORD   nPalEntries;        // Number of entries in the metafile palette.
        SIZE   szlDevice;          // Size of the reference device in pels
        SIZE   szlMillimeters;     // Size of the reference device in millimeters
    } ENHMETAHEADER3;

    typedef struct
    {
        DWORD          Key;            // GDIP_WMF_PLACEABLEKEY
        short           Hmf;            // Metafile HANDLE number (always 0)
        PWMFRect16      BoundingBox;    // Coordinates in metafile units
        short           Inch;           // Number of metafile units per inch
        DWORD          Reserved;       // Reserved (always 0)
        short           Checksum;       // Checksum value for previous 10 WORDs
    }WmfPlaceableFileHeader;
}
 #endif

class CMetaHeader
{

public:
	DWORD cbSize;
	RECT rcBounds;
	POINT ptSize;
	DWORD cbSave;
	BYTE compression;
	BYTE filter;

public:
	CMetaHeader()
	{
	}
	void FromStream(POLE::Stream* pStream)
	{
		cbSize			= StreamUtils::ReadDWORD(pStream);
		
		rcBounds.left	= StreamUtils::ReadLONG(pStream);
		rcBounds.top	= StreamUtils::ReadLONG(pStream);
		rcBounds.right	= StreamUtils::ReadLONG(pStream);
		rcBounds.bottom = StreamUtils::ReadLONG(pStream);

		ptSize.x		= StreamUtils::ReadLONG(pStream);
		ptSize.y		= StreamUtils::ReadLONG(pStream);

		cbSave			= StreamUtils::ReadDWORD(pStream);

		compression		= StreamUtils::ReadBYTE(pStream);
		filter			= StreamUtils::ReadBYTE(pStream);
	}

	void ToEMFHeader(Gdiplus::ENHMETAHEADER3* pHeader)
	{
		if (NULL == pHeader)
			return;

		pHeader->iType				= 0x00000001;
		pHeader->nSize				= 88;

		pHeader->rclBounds.left		= rcBounds.left;
		pHeader->rclBounds.top		= rcBounds.top;
		pHeader->rclBounds.right	= rcBounds.right;
		pHeader->rclBounds.bottom	= rcBounds.bottom;

		// нужно перевести в мм
		pHeader->rclFrame.left		= rcBounds.left;
		pHeader->rclFrame.top		= rcBounds.top;
		pHeader->rclFrame.right		= rcBounds.right;
		pHeader->rclFrame.bottom	= rcBounds.bottom;

		pHeader->dSignature			= 0x464D4520;
		pHeader->nVersion			= 0x00010000;
		pHeader->nBytes				= cbSize;

		pHeader->nRecords			= 1;
		pHeader->nHandles			= 0;

		pHeader->sReserved			= 0;

		pHeader->nDescription		= 0;
		pHeader->offDescription		= 0;

		pHeader->nPalEntries		= 0;

		pHeader->szlDevice.cx		= 200;
		pHeader->szlDevice.cy		= 200;

		// нужно перевести в мм
		pHeader->szlMillimeters.cx	= 100;
		pHeader->szlMillimeters.cy	= 100;
	}

	void ToWMFHeader(Gdiplus::WmfPlaceableFileHeader* pHeader)
	{
		if (NULL == pHeader)
			return;

		pHeader->Key				= 0x9AC6CDD7;
		pHeader->Hmf				= 0;

		pHeader->BoundingBox.Left	= (short)rcBounds.left;
		pHeader->BoundingBox.Top	= (short)rcBounds.top;
		pHeader->BoundingBox.Right	= (short)rcBounds.right;
		pHeader->BoundingBox.Bottom = (short)rcBounds.bottom;

		pHeader->Inch				= 1440; // 1:1
		pHeader->Reserved			= 0;

		pHeader->Checksum			= 0;
		pHeader->Checksum			^= (pHeader->Key & 0x0000FFFFL);
		pHeader->Checksum			^= ((pHeader->Key & 0xFFFF0000L) >> 16);
		
		pHeader->Checksum			^= pHeader->Hmf; 
		
		pHeader->Checksum			^= pHeader->BoundingBox.Left;
		pHeader->Checksum			^= pHeader->BoundingBox.Top; 
		pHeader->Checksum			^= pHeader->BoundingBox.Right;
		pHeader->Checksum			^= pHeader->BoundingBox.Bottom; 
		
		pHeader->Checksum			^= pHeader->Inch;
		pHeader->Checksum			^= (pHeader->Reserved & 0x0000FFFFL);
		pHeader->Checksum			^= ((pHeader->Reserved & 0xFFFF0000L) >> 16);
	}
};

class CMetaFileBuffer
{
public:
    bool			m_bIsCompressed;

    bool			m_bIsValid;
	CString			m_sExtension;
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
            bool bRes	= NSZLib::Decompress(pCompress, (ULONG)lCompressSize, m_pMetaFile, lSize);
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

		oAtom.bPaletteIndex	= (0x01 == (oAtom.Index & 0x01));
		oAtom.bPaletteRGB	= (0x02 == (oAtom.Index & 0x02));
		oAtom.bSystemRGB	= (0x04 == (oAtom.Index & 0x04));
		oAtom.bSchemeIndex	= (0x08 == (oAtom.Index & 0x08));
		oAtom.bSysIndex		= (0x10 == (oAtom.Index & 0x10));
	}
	
	void Read(POLE::Stream* pStream, NSPresentationEditor::CTextSIRun& oRun, bool bIsIndentation = true);
	void Read(POLE::Stream* pStream, NSPresentationEditor::CTextRuler& oRun);
}

class CTextPFRun_ppt
{
public:
	NSPresentationEditor::CTextPFRun	m_oRun;
	LONG								m_lLevel;
	LONG								m_lCount;

	CTextPFRun_ppt() : m_oRun()
	{
		m_lLevel = -1;
		m_lCount = 0;
	}
	CTextPFRun_ppt(const CTextPFRun_ppt& oSrc)
	{
		*this = oSrc;
	}
	CTextPFRun_ppt& operator=(const CTextPFRun_ppt& oSrc)
	{
		m_oRun		= oSrc.m_oRun;
		m_lLevel	= oSrc.m_lLevel;
		m_lCount	= oSrc.m_lCount;
		return *this;
	}

	void LoadFromStream(POLE::Stream* pStream, bool bIsIndentation = true);
};

class CTextCFRun_ppt
{
public:
	NSPresentationEditor::CTextCFRun	m_oRun;
	LONG								m_lCount;

	CTextCFRun_ppt() : m_oRun()
	{
		m_lCount = 0;
	}
	CTextCFRun_ppt(const CTextCFRun_ppt& oSrc)
	{
		*this = oSrc;
	}
	CTextCFRun_ppt& operator=(const CTextCFRun_ppt& oSrc)
	{
		m_oRun		= oSrc.m_oRun;
		m_lCount	= oSrc.m_lCount;
		return *this;
	}

	void LoadFromStream(POLE::Stream* pStream, bool bIsIndentation = true);
};

namespace NSPresentationEditor
{
	void ConvertPPTTextToEditorStructure(std::vector<CTextPFRun_ppt>& oArrayPF, std::vector<CTextCFRun_ppt>& oArrayCF, 
		std::wstring& strText, NSPresentationEditor::CTextAttributesEx& oAttributes);
}
