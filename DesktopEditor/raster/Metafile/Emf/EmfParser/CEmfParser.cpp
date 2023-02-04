#include "CEmfParser.h"
#include "CEmfPlusParser.h"

#include "../EmfInterpretator/CEmfInterpretator.h"
#include "../EmfInterpretator/CEmfInterpretatorArray.h"
#include "../EmfInterpretator/CEmfInterpretatorRender.h"

#ifdef METAFILE_SUPPORT_WMF_EMF_XML
#include "../EmfInterpretator/CEmfInterpretatorXml.h"
#endif

#include "../EmfInterpretator/CEmfInterpretatorSvg.h"
#include "../../Wmf/WmfInterpretator/CWmfInterpretatorSvg.h"

#include "../../Wmf/WmfParser/CWmfParser.h"

namespace MetaFile
{
	CEmfParser::CEmfParser() : m_pEmfPlusParser(NULL){}

	CEmfParser::~CEmfParser()
	{
		ClearFile();
		RELEASEOBJECT(m_pInterpretator);
	}

	bool CEmfParser::ReadFromBuffer(BYTE *pBuffer, unsigned int unSize, const bool& bIsExternal)
	{
		return IMetaFileBase::ReadFromBuffer(pBuffer, unSize, bIsExternal);
	}
	bool CEmfParser::OpenFromFile(const wchar_t *wsFilePath)
	{
		return IMetaFileBase::OpenFromFile(wsFilePath);
	}

	void CEmfParser::PlayFile()
	{
		if (!m_oStream.IsValid())
			SetError();

		unsigned int ulSize, ulType;

		unsigned int ulRecordIndex	= 0;
		unsigned int m_ulRecordPos	= 0;

		do
		{
			if (m_oStream.IsEof())
				break;

			if (m_oStream.CanRead() < 8)
				return SetError();

			m_oStream >> ulType;
			m_oStream >> ulSize;

			if (ulSize < 1)
				continue;

			m_ulRecordPos	= m_oStream.Tell();
			m_ulRecordSize	= ulSize - 8;

			if (ulType < EMR_MIN || ulType > EMR_MAX)
			{
				if (ENHMETA_SIGNATURE != m_oHeader.ulSignature || 0x00010000 != m_oHeader.ulVersion)
					return SetError();
				else
					break;
			}

			if (0 == ulRecordIndex && EMR_HEADER != ulType)
				return SetError();

			switch (ulType)
			{
			//-----------------------------------------------------------
			// 2.3.1 Bitmap
			//-----------------------------------------------------------
			case EMR_ALPHABLEND:        Read_EMR_ALPHABLEND(); break;
			case EMR_BITBLT:            Read_EMR_BITBLT(); break;
			case EMR_STRETCHDIBITS:     Read_EMR_STRETCHDIBITS(); break;
			case EMR_SETDIBITSTODEVICE: Read_EMR_SETDIBITSTODEVICE(); break;
			case EMR_STRETCHBLT:        Read_EMR_STRETCHBLT(); break;
				//-----------------------------------------------------------
				// 2.3.2 Clipping
				//-----------------------------------------------------------
			case EMR_EXCLUDECLIPRECT:   Read_EMR_EXCLUDECLIPRECT(); break;
			case EMR_EXTSELECTCLIPRGN:  Read_EMR_EXTSELECTCLIPRGN(); break;
			case EMR_INTERSECTCLIPRECT: Read_EMR_INTERSECTCLIPRECT(); break;
			case EMR_SELECTCLIPPATH:    Read_EMR_SELECTCLIPPATH(); break;
			case EMR_SETMETARGN:        Read_EMR_SETMETARGN(); break;
				//-----------------------------------------------------------
				// 2.3.4 Control
				//-----------------------------------------------------------
			case EMR_HEADER:    Read_EMR_HEADER(); break;
			case EMR_EOF:       Read_EMR_EOF(); break;
				//-----------------------------------------------------------
				// 2.3.5 Drawing
				//-----------------------------------------------------------
			case EMR_ANGLEARC:          Read_EMR_ANGLEARC(); break;
			case EMR_ARC:               Read_EMR_ARC(); break;
			case EMR_ARCTO:             Read_EMR_ARCTO(); break;
			case EMR_CHORD:             Read_EMR_CHORD(); break;
			case EMR_ELLIPSE:           Read_EMR_ELLIPSE(); break;
			case EMR_EXTTEXTOUTA:       Read_EMR_EXTTEXTOUTA(); break;
			case EMR_EXTTEXTOUTW:       Read_EMR_EXTTEXTOUTW(); break;
			case EMR_FILLPATH:          Read_EMR_FILLPATH(); break;
			case EMR_LINETO:            Read_EMR_LINETO(); break;
			case EMR_PIE:               Read_EMR_PIE(); break;
			case EMR_POLYBEZIER:        Read_EMR_POLYBEZIER(); break;
			case EMR_POLYBEZIER16:      Read_EMR_POLYBEZIER16(); break;
			case EMR_POLYBEZIERTO:      Read_EMR_POLYBEZIERTO(); break;
			case EMR_POLYBEZIERTO16:    Read_EMR_POLYBEZIERTO16(); break;
			case EMR_POLYDRAW:          Read_EMR_POLYDRAW(); break;
			case EMR_POLYDRAW16:        Read_EMR_POLYDRAW16(); break;
			case EMR_POLYGON:           Read_EMR_POLYGON(); break;
			case EMR_POLYGON16:         Read_EMR_POLYGON16(); break;
			case EMR_POLYLINE:          Read_EMR_POLYLINE(); break;
			case EMR_POLYLINE16:        Read_EMR_POLYLINE16(); break;
			case EMR_POLYLINETO:        Read_EMR_POLYLINETO(); break;
			case EMR_POLYLINETO16:      Read_EMR_POLYLINETO16(); break;
			case EMR_POLYPOLYGON:       Read_EMR_POLYPOLYGON(); break;
			case EMR_POLYPOLYGON16:     Read_EMR_POLYPOLYGON16(); break;
			case EMR_POLYPOLYLINE:      Read_EMR_POLYPOLYLINE(); break;
			case EMR_POLYPOLYLINE16:    Read_EMR_POLYPOLYLINE16(); break;
			case EMR_POLYTEXTOUTA:      Read_EMR_POLYTEXTOUTA(); break;
			case EMR_POLYTEXTOUTW:      Read_EMR_POLYTEXTOUTW(); break;
			case EMR_RECTANGLE:         Read_EMR_RECTANGLE(); break;
			case EMR_ROUNDRECT:         Read_EMR_ROUNDRECT(); break;
			case EMR_SETPIXELV:         Read_EMR_SETPIXELV(); break;
			case EMR_SMALLTEXTOUT:      Read_EMR_SMALLTEXTOUT(); break;
			case EMR_STROKEANDFILLPATH: Read_EMR_STROKEANDFILLPATH(); break;
			case EMR_STROKEPATH:        Read_EMR_STROKEPATH(); break;
			case EMR_PAINTRGN:			Read_EMR_PAINTRGN(); break;
			case EMR_FILLRGN:           Read_EMR_FILLRGN(); break;
			case EMR_FRAMERGN:          Read_EMR_FRAMERGN(); break;
				//-----------------------------------------------------------
				// 2.3.7 Object Creation
				//-----------------------------------------------------------
			case EMR_CREATEBRUSHINDIRECT:       Read_EMR_CREATEBRUSHINDIRECT(); break;
			case EMR_CREATEDIBPATTERNBRUSHPT:   Read_EMR_CREATEDIBPATTERNBRUSHPT(); break;
			case EMR_CREATEMONOBRUSH:           Read_EMR_CREATEMONOBRUSH(); break;
			case EMR_CREATEPALETTE:             Read_EMR_CREATEPALETTE(); break;
			case EMR_CREATEPEN:                 Read_EMR_CREATEPEN(); break;
			case EMR_EXTCREATEFONTINDIRECTW:    Read_EMR_EXTCREATEFONTINDIRECTW(); break;
			case EMR_EXTCREATEPEN:              Read_EMR_EXTCREATEPEN(); break;
				//-----------------------------------------------------------
				// 2.3.8 Object Manipulation
				//-----------------------------------------------------------
			case EMR_SELECTOBJECT:  Read_EMR_SELECTOBJECT(); break;
			case EMR_DELETEOBJECT:  Read_EMR_DELETEOBJECT(); break;
			case EMR_SELECTPALETTE: Read_EMR_SELECTPALETTE(); break;
				//-----------------------------------------------------------
				// 2.3.10 Path Bracket
				//-----------------------------------------------------------
			case EMR_BEGINPATH:   Read_EMR_BEGINPATH(); break;
			case EMR_ENDPATH:     Read_EMR_ENDPATH(); break;
			case EMR_CLOSEFIGURE: Read_EMR_CLOSEFIGURE(); break;
			case EMR_FLATTENPATH: Read_EMR_FLATTENPATH(); break;
			case EMR_WIDENPATH:   Read_EMR_WIDENPATH(); break;
			case EMR_ABORTPATH:   Read_EMR_ABORTPATH(); break;
				//-----------------------------------------------------------
				// 2.3.11 State
				//-----------------------------------------------------------
			case EMR_MOVETOEX:          Read_EMR_MOVETOEX(); break;
			case EMR_SETARCDIRECTION:   Read_EMR_SETARCDIRECTION(); break;
			case EMR_SAVEDC:            Read_EMR_SAVEDC(); break;
			case EMR_RESTOREDC:         Read_EMR_RESTOREDC(); break;
			case EMR_SETTEXTCOLOR:      Read_EMR_SETTEXTCOLOR(); break;
			case EMR_SETTEXTALIGN:      Read_EMR_SETTEXTALIGN(); break;
			case EMR_SETBKMODE:         Read_EMR_SETBKMODE(); break;
			case EMR_SETMITERLIMIT:     Read_EMR_SETMITERLIMIT(); break;
			case EMR_SETPOLYFILLMODE:   Read_EMR_SETPOLYFILLMODE(); break;
			case EMR_SETMAPMODE:        Read_EMR_SETMAPMODE(); break;
			case EMR_SETWINDOWORGEX:    Read_EMR_SETWINDOWORGEX(); break;
			case EMR_SETWINDOWEXTEX:    Read_EMR_SETWINDOWEXTEX(); break;
			case EMR_SCALEWINDOWEXTEX:  Read_EMR_SCALEWINDOWEXTEX(); break;
			case EMR_SETVIEWPORTORGEX:  Read_EMR_SETVIEWPORTORGEX(); break;
			case EMR_SETVIEWPORTEXTEX:  Read_EMR_SETVIEWPORTEXTEX(); break;
			case EMR_SCALEVIEWPORTEXTEX:Read_EMR_SCALEVIEWPORTEXTEX(); break;
			case EMR_SETBKCOLOR:        Read_EMR_SETBKCOLOR(); break;
			case EMR_SETSTRETCHBLTMODE: Read_EMR_SETSTRETCHBLTMODE(); break;
			case EMR_SETICMMODE:        Read_EMR_SETICMMODE(); break;
			case EMR_SETROP2:           Read_EMR_SETROP2(); break;
			case EMR_REALIZEPALETTE:    Read_EMR_REALIZEPALETTE(); break;
			case EMR_SETLAYOUT:         Read_EMR_SETLAYOUT(); break;
			case EMR_SETBRUSHORGEX:     Read_EMR_SETBRUSHORGEX(); break;
				//-----------------------------------------------------------
				// 2.3.12 Transform
				//-----------------------------------------------------------
			case EMR_SETWORLDTRANSFORM:	Read_EMR_SETWORLDTRANSFORM(); break;
			case EMR_MODIFYWORLDTRANSFORM:	Read_EMR_MODIFYWORLDTRANSFORM(); break;

			case EMR_GDICOMMENT: Read_EMR_COMMENT(); break;
				//-----------------------------------------------------------
				// Неизвестные записи
				//-----------------------------------------------------------
			default:
			{
				Read_EMR_UNKNOWN();
				break;
			}
			}

			if (m_bEof)
				break;

			int need_skip = m_ulRecordSize - (m_oStream.Tell() - m_ulRecordPos);
			m_oStream.Skip(need_skip);
			ulRecordIndex++;

		} while (!CheckError());

		if (!CheckError())
			m_oStream.SeekToStart();

		if (!m_bEof)
			HANDLE_EMR_EOF();

		ClearFile();
	}

	void CEmfParser::Scan()
	{
		CEmfInterpretatorBase *pInterpretator = m_pInterpretator;
		m_pInterpretator = NULL;
		PlayFile();
		m_pInterpretator = pInterpretator;
	}

	void CEmfParser::ClearFile()
	{
		RELEASEOBJECT(m_pEmfPlusParser);

		CEmfParserBase::ClearFile();
	}

	EmfParserType CEmfParser::GetType()
	{
		return EmfParserType::EmfParser;
	}

	void CEmfParser::SetStream(BYTE *pBuf, unsigned int unSize)
	{
		m_oStream.SetStream(pBuf, unSize);
	}

	void CEmfParser::SetInterpretator(IOutputDevice* pOutput)
	{
		CEmfParserBase::SetInterpretator(pOutput);
		if (m_pEmfPlusParser)
			RELEASEOBJECT(m_pEmfPlusParser);
	}

	bool CEmfParser::ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight)
	{
		int lHeaderOffset         = offBmi - ulSkip;
		unsigned int ulHeaderSize = cbBmi;
		int lBitsOffset           = offBits - offBmi - cbBmi;
		unsigned int ulBitsSize   = cbBits;

		if (ulHeaderSize <= 0 || ulBitsSize <= 0 || lHeaderOffset < 0 || lBitsOffset < 0)
		{
			// TODO: Если попали сюда, значит надо смотреть BitBltRasterOperation
			if (lHeaderOffset > 0)
				m_oStream.Skip(lHeaderOffset);

			m_oStream.Skip(ulHeaderSize);

			if (lBitsOffset > 0)
				m_oStream.Skip(lBitsOffset);

			m_oStream.Skip(ulBitsSize);

			return false;
		}

		m_oStream.Skip(lHeaderOffset);

		BYTE* pHeaderBuffer = m_oStream.GetCurPtr();
		m_oStream.Skip(ulHeaderSize + lBitsOffset);
		BYTE* pBitsBuffer = m_oStream.GetCurPtr();

		unsigned int ulBitsSizeSkip = 0 == ulBitsSize ? 0 : ((int)(((double)ulBitsSize - 0.5) / 4) + 1) * 4;
		m_oStream.Skip(ulBitsSizeSkip);

		MetaFile::ReadImage(pHeaderBuffer, ulHeaderSize, pBitsBuffer, ulBitsSize, ppBgraBuffer, pulWidth, pulHeight);

		return true;
	}

	void CEmfParser::Read_EMR_HEADER()
	{
		m_oStream >> m_oHeader.oBounds;
		m_oStream >> m_oHeader.oFrame;
		m_oStream >> m_oHeader.ulSignature;
		m_oStream >> m_oHeader.ulVersion;
		m_oStream >> m_oHeader.ulSize;
		m_oStream >> m_oHeader.ulRecords;
		m_oStream >> m_oHeader.ushObjects;
		m_oStream >> m_oHeader.ushReserved;
		m_oStream >> m_oHeader.ulSizeDescription;
		m_oStream >> m_oHeader.ulOffsetDescription;
		m_oStream >> m_oHeader.ulPalEntries;
		m_oStream >> m_oHeader.oDevice;
		m_oStream >> m_oHeader.oMillimeters;

		HANDLE_EMR_HEADER(m_oHeader);
	}

	void CEmfParser::Read_EMR_ALPHABLEND()
	{
		TEmfAlphaBlend oBitmap;

		m_oStream >> oBitmap;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_ALPHABLEND(oBitmap);
	}

	void CEmfParser::Read_EMR_STRETCHDIBITS()
	{
		TEmfStretchDIBITS oBitmap;

		m_oStream >> oBitmap;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_STRETCHDIBITS(oBitmap);
	}

	void CEmfParser::Read_EMR_BITBLT()
	{
		TEmfBitBlt oBitmap;

		m_oStream >> oBitmap;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_BITBLT(oBitmap);
	}

	void CEmfParser::Read_EMR_SETDIBITSTODEVICE()
	{
		TEmfSetDiBitsToDevice oBitmap;

		m_oStream >> oBitmap;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETDIBITSTODEVICE(oBitmap);
	}

	void CEmfParser::Read_EMR_STRETCHBLT()
	{
		TEmfStretchBLT oBitmap;

		m_oStream >> oBitmap;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_STRETCHBLT(oBitmap);
	}

	void CEmfParser::Read_EMR_EOF()
	{
		unsigned int ulCount, ulOffset, ulSizeLast;

		m_oStream >> ulCount;
		m_oStream >> ulOffset;

		m_oStream.Skip(m_ulRecordSize - 8 - 4);

		m_oStream >> ulSizeLast;

		HANDLE_EMR_EOF();
	}

	void CEmfParser::Read_EMR_UNKNOWN()
	{
		HANDLE_EMR_UNKNOWN(m_ulRecordSize);
	}

	void CEmfParser::Read_EMR_SAVEDC()
	{
		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SAVEDC();
	}

	void CEmfParser::Read_EMR_RESTOREDC()
	{
		int lSavedDC;

		m_oStream >> lSavedDC;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_RESTOREDC(lSavedDC);
	}

	void CEmfParser::Read_EMR_MODIFYWORLDTRANSFORM()
	{
		TEmfXForm oXForm;
		unsigned int ulMode;

		m_oStream >> oXForm;
		m_oStream >> ulMode;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_MODIFYWORLDTRANSFORM(oXForm, ulMode);
	}

	void CEmfParser::Read_EMR_SETWORLDTRANSFORM()
	{
		TEmfXForm oXForm;

		m_oStream >> oXForm;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETWORLDTRANSFORM(oXForm);
	}

	void CEmfParser::Read_EMR_CREATEBRUSHINDIRECT()
	{
		unsigned int ulBrushIndex;
		CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();
		if (!pBrush)
			return SetError();

		m_oStream >> ulBrushIndex;
		m_oStream >> *pBrush;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_CREATEBRUSHINDIRECT(ulBrushIndex, pBrush);
	}

	void CEmfParser::Read_EMR_SETTEXTCOLOR()
	{
		TEmfColor oColor;

		m_oStream >> oColor;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETTEXTCOLOR(oColor);
	}

	void CEmfParser::Read_EMR_SELECTOBJECT()
	{
		unsigned int ulObjectIndex;

		m_oStream >> ulObjectIndex;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SELECTOBJECT(ulObjectIndex);
	}

	void CEmfParser::Read_EMR_EXTCREATEFONTINDIRECTW()
	{
		unsigned int unSize = m_ulRecordSize - 4;
		bool bFixedLength = unSize <= 0x0140 ? true : false;

		unsigned int ulIndex;
		CEmfLogFont* pFont = new CEmfLogFont(bFixedLength);
		if (!pFont)
			return SetError();

		m_oStream >> ulIndex;
		m_oStream >> *pFont;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_EXTCREATEFONTINDIRECTW(ulIndex, pFont);
	}

	void CEmfParser::Read_EMR_SETTEXTALIGN()
	{
		unsigned int ulAlign;

		m_oStream >> ulAlign;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETTEXTALIGN(ulAlign);
	}

	void CEmfParser::Read_EMR_SETBKMODE()
	{
		unsigned int ulBgMode;

		m_oStream >> ulBgMode;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETBKMODE(ulBgMode);
	}

	void CEmfParser::Read_EMR_DELETEOBJECT()
	{
		unsigned int ulIndex;

		m_oStream >> ulIndex;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_DELETEOBJECT(ulIndex);
	}

	void CEmfParser::Read_EMR_SETMITERLIMIT()
	{
		unsigned int ulMiterLimit;

		m_oStream >> ulMiterLimit;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETMITERLIMIT(ulMiterLimit);
	}

	void CEmfParser::Read_EMR_EXTCREATEPEN()
	{
		unsigned int ulPenIndex;

		m_oStream >> ulPenIndex;
		m_oStream.Skip(4); // offBmi
		m_oStream.Skip(4); // cbBmi
		m_oStream.Skip(4); // offBits
		m_oStream.Skip(4); // cbBits

		unsigned int current_size = m_ulRecordSize - 20;

		CEmfLogPen* pPen = new CEmfLogPen();
		if (!pPen)
			return SetError();

		std::vector<unsigned int> arUnused(2);

		m_oStream >> pPen->PenStyle;
		m_oStream >> pPen->Width;
		m_oStream >> arUnused[0]; // BrushStyle
		m_oStream >> pPen->Color;
		m_oStream >> arUnused[1]; // BrushHatch

		m_oStream >> pPen->NumStyleEntries;

		current_size -= 24;
		if (pPen->NumStyleEntries > 0)
		{
			current_size -= pPen->NumStyleEntries * 4;
			pPen->StyleEntry = new unsigned int[pPen->NumStyleEntries];
			if (!pPen->StyleEntry)
			{
				delete pPen;
				return SetError();
			}

			for (unsigned int ulIndex = 0; ulIndex < pPen->NumStyleEntries; ulIndex++)
				m_oStream >> pPen->StyleEntry[ulIndex];
		}
		else
		{
			pPen->StyleEntry = NULL;
		}

		// Пропускаем часть с картинкой, если она была
		m_oStream.Skip(current_size);

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_EXTCREATEPEN(ulPenIndex, pPen, arUnused);
	}

	void CEmfParser::Read_EMR_CREATEPEN()
	{
		unsigned int ulPenIndex;

		m_oStream >> ulPenIndex;

		CEmfLogPen* pPen = new CEmfLogPen();
		if (!pPen)
			return SetError();

		m_oStream >> pPen->PenStyle;

		unsigned int widthX, widthY;

		m_oStream >> widthX;
		m_oStream >> widthY;
		m_oStream >> pPen->Color;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_CREATEPEN(ulPenIndex, widthX, pPen);
	}

	void CEmfParser::Read_EMR_SETPOLYFILLMODE()
	{
		unsigned int ulFillMode;

		m_oStream >> ulFillMode;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETPOLYFILLMODE(ulFillMode);
	}

	void CEmfParser::Read_EMR_BEGINPATH()
	{
		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_BEGINPATH();
	}

	void CEmfParser::Read_EMR_ENDPATH()
	{
		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_ENDPATH();
	}

	void CEmfParser::Read_EMR_CLOSEFIGURE()
	{
		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_CLOSEFIGURE();
	}

	void CEmfParser::Read_EMR_FLATTENPATH()
	{
		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_FLATTENPATH();
	}

	void CEmfParser::Read_EMR_WIDENPATH()
	{
		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_WIDENPATH();
	}

	void CEmfParser::Read_EMR_ABORTPATH()
	{
		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_ABORTPATH();
	}

	void CEmfParser::Read_EMR_MOVETOEX()
	{
		TEmfPointL oPoint;

		m_oStream >> oPoint;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_MOVETOEX(oPoint);
	}

	void CEmfParser::Read_EMR_SETARCDIRECTION()
	{
		unsigned int unDirection;

		m_oStream >> unDirection;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETARCDIRECTION(unDirection);
	}

	void CEmfParser::Read_EMR_FILLPATH()
	{
		TEmfRectL oBounds;

		m_oStream >> oBounds;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_FILLPATH(oBounds);
	}

	void CEmfParser::Read_EMR_SETMAPMODE()
	{
		unsigned int ulMapMode;

		m_oStream >> ulMapMode;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETMAPMODE(ulMapMode);
	}

	void CEmfParser::Read_EMR_SETWINDOWORGEX()
	{
		TEmfPointL oOrigin;

		m_oStream >> oOrigin;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETWINDOWORGEX(oOrigin);
	}

	void CEmfParser::Read_EMR_SETWINDOWEXTEX()
	{
		TEmfSizeL oExtent;

		m_oStream >> oExtent;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETWINDOWEXTEX(oExtent);
	}

	void CEmfParser::Read_EMR_SCALEWINDOWEXTEX()
	{
		int nXNum, nXDenom, nYNum, nYDenom;

		m_oStream >> nXNum;
		m_oStream >> nXDenom;
		m_oStream >> nYNum;
		m_oStream >> nYDenom;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SCALEWINDOWEXTEX(nXNum, nXDenom, nYNum, nYDenom);
	}

	void CEmfParser::Read_EMR_SETVIEWPORTORGEX()
	{
		TEmfPointL oOrigin;

		m_oStream >> oOrigin;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETVIEWPORTORGEX(oOrigin);
	}

	void CEmfParser::Read_EMR_SETVIEWPORTEXTEX()
	{
		TEmfSizeL oExtent;

		m_oStream >> oExtent;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETVIEWPORTEXTEX(oExtent);
	}

	void CEmfParser::Read_EMR_SCALEVIEWPORTEXTEX()
	{
		int nXNum, nXDenom, nYNum, nYDenom;

		m_oStream >> nXNum;
		m_oStream >> nXDenom;
		m_oStream >> nYNum;
		m_oStream >> nYDenom;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SCALEVIEWPORTEXTEX(nXNum, nXDenom, nYNum, nYDenom);
	}

	void CEmfParser::Read_EMR_SETSTRETCHBLTMODE()
	{
		unsigned int ulStretchMode;

		m_oStream >> ulStretchMode;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETSTRETCHBLTMODE(ulStretchMode);
	}

	void CEmfParser::Read_EMR_SETICMMODE()
	{
		unsigned int ulICMMode;

		m_oStream >> ulICMMode;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETICMMODE(ulICMMode);
	}

	void CEmfParser::Read_EMR_CREATEDIBPATTERNBRUSHPT()
	{
		unsigned int ulBrushIndex;
		TEmfDibPatternBrush oDibBrush;

		m_oStream >> ulBrushIndex;
		m_oStream >> oDibBrush;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(ulBrushIndex, oDibBrush);
	}

	void CEmfParser::Read_EMR_CREATEMONOBRUSH()
	{
		unsigned int ulBrushIndex;
		TEmfDibPatternBrush oDibBrush;

		m_oStream >> ulBrushIndex;
		m_oStream >> oDibBrush;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_CREATEMONOBRUSH(ulBrushIndex, oDibBrush);
	}

	void CEmfParser::Read_EMR_SELECTCLIPPATH()
	{
		unsigned int unRegionMode;

		m_oStream >> unRegionMode;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SELECTCLIPPATH(unRegionMode);
	}

	void CEmfParser::Read_EMR_SETBKCOLOR()
	{
		TEmfColor oColor;

		m_oStream >> oColor;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETBKCOLOR(oColor);
	}

	void CEmfParser::Read_EMR_EXCLUDECLIPRECT()
	{
		// TODO: Проверить как найдется файл
		TEmfRectL oClip;

		m_oStream >> oClip;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_EXCLUDECLIPRECT(oClip);
	}

	void CEmfParser::Read_EMR_EXTSELECTCLIPRGN()
	{
		unsigned int ulRgnDataSize, ulRegionMode;

		m_oStream >> ulRgnDataSize;
		m_oStream >> ulRegionMode;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_EXTSELECTCLIPRGN(ulRgnDataSize, ulRegionMode);
	}

	void CEmfParser::Read_EMR_SETMETARGN()
	{
		HANDLE_EMR_SETMETARGN();
	}

	void CEmfParser::Read_EMR_SETROP2()
	{
		unsigned int ulRop2Mode;

		m_oStream >> ulRop2Mode;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETROP2(ulRop2Mode);
	}

	void CEmfParser::Read_EMR_CREATEPALETTE()
	{
		unsigned int ulPaletteIndex;
		CEmfLogPalette* pPalette = new CEmfLogPalette();
		if (!pPalette)
			return SetError();

		m_oStream >> ulPaletteIndex;
		m_oStream >> *pPalette;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_CREATEPALETTE(ulPaletteIndex, pPalette);
	}

	void CEmfParser::Read_EMR_SELECTPALETTE()
	{
		unsigned int ulIndex;

		m_oStream >> ulIndex;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SELECTPALETTE(ulIndex);
	}

	void CEmfParser::Read_EMR_REALIZEPALETTE()
	{
		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_REALIZEPALETTE();
	}

	void CEmfParser::Read_EMR_INTERSECTCLIPRECT()
	{
		TEmfRectL oClip;

		m_oStream >> oClip;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_INTERSECTCLIPRECT(oClip);
	}

	void CEmfParser::Read_EMR_SETLAYOUT()
	{
		unsigned int ulLayoutMode;

		m_oStream >> ulLayoutMode;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETLAYOUT(ulLayoutMode);
	}

	void CEmfParser::Read_EMR_SETBRUSHORGEX()
	{
		TEmfPointL oOrigin;

		m_oStream >> oOrigin;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETBRUSHORGEX(oOrigin);
	}

	void CEmfParser::Read_EMR_ANGLEARC()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfPointL oCenter;
		unsigned int unRadius;
		double dStartAngle, dSweepAngle;

		m_oStream >> oCenter;
		m_oStream >> unRadius;

		m_oStream >> dStartAngle;
		m_oStream >> dSweepAngle;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_ANGLEARC(oCenter, unRadius, dStartAngle, dSweepAngle);
	}

	void CEmfParser::Read_EMR_ARC_BASE(TEmfRectL& oBox, TEmfPointL& oStart, TEmfPointL& oEnd, double& dStartAngle, double& dSweepAngle)
	{
		m_oStream >> oBox;
		m_oStream >> oStart;
		m_oStream >> oEnd;

		dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
		dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y) - dStartAngle;

		// TODO: Проверить здесь
		if (dSweepAngle < 0.001)
			dSweepAngle += 360;

		// TODO: Проверить здесь
		if (AD_COUNTERCLOCKWISE != m_pDC->GetArcDirection())
		{
			dSweepAngle = dSweepAngle - 360;
		}
	}

	void CEmfParser::Read_EMR_ARC()
	{
		TEmfRectL oBox;
		TEmfPointL oStart, oEnd;
		double dStartAngle, dSweep;

		Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_ARC(oBox, oStart, oEnd);
	}

	void CEmfParser::Read_EMR_ARCTO()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfRectL oBox;
		TEmfPointL oStart, oEnd;
		double dStartAngle, dSweep;

		Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_ARCTO(oBox, oStart, oEnd);
	}

	void CEmfParser::Read_EMR_CHORD()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfRectL oBox;
		TEmfPointL oStart, oEnd;
		double dStartAngle, dSweep;

		Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_CHORD(oBox, oStart, oEnd);
	}

	void CEmfParser::Read_EMR_ELLIPSE()
	{
		TEmfRectL oBox;

		m_oStream >> oBox;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_ELLIPSE(oBox);
	}

	void CEmfParser::Read_EMR_EXTTEXTOUTA()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfExtTextoutA oText;

		m_oStream >> oText;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_EXTTEXTOUTA(oText);
	}

	void CEmfParser::Read_EMR_EXTTEXTOUTW()
	{
		TEmfExtTextoutW oText;

		m_oStream >> oText;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_EXTTEXTOUTW(oText);
	}

	void CEmfParser::Read_EMR_LINETO()
	{
		TEmfPointL oPoint;

		m_oStream >> oPoint;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_LINETO(oPoint);
	}

	void CEmfParser::Read_EMR_PIE()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfRectL oBox;
		TEmfPointL oStart, oEnd;
		double dStartAngle, dSweep;

		Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_PIE(oBox, oStart, oEnd);
	}

	void CEmfParser::Read_EMR_POLYBEZIER()
	{
		Read_EMR_POLYBEZIER_BASE<TEmfPointL>();
	}

	void CEmfParser::Read_EMR_POLYBEZIER16()
	{
		Read_EMR_POLYBEZIER_BASE<TEmfPointS>();
	}

	template<typename T>void CEmfParser::Read_EMR_POLYBEZIER_BASE()
	{
		TEmfRectL oBounds;
		unsigned int ulCount;

		m_oStream >> oBounds;
		m_oStream >> ulCount;

		if (0 == ulCount)
			return;

		std::vector<T> arPoints(ulCount);

		m_oStream >> arPoints[0];

		for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex += 3)
		{
			m_oStream >> arPoints[ulIndex];
			m_oStream >> arPoints[ulIndex + 1];
			m_oStream >> arPoints[ulIndex + 2];
		}

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_POLYBEZIER(oBounds, arPoints);
	}

	void CEmfParser::Read_EMR_POLYBEZIERTO()
	{
		Read_EMR_POLYBEZIERTO_BASE<TEmfPointL>();
	}

	void CEmfParser::Read_EMR_POLYBEZIERTO16()
	{
		Read_EMR_POLYBEZIERTO_BASE<TEmfPointS>();
	}

	template<typename T>void CEmfParser::Read_EMR_POLYBEZIERTO_BASE()
	{
		TEmfRectL oBounds;
		unsigned int ulCount;

		m_oStream >> oBounds;
		m_oStream >> ulCount;

		std::vector<T> arPoints(ulCount);

		for (unsigned int ulIndex = 0; ulIndex < ulCount; ulIndex += 3)
		{
			if (ulCount - ulIndex < 2)
				return SetError();

			m_oStream >> arPoints[ulIndex];
			m_oStream >> arPoints[ulIndex + 1];
			m_oStream >> arPoints[ulIndex + 2];
		}

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_POLYBEZIERTO_BASE(oBounds, arPoints);
	}

	void CEmfParser::Read_EMR_POLYDRAW()
	{
		Read_EMR_POLYDRAW_BASE<TEmfPointL>();
	}

	void CEmfParser::Read_EMR_POLYDRAW16()
	{
		Read_EMR_POLYDRAW_BASE<TEmfPointS>();
	}

	template<typename T>void CEmfParser::Read_EMR_POLYDRAW_BASE()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		//bug #35006 - не прочитывается весь рекорд ... выравнивание?

		TEmfRectL oBounds;
		unsigned int unCount;

		m_oStream >> oBounds;
		m_oStream >> unCount;

		if (0 == unCount)
			return;

		T* pPoints = new T[unCount];
		if (!pPoints)
			return SetError();

		for (unsigned int unIndex = 0; unIndex < unCount; unIndex++)
			m_oStream >> pPoints[unIndex];

		unsigned char* pAbTypes = new unsigned char[unCount];
		if (!pAbTypes)
		{
			delete[] pPoints;
			return SetError();
		}

		for (unsigned int unIndex = 0; unIndex < unCount; unIndex++)
			m_oStream >> pAbTypes[unIndex];

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_POLYDRAW_BASE(oBounds, pPoints, unCount, pAbTypes);

		delete[] pPoints;
		delete[] pAbTypes;
	}

	void CEmfParser::Read_EMR_POLYGON()
	{
		Read_EMR_POLYGON_BASE<TEmfPointL>();
	}

	void CEmfParser::Read_EMR_POLYGON16()
	{
		Read_EMR_POLYGON_BASE<TEmfPointS>();
	}

	template<typename T>void CEmfParser::Read_EMR_POLYGON_BASE()
	{
		TEmfRectL oBounds;
		unsigned int ulCount;

		m_oStream >> oBounds;
		m_oStream >> ulCount;

		if (ulCount <= 0)
			return;

		std::vector<T> arPoints(ulCount);

		for (unsigned int ulIndex = 0; ulIndex < ulCount; ulIndex++)
			m_oStream >> arPoints[ulIndex];

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_POLYGON_BASE(oBounds, arPoints);
	}

	void CEmfParser::Read_EMR_POLYLINE()
	{
		Read_EMR_POLYLINE_BASE<TEmfPointL>();
	}

	void CEmfParser::Read_EMR_POLYLINE16()
	{
		Read_EMR_POLYLINE_BASE<TEmfPointS>();
	}

	template<typename T>void CEmfParser::Read_EMR_POLYLINE_BASE()
	{
		TEmfRectL oBounds;
		unsigned int ulCount;

		m_oStream >> oBounds;
		m_oStream >> ulCount;

		if (0 == ulCount)
			return;

		std::vector<T> arPoints(ulCount);

		for (unsigned int ulIndex = 0; ulIndex < ulCount; ulIndex++)
			m_oStream >> arPoints[ulIndex];

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_POLYLINE_BASE(oBounds, arPoints);
	}

	void CEmfParser::Read_EMR_POLYLINETO()
	{
		Read_EMR_POLYLINETO_BASE<TEmfPointL>();
	}

	void CEmfParser::Read_EMR_POLYLINETO16()
	{
		Read_EMR_POLYLINETO_BASE<TEmfPointS>();
	}

	template<typename T>void CEmfParser::Read_EMR_POLYLINETO_BASE()
	{
		TEmfRectL oBounds;
		unsigned int ulCount;

		m_oStream >> oBounds;
		m_oStream >> ulCount;

		std::vector<T> arPoints(ulCount);

		for (unsigned int ulIndex = 0; ulIndex < ulCount; ulIndex++)
			m_oStream >> arPoints[ulIndex];

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_POLYLINETO_BASE(oBounds, arPoints);
	}

	void CEmfParser::Read_EMR_POLYPOLYGON()
	{
		Read_EMR_POLYPOLYGON_BASE<TEmfPointL>();
	}

	void CEmfParser::Read_EMR_POLYPOLYGON16()
	{
		Read_EMR_POLYPOLYGON_BASE<TEmfPointS>();
	}

	template<typename T>void CEmfParser::Read_EMR_POLYPOLYGON_BASE()
	{
		TEmfRectL oBounds;
		unsigned int ulNumberOfPolygons;
		unsigned int ulTotalPointsCount;

		m_oStream >> oBounds;
		m_oStream >> ulNumberOfPolygons;
		m_oStream >> ulTotalPointsCount;

		unsigned int* pPolygonPointCount = new unsigned int[ulNumberOfPolygons];

		if (!pPolygonPointCount)
			return SetError();

		std::vector<std::vector<T>> arPoints;
		arPoints.resize(ulNumberOfPolygons);

		for (unsigned int ulIndex = 0; ulIndex < ulNumberOfPolygons; ulIndex++)
		{
			m_oStream >> pPolygonPointCount[ulIndex];
			arPoints[ulIndex].resize(pPolygonPointCount[ulIndex]);
		}

		for (unsigned int unPolygonIndex = 0; unPolygonIndex < ulNumberOfPolygons; ++unPolygonIndex)
			for (unsigned int unPointIndex = 0; unPointIndex < arPoints[unPolygonIndex].size(); ++unPointIndex)
				m_oStream >> arPoints[unPolygonIndex][unPointIndex];

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_POLYPOLYGON(oBounds, arPoints);

		delete[] pPolygonPointCount;
	}

	void CEmfParser::Read_EMR_POLYPOLYLINE()
	{
		Read_EMR_POLYPOLYLINE_BASE<TEmfPointL>();
	}

	void CEmfParser::Read_EMR_POLYPOLYLINE16()
	{
		Read_EMR_POLYPOLYLINE_BASE<TEmfPointS>();
	}

	template<typename T>void CEmfParser::Read_EMR_POLYPOLYLINE_BASE()
	{
		TEmfRectL oBounds;
		unsigned int ulNumberOfPolylines;
		unsigned int ulTotalPointsCount;

		//TODO: сделать сохранение в XML

		m_oStream >> oBounds;
		m_oStream >> ulNumberOfPolylines;
		m_oStream >> ulTotalPointsCount;

		if (0 == ulNumberOfPolylines && 0 == ulTotalPointsCount)
			return;
		else if (0 == ulNumberOfPolylines || 0 == ulTotalPointsCount)
			return SetError();

		unsigned int* pPolylinePointCount = new unsigned int[ulNumberOfPolylines];

		std::vector<std::vector<T>> arPoints;
		arPoints.resize(ulNumberOfPolylines);

		for (unsigned int ulIndex = 0; ulIndex < ulNumberOfPolylines; ulIndex++)
		{
			m_oStream >> pPolylinePointCount[ulIndex];
			arPoints[ulIndex].resize(pPolylinePointCount[ulIndex]);
		}

		for (unsigned int unPolylineIndex = 0; unPolylineIndex < ulNumberOfPolylines; ++unPolylineIndex)
			for (unsigned int unPointIndex = 0; unPointIndex < arPoints[unPolylineIndex].size(); ++unPointIndex)
				m_oStream >> arPoints[unPolylineIndex][unPointIndex];

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_POLYPOLYLINE(oBounds, arPoints);

		delete[] pPolylinePointCount;
	}

	void CEmfParser::Read_EMR_POLYTEXTOUTA()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfPolyTextoutA oText;

		//TODO: сделать сохранение в XML

		m_oStream >> oText;

		if (0 == oText.cStrings)
			return;

		if (!oText.aEmrText)
			return SetError();

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
		{
			for (unsigned int unIndex = 0; unIndex < oText.cStrings; unIndex++)
			{
				DrawTextA(oText.aEmrText[unIndex], oText.iGraphicsMode);
			}
		}
	}

	void CEmfParser::Read_EMR_POLYTEXTOUTW()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfPolyTextoutW oText;

		//TODO: сделать сохранение в XML
		m_oStream >> oText;

		if (0 == oText.cStrings)
			return;

		if (!oText.wEmrText)
			return SetError();

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
		{
			for (unsigned int unIndex = 0; unIndex < oText.cStrings; unIndex++)
			{
				DrawTextA(oText.wEmrText[unIndex], oText.iGraphicsMode);
			}
		}
	}

	void CEmfParser::Read_EMR_RECTANGLE()
	{
		TEmfRectL oBox;

		m_oStream >> oBox;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_RECTANGLE(oBox);
	}

	void CEmfParser::Read_EMR_ROUNDRECT()
	{
		TEmfRectL oBox;
		TEmfSizeL oCorner;

		m_oStream >> oBox;
		m_oStream >> oCorner;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_ROUNDRECT(oBox, oCorner);
	}

	void CEmfParser::Read_EMR_SETPIXELV()
	{
		TEmfPointL oPoint;
		TEmfColor oColor;

		m_oStream >> oPoint;
		m_oStream >> oColor;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SETPIXELV(oPoint, oColor);
	}

	void CEmfParser::Read_EMR_SMALLTEXTOUT()
	{
		TEmfSmallTextout oText;

		m_oStream >> oText;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_SMALLTEXTOUT(oText);
	}

	void CEmfParser::Read_EMR_STROKEANDFILLPATH()
	{
		TEmfRectL oBounds;

		m_oStream >> oBounds;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_STROKEANDFILLPATH(oBounds);
	}

	void CEmfParser::Read_EMR_STROKEPATH()
	{
		TEmfRectL oBounds;

		m_oStream >> oBounds;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_STROKEPATH(oBounds);
	}

	void CEmfParser::Read_EMR_COMMENT()
	{
		m_oStream.Skip(4);

		std::string sType = std::string((char*)m_oStream.GetCurPtr(), 4);
		m_oStream.Skip(4);

		if (sType == "EMF+" && NULL != m_pInterpretator)
		{
			if (NULL == m_pEmfPlusParser)
			{
				m_pEmfPlusParser = new CEmfPlusParser(m_pInterpretator, m_oHeader);
				m_pEmfPlusParser->SetFontManager(GetFontManager());
			}

			m_pEmfPlusParser->SetStream(m_oStream.GetCurPtr(), m_ulRecordSize - 8);

			m_pInterpretator->ChangeConditional();

			m_pEmfPlusParser->PlayFile();

			m_pInterpretator->ChangeConditional();

			m_oStream.Skip(m_ulRecordSize - 8);
		}
		else if (sType == "GDIC")
		{
			unsigned int unPublicCommentIdentifier;

			m_oStream >> unPublicCommentIdentifier;

			if (EMR_COMMENT_WINDOWS_METAFILE == unPublicCommentIdentifier)
			{
				m_oStream.Skip(12); // Version, Reserved, Checksum, Flags

				unsigned int unWinMetafileSize;

				m_oStream >> unWinMetafileSize;

				if (0 == unWinMetafileSize)
					return;

				CWmfParser oWmfParser;

				oWmfParser.SetFontManager(GetFontManager());
				oWmfParser.SetStream(m_oStream.GetCurPtr(), unWinMetafileSize);
				oWmfParser.Scan();

				if (!oWmfParser.CheckError())
				{
					if (NULL != m_pInterpretator && InterpretatorType::Render == m_pInterpretator->GetType())
					{
						CMetaFileRenderer oWmfOut(&oWmfParser, ((CEmfInterpretatorRender*)m_pInterpretator)->GetRenderer());
						oWmfParser.SetInterpretator(&oWmfOut);

						oWmfParser.PlayFile();

						m_bEof = true;
					}
					else if (NULL != m_pInterpretator && InterpretatorType::Svg == m_pInterpretator->GetType())
					{
						double dWidth, dHeight;

						((CEmfInterpretatorSvg*)m_pInterpretator)->GetSize(dWidth, dHeight);

						((CWmfParserBase*)&oWmfParser)->SetInterpretator(InterpretatorType::Svg, dWidth, dHeight);

						oWmfParser.PlayFile();

						((CEmfInterpretatorSvg*)m_pInterpretator)->SetXmlWriter(((CWmfInterpretatorSvg*)oWmfParser.GetInterpretator())->GetXmlWriter());

						m_bEof = true;
					}
				}


				m_oStream.Skip(unWinMetafileSize);
			}
		}
	}

	void CEmfParser::Read_EMR_FILLRGN()
	{
		TEmfRectL oBounds;
		unsigned int unRgnDataSize, unIhBrush;

		m_oStream >> oBounds;
		m_oStream >> unRgnDataSize;
		m_oStream >> unIhBrush;

		if (unRgnDataSize <= 32)
			return;

		TRegionDataHeader oRegionDataHeader;

		m_oStream >> oRegionDataHeader;

		if (0x00000020 != oRegionDataHeader.unSize || 0x00000001 != oRegionDataHeader.unType || 0 == oRegionDataHeader.unCountRects)
			return;

		std::vector<TEmfRectL> arRects(oRegionDataHeader.unCountRects);

		for (TEmfRectL &oRect : arRects)
			m_oStream >> oRect;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_FILLRGN(oBounds, unIhBrush, oRegionDataHeader, arRects);
	}

	void CEmfParser::Read_EMR_PAINTRGN()
	{
		TEmfRectL oBounds;
		unsigned int unRgnDataSize;

		m_oStream >> oBounds;
		m_oStream >> unRgnDataSize;

		if (unRgnDataSize <= 32)
			return;

		TRegionDataHeader oRegionDataHeader;

		m_oStream >> oRegionDataHeader;

		if (0x00000020 != oRegionDataHeader.unSize || 0x00000001 != oRegionDataHeader.unType || 0 == oRegionDataHeader.unCountRects)
			return;

		std::vector<TEmfRectL> arRects(oRegionDataHeader.unCountRects);

		for (TEmfRectL &oRect : arRects)
			m_oStream >> oRect;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_PAINTRGN(oBounds, oRegionDataHeader, arRects);
	}

	void CEmfParser::Read_EMR_FRAMERGN()
	{
		TEmfRectL oBounds;
		unsigned int unRgnDataSize, unIhBrush;
		int nWidth, nHeight;

		m_oStream >> oBounds;
		m_oStream >> unRgnDataSize;

		if (unRgnDataSize <= 32)
			return;

		m_oStream >> unIhBrush;
		m_oStream >> nWidth;
		m_oStream >> nHeight;

		TRegionDataHeader oRegionDataHeader;

		m_oStream >> oRegionDataHeader;

		if (0x00000020 != oRegionDataHeader.unSize || 0x00000001 != oRegionDataHeader.unType || 0 == oRegionDataHeader.unCountRects)
			return;

		std::vector<TEmfRectL> arRects(oRegionDataHeader.unCountRects);

		for (TEmfRectL &oRect : arRects)
			m_oStream >> oRect;

		if (NULL == m_pEmfPlusParser || !m_pEmfPlusParser->GetBanEMFProcesses())
			HANDLE_EMR_FRAMERGN(oBounds, unIhBrush, nWidth, nHeight, oRegionDataHeader, arRects);
	}
}
