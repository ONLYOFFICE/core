#include "precompiled_xls.h"
#include "URICreateFlags.h"
#include <Binary/CFRecord.h>

namespace OSHARED
{;


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

//
//void URICreateFlags::toXML(BiffStructurePtr & parent)
//{
//	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::createElement(getClassName(), parent);
//
//	own_tag->setAttribute(L"createAllowRelative", createAllowRelative);
//	own_tag->setAttribute(L"createAllowImplicitWildcardScheme", createAllowImplicitWildcardScheme);
//	own_tag->setAttribute(L"createAllowImplicitFileScheme", createAllowImplicitFileScheme);
//	own_tag->setAttribute(L"createNoFrag", createNoFrag);
//	own_tag->setAttribute(L"createNoCanonicalize", createNoCanonicalize);
//	own_tag->setAttribute(L"createCanonicalize", createCanonicalize);
//	own_tag->setAttribute(L"createFileUseDosPath", createFileUseDosPath);
//	own_tag->setAttribute(L"createDecodeExtraInfo", createDecodeExtraInfo);
//	own_tag->setAttribute(L"createNoDecodeExtraInfo", createNoDecodeExtraInfo);
//	own_tag->setAttribute(L"createCrackUnknownSchemes", createCrackUnknownSchemes);
//	own_tag->setAttribute(L"createNoCrackUnknownSchemes", createNoCrackUnknownSchemes);
//	own_tag->setAttribute(L"createPreProcessHtmlUri", createPreProcessHtmlUri);
//	own_tag->setAttribute(L"createNoPreProcessHtmlUri", createNoPreProcessHtmlUri);
//	own_tag->setAttribute(L"createIESettings", createIESettings);
//	own_tag->setAttribute(L"createNoIESettings", createNoIESettings);
//	own_tag->setAttribute(L"createNoEncodeForbiddenCharacters", createNoEncodeForbiddenCharacters);
//}
//
//
//const bool URICreateFlags::fromXML(BiffStructurePtr & parent)
//{
//#pragma message("####################### URICreateFlags record has no BiffStructure::fromXML() implemented")
//	Log::error(" Error!!! URICreateFlags record has no BiffStructure::fromXML() implemented.");
//	return false;
//}


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

