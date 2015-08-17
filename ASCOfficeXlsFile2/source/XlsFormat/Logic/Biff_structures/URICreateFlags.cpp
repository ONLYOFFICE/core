
#include "URICreateFlags.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{


URICreateFlags::URICreateFlags()
{
}


URICreateFlags::URICreateFlags(XLS::CFRecord& record)
{
	load(record);
}


XLS::BiffStructurePtr URICreateFlags::clone()
{
	return XLS::BiffStructurePtr(new URICreateFlags(*this));
}


void URICreateFlags::store(XLS::CFRecord& record)
{
#pragma message("####################### URICreateFlags record has no BiffStructure::store() implemented")
	Log::error(" Error!!! URICreateFlags record has no BiffStructure::store() implemented.");
	//record << something;
}


void URICreateFlags::load(XLS::CFRecord& record)
{
	unsigned int flags;
	record >> flags;

	createAllowRelative = GETBIT(flags, 0);
	createAllowImplicitWildcardScheme = GETBIT(flags, 1);
	createAllowImplicitFileScheme = GETBIT(flags, 2);
	createNoFrag = GETBIT(flags, 3);
	createNoCanonicalize = GETBIT(flags, 4);
	createCanonicalize = GETBIT(flags, 5);
	createFileUseDosPath = GETBIT(flags, 6);
	createDecodeExtraInfo = GETBIT(flags, 7);
	createNoDecodeExtraInfo = GETBIT(flags, 8);
	createCrackUnknownSchemes = GETBIT(flags, 9);
	createNoCrackUnknownSchemes = GETBIT(flags, 10);
	createPreProcessHtmlUri = GETBIT(flags, 11);
	createNoPreProcessHtmlUri = GETBIT(flags, 12);
	createIESettings = GETBIT(flags, 13);
	createNoIESettings = GETBIT(flags, 14);
	createNoEncodeForbiddenCharacters = GETBIT(flags, 15);
}


} // namespace OSHARED

