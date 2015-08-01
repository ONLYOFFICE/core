#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/DXFN12List.h>

namespace XLS
{;

class List12BlockLevel: public BiffStructure
{	
	BASE_STRUCTURE_DEFINE_CLASS_NAME(List12BlockLevel)
public:
	BiffStructurePtr clone();

	List12BlockLevel();
	~List12BlockLevel();

	static const ElementType	type = typeList12BlockLevel;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	BIFF_DWORD cbdxfHeader;
	BIFF_DWORD istnHeader;
	BIFF_DWORD cbdxfData;
	BIFF_DWORD istnData;
	BIFF_DWORD cbdxfAgg;
	BIFF_DWORD istnAgg;
	BIFF_DWORD cbdxfBorder;
	BIFF_DWORD cbdxfHeaderBorder;
	BIFF_DWORD cbdxfAggBorder;

	DXFN12List dxfHeader;
	DXFN12List dxfData;
	DXFN12List dxfAgg;
	DXFN12List dxfBorder;
	DXFN12List dxfHeaderBorder;
	DXFN12List dxfAggBorder;
	XLUnicodeString stHeader;
	XLUnicodeString stData;
	XLUnicodeString stAgg;
};

typedef boost::shared_ptr<List12BlockLevel> List12BlockLevelPtr;

} // namespace XLS