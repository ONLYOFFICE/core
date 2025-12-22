#ifndef VTPICTURE_H
#define VTPICTURE_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CVtPicture : public IChartObject
{
	bool m_bEmbedded;
	HWP_STRING m_sFilename;
	int m_nMap;
	int m_nType;
public:
	CVtPicture();

	ETypes GetType() const override;
};
}}

#endif // VTPICTURE_H
