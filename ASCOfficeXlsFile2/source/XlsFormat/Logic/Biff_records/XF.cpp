
#include "XF.h"

#include <simple_xml_writer.h>

namespace XLS
{;

XF::XF(size_t& cell_xf_current_id, size_t& style_xf_current_id)
:	cell(cell_xf_current_id, style_xf_current_id),
	style(cell_xf_current_id, style_xf_current_id)
{
}


XF::~XF()
{
}


BaseObjectPtr XF::clone()
{
	return BaseObjectPtr(new XF(*this));
}


void XF::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	
	SETBIT(flags, 0, fLocked);
	SETBIT(flags, 1, fHidden);
	SETBIT(flags, 2, fStyle);
	SETBIT(flags, 3, f123Prefix);
	SETBITS(flags, 4, 15, ixfParent);
	record << ifnt << ifmt << flags;
	
	if(fStyle)
	{
		style.store(record);
	}
	else
	{
		cell.store(record);
	}
}


void XF::readFields(CFRecord& record)
{
	unsigned short flags;
	
	record >> ifnt >> ifmt >> flags;
	
	fLocked		= GETBIT(flags, 0);
	fHidden		= GETBIT(flags, 1);
	fStyle		= GETBIT(flags, 2);
	f123Prefix	= GETBIT(flags, 3);
	ixfParent	= GETBITS(flags, 4, 15);
	
	if(fStyle)
	{
		style.load(record);
	}
	else
	{
		cell.load(record);
	}
}
static const std::wstring HorAlign[8] = {L"general", L"left", L"center", L"right", L"fill", L"justify", L"centerContinuous", L"distributed"};
static const std::wstring VerAlign[5]  = {L"top", L"center", L"bottom", L"justify", L"distributed"};

int XF::serialize(std::wostream & stream)
{
     CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"xf")
		{	
			if (ifnt.value())			CP_XML_ATTR(L"fontId"	, ifnt.getValue());
			
			if (ifmt.value())
			{
				CP_XML_ATTR(L"numFmtId"	, ifmt);
			}

			if(fStyle)
			{
				CP_XML_ATTR(L"borderId"	, style.border_x_id);
				CP_XML_ATTR(L"fillId"	, style.fill_x_id);
			
				//CP_XML_ATTR(L"applyFill"			, style.fAtrPat);
				//CP_XML_ATTR(L"applyBorder"		, style.fAtrBdr);
				//CP_XML_ATTR(L"applyNumberFormat"	, style.fAtrNum);

				if (style.alc  >=0	&& style.alc  <8 && style.alcV >=0	&& style.alcV <5)
				{
					//CP_XML_ATTR(L"applyAlignment", style.fAtrAlc);
					CP_XML_NODE(L"alignment")
					{
						if (style.alc  >=0	&& style.alc  <8)	CP_XML_ATTR(L"horizontal"	, HorAlign[style.alc]);
						if (style.alcV >=0	&& style.alcV <5)	CP_XML_ATTR(L"vertical"		, VerAlign[style.alcV] );

						CP_XML_ATTR(L"shrinkToFit"	, style.fShrinkToFit);
						if (cell.cIndent > 0) CP_XML_ATTR(L"indent", style.cIndent);
						CP_XML_ATTR(L"wrapText"		, style.fWrap);
						
						if (style.trot > 0) CP_XML_ATTR(L"textRotation"	, style.trot);
					}
				}		
			}
			else
			{
				CP_XML_ATTR(L"xfId"		, ixfParent );
				CP_XML_ATTR(L"borderId"	, cell.border_x_id);
				CP_XML_ATTR(L"fillId"	, cell.fill_x_id);

				CP_XML_ATTR(L"applyFill"		, cell.fAtrPat);
				CP_XML_ATTR(L"applyBorder"		, cell.fAtrBdr);
				CP_XML_ATTR(L"applyNumberFormat", cell.fAtrNum);
				CP_XML_ATTR(L"applyFont"		, cell.fAtrFnt);

				if (cell.fAtrAlc)
				{
					CP_XML_ATTR(L"applyAlignment", cell.fAtrAlc);
					CP_XML_NODE(L"alignment")
					{
						if (cell.alc  >=0	&& cell.alc  <8)	CP_XML_ATTR(L"horizontal"	, HorAlign[cell.alc]);
						if (cell.alcV >=0	&& cell.alcV <5)	CP_XML_ATTR(L"vertical"		, VerAlign[cell.alcV] );
						
						CP_XML_ATTR(L"shrinkToFit"	, cell.fShrinkToFit);
						if (cell.cIndent > 0) CP_XML_ATTR(L"indent", cell.cIndent);
						CP_XML_ATTR(L"wrapText"		, cell.fWrap);
					
						if (cell.trot > 0) CP_XML_ATTR(L"textRotation"	, cell.trot);
					}
				}
			}
		}
	 }
	return 0;
}


} // namespace XLS

