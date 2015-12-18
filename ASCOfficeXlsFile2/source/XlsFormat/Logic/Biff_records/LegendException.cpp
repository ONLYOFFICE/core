
#include "LegendException.h"

namespace XLS
{

LegendException::LegendException()
{
}


LegendException::~LegendException()
{
}


BaseObjectPtr LegendException::clone()
{
	return BaseObjectPtr(new LegendException(*this));
}


void LegendException::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fDelete);
	SETBIT(flags, 1, fLabel);
	record << iss << flags;
}


void LegendException::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> iss >> flags;

	fDelete = GETBIT(flags, 0);
	fLabel	= GETBIT(flags, 1);
}
int LegendException::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:delete")
		{
			CP_XML_ATTR(L"val", fDelete); 
		}
	}

	return 0;
}

} // namespace XLS

