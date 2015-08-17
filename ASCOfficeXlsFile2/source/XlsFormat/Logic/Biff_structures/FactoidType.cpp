
#include "FactoidType.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{


XLS::BiffStructurePtr FactoidType::clone()
{
	return XLS::BiffStructurePtr(new FactoidType(*this));
}


void FactoidType::store(XLS::CFRecord& record)
{
#pragma message("####################### FactoidType record has no BiffStructure::store() implemented")
	Log::error(" Error!!! FactoidType record has no BiffStructure::store() implemented.");
	//record << something;
}


void FactoidType::load(XLS::CFRecord& record)
{
	unsigned int cbFactoid;
	record >> cbFactoid >> id >> rgbUri >> rgbTag >> rgbDownLoadURL;
}


} // namespace OSHARED

