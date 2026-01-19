#ifndef VTPICTURE_H
#define VTPICTURE_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CVtPicture : public IChartObject
{
	CHART_BOOLEAN m_bEmbedded;
	CHART_STRING m_sFilename;
	CHART_INTEGER m_nMap;
	CHART_INTEGER m_nType;
public:
	CVtPicture();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // VTPICTURE_H
