#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class FilePointer : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FilePointer)
public:
	FilePointer();
	FilePointer(const unsigned int ptr);

	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned int offset;
};

typedef boost::shared_ptr<FilePointer> FilePointerPtr;

} // namespace XLS

