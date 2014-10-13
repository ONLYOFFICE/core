#include "precompiled_xls.h"
#include "OfficeArtMetafileHeader.h"
#include <Binary/CFRecord.h>
#include "OfficeArtBlip.h"

namespace ODRAW
{;


XLS::BiffStructurePtr OfficeArtMetafileHeader::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtMetafileHeader(*this));
}


//void OfficeArtMetafileHeader::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"clip_left", rcBounds.left);
//	xml_tag->setAttribute(L"clip_right", rcBounds.right);
//	xml_tag->setAttribute(L"clip_top", rcBounds.top);
//	xml_tag->setAttribute(L"clip_bottom", rcBounds.bottom);
//	xml_tag->setAttribute(L"size_x", ptSize.x);
//	xml_tag->setAttribute(L"size_y", ptSize.y);
//	xml_tag->setAttribute(L"cbSave", cbSave);
//	xml_tag->setAttribute(L"compression", compression);
//	xml_tag->setAttribute(L"filter", filter);
//}
//
//void OfficeArtMetafileHeader::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	// We don't support xlsx to xls conversion
//}


void OfficeArtMetafileHeader::store(XLS::CFRecord& record)
{
	// We don't support xlsx to xls conversion
}


void OfficeArtMetafileHeader::load(XLS::CFRecord& record)
{
	record >> cbSize;
	record.loadAnyData(rcBounds);
	record.loadAnyData(ptSize);
	record >> cbSave >> compression >> filter;
}





} // namespace XLS
