#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class FtCmo : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FtCmo)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	unsigned short ot;
private:
	unsigned short id;

	bool fLocked;
	bool fDefaultSize;
	bool fPublished;
	bool fPrint;
	bool fDisabled;
	bool fUIObj;
	bool fRecalcObj;
	bool fRecalcObjAlways;

};

} // namespace XLS

