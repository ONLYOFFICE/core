
#include "Pos.h"
#include <simple_xml_writer.h>
#include <utils.h>

namespace XLS
{

Pos::Pos()
{
}


Pos::~Pos()
{
}


BaseObjectPtr Pos::clone()
{
	return BaseObjectPtr(new Pos(*this));
}


void Pos::writeFields(CFRecord& record)
{
	record << mdTopLt << mdBotRt;
	record << x1;
	record.reserveNunBytes(2); // unused1
	record << y1;
	record.reserveNunBytes(2); // unused2
	record << x2;
	record.reserveNunBytes(2); // unused3
	record << y2;
	record.reserveNunBytes(2); // unused4
}


void Pos::readFields(CFRecord& record)
{
	record >> mdTopLt >> mdBotRt;
	record >> x1;
	record.skipNunBytes(2); // unused1
	record >> y1;
	record.skipNunBytes(2); // unused2
	record >> x2;
	record.skipNunBytes(2); // unused3
	record >> y2;
	record.skipNunBytes(2); // unused4
}

int Pos::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:layout")
		{
			CP_XML_NODE(L"c:manualLayout")
			{
				CP_XML_NODE(L"c:xMode")	{CP_XML_ATTR(L"val", L"edge");}
				CP_XML_NODE(L"c:yMode")	{CP_XML_ATTR(L"val", L"edge");}
				CP_XML_NODE(L"c:x")		{CP_XML_ATTR(L"val", (int)(x1 / 4000));}
				CP_XML_NODE(L"c:y")		{CP_XML_ATTR(L"val", (int)(y1 / 4000));}
				CP_XML_NODE(L"c:w")		{CP_XML_ATTR(L"val", (int)((x2 - x1)/ 4000));}
				CP_XML_NODE(L"c:h")		{CP_XML_ATTR(L"val", (int)((y2 - y1)/ 4000));}
			}
		}
	}
	return 0;
}

} // namespace XLS

