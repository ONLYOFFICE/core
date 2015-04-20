#ifndef _EMF_OBJECTS_H
#define _EMF_OBJECTS_H

#include "EmfTypes.h"
#include "../Wmf/WmfUtils.h"
#include "../Wmf/WmfTypes.h"
#include "../../common/Types.h"

namespace MetaFile
{
	typedef enum
	{
		EMF_OBJECT_UNKNOWN = 0x00,
		EMF_OBJECT_BRUSH   = 0x01,
		EMF_OBJECT_FONT    = 0x02,
		EMF_OBJECT_PEN     = 0x03,
		EMF_OBJECT_PALETTE = 0x04
	} EEmfObjectType;

	class CEmfObjectBase
	{
	public:
		CEmfObjectBase(){}
		virtual ~CEmfObjectBase(){}
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_UNKNOWN;
		}
	};

	class CEmfLogBrushEx : public CEmfObjectBase
	{
	public:

		CEmfLogBrushEx();
		virtual ~CEmfLogBrushEx();
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_BRUSH;
		}
		void SetDibPattern(unsigned char* pBuffer, unsigned long ulWidth, unsigned long ulHeight);

	public:
		unsigned long  BrushStyle;
		TEmfColor      Color;
		unsigned long  BrushHatch;
		unsigned long  BrushAlpha;
		std::wstring   DibPatternPath;
		unsigned char* DibBuffer;
		unsigned long  DibWidth;
		unsigned long  DibHeigth;
	};

	class CEmfLogFont : public CEmfObjectBase
	{
	public:
		CEmfLogFont()
		{
			DesignVector.Values = NULL;
		}
		virtual ~CEmfLogFont()
		{
			if (DesignVector.Values)
				delete[] DesignVector.Values;
		}
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_FONT;
		}

	public:

		TEmfLogFontEx    LogFontEx;
		TEmfDesignVector DesignVector;
	};

	class CEmfLogPen : public CEmfObjectBase
	{
	public:
		CEmfLogPen() : PenStyle(PS_SOLID), Width(1), Color(0, 0, 0)
		{
			StyleEntry = NULL;
		}
		virtual ~CEmfLogPen()
		{
			if (StyleEntry)
				delete[] StyleEntry;
		}
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_PEN;
		}

	public:

		unsigned long  PenStyle;
		unsigned long  Width;
		TEmfColor      Color;
		unsigned long  NumStyleEntries;
		unsigned long* StyleEntry;
	};

	class CEmfLogPalette : public CEmfObjectBase
	{
	public:
		CEmfLogPalette()
		{
			NumberOfEntries = 0;
			PaletteEntries  = NULL;
		}
		virtual ~CEmfLogPalette()
		{
			if (PaletteEntries)
				delete[] PaletteEntries;
		}
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_PALETTE;
		}

	public:

		unsigned short       NumberOfEntries;
		TEmfLogPaletteEntry* PaletteEntries;
	};
}

#endif // _EMF_OBJECTS_H
