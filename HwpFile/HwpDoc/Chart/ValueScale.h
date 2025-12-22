#ifndef VALUESCALE_H
#define VALUESCALE_H

namespace HWP { namespace CHART
{
class CValueScale
{
	bool m_bAuto;
	int m_nMajorDivision;
	double m_dMaximum;
	double m_dMinimum;
	int m_nMinorDivision;
public:
	CValueScale();
};
}}

#endif // VALUESCALE_H
