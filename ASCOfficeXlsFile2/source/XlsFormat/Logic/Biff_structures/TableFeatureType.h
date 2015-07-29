#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/Feat11FieldDataItem.h>


namespace XLS
{;

class TableFeatureType: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(TableFeatureType)
public:
	BiffStructurePtr clone();

	TableFeatureType();
	~TableFeatureType();

	
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	BIFF_DWORD lt;		// SourceType
	BIFF_DWORD idList;
	BIFF_DWORD crwHeader;
	BIFF_DWORD crwTotals;
	BIFF_DWORD idFieldNext;
	BIFF_DWORD cbFSData;
	BIFF_WORD rupBuild;

	BIFF_BYTE fAutoFilter;
	BIFF_BYTE fPersistAutoFilter;
	BIFF_BYTE fShowInsertRow;
	BIFF_BYTE fInsertRowInsCells;
	BIFF_BYTE fLoadPldwIdDeleted;
	BIFF_BYTE fShownTotalRow;
	BIFF_BYTE fNeedsCommit;
	BIFF_BYTE fSingleCell;
	BIFF_BYTE fApplyAutoFilter;
	BIFF_BYTE fForceInsertToBeVis;
	BIFF_BYTE fCompressedXml;
	BIFF_BYTE fLoadCSPName;
	BIFF_BYTE fLoadPldwIdChanged;
	BIFF_BYTE verXL;
	BIFF_BYTE fLoadEntryId;
	BIFF_BYTE fLoadPllstclInvalid;
	BIFF_BYTE fGoodRupBld;
	BIFF_BYTE fPublished;
	
	BIFF_DWORD lPosStmCache;
	BIFF_DWORD cbStmCache;
	BIFF_DWORD cchStmCache;
	BIFF_DWORD lem;		// LEMMode

	XLUnicodeString rgbName;
	BIFF_WORD cFieldData;
	XLUnicodeString cSPName;
	XLUnicodeString entryId;

	BiffStructurePtrVector fieldData;
};

typedef boost::shared_ptr<TableFeatureType> TableFeatureTypePtr;

} // namespace XLS