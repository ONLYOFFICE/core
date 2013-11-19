#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/Stxp.h>
#include <XLS_logic/Biff_structures/BiffString.h>
#include <XLS_logic/Biff_structures/Boolean.h>

namespace XLS
{;

class CFRecord;

class DXFFntD : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DXFFntD)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	XLUnicodeStringNoCch stFontName;
	Stxp stxp;
	long icvFore;
	Ts tsNinch;
	Boolean<DWORD> fSssNinch;
	Boolean<DWORD> fUlsNinch;
	Boolean<DWORD> fBlsNinch;
	long ich;
	long cch;
	WORD iFnt;

};

} // namespace XLS

