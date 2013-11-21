#pragma once

#include <XLS_logic/Biff_records/BiffRecord.h>
#include <XLS_logic/Biff_structures/BiffString.h>
#include <XLS_logic/Biff_structures/DXFN12List.h>

namespace XLS
{;

class List12BlockLevel: public BiffStructTagged
{	
	BASE_OBJECT_DEFINE_CLASS_NAME(List12BlockLevel)
public:
	BiffStructurePtr clone();

	List12BlockLevel();
	~List12BlockLevel();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
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