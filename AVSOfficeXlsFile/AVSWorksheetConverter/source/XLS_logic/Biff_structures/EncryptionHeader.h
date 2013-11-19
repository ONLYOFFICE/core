#pragma once

#include "BiffStructure.h"
#include "EncryptionHeaderFlags.h"

namespace CRYPTO
{;

class CFRecord;

class EncryptionHeader : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(EncryptionHeader)
public:
	XLS::BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	EncryptionHeaderFlags Flags;
	long AlgID;
	long AlgIDHash;
	DWORD KeySize;
	DWORD ProviderType;
	std::wstring CSPName;
};

} // namespace CRYPTO

