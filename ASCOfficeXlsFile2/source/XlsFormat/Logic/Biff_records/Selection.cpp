
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

int Selection::serialize(std::wostream & stream)
{	
	if (pnn == (unsigned char)PaneType::REVTPNNTOPLEFT &&
		activeCell == L"A1"		&&
		irefAct == (_INT16)0		&&
		sqref == L"A1") return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"selection")
		{
			switch(pnn)
			{
			case PaneType::REVTPNNBOTRIGHT:	CP_XML_ATTR(L"pane", L"bottomRight");	break;
			case PaneType::REVTPNNTOPRIGHT:	CP_XML_ATTR(L"pane", L"topRight");		break;
			case PaneType::REVTPNNBOTLEFT:	CP_XML_ATTR(L"pane", L"bottomLeft");	break;
			//	default:					CP_XML_ATTR(L"pane", L"topLeft"); 
			}

			if (*activeCell.value() != L"A1") 
				CP_XML_ATTR(L"activeCell", *activeCell.value());

			if (irefAct != 0) 
				CP_XML_ATTR(L"activeCellId", irefAct);

			if (*sqref.value() != L"A1") 
				CP_XML_ATTR(L"sqref", *sqref.value());
		}
	}
	return 0;
}

} // namespace XLS

