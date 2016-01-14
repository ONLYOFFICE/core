
#include "Chart3DBarShape.h"

namespace XLS
{

Chart3DBarShape::Chart3DBarShape()
{
}


Chart3DBarShape::~Chart3DBarShape()
{
}


BaseObjectPtr Chart3DBarShape::clone()
{
	return BaseObjectPtr(new Chart3DBarShape(*this));
}


void Chart3DBarShape::writeFields(CFRecord& record)
{
	record << riser << taper;
}


void Chart3DBarShape::readFields(CFRecord& record)
{
	record >> riser >> taper;
}

int Chart3DBarShape::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:shape")
		{	
			if (riser == (unsigned char)0)
			{
				switch(taper)
				{
					case 0:	CP_XML_ATTR(L"val", L"box");		break;
					case 1:	CP_XML_ATTR(L"val", L"pyramid");	break;
					case 2:	CP_XML_ATTR(L"val", L"pyramidToMax");break;
				}
			}
			else
			{
				switch(taper)
				{
					case 0:	CP_XML_ATTR(L"val", L"cylinder");	break;
					case 1:	CP_XML_ATTR(L"val", L"cone");	break;
					case 2:	CP_XML_ATTR(L"val", L"coneToMax");break;
				}	
			}
		}
	}
	return 0;
}


} // namespace XLS

