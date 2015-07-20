
#include "ExtProp.h"
#include <Binary/CFRecord.h>
//#include <Exception/WrongBiffRecord.h>

namespace XLS
{;


BiffStructurePtr ExtProp::clone()
{
	return BiffStructurePtr(new ExtProp(*this));
}


//void ExtProp::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//	own_tag->setAttribute(L"extType", extType);
//	own_tag->setAttribute(L"cb", cb);
//
//	switch(extType)
//	{
//		case 0x0004:
//		case 0x0005:
//		case 0x0007:
//		case 0x0008:
//		case 0x0009:
//		case 0x000A:
//		case 0x000B:
//		case 0x000C:
//		case 0x000D:
//			extPropData.color.toXML(own_tag);
//			break;
//		case 0x0006:
//			extPropData.gradient_fill.toXML(own_tag);
//			break;
//		case 0x000E:
//			own_tag->Puttext(STR::int2str(extPropData.font_scheme, 10).c_str());
//			break;
//		case 0x000F:
//			own_tag->Puttext(STR::int2str(extPropData.indent_level, 10).c_str());
//			break;
//		default:
//			break;
//	}
//
//}
//
//const bool ExtProp::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### ExtProp record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! ExtProp record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void ExtProp::store(CFRecord& record)
{
#pragma message("####################### ExtProp record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExtProp record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExtProp::load(CFRecord& record)
{
	record >> extType >> cb;
	switch(extType)
	{
		case 0x0004:
		case 0x0005:
		case 0x0007:
		case 0x0008:
		case 0x0009:
		case 0x000A:
		case 0x000B:
		case 0x000C:
		case 0x000D:
			record >> extPropData.color;
			break;
		case 0x0006:
			record >> extPropData.gradient_fill;
			break;
		case 0x000E:
			record >> extPropData.font_scheme;
			break;
		case 0x000F:
			record >> extPropData.indent_level;
			break;
		default:
			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported type of the extension.", record.getTypeString());
	}
}


} // namespace XLS
