#include "CEmfxParser.h"

#include <iostream>

namespace MetaFile
{
	CEmfxParser::CEmfxParser():
		m_pOutput(new CXmlOutput(TypeXmlOutput::IsReader)){}

	CEmfxParser::~CEmfxParser()
	{
		delete m_pOutput;
	}

	bool CEmfxParser::ReadFromBuffer(BYTE* pBuffer, unsigned int unSize, const bool& bIsExternal)
	{
		return false;
	}
	bool CEmfxParser::OpenFromFile(const wchar_t *wsFilePath)
	{
		return m_pOutput->ReadFromFile(wsFilePath);
	}

	void CEmfxParser::PlayFile()
	{
		bool bEOF = false;

		if (NULL != m_pInterpretator)
			m_pInterpretator->Begin();

		m_pOutput->ReadNextRecord();

		do
		{
			const std::wstring wsRecordName = m_pOutput->GetName();

			if (wsRecordName.size() < 7 || wsRecordName.substr(0, 4) != L"EMR_")
				return SetError();

			switch (wsRecordName[4])
			{
			case 'A':
			{
				switch ((*(wsRecordName.end() - 4)))
				{
				case L'_': Read_EMR_ARC();         break;
				case L'E': Read_EMR_ANGLEARC();    break;
				case L'L': Read_EMR_ALPHABLEND();  break;
				case L'P': Read_EMR_ABORTPATH();   break;
				case L'R': Read_EMR_ARCTO();       break;
				}
				break;
			}
			case 'B':
			{
				(wsRecordName.back() == L'T') ? Read_EMR_BITBLT()
											  : Read_EMR_BEGINPATH();
				break;
			}
			case 'C':
			{
				switch ((*(wsRecordName.end() - 3)))
				{
				case L'E': Read_EMR_CREATEBRUSHINDIRECT();    break;
				case L'H': Read_EMR_CREATEDIBPATTERNBRUSHPT();break;
				case L'O': Read_EMR_CHORD();                  break;
				case L'P': Read_EMR_CREATEPEN();              break;
				case L'T': Read_EMR_CREATEPALETTE();          break;
				case L'U':
				{
					if ((*(wsRecordName.end() - 1)) == 'E')
						Read_EMR_CLOSEFIGURE();
					else
						Read_EMR_CREATEMONOBRUSH();
					break;
				}
				}
				break;
			}
			case 'D': Read_EMR_DELETEOBJECT(); break;
			case 'E':
			{
				switch (wsRecordName.back())
				{
				case L'A': Read_EMR_EXTTEXTOUTA();          break;
				case L'E': Read_EMR_ELLIPSE();              break;
				case L'F': Read_EMR_EOF(); bEOF = true;     break;
				case L'H': Read_EMR_ENDPATH();              break;
				case L'N': ((*(wsRecordName.end() - 2)) == L'E') ? Read_EMR_EXTCREATEPEN()
																 : Read_EMR_EXTSELECTCLIPRGN(); break;
				case L'T': Read_EMR_EXCLUDECLIPRECT();      break;
				case L'W': ((*(wsRecordName.end() - 3)) == L'U') ? Read_EMR_EXTTEXTOUTW()
																 : Read_EMR_EXTCREATEFONTINDIRECTW(); break;
				}
				break;
			}
			case 'F':
			{
				(wsRecordName[5] == L'I') ? ((wsRecordName.back() == L'H') ? Read_EMR_FILLPATH() : Read_EMR_FILLRGN())
										  : Read_EMR_FLATTENPATH();
				break;
			}
			case 'H': Read_EMR_HEADER();            break;
			case 'I': Read_EMR_INTERSECTCLIPRECT(); break;
			case 'L': Read_EMR_LINETO();            break;
			case 'M':
			{
				(wsRecordName[6] == L'V') ? Read_EMR_MOVETOEX()
										  : Read_EMR_MODIFYWORLDTRANSFORM();
				break;
			}
			case 'P':
			{
				if (wsRecordName[5] == L'I')
					Read_EMR_PIE();
				else
				{
					switch (wsRecordName[8])
					{
					case 'B':
					{
						switch (wsRecordName.back())
						{
						case 'R': Read_EMR_POLYBEZIER();       break;
						case 'O': Read_EMR_POLYBEZIERTO();     break;
						case '6': ((*(wsRecordName.end() - 3)) == L'R')
									? Read_EMR_POLYBEZIER16()
									: Read_EMR_POLYBEZIERTO16(); break;
						}
						break;
					}
					case 'D':
					{
						(wsRecordName.back() == L'W') ? Read_EMR_POLYDRAW()
													  : Read_EMR_POLYDRAW16();
						break;
					}
					case 'G':
					{
						(wsRecordName.back() == L'N') ? Read_EMR_POLYGON() :
														Read_EMR_POLYGON16();
						break;
					}
					case 'L':
					{
						switch (wsRecordName.back())
						{
						case 'E': Read_EMR_POLYLINE();      break;
						case 'O': Read_EMR_POLYLINETO();    break;
						case '6': ((*(wsRecordName.end() - 3)) == L'E')
									? Read_EMR_POLYLINE16()
									: Read_EMR_POLYLINETO16();break;
						}
						break;
					}
					case 'P':
					{
						switch (wsRecordName.back())
						{
						case 'E': Read_EMR_POLYPOLYLINE();  break;
						case 'N': Read_EMR_POLYPOLYGON();   break;
						case '6': ((*(wsRecordName.end() - 3)) == L'E')
									? Read_EMR_POLYPOLYLINE16()
									: Read_EMR_POLYPOLYGON16(); break;
						}
						break;
					}
					case 'T':
					{
						(wsRecordName.back() == L'W') ? Read_EMR_POLYTEXTOUTW()
													  : Read_EMR_POLYTEXTOUTA();
						break;
					}
					}
				}
				break;
			}
			case 'R':
			{
				switch (wsRecordName[6])
				{
				case 'A': Read_EMR_REALIZEPALETTE();    break;
				case 'C': Read_EMR_RECTANGLE();         break;
				case 'S': Read_EMR_RESTOREDC();         break;
				case 'U': Read_EMR_ROUNDRECT();         break;
				}
				break;
			}
			case 'S':
			{
				if (wsRecordName[6] == L'T')
				{
					switch (wsRecordName[7])
					{
					case 'A': Read_EMR_SETARCDIRECTION(); break;
					case 'B':
					{
						switch (wsRecordName.back())
						{
						case 'E': Read_EMR_SETBKMODE();     break;
						case 'R': Read_EMR_SETBKCOLOR();    break;
						case 'X': Read_EMR_SETBRUSHORGEX(); break;
						}
						break;
					}
					case 'D': Read_EMR_SETDIBITSTODEVICE(); break;
					case 'I': Read_EMR_SETICMMODE();        break;
					case 'L': Read_EMR_SETLAYOUT();         break;
					case 'M':
					{
						switch (wsRecordName.back())
						{
						case 'E': Read_EMR_SETMAPMODE();    break;
						case 'N': Read_EMR_SETMETARGN();    break;
						case 'T': Read_EMR_SETMITERLIMIT(); break;
						}
						break;
					}
					case 'P':
					{
						(wsRecordName.back() == L'V') ? Read_EMR_SETPIXELV()
													  : Read_EMR_SETPOLYFILLMODE();
						break;
					}
					case 'R': Read_EMR_SETROP2();           break;
					case 'S': Read_EMR_SETSTRETCHBLTMODE(); break;
					case 'T':
					{
						(wsRecordName.back() == L'R') ? Read_EMR_SETTEXTCOLOR()
													  : Read_EMR_SETTEXTALIGN();
						break;
					}
					case 'V':
					{
						((*(wsRecordName.end() - 3)) == L'G') ? Read_EMR_SETVIEWPORTORGEX()
															  : Read_EMR_SETVIEWPORTEXTEX();
						break;
					}
					case 'W':
					{
						switch (*(wsRecordName.end() - 3))
						{
						case 'O': Read_EMR_SETWORLDTRANSFORM(); break;
						case 'G': Read_EMR_SETWINDOWORGEX();    break;
						case 'T': Read_EMR_SETWINDOWEXTEX();    break;
						}
						break;
					}

					}
				}
				else
				{
					switch (*(wsRecordName.end() - 4))
					{
					case 'B': Read_EMR_STRETCHDIBITS(); break;
					case 'E': Read_EMR_SELECTPALETTE(); break;
					case 'H': Read_EMR_STRETCHBLT();    break;
					case 'J': Read_EMR_SELECTOBJECT();  break;
					case 'P':
					{
						switch (*(wsRecordName.end() - 5))
						{
						case 'E': Read_EMR_STROKEPATH();        break;
						case 'L': Read_EMR_STROKEANDFILLPATH(); break;
						case 'P': Read_EMR_SELECTCLIPPATH();    break;
						}
						break;
					}
					case 'T': Read_EMR_SMALLTEXTOUT();  break;
					case 'V': Read_EMR_SAVEDC();        break;
					}
				}
				break;
			}
			case 'U': Read_EMR_UNKNOWN(); break;
			case 'W': Read_EMR_WIDENPATH(); break;
			}
		}while (!CheckError() && !bEOF && m_pOutput->ReadNextRecord());

		if (!CheckError())
			m_pOutput->MoveToStart();
	}

	void CEmfxParser::Scan()
	{
		m_pOutput->ReadNextNode();

		if (m_pOutput->GetName() != L"EMF")
			return SetError();

		m_pOutput->ReadNextRecord();

		if (m_pOutput->GetName() != L"EMR_HEADER")
			return SetError();

		*m_pOutput >> m_oHeader;

		HANDLE_EMR_HEADER(m_oHeader);

		m_pOutput->MoveToStart();
	}

	EmfParserType CEmfxParser::GetType() const
	{
		return EmfParserType::EmfxParser;
	}

	bool CEmfxParser::ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE **ppBgraBuffer, unsigned int *pulWidth, unsigned int *pulHeight)
	{
		int lHeaderOffset         = offBmi - ulSkip;
		unsigned int ulHeaderSize = cbBmi;
		int lBitsOffset           = offBits - offBmi - cbBmi;
		unsigned int ulBitsSize   = cbBits;

		if (ulHeaderSize <= 0 || ulBitsSize <= 0 || lHeaderOffset < 0 || lBitsOffset < 0)
			return false;

		m_oStream.Skip(lHeaderOffset);

		BYTE* pHeaderBuffer = m_oStream.GetCurPtr();
		m_oStream.Skip(ulHeaderSize + lBitsOffset);
		BYTE* pBitsBuffer = m_oStream.GetCurPtr();

		unsigned int ulBitsSizeSkip = 0 == ulBitsSize ? 0 : ((int)(((double)ulBitsSize - 0.5) / 4) + 1) * 4;
		m_oStream.Skip(ulBitsSizeSkip);

		MetaFile::ReadImage(pHeaderBuffer, ulHeaderSize, pBitsBuffer, ulBitsSize, ppBgraBuffer, pulWidth, pulHeight);

		return true;
	}

	void CEmfxParser::Read_EMR_HEADER()
	{
		*m_pOutput >> m_oHeader;

		HANDLE_EMR_HEADER(m_oHeader);
	}

	void CEmfxParser::Read_EMR_ALPHABLEND()
	{
		TEmfAlphaBlend oBitmap;

		*m_pOutput >> oBitmap;

		if (oBitmap.unCbBitsSrc > 0)
			*m_pOutput >> m_oStream;

		HANDLE_EMR_ALPHABLEND(oBitmap);
	}

	void CEmfxParser::Read_EMR_STRETCHDIBITS()
	{
		TEmfStretchDIBITS oBitmap;

		*m_pOutput >> oBitmap;

		if (oBitmap.unCbBitsSrc > 0)
			*m_pOutput >> m_oStream;

		HANDLE_EMR_STRETCHDIBITS(oBitmap);
	}

	void CEmfxParser::Read_EMR_BITBLT()
	{
		TEmfBitBlt oBitmap;

		*m_pOutput >> oBitmap;

		if (oBitmap.unCbBitsSrc > 0)
			*m_pOutput >> m_oStream;

		HANDLE_EMR_BITBLT(oBitmap);
	}

	void CEmfxParser::Read_EMR_SETDIBITSTODEVICE()
	{
		TEmfSetDiBitsToDevice oBitmap;

		*m_pOutput >> oBitmap;

		if (oBitmap.unCbBitsSrc > 0)
			*m_pOutput >> m_oStream;

		HANDLE_EMR_SETDIBITSTODEVICE(oBitmap);
	}

	void CEmfxParser::Read_EMR_STRETCHBLT()
	{
		TEmfStretchBLT oBitmap;

		*m_pOutput >> oBitmap;

		if (oBitmap.unCbBitsSrc > 0)
			*m_pOutput >> m_oStream;

		HANDLE_EMR_STRETCHBLT(oBitmap);
	}

	void CEmfxParser::Read_EMR_EOF()
	{
		unsigned int ulCount, ulOffset, ulSizeLast;

		*m_pOutput >> ulCount;
		*m_pOutput >> ulOffset;
		*m_pOutput >> ulSizeLast;

		HANDLE_EMR_EOF();
	}

	void CEmfxParser::Read_EMR_UNKNOWN()
	{
		*m_pOutput >> m_oStream;
		m_oStream.Skip(8);
		HANDLE_EMR_UNKNOWN(0);
		//                m_pOutput->ReadNextRecord();
	}

	void CEmfxParser::Read_EMR_FILLRGN()
	{
		TRectL oBounds, oRegionBounds;
		unsigned int unIhBrush, unCountRects;

		std::vector<TRectL> arRects;

		*m_pOutput >> oBounds;
		*m_pOutput >> unIhBrush;

		*m_pOutput >> unCountRects;
		*m_pOutput >> oRegionBounds;

		*m_pOutput >> arRects;

		TRegionDataHeader oRegionDataHeader{0x00000020, 0x00000001, unCountRects, unCountRects * 16, oRegionBounds};

		HANDLE_EMR_FILLRGN(oBounds, unIhBrush, oRegionDataHeader, arRects);
	}

	void CEmfxParser::Read_EMR_SAVEDC()
	{
		HANDLE_EMR_SAVEDC();
	}

	void CEmfxParser::Read_EMR_RESTOREDC()
	{
		int lSavedDC;

		*m_pOutput >> lSavedDC;

		HANDLE_EMR_RESTOREDC(lSavedDC);
	}

	void CEmfxParser::Read_EMR_MODIFYWORLDTRANSFORM()
	{
		TEmfXForm oXForm;
		unsigned int ulMode;

		*m_pOutput >> oXForm;
		*m_pOutput >> ulMode;

		HANDLE_EMR_MODIFYWORLDTRANSFORM(oXForm, ulMode);
	}

	void CEmfxParser::Read_EMR_SETWORLDTRANSFORM()
	{
		TEmfXForm oXForm;

		*m_pOutput >> oXForm;

		HANDLE_EMR_SETWORLDTRANSFORM(oXForm);
	}

	void CEmfxParser::Read_EMR_CREATEBRUSHINDIRECT()
	{
		unsigned int ulBrushIndex;
		CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();
		if (!pBrush)
			return SetError();

		*m_pOutput >> ulBrushIndex;
		*m_pOutput >> *pBrush;

		HANDLE_EMR_CREATEBRUSHINDIRECT(ulBrushIndex, pBrush);
	}

	void CEmfxParser::Read_EMR_SETTEXTCOLOR()
	{
		TRGBA oColor;

		*m_pOutput >> oColor;

		HANDLE_EMR_SETTEXTCOLOR(oColor);
	}

	void CEmfxParser::Read_EMR_SELECTOBJECT()
	{
		unsigned int ulObjectIndex;

		*m_pOutput >> ulObjectIndex;

		HANDLE_EMR_SELECTOBJECT(ulObjectIndex);
	}

	void CEmfxParser::Read_EMR_EXTCREATEFONTINDIRECTW()
	{
		//TODO: переделать чтение из-за отсутствия m_ulRecordSize
		unsigned int unSize = m_ulRecordSize - 4;
		bool bFixedLength = unSize <= 0x0140 ? true : false;

		unsigned int ulIndex;
		CEmfLogFont* pFont = new CEmfLogFont(bFixedLength);
		if (!pFont)
			return SetError();

		*m_pOutput >> ulIndex;
		*m_pOutput >> *pFont;

		HANDLE_EMR_EXTCREATEFONTINDIRECTW(ulIndex, pFont);
	}

	void CEmfxParser::Read_EMR_SETTEXTALIGN()
	{
		unsigned int ulAlign;

		*m_pOutput >> ulAlign;

		HANDLE_EMR_SETTEXTALIGN(ulAlign);
	}

	void CEmfxParser::Read_EMR_SETBKMODE()
	{
		unsigned int ulBgMode;

		*m_pOutput >> ulBgMode;

		HANDLE_EMR_SETBKMODE(ulBgMode);
	}

	void CEmfxParser::Read_EMR_DELETEOBJECT()
	{
		unsigned int ulIndex;

		*m_pOutput >> ulIndex;

		HANDLE_EMR_DELETEOBJECT(ulIndex);
	}

	void CEmfxParser::Read_EMR_SETMITERLIMIT()
	{
		unsigned int ulMiterLimit;

		*m_pOutput >> ulMiterLimit;

		HANDLE_EMR_SETMITERLIMIT(ulMiterLimit);
	}

	void CEmfxParser::Read_EMR_EXTCREATEPEN()
	{
		//TODO: переделать чтение из-за отсутствия m_ulRecordSize

		unsigned int ulPenIndex;

		*m_pOutput >> ulPenIndex;

		unsigned int current_size = m_ulRecordSize - 20;

		CEmfLogPen* pPen = new CEmfLogPen();
		if (!pPen)
			return SetError();

		std::vector<unsigned int> arUnused(2);

		m_pOutput->ReadNextNode();

		*m_pOutput >> pPen->unPenStyle;
		*m_pOutput >> pPen->unWidth;
		*m_pOutput >> arUnused[0];
		*m_pOutput >> pPen->oColor;
		*m_pOutput >> arUnused[1];

		*m_pOutput >> pPen->unNumStyleEntries;

		current_size -= 24;
		if (pPen->unNumStyleEntries > 0)
		{
			current_size -= pPen->unNumStyleEntries * 4;
			pPen->pStyleEntry = new unsigned int[pPen->unNumStyleEntries];
			if (!pPen->pStyleEntry)
			{
				delete pPen;
				return SetError();
			}

			for (unsigned int ulIndex = 0; ulIndex < pPen->unNumStyleEntries; ulIndex++)
				*m_pOutput >> pPen->pStyleEntry[ulIndex];
		}
		else
		{
			pPen->pStyleEntry = NULL;
		}

		HANDLE_EMR_EXTCREATEPEN(ulPenIndex, pPen, arUnused);
	}

	void CEmfxParser::Read_EMR_CREATEPEN()
	{
		unsigned int ulPenIndex;

		*m_pOutput >> ulPenIndex;

		CEmfLogPen* pPen = new CEmfLogPen();
		if (!pPen)
			return SetError();

		m_pOutput->ReadNextNode();

		*m_pOutput >> pPen->unPenStyle;

		unsigned int widthX;

		*m_pOutput >> widthX;
		*m_pOutput >> pPen->oColor;

		HANDLE_EMR_CREATEPEN(ulPenIndex, widthX, pPen);
	}

	void CEmfxParser::Read_EMR_SETPOLYFILLMODE()
	{
		unsigned int ulFillMode;

		*m_pOutput >> ulFillMode;

		HANDLE_EMR_SETPOLYFILLMODE(ulFillMode);
	}

	void CEmfxParser::Read_EMR_BEGINPATH()
	{
		HANDLE_EMR_BEGINPATH();
	}

	void CEmfxParser::Read_EMR_ENDPATH()
	{
		HANDLE_EMR_ENDPATH();
	}

	void CEmfxParser::Read_EMR_CLOSEFIGURE()
	{
		HANDLE_EMR_CLOSEFIGURE();
	}

	void CEmfxParser::Read_EMR_FLATTENPATH()
	{
		HANDLE_EMR_FLATTENPATH();
	}

	void CEmfxParser::Read_EMR_WIDENPATH()
	{
		HANDLE_EMR_WIDENPATH();
	}

	void CEmfxParser::Read_EMR_ABORTPATH()
	{
		HANDLE_EMR_ABORTPATH();
	}

	void CEmfxParser::Read_EMR_MOVETOEX()
	{
		TPointL oPoint;

		*m_pOutput >> oPoint;

		HANDLE_EMR_MOVETOEX(oPoint);
	}

	void CEmfxParser::Read_EMR_SETARCDIRECTION()
	{
		unsigned int unDirection;

		*m_pOutput >> unDirection;

		HANDLE_EMR_SETARCDIRECTION(unDirection);
	}

	void CEmfxParser::Read_EMR_FILLPATH()
	{
		TRectL oBounds;

		*m_pOutput >> oBounds;

		HANDLE_EMR_FILLPATH(oBounds);
	}

	void CEmfxParser::Read_EMR_SETMAPMODE()
	{
		unsigned int ulMapMode;

		*m_pOutput >> ulMapMode;

		HANDLE_EMR_SETMAPMODE(ulMapMode);
	}

	void CEmfxParser::Read_EMR_SETWINDOWORGEX()
	{
		TPointL oOrigin;

		*m_pOutput >> oOrigin;

		HANDLE_EMR_SETWINDOWORGEX(oOrigin);
	}

	void CEmfxParser::Read_EMR_SETWINDOWEXTEX()
	{
		TSizeL oExtent;

		*m_pOutput >> oExtent;

		HANDLE_EMR_SETWINDOWEXTEX(oExtent);
	}

	void CEmfxParser::Read_EMR_SETVIEWPORTORGEX()
	{
		TPointL oOrigin;

		*m_pOutput >> oOrigin;

		HANDLE_EMR_SETVIEWPORTORGEX(oOrigin);
	}

	void CEmfxParser::Read_EMR_SETVIEWPORTEXTEX()
	{
		TSizeL oExtent;

		*m_pOutput >> oExtent;

		HANDLE_EMR_SETVIEWPORTEXTEX(oExtent);
	}

	void CEmfxParser::Read_EMR_SETSTRETCHBLTMODE()
	{
		unsigned int ulStretchMode;

		*m_pOutput >> ulStretchMode;

		HANDLE_EMR_SETSTRETCHBLTMODE(ulStretchMode);
	}

	void CEmfxParser::Read_EMR_SETICMMODE()
	{
		unsigned int ulICMMode;

		*m_pOutput >> ulICMMode;

		HANDLE_EMR_SETICMMODE(ulICMMode);
	}

	void CEmfxParser::Read_EMR_CREATEMONOBRUSH()
	{
		unsigned int ulBrushIndex;
		TEmfDibPatternBrush oDibBrush;

		*m_pOutput >> ulBrushIndex;
		*m_pOutput >> oDibBrush;

		if (oDibBrush.unCbBits > 0)
			*m_pOutput >> m_oStream;

		HANDLE_EMR_CREATEMONOBRUSH(ulBrushIndex, oDibBrush);
	}

	void CEmfxParser::Read_EMR_CREATEDIBPATTERNBRUSHPT()
	{
		unsigned int ulBrushIndex;
		TEmfDibPatternBrush oDibBrush;

		*m_pOutput >> ulBrushIndex;
		*m_pOutput >> oDibBrush;

		if (oDibBrush.unCbBits > 0)
			*m_pOutput >> m_oStream;

		HANDLE_EMR_CREATEDIBPATTERNBRUSHPT(ulBrushIndex, oDibBrush);
	}

	void CEmfxParser::Read_EMR_SELECTCLIPPATH()
	{
		unsigned int unRegionMode;

		*m_pOutput >> unRegionMode;

		HANDLE_EMR_SELECTCLIPPATH(unRegionMode);
	}

	void CEmfxParser::Read_EMR_SETBKCOLOR()
	{
		TRGBA oColor;

		*m_pOutput >> oColor;

		HANDLE_EMR_SETBKCOLOR(oColor);
	}

	void CEmfxParser::Read_EMR_EXCLUDECLIPRECT()
	{
		// TODO: Проверить как найдется файл
		TRectL oClip;

		*m_pOutput >> oClip;

		HANDLE_EMR_EXCLUDECLIPRECT(oClip);
	}

	void CEmfxParser::Read_EMR_EXTSELECTCLIPRGN()
	{
		unsigned int ulRgnDataSize, ulRegionMode;

		*m_pOutput >> ulRgnDataSize;
		*m_pOutput >> ulRegionMode;

		HANDLE_EMR_EXTSELECTCLIPRGN(ulRgnDataSize, ulRegionMode);
	}

	void CEmfxParser::Read_EMR_SETMETARGN()
	{
		HANDLE_EMR_SETMETARGN();
	}

	void CEmfxParser::Read_EMR_SETROP2()
	{
		unsigned int ulRop2Mode;

		*m_pOutput >> ulRop2Mode;

		HANDLE_EMR_SETROP2(ulRop2Mode);
	}

	void CEmfxParser::Read_EMR_CREATEPALETTE()
	{
		unsigned int ulPaletteIndex;
		CEmfLogPalette* pPalette = new CEmfLogPalette();
		if (!pPalette)
			return SetError();

		*m_pOutput >> ulPaletteIndex;
		*m_pOutput >> *pPalette;

		HANDLE_EMR_CREATEPALETTE(ulPaletteIndex, pPalette);
	}

	void CEmfxParser::Read_EMR_SELECTPALETTE()
	{
		unsigned int ulIndex;

		*m_pOutput >> ulIndex;

		HANDLE_EMR_SELECTPALETTE(ulIndex);
	}

	void CEmfxParser::Read_EMR_REALIZEPALETTE()
	{
		HANDLE_EMR_REALIZEPALETTE();
	}

	void CEmfxParser::Read_EMR_INTERSECTCLIPRECT()
	{
		TRectL oClip;

		*m_pOutput >> oClip;

		HANDLE_EMR_INTERSECTCLIPRECT(oClip);
	}

	void CEmfxParser::Read_EMR_SETLAYOUT()
	{
		unsigned int ulLayoutMode;

		*m_pOutput >> ulLayoutMode;

		HANDLE_EMR_SETLAYOUT(ulLayoutMode);
	}

	void CEmfxParser::Read_EMR_SETBRUSHORGEX()
	{
		TPointL oOrigin;

		*m_pOutput >> oOrigin;

		HANDLE_EMR_SETBRUSHORGEX(oOrigin);
	}

	void CEmfxParser::Read_EMR_ANGLEARC()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TPointL oCenter;
		unsigned int unRadius;
		double dStartAngle, dSweepAngle;

		*m_pOutput >> oCenter;
		*m_pOutput >> unRadius;

		*m_pOutput >> dStartAngle;
		*m_pOutput >> dSweepAngle;

		HANDLE_EMR_ANGLEARC(oCenter, unRadius, dStartAngle, dSweepAngle);
	}

	void CEmfxParser::Read_EMR_ARC_BASE(TRectL &oBox, TPointL &oStart, TPointL &oEnd)
	{
		*m_pOutput >> oBox;
		*m_pOutput >> oStart;
		*m_pOutput >> oEnd;
	}

	void CEmfxParser::Read_EMR_ARC()
	{
		TRectL oBox;
		TPointL oStart, oEnd;

		Read_EMR_ARC_BASE(oBox, oStart, oEnd);
		HANDLE_EMR_ARC(oBox, oStart, oEnd);
	}

	void CEmfxParser::Read_EMR_ARCTO()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TRectL oBox;
		TPointL oStart, oEnd;

		Read_EMR_ARC_BASE(oBox, oStart, oEnd);
		HANDLE_EMR_ARCTO(oBox, oStart, oEnd);
	}

	void CEmfxParser::Read_EMR_CHORD()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TRectL oBox;
		TPointL oStart, oEnd;

		Read_EMR_ARC_BASE(oBox, oStart, oEnd);
		HANDLE_EMR_CHORD(oBox, oStart, oEnd);
	}

	void CEmfxParser::Read_EMR_ELLIPSE()
	{
		TRectL oBox;

		*m_pOutput >> oBox;

		HANDLE_EMR_ELLIPSE(oBox);
	}

	void CEmfxParser::Read_EMR_EXTTEXTOUTA()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfExtTextoutA oText;

		*m_pOutput >> oText;

		HANDLE_EMR_EXTTEXTOUTA(oText);
	}

	void CEmfxParser::Read_EMR_EXTTEXTOUTW()
	{
		TEmfExtTextoutW oText;

		*m_pOutput >> oText;

		HANDLE_EMR_EXTTEXTOUTW(oText);
	}

	void CEmfxParser::Read_EMR_LINETO()
	{
		TPointL oPoint;

		*m_pOutput >> oPoint;

		HANDLE_EMR_LINETO(oPoint);
	}

	void CEmfxParser::Read_EMR_PIE()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TRectL oBox;
		TPointL oStart, oEnd;

		Read_EMR_ARC_BASE(oBox, oStart, oEnd);
		HANDLE_EMR_PIE(oBox, oStart, oEnd);
	}

	void CEmfxParser::Read_EMR_POLYBEZIER()
	{
		Read_EMR_POLYBEZIER_BASE<TPointL>();
	}

	void CEmfxParser::Read_EMR_POLYBEZIER16()
	{
		Read_EMR_POLYBEZIER_BASE<TPointS>();
	}

	template<typename T>void CEmfxParser::Read_EMR_POLYBEZIER_BASE()
	{
		TRectL oBounds;
		std::vector<T> arPoints;

		*m_pOutput >> oBounds;
		*m_pOutput >> arPoints;

		HANDLE_EMR_POLYBEZIER(oBounds, arPoints);
	}

	void CEmfxParser::Read_EMR_POLYBEZIERTO()
	{
		Read_EMR_POLYBEZIERTO_BASE<TPointL>();
	}

	void CEmfxParser::Read_EMR_POLYBEZIERTO16()
	{
		Read_EMR_POLYBEZIERTO_BASE<TPointS>();
	}

	template<typename T>void CEmfxParser::Read_EMR_POLYBEZIERTO_BASE()
	{
		TRectL oBounds;
		std::vector<T> arPoints;

		*m_pOutput >> oBounds;
		*m_pOutput >> arPoints;

		HANDLE_EMR_POLYBEZIERTO(oBounds, arPoints);
	}

	void CEmfxParser::Read_EMR_POLYDRAW()
	{
		Read_EMR_POLYDRAW_BASE<TPointL>();
	}

	void CEmfxParser::Read_EMR_POLYDRAW16()
	{
		Read_EMR_POLYDRAW_BASE<TPointS>();
	}

	template<typename T>void CEmfxParser::Read_EMR_POLYDRAW_BASE()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		//bug #35006 - не прочитывается весь рекорд ... выравнивание?

		TRectL oBounds;
		unsigned int unCount;

		*m_pOutput >> oBounds;
		*m_pOutput >> unCount;

		if (0 == unCount)
			return;

		T* pPoints = new T[unCount];
		if (!pPoints)
			return SetError();

		for (unsigned int unIndex = 0; unIndex < unCount; unIndex++)
			*m_pOutput >> pPoints[unIndex];

		unsigned char* pAbTypes = new unsigned char[unCount];
		if (!pAbTypes)
		{
			delete[] pPoints;
			return SetError();
		}

		for (unsigned int unIndex = 0; unIndex < unCount; unIndex++)
			*m_pOutput >> pAbTypes[unIndex];

		HANDLE_EMR_POLYDRAW(oBounds, pPoints, unCount, pAbTypes);

		delete[] pPoints;
		delete[] pAbTypes;
	}

	void CEmfxParser::Read_EMR_POLYGON()
	{
		Read_EMR_POLYGON_BASE<TPointL>();
	}

	void CEmfxParser::Read_EMR_POLYGON16()
	{
		Read_EMR_POLYGON_BASE<TPointS>();
	}

	template<typename T>void CEmfxParser::Read_EMR_POLYGON_BASE()
	{
		TRectL oBounds;
		std::vector<T> arPoints;

		*m_pOutput >> oBounds;
		*m_pOutput >> arPoints;

		HANDLE_EMR_POLYGON(oBounds, arPoints);
	}

	void CEmfxParser::Read_EMR_POLYLINE()
	{
		Read_EMR_POLYLINE_BASE<TPointL>();
	}

	void CEmfxParser::Read_EMR_POLYLINE16()
	{
		Read_EMR_POLYLINE_BASE<TPointS>();
	}

	template<typename T>void CEmfxParser::Read_EMR_POLYLINE_BASE()
	{
		TRectL oBounds;
		std::vector<T> arPoints;

		*m_pOutput >> oBounds;
		*m_pOutput >> arPoints;

		HANDLE_EMR_POLYLINE(oBounds, arPoints);
	}

	void CEmfxParser::Read_EMR_POLYLINETO()
	{
		Read_EMR_POLYLINETO_BASE<TPointL>();
	}

	void CEmfxParser::Read_EMR_POLYLINETO16()
	{
		Read_EMR_POLYLINETO_BASE<TPointS>();
	}

	template<typename T>void CEmfxParser::Read_EMR_POLYLINETO_BASE()
	{
		TRectL oBounds;
		std::vector<T> arPoints;

		*m_pOutput >> oBounds;
		*m_pOutput >> arPoints;

		HANDLE_EMR_POLYLINETO(oBounds, arPoints);
	}

	void CEmfxParser::Read_EMR_POLYPOLYGON()
	{
		Read_EMR_POLYPOLYGON_BASE<TPointL>();
	}

	void CEmfxParser::Read_EMR_POLYPOLYGON16()
	{
		Read_EMR_POLYPOLYGON_BASE<TPointS>();
	}

	template<typename T>void CEmfxParser::Read_EMR_POLYPOLYGON_BASE()
	{
		TRectL oBounds;
		std::vector<std::vector<T>> arPoints;

		*m_pOutput >> oBounds;
		*m_pOutput >> arPoints;

		HANDLE_EMR_POLYPOLYGON(oBounds, arPoints);
	}

	void CEmfxParser::Read_EMR_POLYPOLYLINE()
	{
		Read_EMR_POLYPOLYLINE_BASE<TPointL>();
	}

	void CEmfxParser::Read_EMR_POLYPOLYLINE16()
	{
		Read_EMR_POLYPOLYLINE_BASE<TPointS>();
	}

	template<typename T>void CEmfxParser::Read_EMR_POLYPOLYLINE_BASE()
	{
		TRectL oBounds;
		std::vector<std::vector<T>> arPoints;

		*m_pOutput >> oBounds;
		*m_pOutput >> arPoints;

		HANDLE_EMR_POLYPOLYLINE(oBounds, arPoints);
	}

	void CEmfxParser::Read_EMR_POLYTEXTOUTA()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TPolyTextoutA oText;

		//TODO: сделать сохранение в XML и чтение из него
		return;

		m_oStream >> oText;

		if (0 == oText.unCStrings)
			return;

		if (!oText.arEmrText)
			return SetError();

		for (unsigned int unIndex = 0; unIndex < oText.unCStrings; unIndex++)
		{
			DrawTextA(oText.arEmrText[unIndex], oText.unIGraphicsMode);
		}
	}

	void CEmfxParser::Read_EMR_POLYTEXTOUTW()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TPolyTextoutW oText;

		//TODO: сделать сохранение в XML и чтение из него
		return;

		m_oStream >> oText;

		if (0 == oText.unCStrings)
			return;

		if (!oText.arEmrText)
			return SetError();

		for (unsigned int unIndex = 0; unIndex < oText.unCStrings; unIndex++)
		{
			DrawTextW(oText.arEmrText[unIndex], oText.unIGraphicsMode);
		}
	}

	void CEmfxParser::Read_EMR_RECTANGLE()
	{
		TRectL oBox;

		*m_pOutput >> oBox;

		HANDLE_EMR_RECTANGLE(oBox);
	}

	void CEmfxParser::Read_EMR_ROUNDRECT()
	{
		TRectL oBox;
		TSizeL oCorner;

		*m_pOutput >> oBox;
		*m_pOutput >> oCorner;

		HANDLE_EMR_ROUNDRECT(oBox, oCorner);
	}
	void CEmfxParser::Read_EMR_SETPIXELV()
	{
		TPointL oPoint;
		TRGBA oColor;

		*m_pOutput >> oPoint;
		*m_pOutput >> oColor;

		if (NULL != m_pInterpretator)
			m_pInterpretator->HANDLE_EMR_SETPIXELV(oPoint, oColor);

		// Делаем цветом кисти
		BYTE pBgraBuffer[4];
		pBgraBuffer[0] = oColor.b;
		pBgraBuffer[1] = oColor.g;
		pBgraBuffer[2] = oColor.r;
		pBgraBuffer[3] = 0xff;

		DrawImage(oPoint.X, oPoint.Y, 1, 1, pBgraBuffer, 1, 1);
	}
	void CEmfxParser::Read_EMR_SMALLTEXTOUT()
	{
		TEmfSmallTextout oText;

		*m_pOutput >> oText;

		HANDLE_EMR_SMALLTEXTOUT(oText);
	}
	void CEmfxParser::Read_EMR_STROKEANDFILLPATH()
	{
		TRectL oBounds;

		*m_pOutput >> oBounds;

		HANDLE_EMR_STROKEANDFILLPATH(oBounds);
	}
	void CEmfxParser::Read_EMR_STROKEPATH()
	{
		TRectL oBounds;

		*m_pOutput >> oBounds;

		HANDLE_EMR_STROKEPATH(oBounds);
	}
}
