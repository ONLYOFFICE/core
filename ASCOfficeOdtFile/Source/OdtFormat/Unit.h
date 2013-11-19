#pragma once
#ifndef ODT_UNIT_INCLUDE_H_
#define ODT_UNIT_INCLUDE_H_

#include "Utility.h"


namespace Odt
{
	typedef Unit<double, Pt>				UnitPt;
	typedef Unit<double, Cm, 3>	UnitCm;
	typedef Unit<int, Multi>		UnitMulti;
	typedef Unit<int, Percent>	UnitPercent;

} // namespace Odt

#endif // ODT_UNIT_INCLUDE_H_