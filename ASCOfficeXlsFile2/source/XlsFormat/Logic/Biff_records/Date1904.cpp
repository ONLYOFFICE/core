
#include "Date1904.h"

namespace XLS
{

Date1904::Date1904()
{
}


Date1904::~Date1904()
{
}


BaseObjectPtr Date1904::clone()
{
	return BaseObjectPtr(new Date1904(*this));
}


void Date1904::writeFields(CFRecord& record)
{
	record << f1904DateSystem;
}


void Date1904::readFields(CFRecord& record)
{
	record >> f1904DateSystem;
}

} // namespace XLS

