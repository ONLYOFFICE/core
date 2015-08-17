
#include "BiffStructure.h"

namespace XLS
{

// this function will never be called ( look at operator>>(CFRecord& record, T& val))
void BiffStructure_NoVtbl::store(CFRecord& record)
{
#pragma message("####################### BiffStructure_NoVtbl record has no BiffStructure::store() implemented")
	Log::error(" Error!!! BiffStructure_NoVtbl record has no BiffStructure::store() implemented.");
	//record << something;
}


// this function will never be called ( look at operator>>(CFRecord& record, T& val))
void BiffStructure_NoVtbl::load(CFRecord& record)
{
	throw;// EXCEPT::LE::HowCouldItHappen("Unexpected behaviour.", __FUNCTION__);
}


bool DiffBiff(BiffStructure_NoVtbl& val)
{
	return true;
}


bool DiffBiff(BiffStructure& val)
{
	return false;
}



}// namespace XLS
