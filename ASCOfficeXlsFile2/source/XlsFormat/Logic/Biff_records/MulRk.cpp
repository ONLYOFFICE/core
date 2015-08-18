#include "MulRk.h"
#include <simple_xml_writer.h>

namespace XLS
{
extern int cellStyleXfs_count;

MulRk::MulRk()
{
}


MulRk::~MulRk()
{
}


BaseObjectPtr MulRk::clone()
{
	return BaseObjectPtr(new MulRk(*this));
}


void MulRk::writeFields(CFRecord& record)
{
#pragma message("####################### MulRk record is not implemented")
	Log::error("MulRk record is not implemented.");
	//record << some_value;
}


void MulRk::readFields(CFRecord& record)
{
	Col colFirst;
	Col colLast;
// A little hack to extract colLast before it is used
	record.skipNunBytes(record.getDataSize() - sizeof(unsigned short));
	record >> colLast;
	record.resetPointerToBegin();
//------------------
	record >> rw >> colFirst;
	for(unsigned short i = colFirst; i <= colLast; ++i)
	{
		RkRec rec;
		record >> rec;
		rgrkrec.push_back(RkRecPtr(new RkRec(rec)));
		cells.push_back(CellPtr(new Cell(rw, i, rec.get_ixfe())));
	}

	record.skipNunBytes(sizeof(unsigned short));
}

const int MulRk::GetRow() const
{
	return static_cast<unsigned short>(rw);
}

int MulRk::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		int row = GetRow();
			
        for (int i = 0; i < cells.size(); i++)
		{
			Cell * cell = dynamic_cast<Cell *>(cells[i].get());
			RkRec * rkrec = dynamic_cast<RkRec *>(rgrkrec[i].get());
			
			std::wstring ref = cell->getLocation().toString();// getColRowRef(i, row);
			CP_XML_NODE(L"c")
			{
				CP_XML_ATTR(L"r", ref);

				if ((cell->ixfe.value()) && (cell->ixfe > cellStyleXfs_count))
				{
					CP_XML_ATTR(L"s", cell->ixfe - cellStyleXfs_count);
				}
				CP_XML_NODE(L"v")
				{
					CP_XML_STREAM() << rkrec->RK_.value();
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

