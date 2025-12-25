#ifndef VALUESCALE_H
#define VALUESCALE_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CValueScale : public IChartObject
{
	bool m_bAuto;
	int m_nMajorDivision;
	double m_dMaximum;
	double m_dMinimum;
	int m_nMinorDivision;
public:
	CValueScale();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // VALUESCALE_H
