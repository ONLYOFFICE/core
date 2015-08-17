
#include "Table.h"

namespace XLS
{

Table::Table()
{
}


Table::~Table()
{
}


BaseObjectPtr Table::clone()
{
	return BaseObjectPtr(new Table(*this));
}


void Table::writeFields(CFRecord& record)
{
	R_RwU rwInpRw;
	Col_NegativeOne colInpRw;
	R_RwU rwInpCol;
	Col_NegativeOne colInpCol;

	record << ref_;
	unsigned short flags = 0;
	SETBIT(flags, 0, fAlwaysCalc);
	SETBIT(flags, 2, fRw);
	SETBIT(flags, 3, fTbl2);
	SETBIT(flags, 4, fDeleted1);
	SETBIT(flags, 5, fDeleted2);
	record << flags;

	if(!fDeleted1)
	{
		CellRef r1_ref(static_cast<std::wstring >(r1));
		rwInpRw = static_cast<unsigned short>(r1_ref.getRow());
		colInpRw = static_cast<short>(r1_ref.getColumn());
	}
	else
	{
		rwInpRw = 65535;
		colInpRw = -1;
	}
	record << rwInpRw << colInpRw;
	if(fTbl2 && !fDeleted1)
	{
		CellRef r2_ref(static_cast<std::wstring >(r2));
		rwInpCol = static_cast<unsigned short>(r2_ref.getRow());
		colInpCol = static_cast<short>(r2_ref.getColumn());
	}
	else
	{
		rwInpCol = 65535;
		colInpCol = -1;
	}
	record << rwInpCol << colInpCol;
}


void Table::readFields(CFRecord& record)
{
	R_RwU rwInpRw;
	Col_NegativeOne colInpRw;
	R_RwU rwInpCol;
	Col_NegativeOne colInpCol;

	record >> ref_;
	unsigned short flags;
	record >> flags;
	fAlwaysCalc = GETBIT(flags, 0);
	fRw = GETBIT(flags, 2);
	fTbl2 = GETBIT(flags, 3);
	fDeleted1 = GETBIT(flags, 4);
	fDeleted2 = GETBIT(flags, 5);

	record >> rwInpRw >> colInpRw >> rwInpCol >> colInpCol;
	r1 = static_cast<std::wstring >(CellRef(rwInpRw, colInpRw, true, true));
	if(fTbl2)
	{
		r2 = static_cast<std::wstring >(CellRef(rwInpCol, colInpCol, true, true));
	}
}

} // namespace XLS

