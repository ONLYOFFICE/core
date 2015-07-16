#include "BorderFillInfo.h"

#include <simple_xml_writer.h>

namespace XLS
{

	int FillInfo::serialize(std::wostream & stream)
	{
		CP_XML_WRITER(stream)    
		{
			CP_XML_NODE(L"fill")
			{
			}
		}
		return 0;
	}

	int BorderInfo::serialize(std::wostream & stream)
	{
		CP_XML_WRITER(stream)    
		{
			CP_XML_NODE(L"border")
			{
			}
		}
		return 0;
	}

}