#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/HyperlinkMoniker.h>
#include <Logic/Biff_structures/BiffString.h>

#if !defined(_WIN32) && !defined(_WIN64)

    typedef struct _FILETIME {
        _UINT32 dwLowDateTime;
        _UINT32 dwHighDateTime;
    } FILETIME;

#endif

namespace OSHARED
{

class CFRecord;

class HyperlinkObject : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(HyperlinkObject)
public:
	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	unsigned int streamVersion;

	static const XLS::ElementType	type = XLS::typeHyperlinkObject;

	bool hlstmfHasMoniker;
	bool hlstmfIsAbsolute;
	bool hlstmfSiteGaveDisplayName;
	bool hlstmfHasLocationStr;
	bool hlstmfHasDisplayName;
	bool hlstmfHasGUID;
	bool hlstmfHasCreationTime;
	bool hlstmfHasFrameName;
	bool hlstmfMonikerSavedAsStr;
	bool hlstmfAbsFromGetdataRel;

	HyperlinkString displayName;
	HyperlinkString targetFrameName;
	HyperlinkString moniker;
	HyperlinkMoniker oleMoniker;
	HyperlinkString location;
	std::wstring  guid;
	FILETIME fileTime;

};

} // namespace OSHARED

