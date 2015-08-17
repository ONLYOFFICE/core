
#include "PaneType.h"
//#include <Exception/WrongBiffRecord.h>

namespace XLS
{


BiffStructurePtr PaneType::clone()
{
	return BiffStructurePtr(new PaneType(*this));
}


} // namespace XLS

