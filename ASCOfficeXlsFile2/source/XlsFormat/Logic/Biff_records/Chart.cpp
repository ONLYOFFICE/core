#include "precompiled_xls.h"
#include "Chart.h"

namespace XLS
{;

Chart::Chart()
{
}


Chart::~Chart()
{
}


BaseObjectPtr Chart::clone()
{
	return BaseObjectPtr(new Chart(*this));
}


void Chart::writeFields(CFRecord& record)
{
	record << x << y << dx << dy;
}


void Chart::readFields(CFRecord& record)
{
	record >> x >> y >> dx >> dy;
}

} // namespace XLS

