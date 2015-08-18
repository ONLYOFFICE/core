#pragma once

#include "OfficeArtRecord.h"

namespace XLS
{
	class CFRecord;
}


namespace ODRAW
{

class OfficeArtClientAnchorChart : public OfficeArtRecord
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(OfficeArtClientAnchorChart)
public:
	OfficeArtClientAnchorChart();
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typeOfficeArtClientAnchorChart;
	
	
	virtual void loadFields(XLS::CFRecord& record);
	virtual void storeFields(XLS::CFRecord& record);

// 	bool fMove; // This attribute is ignored for chart sheets
	bool fSize;

    int lx1;
    int ly1;
    int lx2;
    int ly2;
};

typedef boost::shared_ptr<OfficeArtClientAnchorChart> OfficeArtClientAnchorChartPtr;


} // namespace XLS
