#ifndef _EMF_OBJECTS_H
#define _EMF_OBJECTS_H

#include "EmfTypes.h"
#include "../Wmf/WmfTypes.h"

namespace Metafile
{
	typedef enum
	{
		EMF_OBJECT_UNKNOWN = 0x00,
		EMF_OBJECT_BRUSH   = 0x01,
		EMF_OBJECT_FONT    = 0x02
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
}

#endif // _EMF_OBJECTS_H