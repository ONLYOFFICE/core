#ifndef _METAFILE_WMF_EMF_COMMON_H
#define _METAFILE_WMF_EMF_COMMON_H

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef NULL
#define NULL 0
#endif

#include <algorithm>

#include "Emf/EmfTypes.h"
#include "Wmf/WmfTypes.h"
#include "Wmf/WmfUtils.h"
#include "Emf/EmfObjects.h"

namespace MetaFile
{

#define METAFILE_RGBA(r, g, b) ((DWORD)( ( (BYTE)(r) )| ( ( (BYTE)(g) ) << 8 ) | ( ( (BYTE)(b) ) << 16 ) | ( (BYTE)(0) << 24 ) ) )

	struct TRgbQuad
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	class CDataStream
	{
	public:

		CDataStream() : pBuffer(NULL)
		{
		}

		~CDataStream()
		{
		}

		void SetStream(BYTE* pBuf, unsigned long unSize)
		{
			pBuffer = pBuf;
			pCur    = pBuf;
			pEnd    = pBuf + unSize + 1;
		};

		unsigned char  ReadUChar()
		{
			if (pCur + 1 >= pEnd)
				return 0;

			unsigned char unResult = pCur[0];
			pCur++;
			return unResult;
		};
		unsigned short ReadUShort()
		{
			if (pCur + 2 >= pEnd)
				return 0;

			unsigned short ushResult = (pCur[0]) | ((pCur[1]) << 8);
			pCur += 2;
			return ushResult;
		};
		unsigned long  ReadULong()
		{
			if (pCur + 4 >= pEnd)
				return 0;

			unsigned long unResult = (unsigned long)((pCur[0] << 0) | ((pCur[1]) << 8) | ((pCur[2]) << 16) | ((pCur[3]) << 24));
			pCur += 4;
			return unResult;
		};
		double         ReadDouble()
		{
			if (pCur + 4 >= pEnd)
				return 0;

			float output;

			*((unsigned char*)(&output) + 0) = pCur[0];
			*((unsigned char*)(&output) + 1) = pCur[1];
			*((unsigned char*)(&output) + 2) = pCur[2];
			*((unsigned char*)(&output) + 3) = pCur[3];

			pCur += 4;

			return output;

			long lIntValue  = (long)((pCur[0] << 16) | ((pCur[1]) << 8) | ((pCur[2]) << 0));
			long lFracValue = (long)(pCur[3]);
			pCur += 4;
			return (double)(lIntValue + (lFracValue / 16.0));
		};
		char           ReadChar()
		{
			return (char)ReadUChar();
		};
		short          ReadShort()
		{
			return (short)ReadUShort();
		};
		long           ReadLong()
		{
			return (long)ReadULong();
		};
		void           ReadBytes(unsigned char*  pBuffer, unsigned long ulSize)
		{
			size_t ulRemainSize = (pEnd - pCur);
			size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

			for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
			{
				pBuffer[ulIndex] = ReadChar();
			}
		};
		void           ReadBytes(unsigned short* pBuffer, unsigned long ulSize)
		{
			size_t ulRemainSize = (pEnd - pCur) / 2;
			size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

			for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
			{
				pBuffer[ulIndex] = ReadUShort();
			}
		}
		void           ReadBytes(unsigned long*  pBuffer, unsigned long ulSize)
		{
			size_t ulRemainSize = (pEnd - pCur) / 4;
			size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

			for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
			{
				pBuffer[ulIndex] = ReadULong();
			}
		}
		CDataStream& operator>>(unsigned char&  nValue)
		{
			nValue = ReadUChar();
			return *this;
		}
		CDataStream& operator>>(unsigned short& nValue)
		{
			nValue = ReadUShort();
			return *this;
		}
		CDataStream& operator>>(unsigned long&  nValue)
		{
			nValue = ReadULong();
			return *this;
		}
		CDataStream& operator>>(unsigned int&   nValue)
		{
			nValue = ReadULong();
			return *this;
		}
		CDataStream& operator>>(double&         dValue)
		{
			dValue = ReadDouble();
			return *this;
		}
		CDataStream& operator>>(char&  nValue)
		{
			nValue = ReadChar();
			return *this;
		}
		CDataStream& operator>>(short& nValue)
		{
			nValue = ReadShort();
			return *this;
		}
		CDataStream& operator>>(long&  nValue)
		{
			nValue = ReadLong();
			return *this;
		}
		CDataStream& operator>>(int&   nValue)
		{
			nValue = ReadLong();
			return *this;
		}
		CDataStream& operator>>(TEmfRect& oRect)
		{
			*this >> oRect.shLeft;
			*this >> oRect.shTop;
			*this >> oRect.shRight;
			*this >> oRect.shBottom;

			return *this;
		}
		CDataStream& operator>>(TEmfRectL& oRect)
		{
			*this >> oRect.lLeft;
			*this >> oRect.lTop;
			*this >> oRect.lRight;
			*this >> oRect.lBottom;

			return *this;
		}
		CDataStream& operator>>(TEmfSizeL& oSize)
		{
			*this >> oSize.cx;
			*this >> oSize.cy;

			return *this;
		}
		CDataStream& operator>>(TEmfColor& oColor)
		{
			*this >> oColor.r;
			*this >> oColor.g;
			*this >> oColor.b;
			*this >> oColor.a;

			return *this;
		}
		CDataStream& operator>>(CEmfLogBrushEx& oBrush)
		{
			*this >> oBrush.BrushStyle;
			*this >> oBrush.Color;
			*this >> oBrush.BrushHatch;

			return *this;
		}
		CDataStream& operator>>(TEmfPointL& oPoint)
		{
			*this >> oPoint.x;
			*this >> oPoint.y;

			return *this;
		}
		CDataStream& operator>>(TEmfPointS& oPoint)
		{
			*this >> oPoint.x;
			*this >> oPoint.y;

			return *this;
		}
		CDataStream& operator>>(TEmfEmrText& oText)
		{
			*this >> oText.Reference;
			*this >> oText.Chars;
			*this >> oText.offString;
			*this >> oText.Options;
			*this >> oText.Rectangle;
			*this >> oText.offDx;

			oText.OutputString = NULL;
			oText.OutputDx     = NULL;

			return *this;
		}
		CDataStream& operator>>(TEmfExtTextoutW& oText)
		{
			*this >> oText.Bounds;
			*this >> oText.iGraphicsMode;
			*this >> oText.exScale;
			*this >> oText.eyScale;
			*this >> oText.wEmrText;

			return *this;
		}
		CDataStream& operator>>(TEmfLogFont& oFont)
		{
			*this >> oFont.Height;
			*this >> oFont.Width;
			*this >> oFont.Escapement;
			*this >> oFont.Orientation;
			*this >> oFont.Weight;
			*this >> oFont.Italic;
			*this >> oFont.Underline;
			*this >> oFont.StrikOut;
			*this >> oFont.CharSet;
			*this >> oFont.OutPrecision;
			*this >> oFont.ClipPrecision;
			*this >> oFont.Quality;
			*this >> oFont.PitchAndFamily;
			ReadBytes(oFont.FaceName, 32);

			return *this;
		}
		CDataStream& operator>>(TEmfLogFontEx& oFont)
		{
			*this >> oFont.LogFont;
			ReadBytes(oFont.FullName, 64);
			ReadBytes(oFont.Style, 32);
			ReadBytes(oFont.Script, 32);

			return *this;
		}
		CDataStream& operator>>(TEmfDesignVector& oVector)
		{
			*this >> oVector.Signature;
			*this >> oVector.NumAxes;
			oVector.Values = NULL;

			if (oVector.NumAxes <= 0)
				return *this;

			oVector.Values = new long[oVector.NumAxes];
			if (!oVector.Values)
				return *this;

			for (unsigned long ulIndex = 0; ulIndex < oVector.NumAxes; ulIndex++)
				*this >> oVector.Values[ulIndex];

			return *this;
		}
		CDataStream& operator>>(CEmfLogFont& oFont)
		{
			*this >> oFont.LogFontEx;
			*this >> oFont.DesignVector;
			return *this;
		}
		CDataStream& operator>>(TEmfBitBlt& oBitBtl)
		{
			*this >> oBitBtl.Bounds;
			*this >> oBitBtl.xDest;
			*this >> oBitBtl.yDest;
			*this >> oBitBtl.cxDest;
			*this >> oBitBtl.cyDest;
			*this >> oBitBtl.BitBltRasterOperation;
			*this >> oBitBtl.xSrc;
			*this >> oBitBtl.ySrc;
			*this >> oBitBtl.XfromSrc;
			*this >> oBitBtl.BkColorSrc;
			*this >> oBitBtl.UsageSrc;
			*this >> oBitBtl.offBmiSrc;
			*this >> oBitBtl.cbBmiSrc;
			*this >> oBitBtl.offBitsSrc;
			*this >> oBitBtl.cbBitsSrc;

			return *this;
		}
		CDataStream& operator>>(TEmfXForm& oXForm)
		{
			*this >> oXForm.M11;
			*this >> oXForm.M12;
			*this >> oXForm.M21;
			*this >> oXForm.M22;
			*this >> oXForm.Dx;
			*this >> oXForm.Dy;

			return *this;
		}
		CDataStream& operator>>(TEmfStretchDIBITS& oBitmap)
		{
			*this >> oBitmap.Bounds;
			*this >> oBitmap.xDest;
			*this >> oBitmap.yDest;
			*this >> oBitmap.xSrc;
			*this >> oBitmap.ySrc;
			*this >> oBitmap.cxSrc;
			*this >> oBitmap.cySrc;
			*this >> oBitmap.offBmiSrc;
			*this >> oBitmap.cbBmiSrc;
			*this >> oBitmap.offBitsSrc;
			*this >> oBitmap.cbBitsSrc;
			*this >> oBitmap.UsageSrc;
			*this >> oBitmap.BitBltRasterOperation;
			*this >> oBitmap.cxDest;
			*this >> oBitmap.cyDest;

			return *this;
		}
		CDataStream& operator>>(TEmfSetDiBitsToDevice& oBitmap)
		{
			*this >> oBitmap.Bounds;
			*this >> oBitmap.xDest;
			*this >> oBitmap.yDest;
			*this >> oBitmap.xSrc;
			*this >> oBitmap.ySrc;
			*this >> oBitmap.cxSrc;
			*this >> oBitmap.cySrc;
			*this >> oBitmap.offBmiSrc;
			*this >> oBitmap.cbBmiSrc;
			*this >> oBitmap.offBitsSrc;
			*this >> oBitmap.cbBitsSrc;
			*this >> oBitmap.UsageSrc;
			*this >> oBitmap.iStartScan;
			*this >> oBitmap.cScans;

			return *this;
		}
		CDataStream& operator>>(TEmfDibPatternBrush& oBitmap)
		{
			*this >> oBitmap.Usage;
			*this >> oBitmap.offBmi;
			*this >> oBitmap.cbBmi;
			*this >> oBitmap.offBits;
			*this >> oBitmap.cbBits;
			
			return *this;
		}
		CDataStream& operator>>(TEmfLogPaletteEntry& oEntry)
		{
			*this >> oEntry.Reserved;
			*this >> oEntry.Blue;
			*this >> oEntry.Green;
			*this >> oEntry.Red;
			return *this;
		}
		CDataStream& operator>>(CEmfLogPalette& oPalette)
		{
			unsigned short ushVersion;

			*this >> ushVersion;
			*this >> oPalette.NumberOfEntries;

			if (oPalette.NumberOfEntries > 0)
			{
				oPalette.PaletteEntries = new TEmfLogPaletteEntry[oPalette.NumberOfEntries];
				if (!oPalette.PaletteEntries)
				{
					oPalette.NumberOfEntries = 0;
					oPalette.PaletteEntries  = NULL;
					return *this;
				}

				for (unsigned short ushIndex = 0; ushIndex < oPalette.NumberOfEntries; ushIndex++)
				{
					*this >> oPalette.PaletteEntries[ushIndex];
				}
			}
			else
				oPalette.PaletteEntries = NULL;

			return *this;
		}
		CDataStream& operator>>(TRgbQuad& oRGB)
		{
			*this >> oRGB.b;
			*this >> oRGB.g;
			*this >> oRGB.r;
			Skip(1); // reserved

			return *this;
		}

		bool IsValid() const
		{
			if (NULL == pBuffer)
				return false;

			return true;
		}

		bool IsEof() const
		{
			if (pCur >= pEnd)
				return true;

			return false;
		}

		unsigned long Tell()
		{
			return (unsigned long)(pCur - pBuffer);
		}

		void Skip(unsigned long ulSkip)
		{
			pCur += ulSkip;
		}

		void SeekBack(unsigned long ulSkipBack)
		{
			pCur -= ulSkipBack;
		}

		void SeekToStart()
		{
			pCur = pBuffer;
		}

		unsigned long CanRead()
		{
			return (unsigned long)(pEnd - pCur);
		}

	private:

		BYTE *pBuffer;
		BYTE *pCur;
		BYTE *pEnd;
	};

	void ReadImage(BYTE* pHeaderBuffer, unsigned long ulHeaderBufferLen, BYTE* pImageBuffer, unsigned long ulImageBufferLen, BYTE** ppDstBuffer, unsigned long* pulWidth, unsigned long* pulHeight);
};

#endif //_METAFILE_WMF_EMF_COMMON_H