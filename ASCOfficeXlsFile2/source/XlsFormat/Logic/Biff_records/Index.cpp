
#include "Index.h"

namespace XLS
{;

Index::Index()
{
}


Index::~Index()
{
}


BaseObjectPtr Index::clone()
{
	return BaseObjectPtr(new Index(*this));
}


void Index::writeFields(CFRecord& record)
{
	record.reserveNunBytes(4); // reserved
	record << rwMic << rwMac;
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned int)/*ibXF*/);

	for(unsigned int i = 0; i < num_pointers ; ++i)
	{
		record.registerDelayedDataReceiver(NULL, sizeof(unsigned int)/*FilePointer*/);
	}
}


void Index::readFields(CFRecord& record)
{
	record.skipNunBytes(4); // reserved
	record >> rwMic >> rwMac >> ibXF;
	while(!record.isEOF())
	{
		FilePointerPtr element(new FilePointer);
		record >> *element;
		rgibRw.push_back(element);
	}
}

} // namespace XLS

