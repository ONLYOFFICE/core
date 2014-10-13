#pragma once

#include "BiffStructure.h"

namespace OSHARED
{;

class CFRecord;

class HyperlinkMoniker : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(HyperlinkMoniker)
public:
	XLS::BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	std::wstring  monikerClsid;
	boost::shared_ptr<BiffStructure> data;
};

} // namespace OSHARED

