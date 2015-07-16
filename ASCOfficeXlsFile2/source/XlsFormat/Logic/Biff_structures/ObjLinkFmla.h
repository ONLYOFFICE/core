#pragma once

#include "BiffStructure.h"
#include "ObjFmla.h"

namespace XLS
{;

class CFRecord;

class ObjLinkFmla : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(ObjLinkFmla)
public:
	ObjLinkFmla();
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record, const unsigned short ot);

private:
	virtual void store(CFRecord& record){}

private:
	ObjFmla fmla;
};

} // namespace XLS

