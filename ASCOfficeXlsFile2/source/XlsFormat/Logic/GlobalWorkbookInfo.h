#pragma once

#include <boost/unordered_map.hpp> 
#include <vector>

#include "../Crypt/Decryptor.h"
#include "Biff_structures/BorderFillInfo.h"

namespace XLS
{;

class GlobalWorkbookInfo
{
public:
	GlobalWorkbookInfo(const unsigned short code_page);

	const size_t RegisterBorderId	(const BorderInfo& border);
	const size_t RegisterFillId		(const FillInfo& fill);

	const unsigned int GenerateAXESId();

	unsigned short						CodePage;
	CRYPT::DecryptorPtr					decryptor;
	std::vector<std::wstring>			sheets_names;
	std::vector<std::wstring>			xti_parsed;
	std::vector<std::wstring>			AddinUdfs;

	boost::unordered_map<BorderInfo, int>	border_x_ids;
	boost::unordered_map<FillInfo, int>		fill_x_ids;
	
	unsigned int last_AXES_id;
	const static unsigned int initial_AXES_id = 0;
};

typedef boost::shared_ptr<GlobalWorkbookInfo> GlobalWorkbookInfoPtr;


} // namespace XLS