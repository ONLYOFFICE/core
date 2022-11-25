#include "CWmfParser.h"

namespace MetaFile
{
	CWmfParser::CWmfParser()
	{
	}

	CWmfParser::~CWmfParser()
	{
		ClearFile();
	}

	bool CWmfParser::ReadFromBuffer(unsigned char* pBuffer, unsigned int unSize)
	{
		return IMetaFileBase::ReadFromBuffer(pBuffer, unSize);
	}
	bool CWmfParser::OpenFromFile(const wchar_t *wsFilePath)
	{
		return IMetaFileBase::OpenFromFile(wsFilePath);
	}

	void CWmfParser::PlayFile()
	{
		if (!m_oStream.IsValid())
			SetError();

		unsigned int unSize;
		unsigned short ushType;
		unsigned int ulNumber = 0;

		if (NULL != m_pInterpretator)
			m_pInterpretator->Begin();

		Read_META_HEADER();

		unsigned int unRecordIndex = 1;

		while (!CheckError())
		{
			if (m_oStream.CanRead() < 6)
				return SetError();

			m_unRecordPos = m_oStream.Tell();

			m_oStream >> unSize;
			m_oStream >> ushType;

			m_unRecordSize = unSize * 2; // Размер указан в WORD

			switch (ushType)
			{
				//-----------------------------------------------------------
				// 2.3.1 Bitmap records
				//-----------------------------------------------------------
				case META_BITBLT: Read_META_BITBLT(); break;
				case META_DIBBITBLT: Read_META_DIBBITBLT(); break;
				case META_DIBSTRETCHBLT: Read_META_DIBSTRETCHBLT(); break;
				case META_SETDIBTODEV: Read_META_SETDIBTODEV(); break;
				case META_STRETCHBLT: Read_META_STRETCHBLT(); break;
				case META_STRETCHDIB: Read_META_STRETCHDIB(); break;
					//-----------------------------------------------------------
					// 2.3.2 Control records
					//-----------------------------------------------------------
				case META_EOF: Read_META_EOF(); break;
					//-----------------------------------------------------------
					// 2.3.3 Drawing records
					//-----------------------------------------------------------
				case META_ARC: Read_META_ARC(); break;
				case META_CHORD: Read_META_CHORD(); break;
				case META_ELLIPSE: Read_META_ELLIPSE(); break;
				case META_EXTFLOODFILL: Read_META_UNKNOWN(); break;
				case META_EXTTEXTOUT: Read_META_EXTTEXTOUT(); break;
				case META_FILLREGION: Read_META_FILLREGION(); break;
				case META_FLOODFILL: Read_META_UNKNOWN(); break;
				case META_FRAMEREGION: Read_META_FRAMEREGION(); break;
				case META_INVERTREGION: Read_META_INVERTREGION(); break;
				case META_LINETO: Read_META_LINETO(); break;
				case META_PAINTREGION: Read_META_PAINTREGION(); break;
				case META_PATBLT: Read_META_PATBLT(); break;
				case META_PIE: Read_META_PIE(); break;
				case META_POLYLINE: Read_META_POLYLINE(); break;
				case META_POLYGON: Read_META_POLYGON(); break;
				case META_POLYPOLYGON: Read_META_POLYPOLYGON(); break;
				case META_RECTANGLE: Read_META_RECTANGLE(); break;
				case META_ROUNDRECT: Read_META_ROUNDRECT(); break;
				case META_SETPIXEL: Read_META_SETPIXEL(); break;
				case META_TEXTOUT: Read_META_TEXTOUT(); break;
					//-----------------------------------------------------------
					// 2.3.4 Object records
					//-----------------------------------------------------------
				case META_CREATEBRUSHINDIRECT: Read_META_CREATEBRUSHINDIRECT(); break;
				case META_CREATEFONTINDIRECT: Read_META_CREATEFONTINDIRECT(); break;
				case META_CREATEPALETTE: Read_META_CREATEPALETTE(); break;
				case META_CREATEPATTERNBRUSH: Read_META_CREATEPATTERNBRUSH(); break;
				case META_CREATEPENINDIRECT: Read_META_CREATEPENINDIRECT(); break;
				case META_CREATEREGION: Read_META_CREATEREGION(); break;
				case META_DELETEOBJECT: Read_META_DELETEOBJECT(); break;
				case META_DIBCREATEPATTERNBRUSH: Read_META_DIBCREATEPATTERNBRUSH(); break;
				case META_SELECTCLIPREGION: Read_META_SELECTCLIPREGION(); break;
				case META_SELECTOBJECT: Read_META_SELECTOBJECT(); break;
				case META_SELECTPALETTE: Read_META_SELECTPALETTE(); break;
					//-----------------------------------------------------------
					// 2.3.5 State records
					//-----------------------------------------------------------
				case META_ANIMATEPALETTE: Read_META_UNKNOWN(); break;
				case META_EXCLUDECLIPRECT: Read_META_EXCLUDECLIPRECT(); break;
				case META_INTERSECTCLIPRECT: Read_META_INTERSECTCLIPRECT(); break;
				case META_MOVETO: Read_META_MOVETO(); break;
				case META_OFFSETCLIPRGN: Read_META_OFFSETCLIPRGN(); break;
				case META_OFFSETVIEWPORTORG: Read_META_OFFSETVIEWPORTORG(); break;
				case META_OFFSETWINDOWORG: Read_META_OFFSETWINDOWORG(); break;
				case META_REALIZEPALETTE: Read_META_UNKNOWN(); break;
				case META_RESIZEPALETTE: Read_META_UNKNOWN(); break;
				case META_RESTOREDC: Read_META_RESTOREDC(); break;
				case META_SAVEDC: Read_META_SAVEDC(); break;
				case META_SCALEVIEWPORTEXT: Read_META_SCALEVIEWPORTEXT(); break;
				case META_SCALEWINDOWEXT: Read_META_SCALEWINDOWEXT(); break;
				case META_SETBKCOLOR: Read_META_SETBKCOLOR(); break;
				case META_SETBKMODE: Read_META_SETBKMODE(); break;
				case META_SETLAYOUT: Read_META_SETLAYOUT(); break;
				case META_SETMAPMODE: Read_META_SETMAPMODE(); break;
				case META_SETMAPPERFLAGS: Read_META_UNKNOWN(); break;
				case META_SETPALENTRIES: Read_META_UNKNOWN(); break;
				case META_SETPOLYFILLMODE: Read_META_SETPOLYFILLMODE(); break;
				case META_SETRELABS: Read_META_UNKNOWN(); break;
				case META_SETROP2: Read_META_SETROP2(); break;
				case META_SETSTRETCHBLTMODE: Read_META_SETSTRETCHBLTMODE(); break;
				case META_SETTEXTALIGN: Read_META_SETTEXTALIGN(); break;
				case META_SETTEXTCHAREXTRA: Read_META_SETTEXTCHAREXTRA(); break;
				case META_SETTEXTCOLOR: Read_META_SETTEXTCOLOR(); break;
				case META_SETTEXTJUSTIFICATION: Read_META_SETTEXTJUSTIFICATION(); break;
				case META_SETVIEWPORTEXT: Read_META_SETVIEWPORTEXT(); break;
				case META_SETVIEWPORTORG: Read_META_SETVIEWPORTORG(); break;
				case META_SETWINDOWEXT: Read_META_SETWINDOWEXT(); break;
				case META_SETWINDOWORG: Read_META_SETWINDOWORG(); break;
					//-----------------------------------------------------------
					// 2.3.6 State records
					//-----------------------------------------------------------
				case META_ESCAPE: Read_META_ESCAPE(); break;
					//-----------------------------------------------------------
					// Неизвестные записи
					//-----------------------------------------------------------
				default:
				{
					//std::cout << ushType << " ";
					Read_META_UNKNOWN();
					break;
				}
			}

			unRecordIndex++;

			if (m_bEof)
				break;

			// Пропускаем лишние байты, которые могли быть в записи
			int need_skip = m_unRecordSize - (m_oStream.Tell() - m_unRecordPos);
			m_oStream.Skip(need_skip);
		};

		if (!m_bEof)
			SetError();

		if (!CheckError())
			m_oStream.SeekToStart();

		if (NULL != m_pInterpretator)
			m_pInterpretator->End();

		ClearFile();
	}

	void CWmfParser::Scan()
	{
		CWmfInterpretatorBase *pInterpretator = m_pInterpretator;
		m_pInterpretator = NULL;
		PlayFile();
		m_pInterpretator = pInterpretator;
		this->ClearFile();
	}

	WmfParserType CWmfParser::GetType()
	{
		return WmfParserType::WmfParser;
	}

	void CWmfParser::SetStream(BYTE *pBuf, unsigned int unSize)
	{
		m_oStream.SetStream(pBuf, unSize);
	}

	void CWmfParser::Read_META_HEADER()
	{
		m_oStream >> m_oPlaceable.Key;
		if (0x9AC6CDD7 == m_oPlaceable.Key)
		{
			m_oStream >> m_oPlaceable.HWmf;
			m_oStream >> m_oPlaceable.BoundingBox;
			m_oStream >> m_oPlaceable.Inch;
			m_oStream >> m_oPlaceable.Reserved;
			m_oStream >> m_oPlaceable.Checksum;

			SkipVoid();
		}
		else
		{
			m_oStream.SeekBack(m_oStream.Tell());
			m_oPlaceable.Key                = 0;
			m_oPlaceable.HWmf               = 0;
			m_oPlaceable.BoundingBox.Left   = 0;
			m_oPlaceable.BoundingBox.Top    = 0;
			m_oPlaceable.BoundingBox.Right  = 0;
			m_oPlaceable.BoundingBox.Bottom = 0;
			m_oPlaceable.Inch               = 0;
			m_oPlaceable.Reserved           = 0;
			m_oPlaceable.Checksum           = 0;
		}

		m_oStream >> m_oHeader.Type;
		m_oStream >> m_oHeader.HeaderSize;
		m_oStream >> m_oHeader.Version;
		m_oStream >> m_oHeader.Size;
		m_oStream >> m_oHeader.NumberOfObjects;
		m_oStream >> m_oHeader.MaxRecord;
		m_oStream >> m_oHeader.NumberOfMembers;

		HANDLE_META_HEADER();
	}

	void CWmfParser::Read_META_BITBLT()
	{
		TWmfBitBlt oBitmap;
		m_oStream >> oBitmap;

		unsigned int unRecordSizeDword = m_unRecordSize >> 1;
		unsigned int unValue = (META_BITBLT >> 8) + 3;
		if (unRecordSizeDword == unValue)
		{
			m_oStream.Skip(2); // Reserved
		}
		else
		{
			if (m_pOutput)
			{
				TWmfBitmap16 oBitmap16;
				m_oStream >> oBitmap16;

				// TODO: Сделать чтение Bitmap16
			}
			else
			{
				RegisterPoint(oBitmap.XDest, oBitmap.YDest);
				RegisterPoint(oBitmap.XDest + oBitmap.Width, oBitmap.YDest + oBitmap.Height);
			}


			int nRemainingBytes = GetRecordRemainingBytesCount();
			if (nRemainingBytes < 0)
				return SetError();
		}

		HANDLE_META_BITBLT(oBitmap, m_oStream);
	}

	void CWmfParser::Read_META_DIBBITBLT()
	{
		TWmfBitBlt oBitmap;
		m_oStream >> oBitmap;

		HANDLE_META_DIBBITBLT(oBitmap, m_oStream);
	}

	void CWmfParser::Read_META_DIBSTRETCHBLT()
	{
		TWmfStretchBlt oBitmap;
		m_oStream >> oBitmap;

		HANDLE_META_DIBSTRETCHBLT(oBitmap, m_oStream);
	}

	void CWmfParser::Read_META_SETDIBTODEV()
	{
		TWmfSetDibToDev oBitmap;
		m_oStream >> oBitmap;

		HANDLE_META_SETDIBTODEV(oBitmap, m_oStream);
	}

	void CWmfParser::Read_META_STRETCHBLT()
	{
		TWmfStretchBlt oBitmap;
		m_oStream >> oBitmap;

		unsigned int unRecordSizeDWORD = m_unRecordSize >> 1;
		unsigned int unValue = (META_STRETCHBLT >> 8) + 3;

		if (unRecordSizeDWORD == ((META_STRETCHBLT >> 8) + 3))
		{
			m_oStream.Skip(2); // Reserved
		}
		else
		{
			if (m_pOutput)
			{
				TWmfBitmap16 oBitmap16;
				m_oStream >> oBitmap16;

				// TODO: Сделать чтение Bitmap16
			}
			else
			{
				RegisterPoint(oBitmap.XDest, oBitmap.YDest);
				RegisterPoint(oBitmap.XDest + oBitmap.DestWidth, oBitmap.YDest + oBitmap.DestHeight);
			}

			int nRemainingBytes = GetRecordRemainingBytesCount();
			if (nRemainingBytes < 0)
				return SetError();
		}

		HANDLE_META_STRETCHBLT(oBitmap, m_oStream);
	}

	void CWmfParser::Read_META_STRETCHDIB()
	{
		TWmfStretchDib oBitmap;
		m_oStream >> oBitmap;

		HANDLE_META_STRETCHDIB(oBitmap, m_oStream);
	}

	void CWmfParser::Read_META_EOF()
	{
		HANDLE_META_EOF();
	}

	void CWmfParser::Read_META_ARC()
	{
		short shYEndArc, shXEndArc, shYStartArc, shXStartArc, shBottom, shRight, shTop, shLeft;
		m_oStream >> shYEndArc >> shXEndArc >> shYStartArc >> shXStartArc >> shBottom >> shRight >> shTop >> shLeft;

		HANDLE_META_ARC(shYEndArc, shXEndArc, shYStartArc, shXStartArc, shBottom, shRight, shTop, shLeft);
	}

	void CWmfParser::Read_META_CHORD()
	{
		short shYEndArc, shXEndArc, shYStartArc, shXStartArc, shBottom, shRight, shTop, shLeft;
		m_oStream >> shYEndArc >> shXEndArc >> shYStartArc >> shXStartArc >> shBottom >> shRight >> shTop >> shLeft;

		HANDLE_META_CHORD(shYEndArc, shXEndArc, shYStartArc, shXStartArc, shBottom, shRight, shTop, shLeft);
	}

	void CWmfParser::Read_META_ELLIPSE()
	{
		short shBottom, shRight, shTop, shLeft;
		m_oStream >> shBottom >> shRight >> shTop >> shLeft;

		HANDLE_META_ELLIPSE(shBottom, shRight, shTop, shLeft);
	}

	void CWmfParser::Read_META_EXTTEXTOUT()
	{
		short shY, shX, shStringLength;
		unsigned short ushFwOptions;
		TWmfRect oRectangle;
		m_oStream >> shY >> shX >> shStringLength >> ushFwOptions;

		if (shStringLength <= 0)
			return;

		if (ushFwOptions & ETO_CLIPPED || ushFwOptions & ETO_OPAQUE)
			m_oStream >> oRectangle;

		unsigned char* pString = new unsigned char[shStringLength + 1];
		if (!pString)
			return SetError();

		pString[shStringLength] = 0x00;
		m_oStream.ReadBytes(pString, shStringLength);

		short* pDx = NULL;
		if (shStringLength > 0 && ((GetRecordRemainingBytesCount() >= shStringLength * 2 && !(ushFwOptions & ETO_PDY)) || (GetRecordRemainingBytesCount() >= shStringLength * 4 && ushFwOptions & ETO_PDY)))
		{
			if (shStringLength & 1) // Если длина нечетная, тогда пропускаем 1 байт, т.к. тут прилегание по 2 байта
				m_oStream.Skip(1);

			pDx = new short[shStringLength];
			if (pDx)
			{
				if (ushFwOptions & ETO_PDY)
				{
					for (short shIndex = 0; shIndex < shStringLength; shIndex++)
					{
						m_oStream >> pDx[shIndex];
						m_oStream.Skip(2);
					}
				}
				else
				{
					for (short shIndex = 0; shIndex < shStringLength; shIndex++)
					{
						m_oStream >> pDx[shIndex];
					}
				}
			}
		}

		HANDLE_META_EXTTEXTOUT(shY, shX, shStringLength, ushFwOptions, oRectangle, pString, pDx);

		RELEASEARRAYOBJECTS(pString);
		RELEASEARRAYOBJECTS(pDx);
	}

	void CWmfParser::Read_META_FILLREGION()
	{
		unsigned short ushRegionIndex, ushBrushIndex;
		m_oStream >> ushRegionIndex >> ushBrushIndex;

		HANDLE_META_FILLREGION(ushRegionIndex, ushBrushIndex);
	}

	void CWmfParser::Read_META_FRAMEREGION()
	{
		unsigned short ushRegionIndex, ushBrushIndex;
		short shHeight, shWidth;
		m_oStream >> ushRegionIndex >> ushBrushIndex >> shHeight >> shWidth;

		HANDLE_META_FRAMEREGION(ushRegionIndex, ushBrushIndex, shHeight, shWidth);
	}

	void CWmfParser::Read_META_INVERTREGION()
	{
		unsigned short ushRegionIndex;
		m_oStream >> ushRegionIndex;

		HANDLE_META_INVERTREGION(ushRegionIndex);
	}

	void CWmfParser::Read_META_LINETO()
	{
		short shY, shX;
		m_oStream >> shY >> shX;

		HANDLE_META_LINETO(shY, shX);
	}

	void CWmfParser::Read_META_PAINTREGION()
	{
		unsigned short ushRegionIndex;
		m_oStream >> ushRegionIndex;

		HANDLE_META_PAINTREGION(ushRegionIndex);
	}

	void CWmfParser::Read_META_PATBLT()
	{
		unsigned int unRasterOperation;
		short shX, shY, shW, shH;
		m_oStream >> unRasterOperation >> shH >> shW >> shY >> shX;

		HANDLE_META_PATBLT(unRasterOperation, shH, shW, shY, shX);
	}

	void CWmfParser::Read_META_PIE()
	{
		short shXRadial1, shYRadial1, shXRadial2, shYRadial2;
		short shL, shT, shR, shB;
		m_oStream >> shYRadial2 >> shXRadial2 >> shYRadial1 >> shXRadial1;
		m_oStream >> shB >> shR >> shT >> shL;

		HANDLE_META_PIE(shXRadial1, shYRadial1, shXRadial2, shYRadial2, shB, shR, shT, shL);
	}

	void CWmfParser::Read_META_POLYLINE()
	{
		short shNumberOfPoints;
		m_oStream >> shNumberOfPoints;
		if (shNumberOfPoints < 1)
			return;

		std::vector<TWmfPointS> arPoints(shNumberOfPoints);

		m_oStream >> arPoints[0];

		for (short shIndex = 1; shIndex < shNumberOfPoints; shIndex++)
			m_oStream >> arPoints[shIndex];

		HANDLE_META_POLYLINE(arPoints);
	}

	void CWmfParser::Read_META_POLYGON()
	{
		short shNumberOfPoints;
		m_oStream >> shNumberOfPoints;
		if (shNumberOfPoints < 1)
			return;

		std::vector<TWmfPointS> arPoints(shNumberOfPoints);

		for (short shIndex = 0; shIndex < shNumberOfPoints; shIndex++)
			m_oStream >> arPoints[shIndex];

		HANDLE_META_POLYGON(arPoints);
	}

	void CWmfParser::Read_META_POLYPOLYGON()
	{
		unsigned short ushNumberOfPolygons;
		m_oStream >> ushNumberOfPolygons;
		if (ushNumberOfPolygons <= 0)
			return;

		std::vector<std::vector<TWmfPointS>> arPolygons(ushNumberOfPolygons);

		unsigned short ushSizePolygon;

		for (unsigned short ushIndex = 0; ushIndex < ushNumberOfPolygons; ushIndex++)
		{
			m_oStream >> ushSizePolygon;
			arPolygons[ushIndex].resize(ushSizePolygon);
		}

		unsigned short ushPointsCount;

		for (unsigned short ushPolygonIndex = 0; ushPolygonIndex < ushNumberOfPolygons; ushPolygonIndex++)
		{
			ushPointsCount = arPolygons[ushPolygonIndex].size();

			if (ushPointsCount <= 0)
				continue;

			m_oStream >> arPolygons[ushPolygonIndex][0];

			for (unsigned short ushPointIndex = 1; ushPointIndex < ushPointsCount; ushPointIndex++)
				m_oStream >> arPolygons[ushPolygonIndex][ushPointIndex];
		}

		HANDLE_META_POLYPOLYGON(arPolygons);
	}

	void CWmfParser::Read_META_RECTANGLE()
	{
		short shL, shT, shR, shB;
		m_oStream >> shB >> shR >> shT >> shL;

		HANDLE_META_RECTANGLE(shB, shR, shT, shL);
	}

	void CWmfParser::Read_META_ROUNDRECT()
	{
		short shL, shT, shR, shB, shW, shH;
		m_oStream >> shH >> shW >> shB >> shR >> shT >> shL;

		HANDLE_META_ROUNDRECT(shH, shW, shB, shR, shT, shL);
	}

	void CWmfParser::Read_META_SETPIXEL()
	{
		TWmfColor oColor;
		short shX, shY;
		m_oStream >> oColor >> shY >> shX;

		HANDLE_META_SETPIXEL(oColor, shY, shX);
	}

	void CWmfParser::Read_META_TEXTOUT()
	{
		short shStringLength;
		m_oStream >> shStringLength;

		if (shStringLength <= 0)
			return;

		unsigned char* pString = new unsigned char[shStringLength + 1];
		if (!pString)
			return SetError();

		pString[shStringLength] = 0x00;
		m_oStream.ReadBytes(pString, shStringLength);

		if (shStringLength & 1)
			m_oStream.Skip(1);

		short shX, shY;
		m_oStream >> shY >> shX;

		HANDLE_META_TEXTOUT(shStringLength, pString, shY, shX);

		if (NULL != pString)
			delete[] pString;
	}

	void CWmfParser::Read_META_CREATEBRUSHINDIRECT()
	{
		TWmfLogBrush oLogBrush;
		m_oStream >> oLogBrush;

		HANDLE_META_CREATEBRUSHINDIRECT(oLogBrush);
	}

	void CWmfParser::Read_META_CREATEFONTINDIRECT()
	{
		CWmfFont* pFont = new CWmfFont();
		if (!pFont)
			return SetError();

		m_oStream >> pFont;

		HANDLE_META_CREATEFONTINDIRECT(*pFont);
	}

	void CWmfParser::Read_META_CREATEPALETTE()
	{
		CWmfPalette* pPalette = new CWmfPalette();
		if (!pPalette)
			return SetError();

		m_oStream >> pPalette;

		HANDLE_META_CREATEPALETTE(*pPalette);
	}

	void CWmfParser::Read_META_CREATEPATTERNBRUSH()
	{
		CWmfBrush* pBrush = new CWmfBrush();
		if (!pBrush)
			return SetError();

		if (m_pOutput)
		{
			TWmfBitmap16 oBitmap16;
			m_oStream >> oBitmap16;

			// TODO: Сделать чтение Bitmap16
		}

		HANDLE_META_CREATEPATTERNBRUSH(*pBrush, m_oStream);
	}

	void CWmfParser::Read_META_CREATEPENINDIRECT()
	{
		CWmfPen* pPen = new CWmfPen();
		if (!pPen)
			return SetError();

		m_oStream >> pPen;

		HANDLE_META_CREATEPENINDIRECT(*pPen);
	}

	void CWmfParser::Read_META_CREATEREGION()
	{
		CWmfRegion* pRegion = new CWmfRegion();
		if (!pRegion)
			return SetError();
		m_oStream >> pRegion;

		HANDLE_META_CREATEREGION(*pRegion);
	}

	void CWmfParser::Read_META_DELETEOBJECT()
	{
		unsigned short ushIndex;
		m_oStream >> ushIndex;

		HANDLE_META_DELETEOBJECT(ushIndex);
	}

	void CWmfParser::Read_META_DIBCREATEPATTERNBRUSH()
	{
		unsigned short ushStyle, ushColorUsage;
		m_oStream >> ushStyle >> ushColorUsage;

		HANDLE_META_DIBCREATEPATTERNBRUSH(ushStyle, ushColorUsage);
	}

	void CWmfParser::Read_META_SELECTCLIPREGION()
	{
		unsigned short ushIndex;
		m_oStream >> ushIndex;

		HANDLE_META_SELECTCLIPREGION(ushIndex);
	}

	void CWmfParser::Read_META_SELECTOBJECT()
	{
		unsigned short ushIndex;
		m_oStream >> ushIndex;

		HANDLE_META_SELECTOBJECT(ushIndex);
	}

	void CWmfParser::Read_META_SELECTPALETTE()
	{
		unsigned short ushIndex;
		m_oStream >> ushIndex;

		HANDLE_META_SELECTPALETTE(ushIndex);
	}

	void CWmfParser::Read_META_EXCLUDECLIPRECT()
	{
		short shLeft, shTop, shRight, shBottom;
		m_oStream >> shBottom >> shRight >> shTop >> shLeft;

		HANDLE_META_EXCLUDECLIPRECT(shBottom, shRight, shTop, shLeft);
	}

	void CWmfParser::Read_META_INTERSECTCLIPRECT()
	{
		short shLeft, shTop, shRight, shBottom;
		m_oStream >> shBottom >> shRight >> shTop >> shLeft;

		HANDLE_META_INTERSECTCLIPRECT(shLeft, shTop, shRight, shBottom);
	}

	void CWmfParser::Read_META_MOVETO()
	{
		short shX, shY;
		m_oStream >> shY >> shX;

		HANDLE_META_MOVETO(shX, shY);
	}

	void CWmfParser::Read_META_OFFSETCLIPRGN()
	{
		short shOffsetX, shOffsetY;
		m_oStream >> shOffsetY >> shOffsetX;

		HANDLE_META_OFFSETCLIPRGN(shOffsetX, shOffsetY);
	}

	void CWmfParser::Read_META_OFFSETVIEWPORTORG()
	{
		short shXOffset, shYOffset;
		m_oStream >> shYOffset >> shXOffset;

		HANDLE_META_OFFSETVIEWPORTORG(shXOffset, shYOffset);
	}

	void CWmfParser::Read_META_OFFSETWINDOWORG()
	{
		short shXOffset, shYOffset;
		m_oStream >> shYOffset >> shXOffset;

		HANDLE_META_OFFSETWINDOWORG(shXOffset, shYOffset);
	}

	void CWmfParser::Read_META_RESTOREDC()
	{
		HANDLE_META_RESTOREDC();
	}

	void CWmfParser::Read_META_SAVEDC()
	{
		HANDLE_META_SAVEDC();
	}

	void CWmfParser::Read_META_SCALEVIEWPORTEXT()
	{
		short yDenom, yNum, xDenom, xNum;
		m_oStream >> yDenom >> yNum >> xDenom >> xNum;

		HANDLE_META_SCALEVIEWPORTEXT(yDenom, yNum, xDenom, xNum);
	}

	void CWmfParser::Read_META_SCALEWINDOWEXT()
	{
		short yDenom, yNum, xDenom, xNum;
		m_oStream >> yDenom >> yNum >> xDenom >> xNum;

		HANDLE_META_SCALEWINDOWEXT(yDenom, yNum, xDenom, xNum);
	}

	void CWmfParser::Read_META_SETBKCOLOR()
	{
		TWmfColor oColor;
		m_oStream >> oColor;

		HANDLE_META_SETBKCOLOR(oColor);
	}

	void CWmfParser::Read_META_SETBKMODE()
	{
		unsigned short ushMode;
		m_oStream >> ushMode;

		HANDLE_META_SETBKMODE(ushMode);
	}

	void CWmfParser::Read_META_SETLAYOUT()
	{
		unsigned short ushLayout, ushReserved;
		m_oStream >> ushLayout >> ushReserved;

		HANDLE_META_SETLAYOUT(ushLayout);
	}

	void CWmfParser::Read_META_SETMAPMODE()
	{
		unsigned short ushMapMode;
		m_oStream >> ushMapMode;

		HANDLE_META_SETMAPMODE(ushMapMode);
	}

	void CWmfParser::Read_META_SETPOLYFILLMODE()
	{
		unsigned short ushMode;
		m_oStream >> ushMode;

		HANDLE_META_SETPOLYFILLMODE(ushMode);
	}

	void CWmfParser::Read_META_SETROP2()
	{
		unsigned short ushMode;
		m_oStream >> ushMode;

		HANDLE_META_SETROP2(ushMode);
	}

	void CWmfParser::Read_META_SETSTRETCHBLTMODE()
	{
		unsigned short ushMode;
		m_oStream >> ushMode;

		HANDLE_META_SETSTRETCHBLTMODE(ushMode);
	}

	void CWmfParser::Read_META_SETTEXTALIGN()
	{
		unsigned short ushTextAlign;
		m_oStream >> ushTextAlign;

		HANDLE_META_SETTEXTALIGN(ushTextAlign);
	}

	void CWmfParser::Read_META_SETTEXTCHAREXTRA()
	{
		unsigned short ushCharSpacing;
		m_oStream >> ushCharSpacing;

		HANDLE_META_SETTEXTCHAREXTRA(ushCharSpacing);
	}

	void CWmfParser::Read_META_SETTEXTCOLOR()
	{
		TWmfColor oColor;
		m_oStream >> oColor;

		HANDLE_META_SETTEXTCOLOR(oColor);
	}

	void CWmfParser::Read_META_SETTEXTJUSTIFICATION()
	{
		unsigned short ushBreakCount, ushBreakExtra;
		m_oStream >> ushBreakCount >> ushBreakExtra;

		HANDLE_META_SETTEXTJUSTIFICATION(ushBreakCount, ushBreakExtra);
	}

	void CWmfParser::Read_META_SETVIEWPORTEXT()
	{
		short shX, shY;
		m_oStream >> shY >> shX;

		HANDLE_META_SETVIEWPORTEXT(shX, shY);
	}

	void CWmfParser::Read_META_SETVIEWPORTORG()
	{
		short shX, shY;
		m_oStream >> shY >> shX;

		HANDLE_META_SETVIEWPORTORG(shX, shY);
	}

	void CWmfParser::Read_META_SETWINDOWEXT()
	{
		short shX, shY;
		m_oStream >> shY >> shX;

		HANDLE_META_SETWINDOWEXT(shX, shY);
	}

	void CWmfParser::Read_META_SETWINDOWORG()
	{
		short shX, shY;
		m_oStream >> shY >> shX;

		HANDLE_META_SETWINDOWORG(shX, shY);
	}

	void CWmfParser::Read_META_ESCAPE()
	{
		unsigned short ushEscapeFunction;
		unsigned short ushByteCount;
		m_oStream >> ushEscapeFunction;
		m_oStream >> ushByteCount;

		HANDLE_META_ESCAPE(ushEscapeFunction, ushByteCount);
	}

	void CWmfParser::Read_META_UNKNOWN()
	{
	}
}
