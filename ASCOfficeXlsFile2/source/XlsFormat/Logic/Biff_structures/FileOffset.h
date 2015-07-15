#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class FileOffset : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FileOffset)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned __int16 offset;
};

typedef boost::shared_ptr<FileOffset> FileOffsetPtr;

} // namespace XLS

