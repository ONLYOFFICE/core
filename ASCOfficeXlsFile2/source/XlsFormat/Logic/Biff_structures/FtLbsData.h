#pragma once

#include "BiffStructure.h"
#include "ObjFmla.h"
#include "LbsDropData.h"
#include "BiffString.h"
#include "Boolean.h"

namespace XLS
{;

class CFRecord;

class FtLbsData : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtLbsData)
public:
	FtLbsData();
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record, const unsigned __int16 ot);
	virtual void store(CFRecord& record, const unsigned __int16 ot);

private:
	virtual void load(CFRecord& record){}
	virtual void store(CFRecord& record){}

private:
	ObjFmla fmla;
	unsigned __int16 cLines;
	unsigned __int16 iSel;

	bool fUseCB;
	bool fValidPlex;
	bool fValidIds;
	bool fNo3d;
	unsigned char wListSelType;
	unsigned char lct;

	unsigned __int16 idEdit;

	LbsDropDataPtr dropData;
	std::vector<XLUnicodeString> rgLines;
	std::vector<Boolean<unsigned char>> bsels;

};

} // namespace XLS

