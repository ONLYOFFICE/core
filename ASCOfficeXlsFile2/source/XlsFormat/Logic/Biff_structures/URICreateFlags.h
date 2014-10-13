#pragma once

#include "BiffStructure.h"
//#include <Logic/Biff_structures/Phs.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace OSHARED
{;

class CFRecord;

class URICreateFlags : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(URICreateFlags)
public:
	URICreateFlags();
	URICreateFlags(XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	bool createAllowRelative;
	bool createAllowImplicitWildcardScheme;
	bool createAllowImplicitFileScheme;
	bool createNoFrag;
	bool createNoCanonicalize;
	bool createCanonicalize;
	bool createFileUseDosPath;
	bool createDecodeExtraInfo;
	bool createNoDecodeExtraInfo;
	bool createCrackUnknownSchemes;
	bool createNoCrackUnknownSchemes;
	bool createPreProcessHtmlUri;
	bool createNoPreProcessHtmlUri;
	bool createIESettings;
	bool createNoIESettings;
	bool createNoEncodeForbiddenCharacters;
};

} // namespace OSHARED

