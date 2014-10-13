#include "precompiled_xls.h"

#include "GlobalWorkbookInfo.h"

namespace XLS
{;


GlobalWorkbookInfo::GlobalWorkbookInfo(const unsigned __int16 code_page)
:	CodePage(code_page)
{
	fill_x_ids[FillInfo(0, 0, 0)] = 0;
	fill_x_ids[FillInfo(17, 64, 65)] = 1;
	last_AXES_id = initial_AXES_id;
}


const size_t GlobalWorkbookInfo::RegisterBorderId(const BorderInfo& border)
{
	std::map<BorderInfo, size_t>::const_iterator it_find = border_x_ids.find(border);
	if(border_x_ids.end() == it_find)
	{
		size_t id = border_x_ids.size();
		return border_x_ids[border] = id;
	}
	else
	{
		return it_find->second;
	}
}

const size_t GlobalWorkbookInfo::RegisterFillId(const FillInfo& fill)
{
	if(0 == fill.fls)
	{
		return 0;
	}

	std::map<FillInfo, size_t>::const_iterator it_find = fill_x_ids.find(fill);
	if(fill_x_ids.end() == it_find)
	{
		size_t id = fill_x_ids.size();
		return fill_x_ids[fill] = id;
	}
	else
	{
		return it_find->second;
	}
}

const unsigned int GlobalWorkbookInfo::GenerateAXESId()
{
	return last_AXES_id += 1;
}



} // namespace XLS