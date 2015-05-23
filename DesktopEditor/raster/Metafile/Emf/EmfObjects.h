#ifndef _METAFILE_EMF_EMFOBJECTS_H
#define _METAFILE_EMF_EMFOBJECTS_H

#include "EmfTypes.h"
//#include "../../common/Types.h"
#include "../Common/MetaFileObjects.h"
#include "../../../common/File.h"

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

	class CEmfLogBrushEx : public CEmfObjectBase, public IBrush
	{
	public:

		CEmfLogBrushEx();
		virtual ~CEmfLogBrushEx();
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_BRUSH;
		}
		void SetDibPattern(unsigned char* pBuffer, unsigned int ulWidth, unsigned int ulHeight);

		// IBrush
		int          GetColor();
		int          GetColor2()
		{
			return 0;
		}
		unsigned int GetStyle()
		{
			return BrushStyle;
		}
		unsigned int GetStyleEx()
		{
			return 0;
		}
		unsigned int GetHatch()
		{
			return BrushHatch;
		}
		unsigned int GetAlpha()
		{
			return BrushAlpha;
		}
		std::wstring GetDibPatterPath()
		{
			return DibPatternPath;
		}
		void GetBounds(double& left, double& top, double& width, double& height) {}

	public:
		unsigned int   BrushStyle;
		TEmfColor      Color;
		unsigned int   BrushHatch;
		unsigned int   BrushAlpha;
		std::wstring   DibPatternPath;
		unsigned char* DibBuffer;
		unsigned int   DibWidth;
		unsigned int   DibHeigth;
	};

	class CEmfLogFont : public CEmfObjectBase, public IFont
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

		// IFont
		int          GetHeight()
		{
			return LogFontEx.LogFont.Height;
		}
		std::wstring GetFaceName()
		{
			return NSFile::CUtf8Converter::GetWStringFromUTF16(LogFontEx.LogFont.FaceName, 32);
		}
		int          GetWeight()
		{
			return LogFontEx.LogFont.Weight;
		}
		bool         IsItalic()
		{
			return (0x01 == LogFontEx.LogFont.Italic ? true : false);
		}
		bool         IsStrikeOut()
		{
			return (0x01 == LogFontEx.LogFont.StrikeOut ? true : false);
		}
		bool         IsUnderline()
		{
			return (0x01 == LogFontEx.LogFont.Underline ? true : false);
		}
		int          GetEscapement()
		{
			return LogFontEx.LogFont.Escapement;
		}
		int          GetCharSet()
		{
			return LogFontEx.LogFont.CharSet;
		}

	public:

		TEmfLogFontEx    LogFontEx;
		TEmfDesignVector DesignVector;
	};

	class CEmfLogPen : public CEmfObjectBase, public IPen
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

		// IPen
		int          GetColor();
		unsigned int GetStyle()
		{
			return PenStyle;
		}
		unsigned int GetWidth()
		{
			return Width;
		}

	public:

		unsigned int  PenStyle;
		unsigned int  Width;
		TEmfColor     Color;
		unsigned int  NumStyleEntries;
		unsigned int* StyleEntry;
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

#endif // _METAFILE_EMF_EMFOBJECTS_H
