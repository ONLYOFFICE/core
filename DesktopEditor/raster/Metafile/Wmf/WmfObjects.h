#ifndef _WMF_OBJECTS_H
#define _WMF_OBJECTS_H

#include "WmfTypes.h"
#include "WmfUtils.h"

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
	class CWmfBrush : public CWmfObjectBase
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

	public:

		unsigned short BrushStyle;
		TWmfColor      Color;
		unsigned short BrushHatch;
		std::wstring   DibPatternPath;
		unsigned char* DibBuffer;
		unsigned int   DibWidth;
		unsigned int   DibHeigth;
	};
	class CWmfFont : public CWmfObjectBase
	{
	public:

		CWmfFont()
		{
			::memset(Facename, 0x00, 32);
		}
		~CWmfFont()
		{

		}

		virtual EWmfObjectType GetType()
		{
			return WMF_OBJECT_FONT;
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
	class CWmfPen : public CWmfObjectBase
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
	public:
		unsigned short PenStyle;
		TWmfPointS     Width;
		TWmfColor      Color;
	};
	class CWmfRegion : public CWmfObjectBase
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
		short           RegionSize;
		short           ScanCount;
		short           MaxScan;
		TWmfRect        BoundingRectangle;
		TWmfScanObject* aScans;
	};
}

#endif //_WMF_OBJECTS_H