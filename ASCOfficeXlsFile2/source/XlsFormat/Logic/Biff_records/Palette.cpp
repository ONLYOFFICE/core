
#include "Palette.h"
#include <simple_xml_writer.h>

namespace XLS
{

Palette::Palette()
:	ccv(56)
{
}


Palette::Palette(const unsigned short ccv_init)
:	ccv(ccv_init)
{

}


BaseObjectPtr Palette::clone()
{
	return BaseObjectPtr(new Palette(*this));
}


void Palette::writeFields(CFRecord& record)
{
	record << ccv;
	for(BiffStructurePtrVector::const_iterator it = rgColor.begin(), itEnd = rgColor.end(); it != itEnd; ++it)
	{
		record << **it;
	}
}


void Palette::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	
	unsigned short ccv;
	record >> ccv;
	
	for(int i = 0; i < ccv; ++i)
	{
		LongRGBPtr rgb(new LongRGB);
		record >> *rgb;
		rgColor.push_back(rgb);

		global_info->RegisterPaletteColor(i, rgb->argb);
	}
}

const std::wstring standart_color[8] = {
										L"00000000",
										L"00FFFFFF",
										L"00FF0000",
										L"0000FF00",
										L"000000FF",
										L"00FFFF00",
										L"00FF00FF",
										L"0000FFFF"};

int Palette::serialize(std::wostream & stream)
{
    CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"indexedColors")
		{		
			for(int i = 0; i < 8; ++i)
			{	
				CP_XML_NODE(L"rgbColor")
				{
					CP_XML_ATTR(L"rgb", standart_color[i]);
				}
			}

			for(int i = 0; i < rgColor.size(); ++i)
			{		
				LongRGB * rgb = dynamic_cast<LongRGB *>(rgColor[i].get());
				CP_XML_NODE(L"rgbColor")
				{
					CP_XML_ATTR(L"rgb", rgb->argb);
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

