
#include "Blank.h"

#include <simple_xml_writer.h>

namespace XLS
{;
extern int cellStyleXfs_count;

Blank::Blank()
{
}


Blank::~Blank()
{
}

BaseObjectPtr Blank::clone()
{
	return BaseObjectPtr(new Blank(*this));
}


void Blank::writeFields(CFRecord& record)
{
	resender.store(record);
	record << cell;
}


void Blank::readFields(CFRecord& record)
{
	record >> cell;
}

const CellRef Blank::getLocation() const
{
	return cell.getLocation();
}

int Blank::serialize(std::wostream & stream)
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

