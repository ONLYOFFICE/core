#pragma once

#include "SerAr.h"

namespace XLS
{

class SerNum : public SerAr
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SerNum)
public:
	SerNum();
	SerNum(const std::wstring& word);
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);
	virtual const std::wstring toString() const;

private:
    double xnum;
};


} // namespace XLS
