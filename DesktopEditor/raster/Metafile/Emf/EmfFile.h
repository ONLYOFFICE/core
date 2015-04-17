#ifndef _EMF_FILE_H
#define _EMF_FILE_H



#include "../Wmf/WmfUtils.h"
#include "../Wmf/WmfTypes.h"

#include "../Common.h"

#include "EmfTypes.h"
#include "EmfOutputDevice.h"
#include "EmfPlayer.h"
#include "EmfPath.h"

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
			m_pPath       = NULL;

			m_oStream.SetStream(NULL, 0);

			m_pDC = m_oPlayer.GetDC();
		};

		~CEmfFile()
		{
			Close();
		};

		bool OpenFromFile(const wchar_t* wsFilePath)
		{
			Close();

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
			RELEASEOBJECT(m_pBufferData);
			RELEASEOBJECT(m_pPath);

			m_pOutput = NULL;
			m_oStream.SetStream(NULL, 0);
			m_bError = false;
			m_oPlayer.Clear();
			m_pDC = m_oPlayer.GetDC();
		}
		TEmfRectL* GetBounds()
		{
			return &m_oHeader.oFrame;
		}
		void SetOutputDevice(CEmfOutputDevice* pOutput)
		{
			m_pOutput = pOutput;
		}
		void Scan()
		{
			CEmfOutputDevice* pOutput = m_pOutput;
			m_pOutput = NULL;
			PlayMetaFile();
			m_pOutput = pOutput;

			RELEASEOBJECT(m_pPath);
			m_oPlayer.Clear();
			m_pDC = m_oPlayer.GetDC();
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

			unsigned long ulRecordIndex = 0;

			if (m_pOutput)
				m_pOutput->Begin();

			do
			{
				m_oStream >> ulType;
				m_oStream >> ulSize;

				m_ulRecordSize = ulSize - 8;

				if (ulType < EMR_MIN || ulType > EMR_MAX)
					return SetError();

				if (0 == ulRecordIndex && EMR_HEADER != ulType)
					return SetError();

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
					case EMR_SETDIBITSTODEVICE:
					{
						Read_EMR_SETDIBITSTODEVICE();
						break;
					}
					//-----------------------------------------------------------
					// 2.3.2 Clipping
					//-----------------------------------------------------------
					case EMR_EXTSELECTCLIPRGN: Read_EMR_EXTSELECTCLIPRGN(); break;
					case EMR_INTERSECTCLIPRECT: Read_EMR_INTERSECTCLIPRECT(); break;
					case EMR_SELECTCLIPPATH: Read_EMR_SELECTCLIPPATH(); break;
					case EMR_SETMETARGN: Read_EMR_SETMETARGN(); break;
					//-----------------------------------------------------------
					// 2.3.4 Control
					//-----------------------------------------------------------
					case EMR_HEADER:
					{
						Read_EMR_HEADER();
						break;
					}
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
					case EMR_FILLPATH:
					{
						Read_EMR_FILLPATH();
						break;
					}
					case EMR_RECTANGLE:
					{
						Read_EMR_RECTANGLE();
						break;
					}
					case EMR_POLYLINE16:
					{
						Read_EMR_POLYLINE16();
						break;
					}
					case EMR_ELLIPSE: Read_EMR_ELLIPSE(); break;
					case EMR_POLYBEZIER16: Read_EMR_POLYBEZIER16(); break;
					case EMR_ROUNDRECT: Read_EMR_ROUNDRECT(); break;
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
					case EMR_CREATEDIBPATTERNBRUSHPT:
					{
						Read_EMR_CREATEDIBPATTERNBRUSHPT();
						break;
					}
					case EMR_CREATEPALETTE: Read_EMR_CREATEPALETTE(); break;
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
					case EMR_SELECTPALETTE: Read_EMR_SELECTPALETTE(); break;
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
					case EMR_SETMAPMODE:
					{
						Read_EMR_SETMAPMODE();
						break;
					}
					case EMR_SETWINDOWORGEX:
					{
						Read_EMR_SETWINDOWORGEX();
						break;
					}
					case EMR_SETWINDOWEXTEX:
					{
						Read_EMR_SETWINDOWEXTEX();
						break;
					}
					case EMR_SETVIEWPORTORGEX:
					{
						Read_EMR_SETVIEWPORTORGEX();
						break;
					}
					case EMR_SETVIEWPORTEXTEX:
					{
						Read_EMR_SETVIEWPORTEXTEX();
						break;
					}
					case EMR_SETBKCOLOR:
					{
						Read_EMR_SETBKCOLOR();
						break;
					}
					case EMR_SETSTRETCHBLTMODE:
					{
						Read_EMR_SETSTRETCHBLTMODE();
						break;
					}
					case EMR_SETICMMODE:
					{
						Read_EMR_SETICMMODE();
						break;
					}
					case EMR_SETROP2: Read_EMR_SETROP2(); break;
					case EMR_REALIZEPALETTE: Read_EMR_REALIZEPALETTE(); break;
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

				ulRecordIndex++;

			} while (!CheckError());

			if (!CheckError())
				m_oStream.SeekToStart();

			if (m_pOutput)
				m_pOutput->End();
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
		TEmfRectL* GetDCBounds()
		{
			return &m_oHeader.oFrameToBounds;
		}
		bool ReadImage(unsigned long offBmi, unsigned long cbBmi, unsigned long offBits, unsigned long cbBits, unsigned long ulSkip, BYTE** ppBgraBuffer, unsigned long* pulWidth, unsigned long* pulHeight)
		{
			long lHeaderOffset         = offBmi - ulSkip;
			unsigned long ulHeaderSize = cbBmi;
			long lBitsOffset           = offBits - offBmi - cbBmi;
			unsigned long ulBitsSize   = cbBits;
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

			BYTE* pHeaderBuffer = new BYTE[ulHeaderSize];
			if (!pHeaderBuffer)
			{
				SetError();
				return false;
			}

			m_oStream.ReadBytes(pHeaderBuffer, ulHeaderSize);

			m_oStream.Skip(lBitsOffset);
			BYTE* pBitsBuffer = new BYTE[ulBitsSize];
			if (!pBitsBuffer)
			{
				delete[] pHeaderBuffer;
				SetError();
				return false;
			}
			m_oStream.ReadBytes(pBitsBuffer, ulBitsSize);

			MetaFile::ReadImage(pHeaderBuffer, ulHeaderSize, pBitsBuffer, ulBitsSize, ppBgraBuffer, pulWidth, pulHeight);

			delete[] pBitsBuffer;
			delete[] pHeaderBuffer;

			return true;
		}
		long TranslateX(long lSrcX)
		{
			long lDstX;

			TEmfWindow* pWindow   = m_pDC->GetWindow();
			TEmfWindow* pViewport = m_pDC->GetViewport();

			lDstX = pWindow->lX + (unsigned long)((double)(lSrcX - pViewport->lX / m_pDC->GetPixelWidth()) * m_pDC->GetPixelWidth());
			return lDstX;
		}
		long TranslateY(long lSrcY)
		{
			long lDstY;

			TEmfWindow* pWindow   = m_pDC->GetWindow();
			TEmfWindow* pViewport = m_pDC->GetViewport();

			lDstY = pWindow->lY + (unsigned long)((double)(lSrcY - pViewport->lY / m_pDC->GetPixelHeight()) * m_pDC->GetPixelHeight());

			return lDstY;
		}

		void MoveTo(TEmfPointL& oPoint)
		{
			MoveTo(oPoint.x, oPoint.y);
		}
		void MoveTo(TEmfPointS& oPoint)
		{
			MoveTo(oPoint.x, oPoint.y);
		}
		void MoveTo(long lX, long lY)
		{
			if (m_pPath)
			{
				if (!m_pPath->MoveTo(lX, lY))
					return SetError();
			}
			else if (m_pOutput)
			{
				m_pOutput->MoveTo(lX, lY);
			}
		}
		void LineTo(long lX, long lY)
		{
			if (m_pPath)
			{
				if (!m_pPath->LineTo(lX, lY))
					return SetError();
			}
			else if (m_pOutput)
			{
				m_pOutput->LineTo(lX, lY);
			}
		}
		void LineTo(TEmfPointL& oPoint)
		{
			LineTo(oPoint.x, oPoint.y);
		}
		void LineTo(TEmfPointS& oPoint)
		{
			LineTo(oPoint.x, oPoint.y);
		}
		void CurveTo(TEmfPointS& oPoint1, TEmfPointS& oPoint2, TEmfPointS& oPointE)
		{
			if (m_pPath)
			{
				if (!m_pPath->CurveTo(oPoint1, oPoint2, oPointE))
					return SetError();
			}
			else if (m_pOutput)
			{
				m_pOutput->CurveTo(oPoint1.x, oPoint1.y, oPoint2.x, oPoint2.y, oPointE.x, oPointE.y);
			}
		}
		void ClosePath()
		{
			if (m_pPath)
			{
				if (!m_pPath->Close())
					return SetError();
			}
			else if (m_pOutput)
				m_pOutput->ClosePath();
		}
		void ArcTo(long lL, long lT, long lR, long lB, double dStart, double dSweep)
		{
			if (m_pPath)
			{
				if (!m_pPath->ArcTo(lL, lT, lR, lB, dStart, dSweep))
					return SetError();
			}
			else if (m_pOutput)
			{
				m_pOutput->ArcTo(lL, lT, lR, lB, dStart, dSweep);
			}
		}
		void DrawPath(bool bStroke, bool bFill)
		{
			if (m_pPath && m_pOutput)
			{
				m_pPath->Draw(m_pOutput, bStroke, bFill);
			}
			else if (m_pOutput)
			{
				long lType = (bStroke ? 1 : 0) + (bFill ? 2 : 0);
				m_pOutput->DrawPath(lType);
				m_pOutput->EndPath();
			}
		}
		void UpdateOutputDC()
		{
			if (m_pOutput)
				m_pOutput->UpdateDC();
		}

		void Read_EMR_HEADER()
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

			if (ENHMETA_SIGNATURE != m_oHeader.ulSignature || 0x00010000 != m_oHeader.ulVersion)
				return SetError();

			// Пропускаем остальную часть заголовка, т.к. она нас пока не интересует
			unsigned long ulRemaining = m_ulRecordSize - 80; // sizeof(TEmfHeader)
			m_oStream.Skip(ulRemaining);

			// По логике мы должны получать рект, точно такой же как и oBounds, но есть файлы, где это не так.
			m_oHeader.oFrameToBounds.lLeft   = (long)(m_oHeader.oFrame.lLeft / 100.0 / m_oHeader.oMillimeters.cx * m_oHeader.oDevice.cx);
			m_oHeader.oFrameToBounds.lRight  = (long)(m_oHeader.oFrame.lRight / 100.0 / m_oHeader.oMillimeters.cx * m_oHeader.oDevice.cx);
			m_oHeader.oFrameToBounds.lTop    = (long)(m_oHeader.oFrame.lTop / 100.0 / m_oHeader.oMillimeters.cy * m_oHeader.oDevice.cy);
			m_oHeader.oFrameToBounds.lBottom = (long)(m_oHeader.oFrame.lBottom/ 100.0 / m_oHeader.oMillimeters.cy * m_oHeader.oDevice.cy);
		}
		void Read_EMR_STRETCHDIBITS()
		{
			TEmfStretchDIBITS oBitmap;
			m_oStream >> oBitmap;

			BYTE* pBgraBuffer = NULL;
			unsigned long ulWidth, ulHeight;

			if (ReadImage(oBitmap.offBmiSrc, oBitmap.cbBmiSrc, oBitmap.offBitsSrc, oBitmap.cbBitsSrc, sizeof(TEmfStretchDIBITS) + 8, &pBgraBuffer, &ulWidth, &ulHeight))
			{
				if (m_pOutput)
					m_pOutput->DrawBitmap(oBitmap.xDest, oBitmap.yDest, oBitmap.cxDest, oBitmap.cyDest, pBgraBuffer, ulWidth, ulHeight);
			}

			if (pBgraBuffer)
				delete[] pBgraBuffer;
		}
		void Read_EMR_BITBLT()
		{
			TEmfBitBlt oBitmap;
			m_oStream >> oBitmap;

			BYTE* pBgraBuffer = NULL;
			unsigned long ulWidth, ulHeight;

			if (ReadImage(oBitmap.offBmiSrc, oBitmap.cbBmiSrc, oBitmap.offBitsSrc, oBitmap.cbBitsSrc, sizeof(TEmfBitBlt) + 8, &pBgraBuffer, &ulWidth, &ulHeight))
			{
				if (m_pOutput)
					m_pOutput->DrawBitmap(oBitmap.xDest, oBitmap.yDest, oBitmap.cxDest, oBitmap.cyDest, pBgraBuffer, ulWidth, ulHeight);
			}

			if (m_pOutput)
			{
				if (0x00000042 == oBitmap.BitBltRasterOperation) // BLACKNESS
				{
					// Делаем все черным цветом
					pBgraBuffer = new BYTE[4];
					pBgraBuffer[0] = 0x00;
					pBgraBuffer[1] = 0x00;
					pBgraBuffer[2] = 0x00;
					pBgraBuffer[3] = 0xff;

					ulWidth  = 1;
					ulHeight = 1;
				}
				if (0x00FF0062 == oBitmap.BitBltRasterOperation) // WHITENESS
				{
					// Делаем все черным цветом
					pBgraBuffer = new BYTE[4];
					pBgraBuffer[0] = 0xff;
					pBgraBuffer[1] = 0xff;
					pBgraBuffer[2] = 0xff;
					pBgraBuffer[3] = 0xff;

					ulWidth  = 1;
					ulHeight = 1;
				}
				else if (0x00f00021 == oBitmap.BitBltRasterOperation) // PATCOPY
				{
					CEmfLogBrushEx* pBrush = m_pDC->GetBrush();
					if (pBrush)
					{					
						// Делаем цветом кисти
						pBgraBuffer = new BYTE[4];
						pBgraBuffer[0] = pBrush->Color.r;
						pBgraBuffer[1] = pBrush->Color.g;
						pBgraBuffer[2] = pBrush->Color.b;
						pBgraBuffer[3] = 0xff;

						ulWidth  = 1;
						ulHeight = 1;
					}
				}
				else if (0x005a0049 == oBitmap.BitBltRasterOperation || 0x00A000C9 == oBitmap.BitBltRasterOperation) // PATINVERT
				{
					CEmfLogBrushEx* pBrush = m_pDC->GetBrush();
					if (pBrush)
					{
						// Делаем цветом кисти
						pBgraBuffer = new BYTE[4];
						pBgraBuffer[0] = pBrush->Color.b;
						pBgraBuffer[1] = pBrush->Color.g;
						pBgraBuffer[2] = pBrush->Color.r;
						pBgraBuffer[3] = 30;

						ulWidth  = 1;
						ulHeight = 1;
					}
				}

				if (pBgraBuffer)
					m_pOutput->DrawBitmap(oBitmap.xDest, oBitmap.yDest, oBitmap.cxDest, oBitmap.cyDest, pBgraBuffer, ulWidth, ulHeight);				
			}

			if (pBgraBuffer)
				delete[] pBgraBuffer;
		}
		void Read_EMR_SETDIBITSTODEVICE()
		{
			TEmfSetDiBitsToDevice oBitmap;
			m_oStream >> oBitmap;

			BYTE* pBgraBuffer = NULL;
			unsigned long ulWidth, ulHeight;
			if (ReadImage(oBitmap.offBmiSrc, oBitmap.cbBmiSrc, oBitmap.offBitsSrc, oBitmap.cbBitsSrc, sizeof(TEmfSetDiBitsToDevice) + 8, &pBgraBuffer, &ulWidth, &ulHeight))
			{
				// TODO: Нужно реализовать обрезку картинки по параметрам oBitmap.iStartScan и oBitmap.cScans
				if (m_pOutput)
					m_pOutput->DrawBitmap(oBitmap.Bounds.lLeft, oBitmap.Bounds.lTop, oBitmap.Bounds.lRight - oBitmap.Bounds.lLeft, oBitmap.Bounds.lBottom - oBitmap.Bounds.lTop, pBgraBuffer, ulWidth, ulHeight);
			}

			if (pBgraBuffer)
				delete[] pBgraBuffer;
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
			UpdateOutputDC();
		}
		void Read_EMR_MODIFYWORLDTRANSFORM()
		{
			TEmfXForm oXForm;
			unsigned long ulMode;

			m_oStream >> oXForm;
			m_oStream >> ulMode;

			TEmfXForm* pCurTransform = m_pDC->GetTransform();
			pCurTransform->Multiply(oXForm, ulMode);
			UpdateOutputDC();
		}
		void Read_EMR_SETWORLDTRANSFORM()
		{
			TEmfXForm oXForm;

			m_oStream >> oXForm;

			TEmfXForm* pCurTransform = m_pDC->GetTransform();
			pCurTransform->Multiply(oXForm, MWT_SET);
			UpdateOutputDC();
		}
		void Read_EMR_CREATEBRUSHINDIRECT()
		{
			unsigned long ulBrushIndex;
			CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();
			if (!pBrush)
				return SetError();

			m_oStream >> ulBrushIndex;
			m_oStream >> *pBrush;

			m_oPlayer.RegisterObject(ulBrushIndex, (CEmfObjectBase*)pBrush);
		}
		void Read_EMR_SETTEXTCOLOR()
		{
			TEmfColor oColor;
			m_oStream >> oColor;

			m_pDC->SetTextColor(oColor);
			UpdateOutputDC();
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
			UpdateOutputDC();
		}
		void Read_EMR_POLYGON16()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;
			unsigned long ulCount;
			m_oStream >> ulCount;

			if (ulCount <= 0)
				return;

			TEmfPointS oPoint;
			m_oStream >> oPoint;
			MoveTo(oPoint);
			for (unsigned long ulIndex = 1; ulIndex < ulCount; ulIndex++)
			{
				m_oStream >> oPoint;
				LineTo(oPoint);
			}
			ClosePath();
			DrawPath(true, true);
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
			UpdateOutputDC();
		}
		void Read_EMR_SETBKMODE()
		{
			unsigned long ulBgMode;
			m_oStream >> ulBgMode;
			m_pDC->SetBgMode(ulBgMode);
			UpdateOutputDC();
		}
		void Read_EMR_DELETEOBJECT()
		{
			unsigned long ulIndex;
			m_oStream >> ulIndex;
			m_oPlayer.DeleteObject(ulIndex);
			UpdateOutputDC();
		}
		void Read_EMR_SETMITERLIMIT()
		{
			unsigned long ulMiterLimit;
			m_oStream >> ulMiterLimit;
			m_pDC->SetMiterLimit(ulMiterLimit);
			UpdateOutputDC();
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
			UpdateOutputDC();
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

			for (unsigned long ulPolygonIndex = 0, unStartPointIndex = 0; ulPolygonIndex < ulNumberOfPolygons; ulPolygonIndex++)
			{
				unsigned long ulCurrentPolygonPointsCount = pPolygonPointCount[ulPolygonIndex];
				if (0 == ulCurrentPolygonPointsCount)
					continue;

				TEmfPointS oPoint;
				m_oStream >> oPoint;
				MoveTo(oPoint);

				for (unsigned long ulPointIndex = 1; ulPointIndex < ulCurrentPolygonPointsCount; ulPointIndex++)
				{
					unsigned long ulRealPointIndex = ulPointIndex + unStartPointIndex;
					if (ulRealPointIndex >= ulTotalPointsCount)
					{
						delete[] pPolygonPointCount;
						return SetError();
					}

					m_oStream >> oPoint;
					LineTo(oPoint);
				}

				ClosePath();
				DrawPath(true, true);
			}

			delete[] pPolygonPointCount;
		}
		void Read_EMR_BEGINPATH()
		{
			if (m_pPath)
				delete m_pPath;

			m_pPath = new CEmfPath();
			if (!m_pPath)
				SetError();
		}
		void Read_EMR_ENDPATH()
		{
			// Ничего не делаем
		}
		void Read_EMR_CLOSEFIGURE()
		{
			if (m_pPath)
			{
				if (!m_pPath->Close())
					return SetError();
			}
		}
		void Read_EMR_FLATTENPATH()
		{
			// Ничего не делаем
		}
		void Read_EMR_WIDENPATH()
		{
			// TODO: реализовать
		}
		void Read_EMR_ABORTPATH()
		{
			if (m_pPath)
			{
				delete m_pPath;
				m_pPath = NULL;
			}
		}
		void Read_EMR_MOVETOEX()
		{
			TEmfPointL oPoint;
			m_oStream >> oPoint;
			MoveTo(oPoint);
		}
		void Read_EMR_LINETO()
		{
			TEmfPointL oPoint;
			m_oStream >> oPoint;
			LineTo(oPoint);
		}
		void Read_EMR_POLYBEZIERTO16()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;

			unsigned long ulCount;
			m_oStream >> ulCount;

			for (unsigned long ulIndex = 0; ulIndex < ulCount; ulIndex += 3)
			{
				if (ulCount - ulIndex < 2)
					return SetError();

				TEmfPointS oPoint1, oPoint2, oPointE;
				m_oStream >> oPoint1 >> oPoint2 >> oPointE;
				CurveTo(oPoint1, oPoint2, oPointE);
			}
		}
		void Read_EMR_POLYLINETO16()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;

			unsigned long ulCount;
			m_oStream >> ulCount;

			for (unsigned long ulIndex = 0; ulIndex < ulCount; ulIndex++)
			{
				TEmfPointS oPoint;
				m_oStream >> oPoint;
				LineTo(oPoint);
			}
		}
		void Read_EMR_STROKEANDFILLPATH()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;
			if (m_pOutput && m_pPath)
			{
				m_pPath->Draw(m_pOutput, true, true);
				RELEASEOBJECT(m_pPath);
			}
		}
		void Read_EMR_STROKEPATH()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;
			if (m_pOutput && m_pPath)
			{
				m_pPath->Draw(m_pOutput, true, false);
				RELEASEOBJECT(m_pPath);
			}
		}
		void Read_EMR_FILLPATH()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;
			if (m_pOutput && m_pPath)
			{
				m_pPath->Draw(m_pOutput, false, true);
				RELEASEOBJECT(m_pPath);
			}
		}
		void Read_EMR_SETMAPMODE()
		{
			unsigned long ulMapMode;
			m_oStream >> ulMapMode;

			m_pDC->SetMapMode(ulMapMode);
		}
		void Read_EMR_SETWINDOWORGEX()
		{
			TEmfPointL oOrigin;
			m_oStream >> oOrigin;
			m_pDC->SetWindowOrigin(oOrigin);
		}
		void Read_EMR_SETWINDOWEXTEX()
		{
			TEmfSizeL oExtent;
			m_oStream >> oExtent;
			m_pDC->SetWindowExtents(oExtent);
		}
		void Read_EMR_SETVIEWPORTORGEX()
		{
			TEmfPointL oOrigin;
			m_oStream >> oOrigin;
			m_pDC->SetViewportOrigin(oOrigin);
		}
		void Read_EMR_SETVIEWPORTEXTEX()
		{
			TEmfSizeL oExtent;
			m_oStream >> oExtent;
			m_pDC->SetViewportExtents(oExtent);
		}
		void Read_EMR_RECTANGLE()
		{
			TEmfRectL oBox;
			m_oStream >> oBox;

			MoveTo(oBox.lLeft, oBox.lTop);
			LineTo(oBox.lRight, oBox.lTop);
			LineTo(oBox.lRight, oBox.lBottom);
			LineTo(oBox.lLeft, oBox.lBottom);
			ClosePath();
			DrawPath(true, true);
		}
		void Read_EMR_SELECTCLIPPATH()
		{
			unsigned long ulRegionMode;
			m_oStream >> ulRegionMode;
			// TODO: реализовать клип
		}
		void Read_EMR_SETBKCOLOR()
		{
			TEmfColor oColor;
			m_oStream >> oColor;
			// TODO: реализовать

			UpdateOutputDC();
		}
		void Read_EMR_SETSTRETCHBLTMODE()
		{
			unsigned long ulStretchMode;
			m_oStream >> ulStretchMode;
			m_pDC->SetStretchMode(ulStretchMode);
		}
		void Read_EMR_SETICMMODE()
		{
			unsigned long ulICMMode;
			m_oStream >> ulICMMode;
		}
		void Read_EMR_CREATEDIBPATTERNBRUSHPT()
		{
			unsigned long ulBrushIndex;
			TEmfDibPatternBrush oDibBrush;
			m_oStream >> ulBrushIndex;
			m_oStream >> oDibBrush;

			BYTE* pBgraBuffer = NULL;
			unsigned long ulWidth, ulHeight;

			if (ReadImage(oDibBrush.offBmi, oDibBrush.cbBmi, oDibBrush.offBits, oDibBrush.cbBits, sizeof(TEmfDibPatternBrush) + 12, &pBgraBuffer, &ulWidth, &ulHeight))
			{
				CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();
				if (!pBrush)
					return SetError();

				pBrush->SetDibPattern(pBgraBuffer, ulWidth, ulHeight);
				m_oPlayer.RegisterObject(ulBrushIndex, (CEmfObjectBase*)pBrush);
			}
		}
		void Read_EMR_POLYLINE16()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;
			unsigned long ulCount;
			m_oStream >> ulCount;

			if (0 == ulCount)
				return;

			TEmfPointS oPoint;
			m_oStream >> oPoint;
			MoveTo(oPoint);

			for (unsigned long ulIndex = 1; ulIndex < ulCount; ulIndex++)
			{
				m_oStream >> oPoint;
				LineTo(oPoint);
			}

			DrawPath(true, false);
		}
		void Read_EMR_EXTSELECTCLIPRGN()
		{
			unsigned long ulRgnDataSize, ulRegionMode;
			m_oStream >> ulRgnDataSize >> ulRegionMode;

			m_oStream.Skip(m_ulRecordSize - 8);
			// TODO: Реализовать клип
		}
		void Read_EMR_SETMETARGN()
		{
			// TODO: Реализовать клип
		}
		void Read_EMR_ELLIPSE()
		{
			TEmfRectL oBox;
			m_oStream >> oBox;
			ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, 0, 360);
			DrawPath(true, true);
		}
		void Read_EMR_POLYBEZIER16()
		{
			TEmfRectL oBounds;
			m_oStream >> oBounds;

			unsigned long ulCount;
			m_oStream >> ulCount;

			if (0 == ulCount)
				return;

			TEmfPointS oStartPoint;
			m_oStream >> oStartPoint;
			MoveTo(oStartPoint);

			TEmfPointS oPoint1, oPoint2, oPointE;
			for (unsigned long ulIndex = 1; ulIndex < ulCount; ulIndex += 3)
			{
				m_oStream >> oPoint1 >> oPoint2 >> oPointE;
				CurveTo(oPoint1, oPoint2, oPointE);
			}
			DrawPath(true, false);
		}
		void Read_EMR_SETROP2()
		{
			unsigned long ulRop2Mode;
			m_oStream >> ulRop2Mode;

		}
		void Read_EMR_CREATEPALETTE()
		{
			unsigned long ulPaletteIndex;
			CEmfLogPalette* pPalette = new CEmfLogPalette();
			if (!pPalette)
				return SetError();

			m_oStream >> ulPaletteIndex;
			m_oStream >> *pPalette;
			m_oPlayer.RegisterObject(ulPaletteIndex, (CEmfObjectBase*)pPalette);
		}
		void Read_EMR_SELECTPALETTE()
		{
			unsigned long ulIndex;
			m_oStream >> ulIndex;
			m_oPlayer.SelectPalette(ulIndex);
		}
		void Read_EMR_REALIZEPALETTE()
		{
			// TODO: Реализовать
		}
		void Read_EMR_INTERSECTCLIPRECT()
		{
			TEmfRectL oClip;
			m_oStream >> oClip;
			// TODO: реализовать клип
		}
		void Read_EMR_ROUNDRECT()
		{
			TEmfRectL oBox;
			TEmfSizeL oCorner;
			m_oStream >> oBox >> oCorner;

			long lBoxW = oBox.lRight - oBox.lLeft;
			long lBoxH = oBox.lBottom - oBox.lTop;

			long lRoundW = (std::min)((long)oCorner.cx, lBoxW / 2);
			long lRoundH = (std::min)((long)oCorner.cy, lBoxH / 2);

			MoveTo(oBox.lLeft + lRoundW, oBox.lTop);
			LineTo(oBox.lRight - lRoundW, oBox.lTop);
			ArcTo(oBox.lRight - lRoundW, oBox.lTop, oBox.lRight, oBox.lTop + lRoundH, -90, 90);
			LineTo(oBox.lRight, oBox.lBottom - lRoundH);
			ArcTo(oBox.lRight - lRoundW, oBox.lBottom - lRoundH, oBox.lRight, oBox.lBottom, 0, 90);
			LineTo(oBox.lLeft + lRoundW, oBox.lBottom);
			ArcTo(oBox.lLeft, oBox.lBottom - lRoundH, oBox.lLeft + lRoundW, oBox.lBottom, 90, 90);
			LineTo(oBox.lLeft, oBox.lTop + lRoundH);
			ArcTo(oBox.lLeft, oBox.lTop, oBox.lLeft + lRoundW, oBox.lTop + lRoundW, 180, 90);
			ClosePath();
			DrawPath(true, true);
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

		CEmfPath*         m_pPath;

		friend class CEmfRendererOutput;
		friend class CEmfPlayer;
	};
}

#endif // _EMF_FILE_H