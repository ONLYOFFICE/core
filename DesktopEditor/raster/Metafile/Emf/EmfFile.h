#ifndef _EMF_FILE_H
#define _EMF_FILE_H



#include "../Wmf/WmfUtils.h"
#include "../Wmf/WmfTypes.h"

#include "../Common.h"

#include "EmfTypes.h"
#include "EmfOutputDevice.h"
#include "EmfPlayer.h"

#include "../../../fontengine/FontManager.h"
#include <iostream>

#if !defined(ETO_PDY)
	# define ETO_PDY 0x2000
#endif

namespace MetaFile
{
	class CEmfFile
	{
	public:

		CEmfFile() : m_oPlayer(this)
		{
			m_pBufferData = NULL;
			m_bError      = false;
			m_pOutput     = NULL;

			m_oStream.SetStream(NULL, 0);

			m_pDC = m_oPlayer.GetDC();
		};

		~CEmfFile()
		{

		};

		bool OpenFromFile(const wchar_t* wsFilePath)
		{
			if (m_pBufferData)
				delete m_pBufferData;

			NSFile::CFileBinary oFile;
			oFile.OpenFile(wsFilePath);
			long lFileSize = oFile.GetFileSize();

			m_pBufferData = new BYTE[lFileSize];
			if (!m_pBufferData)
				return false;

			DWORD lReadedSize;
			oFile.ReadFile(m_pBufferData, lFileSize, lReadedSize);

			m_oStream.SetStream(m_pBufferData, lFileSize);

			return true;
		}
		void Close()
		{
			if (m_pBufferData)
				delete m_pBufferData;

			m_pOutput = NULL;
			m_oStream.SetStream(NULL, 0);
			m_bError = false;
			m_oPlayer.Clear();
			m_pDC = m_oPlayer.GetDC();
		}
		TEmfRectL GetBounds()
		{
			return m_oHeader.oFrame;
		}
		void SetOutputDevice(CEmfOutputDevice* pOutput)
		{
			m_pOutput = pOutput;
		}
		void Scan()
		{
			Read_EMR_HEADER();
		}

		bool CheckError()
		{
			return m_bError;
		}

		void SetFontManager(CFontManager* pManager)
		{
			m_pFontManager = pManager;
		}

		void PlayMetaFile()
		{
			unsigned long ulSize, ulType;
			unsigned long ulNumber = 0;

			bool bEof = false;

			do
			{
				m_oStream >> ulType;
				m_oStream >> ulSize;

				m_ulRecordSize = ulSize - 8;

				switch (ulType)
				{
					//-----------------------------------------------------------
					// 2.3.1 Bitmap
					//-----------------------------------------------------------
					case EMR_BITBLT:
					{
						Read_EMR_BITBLT();
						break;
					}
					case EMR_STRETCHDIBITS:
					{
						Read_EMR_STRETCHDIBITS();
						break;
					}
					//-----------------------------------------------------------
					// 2.3.4 Control
					//-----------------------------------------------------------
					case EMR_EOF:
					{
						Read_EMR_EOF();
						bEof = true;
						break;
					}
					//-----------------------------------------------------------
					// 2.3.5 Drawing
					//-----------------------------------------------------------
					case EMR_EXTTEXTOUTW:
					{
						Read_EMR_EXTTEXTOUTW();
						break;
					}
					case EMR_POLYGON16:
					{
						Read_EMR_POLYGON16();
						break;
					}
					case EMR_POLYPOLYGON16:
					{
						Read_EMR_POLYPOLYGON16();
						break;
					}
					case EMR_LINETO:
					{
						Read_EMR_LINETO();
						break;
					}
					case EMR_POLYBEZIERTO16:
					{
						Read_EMR_POLYBEZIERTO16();
						break;
					}
					case EMR_POLYLINETO16:
					{
						Read_EMR_POLYLINETO16();
						break;
					}
					case EMR_STROKEANDFILLPATH:
					{
						Read_EMR_STROKEANDFILLPATH();
						break;
					}
					case EMR_STROKEPATH:
					{
						Read_EMR_STROKEPATH();
						break;
					}
					//-----------------------------------------------------------
					// 2.3.7 Object Creation
					//-----------------------------------------------------------
					case EMR_CREATEBRUSHINDIRECT:
					{
						Read_EMR_CREATEBRUSHINDIRECT();
						break;
					}
					case EMR_CREATEPEN:
					{
						Read_EMR_CREATEPEN();
						break;
					}
					case EMR_EXTCREATEFONTINDIRECTW:
					{
						Read_EMR_EXTCREATEFONTINDIRECTW();
						break;
					}
					case EMR_EXTCREATEPEN:
					{
						Read_EMR_EXTCREATEPEN();
						break;
					}
					//-----------------------------------------------------------
					// 2.3.8 Object Manipulation
					//-----------------------------------------------------------
					case EMR_SELECTOBJECT:
					{
						Read_EMR_SELECTOBJECT();
						break;
					}
					case EMR_DELETEOBJECT:
					{
						Read_EMR_DELETEOBJECT();
						break;
					}
					//-----------------------------------------------------------
					// 2.3.10 Path Bracket
					//-----------------------------------------------------------
					case EMR_BEGINPATH:
					{
						Read_EMR_BEGINPATH();
						break;
					}
					case EMR_ENDPATH:
					{
						Read_EMR_ENDPATH();
						break;
					}
					case EMR_CLOSEFIGURE:
					{
						Read_EMR_CLOSEFIGURE();
						break;
					}
					case EMR_FLATTENPATH:
					{
						Read_EMR_FLATTENPATH();
						break;
					}
					case EMR_WIDENPATH:
					{
						Read_EMR_WIDENPATH();
						break;
					}
					case EMR_ABORTPATH:
					{
						Read_EMR_ABORTPATH();
						break;
					}
					//-----------------------------------------------------------
					// 2.3.11 State
					//-----------------------------------------------------------
					case EMR_MOVETOEX:
					{
						Read_EMR_MOVETOEX();
						break;
					}
					case EMR_SAVEDC:
					{
						Read_EMR_SAVEDC();
						break;
					}
					case EMR_RESTOREDC:
					{
						Read_EMR_RESTOREDC();
						break;
					}
					case EMR_SETTEXTCOLOR:
					{
						Read_EMR_SETTEXTCOLOR();
						break;
					}
					case EMR_SETTEXTALIGN:
					{
						Read_EMR_SETTEXTALIGN();
						break;
					}
					case EMR_SETBKMODE:
					{
						Read_EMR_SETBKMODE();
						break;
					}
					case EMR_SETMITERLIMIT:
					{
						Read_EMR_SETMITERLIMIT();
						break;
					}
					case EMR_SETPOLYFILLMODE:
					{
						Read_EMR_SETPOLYFILLMODE();
						break;
					}
					//-----------------------------------------------------------
					// 2.3.12 Transform
					//-----------------------------------------------------------
					case EMR_SETWORLDTRANSFORM:
					{
						Read_EMR_SETWORLDTRANSFORM();
						break;
					}
					case EMR_MODIFYWORLDTRANSFORM:
					{
						Read_EMR_MODIFYWORLDTRANSFORM();
						break;
					}
					//-----------------------------------------------------------
					// Неподдерживаемые записи
					//-----------------------------------------------------------
					case EMR_GDICOMMENT:
					case EMR_SETICMMODE:
					{
						Read_EMR_UNKNOWN();
						break;
					}
					//-----------------------------------------------------------
					// Неизвестные записи
					//-----------------------------------------------------------
					default:
					{
						std::cout << ulType << " ";
						Read_EMR_UNKNOWN();
						break;
					}
				}

				if (bEof)
					break;

				if (!m_oStream.IsValid())
					SetError();

			} while (!CheckError());
		}



	private:

		void SetError()
		{
			m_bError = true;
		}
		CEmfDC* GetDC()
		{
			return m_pDC;
		}

		void Read_EMR_HEADER()
		{
			unsigned long ulType, ulSize;

			m_oStream >> ulType;
			m_oStream >> ulSize;

			if (EMR_HEADER != ulType)
				return SetError();

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

			if (ENHMETA_SIGNATURE != m_oHeader.ulSignature || 0x00010000 != m_oHeader.ulVersion)
				return SetError();

			// Пропускаем остальную часть заголовка, т.к. она нас пока не интересует
			unsigned long ulRemaining = ulSize - 88; // 8 + sizeof(TEmfHeader)
			m_oStream.Skip(ulRemaining);
		}
		void Read_EMR_STRETCHDIBITS()
		{
			TEmfStretchDIBITS oBitmap;
			m_oStream >> oBitmap;

			long lHeaderOffset         = oBitmap.offBmiSrc - sizeof(TEmfStretchDIBITS) - 8;
			unsigned long ulHeaderSize = oBitmap.cbBmiSrc;
			long lBitsOffset           = oBitmap.offBitsSrc - oBitmap.offBmiSrc - oBitmap.cbBmiSrc;
			unsigned long ulBitsSize   = oBitmap.cbBitsSrc;
			if (ulHeaderSize <= 0 || ulBitsSize <= 0 || lHeaderOffset < 0 || lBitsOffset < 0)
			{
				// TODO: Если попали сюда, значит надо смотреть oBitmap.BitBltRasterOperation

				if (lHeaderOffset > 0)
					m_oStream.Skip(lHeaderOffset);

				m_oStream.Skip(ulHeaderSize);

				if (lBitsOffset > 0)
					m_oStream.Skip(lBitsOffset);

				m_oStream.Skip(ulBitsSize);

				return;
			}

			m_oStream.Skip(lHeaderOffset);

			BYTE* pHeaderBuffer = new BYTE[ulHeaderSize];
			if (!pHeaderBuffer)
				return SetError();

			m_oStream.ReadBytes(pHeaderBuffer, ulHeaderSize);

			m_oStream.Skip(lBitsOffset);
			BYTE* pBitsBuffer = new BYTE[ulBitsSize];
			if (!pBitsBuffer)
			{
				delete[] pHeaderBuffer;
				return SetError();
			}
			m_oStream.ReadBytes(pBitsBuffer, ulBitsSize);

			BYTE* pBgraBuffer;
			unsigned long ulWidth, ulHeight;
			ReadImage(pHeaderBuffer, ulHeaderSize, pBitsBuffer, ulBitsSize, &pBgraBuffer, &ulWidth, &ulHeight);

			if (m_pOutput)
				m_pOutput->DrawBitmap(oBitmap.xDest, oBitmap.yDest, oBitmap.cxDest, oBitmap.cyDest, pBgraBuffer, ulWidth, ulHeight);

			if (pBgraBuffer)
				delete[] pBgraBuffer;

			delete[] pBitsBuffer;
			delete[] pHeaderBuffer;
		}
		void Read_EMR_BITBLT()
		{
			TEmfBitBlt oBitmap;
			m_oStream >> oBitmap;

			long lHeaderOffset         = oBitmap.offBmiSrc - sizeof(TEmfBitBlt) - 8;
			unsigned long ulHeaderSize = oBitmap.cbBmiSrc;
			long lBitsOffset           = oBitmap.offBitsSrc - oBitmap.offBmiSrc - oBitmap.cbBmiSrc;
			unsigned long ulBitsSize   = oBitmap.cbBitsSrc;
			if (ulHeaderSize <= 0 || ulBitsSize <= 0 || lHeaderOffset < 0 || lBitsOffset < 0)
			{
				// TODO: Если попали сюда, значит надо смотреть oBitmap.BitBltRasterOperation

				if (lHeaderOffset > 0)
					m_oStream.Skip(lHeaderOffset);

				m_oStream.Skip(ulHeaderSize);

				if (lBitsOffset > 0)
					m_oStream.Skip(lBitsOffset);

				m_oStream.Skip(ulBitsSize);

				return;
			}

			m_oStream.Skip(lHeaderOffset);

			BYTE* pHeaderBuffer = new BYTE[ulHeaderSize];
			if (!pHeaderBuffer)
				return SetError();

			m_oStream.ReadBytes(pHeaderBuffer, ulHeaderSize);

			m_oStream.Skip(lBitsOffset);
			BYTE* pBitsBuffer = new BYTE[ulBitsSize];
			if (!pBitsBuffer)
			{
				delete[] pHeaderBuffer;
				return SetError();
			}
			m_oStream.ReadBytes(pBitsBuffer, ulBitsSize);

			BYTE* pBgraBuffer;
			unsigned long ulWidth, ulHeight;
			ReadImage(pHeaderBuffer, ulHeaderSize, pBitsBuffer, ulBitsSize, &pBgraBuffer, &ulWidth, &ulHeight);

			if (m_pOutput)
				m_pOutput->DrawBitmap(oBitmap.xDest, oBitmap.yDest, oBitmap.cxDest, oBitmap.cyDest, pBgraBuffer, ulWidth, ulHeight);

			if (pBgraBuffer)
				delete[] pBgraBuffer;

			delete[] pBitsBuffer;
			delete[] pHeaderBuffer;
		}
		void Read_EMR_EOF()
		{
			unsigned long ulCount, ulOffset, ulSizeLast;

			m_oStream >> ulCount;
			m_oStream >> ulOffset;

			m_oStream.Skip(m_ulRecordSize - 8 - 4);

			m_oStream >> ulSizeLast;
		}
		void Read_EMR_UNKNOWN()
		{
			// Неизвестные и нереализованные записи мы пропускаем
			m_oStream.Skip(m_ulRecordSize);
		}
		void Read_EMR_SAVEDC()
		{
			m_pDC = m_oPlayer.SaveDC();
		}
		void Read_EMR_RESTOREDC()
		{
			long lSavedDC;
			m_oStream >> lSavedDC;

			if (lSavedDC >= 0)
			{
				SetError();
				return;
			}

			long lCount = -lSavedDC;
			for (long lIndex = 0; lIndex < lCount; lIndex++)
				m_oPlayer.RestoreDC();

			m_pDC = m_oPlayer.GetDC();
		}
		void Read_EMR_MODIFYWORLDTRANSFORM()
		{
			TEmfXForm oXForm;
			unsigned long ulMode;

			m_oStream >> oXForm;
			m_oStream >> ulMode;

			TEmfXForm* pCurTransform = m_pDC->GetTransform();
			pCurTransform->Multiply(oXForm, ulMode);
		}
		void Read_EMR_SETWORLDTRANSFORM()
		{
			TEmfXForm oXForm;

			m_oStream >> oXForm;

			TEmfXForm* pCurTransform = m_pDC->GetTransform();
			pCurTransform->Multiply(oXForm, MWT_SET);
		}
		void Read_EMR_CREATEBRUSHINDIRECT()
		{
			unsigned long ulBrushIndex;
			CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();

			m_oStream >> ulBrushIndex;
			m_oStream >> *pBrush;

			m_oPlayer.RegisterObject(ulBrushIndex, (CEmfObjectBase*)pBrush);
		}
		void Read_EMR_SETTEXTCOLOR()
		{
			TEmfColor oColor;
			m_oStream >> oColor;

			m_pDC->SetTextColor(oColor);
		}

		void Read_EMR_EXTTEXTOUTW()
		{
			TEmfExtTextoutW oText;

			m_oStream >> oText;

			// Читаем OutputString
			const unsigned long ulCharsCount = oText.wEmrText.Chars;
			long lSkip = oText.wEmrText.offString - 76; // 8 + 28 + 40
			m_oStream.Skip(lSkip);
			unsigned short* pUnicode = new unsigned short[ulCharsCount + 1];
			pUnicode[ulCharsCount] = 0x0000;
			m_oStream.ReadBytes(pUnicode, ulCharsCount);
			oText.wEmrText.OutputString = (void*)pUnicode;

			// Читаем OutputDx
			lSkip = oText.wEmrText.offDx - oText.wEmrText.offString - 2 * ulCharsCount;
			m_oStream.Skip(lSkip);
			const unsigned long ulDxCount = oText.wEmrText.Options & ETO_PDY ? 2 * ulCharsCount : ulCharsCount;
			unsigned long* pDx = new unsigned long[ulDxCount];
			m_oStream.ReadBytes(pDx, ulDxCount);

			std::wstring wsText((wchar_t*)pUnicode);

			if (m_pOutput)
				m_pOutput->DrawText(wsText.c_str(), ulCharsCount, oText.wEmrText.Reference.x, oText.wEmrText.Reference.y);

			delete[] pUnicode;
			delete[] pDx;
		}
		void Read_EMR_SELECTOBJECT()
		{
			unsigned long ulObjectIndex;
			m_oStream >> ulObjectIndex;

			m_oPlayer.SelectObject(ulObjectIndex);
		}
		void Read_EMR_POLYGON16()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;
			unsigned long ulCount;
			m_oStream >> ulCount;

			if (ulCount <= 0)
				return;

			TEmfPointS* pPoints = new TEmfPointS[ulCount];
			if (!pPoints)
				return SetError();

			for (unsigned long ulIndex = 0; ulIndex < ulCount; ulIndex++)
			{
				m_oStream >> pPoints[ulIndex];
			}

			CEmfOutputDevice* pOut = m_pOutput;
			if (pOut)
			{
				pOut->StartPath();
				pOut->MoveTo(pPoints[0].x, pPoints[0].y);

				for (unsigned long ulIndex = 1; ulIndex < ulCount; ulIndex++)
				{
					pOut->LineTo(pPoints[ulIndex].x, pPoints[ulIndex].y);
				}

				pOut->ClosePath();
				pOut->DrawPath();
				pOut->EndPath();
			}

			delete[] pPoints;
		}
		void Read_EMR_EXTCREATEFONTINDIRECTW()
		{
			unsigned long ulIndex;
			CEmfLogFont* pFont = new CEmfLogFont();
			if (!pFont)
				return SetError();

			m_oStream >> ulIndex;
			m_oStream >> *pFont;
			m_oPlayer.RegisterObject(ulIndex, (CEmfObjectBase*)pFont);
		}
		void Read_EMR_SETTEXTALIGN()
		{
			unsigned long ulAlign;
			m_oStream >> ulAlign;

			m_pDC->SetTextAlign(ulAlign);
		}
		void Read_EMR_SETBKMODE()
		{
			unsigned long ulBgMode;
			m_oStream >> ulBgMode;
			m_pDC->SetBgMode(ulBgMode);
		}
		void Read_EMR_DELETEOBJECT()
		{
			unsigned long ulIndex;
			m_oStream >> ulIndex;

			// Не удаляем объекты. Все объекты удаляться в любом случае в конце, а если удалять тут, то придется следить 
			// за всеми классами, у которых есть ссылки на удаляемые объекты.
		}
		void Read_EMR_SETMITERLIMIT()
		{
			unsigned long ulMiterLimit;
			m_oStream >> ulMiterLimit;
			m_pDC->SetMiterLimit(ulMiterLimit);
		}
		void Read_EMR_EXTCREATEPEN()
		{
			unsigned long ulPenIndex;
			m_oStream >> ulPenIndex;

			m_oStream.Skip(4); // offBmi
			m_oStream.Skip(4); // cbBmi
			m_oStream.Skip(4); // offBits
			m_oStream.Skip(4); // cbBits

			m_ulRecordSize -= 20;

			CEmfLogPen* pPen = new CEmfLogPen();
			if (!pPen)
				return SetError();

			// LogPenEx
			m_oStream >> pPen->PenStyle;
			m_oStream >> pPen->Width;
			m_oStream.Skip(4); // BrushStyle
			m_oStream >> pPen->Color;
			m_oStream.Skip(4); // BrushHatch
			m_oStream >> pPen->NumStyleEntries;

			m_ulRecordSize -= 24;
			if (pPen->NumStyleEntries > 0)
			{
				m_ulRecordSize -= pPen->NumStyleEntries * 4;
				pPen->StyleEntry = new unsigned long[pPen->NumStyleEntries];
				if (!pPen->StyleEntry)
				{
					delete pPen;
					return SetError();
				}

				for (unsigned long ulIndex = 0; ulIndex < pPen->NumStyleEntries; ulIndex++)
				{
					m_oStream >> pPen->StyleEntry[ulIndex];
				}
			}
			else
			{
				pPen->StyleEntry = NULL;
			}

			// Пропускаем часть с картинкой, если она была
			m_oStream.Skip(m_ulRecordSize);

			m_oPlayer.RegisterObject(ulPenIndex, (CEmfObjectBase*)pPen);
		}
		void Read_EMR_CREATEPEN()
		{
			unsigned long ulPenIndex;
			m_oStream >> ulPenIndex;
			CEmfLogPen* pPen = new CEmfLogPen();
			if (!pPen)
				return SetError();

			m_oStream >> pPen->PenStyle;
			m_oStream >> pPen->Width;
			m_oStream.Skip(4); // Width.y
			m_oStream >> pPen->Color;
			m_oPlayer.RegisterObject(ulPenIndex, (CEmfObjectBase*)pPen);
		}
		void Read_EMR_SETPOLYFILLMODE()
		{
			unsigned long ulFillMode;
			m_oStream >> ulFillMode;
			m_pDC->SetFillMode(ulFillMode);
		}
		void Read_EMR_POLYPOLYGON16()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;
			unsigned long ulNumberOfPolygons;
			m_oStream >> ulNumberOfPolygons;
			unsigned long ulTotalPointsCount;
			m_oStream >> ulTotalPointsCount;

			unsigned long* pPolygonPointCount = new unsigned long[ulNumberOfPolygons];
			if (!pPolygonPointCount)
				return SetError();

			for (unsigned long ulIndex = 0; ulIndex < ulNumberOfPolygons; ulIndex++)
			{
				m_oStream >> pPolygonPointCount[ulIndex];
			}

			TEmfPointS* pPoints = new TEmfPointS[ulTotalPointsCount];
			if (!pPoints)
			{
				delete[] pPolygonPointCount;
				return SetError();
			}

			for (unsigned long ulIndex = 0; ulIndex < ulTotalPointsCount; ulIndex++)
			{
				m_oStream >> pPoints[ulIndex];
			}

			if (m_pOutput)
			{
				for (unsigned long ulPolygonIndex = 0, unStartPointIndex = 0; ulPolygonIndex < ulNumberOfPolygons; ulPolygonIndex++)
				{
					m_pOutput->StartPath();
					for (unsigned long ulPointIndex = 0; ulPointIndex < pPolygonPointCount[ulPolygonIndex]; ulPointIndex++)
					{
						unsigned long ulRealPointIndex = ulPointIndex + unStartPointIndex;
						if (ulRealPointIndex >= ulTotalPointsCount)
						{
							delete[] pPolygonPointCount;
							delete[] pPoints;
							return SetError();
						}

						if (0 == ulPointIndex)
							m_pOutput->MoveTo(pPoints[ulRealPointIndex].x, pPoints[ulRealPointIndex].y);
						else
							m_pOutput->LineTo(pPoints[ulRealPointIndex].x, pPoints[ulRealPointIndex].y);
					}
					m_pOutput->ClosePath();
					m_pOutput->DrawPath();
					m_pOutput->EndPath();
				}
			}

			delete[] pPolygonPointCount;
			delete[] pPoints;
		}
		void Read_EMR_BEGINPATH()
		{
			// Ничего не делаем
		}
		void Read_EMR_ENDPATH()
		{
			// Ничего не делаем
		}
		void Read_EMR_CLOSEFIGURE()
		{
			// Ничего не делаем
		}
		void Read_EMR_FLATTENPATH()
		{
			// Ничего не делаем
		}
		void Read_EMR_WIDENPATH()
		{
			// Ничего не делаем
		}
		void Read_EMR_ABORTPATH()
		{
			// Ничего не делаем
		}
		void Read_EMR_MOVETOEX()
		{
			TEmfPointL oPoint;
			m_oStream >> oPoint;

			// TODO: Реализовать
		}
		void Read_EMR_LINETO()
		{
			TEmfPointL oPoint;
			m_oStream >> oPoint;

			// TODO: реализовать
		}
		void Read_EMR_POLYBEZIERTO16()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;

			unsigned long ulCount;
			m_oStream >> ulCount;

			TEmfPointS* pPoints = new TEmfPointS[ulCount];
			if (!pPoints)
				return SetError();

			for (unsigned long ulIndex = 0; ulIndex < ulCount; ulIndex++)
			{
				m_oStream >> pPoints[ulIndex];
			}

			delete[] pPoints;
		}
		void Read_EMR_POLYLINETO16()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;

			unsigned long ulCount;
			m_oStream >> ulCount;

			TEmfPointS* pPoints = new TEmfPointS[ulCount];
			if (!pPoints)
				return SetError();

			for (unsigned long ulIndex = 0; ulIndex < ulCount; ulIndex++)
			{
				m_oStream >> pPoints[ulIndex];
			}

			delete[] pPoints;
		}
		void Read_EMR_STROKEANDFILLPATH()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;
		}
		void Read_EMR_STROKEPATH()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;
		}

	private:

		CDataStream       m_oStream;
		BYTE*             m_pBufferData;
		bool              m_bError;
		CFontManager*     m_pFontManager;
		TEmfHeader        m_oHeader;

		unsigned long     m_ulRecordSize;
		CEmfOutputDevice* m_pOutput;

		CEmfDC*           m_pDC;
		CEmfPlayer        m_oPlayer;

		friend class CEmfRendererOutput;
		friend class CEmfPlayer;
	};
}

#endif // _EMF_FILE_H