#ifndef SHADOW_H
#define SHADOW_H

#include "Brush.h"
#include "Coor.h"

namespace HWP { namespace CHART
{
class CShadow : public IChartObject
{
	CBrush m_oBrush;
	CCoor m_oOffset;
	int m_nStyle;
public:
	CShadow();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // SHADOW_H
