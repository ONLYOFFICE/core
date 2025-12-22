#ifndef WALL_H
#define WALL_H

#include "Brush.h"
#include "Pen.h"

namespace HWP { namespace CHART
{
class CWall
{
	CBrush m_oBrush;
	CPen m_oPen;
	SINGLE m_snWidth;
public:
	CWall();
};
}}

#endif // WALL_H
