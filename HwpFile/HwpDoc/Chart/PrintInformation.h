#ifndef PRINTINFORMATION_H
#define PRINTINFORMATION_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CPrintInformation : public IChartObject
{
	CHART_SINGLE m_snBottomMargin;
	CHART_BOOLEAN m_bCenterHorizontally;
	CHART_BOOLEAN m_bCenterVertically;
	CHART_BOOLEAN m_bLayoutForPrinter;
	CHART_SINGLE m_snLeftMargin;
	CHART_BOOLEAN m_bMonochrome;
	CHART_INTEGER m_nOrientation;
	CHART_SINGLE m_snRightMargin;
	CHART_INTEGER m_nScaleType;
	CHART_SINGLE m_snTopMargin;
public:
	CPrintInformation();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // PRINTINFORMATION_H
