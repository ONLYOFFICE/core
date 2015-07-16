#include "precompiled_xls.h"
#include "OfficeArtCOLORREF.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


OfficeArtCOLORREF::OfficeArtCOLORREF()
{
}

OfficeArtCOLORREF::OfficeArtCOLORREF(const long raw_data)
{
	red = static_cast<unsigned char>(GETBITS(raw_data, 0, 7));
	green = static_cast<unsigned char>(GETBITS(raw_data, 8, 15));
	blue = static_cast<unsigned char>(GETBITS(raw_data, 16, 23));

	fPaletteIndex = GETBIT(raw_data, 24);
	fPaletteRGB = GETBIT(raw_data, 25);
	fSystemRGB = GETBIT(raw_data, 26);
	fSchemeIndex = GETBIT(raw_data, 27);
	fSysIndex = GETBIT(raw_data, 28);
}

XLS::BiffStructurePtr OfficeArtCOLORREF::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtCOLORREF(*this));
}


//void OfficeArtCOLORREF::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	if(!fSchemeIndex && !fPaletteIndex && !fSysIndex)
//	{
//		own_tag->setAttribute(L"rgb", STR::toRGB(red, green, blue).c_str());
//	}
//	else if(fSchemeIndex)
//	{
//		own_tag->setAttribute(L"scheme_index", STR::int2str(red).c_str());
//	}
//	else if(fPaletteIndex)
//	{
//		own_tag->setAttribute(L"palette_index", STR::int2str((static_cast<unsigned short>(green) << 8) + red).c_str());
//	}
//	else if(fSysIndex)
//	{
//		own_tag->setAttribute(L"system_index", STR::int2str((static_cast<unsigned short>(green) << 8) + red).c_str());
//	}
//
//	own_tag->setAttribute(L"fPaletteIndex", fPaletteIndex);
//	own_tag->setAttribute(L"fPaletteRGB", fPaletteRGB);
//	own_tag->setAttribute(L"fSystemRGB", fSystemRGB);
//	own_tag->setAttribute(L"fSchemeIndex", fSchemeIndex);
//	own_tag->setAttribute(L"fSysIndex", fSysIndex);
//
//}
//
//void OfficeArtCOLORREF::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	// We don't support xlsx to xls conversion
//}
//
//
//
//
} // namespace XLS
