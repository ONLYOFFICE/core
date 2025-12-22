#ifndef PRINTINFORMATION_H
#define PRINTINFORMATION_H

#include "../Common/Common.h"

namespace HWP { namespace CHART
{
class CPrintInformation
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
};
}}

#endif // PRINTINFORMATION_H
