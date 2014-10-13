#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class FtPioGrbit : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FtPioGrbit)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	bool fAutoPict;
	bool fDde;
	bool fPrintCalc;
	bool fIcon;
	bool fCtl;
	bool fPrstm;
	bool fCamera;
	bool fDefaultSize;
	bool fAutoLoad;
};

} // namespace XLS

