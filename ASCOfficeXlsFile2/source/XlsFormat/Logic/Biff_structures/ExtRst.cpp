
#include "ExtRst.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr ExtRst::clone()
{
	return BiffStructurePtr(new ExtRst(*this));
}

void ExtRst::store(CFRecord& record)
{
//	unsigned short reserved = 1;
//	record  << reserved << cb << phs << rphssub << rgphruns;
}

ExtRst::ExtRst(std::list<CFRecordPtr>& cont_recs) : cont_recs_(cont_recs)
{
}


void ExtRst::load(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
	record >> cb;
	size_t data_start = record.getRdPtr();
	record >> phs;

	rphssub.sz = cb;

	record >> rphssub;
	for(unsigned short i = 0; i < rphssub.getRunsNumber(); ++i)
	{
		PhRuns run;
		record >> run;
		rgphruns.push_back(run);

	}
	size_t data_end = record.getRdPtr();
	if(data_end - data_start != cb)
	{
		//ОШИБКА - нехватило Continue records - нужно найти место где именно и подзагрузить
		if ((data_end - data_start)  < cb )
			record.skipNunBytes( cb - (data_end - data_start)); // trash for unknown reason
		else
			record.RollRdPtrBack((data_end - data_start) - cb);
	}

}


const size_t ExtRst::getSize() const
{
	return 2/*reserved*/ + sizeof(unsigned short) /*cb*/ + sizeof(unsigned short) * 2/*phs*/ + rphssub.getSize() + (rgphruns.size() ? rgphruns.size() * sizeof(short) * 3 : 0);
}


} // namespace XLS
