
#include "Pos.h"
#include "Frame.h"

#include <utils.h>

namespace XLS
{

Pos::Pos()
{
	m_iLinkObject = -1;
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
	pGlobalWorkbookInfoPtr = record.getGlobalWorkbookInfo();

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
	bool bAutoPosition	= true;
	bool bAutoSize		= true;

	if (m_Frame)
	{
		Frame* Frame_ = dynamic_cast<Frame*>(m_Frame.get());

		bAutoPosition	= !Frame_->fAutoPosition;
		bAutoSize		= !Frame_->fAutoSize;
	}
	double x = x1 / 4000.;
	double y = y1 / 4000.;

	double w = x2 / 4000.;
	double h = y2 / 4000.;

	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:layout")
		{
			if (bAutoSize && bAutoPosition)
			{
			}
			else
			{
				CP_XML_NODE(L"c:manualLayout")
				{
					//if (m_iLinkObject == 1) x += 0.5	+ (w > 0 ? w : 0);
					//if (m_iLinkObject == 2) x += 0.5	+ (w > 0 ? w : 0);
					if (m_iLinkObject == 3) y += 0		+ (h > 0 ? h : 0);

					CP_XML_NODE(L"c:xMode")	{CP_XML_ATTR(L"val", L"edge");}
					CP_XML_NODE(L"c:yMode")	{CP_XML_ATTR(L"val", L"edge");}

					//if (x < 0) x = 0;
					//if (y < 0) y = 0;
					
					if (!bAutoPosition)
					{
						if (x >= 0 && x < 1) CP_XML_NODE(L"c:x")		{CP_XML_ATTR(L"val", x);}
						if (y >= 0 && y < 1) CP_XML_NODE(L"c:y")		{CP_XML_ATTR(L"val", y);}
					}
					
					if (!bAutoSize && m_iLinkObject != 1 && m_iLinkObject != 2 && m_iLinkObject != 3) 
						//title, axis title vert, axis title horiz
					{
						if (w > 0 && w < 1) CP_XML_NODE(L"c:w")		{CP_XML_ATTR(L"val", w);}
						if (h > 0 && h < 1) CP_XML_NODE(L"c:h")		{CP_XML_ATTR(L"val", h);}
					}
				}
			}
		}
		if (m_iLinkObject == 1 || m_iLinkObject == 2 || m_iLinkObject == 3) 
		{
			CP_XML_NODE(L"c:overlay")
			{
				CP_XML_ATTR(L"val", 0);
			}
		}
	}
	return 0;
}

} // namespace XLS

