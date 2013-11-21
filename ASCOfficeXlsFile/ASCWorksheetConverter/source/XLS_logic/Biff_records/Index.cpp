#include "stdafx.h"
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
	record.reserveNBytes(4); // reserved
	record << rwMic << rwMac;
	record.registerDelayedDataReceiver(NULL, sizeof(DWORD)/*ibXF*/);

	for(DWORD i = 0; i < num_pointers ; ++i)
	{
		record.registerDelayedDataReceiver(NULL, sizeof(DWORD)/*FilePointer*/);
	}
}


void Index::readFields(CFRecord& record)
{
	record.skipNBytes(4); // reserved
	record >> rwMic >> rwMac >> ibXF;
	while(!record.isEOF())
	{
		FilePointerPtr element(new FilePointer);
		record >> *element;
		rgibRw.push_back(element);
	}
}

} // namespace XLS

