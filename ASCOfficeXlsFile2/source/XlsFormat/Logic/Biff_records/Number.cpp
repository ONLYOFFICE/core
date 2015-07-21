#include "Number.h"
#include <simple_xml_writer.h>

namespace XLS
{;
extern int cellStyleXfs_count;

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
	record >> cell >> num;
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

			if (cell.ixfe.value())
			{
				CP_XML_ATTR(L"s", *cell.ixfe.value() - cellStyleXfs_count);
			}
		}			
	}
	return 0;
}

} // namespace XLS

