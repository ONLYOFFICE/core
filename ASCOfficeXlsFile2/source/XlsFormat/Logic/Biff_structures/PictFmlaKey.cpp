#include "precompiled_xls.h"
#include "PictFmlaKey.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PictFmlaKey::PictFmlaKey()
:	fmlaLinkedCell(false), fmlaListFillRange(false)
{
}

BiffStructurePtr PictFmlaKey::clone()
{
	return BiffStructurePtr(new PictFmlaKey(*this));
}


//void PictFmlaKey::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	fmlaLinkedCell.toXML(own_tag);
//	fmlaListFillRange.toXML(own_tag);
//}
//
//
//void PictFmlaKey::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	fmlaLinkedCell.fromXML(own_tag);
//	fmlaListFillRange.fromXML(own_tag);
//}


void PictFmlaKey::store(CFRecord& record)
{
#pragma message("####################### PictFmlaKey record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! PictFmlaKey record has no BiffStructure::fromXML() implemented.");
}


void PictFmlaKey::load(CFRecord& record)
{
	unsigned int cbKey;
	record >> cbKey;

	record.skipNunBytes(cbKey); // ActiveX license key is here

	fmlaLinkedCell.load(record);
	fmlaListFillRange.load(record);
}


} // namespace XLS

