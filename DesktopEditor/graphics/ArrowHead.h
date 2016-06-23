#ifndef _BUILD_ARROW_HEAD_H_
#define _BUILD_ARROW_HEAD_H_

#include "GraphicsPath.h"

namespace Aggplus
{
	class CArrowHead
	{
	private:
		LONG			m_lType;
		CMatrix			m_oTransform;

		bool m_bIsMoveTo;		
		CGraphicsPath	m_oPath;
		
	public:
		double m_dX1;
		double m_dY1;
		double m_dX2;
		double m_dY2;

	public:
		CArrowHead();
		~CArrowHead();

	public:

		void SetType(LONG lType);
		bool IsNeed();
		
		void PathCommandMoveTo(double fX, double fY);
		void PathCommandLineTo(double fX, double fY);
		void PathCommandLinesTo(double* pPoints, LONG lCount);
		void PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3);
		void PathCommandCurvesTo(double* pPoints, LONG lCount);
		void PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);
		void PathCommandClose();
		void PathCommandEnd();
		void PathCommandStart();

		bool DrawPath(LONG lType);
	};
}

#endif // _BUILD_ARROW_HEAD_H_