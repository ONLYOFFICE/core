
#include "HideObjEnum.h"

namespace XLS
{


BiffStructurePtr HideObjEnum::clone()
{
	return BiffStructurePtr(new HideObjEnum(*this));
}

} // namespace XLS

