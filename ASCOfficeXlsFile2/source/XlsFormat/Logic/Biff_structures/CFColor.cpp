#include "precompiled_xls.h"
#include "CFColor.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


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

//
//void CFColor::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"xclrType", xclrType.type);
//	switch(xclrType.type)
//	{
//		case XColorType::XCLRINDEXED:
//			own_tag->setAttribute(L"xclrValue", icv);
//			break;
//		case XColorType::XCLRRGB:
//			rgb.toXML(own_tag);
//			break;
//		case XColorType::XCLRTHEMED:
//			own_tag->setAttribute(L"xclrValue", theme);
//			break;
//	}
//	own_tag->setAttribute(L"numTint", numTint);
//}
//
//
//const bool CFColor::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### CFColor record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! CFColor record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


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

