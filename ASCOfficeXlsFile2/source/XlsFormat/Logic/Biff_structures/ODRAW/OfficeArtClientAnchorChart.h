#pragma once

#include "OfficeArtRecord.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{;

class OfficeArtClientAnchorChart : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtClientAnchorChart)
public:
	OfficeArtClientAnchorChart();
	XLS::BiffStructurePtr clone();

	
	
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

private:
// 	bool fMove; // This attribute is ignored for chart sheets
	bool fSize;

	long lx1;
	long ly1;
	long lx2;
	long ly2;
};

typedef boost::shared_ptr<OfficeArtClientAnchorChart> OfficeArtClientAnchorChartPtr;


} // namespace XLS
