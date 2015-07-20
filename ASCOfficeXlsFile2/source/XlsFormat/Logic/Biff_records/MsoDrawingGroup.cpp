
#include "MsoDrawingGroup.h"

namespace XLS
{;

MsoDrawingGroup::MsoDrawingGroup(const bool is_inside_chart_sheet)
:	rgChildRec(is_inside_chart_sheet ? ODRAW::OfficeArtRecord::CA_Chart : ODRAW::OfficeArtRecord::CA_Sheet)
{
}


MsoDrawingGroup::~MsoDrawingGroup()
{
}


BaseObjectPtr MsoDrawingGroup::clone()
{
	return BaseObjectPtr(new MsoDrawingGroup(*this));
}


void MsoDrawingGroup::writeFields(CFRecord& record)
{
	record << rgChildRec;
}


void MsoDrawingGroup::readFields(CFRecord& record)
{
	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];
	while(!recs.empty())
	{
		record.appendRawData(recs.front());
		recs.pop_front();
	}

	record >> rgChildRec;
}

} // namespace XLS

