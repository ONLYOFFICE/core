#ifndef VTCOLOR_H
#define VTCOLOR_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CVtColor : public IChartObject
{
	bool m_bAutomatic;
	int m_nBlue;
	int m_nGreen;
	int m_nRed;
	int m_nValue;
public:
	CVtColor();

	ETypes GetType() const override;
};
}}

#endif // VTCOLOR_H
