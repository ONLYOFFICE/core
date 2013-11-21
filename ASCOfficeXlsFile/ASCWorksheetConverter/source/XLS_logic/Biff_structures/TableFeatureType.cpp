#include "stdafx.h"
#include "TableFeatureType.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr TableFeatureType::clone()
{
	return BiffStructurePtr(new TableFeatureType(*this));
}

TableFeatureType::TableFeatureType()
{		
}


TableFeatureType::~TableFeatureType()
{
}


void TableFeatureType::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{		
	xml_tag->setAttribute(L"lt", lt);
	xml_tag->setAttribute(L"idList", idList);
	xml_tag->setAttribute(L"crwHeader", crwHeader);
	xml_tag->setAttribute(L"crwTotals", crwTotals);
	xml_tag->setAttribute(L"idFieldNext", idFieldNext);
	xml_tag->setAttribute(L"cbFSData", cbFSData);
	xml_tag->setAttribute(L"rupBuild", rupBuild);

	xml_tag->setAttribute(L"fAutoFilter", fAutoFilter);
	xml_tag->setAttribute(L"fPersistAutoFilter", fPersistAutoFilter);
	xml_tag->setAttribute(L"fShowInsertRow", fShowInsertRow);
	xml_tag->setAttribute(L"fInsertRowInsCells", fInsertRowInsCells);
	xml_tag->setAttribute(L"fLoadPldwIdDeleted", fLoadPldwIdDeleted);
	xml_tag->setAttribute(L"fShownTotalRow", fShownTotalRow);
	xml_tag->setAttribute(L"fNeedsCommit", fNeedsCommit);
	xml_tag->setAttribute(L"fSingleCell", fSingleCell);
	xml_tag->setAttribute(L"fApplyAutoFilter", fApplyAutoFilter);
	xml_tag->setAttribute(L"fForceInsertToBeVis", fForceInsertToBeVis);
	xml_tag->setAttribute(L"fCompressedXml", fCompressedXml);
	xml_tag->setAttribute(L"fLoadCSPName", fLoadCSPName);
	xml_tag->setAttribute(L"fLoadPldwIdChanged", fLoadPldwIdChanged);
	xml_tag->setAttribute(L"verXL", verXL);
	xml_tag->setAttribute(L"fLoadEntryId", fLoadEntryId);
	xml_tag->setAttribute(L"fLoadPllstclInvalid", fLoadPllstclInvalid);
	xml_tag->setAttribute(L"fGoodRupBld", fGoodRupBld);
	xml_tag->setAttribute(L"fPublished", fPublished);	

	if (lt != 1)
	{
		xml_tag->setAttribute(L"lPosStmCache", lPosStmCache);	
		xml_tag->setAttribute(L"cbStmCache", cbStmCache);			
		xml_tag->setAttribute(L"cchStmCache", cchStmCache);
	}

	xml_tag->setAttribute(L"cbStmCache", cbStmCache);
	xml_tag->setAttribute(L"rgbName", rgbName);
	xml_tag->setAttribute(L"cFieldData", cFieldData);
	
	if (fLoadCSPName == 1)
		xml_tag->setAttribute(L"cSPName", cSPName);

	if (fLoadEntryId == 1)
		xml_tag->setAttribute(L"entryId", entryId);

	for (size_t i = 0; i < fieldData.size(); i++)
	{
		fieldData[i]->toXML(xml_tag);
	}
}


void TableFeatureType::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{	
}


void TableFeatureType::store(CFRecord& record)
{	
}


void TableFeatureType::load(CFRecord& record)
{	
	record >> lt;				// SourceType
	record >> idList;			// Unique ID
	record >> crwHeader;		// 0 - Table has no header row, 1 - Table has a header row
	record >> crwTotals;		// 0 - Table has no total row, 1 - Table has a total row
	record >> idFieldNext;		// Next unique identifier
	record >> cbFSData;			// 64
	record >> rupBuild;			// Build number of the application

	DWORD _lt = lt;
	DWORD _idList = idList;
	DWORD _crwHeader = crwHeader;
	DWORD _crwTotals = crwTotals;
	DWORD _idFieldNext = idFieldNext;
	DWORD _cbFSData = cbFSData;
	DWORD _rupBuild = rupBuild;

	record.skipNBytes(2);		// unused1

	WORD flags;
	record >> flags;
	
	fAutoFilter = static_cast<BYTE>(GETBIT(flags, 1));
	fPersistAutoFilter = static_cast<BYTE>(GETBIT(flags, 2));
	fShowInsertRow = static_cast<BYTE>(GETBIT(flags, 3));
	fInsertRowInsCells = static_cast<BYTE>(GETBIT(flags, 4));
	fLoadPldwIdDeleted = static_cast<BYTE>(GETBIT(flags, 5));
	fShownTotalRow = static_cast<BYTE>(GETBIT(flags, 6));
	fNeedsCommit = static_cast<BYTE>(GETBIT(flags, 8));
	fSingleCell = static_cast<BYTE>(GETBIT(flags, 9));
	fApplyAutoFilter = static_cast<BYTE>(GETBIT(flags, 11));
	fForceInsertToBeVis = static_cast<BYTE>(GETBIT(flags, 12));
	fCompressedXml = static_cast<BYTE>(GETBIT(flags, 13));
	fLoadCSPName = static_cast<BYTE>(GETBIT(flags, 14));
	fLoadPldwIdChanged = static_cast<BYTE>(GETBIT(flags, 15));

	record >> flags;
	verXL = static_cast<BYTE>(GETBITS(flags, 0, 4));
	fLoadEntryId = static_cast<BYTE>(GETBIT(flags, 4));
	fLoadPllstclInvalid = static_cast<BYTE>(GETBIT(flags, 5));
	fGoodRupBld = static_cast<BYTE>(GETBIT(flags, 6));
	fPublished = static_cast<BYTE>(GETBIT(flags, 8));

	if (lt != 1)
	{
		record >> lPosStmCache;
		record >> cbStmCache;
		record >> cchStmCache;
	}
	record >> lem;
	if (lt != 1)
		record.skipNBytes(16);		// rgbHashParam

	record >> rgbName;
	record >> cFieldData;

	if (fLoadCSPName)
		record >> cSPName;

	if (fLoadEntryId == 1)
		record >> entryId;
	
	for (size_t i = 0; i < cFieldData; i++)
	{
		Feat11FieldDataItemPtr item(new Feat11FieldDataItem);
		item->load(record);
		fieldData.push_back(item);
	}
}


} // namespace XLS

