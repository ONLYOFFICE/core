#ifndef HILO_H
#define HILO_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CHiLo : public IChartObject
{
	CVtColor m_oGainColor;
	CVtColor m_oLossColor;
public:
	CHiLo();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // HILO_H
