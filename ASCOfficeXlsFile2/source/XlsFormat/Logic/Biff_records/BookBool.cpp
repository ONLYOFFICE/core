
#include "BookBool.h"
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{;

BookBool::BookBool()
{
}


BookBool::~BookBool()
{
}


BaseObjectPtr BookBool::clone()
{
	return BaseObjectPtr(new BookBool(*this));
}


void BookBool::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fNoSaveSup);
	SETBIT(flags, 2, fHasEnvelope);
	SETBIT(flags, 3, fEnvelopeVisible);
	SETBIT(flags, 4, fEnvelopeInitDone);
	
	unsigned char grUpdateLinks_num;

	if(grUpdateLinks == std::wstring (L"userSet"))
	{
		grUpdateLinks_num = 0;
	}
	else if(grUpdateLinks == std::wstring (L"never"))
	{
		grUpdateLinks_num = 1;
	}
	else if(grUpdateLinks == std::wstring (L"always"))
	{
		grUpdateLinks_num = 2;
	}
	else
	{
		throw;// EXCEPT::LE::AttributeDataWrong(L"grUpdateLinks", record.getTypeString().c_str(), grUpdateLinks);
	}
	SETBITS(flags, 5, 6, grUpdateLinks_num);
	SETBIT(flags, 8, fHideBorderUnselLists);
	record << flags;
}


void BookBool::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	fNoSaveSup = GETBIT(flags, 0);
	fHasEnvelope = GETBIT(flags, 2);
	fEnvelopeVisible = GETBIT(flags, 3);
	fEnvelopeInitDone = GETBIT(flags, 4);
	unsigned char grUpdateLinks_num = GETBITS(flags, 5, 6);
	switch(grUpdateLinks_num)
	{
		case 0:
			grUpdateLinks = std::wstring (L"userSet");
			break;
		case 1:
			grUpdateLinks = std::wstring (L"never");
			break;
		case 2:
			grUpdateLinks = std::wstring (L"always");
			break;
		default:
			throw;// EXCEPT::RT::WrongBiffRecord("Unsupported value of grUpdateLinks.", record.getTypeString());

	}
	fHideBorderUnselLists = GETBIT(flags, 8);
}

} // namespace XLS

