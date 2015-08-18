#pragma once

#include "BiffStructure.h"

namespace OSHARED
{

class SharedProperty : public XLS::BiffStructure
{
    BASE_STRUCTURE_DEFINE_CLASS_NAME(SharedProperty)
public:
    SharedProperty ();
    SharedProperty (XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeProperty;

    virtual void load(XLS::CFRecord& record);
    virtual void store(XLS::CFRecord& record);

	unsigned int keyIndex;
	unsigned int valueIndex;
};

} // namespace OSHARED

