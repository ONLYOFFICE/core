
#include "ExtPtgAreaErr3D.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr ExtPtgAreaErr3D::clone()
{
	return BiffStructurePtr(new ExtPtgAreaErr3D(*this));
}


void ExtPtgAreaErr3D::store(CFRecord& record)
{
#pragma message("####################### ExtPtgAreaErr3D record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExtPtgAreaErr3D record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExtPtgAreaErr3D::load(CFRecord& record)
{
	record >> iTabs;
	record.skipNunBytes(8); // unused
}


void ExtPtgAreaErr3D::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
#pragma message("####################### ExtPtgAreaErr3D record is not implemented")
	Log::info("ExtPtgAreaErr3D record is not implemented.");
	ptg_stack.push(L"#REF!");
}




} // namespace XLS

