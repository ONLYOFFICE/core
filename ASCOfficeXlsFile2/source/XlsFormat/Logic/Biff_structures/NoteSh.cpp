
#include "NoteSh.h"
#include "CellRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{


NoteSh::NoteSh()
{
	x_ = y_ = 0;
}


NoteSh::NoteSh(CFRecord& record)
{
	x_ = y_ = 0;
	load(record);
}


BiffStructurePtr NoteSh::clone()
{
	return BiffStructurePtr(new NoteSh(*this));
}


void NoteSh::store(CFRecord& record)
{
#pragma message("####################### NoteSh record has no BiffStructure::store() implemented")
	Log::error(" Error!!! NoteSh record has no BiffStructure::store() implemented.");
	//record << something;
}


void NoteSh::load(CFRecord& record)
{
	XLS::GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();

	unsigned short flags;
	record >> row >> col >> flags;
	
	fShow		= GETBIT(flags, 1);
	fRwHidden	= GETBIT(flags, 7);
	fColHidden	= GETBIT(flags, 8);

	record >> idObj >> stAuthor;
	record.skipNunBytes(1); // unused

//-----------------------------------------------------------------------
	ref_ = CellRef(row, col, true, true).toString();

	double kfCol	= 17640 / 256.;
	double kfRow	= ( 360000 * 2.54 / 72) / 256. ;

	for (int i = 0 ; i < col; i++)
	{
		if (global_info->customColumnsWidth.find(i) != global_info->customColumnsWidth.end())
			x_ +=  256 * kfCol * global_info->customColumnsWidth[i];	
		else 
			x_ +=  256 * kfCol * global_info->defaultColumnWidth;
	}

	for (int i = 0 ; i < row; i++)
	{
		if (global_info->customRowsHeight.find(i) != global_info->customRowsHeight.end())
		{
			y_ += 256 * kfRow * global_info->customRowsHeight[i];	
		}
		else 
			y_ += 256 * kfRow * global_info->defaultRowHeight;	
	}
}


} // namespace XLS

