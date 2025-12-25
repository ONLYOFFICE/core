#ifndef BACKDROP_H
#define BACKDROP_H

#include "Frame.h"
#include "Fill.h"
#include "Shadow.h"

namespace HWP { namespace CHART
{
class CBackdrop : public IChartObject
{
	CFrame m_oFrame;
	CFill m_oFill;
	CShadow m_oShadow;
public:
	CBackdrop();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // BACKDROP_H
