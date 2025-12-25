#ifndef XYZ_H
#define XYZ_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CXYZ : public IChartObject
{
	bool m_bAutomatic;
	double m_dxIntersection;
	double m_dyIntersection;
	double m_dzIntersection;
public:
	CXYZ();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // XYZ_H
