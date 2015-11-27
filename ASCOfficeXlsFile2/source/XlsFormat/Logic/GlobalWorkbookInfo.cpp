#include "GlobalWorkbookInfo.h"

namespace XLS
{;


GlobalWorkbookInfo::GlobalWorkbookInfo(const unsigned short code_page, XlsConverter * xls_converter_)
:	CodePage(code_page)
{
	fill_x_ids[FillInfo(0, 0, 0)]		= 0;
	fill_x_ids[FillInfo(17, 64, 65)]	= 1;
	
	last_AXES_id			= initial_AXES_id;

	defaultColumnWidth		= 8;
	defaultRowHeight		= 14.4; 

	Version					= 0x0600; //собственно xls

	xls_converter			= xls_converter_;
}


const size_t GlobalWorkbookInfo::RegisterBorderId(const BorderInfo& border)
{
	boost::unordered_map<BorderInfo, int>::const_iterator it_find = border_x_ids.find(border);
	if(border_x_ids.end() == it_find)
	{
		int id = border_x_ids.size();
		border_x_ids[border] = id;
		return id;
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

	boost::unordered_map<FillInfo, int>::const_iterator it_find = fill_x_ids.find(fill);
	if(fill_x_ids.end() == it_find)
	{
		int id = fill_x_ids.size();
		fill_x_ids[fill] = id;
		return id;
	}
	else
	{
		return it_find->second;
	}
}

void GlobalWorkbookInfo::RegisterFontColorId (int id, const FillInfoExt & font_color)
{
	fonts_color_ext.insert(std::pair<int, FillInfoExt>(id, font_color));
}

void GlobalWorkbookInfo::RegisterPaletteColor(int id, const std::wstring & argb)
{
	colors_palette.insert(std::pair<int, std::wstring>(id, argb));
}

const unsigned int GlobalWorkbookInfo::GenerateAXESId()
{
	return last_AXES_id += 1;
}



} // namespace XLS