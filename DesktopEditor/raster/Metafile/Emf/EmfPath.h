#ifndef _EMF_PATH_H
#define _EMF_PATH_H

#include "EmfTypes.h"
#include "../Wmf/WmfTypes.h"
#include <vector>

namespace MetaFile
{
	class CEmfOutputDevice;

	typedef enum
	{
		EMF_PATHCOMMAND_UNKNOWN = 0x00,
		EMF_PATHCOMMAND_MOVETO  = 0x01,
		EMF_PATHCOMMAND_LINETO  = 0x02,
		EMF_PATHCOMMAND_CURVETO = 0x03,
		EMF_PATHCOMMAND_ARCTO   = 0x04,
		EMF_PATHCOMMAND_CLOSE   = 0x05
	} EEmfPathCommandType;

	class CEmfPathCommandBase
	{
	public:
		CEmfPathCommandBase()
		{
		}
		virtual ~CEmfPathCommandBase()
		{
		}
		virtual EEmfPathCommandType GetType() = 0;
	};
	class CEmfPathMoveTo : public CEmfPathCommandBase
	{
	public:
		CEmfPathMoveTo(TEmfPointL& oPoint)
		{
			x = oPoint.x;
			y = oPoint.y;
		}
		CEmfPathMoveTo(TEmfPointS& oPoint)
		{
			x = oPoint.x;
			y = oPoint.y;
		}
		CEmfPathMoveTo(long lX, long lY)
		{
			x = lX;
			y = lY;
		}
		virtual ~CEmfPathMoveTo()
		{
		}
		virtual EEmfPathCommandType GetType()
		{
			return EMF_PATHCOMMAND_MOVETO;
		}

	public:
		
		long x;
		long y;
	};
	class CEmfPathLineTo : public CEmfPathCommandBase
	{
	public:
		CEmfPathLineTo(TEmfPointL& oPoint)
		{
			x = oPoint.x;
			y = oPoint.y;
		}
		CEmfPathLineTo(TEmfPointS& oPoint)
		{
			x = oPoint.x;
			y = oPoint.y;
		}
		CEmfPathLineTo(long lX, long lY)
		{
			x = lX;
			y = lY;
		}
		virtual ~CEmfPathLineTo()
		{
		}
		virtual EEmfPathCommandType GetType()
		{
			return EMF_PATHCOMMAND_LINETO;
		}

	public:

		long x;
		long y;
	};
	class CEmfPathCurveTo : public CEmfPathCommandBase
	{
	public:
		CEmfPathCurveTo(TEmfPointL& oPoint1, TEmfPointL& oPoint2, TEmfPointL& oPointE)
		{
			x1 = oPoint1.x;
			y1 = oPoint1.y;
			x2 = oPoint2.x;
			y2 = oPoint2.y;
			xE = oPointE.x;
			yE = oPointE.y;
		}
		CEmfPathCurveTo(TEmfPointS& oPoint1, TEmfPointS& oPoint2, TEmfPointS& oPointE)
		{
			x1 = oPoint1.x;
			y1 = oPoint1.y;
			x2 = oPoint2.x;
			y2 = oPoint2.y;
			xE = oPointE.x;
			yE = oPointE.y;
		}
		CEmfPathCurveTo(long lX1, long lY1, long lX2, long lY2, long lXE, long lYE)
		{
			x1 = lX1;
			y1 = lY1;
			x2 = lX2;
			y2 = lY2;
			xE = lXE;
			yE = lYE;
		}
		virtual ~CEmfPathCurveTo()
		{
		}
		virtual EEmfPathCommandType GetType()
		{
			return EMF_PATHCOMMAND_CURVETO;
		}

	public:

		long x1;
		long y1;
		long x2;
		long y2;
		long xE;
		long yE;
	};
	class CEmfPathArcTo : public CEmfPathCommandBase
	{
	public:
		CEmfPathArcTo(long lL, long lT, long lR, long lB, double dStart, double dSweep)
		{
			left   = lL;
			top    = lT;
			right  = lR;
			bottom = lB;
			start  = dStart;
			sweep  = dSweep;
		}
		virtual ~CEmfPathArcTo()
		{
		}
		virtual EEmfPathCommandType GetType()
		{
			return EMF_PATHCOMMAND_ARCTO;
		}

	public:

		long   left;
		long   top;
		long   right;
		long   bottom;
		double start;
		double sweep;
	};
	class CEmfPathClose : public CEmfPathCommandBase
	{
	public:
		CEmfPathClose()
		{
		}
		virtual ~CEmfPathClose()
		{
		}
		virtual EEmfPathCommandType GetType()
		{
			return EMF_PATHCOMMAND_CLOSE;
		}

	public:
	};

	class CEmfPath
	{
	public:

		CEmfPath();
		~CEmfPath();

		bool MoveTo(TEmfPointS& oPoint);
		bool MoveTo(TEmfPointL& oPoint);
		bool MoveTo(long lX, long lY);
		bool LineTo(TEmfPointS& oPoint);
		bool LineTo(TEmfPointL& oPoint);
		bool LineTo(long lX, long lY);
		bool CurveTo(TEmfPointS& oPoint1, TEmfPointS& oPoint2, TEmfPointS& oPointE);
		bool CurveTo(TEmfPointL& oPoint1, TEmfPointL& oPoint2, TEmfPointL& oPointE);
		bool CurveTo(long lX1, long lY1, long lX2, long lY2, long lXE, long lYE);
		bool ArcTo(long lL, long lT, long lR, long lB, double dStart, double dSweep);
		bool Close();
		void Draw(CEmfOutputDevice* pOutput, bool bStroke, bool bFill);

	private:

		void Clear();

	public:

		std::vector<CEmfPathCommandBase*> m_pCommands;
	};
}

#endif //_EMF_PATH_H