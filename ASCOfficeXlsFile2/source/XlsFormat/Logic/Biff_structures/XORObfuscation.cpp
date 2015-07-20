
#include "XORObfuscation.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr XORObfuscation::clone()
{
	return BiffStructurePtr(new XORObfuscation(*this));
}

//
//void XORObfuscation::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"key", key);
//	own_tag->setAttribute(L"verificationBytes", STR::int2hex_wstr(verificationBytes, sizeof(verificationBytes)).c_str());
//}
//
//
//const bool XORObfuscation::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### XORObfuscation record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! XORObfuscation record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


void XORObfuscation::store(CFRecord& record)
{
	record << key << verificationBytes;
}


void XORObfuscation::load(CFRecord& record)
{
	record >> key >> verificationBytes;
}


} // namespace XLS

