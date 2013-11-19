#pragma once

#include "BiffStructure.h"
//#include <XLS_logic/Biff_structures/Phs.h>
//#include <XLS_logic/Biff_structures/BiffString.h>

namespace CRYPTO
{;

class CFRecord;

class EncryptionHeaderFlags : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(EncryptionHeaderFlags)
public:
	XLS::BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	bool fCryptoAPI;
	bool fDocProps;
	bool fExternal;
	bool fAES;
};

} // namespace CRYPTO

