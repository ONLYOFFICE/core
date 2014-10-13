#include "precompiled_xls.h"
#include "HyperlinkObject.h"
#include <Binary/CFRecord.h>
//#include <Exception/AttributeDataWrong.h>

namespace OSHARED
{;


XLS::BiffStructurePtr HyperlinkObject::clone()
{
	return XLS::BiffStructurePtr(new HyperlinkObject(*this));
}


//void HyperlinkObject::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"streamVersion", streamVersion);
//	
//	xml_tag->setAttribute(L"hlstmfHasMoniker", hlstmfHasMoniker);
//	xml_tag->setAttribute(L"hlstmfIsAbsolute", hlstmfIsAbsolute);
//	xml_tag->setAttribute(L"hlstmfSiteGaveDisplayName", hlstmfSiteGaveDisplayName);
//	xml_tag->setAttribute(L"hlstmfHasLocationStr", hlstmfHasLocationStr);
//	xml_tag->setAttribute(L"hlstmfHasDisplayName", hlstmfHasDisplayName);
//	xml_tag->setAttribute(L"hlstmfHasGUID", hlstmfHasGUID);
//	xml_tag->setAttribute(L"hlstmfHasCreationTime", hlstmfHasCreationTime);
//	xml_tag->setAttribute(L"hlstmfHasFrameName", hlstmfHasFrameName);
//	xml_tag->setAttribute(L"hlstmfMonikerSavedAsStr", hlstmfMonikerSavedAsStr);
//	xml_tag->setAttribute(L"hlstmfAbsFromGetdataRel", hlstmfAbsFromGetdataRel);
//
//	if(hlstmfHasDisplayName)
//	{
//		xml_tag->setAttribute(L"displayName", displayName);
//	}
//	if(hlstmfHasFrameName)
//	{
//		xml_tag->setAttribute(L"targetFrameName", targetFrameName);
//	}
//	if(hlstmfHasMoniker && hlstmfMonikerSavedAsStr)
//	{
//		xml_tag->setAttribute(L"moniker", moniker);
//	}
//	if(hlstmfHasMoniker && !hlstmfMonikerSavedAsStr)
//	{
//		oleMoniker.toXML(xml_tag);
//	}
//	if(hlstmfHasLocationStr)
//	{
//		xml_tag->setAttribute(L"location", location);
//	}
//	if(hlstmfHasGUID)
//	{
//		xml_tag->setAttribute(L"guid", guid);
//	}
//	if(hlstmfHasCreationTime)
//	{
//		xml_tag->setAttribute(L"dwHighDateTime", fileTime.dwHighDateTime);
//		xml_tag->setAttribute(L"dwLowDateTime", fileTime.dwLowDateTime);
//	}
//}
//
//
//void HyperlinkObject::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	streamVersion = 2;
//
//	hlstmfHasMoniker = getStructAttribute(xml_tag, L"hlstmfHasMoniker");
//	hlstmfIsAbsolute = getStructAttribute(xml_tag, L"hlstmfIsAbsolute");
//	hlstmfSiteGaveDisplayName = getStructAttribute(xml_tag, L"hlstmfSiteGaveDisplayName");
//	hlstmfHasLocationStr = getStructAttribute(xml_tag, L"hlstmfHasLocationStr");
//	hlstmfHasDisplayName = getStructAttribute(xml_tag, L"hlstmfHasDisplayName");
//	hlstmfHasGUID = getStructAttribute(xml_tag, L"hlstmfHasGUID");
//	hlstmfHasCreationTime = getStructAttribute(xml_tag, L"hlstmfHasCreationTime");
//	hlstmfHasFrameName = getStructAttribute(xml_tag, L"hlstmfHasFrameName");
//	hlstmfMonikerSavedAsStr = getStructAttribute(xml_tag, L"hlstmfMonikerSavedAsStr");
//	hlstmfAbsFromGetdataRel = getStructAttribute(xml_tag, L"hlstmfAbsFromGetdataRel");
//
//	if(hlstmfHasDisplayName)
//	{
//		displayName = static_cast<std::wstring >(getStructAttribute(xml_tag, L"displayName"));
//	}
//	if(hlstmfHasFrameName)
//	{
//		targetFrameName = static_cast<std::wstring >(getStructAttribute(xml_tag, L"targetFrameName"));
//	}
//	if(hlstmfHasMoniker && hlstmfMonikerSavedAsStr)
//	{
//		moniker = static_cast<std::wstring >(getStructAttribute(xml_tag, L"moniker"));
//	}
//	if(hlstmfHasMoniker && !hlstmfMonikerSavedAsStr)
//	{
//		oleMoniker.fromXML(xml_tag);
//	}
//	if(hlstmfHasLocationStr)
//	{
//		location = static_cast<std::wstring >(getStructAttribute(xml_tag, L"location"));
//	}
//	if(hlstmfHasGUID)
//	{
//		guid = getStructAttribute(xml_tag, L"guid");
//	}
//	if(hlstmfHasCreationTime)
//	{
//		fileTime.dwHighDateTime = xml_tag->getAttribute(L"dwHighDateTime");
//		fileTime.dwLowDateTime = xml_tag->getAttribute(L"dwLowDateTime");
//	}
//}
//
//
void HyperlinkObject::store(XLS::CFRecord& record)
{
	unsigned int flags = 0;
	SETBIT(flags, 0, hlstmfHasMoniker);
	SETBIT(flags, 1, hlstmfIsAbsolute);
	SETBIT(flags, 2, hlstmfSiteGaveDisplayName);
	SETBIT(flags, 3, hlstmfHasLocationStr);
	SETBIT(flags, 4, hlstmfHasDisplayName);
	SETBIT(flags, 5, hlstmfHasGUID);
	SETBIT(flags, 6, hlstmfHasCreationTime);
	SETBIT(flags, 7, hlstmfHasFrameName);
	SETBIT(flags, 8, hlstmfMonikerSavedAsStr);
	SETBIT(flags, 9, hlstmfAbsFromGetdataRel);

	record << streamVersion << flags;

	if(hlstmfHasDisplayName)
	{
		record << displayName;
	}
	if(hlstmfHasFrameName)
	{
		record << targetFrameName;
	}
	if(hlstmfHasMoniker && hlstmfMonikerSavedAsStr)
	{
		record << moniker;
	}
	if(hlstmfHasMoniker && !hlstmfMonikerSavedAsStr)
	{
		record << oleMoniker;
	}
	if(hlstmfHasLocationStr)
	{
		record << location;
	}
	if(hlstmfHasGUID)
	{
		GUID guid_num;
		if(!STR::bstr2guid(guid, guid_num))
		{
			throw;// EXCEPT::LE::AttributeDataWrong(L"guid", L"HyperlinkObject", guid);
		}
		record << guid_num;
	}
	if(hlstmfHasCreationTime)
	{
		record.storeAnyData(fileTime);
	}
}


void HyperlinkObject::load(XLS::CFRecord& record)
{
	unsigned int flags;
	record >> streamVersion >> flags;
	
	hlstmfHasMoniker = GETBIT(flags, 0);
	hlstmfIsAbsolute = GETBIT(flags, 1);
	hlstmfSiteGaveDisplayName = GETBIT(flags, 2);
	hlstmfHasLocationStr = GETBIT(flags, 3);
	hlstmfHasDisplayName = GETBIT(flags, 4);
	hlstmfHasGUID = GETBIT(flags, 5);
	hlstmfHasCreationTime = GETBIT(flags, 6);
	hlstmfHasFrameName = GETBIT(flags, 7);
	hlstmfMonikerSavedAsStr = GETBIT(flags, 8);
	hlstmfAbsFromGetdataRel = GETBIT(flags, 9);

	if(hlstmfHasDisplayName)
	{
		record >> displayName;
	}
	if(hlstmfHasFrameName)
	{
		record >> targetFrameName;
	}
	if(hlstmfHasMoniker && hlstmfMonikerSavedAsStr)
	{
		record >> moniker;
	}
	if(hlstmfHasMoniker && !hlstmfMonikerSavedAsStr)
	{
		record >> oleMoniker;
	}
	if(hlstmfHasLocationStr)
	{
		record >> location;
	}
	if(hlstmfHasGUID)
	{
		GUID guid_num;
		record >> guid_num;
		guid = STR::guid2bstr(guid_num);
	}
	if(hlstmfHasCreationTime)
	{
		record.loadAnyData(fileTime);
	}
}


} // namespace OSHARED

