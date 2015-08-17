
#include "MSOCR.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{


XLS::BiffStructurePtr MSOCR::clone()
{
	return XLS::BiffStructurePtr(new MSOCR(*this));
}

void MSOCR::store(XLS::CFRecord& record)
{
	unsigned char flags = 0;
	SETBIT(flags, 3, fSchemeIndex);
	record << red << green << blue << flags;
}


void MSOCR::load(XLS::CFRecord& record)
{
	unsigned char flags;
	record >> red >> green >> blue >> flags;
	fSchemeIndex = GETBIT(flags, 3);
}


} // namespace XLS
