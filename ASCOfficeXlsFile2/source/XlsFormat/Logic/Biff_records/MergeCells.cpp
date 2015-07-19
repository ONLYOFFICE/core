#include "precompiled_xls.h"
#include "MergeCells.h"

#include <simple_xml_writer.h>

namespace XLS
{;

MergeCells::MergeCells()
{
}


MergeCells::~MergeCells()
{
}


BaseObjectPtr MergeCells::clone()
{
	return BaseObjectPtr(new MergeCells(*this));
}


void MergeCells::writeFields(CFRecord& record)
{
	record << cmcs;
	for(BiffStructurePtrVector::const_iterator it = rgref.begin(), itEnd = rgref.end(); it != itEnd; ++it)
	{
		record << **it;
	}
}


void MergeCells::readFields(CFRecord& record)
{
	record >> cmcs;
	for(int i = 0; i < cmcs; ++i)
	{
		BiffStructurePtr ref8(new Ref8);
		record >> *ref8;
		rgref.push_back(ref8);
	}
}

int MergeCells::serialize(std::wostream & stream)
{

	return 0;
}

} // namespace XLS

