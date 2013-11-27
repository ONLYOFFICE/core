#pragma once
#include "..\..\ASCPresentationEditor\OfficeDrawing\TextAttributesEx.h"
#include "SlidePersist.h"

#include "gdiplus.h"
#include "CommonZLib.h"

using namespace Gdiplus;
using namespace NSPresentationEditor;

/**************************************************************
теперь все структуры...
которые участвуют в записях
**************************************************************/
struct SRatioAtom 
{ 
	LONG Number; 
	LONG Denom; 

	CString ToString()
	{
		CString str = _T("");
		str.Format(_T("<Ratio info='(%d,%d)' />"), Number, Denom);
		return str;
	}

	void FromStream(IStream* pStream)
	{
		Number	= StreamUtils::ReadLONG(pStream);
		Denom	= StreamUtils::ReadLONG(pStream);
	}
}; 
 
struct SScalingAtom 
{ 
	SRatioAtom X; 
	SRatioAtom Y; 

	CString ToString()
	{
		return X.ToString() + Y.ToString();
	}

	void FromStream(IStream* pStream)
	{
		X.FromStream(pStream);
		Y.FromStream(pStream);
	}
};

struct SSlideLayoutAtom
{
	INT m_nGeom;
	BYTE m_pPlaceHolderID[8];

	CString ToString()
	{
		CString str = _T("");
		str.Format(_T("<SSlideLayoutAtom Geom='%d' PlaceHolderIDs='(%d,%d,%d,%d,%d,%d,%d,%d)' />"),
			m_nGeom, m_pPlaceHolderID[0], m_pPlaceHolderID[1], m_pPlaceHolderID[2], m_pPlaceHolderID[3], 
			m_pPlaceHolderID[4], m_pPlaceHolderID[5], m_pPlaceHolderID[6], m_pPlaceHolderID[7]);
		return str;
	}
};

struct SFileIdCluster
{
	UINT DrawingGroupID;
	UINT CurrentShapeID;

	void ReadFromStream(IStream* pStream)
	{
		DrawingGroupID = (UINT)StreamUtils::ReadDWORD(pStream);
		CurrentShapeID = (UINT)StreamUtils::ReadDWORD(pStream);
	}
	
	CString ToString()
	{
		CString str = _T("");
		str.Format(_T("<FileIdCluster drawinggroupID='%d' currentshapeID='%d' />"), DrawingGroupID, CurrentShapeID);
		return str;
	}
};

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
	void FromStream(IStream* pStream)
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

	void ToEMFHeader(ENHMETAHEADER3* pHeader)
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

	void ToWMFHeader(WmfPlaceableFileHeader* pHeader)
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

	static void DecompressDeflate(BYTE* pSrc, LONG lSrcSize, BYTE** ppDst, LONG& lDstSize)
	{
		CComPtr<IEncodingFilterFactory> pEFF;
		HRESULT hr = pEFF.CoCreateInstance(CLSID_StdEncodingFilterFac);
		CComPtr<IDataFilter> pDF;
		//accquire suitable filter
		if (!pEFF || FAILED(pEFF->GetDefaultFilter( L"deflate", L"text", &pDF))) 
			return;

		//temporary out buffers
		BYTE* outBuff = new BYTE[200000];
		DWORD dwOutBuffSize = sizeof(outBuff);

		long dwRead = 0, dwWritten = 0;
		//encode chunk of data
		//hr = pDF->DoDecode(0, lSrcSize, pSrc, lDstSize, *ppDst, lSrcSize, &dwRead, &dwWritten, 0);
		hr = pDF->DoDecode(0, lSrcSize, pSrc, dwOutBuffSize, outBuff, lSrcSize, &dwRead, &dwWritten, 0);
	}
};

class CMetaFileBuffer
{
public:
	BOOL m_bIsValid;

private:
	BYTE* m_pMetaHeader;
	BYTE* m_pMetaFile;

	LONG m_lMetaHeaderSize;
	LONG m_lMetaFileSize;

public:
	CMetaFileBuffer()
	{
		m_bIsValid			= FALSE;

		m_pMetaHeader		= NULL;
		m_pMetaFile			= NULL;

		m_lMetaHeaderSize	= 0;
		m_lMetaFileSize		= 0;
	}
	~CMetaFileBuffer()
	{
		RELEASEARRAYOBJECTS(m_pMetaHeader);
		RELEASEARRAYOBJECTS(m_pMetaFile);
	}

	void SetHeader(BYTE* pHeader, LONG lSize)
	{
		m_pMetaHeader		= pHeader;
		m_lMetaHeaderSize	= lSize;
	}

	void SetData(BYTE* pCompress, LONG lCompressSize, LONG lUncompressSize, BOOL bIsCompressed)
	{
		if (!bIsCompressed)
		{
			m_pMetaFile		= pCompress;
			m_lMetaFileSize = lUncompressSize;
		}
		else
		{
			ULONG lSize = lUncompressSize;
			m_pMetaFile = new BYTE[lUncompressSize];
			BOOL bRes	= NSZLib::Decompress(pCompress, (ULONG)lCompressSize, m_pMetaFile, lSize);
			if (bRes)
			{
				m_lMetaFileSize = (LONG)lSize;
			}
			else
			{
				RELEASEARRAYOBJECTS(m_pMetaFile);
				m_lMetaFileSize = 0;
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
	static inline void Read(IStream* pStream, SPointAtom& oAtom)
	{
		oAtom.X = StreamUtils::ReadLONG(pStream);
		oAtom.Y = StreamUtils::ReadLONG(pStream);
	}

	static inline void Read(IStream* pStream, SColorAtom& oAtom)
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
	
	static inline void Read(IStream* pStream, NSPresentationEditor::CTextSIRun& oRun, bool bIsIndentation = true)
	{
		if (bIsIndentation)
		{
			oRun.lCount = StreamUtils::ReadDWORD(pStream);
		}
		
		DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
		BYTE flag1 = (BYTE)(dwFlags);
		BYTE flag2 = (BYTE)(dwFlags >> 8);

		oRun.bSpell					= (0x01 == (0x01 & flag1));
		oRun.bLang					= (0x02 == (0x02 & flag1));
		oRun.bAltLang				= (0x04 == (0x04 & flag1));
		// unused
		// unused		
		oRun.bPp10ext				= (0x20 == (0x20 & flag1));
		oRun.bBidi					= (0x40 == (0x40 & flag1));
		// unused
		// reserved
		oRun.bSmartTag				= (0x02 == (0x02 & flag2));

		if (oRun.bSpell)
		{
			oRun.Spell = StreamUtils::ReadWORD(pStream);
		}
		if (oRun.bLang)
		{
			oRun.Lang = StreamUtils::ReadWORD(pStream);
		}
		if (oRun.bAltLang)
		{
			oRun.AltLang = StreamUtils::ReadWORD(pStream);
		}
		if (oRun.bBidi)
		{
			oRun.Bidi = StreamUtils::ReadWORD(pStream);
		}
		if (oRun.bPp10ext)
		{
			DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
			BYTE flag1 = (BYTE)(dwFlags);
			BYTE flag2 = (BYTE)(dwFlags >> 8);
			BYTE flag3 = (BYTE)(dwFlags >> 16);
			BYTE flag4 = (BYTE)(dwFlags >> 24);

			oRun.pp10runid = (0x0F & flag1);
			oRun.bGramma = (0x80 == (0x80 & flag4));
		}

		/*if (bSmartTag)
		{
			DWORD tabStopsCount = StreamUtils::ReadDWORD(pStream);
			arSmartTags.RemoveAll();

			for (int i = 0; i < (int)tabStopsCount; ++i)
			{
				arSmartTags.Add(StreamUtils::ReadDWORD(pStream));
			}
		}*/
	}

	static inline void Read(IStream* pStream, NSPresentationEditor::CTextRuler& oRun)
	{
		DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
		BYTE flag1 = (BYTE)(dwFlags);
		BYTE flag2 = (BYTE)(dwFlags >> 8);
		BYTE flag3 = (BYTE)(dwFlags >> 16);
		BYTE flag4 = (BYTE)(dwFlags >> 24);

		bool bDefaultTabSize_				= (0x01 == (0x01 & flag1));
		bool bCLevels_						= (0x02 == (0x02 & flag1));
		bool bTabStops_						= (0x04 == (0x04 & flag1));
		
		bool bLeftMargin1_					= (0x08 == (0x08 & flag1));
		bool bLeftMargin2_					= (0x10 == (0x10 & flag1));
		bool bLeftMargin3_					= (0x20 == (0x20 & flag1));
		bool bLeftMargin4_					= (0x40 == (0x40 & flag1));
		bool bLeftMargin5_					= (0x80 == (0x80 & flag1));

		bool bIndent1_						= (0x01 == (0x01 & flag2));
		bool bIndent2_						= (0x02 == (0x02 & flag2));
		bool bIndent3_						= (0x04 == (0x04 & flag2));
		bool bIndent4_						= (0x08 == (0x08 & flag2));
		bool bIndent5_						= (0x10 == (0x10 & flag2));

		if (bCLevels_)
			oRun.CLevels = StreamUtils::ReadSHORT(pStream);
		if (bDefaultTabSize_)
			oRun.DefaultTabSize = StreamUtils::ReadSHORT(pStream);
		
		if (bTabStops_)
        {
            WORD tabStopsCount = StreamUtils::ReadWORD(pStream);
			oRun.tabsStops.RemoveAll();

            for (int i = 0; i < (int)tabStopsCount; ++i)
            {
				oRun.tabsStops.Add(StreamUtils::ReadDWORD(pStream));
            }
        }

		if (bLeftMargin1_)
			oRun.LeftMargin1 = StreamUtils::ReadSHORT(pStream);
		if (bIndent1_)
			oRun.Indent1 = StreamUtils::ReadSHORT(pStream);

		if (bLeftMargin2_)
			oRun.LeftMargin2 = StreamUtils::ReadSHORT(pStream);
		if (bIndent2_)
			oRun.Indent2 = StreamUtils::ReadSHORT(pStream);

		if (bLeftMargin3_)
			oRun.LeftMargin3 = StreamUtils::ReadSHORT(pStream);
		if (bIndent3_)
			oRun.Indent3 = StreamUtils::ReadSHORT(pStream);

		if (bLeftMargin4_)
			oRun.LeftMargin4 = StreamUtils::ReadSHORT(pStream);
		if (bIndent4_)
			oRun.Indent4 = StreamUtils::ReadSHORT(pStream);

		if (bLeftMargin5_)
			oRun.LeftMargin5 = StreamUtils::ReadSHORT(pStream);
		if (bIndent5_)
			oRun.Indent5 = StreamUtils::ReadSHORT(pStream);
	}
}

class CTextPFRun_ppt
{
public:
	NSPresentationEditor::CTextPFRun	m_oRun;
	LONG								m_lLevel;
	LONG								m_lCount;

public:
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

public:
	void LoadFromStream(IStream* pStream, bool bIsIndentation = true)
	{
		if (bIsIndentation)
		{
			m_lCount	= StreamUtils::ReadLONG(pStream);
			m_lLevel	= (LONG)StreamUtils::ReadWORD(pStream);

			if (m_lLevel > 0x0004)
				m_lLevel = 0x0004;
		}

		DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
		BYTE flag1 = (BYTE)(dwFlags);
		BYTE flag2 = (BYTE)(dwFlags >> 8);
		BYTE flag3 = (BYTE)(dwFlags >> 16);
		BYTE flag4 = (BYTE)(dwFlags >> 24);

		bool hasBullet_				= (0x01 == (0x01 & flag1));
		bool bulletHasFont_			= (0x02 == (0x02 & flag1));
		bool bulletHasColor_		= (0x04 == (0x04 & flag1));
		bool bulletHasSize_			= (0x08 == (0x08 & flag1));

		bool bulletFontRef_			= (0x10 == (0x10 & flag1));
		bool bulletColor_			= (0x20 == (0x20 & flag1));
		bool bulletSize_			= (0x40 == (0x40 & flag1));
		bool bulletChar_			= (0x80 == (0x80 & flag1));

		bool leftMargin_			= (0x01 == (0x01 & flag2));
		// reserved
		bool indent_				= (0x04 == (0x04 & flag2));
		bool textAlignment_			= (0x08 == (0x08 & flag2));
		bool lineSpacing_			= (0x10 == (0x10 & flag2));
		bool spaceBefore_			= (0x20 == (0x20 & flag2));
		bool spaceAfter_			= (0x40 == (0x40 & flag2));
		bool defaultTabSize_		= (0x80 == (0x80 & flag2));

		bool fontAlign_				= (0x01 == (0x01 & flag3));
		bool charWrap_				= (0x02 == (0x02 & flag3));
		bool wordWrap_				= (0x04 == (0x04 & flag3));
		bool overflow_				= (0x08 == (0x08 & flag3));		
		bool tabStops_				= (0x10 == (0x10 & flag3));
		bool textDirection_			= (0x20 == (0x20 & flag3));
		//reserved
		bool bulletBlip_			= (0x80 == (0x80 & flag3));

		bool bulletScheme_			= (0x01 == (0x01 & flag4));
		bool bulletHasScheme_		= (0x02 == (0x02 & flag4));

		if (hasBullet_ || bulletHasFont_ || bulletHasColor_ || bulletHasSize_)
		{
			WORD bulletFlag		= StreamUtils::ReadWORD(pStream);
			if (bulletFlag & 0x0F)
				m_oRun.hasBullet	= (BOOL)(0x01 == (bulletFlag & 0x01));
			else
				m_oRun.hasBullet	= (BOOL)FALSE;
		}

		if (bulletChar_)
			m_oRun.bulletChar		= (WCHAR)StreamUtils::ReadWORD(pStream);
		if (bulletFontRef_)
			m_oRun.bulletFontRef	= StreamUtils::ReadWORD(pStream);
		if (bulletSize_)
			m_oRun.bulletSize		= StreamUtils::ReadWORD(pStream);
		if (bulletColor_)
		{
			SColorAtom oColorAtom;
			NSStreamReader::Read(pStream, oColorAtom);

			NSPresentationEditor::CColor oColor;
			
			oColor.R = oColorAtom.R;
			oColor.G = oColorAtom.G;
			oColor.B = oColorAtom.B;
			oColor.A = 255;
			oColor.m_lSchemeIndex = -1;

			if (oColorAtom.Index < 10)
			{
				oColor.m_lSchemeIndex = oColorAtom.Index;
				NSPresentationEditor::CorrectColorPPT(oColor.m_lSchemeIndex);
			}

			m_oRun.bulletColor = oColor;
		}

		if (TRUE) // заглушка, чтобы буллеты были в цвет текста
			m_oRun.bulletColor.reset();

		if (textAlignment_)
			m_oRun.textAlignment		= StreamUtils::ReadWORD(pStream);

		if (lineSpacing_)
			m_oRun.lineSpacing			= StreamUtils::ReadSHORT(pStream);

		if (spaceBefore_)
			m_oRun.spaceBefore			= (LONG)StreamUtils::ReadSHORT(pStream);

		if (spaceAfter_)
			m_oRun.spaceAfter			= (LONG)StreamUtils::ReadSHORT(pStream);

		if (leftMargin_)
			m_oRun.leftMargin			= (LONG)StreamUtils::ReadSHORT(pStream);

		if (indent_)
			m_oRun.indent				= (LONG)StreamUtils::ReadSHORT(pStream);

		if (defaultTabSize_)
			m_oRun.defaultTabSize		= (LONG)StreamUtils::ReadWORD(pStream);

		if (tabStops_)
		{
			WORD tabStopsCount = StreamUtils::ReadWORD(pStream);
			m_oRun.tabStops.RemoveAll();

			if (tabStopsCount > 10)
				tabStopsCount = 10;

			for (int i = 0; i < (int)tabStopsCount; ++i)
			{
				m_oRun.tabStops.Add(StreamUtils::ReadDWORD(pStream));
			}

			if (0 < m_oRun.tabStops.GetCount())
				m_oRun.defaultTabSize = m_oRun.tabStops[0];
		}

		if (fontAlign_)
			m_oRun.fontAlign	= StreamUtils::ReadWORD(pStream);
		
		if (charWrap_ || wordWrap_ || overflow_)
		{
			m_oRun.wrapFlags	= StreamUtils::ReadWORD(pStream);
		}

		if (textDirection_)
			m_oRun.textDirection = StreamUtils::ReadWORD(pStream);
	}
};

class CTextCFRun_ppt
{
public:
	NSPresentationEditor::CTextCFRun	m_oRun;
	LONG								m_lCount;

public:
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

public:
	void LoadFromStream(IStream* pStream, bool bIsIndentation = true)
	{
		if (bIsIndentation)
		{
			m_lCount	= StreamUtils::ReadLONG(pStream);
		}

		DWORD dwFlags = StreamUtils::ReadDWORD(pStream);
		BYTE flag1 = (BYTE)(dwFlags);
		BYTE flag2 = (BYTE)(dwFlags >> 8);
		BYTE flag3 = (BYTE)(dwFlags >> 16);
		BYTE flag4 = (BYTE)(dwFlags >> 24);

		bool hasBold				= (0x01 == (0x01 & flag1));
		bool hasItalic				= (0x02 == (0x02 & flag1));
		bool hasUnderline			= (0x04 == (0x04 & flag1));
		// unused
		bool hasShadow				= (0x10 == (0x10 & flag1));
		bool hasFehint				= (0x20 == (0x20 & flag1));
		// unused
		bool hasKimi				= (0x80 == (0x80 & flag1));

		// unused
		bool hasEmboss				= (0x02 == (0x02 & flag2));
		// unused
		BYTE hasStyle				= ((0x3C & flag2) >> 2);
		// unused

		bool typeface_				= (0x01 == (0x01 & flag3));
		bool size_					= (0x02 == (0x02 & flag3));
		bool color_					= (0x04 == (0x04 & flag3));
		bool BaseLineOffset_		= (0x08 == (0x08 & flag3));
		bool EAFontRef_				= (0x20 == (0x20 & flag3));
		bool AnsiFontRef_			= (0x40 == (0x40 & flag3));
		bool SymbolFontRef_			= (0x80 == (0x80 & flag3));

		bool hasNewEATypeface		= (0x01 == (0x01 & flag4));
		bool hasCsTypeface			= (0x02 == (0x02 & flag4));
		bool hasPp11ext				= (0x04 == (0x04 & flag4));

		bool bIsFontStylePresent = (hasBold || hasItalic || hasUnderline || hasShadow || 
			hasFehint || hasKimi || hasEmboss || hasStyle != 0);
		
		if (bIsFontStylePresent)
		{
			WORD fontStyle = StreamUtils::ReadWORD(pStream);

			if (0x01 == (0x01 & fontStyle))
				m_oRun.FontBold = (BOOL)TRUE;
			if (0x02 == (0x02 & fontStyle))
				m_oRun.FontItalic = (BOOL)TRUE;
			if (0x04 == (0x04 & fontStyle))
				m_oRun.FontUnderline = (BOOL)TRUE;
			if (0x10 == (0x10 & fontStyle))
				m_oRun.FontShadow = (BOOL)TRUE;
		}

		if (typeface_)
			m_oRun.Typeface = StreamUtils::ReadWORD(pStream);

		if (EAFontRef_)
			m_oRun.EAFontRef = StreamUtils::ReadWORD(pStream);

		if (AnsiFontRef_)
			m_oRun.AnsiFontRef = StreamUtils::ReadWORD(pStream);

		if (SymbolFontRef_)
			m_oRun.SymbolFontRef = StreamUtils::ReadWORD(pStream);
		
		if (size_)
			m_oRun.Size = StreamUtils::ReadWORD(pStream);

		if (color_)
		{
			SColorAtom oColorAtom;
			NSStreamReader::Read(pStream, oColorAtom);

			NSPresentationEditor::CColor oColor;
			
			oColor.R = oColorAtom.R;
			oColor.G = oColorAtom.G;
			oColor.B = oColorAtom.B;
			oColor.A = 255;
			oColor.m_lSchemeIndex = -1;

			if (oColorAtom.Index < 10)
			{
				oColor.m_lSchemeIndex = oColorAtom.Index;
				NSPresentationEditor::CorrectColorPPT(oColor.m_lSchemeIndex);
			}

			m_oRun.Color = oColor;
		}

		if (BaseLineOffset_)
			m_oRun.BaseLineOffset = (double)StreamUtils::ReadSHORT(pStream);

		// или два последних наоборот????
	}
};

namespace NSPresentationEditor
{
	static void ConvertPPTTextToEditorStructure(CAtlArray<CTextPFRun_ppt>& oArrayPF, CAtlArray<CTextCFRun_ppt>& oArrayCF, 
		CString& strText, NSPresentationEditor::CTextAttributesEx& oAttributes)
	{
		int nCountPFs = (int)oArrayPF.GetCount();
		int nCountCFs = (int)oArrayCF.GetCount();

		oAttributes.m_arParagraphs.RemoveAll();
		int nCurrentPF = 0;
		int nCurrentCF = 0;
		int nOffsetCF  = 0;

		int nIndexLast = strText.GetLength();
		int nIndexText = 0;
		for (int nIndexPF = 0; nIndexPF < nCountPFs; ++nIndexPF)
		{
			oAttributes.m_arParagraphs.Add();
			NSPresentationEditor::CParagraph* pPar = &oAttributes.m_arParagraphs[nIndexPF];

			pPar->m_oPFRun = oArrayPF[nIndexPF].m_oRun;
			pPar->m_lTextType = oAttributes.m_lTextType;
			pPar->m_lTextLevel = oArrayPF[nIndexPF].m_lLevel;

			int nCountInPF = oArrayPF[nIndexPF].m_lCount;

			while (true)
			{
				if (nCurrentCF >= nCountCFs)
					break;

				int nCountAdd = oArrayCF[nCurrentCF].m_lCount - nOffsetCF;
				if (nCountAdd > nCountInPF)
				{
					nOffsetCF += nCountInPF;

					NSPresentationEditor::CSpan oSpan;
					oSpan.m_oRun	= oArrayCF[nCurrentCF].m_oRun;
					oSpan.m_strText = strText.Mid(nIndexText, nCountInPF);

					pPar->m_arSpans.Add(oSpan);
					
					nIndexText += nCountInPF;
					break;
				}
				else if (nCountAdd == nCountInPF)
				{					
					nOffsetCF = 0;

					NSPresentationEditor::CSpan oSpan;
					oSpan.m_oRun	= oArrayCF[nCurrentCF].m_oRun;
					oSpan.m_strText = strText.Mid(nIndexText, nCountAdd);

					pPar->m_arSpans.Add(oSpan);

					nIndexText += nCountAdd;
					++nCurrentCF;
					break;
				}
				else
				{
					nOffsetCF = 0;

					NSPresentationEditor::CSpan oSpan;
					oSpan.m_oRun	= oArrayCF[nCurrentCF].m_oRun;
					oSpan.m_strText = strText.Mid(nIndexText, nCountAdd);

					pPar->m_arSpans.Add(oSpan);

					nIndexText += nCountAdd;
					nCountInPF -= nCountAdd;
					++nCurrentCF;
				}
			}
		}
	}
}