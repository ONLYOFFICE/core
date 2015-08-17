
#include "DXFFntD.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr DXFFntD::clone()
{
	return BiffStructurePtr(new DXFFntD(*this));
}


//void DXFFntD::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"stFontName", stFontName);
//	stxp.toXML(xml_tag);
//	xml_tag->setAttribute(L"icvFore", icvFore);
//	xml_tag->setAttribute(L"ftsItalic", Boolean<unsigned char>(tsNinch.ftsItalic));
//	xml_tag->setAttribute(L"ftsStrikeout", Boolean<unsigned char>(tsNinch.ftsStrikeout));
//	xml_tag->setAttribute(L"fSssNinch", fSssNinch);
//	xml_tag->setAttribute(L"fUlsNinch", fUlsNinch);
//	xml_tag->setAttribute(L"fBlsNinch", fBlsNinch);
//	xml_tag->setAttribute(L"ich", ich);
//	xml_tag->setAttribute(L"cch", cch);
//	xml_tag->setAttribute(L"iFnt", iFnt);
//}
//
//
//void DXFFntD::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	stFontName = static_cast<std::wstring >(getStructAttribute(xml_tag, L"stFontName"));
//	stxp.fromXML(xml_tag);
//	icvFore = getStructAttribute(xml_tag, L"icvFore");
//	Boolean<unsigned char> tsNinch_ftsItalic;
//	tsNinch_ftsItalic.fromXML(xml_tag, L"ftsItalic");
//	tsNinch.ftsItalic = tsNinch_ftsItalic;
//	Boolean<unsigned char> tsNinch_ftsStrikeout;
//	tsNinch_ftsStrikeout.fromXML(xml_tag, L"ftsStrikeout");
//	tsNinch.ftsStrikeout = tsNinch_ftsStrikeout;
//	fSssNinch.fromXML(xml_tag, L"fSssNinch");
//	fUlsNinch.fromXML(xml_tag, L"fUlsNinch");
//	fBlsNinch.fromXML(xml_tag, L"fBlsNinch");
//	ich = getStructAttribute(xml_tag, L"ich");
//	cch = getStructAttribute(xml_tag, L"cch");
//	iFnt = getStructAttribute(xml_tag, L"iFnt");
//}
//
//
void DXFFntD::store(CFRecord& record)
{
	unsigned char cchFont = stFontName.getSize();
	record << cchFont;
	if(cchFont)
	{
		stFontName.setSize(cchFont);
		record << stFontName;
		record.reserveNunBytes(63 - stFontName.getStructSize()); // unused1
	}
	else
	{
		record.reserveNunBytes(63); // unused1
	}
	record << stxp << icvFore;
	record.reserveNunBytes(4); // reserved
	record << tsNinch << fSssNinch << fUlsNinch << fBlsNinch;
	record.reserveNunBytes(4); // unused1
	record << ich << cch << iFnt;
}


void DXFFntD::load(CFRecord& record)
{
	unsigned char cchFont;
	record >> cchFont;
	if(cchFont)
	{
		stFontName.setSize(cchFont);
		record >> stFontName;
		record.skipNunBytes(63 - stFontName.getStructSize()); // unused1
	}
	else
	{
		record.skipNunBytes(63); // unused1
	}
	record >> stxp >> icvFore;
	record.skipNunBytes(4); // reserved
	record >> tsNinch >> fSssNinch >> fUlsNinch >> fBlsNinch;
	record.skipNunBytes(4); // unused1
	record >> ich >> cch >> iFnt;
}


} // namespace XLS

