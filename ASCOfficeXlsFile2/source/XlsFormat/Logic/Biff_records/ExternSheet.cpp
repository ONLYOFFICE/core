
#include "ExternSheet.h"
#include <Logic/Biff_structures/XTI.h>

namespace XLS
{

ExternSheet::ExternSheet()
{
}


ExternSheet::~ExternSheet()
{
}


BaseObjectPtr ExternSheet::clone()
{
	return BaseObjectPtr(new ExternSheet(*this));
}


void ExternSheet::writeFields(CFRecord& record)
{
	cXTI = rgXTI.size();
	record << cXTI;
	for(BiffStructurePtrVector::const_iterator it = rgXTI.begin(), itEnd = rgXTI.end(); it != itEnd; ++it)
	{
		record << **it;
	}
}


void ExternSheet::readFields(CFRecord& record)
{
	record >> cXTI;
	for(int i = 0; i < cXTI; ++i)
	{
		XTIPtr xti(new XTI);
		record >> *xti;
		rgXTI.push_back(xti);
	}
}

} // namespace XLS

