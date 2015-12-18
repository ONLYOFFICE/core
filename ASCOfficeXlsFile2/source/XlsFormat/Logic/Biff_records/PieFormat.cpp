
#include "PieFormat.h"

namespace XLS
{

PieFormat::PieFormat()
{
}


PieFormat::~PieFormat()
{
}


BaseObjectPtr PieFormat::clone()
{
	return BaseObjectPtr(new PieFormat(*this));
}


void PieFormat::writeFields(CFRecord& record)
{
	record << pcExplode;
}


void PieFormat::readFields(CFRecord& record)
{
	record >> pcExplode;
}

int PieFormat::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:explosion")
		{	
			CP_XML_ATTR(L"val", pcExplode);
		}
	}
	return 0;
}

} // namespace XLS

