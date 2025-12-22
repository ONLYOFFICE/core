#ifndef DATESCALE_H
#define DATESCALE_H

namespace HWP { namespace CHART
{
class CDateScale
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
};
}}

#endif // DATESCALE_H
