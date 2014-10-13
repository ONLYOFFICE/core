#include "precompiled_xls.h"
#include "Chart3DBarShape.h"

namespace XLS
{;

Chart3DBarShape::Chart3DBarShape()
{
}


Chart3DBarShape::~Chart3DBarShape()
{
}


BaseObjectPtr Chart3DBarShape::clone()
{
	return BaseObjectPtr(new Chart3DBarShape(*this));
}


void Chart3DBarShape::writeFields(CFRecord& record)
{
	record << riser << taper;
}


void Chart3DBarShape::readFields(CFRecord& record)
{
	record >> riser >> taper;
}

} // namespace XLS

