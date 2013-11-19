#pragma once

#include "BiffStructTagged.h"

namespace OSHARED
{;

class CFRecord;

class FileMoniker : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FileMoniker)
public:
	FileMoniker();
	FileMoniker(XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	WORD cAnti;
	std::string ansiPath;
	WORD endServer;
	WORD versionNumber;
	DWORD cbUnicodePathSize;
	WORD usKeyValue;
	std::wstring unicodePath;
};

} // namespace OSHARED

