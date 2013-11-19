#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/HyperlinkMoniker.h>
#include <XLS_logic/Biff_structures/BiffString.h>

namespace OSHARED
{;

class CFRecord;

class HyperlinkObject : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(HyperlinkObject)
public:
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	DWORD streamVersion;

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
	_bstr_t guid;
	FILETIME fileTime;

};

} // namespace OSHARED

