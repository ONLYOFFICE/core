
#include "CFColor.h"
#include <Binary/CFRecord.h>

namespace XLS
{


CFColor::CFColor()
{
}


CFColor::CFColor(CFRecord& record)
{
	load(record);
}


BiffStructurePtr CFColor::clone()
{
	return BiffStructurePtr(new CFColor(*this));
}


void CFColor::store(CFRecord& record)
{
#pragma message("####################### CFColor record has no BiffStructure::store() implemented")
	Log::error(" Error!!! CFColor record has no BiffStructure::store() implemented.");
	//record << something;
}


void CFColor::load(CFRecord& record)
{
	record >> xclrType;
	switch(xclrType.type)
	{
		case XColorType::XCLRINDEXED:
			record >> icv;
			break;
		case XColorType::XCLRRGB:
			record >> rgb;
			break;
		case XColorType::XCLRTHEMED:
			record >> theme;
			break;
		case XColorType::XCLRNINCHED:
		case XColorType::XCLRAUTO:
			record.skipNunBytes(4); // ignored
			break;
		default:
			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of XColorType.", record.getTypeString());
			break;
	}
	record >> numTint;
}


} // namespace XLS

