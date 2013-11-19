#include "stdafx.h"
#include "EncryptionHeader.h"
#include <XLS_bin/CFRecord.h>

namespace CRYPTO
{;


XLS::BiffStructurePtr EncryptionHeader::clone()
{
	return XLS::BiffStructurePtr(new EncryptionHeader(*this));
}


void EncryptionHeader::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	Flags.toXML(own_tag);
	own_tag->setAttribute(L"AlgID", AlgID);
	own_tag->setAttribute(L"AlgIDHash", AlgIDHash);
	own_tag->setAttribute(L"KeySize", KeySize);
	own_tag->setAttribute(L"ProviderType", ProviderType);
	own_tag->setAttribute(L"CSPName", CSPName.c_str());
}


const bool EncryptionHeader::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### EncryptionHeader record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! EncryptionHeader record has no BiffStructure::fromXML() implemented.");
	return false;
}


void EncryptionHeader::store(XLS::CFRecord& record)
{
#pragma message("####################### EncryptionHeader record has no BiffStructure::store() implemented")
	Log::error(" Error!!! EncryptionHeader record has no BiffStructure::store() implemented.");
	//record << something;
}


void EncryptionHeader::load(XLS::CFRecord& record)
{
	record.skipNBytes(2); // reserved
	WORD flags;
	record >> flags;
	record >> Flags;
	record. skipNBytes(4); // reserved (SizeExtra)
	record >> AlgID >> AlgIDHash >> KeySize >> ProviderType;
	record. skipNBytes(8); // reserved
	record >> CSPName;
}


} // namespace CRYPTO

