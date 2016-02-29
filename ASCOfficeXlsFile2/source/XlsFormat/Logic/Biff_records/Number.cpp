#include "Number.h"
#include <Auxiliary/HelpFunc.h>

namespace XLS
{

Number::Number()
{
}


Number::~Number()
{
}


BaseObjectPtr Number::clone()
{
	return BaseObjectPtr(new Number(*this));
}


void Number::writeFields(CFRecord& record)
{
	resender.store(record);
	record << cell << num;
}


void Number::readFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();

	record >> cell >> num;

	_INT32 val = 0;
	if (record.getDataSize() >= 18)//SchetPrintForm.xls
	{
		record >> val;
	}
}

const CellRef Number::getLocation() const
{
	return cell.getLocation();
}

int Number::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		int row = cell.rw;
			
		std::wstring ref = cell.getLocation().toString();// getColRowRef(i, row);
		CP_XML_NODE(L"c")
		{
			CP_XML_ATTR(L"r", ref);

			if (cell.ixfe > global_info_->cellStyleXfs_count)
			{
				CP_XML_ATTR(L"s", cell.ixfe - global_info_->cellStyleXfs_count);
			}
			if (num.value())
			{
				CP_XML_NODE(L"v")
				{
					CP_XML_STREAM() << STR::double2str(num);
				}
			}
		}			
	}
	return 0;
}

} // namespace XLS

