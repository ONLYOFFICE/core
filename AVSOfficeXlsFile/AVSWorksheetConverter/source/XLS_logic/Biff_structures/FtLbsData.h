#pragma once

#include "BiffStructTagged.h"
#include "ObjFmla.h"
#include "LbsDropData.h"
#include "BiffString.h"
#include "Boolean.h"

namespace XLS
{;

class CFRecord;

class FtLbsData : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FtLbsData)
public:
	FtLbsData();
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record, const WORD ot);
	virtual void store(CFRecord& record, const WORD ot);

private:
	virtual void load(CFRecord& record){}
	virtual void store(CFRecord& record){}

private:
	ObjFmla fmla;
	WORD cLines;
	WORD iSel;

	bool fUseCB;
	bool fValidPlex;
	bool fValidIds;
	bool fNo3d;
	BYTE wListSelType;
	BYTE lct;

	WORD idEdit;

	LbsDropDataPtr dropData;
	std::vector<XLUnicodeString> rgLines;
	std::vector<Boolean<BYTE>> bsels;

};

} // namespace XLS

