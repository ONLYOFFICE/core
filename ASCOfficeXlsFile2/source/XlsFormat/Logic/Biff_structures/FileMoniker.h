#pragma once

#include "BiffStructure.h"

namespace OSHARED
{;

class CFRecord;

class FileMoniker : public XLS::BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FileMoniker)
public:
	FileMoniker();
	FileMoniker(XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	unsigned __int16 cAnti;
	std::string ansiPath;
	unsigned __int16 endServer;
	unsigned __int16 versionNumber;
	unsigned int cbUnicodePathSize;
	unsigned __int16 usKeyValue;
	std::wstring unicodePath;
};

} // namespace OSHARED

