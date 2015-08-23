#pragma once

#include <boost/unordered_map.hpp> 
#include <boost/smart_ptr/shared_array.hpp>
#include <vector>
#include <map>

#include "../Crypt/Decryptor.h"
#include "Biff_structures/BorderFillInfo.h"

namespace XLS
{;

class BaseObject;
typedef boost::shared_ptr<BaseObject> BaseObjectPtr;

class GlobalWorkbookInfo
{
public:
	GlobalWorkbookInfo(const unsigned short code_page);

	const size_t	RegisterBorderId	(const BorderInfo& border);
	const size_t	RegisterFillId		(const FillInfo& fill);	
	void			RegisterFontColorId	(int id, const FillInfoExt & font_color);
	void			RegisterPaletteColor(int id, const std::wstring & argb);

	const unsigned int	GenerateAXESId();

	unsigned short								CodePage;
	CRYPT::DecryptorPtr							decryptor;
	
	std::vector<std::wstring>					sheets_names;
	std::vector<std::wstring>					xti_parsed;
	std::vector<std::wstring>					AddinUdfs;

	boost::unordered_map<BorderInfo, int>		border_x_ids;
	boost::unordered_map<FillInfo, int>			fill_x_ids;
	std::map<int,  FillInfoExt>					fonts_color_ext;
	std::map<int,  std::wstring>				colors_palette;

	std::vector<BaseObjectPtr>					*fonts;
	
	unsigned int								last_AXES_id;
	const static unsigned int					initial_AXES_id = 0;

	std::vector<std::wstring>					defineNames;

	std::vector<std::pair<boost::shared_array<char>, size_t> >	bin_data;

	std::map<int, double>						customColumnsWidth;
	std::map<int, double>						customRowsHeight;

	double										defaultColumnWidth;
	double										defaultRowHeight;

	int Version;

};

typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;


} // namespace XLS