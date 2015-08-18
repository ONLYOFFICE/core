#pragma once

#include "BiffStructure.h"
#include "Stxp.h"
#include "BiffString.h"
#include "Boolean.h"

namespace XLS
{

class CFRecord;

class DXFFntD : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFFntD)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeDXFFntD;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	XLUnicodeStringNoCch stFontName;
	Stxp stxp;
    int icvFore;
	Ts tsNinch;
	Boolean<unsigned int> fSssNinch;
	Boolean<unsigned int> fUlsNinch;
	Boolean<unsigned int> fBlsNinch;
    int ich;
    int cch;
	unsigned short iFnt;

};

} // namespace XLS

