#include "stdafx.h"
#include "AutoFilter.h"

namespace XLS
{;

AutoFilter::AutoFilter()
{
}


AutoFilter::~AutoFilter()
{
}


BaseObjectPtr AutoFilter::clone()
{
	return BaseObjectPtr(new AutoFilter(*this));
}


void AutoFilter::writeFields(CFRecord& record)
{
}


void AutoFilter::readFields(CFRecord& record)
{
	//char* buffer = new char[record.getDataSize()];
	//memcpy(buffer, record.getCurData<char>(), record.getDataSize());

	m_bAutoFilter12 = false;

	WORD flags;
	record >> iEntry >> flags;
	wJoin = static_cast<BYTE>(GETBITS(flags, 0, 1));
	fSimple1 = GETBIT(flags, 2);
	fSimple2 = GETBIT(flags, 3);
	fTopN = GETBIT(flags, 4);
	fTop = GETBIT(flags, 5);
	fPercent = GETBIT(flags, 6);
	wTopN = static_cast<WORD>(GETBITS(flags, 7, 15));

	WORD _iEntry = iEntry;
	BYTE _wJoin = wJoin;			
	BYTE _fSimple1 = fSimple1;
	BYTE _fSimple2 = fSimple2;
	BYTE _fTopN = fTopN;			
	BYTE _fTop = fTop;				
	BYTE _fPercent = fPercent;		
	WORD _wTopN = wTopN;			
	
	if (fTopN != 1)
		record >> doper1;			
	if (fTopN != 1)
		record >> doper2;

	if ((doper1.vt == 0) && (doper1.vt == 0))
	{
		m_bAutoFilter12 = true;	
		return;
	}

	if (doper1.vt == 0x06)
	{
		str1.setSize(doper1.vtValue.cch);
		record >> str1;		
	}

	if (doper2.vt == 0x06)
	{
		str2.setSize(doper2.vtValue.cch);
		record >> str2;
	}	
}

} // namespace XLS

