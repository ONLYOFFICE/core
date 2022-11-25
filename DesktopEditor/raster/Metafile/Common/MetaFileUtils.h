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
#ifndef _METAFILE_COMMON_METAFILEUTILS_H
#define _METAFILE_COMMON_METAFILEUTILS_H

#include "MetaFileTypes.h"

#include "../Emf/EmfTypes.h"
#include "../Emf/EmfPlusTypes.h"
#include "../Wmf/WmfTypes.h"
#include "../Emf/EmfObjects.h"
#include "../Emf/EmfPlusObjects.h"
#include "../Wmf/WmfObjects.h"

#include <algorithm>
#include <cfloat>

namespace MetaFile
{
	struct TRgbQuad
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;

		TRgbQuad()
		{
			r = 0;
			g = 0;
			b = 0;
		}
	};
	struct TSvgViewport
	{
		double dLeft;
		double dTop;
		double dRight;
		double dBottom;

		TSvgViewport() : dLeft(DBL_MAX), dTop(DBL_MAX), dRight(DBL_MIN), dBottom(DBL_MIN) {}

		bool Empty() const
		{
			return DBL_MAX == dLeft || DBL_MAX == dTop || DBL_MIN == dRight || DBL_MIN == dBottom || dRight == dLeft || dBottom == dTop;
		}

		double GetWidth() const
		{
			return (DBL_MAX == dLeft || DBL_MIN == dRight) ? 0 : dRight - dLeft;
		}

		double GetHeight() const
		{
			return (DBL_MAX == dTop || DBL_MIN == dBottom) ? 0 : dBottom - dTop;
		}
	};

	typedef std::pair<const std::wstring, std::wstring>     NodeAttribute;
	typedef std::vector<NodeAttribute>                      NodeAttributes;

	class CDataStream
	{
	public:

		CDataStream() : pBuffer(NULL)
		{
		}

		~CDataStream()
		{
		}

		void SetStream(BYTE* pBuf, unsigned int unSize)
		{
			pBuffer = pBuf;
			pCur    = pBuf;
			pEnd    = pBuf + unSize + 1;
		};
		BYTE* GetCurPtr()
		{
			return pCur;
		}

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
		unsigned int   ReadULong()
		{
			if (pCur + 4 >= pEnd)
				return 0;

			unsigned int unResult = (unsigned int)((pCur[0] << 0) | ((pCur[1]) << 8) | ((pCur[2]) << 16) | ((pCur[3]) << 24));
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

			int lIntValue  = (int)((pCur[0] << 16) | ((pCur[1]) << 8) | ((pCur[2]) << 0));
			int lFracValue = (int)(pCur[3]);
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
		int            ReadLong()
		{
			return (int)ReadULong();
		};
		void           ReadBytes(unsigned char*  pBuffer, unsigned int ulSize)
		{
			size_t ulRemainSize = (pEnd - pCur);
			size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

			for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
			{
				pBuffer[ulIndex] = ReadChar();
			}
		};
		void           ReadBytes(unsigned short* pBuffer, unsigned int ulSize)
		{
			size_t ulRemainSize = (pEnd - pCur) / 2;
			size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

			for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
			{
				pBuffer[ulIndex] = ReadUShort();
			}
		}
		void           ReadBytes(short* pBuffer, unsigned int ulSize)
		{
			size_t ulRemainSize = (pEnd - pCur) / 2;
			size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

			for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
			{
				pBuffer[ulIndex] = ReadShort();
			}
		}
		void           ReadBytes(unsigned int*  pBuffer, unsigned int ulSize)
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
		CDataStream& operator>>(unsigned int&  nValue)
		{
			nValue = ReadULong();
			return *this;
		}
		CDataStream& operator>>(unsigned long&   nValue)
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
		CDataStream& operator>>(bool&  nValue)
		{
			nValue = !!ReadChar();
			return *this;
		}
		CDataStream& operator>>(short& nValue)
		{
			nValue = ReadShort();
			return *this;
		}
		CDataStream& operator>>(int&  nValue)
		{
			nValue = ReadLong();
			return *this;
		}
		CDataStream& operator>>(long&   nValue)
		{
			nValue = ReadLong();
			return *this;
		}
		CDataStream& operator>>(TRectD& oRect)
		{
			*this >> oRect.dLeft;
			*this >> oRect.dTop;
			*this >> oRect.dRight;
			*this >> oRect.dBottom;

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
		CDataStream& operator>>(TEmfPointD& oPoint)
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
			ReadEmrTextW(oText.wEmrText, 36); // 8 + 28 (8 - тип и размер, 28 - размер данной структуры)

			return *this;
		}
		CDataStream& operator>>(TEmfExtTextoutA& oText)
		{
			*this >> oText.Bounds;
			*this >> oText.iGraphicsMode;
			*this >> oText.exScale;
			*this >> oText.eyScale;
			ReadEmrTextA(oText.aEmrText, 36); // 8 + 28 (8 - тип и размер, 28 - размер данной структуры)

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
			*this >> oFont.StrikeOut;
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
			if (oVector.Signature != 0x08007664 || oVector.NumAxes > 16 || oVector.NumAxes <= 0)
				return *this;

			oVector.Values = new int[oVector.NumAxes];
			if (!oVector.Values)
				return *this;

			for (unsigned int ulIndex = 0; ulIndex < oVector.NumAxes; ulIndex++)
				*this >> oVector.Values[ulIndex];

			return *this;
		}
		CDataStream& operator>>(CEmfLogFont& oFont)
		{
			if (oFont.IsFixedLength())
			{
				*this >> oFont.LogFontEx.LogFont;
				ReadBytes(oFont.LogFontEx.FullName, 64);
				ReadBytes(oFont.LogFontEx.Style, 32);
				ReadBytes(oFont.LogFontEx.Script, 18);
			}
			else
			{
				*this >> oFont.LogFontEx;
				*this >> oFont.DesignVector;
			}

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
		CDataStream& operator>>(TEmfStretchBLT& oBitmap)
		{
			*this >> oBitmap.Bounds;
			*this >> oBitmap.xDest;
			*this >> oBitmap.yDest;
			*this >> oBitmap.cxDest;
			*this >> oBitmap.cyDest;
			*this >> oBitmap.BitBltRasterOperation;
			*this >> oBitmap.xSrc;
			*this >> oBitmap.ySrc;
			*this >> oBitmap.XformSrc;
			*this >> oBitmap.BkColorSrc;
			*this >> oBitmap.UsageSrc;
			*this >> oBitmap.offBmiSrc;
			*this >> oBitmap.cbBmiSrc;
			*this >> oBitmap.offBitsSrc;
			*this >> oBitmap.cbBitsSrc;
			*this >> oBitmap.cxSrc;
			*this >> oBitmap.cySrc;

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
		CDataStream& operator>>(TRegionDataHeader& oRegionDataHeader)
		{
			*this >> oRegionDataHeader.unSize;
			*this >> oRegionDataHeader.unType;
			*this >> oRegionDataHeader.unCountRects;
			*this >> oRegionDataHeader.unRgnSize;
			*this >> oRegionDataHeader.oBounds;

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
		CDataStream& operator>>(TEmfPolyTextoutA& oText)
		{
			*this >> oText.Bounds;
			*this >> oText.iGraphicsMode;
			*this >> oText.exScale;
			*this >> oText.eyScale;
			*this >> oText.cStrings;

			if (0 != oText.cStrings)
			{
				oText.aEmrText = new TEmfEmrText[oText.cStrings];
				if (!oText.aEmrText)
					return *this;

				unsigned int nStartPos = Tell();
				for (unsigned int unIndex = 0; unIndex < oText.cStrings; unIndex++)
				{
					unsigned int nCurPos = Tell();
					ReadEmrTextA(oText.aEmrText[unIndex], nCurPos - nStartPos + 36); // 8 + 28 (8 - тип и размер, 28 - размер данной структуры)
				}
			}
			else
			{
				oText.aEmrText = NULL;
			}

			return *this;
		}
		CDataStream& operator>>(TEmfPolyTextoutW& oText)
		{
			*this >> oText.Bounds;
			*this >> oText.iGraphicsMode;
			*this >> oText.exScale;
			*this >> oText.eyScale;
			*this >> oText.cStrings;

			if (0 != oText.cStrings)
			{
				oText.wEmrText = new TEmfEmrText[oText.cStrings];
				if (!oText.wEmrText)
					return *this;

				unsigned int nStartPos = Tell();
				for (unsigned int unIndex = 0; unIndex < oText.cStrings; unIndex++)
				{
					unsigned int nCurPos = Tell();
					ReadEmrTextW(oText.wEmrText[unIndex], nCurPos - nStartPos + 36); // 8 + 28 (8 - тип и размер, 28 - размер данной структуры)
				}
			}
			else
			{
				oText.wEmrText = NULL;
			}

			return *this;
		}
		CDataStream& operator>>(TEmfSmallTextout& oText)
		{
			*this >> oText.x;
			*this >> oText.y;
			*this >> oText.cChars;
			*this >> oText.fuOptions;
			*this >> oText.iGraphicsMode;
			*this >> oText.exScale;
			*this >> oText.eyScale;

			if (!(oText.fuOptions & ETO_NO_RECT))
				*this >> oText.Bounds;

			oText.TextString = NULL;
			if (oText.cChars)
			{
				unsigned short* pUnicode = NULL;
				if (oText.fuOptions & ETO_SMALL_CHARS)
				{
					unsigned char* pString = new unsigned char[oText.cChars];
					if (!pString)
						return *this;

					ReadBytes(pString, oText.cChars);

					pUnicode = new unsigned short[oText.cChars];
					if (!pUnicode)
					{
						delete[] pString;
						return *this;
					}

					for (unsigned int unIndex = 0; unIndex < oText.cChars; unIndex++)
					{
						pUnicode[unIndex] = pString[unIndex];
					}
					delete[] pString;
				}
				else
				{
					pUnicode = new unsigned short[oText.cChars];
					if (!pUnicode)
						return *this;

					ReadBytes(pUnicode, oText.cChars);
				}
				oText.TextString = pUnicode;
			}

			return *this;
		}
		CDataStream& operator>>(TEmfAlphaBlend& oBitmap)
		{
			*this >> oBitmap.Bounds;
			*this >> oBitmap.xDest;
			*this >> oBitmap.yDest;
			*this >> oBitmap.cxDest;
			*this >> oBitmap.cyDest;
			*this >> oBitmap.BlendOperation;
			*this >> oBitmap.BlendFlags;
			*this >> oBitmap.SrcConstantAlpha;
			*this >> oBitmap.AlphaFormat;
			*this >> oBitmap.xSrc;
			*this >> oBitmap.ySrc;
			*this >> oBitmap.XformSrc;
			*this >> oBitmap.BkColor;
			*this >> oBitmap.UsageSrc;
			*this >> oBitmap.offBmiSrc;
			*this >> oBitmap.cbBmiSrc;
			*this >> oBitmap.offBitsSrc;
			*this >> oBitmap.cbBitsSrc;
			*this >> oBitmap.cxSrc;
			*this >> oBitmap.cySrc;

			return *this;
		}
		CDataStream& operator>>(TEmfPlusRect& oRect)
		{
			*this >> oRect.shX;
			*this >> oRect.shY;
			*this >> oRect.shWidth;
			*this >> oRect.shHeight;

			return *this;
		}
		CDataStream& operator>>(TEmfPlusRectF& oRect)
		{
			*this >> oRect.dX;
			*this >> oRect.dY;
			*this >> oRect.dWidth;
			*this >> oRect.dHeight;

			return *this;
		}
		CDataStream& operator>>(TEmfPlusARGB &oARGB)
		{
			*this >> oARGB.chBlue;
			*this >> oARGB.chGreen;
			*this >> oARGB.chRed;
			*this >> oARGB.chAlpha;

			return *this;
		}
		CDataStream& operator>>(TEmfPlusPointR &oPoint)
		{
			//TODO: реализовать
			return *this;
		}
		CDataStream& operator>>(TEmfPlusPointF &oPoint)
		{
			*this >> oPoint.X;
			*this >> oPoint.Y;

			return *this;
		}
		CDataStream& operator>>(TEmfPlusRectR &oTEmfPlusRectR)
		{
			*this >> oTEmfPlusRectR.chX;
			*this >> oTEmfPlusRectR.chY;
			*this >> oTEmfPlusRectR.chWidth;
			*this >> oTEmfPlusRectR.chHeight;

			return *this;
		}
		CDataStream& operator>>(TGUID& oTGUID)
		{
			*this >> oTGUID.nData1;
			*this >> oTGUID.shData2;
			*this >> oTGUID.shData3;

			BYTE *arBytes = new BYTE[8];

			ReadBytes(arBytes, 8);

			long long int llnValue;

			*((unsigned char*)(&llnValue) + 0) = pCur[0];
			*((unsigned char*)(&llnValue) + 1) = pCur[1];
			*((unsigned char*)(&llnValue) + 2) = pCur[2];
			*((unsigned char*)(&llnValue) + 3) = pCur[3];
			*((unsigned char*)(&llnValue) + 4) = pCur[4];
			*((unsigned char*)(&llnValue) + 5) = pCur[5];
			*((unsigned char*)(&llnValue) + 6) = pCur[6];
			*((unsigned char*)(&llnValue) + 7) = pCur[7];

			oTGUID.llnData4 = llnValue;

			return *this;
		}
		CDataStream& operator>>(CEmfPlusStringFormat& oStringFormat)
		{
			Skip(4); // Version
			*this >> oStringFormat.unStringFormatFlags;
			Skip(4); // Language
			*this >> oStringFormat.unStringAlignment;
			*this >> oStringFormat.unLineAlign;
			Skip(8); // DigitSubstitution, DigitLanguage
			*this >> oStringFormat.dFirstTabOffset;
			Skip(4); // HotkeyPrefix
			*this >> oStringFormat.dLeadingMargin;
			*this >> oStringFormat.dTrailingMargin;
			*this >> oStringFormat.dTracking;
			*this >> oStringFormat.unTrimming;

			int nTabStopCount, nRangeCount;

			*this >> nTabStopCount;
			*this >> nRangeCount;

			Skip(nTabStopCount * 4); // TabStops
			Skip(nRangeCount * 8);   // CharRange

			return *this;
		}
		CDataStream& operator>>(CEmfPlusImageAttributes& oAttributes)
		{
			Skip(8); //Version, Reserved 1 (4 bytes)

			unsigned int unWrapMode;

			*this >> unWrapMode;

			if (unWrapMode < WrapModeTile || unWrapMode > WrapModeClamp)
				return *this;

			oAttributes.eWrapMode = static_cast<EEmfPlusWrapMode>(unWrapMode);

			*this >> oAttributes.oClampColor;
			*this >> oAttributes.nObjectClamp;

			Skip(4); //Reserved 2 (4 bytes)
			return *this;
		}
		CDataStream& operator>>(TEmfPlusCustomLineCapArrowData& oLineCapData)
		{
			*this >> oLineCapData.dWidth;
			*this >> oLineCapData.dHeight;
			*this >> oLineCapData.dMiddleInset;

			unsigned int unTempValue;
			*this >> unTempValue;

			oLineCapData.bFillState = ((1 == unTempValue) ? true : false);

			*this >> oLineCapData.unLineStartCap;
			*this >> oLineCapData.unLineEndCap;
			*this >> oLineCapData.unLineJoin;
			*this >> oLineCapData.dLineMiterLimit;
			*this >> oLineCapData.dWidthScale;
			*this >> oLineCapData.oFillHotSpot;
			*this >> oLineCapData.oLineHotSpot;

			return *this;
		}
		CDataStream& operator>>(TEmfPlusCustomLineCapData& oLineCapData)
		{
			*this >> oLineCapData.unCustomLineCapDataFlags;
			*this >> oLineCapData.unBaseCap;
			*this >> oLineCapData.dBaseInset;
			*this >> oLineCapData.unStrokeStartCap;
			*this >> oLineCapData.unStrokeEndCap;
			*this >> oLineCapData.unStrokeJoin;
			*this >> oLineCapData.dStrokeMiterLimit;
			*this >> oLineCapData.dWidthScale;
			*this >> oLineCapData.oFillHotSpot;
			*this >> oLineCapData.oStrokeHotSpot;

			return *this;
		}
		CDataStream& operator>>(TWmfRect& oRect)
		{
			*this >> oRect.Left;
			*this >> oRect.Top;
			*this >> oRect.Right;
			*this >> oRect.Bottom;

			return *this;
		}
		CDataStream& operator>>(TWmfColor& oColor)
		{
			*this >> oColor.r;
			*this >> oColor.g;
			*this >> oColor.b;
			*this >> oColor.a;

			return *this;
		}
		CDataStream& operator>>(TWmfPointS& oPoint)
		{
			*this >> oPoint.x;
			*this >> oPoint.y;

			return *this;
		}
		CDataStream& operator>>(TWmfLogBrush& oBrush)
		{
			*this >> oBrush.BrushStyle;
			*this >> oBrush.Color;
			*this >> oBrush.BurshHatch;

			return *this;
		}
		CDataStream& operator>>(CWmfFont* pFont)
		{
			*this >> pFont->Height;
			*this >> pFont->Width;
			*this >> pFont->Escapement;
			*this >> pFont->Orientation;
			*this >> pFont->Weight;
			*this >> pFont->Italic;
			*this >> pFont->Underline;
			*this >> pFont->StrikeOut;
			*this >> pFont->CharSet;
			*this >> pFont->OutPrecision;
			*this >> pFont->ClipPrecision;
			*this >> pFont->Quality;
			*this >> pFont->PitchAndFamily;

			// Читаем до тех пор пока не встретим нулевой символ
			unsigned char unIndex = 0;
			*this >> pFont->Facename[unIndex];
			while (0x00 != pFont->Facename[unIndex])
			{
				unIndex++;
				if (32 == unIndex)
					break;

				*this >> pFont->Facename[unIndex];
			}

			return *this;
		}
		CDataStream& operator>>(TWmfPaletteEntry& oEntry)
		{
			*this >> oEntry.Values;
			*this >> oEntry.Blue;
			*this >> oEntry.Green;
			*this >> oEntry.Red;
			return *this;
		}
		CDataStream& operator>>(CWmfPalette* pPalette)
		{
			*this >> pPalette->Start;
			*this >> pPalette->NumberOfEntries;

			if (pPalette->NumberOfEntries > 0)
			{
				pPalette->aPaletteEntries = new TWmfPaletteEntry[pPalette->NumberOfEntries];
				if (!pPalette->aPaletteEntries)
				{
					pPalette->aPaletteEntries = NULL;
					pPalette->NumberOfEntries = 0;
					return *this;
				}

				for (unsigned short ushIndex = 0; ushIndex < pPalette->NumberOfEntries; ushIndex++)
				{
					*this >> pPalette->aPaletteEntries[ushIndex];
				}
			}
			else
			{
				pPalette->aPaletteEntries = NULL;
			}

			return *this;
		}
		CDataStream& operator>>(CWmfPen* pPen)
		{
			*this >> pPen->PenStyle;
			*this >> pPen->Width;
			*this >> pPen->Color;

			return *this;
		}
		CDataStream& operator>>(TWmfScanLine& oLine)
		{
			*this >> oLine.Left;
			*this >> oLine.Right;

			return *this;
		}
		CDataStream& operator>>(TWmfScanObject& oScan)
		{
			*this >> oScan.Count;
			*this >> oScan.Top;
			*this >> oScan.Bottom;

			if (oScan.Count > 0 && !(oScan.Count & 1)) // Должно делиться на 2
			{
				unsigned short ushCount = oScan.Count >> 1;
				oScan.ScanLines = new TWmfScanLine[ushCount];
				if (oScan.ScanLines)
				{
					for (unsigned short ushIndex = 0; ushIndex < ushCount; ushIndex++)
					{
						*this >> oScan.ScanLines[ushIndex];
					}
				}
				else
				{
					oScan.ScanLines = NULL;
					oScan.Count = 0;
				}
			}
			else
			{
				oScan.ScanLines = NULL;
			}
			*this >> oScan.Count2;

			return *this;
		}
		CDataStream& operator>>(CWmfRegion* pRegion)
		{
			*this >> pRegion->nextInChain;
			*this >> pRegion->ObjectType;

			if (0x0006 != pRegion->ObjectType)
				return *this;

			*this >> pRegion->ObjectCount;
			*this >> pRegion->RegionSize;
			*this >> pRegion->ScanCount;
			*this >> pRegion->MaxScan;
			*this >> pRegion->BoundingRectangle;

			if (pRegion->ScanCount > 0)
			{
				pRegion->aScans = new TWmfScanObject[pRegion->ScanCount];
				if (!pRegion->aScans)
				{
					pRegion->aScans = NULL;
					pRegion->ScanCount = 0;
					return *this;
				}
				for (unsigned short ushIndex = 0; ushIndex < pRegion->ScanCount; ushIndex++)
				{
					*this >> pRegion->aScans[ushIndex];
				}
			}
			else
			{
				pRegion->aScans = NULL;
			}


			return *this;
		}
		CDataStream& operator>>(TWmfStretchBlt& oBitmap)
		{
			*this >> oBitmap.RasterOperation;
			*this >> oBitmap.SrcHeight;
			*this >> oBitmap.SrcWidth;
			*this >> oBitmap.YSrc;
			*this >> oBitmap.XSrc;
			*this >> oBitmap.DestHeight;
			*this >> oBitmap.DestWidth;
			*this >> oBitmap.YDest;
			*this >> oBitmap.XDest;
			return *this;
		}
		CDataStream& operator>>(TWmfBitmap16& oBitmap)
		{
			*this >> oBitmap.Type;
			*this >> oBitmap.Width;
			*this >> oBitmap.Height;
			*this >> oBitmap.WidthBytes;
			*this >> oBitmap.Planes;
			*this >> oBitmap.BitsPixel;

			unsigned int unBitsCount = (((oBitmap.Width * oBitmap.BitsPixel + 15) >> 4) << 1) * oBitmap.Height;
			if (CanRead() >= unBitsCount)
			{
				//oBitmap.Bits = new unsigned char[unBitsCount];
			}
			else
			{
				oBitmap.Bits   = NULL;
				oBitmap.Width  = 0;
				oBitmap.Height = 0;
			}

			return *this;
		}
		CDataStream& operator>>(TWmfBitBlt& oBitmap)
		{
			*this >> oBitmap.RasterOperation;
			*this >> oBitmap.YSrc;
			*this >> oBitmap.XSrc;
			*this >> oBitmap.Height;
			*this >> oBitmap.Width;
			*this >> oBitmap.YDest;
			*this >> oBitmap.XDest;
			return *this;
		}
		CDataStream& operator>>(TWmfSetDibToDev& oBitmap)
		{
			*this >> oBitmap.ColorUsage;
			*this >> oBitmap.ScanCount;
			*this >> oBitmap.StartScan;
			*this >> oBitmap.yDib;
			*this >> oBitmap.xDib;
			*this >> oBitmap.Height;
			*this >> oBitmap.Width;
			*this >> oBitmap.yDest;
			*this >> oBitmap.xDest;
			return *this;
		}
		CDataStream& operator>>(TWmfStretchDib& oBitmap)
		{
			*this >> oBitmap.RasterOperation;
			*this >> oBitmap.ColorUsage;
			*this >> oBitmap.SrcHeight;
			*this >> oBitmap.SrcWidth;
			*this >> oBitmap.YSrc;
			*this >> oBitmap.XSrc;
			*this >> oBitmap.DestHeight;
			*this >> oBitmap.DestWidth;
			*this >> oBitmap.yDst;
			*this >> oBitmap.xDst;
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

		unsigned int Tell()
		{
			return (unsigned int)(pCur - pBuffer);
		}

		void Skip(unsigned int ulSkip)
		{
			pCur += ulSkip;
		}

		void SeekBack(unsigned int ulSkipBack)
		{
			pCur -= ulSkipBack;
		}

		void SeekToStart()
		{
			pCur = pBuffer;
		}

		unsigned int CanRead()
		{
			return (unsigned int)(pEnd - pCur);
		}

	private:

		template<typename T>void ReadEmrTextBase(TEmfEmrText& oText, unsigned int unOffset)
		{
			*this >> oText;

			// Читаем OutputString
			const unsigned int unCharsCount = oText.Chars;
			int nSkip = oText.offString - (unOffset + 40); // 40 - размер структуры TEmfEmrText
			Skip(nSkip);
			T* pString = new T[unCharsCount + 1];
			if (pString)
			{
				pString[unCharsCount] = 0x00;
				ReadBytes(pString, unCharsCount);
				oText.OutputString = (void*)pString;
			}

			// Читаем OutputDx
			nSkip = oText.offDx - oText.offString - 2 * unCharsCount;
			Skip(nSkip);
			const unsigned int unDxCount = oText.Options & ETO_PDY ? 2 * unCharsCount : unCharsCount;
			unsigned int* pDx = new unsigned int[unDxCount];
			if (pDx)
			{
				ReadBytes(pDx, unDxCount);
				oText.OutputDx = pDx;
			}
		}
		void ReadEmrTextA(TEmfEmrText& oText, unsigned int unOffset)
		{
			ReadEmrTextBase<unsigned char>(oText, unOffset);
		}
		void ReadEmrTextW(TEmfEmrText& oText, unsigned int unOffset)
		{
			ReadEmrTextBase<unsigned short>(oText, unOffset);
		}

	private:

		BYTE *pBuffer;
		BYTE *pCur;
		BYTE *pEnd;
	};

	void ReadImage(BYTE* pHeaderBuffer, unsigned int ulHeaderBufferLen, BYTE* pImageBuffer, unsigned int ulImageBufferLen, BYTE** ppDstBuffer, unsigned int* pulWidth, unsigned int* pulHeight);
	void ReadImage(BYTE* pImageBuffer, unsigned int unBufferLen, unsigned int unColorUsage, BYTE** ppDstBuffer, unsigned int* punWidth, unsigned int* punHeight);
	double GetEllipseAngle(int nL, int nT, int nR, int nB, int nX, int nY);
	void ProcessRasterOperation(unsigned int unRasterOperation, BYTE** ppBgra, unsigned int unWidth, unsigned int unHeight);
	std::wstring GetTempFilename(const std::wstring& sFolder = L"");

	std::wstring StringNormalization(std::wstring wsString);
	std::wstring ConvertToWString(double dValue, int nAccuracy = -1);
};
#endif // _METAFILE_COMMON_METAFILEUTILS_H
