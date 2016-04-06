
#include "Row.h"

namespace XLS
{

Row::Row()
{
}


Row::~Row()
{
}


BaseObjectPtr Row::clone()
{
	return BaseObjectPtr(new Row(*this));
}


void Row::writeFields(CFRecord& record)
{
	if(1 == in_block_position)
	{
		record.registerDelayedFilePointerSource(rt_DBCell); // For DBCell::dbRtrw 
	}

	record << rw << colMic << colMac << miyRw;
	record.reserveNunBytes(4); // reserved / unused
	unsigned short flags = 0x0100;
	SETBITS(flags, 0, 2, iOutLevel);
	SETBIT(flags, 4, fCollapsed);
	SETBIT(flags, 5, fDyZero);
	SETBIT(flags, 6, fUnsynced);
	SETBIT(flags, 7, fGhostDirty);
	record << flags;

	flags = 0;
	SETBITS(flags, 0, 11, ixfe_val);
	SETBIT(flags, 12, fExAsc);
	SETBIT(flags, 13, fExDes);
	SETBIT(flags, 14, fPhonetic);
	record << flags;

	if(1 == in_block_position)
	{
		record.registerDelayedFilePointerAndOffsetSource(record.getDataSize() + sizeof(unsigned short)/*size_short*/ + sizeof(CFRecordType::TypeId), rt_Blank); // For CELL container. All Cell containers will listen for rt_Blank
	}
}


void Row::readFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();
	record >> rw >> colMic >> colMac >> miyRw;

	record.skipNunBytes(4); // reserved / unused

	unsigned short flags;
	record >> flags;
	
	iOutLevel	= GETBITS(flags, 0, 2);
	fCollapsed	= GETBIT(flags, 4);
	fDyZero		= GETBIT(flags, 5);
	fUnsynced	= GETBIT(flags, 6);
	fGhostDirty = GETBIT(flags, 7);

	record >> flags;

	ixfe_val	= GETBITS(flags, 0, 11);
	fExAsc		= GETBIT(flags, 12);
	fExDes		= GETBIT(flags, 13);
	fPhonetic	= GETBIT(flags, 14);
}

int Row::serialize(std::wostream &stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"row")
		{	
			CP_XML_ATTR(L"r", rw + 1);

			bool xf_set = true;
			if (fGhostDirty == false) xf_set = false;
			
			if (ixfe_val && xf_set)
			{
				int xf = ixfe_val > global_info_->cellStyleXfs_count ? ixfe_val - global_info_->cellStyleXfs_count : ixfe_val;
				
				if (xf < global_info_->cellXfs_count)
				{
					CP_XML_ATTR(L"s", xf);
					CP_XML_ATTR(L"customFormat", true);
				}
			}
			if (miyRw > 0/* && std::abs(miyRw/20. - sheet_info.defaultRowHeight) > 0.01*/)
			{
				CP_XML_ATTR(L"ht", miyRw / 20.);
				CP_XML_ATTR(L"customHeight", true);
			}
			if (iOutLevel > 0)
			{
				CP_XML_ATTR(L"outlineLevel", iOutLevel);
			}
			if (fCollapsed)
			{
				CP_XML_ATTR(L"collapsed", fCollapsed);
			}
			if (fExAsc)
			{
				CP_XML_ATTR(L"thickTop", true);
			}
			if (fExDes)
			{
				CP_XML_ATTR(L"thickBot", true);
			}
			if (fDyZero)
			{
				CP_XML_ATTR(L"hidden", true);
			}
		}
	}

	return 0;
}

} // namespace XLS

