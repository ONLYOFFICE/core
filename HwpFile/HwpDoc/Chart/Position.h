#ifndef POSITION_H
#define POSITION_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CPosition : public IChartObject
{
	bool m_bExcluded;
	bool m_bHidden;
	int m_nOrder;
	int m_nStackOrder;
public:
	CPosition();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // POSITION_H
