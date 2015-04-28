#include "MetaFileTypes.h"
#include "../Emf/EmfTypes.h"
#include "../Wmf/WmfTypes.h"

namespace MetaFile
{
	TRect::TRect()
	{
		nLeft   = 0;
		nTop    = 0;
		nRight  = 1024;
		nBottom = 1024;
	}
	TRect& TRect::operator=(TWmfRect& oRect)
	{
		nLeft   = oRect.Left;
		nTop    = oRect.Top;
		nRight  = oRect.Right;
		nBottom = oRect.Bottom;
		return *this;
	}

	TPointL::TPointL()
	{
		x = 0;
		y = 0;
	}
	TPointL::TPointL(TEmfPointL& oPoint)
	{
		x = oPoint.x;
		y = oPoint.y;
	}
	TPointL::TPointL(TWmfPointS& oPoint)
	{
		x = oPoint.x;
		y = oPoint.y;
	}
	TPointL& TPointL::operator=(TWmfPointS& oPoint)
	{
		x = oPoint.x;
		y = oPoint.y;
		return *this;
	}
	TPointL& TPointL::operator=(TEmfPointL& oPoint)
	{
		x = oPoint.x;
		y = oPoint.y;
		return *this;
	}
}