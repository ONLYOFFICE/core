
#include "OfficeArtCOLORREF.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


OfficeArtCOLORREF::OfficeArtCOLORREF()
{
	index = -1;
}

OfficeArtCOLORREF::OfficeArtCOLORREF(const long raw_data)
{
	red		= static_cast<unsigned char>(GETBITS(raw_data, 0, 7));
	green	= static_cast<unsigned char>(GETBITS(raw_data, 8, 15));
	blue	= static_cast<unsigned char>(GETBITS(raw_data, 16, 23));

	fPaletteIndex	= GETBIT(raw_data, 24);
	fPaletteRGB		= GETBIT(raw_data, 25);
	fSystemRGB		= GETBIT(raw_data, 26);
	fSchemeIndex	= GETBIT(raw_data, 27);
	fSysIndex		= GETBIT(raw_data, 28);


	if(!fSchemeIndex && !fPaletteIndex && !fSysIndex)
	{
		colorRGB = STR::toRGB(red, green, blue);
	}
	else if(fSchemeIndex)
	{
		index = red;
	}
	else if(fPaletteIndex)
	{
		index = ((green) << 8) + red;
	}
	else if(fSysIndex)
	{
		index = ((green) << 8) + red;
	}
}

XLS::BiffStructurePtr OfficeArtCOLORREF::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtCOLORREF(*this));
}

} // namespace XLS
