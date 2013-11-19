#include "stdafx.h"
#include "Selection.h"

namespace XLS
{;

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
	rwAct = static_cast<WORD>(activeCellRef.getRow());
	colAct = static_cast<WORD>(activeCellRef.getColumn());
	
	record << pnn << rwAct << colAct << irefAct;
	
	std::vector<CellRangeRef> refs;
	AUX::str2refs(static_cast<wchar_t*>(static_cast<_bstr_t>(sqref)), refs);

	WORD cref = refs.size();
	record << cref;
	for(std::vector<CellRangeRef>::const_iterator it = refs.begin(), itEnd = refs.end(); it != itEnd ; ++it)
	{
		RefU refu(*it);
		record << refu;
	}
}


void Selection::readFields(CFRecord& record)
{
	WORD cref;
	record >> pnn >> rwAct >> colAct >> irefAct >> cref;
	activeCell = static_cast<_bstr_t>(CellRef(rwAct, colAct, true, true));
	_bstr_t sqref_str;
	for(int i = 0; i < cref; ++i)
	{
		RefU refu;
		record >> refu;
		sqref_str += _bstr_t(refu.toString(false).c_str()) + ((i == cref - 1) ? L"" : L" ");
	}
	sqref = sqref_str;
}

} // namespace XLS

