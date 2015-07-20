
#include "SHAPEPROPS.h"
#include <Logic/Biff_records/ShapePropsStream.h>
#include <Logic/Biff_records/ContinueFrt12.h>

//#include <Logic/Biff_unions/PAGESETUP.h>

namespace XLS
{;


SHAPEPROPS::SHAPEPROPS()
{
}


SHAPEPROPS::~SHAPEPROPS()
{
}


BaseObjectPtr SHAPEPROPS::clone()
{
	return BaseObjectPtr(new SHAPEPROPS(*this));
}


// SHAPEPROPS = ShapePropsStream *ContinueFrt12
const bool SHAPEPROPS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<ShapePropsStream>())
	{
		return false;
	}
	proc.repeated<ContinueFrt12>(0, 0);

	return true;
}

} // namespace XLS

