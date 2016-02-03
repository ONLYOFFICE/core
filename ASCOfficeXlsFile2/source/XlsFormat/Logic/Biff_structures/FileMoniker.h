#pragma once

#include "BiffStructure.h"

namespace OSHARED
{

class CFRecord;

class FileMoniker : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FileMoniker)
public:
	FileMoniker();
	FileMoniker(XLS::CFRecord& record);
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeFileMoniker;

	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	_UINT16			cAnti;
	std::string		ansiPath;
	_UINT16			endServer;
	_UINT16			versionNumber;
	_UINT32			cbUnicodePathSize;
	_UINT16			usKeyValue;
	std::wstring	unicodePath;
};

} // namespace OSHARED

