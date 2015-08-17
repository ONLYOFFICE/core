
#include "Excel9File.h"

namespace XLS
{

Excel9File::Excel9File()
{
}


Excel9File::~Excel9File()
{
}


BaseObjectPtr Excel9File::clone()
{
	return BaseObjectPtr(new Excel9File(*this));
}


void Excel9File::writeFields(CFRecord& record)
{
	// no data in this tag;
}


void Excel9File::readFields(CFRecord& record)
{
	// no data in this tag;
}

} // namespace XLS

