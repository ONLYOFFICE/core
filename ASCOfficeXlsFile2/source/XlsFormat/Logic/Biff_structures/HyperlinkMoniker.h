#pragma once

#include "BiffStructure.h"

namespace OSHARED
{;

class CFRecord;

class HyperlinkMoniker : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(HyperlinkMoniker)
public:
	XLS::BiffStructurePtr clone();

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeHyperlinkMoniker;

	std::wstring  monikerClsid;
	boost::shared_ptr<BiffStructure> data;
};

} // namespace OSHARED

