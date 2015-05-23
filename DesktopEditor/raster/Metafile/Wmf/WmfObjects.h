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
		std::wstring GetDibPatterPath();
		void GetBounds(double& left, double& top, double& width, double& height) {}

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
		int          GetHeight()
		{
			return (int)Height;
		}
		std::wstring GetFaceName()
		{
			return NSString::CConverter::GetUnicodeFromSingleByteString((const unsigned char*)Facename, 32);
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
		unsigned int GetWidth()
		{
			return (unsigned int)Width.x;
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

#endif //_METAFILE_WMF_WMFOBJECTS_H
