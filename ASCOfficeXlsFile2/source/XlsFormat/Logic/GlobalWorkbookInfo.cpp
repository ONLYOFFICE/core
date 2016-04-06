#include "GlobalWorkbookInfo.h"

namespace XLS
{;

GlobalWorkbookInfo::GlobalWorkbookInfo(const unsigned short code_page, XlsConverter * xls_converter_)
:	CodePage(code_page)
{
	fill_x_ids[FillInfo(0, 0, 0)]		= 0;
	fill_x_ids[FillInfo(17, 64, 65)]	= 1;
	
	last_AXES_id			= initial_AXES_id;

	Version					= 0x0600; //собственно xls

	xls_converter			= xls_converter_;

	startAddedSharedStrings = 0;
	current_sheet			= 0;
	cmt_rules				= 0;

	cellXfs_count			= 0;
	cellStyleXfs_count		= 0;
	cellStyleDxfs_count		= 0;
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

void GlobalWorkbookInfo::RegisterPaletteColor(int id, const std::wstring & rgb)
{
	colors_palette.insert(std::pair<int, std::wstring>(id, rgb));
}

const unsigned int GlobalWorkbookInfo::GenerateAXESId()
{
	return last_AXES_id += 1;
}



} // namespace XLS