
#include "CellOffsetResender.h"
#include <Binary/CFRecord.h>
#include <Binary/CFStream.h>

namespace XLS
{;


BiffStructurePtr CellOffsetResender::clone()
{
	return BiffStructurePtr(new CellOffsetResender(*this));
}

//
// // Stubbed to avoid generating unnecessary tags
//void CellOffsetResender::toXML(BiffStructurePtr & parent)
//{
//}
//
//
// // Stubbed to make class non-abstract
//void CellOffsetResender::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//}

//
//void CellOffsetResender::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	row_starts = getStructAttribute(xml_tag, L"row_starts");
//}


void CellOffsetResender::resend(CFStream& stream, const unsigned int data_place, const unsigned int data)
{
	unsigned int CELL_start_pos = data_place - sizeof(unsigned short)/*size_short*/ - sizeof(CFRecordType::TypeId);
	CFStream::SourceItems source_items;
	CFStream::SourceItem item;
	item.is_file_ptr = false;
	item.data = CELL_start_pos - data;
	item.receiver_id = rt_DBCell;
	item.source_id = rt_Blank;
	source_items.push_back(item); // to DBCell

	item.is_file_ptr = false;
	item.data = CELL_start_pos;
	item.receiver_id = rt_Blank;
	item.source_id = rt_Blank;
	source_items.push_back(item); // To the next cell

	stream.appendDelayedItems(CFStream::ReceiverItems(), source_items);
}


void CellOffsetResender::store(CFRecord& record)
{
	if(row_starts)
	{
		record.registerDelayedDataReceiver(&resend, 0, rt_Blank);
	}
}


void CellOffsetResender::load(CFRecord& record)
{
	// do nothing
}


} // namespace XLS

