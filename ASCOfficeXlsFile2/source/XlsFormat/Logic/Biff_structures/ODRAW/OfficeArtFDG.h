#pragma once

#include "OfficeArtRecord.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

class OfficeArtFDG : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFDG)
public:
	OfficeArtFDG();
	XLS::BiffStructurePtr clone();

	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

	static const XLS::ElementType	type = XLS::typeOfficeArtFDG;

	_UINT32 csp;
	_UINT32 spidCur;
};

typedef boost::shared_ptr<OfficeArtFDG> OfficeArtFDGPtr;


} // namespace XLS
