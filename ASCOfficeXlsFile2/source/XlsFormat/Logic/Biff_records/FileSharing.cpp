
#include "FileSharing.h"

namespace XLS
{

FileSharing::FileSharing()
{
}


FileSharing::~FileSharing()
{
}


BaseObjectPtr FileSharing::clone()
{
	return BaseObjectPtr(new FileSharing(*this));
}


void FileSharing::writeFields(CFRecord& record)
{
#pragma message("####################### FileSharing record is not implemented")
	Log::error("FileSharing record is not implemented.");
	//record << some_value;
}


void FileSharing::readFields(CFRecord& record)
{
	record >> fReadOnlyRec >> wResPassNum;
	wResPass = std::wstring (STR::int2hex_wstr(wResPassNum, sizeof(wResPassNum)).c_str());
	if(!wResPassNum)
	{
		record >> iNoResPass;
	}
	else
	{
		record >> stUNUsername;
	}
}

} // namespace XLS

