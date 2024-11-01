/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
	bool Equals(double dFirst, double dSecond, double dEpsilon = DBL_EPSILON);
	std::wstring ConvertToUnicode(const unsigned char* pText, unsigned long unLength, unsigned short uchCharSet);
	std::wstring ConvertToWString(double dValue, int nAccuracy = -1);
	std::wstring ConvertToWString(const std::vector<double>& arValues, int nAccuracy = -1);

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

		TSvgViewport() : dLeft(DBL_MAX), dTop(DBL_MAX), dRight(-DBL_MAX), dBottom(-DBL_MAX) {}

		bool Empty() const
		{
			return Equals(DBL_MAX, dLeft) || Equals(DBL_MAX, dTop) || Equals(-DBL_MAX, dRight) || Equals(-DBL_MAX, dBottom) || Equals(dRight, dLeft) || Equals(dBottom, dTop);
		}

		double GetWidth() const
		{
			return (Equals(DBL_MAX, dLeft) || Equals(-DBL_MAX, dRight)) ? 0 : dRight - dLeft;
		}

		double GetHeight() const
		{
			return (Equals(DBL_MAX, dTop) || Equals(-DBL_MAX, dBottom)) ? 0 : dBottom - dTop;
		}
	};

	typedef std::pair<const std::wstring, std::wstring>     NodeAttribute;
	// typedef std::vector<NodeAttribute>                      NodeAttributes;

	class NodeAttributes : public std::vector<NodeAttribute>
	{
	public:
		using std::vector<NodeAttribute>::vector;

		void Add(const std::wstring& wsNameArgument, const std::wstring& wsValueArgument)
		{
			emplace_back(NodeAttribute{wsNameArgument, wsValueArgument});
		}

		void Add(const std::wstring& wsNameArgument, const double& dValueArgument, int nAccuracy = -1)
		{
			emplace_back(NodeAttribute{wsNameArgument, ConvertToWString(dValueArgument, nAccuracy)});
		}

		void Add(const std::wstring& wsNameArgument, const int& nValueArgument)
		{
			emplace_back(NodeAttribute{wsNameArgument, std::to_wstring(nValueArgument)});
		}
	};

	class CDataStream
	{
	public:

		CDataStream() : pBuffer(NULL), pBufferEnd(NULL), pCur(NULL), pEnd(NULL)
		{
		}

		~CDataStream()
		{
		}

		void SetStream(BYTE* pBuf, unsigned int unSize)
		{
			pBuffer       = pBuf;
			pCur          = pBuf;
			pBufferEnd    = pBuf + unSize;
			pEnd          = pBufferEnd;
		}

		void SetCurrentBlockSize(unsigned int unSize)
		{
			if (pCur + unSize >= pBufferEnd)
				pEnd = pBufferEnd;
			else
				pEnd = pCur + unSize;
		}

		void ClearCurrentBlockSize()
		{
			pEnd = pBufferEnd;
		}

		BYTE* GetCurPtr()
		{
			return pCur;
		}

		unsigned char  ReadUChar()
		{
			if (pCur >= pEnd)
				return 0;

			unsigned char unResult = pCur[0];
			pCur++;
			return unResult;
		}
		unsigned short ReadUShort()
		{
			if (pCur + 1 >= pEnd)
				return 0;

			unsigned short ushResult = (pCur[0]) | ((pCur[1]) << 8);
			pCur += 2;
			return ushResult;
		}
		unsigned int   ReadULong()
		{
			if (pCur + 3 >= pEnd)
				return 0;

			unsigned int unResult = (unsigned int)((pCur[0] << 0) | ((pCur[1]) << 8) | ((pCur[2]) << 16) | ((pCur[3]) << 24));
			pCur += 4;
			return unResult;
		}
		double         ReadDouble()
		{
			if (pCur + 3 >= pEnd)
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
		}
		char           ReadChar()
		{
			return (char)ReadUChar();
		}
		short          ReadShort()
		{
			return (short)ReadUShort();
		}
		int            ReadLong()
		{
			return (int)ReadULong();
		}
		void           ReadBytes(unsigned char*  pBuffer, unsigned int ulSize)
		{
			size_t ulRemainSize = (pEnd - pCur);
			size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

			for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
			{
				pBuffer[ulIndex] = ReadChar();
			}
		}
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
		template<typename T>
		CDataStream& operator>>(TRect<T>& oRect)
		{
			*this >> oRect.Left;
			*this >> oRect.Top;
			*this >> oRect.Right;
			*this >> oRect.Bottom;

			return *this;
		}
		template<typename T>
		CDataStream& operator>>(TPoint<T>& oPoint)
		{
			*this >> oPoint.X;
			*this >> oPoint.Y;
			
			return *this;
		}
		CDataStream& operator>>(TRGBA& oColor)
		{
			*this >> oColor.r;
			*this >> oColor.g;
			*this >> oColor.b;
			*this >> oColor.a;

			return *this;
		}
		CDataStream& operator>>(CEmfLogBrushEx& oBrush)
		{
			*this >> oBrush.unBrushStyle;
			*this >> oBrush.oColor;
			*this >> oBrush.unBrushHatch;

			return *this;
		}
		template<typename T>
		CDataStream& operator>>(TEmrText<T>& oText)
		{
			*this >> oText.oReference;
			*this >> oText.unChars;
			*this >> oText.unOffString;
			*this >> oText.unOptions;
			*this >> oText.oRectangle;
			*this >> oText.unOffDx;

			oText.pOutputString = NULL;
			oText.pOutputDx     = NULL;

			return *this;
		}
		template<typename T> 
		CDataStream& operator>>(TExtTextout<T>& oText)
		{
			*this >> oText.oBounds;
			*this >> oText.unIGraphicsMode;
			*this >> oText.dExScale;
			*this >> oText.dEyScale;
			ReadEmrText(oText.oEmrText, 36); // 8 + 28 (8 - тип и размер, 28 - размер данной структуры)

			return *this;
		}
		CDataStream& operator>>(TEmfLogFont& oFont)
		{
			*this >> oFont.nHeight;
			*this >> oFont.nWidth;
			*this >> oFont.nEscapement;
			*this >> oFont.nOrientation;
			*this >> oFont.nWeight;
			*this >> oFont.uchItalic;
			*this >> oFont.uchUnderline;
			*this >> oFont.uchStrikeOut;
			*this >> oFont.uchCharSet;
			*this >> oFont.uchOutPrecision;
			*this >> oFont.uchClipPrecision;
			*this >> oFont.uchQuality;
			*this >> oFont.uchPitchAndFamily;
			ReadBytes(oFont.ushFaceName, 32);

			return *this;
		}
		CDataStream& operator>>(TEmfLogFontEx& oFont)
		{
			*this >> oFont.oLogFont;
			ReadBytes(oFont.ushFullName, 64);
			ReadBytes(oFont.ushStyle, 32);
			ReadBytes(oFont.ushScript, 32);

			return *this;
		}
		CDataStream& operator>>(TEmfDesignVector& oVector)
		{
			*this >> oVector.unSignature;
			*this >> oVector.unNumAxes;

			oVector.pValues = NULL;
			if (oVector.unSignature != 0x08007664 || oVector.unNumAxes > 16 || oVector.unNumAxes <= 0)
				return *this;

			oVector.pValues = new int[oVector.unNumAxes];
			if (!oVector.pValues)
				return *this;

			for (unsigned int ulIndex = 0; ulIndex < oVector.unNumAxes; ulIndex++)
				*this >> oVector.pValues[ulIndex];

			return *this;
		}
		CDataStream& operator>>(CEmfLogFont& oFont)
		{
			if (oFont.IsFixedLength())
			{
				*this >> oFont.oLogFontEx.oLogFont;
				ReadBytes(oFont.oLogFontEx.ushFullName, 64);
				ReadBytes(oFont.oLogFontEx.ushStyle, 32);
				ReadBytes(oFont.oLogFontEx.ushScript, 18);
			}
			else
			{
				*this >> oFont.oLogFontEx;
				*this >> oFont.oDesignVector;
			}

			return *this;
		}
		CDataStream& operator>>(TEmfBitBlt& oBitBtl)
		{
			*this >> oBitBtl.oBounds;
			*this >> oBitBtl.nXDest;
			*this >> oBitBtl.nYDest;
			*this >> oBitBtl.nCxDest;
			*this >> oBitBtl.nCyDest;
			*this >> oBitBtl.unBitBltRasterOperation;
			*this >> oBitBtl.nXSrc;
			*this >> oBitBtl.nYSrc;
			*this >> oBitBtl.oXfromSrc;
			*this >> oBitBtl.oBkColorSrc;
			*this >> oBitBtl.unUsageSrc;
			*this >> oBitBtl.unOffBmiSrc;
			*this >> oBitBtl.unCbBmiSrc;
			*this >> oBitBtl.unOffBitsSrc;
			*this >> oBitBtl.unCbBitsSrc;

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
			*this >> oBitmap.oBounds;
			*this >> oBitmap.nXDest;
			*this >> oBitmap.nYDest;
			*this >> oBitmap.nXSrc;
			*this >> oBitmap.nYSrc;
			*this >> oBitmap.nCxSrc;
			*this >> oBitmap.nCySrc;
			*this >> oBitmap.unOffBmiSrc;
			*this >> oBitmap.unCbBmiSrc;
			*this >> oBitmap.unOffBitsSrc;
			*this >> oBitmap.unCbBitsSrc;
			*this >> oBitmap.unUsageSrc;
			*this >> oBitmap.unBitBltRasterOperation;
			*this >> oBitmap.nCxDest;
			*this >> oBitmap.nCyDest;

			return *this;
		}
		CDataStream& operator>>(TEmfStretchBLT& oBitmap)
		{
			*this >> oBitmap.oBounds;
			*this >> oBitmap.nXDest;
			*this >> oBitmap.nYDest;
			*this >> oBitmap.nCxDest;
			*this >> oBitmap.nCyDest;
			*this >> oBitmap.unBitBltRasterOperation;
			*this >> oBitmap.nXSrc;
			*this >> oBitmap.nYSrc;
			*this >> oBitmap.oXformSrc;
			*this >> oBitmap.oBkColorSrc;
			*this >> oBitmap.unUsageSrc;
			*this >> oBitmap.unOffBmiSrc;
			*this >> oBitmap.unCbBmiSrc;
			*this >> oBitmap.unOffBitsSrc;
			*this >> oBitmap.unCbBitsSrc;
			*this >> oBitmap.nCxSrc;
			*this >> oBitmap.nCySrc;

			return *this;
		}
		CDataStream& operator>>(TEmfSetDiBitsToDevice& oBitmap)
		{
			*this >> oBitmap.oBounds;
			*this >> oBitmap.nXDest;
			*this >> oBitmap.nYDest;
			*this >> oBitmap.nXSrc;
			*this >> oBitmap.nYSrc;
			*this >> oBitmap.nCxSrc;
			*this >> oBitmap.nCySrc;
			*this >> oBitmap.unOffBmiSrc;
			*this >> oBitmap.unCbBmiSrc;
			*this >> oBitmap.unOffBitsSrc;
			*this >> oBitmap.unCbBitsSrc;
			*this >> oBitmap.unUsageSrc;
			*this >> oBitmap.unIStartScan;
			*this >> oBitmap.unCScans;

			return *this;
		}
		CDataStream& operator>>(TEmfDibPatternBrush& oBitmap)
		{
			*this >> oBitmap.unUsage;
			*this >> oBitmap.unOffBmi;
			*this >> oBitmap.unCbBmi;
			*this >> oBitmap.unOffBits;
			*this >> oBitmap.unCbBits;

			return *this;
		}
		CDataStream& operator>>(TEmfLogPaletteEntry& oEntry)
		{
			*this >> oEntry.uchReserved;
			*this >> oEntry.uchBlue;
			*this >> oEntry.uchGreen;
			*this >> oEntry.uchRed;
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
			*this >> oPalette.ushNumberOfEntries;

			if (oPalette.ushNumberOfEntries > 0)
			{
				oPalette.pPaletteEntries = new TEmfLogPaletteEntry[oPalette.ushNumberOfEntries];
				if (!oPalette.pPaletteEntries)
				{
					oPalette.ushNumberOfEntries = 0;
					oPalette.pPaletteEntries  = NULL;
					return *this;
				}

				for (unsigned short ushIndex = 0; ushIndex < oPalette.ushNumberOfEntries; ushIndex++)
				{
					*this >> oPalette.pPaletteEntries[ushIndex];
				}
			}
			else
				oPalette.pPaletteEntries = NULL;

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
		template<typename T>
		CDataStream& operator>>(TPolyTextout<T>& oText)
		{
			*this >> oText.oBounds;
			*this >> oText.unIGraphicsMode;
			*this >> oText.dExScale;
			*this >> oText.dEyScale;
			*this >> oText.unCStrings;

			if (0 != oText.unCStrings)
			{
				oText.arEmrText = new TEmrText<T>[oText.unCStrings];
				if (!oText.arEmrText)
					return *this;

				unsigned int nStartPos = Tell();
				for (unsigned int unIndex = 0; unIndex < oText.unCStrings; unIndex++)
				{
					unsigned int nCurPos = Tell();
					ReadEmrText(oText.arEmrText[unIndex], nCurPos - nStartPos + 36); // 8 + 28 (8 - тип и размер, 28 - размер данной структуры)
				}
			}
			else
			{
				oText.arEmrText = NULL;
			}

			return *this;
		}
		CDataStream& operator>>(TEmfSmallTextout& oText)
		{
			*this >> oText.nX;
			*this >> oText.nY;
			*this >> oText.unCChars;
			*this >> oText.unFuOptions;
			*this >> oText.unIGraphicsMode;
			*this >> oText.dExScale;
			*this >> oText.dEyScale;

			if (!(oText.unFuOptions & ETO_NO_RECT))
				*this >> oText.oBounds;

			oText.pTextString = NULL;
			if (oText.unCChars)
			{
				unsigned short* pUnicode = NULL;
				if (oText.unFuOptions & ETO_SMALL_CHARS)
				{
					unsigned char* pString = new unsigned char[oText.unCChars];
					if (!pString)
						return *this;

					ReadBytes(pString, oText.unCChars);

					pUnicode = new unsigned short[oText.unCChars];
					if (!pUnicode)
					{
						delete[] pString;
						return *this;
					}

					for (unsigned int unIndex = 0; unIndex < oText.unCChars; unIndex++)
					{
						pUnicode[unIndex] = pString[unIndex];
					}
					delete[] pString;
				}
				else
				{
					pUnicode = new unsigned short[oText.unCChars];
					if (!pUnicode)
						return *this;

					ReadBytes(pUnicode, oText.unCChars);
				}
				oText.pTextString = pUnicode;
			}

			return *this;
		}
		CDataStream& operator>>(TEmfAlphaBlend& oBitmap)
		{
			*this >> oBitmap.oBounds;
			*this >> oBitmap.nXDest;
			*this >> oBitmap.nYDest;
			*this >> oBitmap.nCxDest;
			*this >> oBitmap.nCyDest;
			*this >> oBitmap.uchBlendOperation;
			*this >> oBitmap.uchBlendFlags;
			*this >> oBitmap.uchSrcConstantAlpha;
			*this >> oBitmap.uchAlphaFormat;
			*this >> oBitmap.nXSrc;
			*this >> oBitmap.nYSrc;
			*this >> oBitmap.oXformSrc;
			*this >> oBitmap.oBkColor;
			*this >> oBitmap.unUsageSrc;
			*this >> oBitmap.unOffBmiSrc;
			*this >> oBitmap.unCbBmiSrc;
			*this >> oBitmap.unOffBitsSrc;
			*this >> oBitmap.unCbBitsSrc;
			*this >> oBitmap.nCxSrc;
			*this >> oBitmap.nCySrc;

			return *this;
		}
		CDataStream& operator>>(TTriVertex& oVertex)
		{
			*this >> oVertex.nX;
			*this >> oVertex.nY;
			*this >> oVertex.ushRed;
			*this >> oVertex.ushGreen;
			*this >> oVertex.ushBlue;
			*this >> oVertex.ushAlpha;

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
		CDataStream& operator>>(TWmfLogBrush& oBrush)
		{
			*this >> oBrush.ushBrushStyle;
			*this >> oBrush.oColor;
			*this >> oBrush.ushBrushHatch;

			return *this;
		}
		CDataStream& operator>>(CWmfFont* pFont)
		{
			*this >> pFont->shHeight;
			*this >> pFont->shWidth;
			*this >> pFont->shEscapement;
			*this >> pFont->shOrientation;
			*this >> pFont->shWeight;
			*this >> pFont->uchItalic;
			*this >> pFont->uchUnderline;
			*this >> pFont->uchStrikeOut;
			*this >> pFont->uchCharSet;
			*this >> pFont->uchOutPrecision;
			*this >> pFont->uchClipPrecision;
			*this >> pFont->uchQuality;
			*this >> pFont->uchPitchAndFamily;

			// Читаем до тех пор пока не встретим нулевой символ
			unsigned char unIndex = 0;
			*this >> pFont->uchFacename[unIndex];
			while (0x00 != pFont->uchFacename[unIndex])
			{
				unIndex++;
				if (32 == unIndex)
					break;

				*this >> pFont->uchFacename[unIndex];
			}

			return *this;
		}
		CDataStream& operator>>(TWmfPaletteEntry& oEntry)
		{
			*this >> oEntry.uchValues;
			*this >> oEntry.uchBlue;
			*this >> oEntry.uchGreen;
			*this >> oEntry.uchRed;
			return *this;
		}
		CDataStream& operator>>(CWmfPalette* pPalette)
		{
			*this >> pPalette->ushStart;
			*this >> pPalette->ushNumberOfEntries;

			if (pPalette->ushNumberOfEntries > 0)
			{
				pPalette->pPaletteEntries = new TWmfPaletteEntry[pPalette->ushNumberOfEntries];
				if (!pPalette->pPaletteEntries)
				{
					pPalette->pPaletteEntries = NULL;
					pPalette->ushNumberOfEntries = 0;
					return *this;
				}

				for (unsigned short ushIndex = 0; ushIndex < pPalette->ushNumberOfEntries; ushIndex++)
				{
					*this >> pPalette->pPaletteEntries[ushIndex];
				}
			}
			else
			{
				pPalette->pPaletteEntries = NULL;
			}

			return *this;
		}
		CDataStream& operator>>(CWmfPen& oPen)
		{
			*this >> oPen.ushPenStyle;
			*this >> oPen.oWidth;
			*this >> oPen.oColor;

			return *this;
		}
		CDataStream& operator>>(TWmfScanLine& oLine)
		{
			*this >> oLine.ushLeft;
			*this >> oLine.ushRight;

			return *this;
		}
		CDataStream& operator>>(TWmfScanObject& oScan)
		{
			*this >> oScan.ushCount;
			*this >> oScan.ushTop;
			*this >> oScan.ushBottom;

			if (oScan.ushCount > 0 && !(oScan.ushCount & 1)) // Должно делиться на 2
			{
				unsigned short ushCount = oScan.ushCount >> 1;
				oScan.pScanLines = new TWmfScanLine[ushCount];
				if (oScan.pScanLines)
				{
					for (unsigned short ushIndex = 0; ushIndex < ushCount; ushIndex++)
					{
						*this >> oScan.pScanLines[ushIndex];
					}
				}
				else
				{
					oScan.pScanLines = NULL;
					oScan.ushCount = 0;
				}
			}
			else
			{
				oScan.pScanLines = NULL;
			}
			*this >> oScan.ushCount2;

			return *this;
		}
		CDataStream& operator>>(CWmfRegion* pRegion)
		{
			*this >> pRegion->shNextInChain;
			*this >> pRegion->shObjectType;

			if (0x0006 != pRegion->shObjectType)
				return *this;

			*this >> pRegion->shObjectCount;
			*this >> pRegion->shRegionSize;
			*this >> pRegion->shScanCount;
			*this >> pRegion->shMaxScan;
			*this >> pRegion->oBoundingRectangle;

			if (pRegion->shScanCount > 0)
			{
				pRegion->pScans = new TWmfScanObject[pRegion->shScanCount];
				if (!pRegion->pScans)
				{
					pRegion->pScans = NULL;
					pRegion->shScanCount = 0;
					return *this;
				}
				for (unsigned short ushIndex = 0; ushIndex < pRegion->shScanCount; ushIndex++)
				{
					*this >> pRegion->pScans[ushIndex];
				}
			}
			else
			{
				pRegion->shScanCount = NULL;
			}


			return *this;
		}
		CDataStream& operator>>(TWmfStretchBlt& oBitmap)
		{
			*this >> oBitmap.unRasterOperation;
			*this >> oBitmap.shSrcHeight;
			*this >> oBitmap.shSrcWidth;
			*this >> oBitmap.shYSrc;
			*this >> oBitmap.shXSrc;
			*this >> oBitmap.shDestHeight;
			*this >> oBitmap.shDestWidth;
			*this >> oBitmap.shYDest;
			*this >> oBitmap.shXDest;
			return *this;
		}
		CDataStream& operator>>(TWmfBitmap16& oBitmap)
		{
			*this >> oBitmap.shType;
			*this >> oBitmap.shWidth;
			*this >> oBitmap.shHeight;
			*this >> oBitmap.shWidthBytes;
			*this >> oBitmap.uchPlanes;
			*this >> oBitmap.uchBitsPixel;

			unsigned int unBitsCount = (((oBitmap.shWidth * oBitmap.uchBitsPixel + 15) >> 4) << 1) * oBitmap.shHeight;
			if (CanRead() >= unBitsCount)
			{
				//oBitmap.Bits = new unsigned char[unBitsCount];
			}
			else
			{
				oBitmap.pBits   = NULL;
				oBitmap.shWidth  = 0;
				oBitmap.shHeight = 0;
			}

			return *this;
		}
		CDataStream& operator>>(TWmfBitBlt& oBitmap)
		{
			*this >> oBitmap.unRasterOperation;
			*this >> oBitmap.shYSrc;
			*this >> oBitmap.shXSrc;
			*this >> oBitmap.shHeight;
			*this >> oBitmap.shWidth;
			*this >> oBitmap.shYDest;
			*this >> oBitmap.shXDest;
			return *this;
		}
		CDataStream& operator>>(TWmfSetDibToDev& oBitmap)
		{
			*this >> oBitmap.ushColorUsage;
			*this >> oBitmap.ushScanCount;
			*this >> oBitmap.ushStartScan;
			*this >> oBitmap.ushYDib;
			*this >> oBitmap.ushXDib;
			*this >> oBitmap.ushHeight;
			*this >> oBitmap.ushWidth;
			*this >> oBitmap.ushYDest;
			*this >> oBitmap.ushXDest;
			return *this;
		}
		CDataStream& operator>>(TWmfStretchDib& oBitmap)
		{
			*this >> oBitmap.unRasterOperation;
			*this >> oBitmap.ushColorUsage;
			*this >> oBitmap.shSrcHeight;
			*this >> oBitmap.shSrcWidth;
			*this >> oBitmap.shYSrc;
			*this >> oBitmap.shXSrc;
			*this >> oBitmap.shDestHeight;
			*this >> oBitmap.shDestWidth;
			*this >> oBitmap.shYDst;
			*this >> oBitmap.shXDst;
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
			if (pCur >= pBufferEnd)
				return true;

			return false;
		}

		unsigned int Tell() const
		{
			return (unsigned int)(pCur - pBuffer);
		}

		void Skip(unsigned int ulSkip)
		{
			if (pCur + ulSkip >= pEnd)
				pCur = pEnd;
			else
				pCur += ulSkip;
		}

		void SeekBack(unsigned int ulSkipBack)
		{
			pCur -= ulSkipBack;
		}

		void SeekToStart()
		{
			pCur = pBuffer;
			ClearCurrentBlockSize();
		}

		unsigned int CanRead() const
		{
			return (unsigned int)(pEnd - pCur);
		}

	private:

		template<typename T>void ReadEmrTextBase(TEmrText<T>& oText, unsigned int unOffset)
		{
			*this >> oText;

			// Читаем OutputString
			oText.unChars = std::min(oText.unChars, (UINT)(CanRead() / sizeof(T)));

			if (0 == oText.unChars)
				return;

			if (oText.unOffString - 40 > unOffset)
				Skip(oText.unOffString - (unOffset + 40)); // 40 - размер структуры TEmfEmrText

			T* pString = new T[oText.unChars + 1];
			if (pString)
			{
				pString[oText.unChars] = 0x00;
				ReadBytes(pString, oText.unChars);
				oText.pOutputString = pString;
			}

			// Читаем OutputDx
			if (oText.unChars < (UINT32_MAX / 2) && (oText.unOffDx > oText.unOffString) && (oText.unOffDx - oText.unOffString > 2 * oText.unChars))
				Skip(oText.unOffDx - oText.unOffString - 2 * oText.unChars);

			const unsigned int unDxCount = (oText.unOptions & ETO_PDY ? 2 * oText.unChars : oText.unChars);

			if ((CanRead() / 4) < unDxCount || 0 == unDxCount)
				return;

			unsigned int* pDx = new unsigned int[unDxCount];
			if (pDx)
			{
				ReadBytes(pDx, unDxCount);
				oText.pOutputDx = pDx;
			}
		}
		
		template<typename T>
		void ReadEmrText(TEmrText<T>& oText, unsigned int unOffset)
		{
			ReadEmrTextBase<T>(oText, unOffset);
		}
	private:

		BYTE *pBuffer;
		BYTE *pBufferEnd;
		BYTE *pCur;
		BYTE *pEnd;
	};

	void ReadImage(BYTE* pHeaderBuffer, unsigned int ulHeaderBufferLen, BYTE* pImageBuffer, unsigned int ulImageBufferLen, BYTE** ppDstBuffer, unsigned int* pulWidth, unsigned int* pulHeight);
	void ReadImage(BYTE* pImageBuffer, unsigned int unBufferLen, unsigned int unColorUsage, BYTE** ppDstBuffer, unsigned int* punWidth, unsigned int* punHeight);
	double GetEllipseAngle(int nL, int nT, int nR, int nB, int nX, int nY);
	void ProcessRasterOperation(unsigned int unRasterOperation, BYTE** ppBgra, unsigned int unWidth, unsigned int unHeight);
	std::wstring GetTempFilename(const std::wstring& sFolder = L"");

	std::wstring StringNormalization(const std::wstring& wsString);
	bool StringEquals(const std::wstring& wsFirstString, const std::wstring& wsSecondString);
};
#endif // _METAFILE_COMMON_METAFILEUTILS_H
