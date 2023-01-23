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
#ifndef _METAFILE_WMF_WMFOBJECTS_H
#define _METAFILE_WMF_WMFOBJECTS_H

#include "../Common/MetaFileTypes.h"
#include "../Common/MetaFileObjects.h"

#include "WmfTypes.h"

#ifdef __linux__
    #include <string.h> //memset oO
#endif
namespace MetaFile
{
	typedef enum
	{
		WMF_OBJECT_UNKNOWN = 0x00,
		WMF_OBJECT_BRUSH   = 0x01,
		WMF_OBJECT_FONT    = 0x02,
		WMF_OBJECT_PEN     = 0x03,
		WMF_OBJECT_PALETTE = 0x04,
		WMF_OBJECT_REGION  = 0x05
	} EWmfObjectType;
	class CWmfObjectBase
	{
	public:
		CWmfObjectBase(){}
		virtual ~CWmfObjectBase(){}
		virtual EWmfObjectType GetType()
		{
			return WMF_OBJECT_UNKNOWN;
		}
	};
	class CWmfBrush : public CWmfObjectBase, public IBrush
	{
	public:
		CWmfBrush();
		CWmfBrush(TWmfLogBrush& oBrush);
		virtual ~CWmfBrush();
		virtual EWmfObjectType GetType()
		{
			return WMF_OBJECT_BRUSH;
		}
		void SetDibPattern(unsigned char* pBuffer, unsigned int unWidth, unsigned int unHeight);

		// IBrush
		int          GetColor();
		int          GetColor2()
		{
			return 0;
		}
		unsigned int GetStyle();
		unsigned int GetStyleEx()
		{
			return 0;
		}
		unsigned int GetHatch();
		unsigned int GetAlpha();
		unsigned int GetAlpha2()
		{
			return 0xff;
		}
		std::wstring GetDibPatterPath();
		void GetBounds(double& left, double& top, double& width, double& height) {}
		void GetCenterPoint(double& dX, double& dY) {}

		void GetDibPattern(unsigned char** pBuffer, unsigned int &unWidth, unsigned int &unHeight)
		{
			*pBuffer	= DibBuffer;
			unWidth		= DibWidth;
			unHeight	= DibHeigth;
		}

	public:

		unsigned short BrushStyle;
		TWmfColor      Color;
		unsigned short BrushHatch;
		std::wstring   DibPatternPath;
		unsigned char* DibBuffer;
		unsigned int   DibWidth;
		unsigned int   DibHeigth;
	};
	class CWmfFont : public CWmfObjectBase, public IFont
	{
	public:

		CWmfFont()
		{
            memset(Facename, 0x00, 32);
		}
		~CWmfFont()
		{

		}

		virtual EWmfObjectType GetType()
		{
			return WMF_OBJECT_FONT;
		}

		// IFont
		double          GetHeight()
		{
			return (double)Height;
		}
		std::wstring GetFaceName()
		{
			return std::wstring(NSStringExt::CConverter::GetUnicodeFromSingleByteString((const unsigned char*)Facename, 32).c_str());
		}
		int          GetWeight()
		{
			return (int)Weight;
		}
		bool         IsItalic()
		{
			return (0x01 == Italic ? true : false);
		}
		bool         IsStrikeOut()
		{
			return (0x01 == StrikeOut ? true : false);
		}
		bool         IsUnderline()
		{
			return (0x01 == Underline ? true : false);
		}
		int          GetEscapement()
		{
			return (int)Escapement;
		}
		int          GetCharSet()
		{
			return (int)CharSet;
		}
		int          GetOrientation()
		{
			return (int)Orientation;
		}

	public:

		short         Height;
		short         Width;
		short         Escapement;
		short         Orientation;
		short         Weight;
		unsigned char Italic;
		unsigned char Underline;
		unsigned char StrikeOut;
		unsigned char CharSet;
		unsigned char OutPrecision;
		unsigned char ClipPrecision;
		unsigned char Quality;
		unsigned char PitchAndFamily;
		unsigned char Facename[32]; // Согласно спецификации длина имени не должна превышать 32 знака с учетом нулевого символа в конце
	};
	class CWmfPalette : public CWmfObjectBase
	{
	public:
		CWmfPalette()
		{
			aPaletteEntries = NULL;
			NumberOfEntries = 0;
		}
		~CWmfPalette()
		{
			if (aPaletteEntries)
				delete[] aPaletteEntries;
		}
		virtual EWmfObjectType GetType()
		{
			return WMF_OBJECT_PALETTE;
		}
	public:
		unsigned short    Start;
		unsigned short    NumberOfEntries;
		TWmfPaletteEntry* aPaletteEntries;
	};
	class CWmfPen : public CWmfObjectBase, public IPen
	{
	public:
		CWmfPen()
		{

		}
		~CWmfPen()
		{

		}
		virtual EWmfObjectType GetType()
		{
			return WMF_OBJECT_PEN;
		}

		// IPen
		int          GetColor();
		unsigned int GetStyle()
		{
			return (unsigned int)PenStyle;
		}
		double GetWidth()
		{
			return (double)Width.x;
		}
		unsigned int GetAlpha()
		{
			return 255;
		}
		double GetMiterLimit()
		{
			return 0;
		}
		double GetDashOffset()
		{
			return 0;
		}
		void GetDashData(double*& arDatas, unsigned int& unSize)
		{
			arDatas = NULL;
			unSize  = 0;
		}

	public:
		unsigned short PenStyle;
		TWmfPointS     Width;
		TWmfColor      Color;
	};
	class CWmfRegion : public CWmfObjectBase, public IRegion
	{
	public:
		CWmfRegion()
		{

		}
		~CWmfRegion()
		{

		}
		virtual EWmfObjectType GetType()
		{
			return WMF_OBJECT_REGION;
		}
	public:
		short           nextInChain; // не используется
		short           ObjectType;  // не используется
		int             ObjectCount; // не используется
		short           RegionSize;
		short           ScanCount;
		short           MaxScan;
		TWmfRect        BoundingRectangle;
		TWmfScanObject* aScans;
	};
}

#endif //_METAFILE_WMF_WMFOBJECTS_H
