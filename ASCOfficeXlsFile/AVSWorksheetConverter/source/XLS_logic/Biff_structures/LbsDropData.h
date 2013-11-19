#pragma once

#include "BiffStructTagged.h"
#include "BiffString.h"

namespace XLS
{;

class CFRecord;

class LbsDropData : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(LbsDropData)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	BYTE wStyle;
	bool fFiltered;

	WORD cLine;
	WORD dxMin;

	XLUnicodeString str;

};

typedef boost::shared_ptr<LbsDropData> LbsDropDataPtr;

} // namespace XLS

