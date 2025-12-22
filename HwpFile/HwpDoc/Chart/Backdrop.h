#ifndef BACKDROP_H
#define BACKDROP_H

#include "Frame.h"
#include "Fill.h"
#include "Shadow.h"

namespace HWP { namespace CHART
{
class CBackdrop
{
	CFrame m_oFrame;
	CFill m_oFill;
	CShadow m_oShadow;
public:
	CBackdrop();
};
}}

#endif // BACKDROP_H
