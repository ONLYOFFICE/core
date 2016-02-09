#pragma once

#include "BiffStructure.h"
#include "Stxp.h"
#include "BiffString.h"
#include "Boolean.h"

namespace XLS
{
class CFRecord;
class DXFN;

class DXFFntD : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFFntD)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeDXFFntD;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	int serialize(std::wostream & stream);

	XLUnicodeStringNoCch stFontName;

	Stxp	stxp;
    _INT32	icvFore;
	
	Ts		tsNinch;	
	
	_UINT32 fSssNinch;
	_UINT32 fUlsNinch;
	_UINT32 fBlsNinch;
    
	_INT32	ich;
    _INT32	cch;
	_UINT16 iFnt;

//------------------------------------------------
	DXFN *parent;
};

} // namespace XLS

