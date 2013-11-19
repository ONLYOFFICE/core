#pragma once

#include "OfficeArtContainer.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtDgContainer : public OfficeArtContainer
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OfficeArtDgContainer)
public:
	XLS::BiffStructurePtr clone();
	OfficeArtDgContainer(const OfficeArtClientAnchorType anchor_type);

	static const bool CheckIfContainerStartFound(XLS::CFRecord& record);
	static const bool CheckIfContainerSizeOK(XLS::CFRecord& record);

};

typedef boost::shared_ptr<OfficeArtDgContainer> OfficeArtDgContainerPtr;


} // namespace ODRAW
