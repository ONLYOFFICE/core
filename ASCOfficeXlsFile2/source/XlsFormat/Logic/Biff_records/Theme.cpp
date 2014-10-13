#include "precompiled_xls.h"
#include "Theme.h"
#include <fstream>
#include <iostream>      // endl

namespace XLS
{;

Theme::Theme()
{
}


Theme::~Theme()
{
}


BaseObjectPtr Theme::clone()
{
	return BaseObjectPtr(new Theme(*this));
}


void Theme::writeFields(CFRecord& record)
{
#pragma message("####################### Theme record is not implemented")
	Log::error("Theme record is not implemented.");
	//record << some_value;
}


void Theme::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	record >> dwThemeVersion;
	if(!dwThemeVersion)
	{
		Log::info("\"Theme\" binary parsing is not implemented.");

/*
		std::ofstream file1("D:\\temp.xlsx", std::ios_base::binary);
		file1.write(record.getCurData<char>(), (record.getDataSize() - record.getRdPtr()));
*/
	}

}

} // namespace XLS

