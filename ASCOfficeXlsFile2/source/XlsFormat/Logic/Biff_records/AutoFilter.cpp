//
#include "AutoFilter.h"

//#include <algorithm>
//#include <functional>

namespace XLS
{
//static inline std::wstring &ltrim(std::wstring &s) 
//{
//	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(isspace))));
//	return s;
//}
//
//// trim from end
//static inline std::wstring &rtrim(std::wstring &s) 
//{
//	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base(), s.end());
//	return s;
//}

// trim from both ends
static inline void/*std::wstring &*/trim(std::wstring &s) 
{
	int new_size = s.length();
	for (int i = new_size - 1; i >=0; i--)
	{
		if (s[i] != 0) break;
		else new_size--;
	}
	if (new_size < s.length())
		s.erase(new_size);
	//return ltrim(rtrim(s));
}

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
	m_bAutoFilter12 = false;

	unsigned short flags;
	record >> iEntry >> flags;
	
	wJoin		= static_cast<unsigned char>(GETBITS(flags, 0, 1));
	
	fSimple1	= GETBIT(flags, 2);
	fSimple2	= GETBIT(flags, 3);
	fTopN		= GETBIT(flags, 4);
	fTop		= GETBIT(flags, 5); //top(1) or bottom(0)
	fPercent	= GETBIT(flags, 6);
	
	wTopN		= static_cast<unsigned short>(GETBITS(flags, 7, 15));

	unsigned short	_iEntry		= iEntry;
	unsigned char	_wJoin		= wJoin;			
	unsigned short	_wTopN		= wTopN;			
	
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
		XLUnicodeStringNoCch s;
		s.setSize(doper1.vtValueStr.cch);
		record >> s;	

		str1 = s.value();
		trim(str1);
	}

	if (doper2.vt == BIFF_BYTE(0x06))
	{
		XLUnicodeStringNoCch s;
		s.setSize(doper2.vtValueStr.cch);
		record >> s;
		
		str2 = s.value();
		trim(str2);
	}	

	if (record.getRdPtr()  < record.getDataSize())
	{
		int sz = record.getDataSize() - record.getRdPtr();
		char *dd = new char [sz];
		
		memcpy(dd, record.getCurData<char>(), sz);

		record.skipNunBytes(sz);
		delete []dd;
	}
}

} // namespace XLS

