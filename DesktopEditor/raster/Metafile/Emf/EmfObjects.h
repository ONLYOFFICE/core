#ifndef _EMF_OBJECTS_H
#define _EMF_OBJECTS_H

#include "EmfTypes.h"
#include "../Wmf/WmfTypes.h"

namespace MetaFile
{
	typedef enum
	{
		EMF_OBJECT_UNKNOWN = 0x00,
		EMF_OBJECT_BRUSH   = 0x01,
		EMF_OBJECT_FONT    = 0x02,
		EMF_OBJECT_PEN     = 0x03
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

		CEmfLogBrushEx()
		{
			BrushStyle = BS_SOLID;
			BrushHatch = HS_HORIZONTAL;
		}
		virtual ~CEmfLogBrushEx()
		{
		}
		virtual EEmfObjectType GetType()
		{
			return EMF_OBJECT_BRUSH;
		}

	public:
		unsigned long BrushStyle;
		TEmfColor     Color;
		unsigned long BrushHatch;
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
		CEmfLogPen()
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
}

#endif // _EMF_OBJECTS_H