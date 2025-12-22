#ifndef POSITION_H
#define POSITION_H

namespace HWP { namespace CHART
{
class CPosition
{
	bool m_bExcluded;
	bool m_bHidden;
	int m_nOrder;
	int m_nStackOrder;
public:
	CPosition();
};
}}

#endif // POSITION_H
