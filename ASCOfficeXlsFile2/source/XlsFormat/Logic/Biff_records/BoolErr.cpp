#include "BoolErr.h"

namespace XLS
{

BoolErr::BoolErr()
{
}


BoolErr::~BoolErr()
{
}


BaseObjectPtr BoolErr::clone()
{
	return BaseObjectPtr(new BoolErr(*this));
}


void BoolErr::writeFields(CFRecord& record)
{
	resender.store(record);
	record << cell << bes;
}


void BoolErr::readFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();

	record >> cell >> bes;
}

const CellRef BoolErr::getLocation() const
{
	return cell.getLocation();
}

int BoolErr::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		int row = cell.rw;
			
		std::wstring ref = cell.getLocation().toString();// getColRowRef(i, row);
		CP_XML_NODE(L"c")
		{
			CP_XML_ATTR(L"r", ref);

			if (cell.ixfe - global_info_->cellStyleXfs_count > 0)
			{
				CP_XML_ATTR(L"s", cell.ixfe - global_info_->cellStyleXfs_count);
			}
		}			
	}
	return 0;
}

} // namespace XLS

