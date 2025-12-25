#ifndef WALL_H
#define WALL_H

#include "Brush.h"
#include "Pen.h"

namespace HWP { namespace CHART
{
class CWall : public IChartObject
{
	CBrush m_oBrush;
	CPen m_oPen;
	SINGLE m_snWidth;
public:
	CWall();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // WALL_H
