#pragma once

#include "OfficeArtRecord.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

class OfficeArtFSPGR : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtFSPGR)
public:
	OfficeArtFSPGR();
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtFSPGR;
	
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

    _INT32 xLeft;
    _INT32 yTop;
    _INT32 xRight;
    _INT32 yBottom;
};

typedef boost::shared_ptr<OfficeArtFSPGR> OfficeArtFSPGRPtr;


} // namespace XLS
