
#include "RPHSSub.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr RPHSSub::clone()
{
	return BiffStructurePtr(new RPHSSub(*this));
}


void RPHSSub::store(CFRecord& record)
{
	unsigned short cch = st.getSize();
	record << crun << cch << st;
}


void RPHSSub::load(CFRecord& record)
{
	unsigned short cch;
	record >> crun >> cch;
	
	st.setSize(cch);

	record >> st;
}


const size_t RPHSSub::getSize() const
{
	return sizeof(unsigned short)/*crun*/ + sizeof(unsigned short)/*cch*/ + st.getStructSizeWouldWritten();
}

const unsigned short RPHSSub::getRunsNumber() const
{
	return crun;
}


} // namespace XLS

