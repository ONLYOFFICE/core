
#include "PictFmlaKey.h"
#include <Binary/CFRecord.h>

namespace XLS
{


PictFmlaKey::PictFmlaKey()
:	fmlaLinkedCell(false), fmlaListFillRange(false)
{
}

BiffStructurePtr PictFmlaKey::clone()
{
	return BiffStructurePtr(new PictFmlaKey(*this));
}


void PictFmlaKey::store(CFRecord& record)
{
#pragma message("####################### PictFmlaKey record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! PictFmlaKey record has no BiffStructure::fromXML() implemented.");
}


void PictFmlaKey::load(CFRecord& record)
{
	_UINT32 cbKey;
	record >> cbKey;

	record.skipNunBytes(cbKey); // ActiveX license key is here

	fmlaLinkedCell.load(record);
	fmlaListFillRange.load(record);
}


} // namespace XLS

