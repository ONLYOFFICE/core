#pragma once

#include <boost/unordered_map.hpp> 
#include <boost/smart_ptr/shared_array.hpp>
#include <vector>
#include <map>

#include <Logic/Biff_structures/BorderFillInfo.h>

#include "../Crypt/Decryptor.h"

class XlsConverter;


namespace XLS
{;

class BaseObject;
typedef boost::shared_ptr<BaseObject> BaseObjectPtr;

static const std::wstring DefaultPalette[] = {
	L"00000000",	L"00FFFFFF",	L"00FF0000",	L"0000FF00",	L"000000FF",	L"00FFFF00",	L"00FF00FF",	L"0000FFFF",	
	L"00000000",	L"00FFFFFF",	L"00FF0000",	L"0000FF00",	L"000000FF",	L"00FFFF00",	L"00FF00FF",	L"0000FFFF",	
	L"00800000",	L"00008000",	L"00000080",	L"00808000",	L"00800080",	L"00008080",	L"00C0C0C0",	L"00808080",	
	L"009999FF",	L"00993366",	L"00FFFFCC",	L"00CCFFFF",	L"00660066",	L"00FF8080",	L"000066CC",	L"00CCCCFF",	
	L"00000080",	L"00FF00FF",	L"00FFFF00",	L"0000FFFF",	L"00800080",	L"00800000",	L"00008080",	L"000000FF",	
	L"0000CCFF",	L"00CCFFFF",	L"00CCFFCC",	L"00FFFF99",	L"0099CCFF",	L"00FF99CC",	L"00CC99FF",	L"00FFCC99",	
	L"003366FF",	L"0033CCCC",	L"0099CC00",	L"00FFCC00",	L"00FF9900",	L"00FF6600",	L"00666699",	L"00969696",	
	L"00003366",	L"00339966",	L"00003300",	L"00333300",	L"00993300",	L"00993366",	L"00333399",	L"00333333"
};

class GlobalWorkbookInfo
{
public:
	GlobalWorkbookInfo(const unsigned short code_page, XlsConverter * xls_converter_);

	const size_t	RegisterBorderId	(const BorderInfo& border);
	const size_t	RegisterFillId		(const FillInfo& fill);	
	void			RegisterFontColorId	(int id, const FillInfoExt & font_color);
	void			RegisterPaletteColor(int id, const std::wstring & argb);

	const unsigned int	GenerateAXESId();

	unsigned short								CodePage;
	CRYPT::DecryptorPtr							decryptor;
	
	std::vector<std::wstring>					sheets_state;
	std::vector<std::wstring>					sheets_names;
	std::vector<std::wstring>					xti_parsed;
	std::vector<std::wstring>					AddinUdfs;

	boost::unordered_map<BorderInfo, int>		border_x_ids;
	boost::unordered_map<FillInfo, int>			fill_x_ids;
	std::map<int,  FillInfoExt>					fonts_color_ext;
	std::map<int,  std::wstring>				colors_palette;

	std::vector<BaseObjectPtr>					*m_arFonts;
	
	unsigned int								current_sheet;

	unsigned int								last_AXES_id;
	const static unsigned int					initial_AXES_id = 0x2000000;

	std::map<std::wstring, std::vector<std::wstring>>	mapDefineNames;
	std::vector<std::wstring>							arDefineNames;
	std::vector<std::wstring>							arExternalNames;

	unsigned int								startAddedSharedStrings;
	std::vector<std::wstring>					arAddedSharedStrings;

	std::vector<std::pair<boost::shared_array<char>, size_t> >	bin_data;

	struct _sheet_size_info
	{
		_sheet_size_info() : defaultColumnWidth(8.), defaultRowHeight (14.4) {}
		std::map<int, double>					customColumnsWidth;
		std::map<int, double>					customRowsHeight;
		
		double									defaultColumnWidth;
		double									defaultRowHeight;
		
	};
	std::vector<_sheet_size_info>				sheet_size_info;

	int Version;

	int											cmt_rules;
	int											cellStyleXfs_count;
	int											cellStyleDxfs_count;

	std::wstringstream							users_Dxfs_stream;

	XlsConverter								*xls_converter;

};

typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;


} // namespace XLS