
#include "Text.h"

namespace XLS
{

Text::Text()
{
	is_area = false;
}


Text::~Text()
{
}


BaseObjectPtr Text::clone()
{
	return BaseObjectPtr(new Text(*this));
}


void Text::writeFields(CFRecord& record)
{
	unsigned short flags1 = 0;
	unsigned short flags2 = 0;

	SETBIT(flags1, 0, fAutoColor);
	SETBIT(flags1, 1, fShowKey);
	SETBIT(flags1, 2, fShowValue);
	SETBIT(flags1, 4, fAutoText);
	SETBIT(flags1, 5, fGenerated);
	SETBIT(flags1, 6, fDeleted);
	SETBIT(flags1, 7, fAutoMode);
	SETBIT(flags1, 11, fShowLabelAndPerc);
	SETBIT(flags1, 12, fShowPercent);
	SETBIT(flags1, 13, fShowBubbleSizes);
	SETBIT(flags1, 14, fShowLabel);

	SETBITS(flags2, 0, 3, dlp);
	SETBITS(flags2, 14, 15, iReadingOrder);

	record << at  << vat << wBkgMode << rgbText << x << y << dx << dy << flags1 << icvText << flags2 << trot;
}


void Text::readFields(CFRecord& record)
{
	unsigned short flags1;
	unsigned short flags2;
	record >> at  >> vat >> wBkgMode >> rgbText >> x >> y >> dx >> dy >> flags1 >> icvText >> flags2 >> trot;
	
	fAutoColor = GETBIT(flags1, 0);
	fShowKey = GETBIT(flags1, 1);
	fShowValue = GETBIT(flags1, 2);
	fAutoText = GETBIT(flags1, 4);
	fGenerated = GETBIT(flags1, 5);
	fDeleted = GETBIT(flags1, 6);
	fAutoMode = GETBIT(flags1, 7);
	fShowLabelAndPerc = GETBIT(flags1, 11);
	fShowPercent = GETBIT(flags1, 12);
	fShowBubbleSizes = GETBIT(flags1, 13);
	fShowLabel = GETBIT(flags1, 14);

	dlp				= static_cast<unsigned char>(GETBITS(flags2, 0, 3));
	iReadingOrder	= static_cast<unsigned char>(GETBITS(flags2, 14, 15));

	rot = trot;
}

int Text::serialize(std::wostream & _stream)
{
	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:showVal")			{	CP_XML_ATTR (L"val" , fShowValue); }
		if ( is_area )
		{
		 	CP_XML_NODE(L"c:showSerName")	{	CP_XML_ATTR (L"val" , fShowLabel); }
			CP_XML_NODE(L"c:showCatName")	{	CP_XML_ATTR (L"val" , 0); }
		}
		else
		{
		 	CP_XML_NODE(L"c:showSerName")	{	CP_XML_ATTR (L"val" , 0); }
			CP_XML_NODE(L"c:showCatName")	{	CP_XML_ATTR (L"val" , fShowLabel); }
		}
		CP_XML_NODE(L"c:showPercent")		{	CP_XML_ATTR (L"val" , fShowPercent); }
		CP_XML_NODE(L"c:showBubbleSize")	{	CP_XML_ATTR (L"val" , fShowBubbleSizes); }
	}
	return 0;
}

} // namespace XLS

