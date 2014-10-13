#pragma once

#include "BiffStructure.h"
//#include <Logic/Biff_structures/Phs.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace CRYPTO
{;

class CFRecord;

class EncryptionHeaderFlags : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(EncryptionHeaderFlags)
public:
	XLS::BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	bool fCryptoAPI;
	bool fDocProps;
	bool fExternal;
	bool fAES;
};

} // namespace CRYPTO

