
#include "MulBlank.h"

#include <boost/lexical_cast.hpp>
#include <simple_xml_writer.h>

namespace XLS
{;

extern int cellStyleXfs_count;

std::wstring getColAddress(int col)
{
    static const size_t r = (L'Z' - L'A' + 1);
    std::wstring res;
    size_t r0 = col / r;

    if (r0 > 0)
    {
        const std::wstring rest = getColAddress(col - r*r0);
        const std::wstring res = getColAddress(r0-1) + rest;
        return res;
    }
    else
        return std::wstring(1, (wchar_t)(L'A' + col));
}



std::wstring getRowAddress(int row)
{
    return boost::lexical_cast<std::wstring>(row+1);
}

std::wstring getColRowRef(int col, int row)
{
	//(0,0) = A1

	return getColAddress(col) + getRowAddress(row);
}

MulBlank::MulBlank()
{
}


MulBlank::~MulBlank()
{
}


BaseObjectPtr MulBlank::clone()
{
	return BaseObjectPtr(new MulBlank(*this));
}


void MulBlank::writeFields(CFRecord& record)
{
#pragma message("####################### MulBlank record is not implemented")
	Log::error("MulBlank record is not implemented.");
	//record << some_value;
}


void MulBlank::readFields(CFRecord& record)
{
	// A little hack to extract colLast before it is used
	record.skipNunBytes(record.getDataSize() - sizeof(unsigned short));
	record >> colLast;
	record.resetPointerToBegin();
	//------------------
	record >> rw >> colFirst;

	rgixfe.load(record, colLast - colFirst + 1);

	record.skipNunBytes(sizeof(unsigned short));
}

const long MulBlank::GetRow() const
{
	return static_cast<unsigned short>(rw);
}

int MulBlank::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		int row = GetRow();

		for (long i = colFirst; i <= colLast; i++)
		{
			std::wstring ref =  getColRowRef(i, row);
			CP_XML_NODE(L"c")
			{
				CP_XML_ATTR(L"r", ref);

				if(( (i-colFirst) < rgixfe.rgixfe.size()) && (rgixfe.rgixfe[i-colFirst] > cellStyleXfs_count))
				{
					CP_XML_ATTR(L"s", rgixfe.rgixfe[i-colFirst] - cellStyleXfs_count);
				}
				else if  ((rgixfe.common_ixfe > 0) && (rgixfe.common_ixfe > cellStyleXfs_count))
				{
					CP_XML_ATTR(L"s", rgixfe.common_ixfe - cellStyleXfs_count);
				}

			}			
		}
	}
	return 0;
}



BiffStructurePtr IXFCellMulBlankSpecial::clone()
{
	return BiffStructurePtr(new IXFCellMulBlankSpecial(*this));
}




void IXFCellMulBlankSpecial::load(CFRecord& record, const size_t num_cells)
{
	unsigned short ixfe;
	for(size_t i = 0; i < num_cells; ++i)
	{
		record >> ixfe;
		rgixfe.push_back(ixfe);
	}
	if(rgixfe.size() != 0)
	{
		common_ixfe = rgixfe[0];
		size_t j = 0;
		for(; j < rgixfe.size(); ++j)
		{
			if(rgixfe[j] != common_ixfe) break;
		}
		if(j == rgixfe.size())
		{
			rgixfe.clear();
		}
	}
	else
	{
		common_ixfe = 0; // just to avoid warnings
	}
}



} // namespace XLS

