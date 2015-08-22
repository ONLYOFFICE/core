
#include "HyperlinkObject.h"
#include <Binary/CFRecord.h>
//#include <Exception/AttributeDataWrong.h>

namespace OSHARED
{


XLS::BiffStructurePtr HyperlinkObject::clone()
{
	return XLS::BiffStructurePtr(new HyperlinkObject(*this));
}


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
		_GUID_ guid_num;
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
		_GUID_ guid_num;
		record >> guid_num;
		guid = STR::guid2bstr(guid_num);
	}
	if(hlstmfHasCreationTime)
	{
		record.loadAnyData(fileTime);
	}
}


} // namespace OSHARED

