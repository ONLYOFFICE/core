#ifndef _METAFILE_EMF_EMFPATH_H
#define _METAFILE_EMF_EMFPATH_H

#include "EmfTypes.h"
#include "../Wmf/WmfTypes.h"
#include <vector>
#include "../Common/IOutputDevice.h"

namespace MetaFile
{
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
		CEmfPathMoveTo(double dX, double dY)
		{
			x = dX;
			y = dY;
		}
		virtual ~CEmfPathMoveTo()
		{
		}
		virtual EEmfPathCommandType GetType()
		{
			return EMF_PATHCOMMAND_MOVETO;
		}

	public:
		
		double x;
		double y;
	};
	class CEmfPathLineTo : public CEmfPathCommandBase
	{
	public:
		CEmfPathLineTo(double dX, double dY)
		{
			x = dX;
			y = dY;
		}
		virtual ~CEmfPathLineTo()
		{
		}
		virtual EEmfPathCommandType GetType()
		{
			return EMF_PATHCOMMAND_LINETO;
		}

	public:

		double x;
		double y;
	};
	class CEmfPathCurveTo : public CEmfPathCommandBase
	{
	public:
		CEmfPathCurveTo(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE)
		{
			x1 = dX1;
			y1 = dY1;
			x2 = dX2;
			y2 = dY2;
			xE = dXE;
			yE = dYE;
		}
		virtual ~CEmfPathCurveTo()
		{
		}
		virtual EEmfPathCommandType GetType()
		{
			return EMF_PATHCOMMAND_CURVETO;
		}

	public:

		double x1;
		double y1;
		double x2;
		double y2;
		double xE;
		double yE;
	};
	class CEmfPathArcTo : public CEmfPathCommandBase
	{
	public:
		CEmfPathArcTo(double dL, double dT, double dR, double dB, double dStart, double dSweep)
		{
			left   = dL;
			top    = dT;
			right  = dR;
			bottom = dB;
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

		double left;
		double top;
		double right;
		double bottom;
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

		bool MoveTo(double dX, double dY);
		bool LineTo(double dX, double dY);
		bool CurveTo(double dX1, double dY1, double dX2, double dY2, double dXE, double dYE);
		bool ArcTo(double dL, double dT, double dR, double dB, double dStart, double dSweep);
		bool Close();
		void Draw(IOutputDevice* pOutput, bool bStroke, bool bFill, unsigned int unClipMode = -1);

	private:

		void Clear();

	public:

		std::vector<CEmfPathCommandBase*> m_pCommands;
	};
}

#endif //_METAFILE_EMF_EMFPATH_H