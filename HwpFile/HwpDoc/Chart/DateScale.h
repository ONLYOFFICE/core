#ifndef DATESCALE_H
#define DATESCALE_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CDateScale : public IChartObject
{
	bool m_bAuto;
	int m_nMajFreq;
	int m_nMajInt;
	double m_dMaximum;
	double m_dMinimum;
	int m_nMinFreq;
	int m_nMinInt;
	bool m_bSkipWeekend;
public:
	CDateScale();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // DATESCALE_H
