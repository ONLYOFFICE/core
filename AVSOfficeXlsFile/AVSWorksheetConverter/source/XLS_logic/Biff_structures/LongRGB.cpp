#include "stdafx.h"
#include "LongRGB.h"
#include <XLS_bin/CFRecord.h>
#include <Exception/AttributeDataWrong.h>

namespace XLS
{;


LongRGBA::LongRGBA()
:	is_alpha(true)
{
}


LongRGBA::LongRGBA(const bool alpha_present)
:	is_alpha(alpha_present)
{
}


LongRGB::LongRGB()
:	LongRGBA(false)
{
}


BiffStructurePtr LongRGBA::clone()
{
	return BiffStructurePtr(new LongRGBA(*this));
}


BiffStructurePtr LongRGB::clone()
{
	return BiffStructurePtr(new LongRGB(*this));
}


void LongRGBA::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"red", red);
	xml_tag->setAttribute(L"green", green);
	xml_tag->setAttribute(L"blue", blue);
	if(is_alpha)
	{
		xml_tag->setAttribute(L"alpha", alpha);
	}
	xml_tag->setAttribute(L"argb", argb);
}

void LongRGBA::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	argb = getStructAttribute(xml_tag, L"argb");
	if(!STR::fromARGB(static_cast<wchar_t*>(argb), red, green, blue, alpha))
	{
		throw EXCEPT::LE::AttributeDataWrong(L"argb", static_cast<char*>(getClassName()), argb);
	}
}


void LongRGBA::store(CFRecord& record)
{
	record << red << green << blue;
	if(is_alpha)
	{
		record << alpha;
	}
	else
	{
		record.reserveNBytes(1); // reserved
	}
}


void LongRGBA::load(CFRecord& record)
{
	record >> red >> green >> blue;
	if(is_alpha)
	{
		record >> alpha;
	}
	else
	{
		alpha = 0;
		record.skipNBytes(1); // reserved
	}
	argb = STR::toARGB(red, green, blue, alpha).c_str();
}


} // namespace XLS
