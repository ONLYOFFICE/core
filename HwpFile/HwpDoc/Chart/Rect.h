#ifndef RECT_H
#define RECT_H

#include "Coor.h"

namespace HWP { namespace CHART
{
class CRect : public IChartObject
{
	CCoor m_oMin;
	CCoor m_oMax;
public:
	CRect();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // RECT_H
