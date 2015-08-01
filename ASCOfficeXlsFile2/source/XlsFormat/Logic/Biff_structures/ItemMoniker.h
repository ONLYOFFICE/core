#pragma once

#include "BiffStructure.h"

namespace OSHARED
{;

class CFRecord;

class ItemMoniker : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ItemMoniker)
public:
	ItemMoniker();
	ItemMoniker(XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeItemMoniker;
	
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	std::string delimiterAnsi;
	std::wstring delimiterUnicode;
	std::string itemAnsi;
	std::wstring itemUnicode;
};

} // namespace OSHARED

