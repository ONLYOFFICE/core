
#include "End.h"

namespace XLS
{

End::End()
{
}


End::~End()
{
}


BaseObjectPtr End::clone()
{
	return BaseObjectPtr(new End(*this));
}


void End::writeFields(CFRecord& record)
{

}


void End::readFields(CFRecord& record)
{
	// no data
}

} // namespace XLS

