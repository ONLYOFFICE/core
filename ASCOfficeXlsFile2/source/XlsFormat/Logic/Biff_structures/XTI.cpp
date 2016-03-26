
#include "XTI.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr XTI::clone()
{
	return BiffStructurePtr(new XTI(*this));
}

void XTI::store(CFRecord& record)
{
	record << iSupBook << itabFirst << itabLast;
}


void XTI::load(CFRecord& record)
{
	if (record.checkFitReadSafe(6) == false) //Version < 0x0600
		return;

	record >> iSupBook >> itabFirst >> itabLast;

	if (itabFirst < 0 && itabLast < 0)
	{
		record.getGlobalWorkbookInfo()->xti_parsed.push_back(L"");
	}
	else
	{
		record.getGlobalWorkbookInfo()->xti_parsed.push_back(
			XMLSTUFF::xti_indexes2sheet_name(itabFirst, itabLast, record.getGlobalWorkbookInfo()->sheets_names));
	}
}


} // namespace XLS
