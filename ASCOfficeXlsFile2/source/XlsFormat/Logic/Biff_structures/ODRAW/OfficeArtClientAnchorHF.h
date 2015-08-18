#pragma once

#include "OfficeArtRecord.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

class OfficeArtClientAnchorHF : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtClientAnchorHF)
public:
	OfficeArtClientAnchorHF();
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtClientAnchorHF;
	
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

    int width;
    int height;
};

typedef boost::shared_ptr<OfficeArtClientAnchorHF> OfficeArtClientAnchorHFPtr;


} // namespace XLS
