#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/DXFN12List.h>

namespace XLS
{

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

	_UINT32	cbdxfHeader;
	_UINT32	istnHeader;
	_UINT32	cbdxfData;
	_UINT32	istnData;
	_UINT32	cbdxfAgg;
	_UINT32	istnAgg;
	_UINT32	cbdxfBorder;
	_UINT32	cbdxfHeaderBorder;
	_UINT32	cbdxfAggBorder;

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