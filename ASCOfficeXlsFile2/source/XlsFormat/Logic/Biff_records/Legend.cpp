
#include "Legend.h"

namespace XLS
{

Legend::Legend()
{
}


Legend::~Legend()
{
}


BaseObjectPtr Legend::clone()
{
	return BaseObjectPtr(new Legend(*this));
}


void Legend::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fAutoPosition);
	SETBIT(flags, 2, fAutoPosX);
	SETBIT(flags, 3, fAutoPosY);
	SETBIT(flags, 4, fVert);
	SETBIT(flags, 5, fWasDataTable);
	record << x << y << dx << dy;
	record.reserveNunBytes(1); // unused
	record << wSpace << flags;
}


void Legend::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> x >> y >> dx >> dy;
	record.skipNunBytes(1); // unused
	record >> wSpace >> flags;

	fAutoPosition	= GETBIT(flags, 0);
	fAutoPosX		= GETBIT(flags, 2);
	fAutoPosY		= GETBIT(flags, 3);
	fVert			= GETBIT(flags, 4);
	
	fWasDataTable	= GETBIT(flags, 5);	
}

int Legend::serialize(std::wostream & _stream, int size)
{
	double x1Kf = x / 4000.;
	double y1Kf = y / 4000.;

	double x2Kf = (4000 - dx - x) / 4000.;
	double y2Kf = (4000 - dy - y) / 4000.;

	CP_XML_WRITER(_stream)    
	{
		if (fAutoPosX && fAutoPosY)
		{
			CP_XML_NODE(L"c:legendPos")
			{
				if (y1Kf > 0.5)
				{
					CP_XML_ATTR(L"val", "b");
					x = x - (size - 1 ) * dx / 2;
					dx = dx * size;
				}
				else if (x1Kf > 0.5)
				{
					CP_XML_ATTR(L"val", "r");
					y = y - (size - 1 ) * dy / 2;
					dy = dy * size;
				}
				else if (x2Kf > 0.5)
				{
					CP_XML_ATTR(L"val", "l");
					y = y - (size - 1 ) * dy / 2;
					dy = dy * size;
				}
				else if (y2Kf > 0.5)
				{
					CP_XML_ATTR(L"val", "t");
					x = x - (size - 1 ) * dx / 2;
					dx = dx * size;
				}
				else
				{
					//нету откровенного смещения
					CP_XML_ATTR(L"val", "l");
					dy = dy * size;
				}
			}
		}
		else
		{
			CP_XML_NODE(L"c:layout")
			{
				CP_XML_NODE(L"c:manualLayout")
				{
					CP_XML_NODE(L"c:xMode")	{CP_XML_ATTR(L"val", L"edge");}
					CP_XML_NODE(L"c:yMode")	{CP_XML_ATTR(L"val", L"edge");}
					
					if (fAutoPosX == false)	CP_XML_NODE(L"c:x")		{CP_XML_ATTR(L"val", x / 4000.);}
					if (fAutoPosY == false)	CP_XML_NODE(L"c:y")		{CP_XML_ATTR(L"val", y / 4000.);}

					if (fAutoPosX == false)	CP_XML_NODE(L"c:w")		{CP_XML_ATTR(L"val", (dx )/ 4000.);}
					if (fAutoPosY == false) CP_XML_NODE(L"c:h")		{CP_XML_ATTR(L"val", (dy )/ 4000.);}
				}
			}
		}
	}
	
	return 0;
}

} // namespace XLS

