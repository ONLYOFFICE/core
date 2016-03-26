#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/Feat11FieldDataItem.h>


namespace XLS
{

class TableFeatureType: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(TableFeatureType)
public:
	BiffStructurePtr clone();

	TableFeatureType();
	~TableFeatureType();

	static const ElementType	type = typeTableFeatureType;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	_UINT32	lt;		// SourceType
	_UINT32	idList;
	_UINT32	crwHeader;
	_UINT32	crwTotals;
	_UINT32	idFieldNext;
	_UINT32	cbFSData;
	_UINT16 rupBuild;

	unsigned char	fAutoFilter;
	unsigned char	fPersistAutoFilter;
	unsigned char	fShowInsertRow;
	unsigned char	fInsertRowInsCells;
	unsigned char	fLoadPldwIdDeleted;
	unsigned char	fShownTotalRow;
	unsigned char	fNeedsCommit;
	unsigned char	fSingleCell;
	unsigned char	fApplyAutoFilter;
	unsigned char	fForceInsertToBeVis;
	unsigned char	fCompressedXml;
	unsigned char	fLoadCSPName;
	unsigned char	fLoadPldwIdChanged;
	unsigned char	verXL;
	unsigned char	fLoadEntryId;
	unsigned char	fLoadPllstclInvalid;
	unsigned char	fGoodRupBld;
	unsigned char	fPublished;
	
	_UINT32	lPosStmCache;
	_UINT32	cbStmCache;
	_UINT32	cchStmCache;
	_UINT32	lem;		// LEMMode

	XLUnicodeString rgbName;
	_UINT16 cFieldData;
	XLUnicodeString cSPName;
	XLUnicodeString entryId;

	BiffStructurePtrVector fieldData;
};

typedef boost::shared_ptr<TableFeatureType> TableFeatureTypePtr;

} // namespace XLS