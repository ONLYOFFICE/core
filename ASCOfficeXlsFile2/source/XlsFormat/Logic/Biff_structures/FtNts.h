#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/Boolean.h>

namespace XLS
{;

class CFRecord;

class FtNts : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FtNts)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	std::wstring  guid;
	Boolean<unsigned __int16> fSharedNote;
};

} // namespace XLS

