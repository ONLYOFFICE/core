
#include "Selection.h"

namespace XLS
{

Selection::Selection()
{
}


Selection::~Selection()
{
}


BaseObjectPtr Selection::clone()
{
	return BaseObjectPtr(new Selection(*this));
}


void Selection::writeFields(CFRecord& record)
{
	CellRef activeCellRef(activeCell);
	rwAct = static_cast<unsigned short>(activeCellRef.getRow());
	colAct = static_cast<unsigned short>(activeCellRef.getColumn());
	
	record << pnn << rwAct << colAct << irefAct;
	
	std::vector<CellRangeRef> refs;
	AUX::str2refs(sqref, refs);

	unsigned short cref = refs.size();
	record << cref;
	for(std::vector<CellRangeRef>::const_iterator it = refs.begin(), itEnd = refs.end(); it != itEnd ; ++it)
	{
		RefU refu(*it);
		record << refu;
	}
}


void Selection::readFields(CFRecord& record)
{
	unsigned short cref;
	record >> pnn >> rwAct >> colAct >> irefAct >> cref;
	activeCell = static_cast<std::wstring >(CellRef(rwAct, colAct, true, true));
	std::wstring  sqref_str;
	for(int i = 0; i < cref; ++i)
	{
		RefU refu;
		record >> refu;
		sqref_str += std::wstring (refu.toString(false).c_str()) + ((i == cref - 1) ? L"" : L" ");
	}
	sqref = sqref_str;
}

} // namespace XLS

