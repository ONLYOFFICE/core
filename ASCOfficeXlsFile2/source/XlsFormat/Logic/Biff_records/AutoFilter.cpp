//
#include "AutoFilter.h"

namespace XLS
{

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

	unsigned short flags;
	record >> iEntry >> flags;
	wJoin = static_cast<unsigned char>(GETBITS(flags, 0, 1));
	fSimple1 = GETBIT(flags, 2);
	fSimple2 = GETBIT(flags, 3);
	fTopN = GETBIT(flags, 4);
	fTop = GETBIT(flags, 5);
	fPercent = GETBIT(flags, 6);
	wTopN = static_cast<unsigned short>(GETBITS(flags, 7, 15));

	unsigned short _iEntry = iEntry;
	unsigned char _wJoin = wJoin;			
	unsigned char _fSimple1 = fSimple1;
	unsigned char _fSimple2 = fSimple2;
	unsigned char _fTopN = fTopN;			
	unsigned char _fTop = fTop;				
	unsigned char _fPercent = fPercent;		
	unsigned short _wTopN = wTopN;			
	
	if (fTopN != 1)
		record >> doper1;			
	if (fTopN != 1)
		record >> doper2;

	if ((doper1.vt == BIFF_BYTE(0)) && (doper1.vt == BIFF_BYTE(0)))
	{
		m_bAutoFilter12 = true;	
		return;
	}

	if (doper1.vt == BIFF_BYTE(0x06))
	{
		str1.setSize(doper1.vtValue.cch);
		record >> str1;		
	}

	if (doper2.vt == BIFF_BYTE(0x06))
	{
		str2.setSize(doper2.vtValue.cch);
		record >> str2;
	}	
}

} // namespace XLS

