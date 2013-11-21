#pragma once

#include "BiffStructTagged.h"

namespace XLS
{;

class CFRecord;

class FilePointer : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FilePointer)
public:
	FilePointer();
	FilePointer(const DWORD ptr);

	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	DWORD offset;
};

typedef boost::shared_ptr<FilePointer> FilePointerPtr;

} // namespace XLS

