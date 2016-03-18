
#include "OfficeArtFRIT.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


XLS::BiffStructurePtr OfficeArtFRIT::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFRIT(*this));
}


void OfficeArtFRIT::store(XLS::CFRecord& record)
{
	record << fridNew << fridOld;
}


void OfficeArtFRIT::load(XLS::CFRecord& record)
{
	record >> fridNew >> fridOld;
}


} // namespace XLS
