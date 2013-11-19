#pragma once

#include "BiffStructTagged.h"

namespace XLS
{;

class CFRecord;

class DXFBdr : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DXFBdr)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	BYTE dgLeft;
	BYTE dgRight;
	BYTE dgTop;
	BYTE dgBottom;
	BYTE icvLeft;
	BYTE icvRight;
	bool bitDiagDown;
	bool bitDiagUp;

	BYTE icvTop;
	BYTE icvBottom;
	BYTE icvDiag;
	BYTE dgDiag;

};

} // namespace XLS

