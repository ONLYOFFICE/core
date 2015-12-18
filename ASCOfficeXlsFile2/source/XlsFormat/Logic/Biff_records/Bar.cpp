
#include "Bar.h"
#include "Chart3d.h"

namespace XLS
{

Bar::Bar()
{
}


Bar::~Bar()
{
}


BaseObjectPtr Bar::clone()
{
	return BaseObjectPtr(new Bar(*this));
}


void Bar::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fTranspose);
	SETBIT(flags, 1, fStacked);
	SETBIT(flags, 2, f100);
	SETBIT(flags, 3, fHasShadow);
	record >> pcOverlap >> pcGap >> flags;
}


void Bar::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> pcOverlap >> pcGap >> flags;
	
	fTranspose	= GETBIT(flags, 0);
	fStacked	= GETBIT(flags, 1);
	f100		= GETBIT(flags, 2);
	fHasShadow	= GETBIT(flags, 3);
}

int	Bar::serialize(std::wostream & _stream)
{
	Chart3d *chart3D = dynamic_cast<Chart3d *>(m_chart3D.get());
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:barDir")
		{
			if (fTranspose) CP_XML_ATTR (L"val" , L"bar");
			else			CP_XML_ATTR (L"val" , L"col");
		}
		if (fStacked || f100)
		{
			CP_XML_NODE(L"c:grouping")
			{
				if (f100)			CP_XML_ATTR (L"val" , L"percentStacked");				
				else if (fStacked)	CP_XML_ATTR (L"val" , L"stacked");
			}
		}
		else
		{
			CP_XML_NODE(L"c:grouping")
			{
				if ((chart3D) && (chart3D->fCluster))
					CP_XML_ATTR (L"val" , L"clustered"); 
				else		
					CP_XML_ATTR (L"val" , L"standard");
			}
		}

		CP_XML_NODE(L"c:gapWidth")
		{
			CP_XML_ATTR (L"val" , pcGap);
		}
		if (!chart3D && pcOverlap != 0)
		{
			CP_XML_NODE(L"c:overlap")
			{
				CP_XML_ATTR (L"val" , -pcOverlap);
			}
		}

		if ((chart3D) && (chart3D->pcGap != 150))
		{
			CP_XML_NODE(L"c:gapDepth")
			{
				CP_XML_ATTR (L"val" , chart3D->pcGap);
			}
		}
	}
	return 0;
}

} // namespace XLS

