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
		CEmfPathMoveTo(int lX, int lY)
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
		
		int x;
		int y;
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
		CEmfPathLineTo(int lX, int lY)
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

		int x;
		int y;
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
		CEmfPathCurveTo(int lX1, int lY1, int lX2, int lY2, int lXE, int lYE)
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

		int x1;
		int y1;
		int x2;
		int y2;
		int xE;
		int yE;
	};
	class CEmfPathArcTo : public CEmfPathCommandBase
	{
	public:
		CEmfPathArcTo(int lL, int lT, int lR, int lB, double dStart, double dSweep)
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

		int   left;
		int   top;
		int   right;
		int   bottom;
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
		CEmfPath(CEmfPath* pPath);
		~CEmfPath();

		bool MoveTo(TEmfPointS& oPoint);
		bool MoveTo(TEmfPointL& oPoint);
		bool MoveTo(int lX, int lY);
		bool LineTo(TEmfPointS& oPoint);
		bool LineTo(TEmfPointL& oPoint);
		bool LineTo(int lX, int lY);
		bool CurveTo(TEmfPointS& oPoint1, TEmfPointS& oPoint2, TEmfPointS& oPointE);
		bool CurveTo(TEmfPointL& oPoint1, TEmfPointL& oPoint2, TEmfPointL& oPointE);
		bool CurveTo(int lX1, int lY1, int lX2, int lY2, int lXE, int lYE);
		bool ArcTo(int lL, int lT, int lR, int lB, double dStart, double dSweep);
		bool Close();
		void Draw(CEmfOutputDevice* pOutput, bool bStroke, bool bFill);

	private:

		void Clear();

	public:

		std::vector<CEmfPathCommandBase*> m_pCommands;
	};
}

#endif //_EMF_PATH_H