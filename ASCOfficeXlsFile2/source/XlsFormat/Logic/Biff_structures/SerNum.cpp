
#include "SerNum.h"
#include <Binary/CFRecord.h>

namespace XLS
{


SerNum::SerNum()
{
}


SerNum::SerNum(const std::wstring& word)
{
#if defined(_WIN32) || defined (_WIN64)
    xnum = _wtof(word.c_str());
#else
    wchar_t *pEnd;
    xnum = wcstod(word.c_str(), &pEnd);
#endif
}


BiffStructurePtr SerNum::clone()
{
	return BiffStructurePtr(new SerNum(*this));
}


void SerNum::store(CFRecord& record)
{
    record.storeAnyData(fixed_type);
	record << xnum;
}


void SerNum::load(CFRecord& record)
{
	record >> xnum;
}


const std::wstring SerNum::toString() const
{
	return STR::double2str(xnum);
}


} // namespace XLS
