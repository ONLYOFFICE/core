
#include "CRN.h"
#include <Logic/Biff_structures/SerAr.h>

namespace XLS
{

CRN::CRN()
{
}


CRN::~CRN()
{
}


BaseObjectPtr CRN::clone()
{
	return BaseObjectPtr(new CRN(*this));
}


void CRN::writeFields(CFRecord& record)
{

}


void CRN::readFields(CFRecord& record)
{
	record >> colLast >> colFirst >> row;
	for(int i = 0; i < colLast - colFirst + 1; ++i)
	{
		unsigned char rec_type;
		record >> rec_type;
		SerArPtr ser(SerAr::createSerAr(rec_type));
		record >> *ser;
		crnOper.push_back(ser);
	}
}

} // namespace XLS

