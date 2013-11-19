#pragma once

#include "BiffStructTagged.h"
#include "ObjFmla.h"
#include "PictFmlaKey.h"
#include "FtPioGrbit.h"

namespace XLS
{;

class CFRecord;

class FtPictFmla : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FtPictFmla)
public:
	FtPictFmla();
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag);
	virtual void load(CFRecord& record, FtPioGrbit& pictFlags);
	virtual void store(CFRecord& record, FtPioGrbit& pictFlags);
private:
	virtual void load(CFRecord& record){}
	virtual void store(CFRecord& record){}

private:
	ObjFmla fmla;
	DWORD lPosInCtlStm;
	DWORD cbBufInCtlStm;
	PictFmlaKey key;
};

} // namespace XLS

