
#include "Pls.h"
#include <Document/Document.h>

namespace XLS
{;

Pls::Pls(BaseObject* parent)
:	parent_(parent)
{
}


Pls::~Pls()
{
}


BaseObjectPtr Pls::clone()
{
	return BaseObjectPtr(new Pls(*this));
}


void Pls::writeFields(CFRecord& record)
{
	record.reserveNunBytes(2); // reserved

	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];
	Document* doc = Document::findDocumentByElement(parent_);

	const std::pair<char*, size_t> bin_data = doc->getBinaryData(bin_data_id);
	if(record.checkFitWriteSafe(bin_data.second))
	{
		record.storeLongData(bin_data.first, bin_data.second);
	}
	else
	{
		char* buffer = bin_data.first;
		size_t size = bin_data.second;
		while(size) 
		{
			CFRecord& current_record = recs.size() ? *recs.back() : record; // Points to the original 'record' or the last created 'Continue'
			size_t space_left = current_record.getMaxRecordSize() - current_record.getDataSize();
			size_t write_size = min(size, space_left);
			current_record.storeLongData(buffer, write_size);
			buffer += write_size;
			size -= write_size;
			current_record.commitData();

			if(size)
			{
				CFRecordPtr cont(new CFRecord(rt_Continue, record.getGlobalWorkbookInfo()));
				recs.push_back(cont);
			}
		}
	}
}


void Pls::readFields(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
	record.loadAnyData(rgb);
	bin_data_id = Document::appendBinaryData(parent_, record.getData() + 2, record.getDataSize() - 2);
	record.skipNunBytes(record.getDataSize() - record.getRdPtr()); // avoid size inconsistency warning
}

} // namespace XLS

