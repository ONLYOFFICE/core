
#include "OfficeArtIDCL.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


XLS::BiffStructurePtr OfficeArtIDCL::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtIDCL(*this));
}

void OfficeArtIDCL::store(XLS::CFRecord& record)
{
	record << dgid << cspidCur;
}


void OfficeArtIDCL::load(XLS::CFRecord& record)
{
	record >> dgid >> cspidCur;
}


} // namespace XLS
