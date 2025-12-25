#ifndef LRECT_H
#define LRECT_H

#include "Coor.h"

namespace HWP { namespace CHART
{
class CLRect : public IChartObject
{
	CCoor m_oMax;
	CCoor m_oMin;
public:
	CLRect();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // LRECT_H
