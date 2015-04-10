#ifndef _EMF_OBJECTS_H
#define _EMF_OBJECTS_H

#include "EmfTypes.h"
#include "../Wmf/WmfTypes.h"

namespace Metafile
{
	class CEmfObjectBase
	{
	public:
		CEmfObjectBase(){}
		virtual ~CEmfObjectBase(){}
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

	public:
		unsigned long BrushStyle;
		TEmfColor     Color;
		unsigned long BrushHatch;
	};
}

#endif // _EMF_OBJECTS_H