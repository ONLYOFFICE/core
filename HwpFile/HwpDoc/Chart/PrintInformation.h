#ifndef PRINTINFORMATION_H
#define PRINTINFORMATION_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CPrintInformation : public IChartObject
{
	SINGLE m_snBottomMargin;
	bool m_bCenterHorizontally;
	bool m_bCenterVertically;
	bool m_bLayoutForPrinter;
	SINGLE m_snLeftMargin;
	bool m_bMonochrome;
	int m_nOrientation;
	SINGLE m_snRightMargin;
	int m_nScaleType;
	SINGLE m_snTopMargin;
public:
	CPrintInformation();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // PRINTINFORMATION_H
