
#include "ObjFmla.h"
#include <Binary/CFRecord.h>

namespace XLS
{


ObjFmla::ObjFmla(const bool is_part_of_FtPictFmla)
:	is_part_of_FtPictFmla_(is_part_of_FtPictFmla)
{
}

BiffStructurePtr ObjFmla::clone()
{
	return BiffStructurePtr(new ObjFmla(*this));
}

//
//void ObjFmla::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	fmla.toXML(own_tag);
//}
//
//
//void ObjFmla::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	fmla_found = fmla.fromXML(own_tag);
//}


void ObjFmla::load(CFRecord& record)
{
	unsigned short cbFmla;
	record >> cbFmla;
	size_t start_ptr = record.getRdPtr();

	if(0 != cbFmla)
	{
		fmla.load(record);

		if(is_part_of_FtPictFmla_ && fmla.HasPtgTbl())
		{
			record >> embedInfo;
		}
	}


	size_t data_size = record.getRdPtr() - start_ptr;
	size_t padding_size = cbFmla - data_size;
	if(0 != padding_size)
	{
		record.skipNunBytes(padding_size);
	}
}


void ObjFmla::store(CFRecord& record)
{
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cbFmla*/);
	size_t start_ptr = record.getDataSize();

	if(fmla_found)
	{
		fmla.store(record);
	}

	if(true)
	{
		record << embedInfo;
	}

	size_t data_size = record.getDataSize() - start_ptr;
	if(0 != (data_size & 1))
	{
		record.reserveNunBytes(1); // padding
		data_size++;
	}
	record.registerDelayedDataSource(data_size, rt_Obj);
}


} // namespace XLS

