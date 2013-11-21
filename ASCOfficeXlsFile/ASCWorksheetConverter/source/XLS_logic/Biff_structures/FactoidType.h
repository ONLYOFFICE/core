#pragma once

#include "BiffStructure.h"
//#include <XLS_logic/Biff_structures/Phs.h>
#include <XLS_logic/Biff_structures/BiffString.h>

namespace OSHARED
{;

class CFRecord;

class FactoidType : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FactoidType)
public:
	XLS::BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	DWORD id;
	PBString rgbUri;
	PBString rgbTag;
	PBString rgbDownLoadURL;
};

} // namespace OSHARED

